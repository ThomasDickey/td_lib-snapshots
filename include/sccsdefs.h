/* $Id: sccsdefs.h,v 12.14 2010/07/04 16:45:52 tom Exp $ */

#ifndef	SCCSDEFS_H
#define	SCCSDEFS_H

#define	SCCS_DIR	"SCCS"
#define	SCCS_PREFIX	"s."

#if !defined(HAVE_LONG_FILE_NAMES)
#define S_FILES_14 1
#endif

	/* sccs_dir.c ------------------------------------------------- */
	char *	sccs_dir(
			const char * working_dir,
			const char * pathname
			)
			;

	/* sccsdbug.c ------------------------------------------------- */
	int	sccs_debug(void)
			;

	/* sccslast.c ------------------------------------------------- */
	int	sccsyear(
			char *	yy)
			;

	void	sccslast(
			const char *	wd,
			const char *	n,
			const char **	v_,
			time_t *	t_,
			const char **	l_
			)
			;

	/* sccsname.c ------------------------------------------------- */
	char *	sccs2name(
			const char *	name,
			int		full
			)
			;

	char *	name2sccs(
			const char *	name,
			int		full
			)
			;

	/* sccspath.c ------------------------------------------------- */
	char *	sccspath(
			const char *	name
			)
			;

#endif	/* SCCSDEFS_H */
