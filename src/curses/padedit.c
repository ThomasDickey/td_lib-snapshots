#ifndef	lint
static	char	sccs_id[] = "@(#)padedit.c	1.1 87/12/14 10:51:03";
#endif	lint

/*
 * Title:	padedit.c (pad-edit)
 * Author:	T.E.Dickey
 * Created:	14 Dec 1987
 * Modified:
 *
 * Function:	Open an edit-pad on the specified pathname.  If the pad
 *		is not read-only, suspend the calling process until the
 *		pad is closed.
 *
 * Notes:	Unlike a normal pad, we cannot toggle the RO-command.
 *		This is probably due to an APOLLO bug.
 */

#include	"/sys/ins/base.ins.c"
#include	"/sys/ins/error.ins.c"
#include	"/sys/ins/pad.ins.c"
#include	"/sys/ins/streams.ins.c"
extern	char	*strcpy();

padedit(name, readonly)
char	*name;
{
name_$pname_t		in_name;
pinteger		in_len;
status_$t		st;
pad_$window_desc_t	window;
stream_$id_t		stream_id;

	in_len = strlen(strcpy(in_name, name));

	/* force default-sized window */
	window.top =
	window.left =
	window.width =
	window.height = 0;
	pad_$create_window(
		in_name, in_len,
		(pad_$type_t) (readonly ? pad_$read_edit : pad_$edit),
		(short)1, window,
		stream_id, st);

	if (error_$fail(st)) {
		error_$print(st);
		return(-1);
	} else {
		if (!readonly) {
			pad_$edit_wait(stream_id, st);
		}
		stream_$close(stream_id, st);
	}
	return(0);
}

#ifdef	TEST
main(argc, argv)
char	*argv[];
{
register j;
	for (j = 1; j < argc; j++)
		padedit(argv[j], 0);
}
#endif	TEST
