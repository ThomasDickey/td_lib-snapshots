/*
 * Title:	cvslast.c (scan for last CVS date)
 * Author:	T.E.Dickey
 * Created:	15 Feb 1988
 *
 * Modified:
 *		08 Mar 2004, fix a couple of logic errors that made this
 *			     read the cache an extra time and omit data for
 *			     the first two items requested per directory.
 *		07 Mar 2004, remove K&R support, indent'd.
 *		26 Apr 2003, finally got around to finishing...
 */

#define	STR_PTYPES
#include	"ptypes.h"
#include	<ctype.h>
#include	<time.h>
#include	"rcsdefs.h"

MODULE_ID("$Id: cvslast.c,v 12.6 2010/07/03 15:42:55 tom Exp $")

#define NAME_LIST "Entries"
#define NAME_ARCH "Repository"
#define NAME_ROOT "Root"

#define	typealloc(type)	(type *)doalloc((char *)0, sizeof(type))

static char *
cvs_dir(char *working_directory)
{
    char temp[MAXPATHLEN];
    abspath(pathcat(temp, working_directory, "CVS"));
    return txtalloc(temp);
}

/*
 * Maintain a list of CVS working directories that we have visited.  Each
 * list item contains the data from Entries, Repository and Root.
 */
typedef struct {
    char *filename;
    char *version;
    char *status;
    time_t timestamp;
} CVS_ENTRY;

typedef struct _cvs_work {
    struct _cvs_work *next;
    char *working;
    char *Repository;
    char *Root;
    int num_entries;
    CVS_ENTRY *Entries;
    time_t timestamp;
} CVS_WORK;

static CVS_WORK *my_work = 0;

static char *
admin_filename(char *dest, CVS_WORK * cache, const char *leaf)
{
    return pathcat(dest, cvs_dir(cache->working), leaf);
}

/*
 * Check the cache's timestamp against the Entries file.  If it is newer, drop
 * our copy of the list, and re-read it.
 */
static int
check_timestamp(CVS_WORK * cache)
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

/*
 * FIXME: CVS uses a generated timestamp; we do not need generality, do we?
 */
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
    char day_of_week[80];
    char month_of_year[80];
    static const char *months[] =
    {
	"Jan", "Feb", "Mar", "Apr", "May", "Jun",
	"Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
    };

    if (sscanf(string, "%s %s %d %d:%d:%d %d",
	       day_of_week,
	       month_of_year,
	       &day,
	       &hour, &min, &sec,
	       &year) == 7) {
	unsigned n;
	for (n = 0; n < SIZEOF(months); ++n) {
	    if (!strcmp(months[n], month_of_year)) {
		mon = (int) (n + 1);
		break;
	    }
	}
    }

    result = packdate(year, mon, day, hour, min, sec);
    return result - gmt_offset(result);
}

/*
 * Would use strtok(), but must work around empty fields.
 */
static char *
parse_field(char **s)
{
    int skip = strcspn(*s, "/");
    char *result = 0;

    if (skip != 0) {
	result = *s;
    }
    *s += skip;
    if (**s == '/')
	*s += 1;
    return result;
}

static char *
copy_field(char *src)
{
    char *result = 0;

    if (src != 0 && *src != '/' && *src != EOS) {
	char *save = strchr(src, '/');

	if (save != 0)
	    *save = '\0';
	result = txtalloc(src);
	if (save != 0)
	    *save = '/';
    } else {
	result = txtalloc("?");
    }
    return result;
}

/*
 * Read the list of file-entries
 */
static void
read_entries(CVS_WORK * cache)
{
    char name[MAXPATHLEN];
    char **list = 0;
    int j, k;

    if ((k = file2argv(admin_filename(name, cache, NAME_LIST), &list)) > 0) {
	cache->Entries = DOALLOC(0, CVS_ENTRY, (size_t) k);
	for (j = k = 0; list[j] != 0; ++j) {
	    char *s = list[j];
	    char *t;

	    cache->Entries[j].filename = 0;
	    cache->Entries[j].version = 0;
	    cache->Entries[j].status = 0;
	    cache->Entries[j].timestamp = 0;

	    if (s[0] == '/' && s[1] != '/') {
		++s;
		cache->Entries[k].filename = copy_field(parse_field(&s));
		cache->Entries[k].version = copy_field(parse_field(&s));
		if ((t = parse_field(&s)) != 0)
		    cache->Entries[k].timestamp = string2time(t);
		cache->Entries[k].status = copy_field(parse_field(&s));
		++k;
	    }
	}
	cache->num_entries = k;
	vecfree(list);
    }
}

static int
read_from_cache(CVS_WORK * cache,
		char *leaf,
		char **vers_,
		time_t * date_,
		char **lock_)
{
    int n;
    CVS_ENTRY *my_list = cache->Entries;

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
tryCVS(char *path,
       char **vers_,
       time_t * date_,
       char **lock_)
{
    char working[MAXPATHLEN];
    char *leaf = fleaf(path);
    char *s;
    CVS_WORK *cache;

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
	cache = typealloc(CVS_WORK);
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
cvslast(char *working,		/* working directory (absolute) */
	char *path,		/* pathname to check (may be relative) */
	char **vers_,
	time_t * date_,
	char **lock_)
{
    char temp[MAXPATHLEN];

    abspath(pathcat(temp, working, path));
    tryCVS(temp, vers_, date_, lock_);
}
