/*
 * Title:	svnlast.c (scan for last SVN date)
 * Author:	T.E.Dickey
 * Created:	15 Oct 2009
 *
 * Modified:
 */

#define	STR_PTYPES
#include	"ptypes.h"
#include	<ctype.h>
#include	<time.h>
#include	"rcsdefs.h"

MODULE_ID("$Id: svn_last.c,v 12.2 2009/10/16 09:20:52 tom Exp $")

#define NAME_LIST "entries"

#define	typealloc(type)	(type *)doalloc((char *)0, sizeof(type))

static char *
svn_dir(char *working_directory)
{
    char temp[MAXPATHLEN];
    abspath(pathcat(temp, working_directory, ".svn"));
    return txtalloc(temp);
}

/*
 * Maintain a list of SVN working directories that we have visited.  Each
 * list item contains the data from "entries".
 */
typedef struct {
    char *filename;
    char *version;
    char *status;
    time_t timestamp;
} SVN_ENTRY;

typedef struct _svn_work {
    struct _svn_work *next;
    char *working;
    char *Repository;
    char *Root;
    int num_entries;
    SVN_ENTRY *Entries;
    time_t timestamp;
} SVN_WORK;

static SVN_WORK *my_work = 0;

static char *
admin_filename(char *dest, SVN_WORK * cache, char *leaf)
{
    return pathcat(dest, svn_dir(cache->working), leaf);
}

/*
 * Check the cache's timestamp against the Entries file.  If it is newer, drop
 * our copy of the list, and re-read it.
 */
static int
check_timestamp(SVN_WORK * cache)
{
    if (cache->num_entries) {
	char list[MAXPATHLEN];
	Stat_t sb;
	if (stat_file(admin_filename(list, cache, NAME_LIST), &sb) == 0
	    && sb.st_mtime == cache->timestamp) {
	    return 0;
	}
	cache->timestamp = sb.st_mtime;
    } else {
	cache->timestamp = 0;
    }
    return 1;
}

static time_t
string2time(char *string)
{
    long result = 0;
    int year = 0;
    int mon = 0;
    int day = 0;
    int hour = 0;
    int min = 0;
    int sec = 0;
    int microsecs;

    if (sscanf(string, "%d-%d-%dT%d:%d:%d.%dZ",
	       &year, &mon, &day,
	       &hour, &min, &sec,
	       &microsecs) == 7) {
	result = packdate(year, mon, day, hour, min, sec);
	result -= gmt_offset(result);
    }

    return result;
}

/*
 * Read the list of file-entries.  It's adapted from the CVS scheme, using
 * an entries file storing file properties.
 *
 * SVN does store the file-modification date in field 9, but doesn't use it
 * for the file-system.  Instead, it uses the checkout time in field 7.
 *
 * Like CVS, there is no file-locker (for the status field).  But it does have
 * a username who was the last committer for a given file.
 *
 * Revisions in SVN are just integers (no dots).
 */
static void
read_entries(SVN_WORK * cache)
{
    char name[MAXPATHLEN];
    char **list = 0;
    int j, k, last;

    if ((last = file2argv(admin_filename(name, cache, NAME_LIST), &list)) > 0) {
	cache->Entries = DOALLOC(0, SVN_ENTRY, last);
	for (j = 0; list[j] != 0; ++j) {
	    if (list[j][0] != '\f')
		strtrim(list[j]);
	}
	for (j = k = 0; list[j] != 0; ++j) {
	    if (list[j][0] != '\f')
		continue;
	    if ((j + 12) >= last)
		break;
	    if (strcmp(list[j + 2], "file"))
		continue;
	    cache->Entries[k].filename = txtalloc(list[j + 1]);
	    cache->Entries[k].version = txtalloc(list[j + 10]);
	    cache->Entries[k].status = txtalloc(list[j + 11]);
	    cache->Entries[k].timestamp = string2time(list[j + 7]);
	    ++k;
	}
	cache->num_entries = k;
	vecfree(list);
    }
}

static int
read_from_cache(SVN_WORK * cache,
		char *leaf,
		char **vers_,
		time_t * date_,
		char **lock_)
{
    int n;
    SVN_ENTRY *my_list = cache->Entries;

    for (n = 0; n < cache->num_entries; ++n) {
	if (!strcmp(my_list[n].filename, leaf)) {
	    *vers_ = txtalloc(my_list[n].version);
	    *lock_ = txtalloc(my_list[n].status);
	    *date_ = my_list[n].timestamp;
	    return TRUE;
	}
    }
    return FALSE;
}

static void
trySVN(char *path,
       char **vers_,
       time_t * date_,
       char **lock_)
{
    char working[MAXPATHLEN];
    char *leaf = fleaf(path);
    char *s;
    SVN_WORK *cache;

    strcpy(working, path);
    if ((s = fleaf_delim(working)) != 0)
	*s = EOS;
    /*
     * Look for cached data for this working directory.
     */
    for (cache = my_work; cache != 0; cache = cache->next) {
	if (!strcmp(cache->working, working)) {
	    if (check_timestamp(cache)) {
		break;
	    }
	    if (read_from_cache(cache, leaf, vers_, date_, lock_))
		return;
	}
    }

    /*
     * If we did not find it in the cache list, or our cache is obsolete,
     * look for new information.
     */
    if (cache == 0) {
	cache = typealloc(SVN_WORK);
	cache->next = my_work;
	my_work = cache;

	cache->timestamp = 0;
	cache->working = txtalloc(working);
	cache->num_entries = 0;
	cache->Entries = 0;
	cache->Repository = 0;
	cache->Root = 0;
	read_entries(cache);
	(void) check_timestamp(cache);
    } else {
	read_entries(cache);
    }
    (void) read_from_cache(cache, leaf, vers_, date_, lock_);
}

void
svnlast(char *working,		/* working directory (absolute) */
	char *path,		/* pathname to check (may be relative) */
	char **vers_,
	time_t * date_,
	char **lock_)
{
    char temp[MAXPATHLEN];

    abspath(pathcat(temp, working, path));
    trySVN(temp, vers_, date_, lock_);
}