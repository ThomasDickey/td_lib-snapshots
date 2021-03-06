/* $Id: cmv_defs.h,v 12.9 2010/07/05 10:59:41 tom Exp $ */

#ifndef	CMV_DEFS_H
#define	CMV_DEFS_H

	/*
	 * The CmVision vault-tree contains three visible types of files:
	 *
	 *	s-curr
	 *		contains a list of directory leaf names in internal
	 *		form, two semicolons (?) and the external leaf name.
	 *
	 *		Example:
	 *			d-1385;;tools
	 *	r-curr
	 *		contains a list of file leaf names in internal format,
	 *		the string ";/;" (?), the external leaf name, ";" and
	 *		the current release version.
	 *
	 *		Example
	 *			f-3001;/;jb_error.c;4.1
	 *	f-xxxx
	 *		an SCCS (or RCS?) file containing the actual delta
	 *		history.
	 *
	 */

	/* cmv_dir.c ------------------------------------------------- */
	char *	cmv_dir(
			const char *	working_dir,
			const char *	pathname
			)
			;

	char *	cmv_file(
			const char *	working_dir,
			const char *	pathname
			)
			;

	void	get_cmv_lock(
			const char *	working_dir,
			const char *	pathname,
			const char **	lockedby,
			const char **	revision,
			time_t *	t_
			)
			;

	void	purge_cmv_dir(
			const char *	working_dir,
			const char *	pathname
			)
			;

	/* cmv_dbug.c ------------------------------------------------- */
	int	cmv_debug(void)
			;

	/* cmv_last.c ------------------------------------------------- */
	void	cmv_last(
			const char *	wd,
			const char *	n,
			const char **	v_,
			time_t *	t_,
			const char **	l_
			)
			;

	/* lastrev.c -------------------------------------------------- */
	void	lastrev(
			const char *	wd,
			const char *	n,
			const char **	v_,
			time_t *	t_,
			const char **	l_
			)
			;

#endif	/* CMV_DEFS_H */
