/* $Id: td_sheet.h,v 11.0 1992/02/04 09:24:27 ste_cm Rel $ */

/*
 * Functions for manipulating spreadsheet text which has been loaded into
 * memory with 'file2argv()'
 */
#ifndef	 _SPREADSHEET_H_
#define	 _SPREADSHEET_H_

#include "ptypes.h"

		/* ***(field_of.c)*** */
extern	void	field_uses_quotes(
		_ar1(int,	flag)
			)
		_dcl(int,	flag)
		_nul

extern	char *	get_field_of(
		_arx(char *,	list)
		_arx(int,	col)
		_arx(char *,	buffer)
		_ar1(char *,	dftval)
			)
		_dcl(char *,	list)
		_dcl(int,	col)
		_dcl(char *,	buffer)
		_dcl(char *,	dftval)
		_ret

extern	char *	set_field_of(
		_arx(char *,	list)
		_arx(int,	col)
		_ar1(char *,	buffer)
			)
		_dcl(char *,	list)
		_dcl(int,	col)
		_dcl(char *,	buffer)
		_ret

		/* ***(ss_field.c)*** */
extern	char *	get_ss_field(
		_arx(char **,	vec)
		_arx(int,	row)
		_arx(int,	col)
		_arx(char *,	buffer)
		_ar1(char *,	dftval)
			)
		_dcl(char **,	vec)
		_dcl(int,	row)
		_dcl(int,	col)
		_dcl(char *,	buffer)
		_dcl(char *,	dftval)
		_ret

extern	char *	set_ss_field(
		_arx(char **,	vec)
		_arx(int,	row)
		_arx(int,	col)
		_ar1(char *,	buffer)
			)
		_dcl(char **,	vec)
		_dcl(int,	row)
		_dcl(int,	col)
		_dcl(char *,	buffer)
		_ret

		/* ***(count_ss.c)*** */
extern	int	count_ss_cols(
		_ar1(char **,	vec)
			)
		_dcl(char **,	vec)
		_ret

extern	int	count_ss_rows(
		_ar1(char **,	vec)
			)
		_dcl(char **,	vec)
		_ret

#endif/* _SPREADSHEET_H_ */
