/* $Id: td_sheet.h,v 12.2 2004/03/07 21:45:41 tom Exp $ */

/*
 * Functions for manipulating spreadsheet text which has been loaded into
 * memory with 'file2argv()'
 */
#ifndef	 _SPREADSHEET_H_
#define	 _SPREADSHEET_H_

#include <ptypes.h>
#include <dyn_str.h>

		/* ***(field_of.c)*** */
extern	void	field_uses_quotes(
		int	flag
			)
		;

extern	char *	get_field_of(
		char *	list,
		int	col,
		char *	dftval,
		DYN **	result
			)
		;

extern	char *	set_field_of(
		char *	list,
		int	col,
		char *	buffer
			)
		;

		/* ***(ss_field.c)*** */
extern	char *	get_ss_field(
		char **	vec,
		int	row,
		int	col,
		char *	dftval,
		DYN **	result
			)
		;

extern	char *	set_ss_field(
		char **	vec,
		int	row,
		int	col,
		char *	buffer
			)
		;

		/* ***(count_ss.c)*** */
extern	int	count_ss_cols(
		char **	vec
			)
		;

extern	int	count_ss_rows(
		char **	vec
			)
		;

#endif/* _SPREADSHEET_H_ */
