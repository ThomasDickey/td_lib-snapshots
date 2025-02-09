/*
 * Title:	padedit.c (pad-edit)
 * Author:	T.E.Dickey
 * Created:	14 Dec 1987
 * Modified:
 *		01 Dec 2019, use executev(), deprecate catarg().
 *			     require $DISPLAY to be set.
 *		07 Mar 2004, remove K&R support, indent'd.
 *		29 Oct 1993, ifdef-ident
 *		21 Sep 1993, gcc-warnings
 *		22 Dec 1992, reset process-group on SunOs to avoid having
 *			     signals kill the spawned xterms.
 *		04 Dec 1992, split 'editor' param in case it has options.
 *		09 Apr 1992, show filename in xterm-title.
 *		04 Oct 1991, conversion to ANSI
 *		12 Sep 1991, removed redundant def for 'errno' (VMS C 3.2)
 *		09 Sep 1991, lint (apollo SR10.3)
 *		06 Jun 1991, modified debug-traces. If xterm is invoked,
 *			     explicitly add display-argument to make this
 *			     visible in a "ps" command.  Added "-r" to test-
 *			     driver.
 *		31 May 1991, lint (SunOS)
 *		15 May 1991, mods to compile under apollo sr10.3
 *		07 Dec 1989, lint (SunOs 3.4)
 *		06 Sep 1989, use getwd definition from "ptypes.h"
 *		25 Jul 1989, recompiled with apollo SR10 -- mods for function
 *			     prototypes
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
#define	WAI_PTYPES
#include	"ptypes.h"
#include	<errno.h>

MODULE_ID("$Id: padedit.c,v 12.14 2025/01/06 23:59:56 tom Exp $")

#ifdef	SYS_UNIX

#ifdef	apollo
#ifdef	apollo_sr10
#include 	<apollo/base.h>
#include 	<apollo/error.h>
#include 	<apollo/pad.h>
#include 	<apollo/ios.h>
#else /* sr9.x */
#include	"/sys/ins/base.ins.c"
#include	"/sys/ins/error.ins.c"
#include	"/sys/ins/pad.ins.c"
#include	"/sys/ins/streams.ins.c"
#endif
#endif

#ifdef	apollo
int
apollo_edit(char *name, int readonly)
{
#ifdef	apollo_sr10
    name_$pname_t in_name;
    pinteger in_len;
    status_$t st;
    pad_$window_desc_t window;
    stream_$id_t stream_id;

    in_len = (size_t) strlen(strcpy(in_name, name));

    /* force default-sized window */
    window.top =
	window.left =
	window.width =
	window.height = 0;
    pad_$create_window(
			  in_name, in_len,
			  (pad_$type_t) (readonly ? pad_$read_edit : pad_$edit),
			  (short) 1, window,
			  &stream_id, &st);

    if (error_$fail(st)) {
	error_$print(st);
	return (-1);
    } else {
	if (!readonly) {
	    pad_$edit_wait(stream_id, &st);
	}
	ios_$close(stream_id, &st);
    }
#else /* sr9.x */
    name_$pname_t in_name;
    pinteger in_len;
    status_$t st;
    pad_$window_desc_t window;
    stream_$id_t stream_id;

    in_len = (size_t) strlen(strcpy(in_name, name));

    /* force default-sized window */
    window.top =
	window.left =
	window.width =
	window.height = 0;
    pad_$create_window(
			  in_name, in_len,
			  (pad_$type_t) (readonly ? pad_$read_edit : pad_$edit),
			  (short) 1, window,
			  stream_id, st);

    if (error_$fail(st)) {
	error_$print(st);
	return (-1);
    } else {
	if (!readonly) {
	    pad_$edit_wait(stream_id, st);
	}
	stream_$close(stream_id, st);
    }
#endif /* sr9.x */
    return (0);
}
#endif /* apollo */

/*
 * Spawn a process which is detached from the current one.
 */
static int
spawn(char *cmd, char **argv)
{
    int pid;
#ifdef	TEST
    int debug = 0;
#define	DEBUG(s,a)	if (debug) printf(s,a)
#else /* TEST */
#define	DEBUG(s,a)
#endif /* TEST */

    DCL_WAIT(status);

    if ((pid = fork()) > 0) {
	DEBUG("** spawn-1st (pid= %d)\r\n", pid);
	while (wait(ARG_WAIT(status)) >= 0) {
	    ;
	}
	DEBUG("spawn-1st (status= %#x)\n", W_RETCODE(status));
	if ((errno = W_RETCODE(status)) != 0)
	    return (-1);
	return (0);
    } else if (pid == 0) {
	DEBUG("spawn-%dst\n", 1);
	if ((pid = fork()) > 0) {
	    DEBUG("** spawn-%dnd\r\n", 2);
	    (void) _exit(0);	/* abandon exec'ing process */
	    /*NOTREACHED */
	} else if (pid == 0) {
	    DEBUG("** exec'ing %drd-process\r\n", 3);
#ifdef	sun
	    setsid();
#endif
	    (void) execvp(cmd, argv);
	    (void) _exit(errno);	/* just in case exec-failed */
	    /*NOTREACHED */
	}
    }
    return (-1);
}

int
padedit(const char *name, int readonly, const char *editor)
{
    int lc[2];
    int code = scr_size(lc);

#ifdef	apollo
    if (code > 0)
	return (apollo_edit(name, readonly));
    else
#endif /* apollo */
    if (code == 0) {
	char *display;
	char *argv[20];
	int argc;

	char wd[MAXPATHLEN], xt[MAXPATHLEN], tmp[BUFSIZ], the_title[BUFSIZ];

	if (getwd(wd) == NULL)
	    return (-1);
	if ((display = getenv("DISPLAY")) == NULL)
	    return -1;
	if (which(xt, (int) sizeof(xt), "xterm", wd) <= 0)
	    return (-1);

	FORMAT(the_title, "%s:%s", readonly ? "view" : "edit", name);

	argc = 0;
	argv[argc++] = xt;
	argv[argc++] = txtalloc("-display");
	argv[argc++] = display;
	argv[argc++] = txtalloc("-title");
	argv[argc++] = the_title;
	argv[argc++] = txtalloc("-e");
	argc += makeargv(&argv[argc], ((int) SIZEOF(argv) - argc - 2), tmp, editor);
	argv[argc++] = txtalloc(name);
	argv[argc] = NULL;

	if (readonly) {		/* spawn and run away */
	    return (spawn(xt, argv));
	} else {
	    return (executev(argv));
	}
    }
    return (-1);
}

#ifdef	TEST
/*ARGSUSED*/
_MAIN
{
    int j;
    int readonly = FALSE;
    for (j = 1; j < argc; j++) {
	if (!strcmp(argv[j], "-r"))
	    readonly = TRUE;
	else
	    padedit(argv[j], readonly, "view");
    }
    exit(SUCCESS);
    /*NOTREACHED */
}
#endif /* TEST */

#endif /* SYS_UNIX */
