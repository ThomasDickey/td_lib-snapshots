/* $Id: sccsdefs.h,v 12.12 2010/07/03 15:47:11 tom Exp $ */

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
			char *	wd,
			char *	n,
			char **	v_,
			time_t *	t_,
			char **	l_
			)
			;

	/* sccsname.c ------------------------------------------------- */
	char *	sccs2name(
			char *	name,
			int	full
			)
			;

	char *	name2sccs(
			char *	name,
			int	full
			)
			;

	/* sccspath.c ------------------------------------------------- */
	char *	sccspath(
			char *	name
			)
			;

#endif	/* SCCSDEFS_H */
