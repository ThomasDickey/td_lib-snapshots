/* @(#)rcsdefs.h	1.8 89/03/08 13:02:47 */

/*
 * State information for scanning RCS-file header
 */
#define	S_HEAD		0	
#define	S_ACCESS	1
#define	S_SYMBOLS	2
#define	S_LOCKS		3
#define	S_COMMENT	4
#define	S_STRICT	5

#define	S_VERS		99	/* <version_string>		*/
#define	S_DATE		6
#define	S_AUTHOR	7
#define	S_STATE		8
#define	S_BRANCHES	9
#define	S_NEXT		10

#define	S_DESC		11
#define	S_LOG		12
#define	S_TEXT		13

#define	S_FAIL		999

#define	RCS_DIR		"RCS"
#define	RCS_SUFFIX	",v"

#define	VCS_FILE	"RCS,v"	/* whole-directory permissions and baseline */
#define	VCS_LOCK	"RCS,,"

#define	FMT_DATE	"%02d.%02d.%02d.%02d.%02d.%02d"

#define NULL_FUNC (int (*)())0

/*
 * If the environment variable RCS_DEBUG is defined, we activate additional
 * tracing (e.g., in 'rcsedit.c')
 */
#define	RCS_DEBUG	(getenv("RCS_DEBUG") != 0)

/*
 * Note: since 'rcsload()' uses 'txtalloc()' to allocate the '.revision' and
 * '.parent' strings; we can do equality-comparison simply by comparing the
 * pointers.
 */
#ifdef	S_IFMT
typedef	struct	{
		char	*revision;
		char	*parent;
		time_t	tstamp;		/* checkin time		*/
		int	num_lines;	/* total lines in file	*/
		int	num_added;	/* lines added		*/
		int	num_deleted;	/* lines deleted	*/
	} RCSTREE;
extern	RCSTREE *rcsload();
#endif	S_IFMT

/*
 * Useful external-definitions
 */
extern	char *	getenv();
extern	char *	name2rcs();
extern	char *	rcs2name();
extern	char *	rcs_dir();
extern		rcsedit ();
extern		rcsclose();
extern	int	rcskeys();
extern		rcslast();
extern	char *	rcslocks();
extern	int	rcsopen();
extern	char *	rcsparse_num();
extern	char *	rcsparse_id();
extern	char *	rcsparse_str();
extern	char *	rcspath();
extern	char *	rcsread();
extern	char *	rcstemp();
extern		rcsunload();
