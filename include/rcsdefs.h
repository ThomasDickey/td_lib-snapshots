/* @(#)rcsdefs.h	1.2 88/08/15 06:54:12 */

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
#define	S_FAIL		999

#ifndef	CI_PATH
#define	CI_PATH		"ci"
#endif	CI_PATH

#ifndef	CO_PATH
#define	CO_PATH		"co"
#endif	CO_PATH

#define	RCS_DIR		"RCS"
#define	RCS_SUFFIX	",v"

#define	FMT_DATE	"%02d.%02d.%02d.%02d.%02d.%02d"
