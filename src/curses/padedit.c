#ifndef	lint
static	char	Id[] = "$Id: padedit.c,v 5.0 1989/09/06 15:49:36 ste_cm Rel $";
#endif	lint

/*
 * Title:	padedit.c (pad-edit)
 * Author:	T.E.Dickey
 * Created:	14 Dec 1987
 * $Log: padedit.c,v $
 * Revision 5.0  1989/09/06 15:49:36  ste_cm
 * BASELINE Fri Oct 27 12:27:25 1989 -- apollo SR10.1 mods + ADA_PITS 4.0
 *
 *		Revision 4.1  89/09/06  15:49:36  dickey
 *		use getwd definition from "ptypes.h"
 *		
 *		Revision 4.0  89/07/25  09:15:21  ste_cm
 *		BASELINE Thu Aug 24 09:38:55 EDT 1989 -- support:navi_011(rel2)
 *		
 *		Revision 3.1  89/07/25  09:15:21  dickey
 *		recompiled with apollo SR10 -- mods for function prototypes
 *		
 *		Revision 3.0  88/08/15  09:49:33  ste_cm
 *		BASELINE Mon Jun 19 13:27:01 EDT 1989
 *		
 *		Revision 2.0  88/08/15  09:49:33  ste_cm
 *		BASELINE Thu Apr  6 09:45:13 EDT 1989
 *		
 *		Revision 1.6  88/08/15  09:49:33  dickey
 *		sccs2rcs keywords
 *		
 *		27 Jul 1988, if we don't have Apollo pad, assume we may open
 *			     xterm-window.
 *
 * Function:	Open an edit-pad on the specified pathname.  If the pad
 *		is not read-only, suspend the calling process until the
 *		pad is closed.
 *
 * Notes:	Unlike a normal pad, we cannot toggle the RO-command.
 *		This is probably due to an APOLLO bug.
 */

#define	STR_PTYPES
#include	"ptypes.h"
#ifdef	apollo
#include	"/sys/ins/base.ins.c"
#include	"/sys/ins/error.ins.c"
#include	"/sys/ins/pad.ins.c"
#include	"/sys/ins/streams.ins.c"
#endif	apollo

#ifdef	apollo
apollo_edit(name, readonly)
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
#endif	apollo

/*
 * Spawn a process which is detached from the current one.
 */
static
spawn(cmd, argv)
char	*cmd;
char	*argv[];
{
	extern	int	errno;
	int	pid, status;
#ifdef	TEST
	int	debug	= 0;
#define	DEBUG(s,a)	if (debug) printf(s,a)
#else	TEST
#define	DEBUG(s,a)
#endif	TEST
#define	ERRNO	((status >> 8) & 0xff)

	if ((pid = fork()) > 0) {
		DEBUG("spawn-1st (pid= %d)\n", pid);
		while (wait(&status) >= 0);
		DEBUG("spawn-1st (status= %#x)\n", status);
		if (errno = ERRNO)
			return (-1);
		return (0);
	} else if (pid == 0) {
		DEBUG("spawn-1st\n",0);
		if ((pid = fork()) > 0) {
			DEBUG("spawn-2nd\n",0);
			(void)_exit(0);		/* abandon exec'ing process */
			/*NOTREACHED*/
		} else if (pid == 0) {
			DEBUG("exec'ing process\n",0);
			(void)execvp(cmd, argv);
			(void)_exit(errno);	/* just in case exec-failed */
			/*NOTREACHED*/
		}
	}
	return (-1);
}

padedit(name, readonly, editor)
char	*name, *editor;
int	readonly;
{
	int	lc[2];
	int	code = scr_size(lc);

#ifdef	apollo
	if (code > 0)
		return (apollo_edit(name, readonly));
	else
#endif	apollo
	if (code == 0) {
		static	char	*vec[] = {
				"",	/* path */
				"-e",
				"",	/* editor */
				"",	/* name */
				0
			};

		char	wd[BUFSIZ],
			xt[BUFSIZ];

		if (getwd(wd) == 0)
			return (-1);
		if (which(xt, sizeof(xt), "xterm", wd) <= 0)
			return (-1);

		vec[0] = xt;
		vec[2] = editor;
		vec[3] = name;

		if (readonly) {	/* spawn and run away */
			return (spawn(xt, vec));
		} else {
			char	args[BUFSIZ];
			int	j;

			*args = '\0';
			for (j = 1; vec[j]; j++)
				(void)strcat(strcat(args, " "), vec[j]);
			return (execute(xt, args));
		}
	}
	return (-1);
}

#ifdef	TEST
main(argc, argv)
char	*argv[];
{
register j;
	for (j = 1; j < argc; j++)
		padedit(argv[j], 1, "view");
}

failed(s)
char	*s;
{
	perror(s);
	exit(1);
}
#endif	TEST
