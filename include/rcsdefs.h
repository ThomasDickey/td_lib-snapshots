/* @(#)rcsdefs.h	1.6 88/09/09 09:36:59 */

/*
 * State information for scanning RCS-file header
 */
#define	S_HEAD		0	
#define	S_ACCESS	1
#define	S_SYMBOLS	2
#define	S_LOCKS		3
#define	S_COMMENT	4

#define	S_VERS		99	/* <version_string>		*/
#define	S_DATE		5
#define	S_AUTHOR	6
#define	S_STATE		7
#define	S_BRANCHES	8
#define	S_NEXT		9

#define	S_DESC		10
#define	S_LOG		11
#define	S_TEXT		12

#define	S_FAIL		999

#define	RCS_DIR		"RCS"
#define	RCS_SUFFIX	",v"

#define	FMT_DATE	"%02d.%02d.%02d.%02d.%02d.%02d"

#define NULL_FUNC (int (*)())0

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
