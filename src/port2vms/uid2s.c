#ifndef	lint
static	char	what[] = "$Header: /users/source/archives/td_lib.vcs/src/port2vms/RCS/uid2s.c,v 3.0 1989/03/14 14:37:37 ste_cm Rel $";
#endif	lint

/*
 * Title:	uid2s.c
 * Author:	T.E.Dickey
 * Created:	15 Dec 1988
 * Modified:
 *		14 Mar 1989, extended test-driver
 *
 * Function:	Given a unix-style uid, finds the corresponding username string
 *		and returns a pointer to it.
 *
 *		The corresponding inverse translation is bundled with this
 *		module on VMS since no password support is provided.
 */

#include	"portunix.h"
#include	<string.h>
#ifdef	vms
#include	<ssdef.h>
#endif	vms

typedef	unsigned long	LID;

#ifdef	TEST
#define	UID_MASK	0xffffffff
#else
#define	UID_MASK	0xffff
#endif	TEST

typedef	struct	{
		LID	id;
		char	*name;
	} LIST;

static	LIST	*vec;		/* list of uid/gid values */
static	int	numvec;		/* number of items in 'vec[]' */

#ifdef	TEST
static
char *
show(uid)
{
	static	char	buffer[80];
	sprintf(buffer, "%o,%o", (uid>>16) & 0xffff, uid & 0xffff);
	return (buffer);
}
#endif	TEST

static	build_vec()
{
	register int	j;

	auto	long	st;
	auto	unsigned amount;
	auto	char	buffer[80];

	auto	long	contxt	= 0;
	auto	LID	id	= -1;
	auto	short	namlen	= 0;
	auto	LID	resid	= 0;
	auto	long	attrib	= 0;
	auto	$DESCRIPTOR(nambuf,buffer);

	/*
	 * Unix-style uid's are mapped into VMS id's by gluing in the gid field.
	 * If the caller does not provide both, we must do a scan to find the
	 * name assuming that there is a single gid value (usually the case!).
	 *
	 * This saves the id's returned so that successive calls will be faster.
	 */
	if (vec == 0) {
		while ((st = sys$idtoasc(
				id,
				&namlen,
				&nambuf,
				&resid,
				0/*&attrib*/,
				&contxt)) == SS$_NORMAL) {
			buffer[namlen] = '\0';
			j = numvec;
			amount	= (++numvec) * sizeof(LIST);
			vec = doalloc(vec, amount);
			vec[j].id   = resid;
			vec[j].name = stralloc(buffer);
#ifdef	TEST
printf("id = %s, name = %s\n", show(vec[j].id), vec[j].name);
#endif	TEST
		}
		if (st != SS$_NOSUCHID)
			sys$finish_rdb(contxt);
	}
}

/*
 * Translate a uid to a string, returning a pointer to my buffer
 */
char *
uid2s(uid)
{
	static	char	buffer[80];
	register int	j;
	auto	LID	resid;

	build_vec();
	(void) strcpy(buffer, "?");
	for (j = 0; j < numvec; j++) {
		resid = vec[j].id;
		if (((resid & 0xffff) == (LID)uid)
		||  (resid == (LID)uid))
			(void) strcpy(buffer, vec[j].name);
	}
	return (buffer);
}

/*
 * Translate a string back to a uid, returning a negative number iff not found
 */
s2uid(name)
char	*name;
{
	register int	j;

	build_vec();
	for (j = 0; j < numvec; j++) {
		if (!strucmp(name, vec[j].name))
			return (vec[j].id & UID_MASK);
	}
	return (-1);
}

#ifdef	TEST
main(argc, argv)
char	*argv[];
{
	auto	 int	uid = getuid();
	register int	j;
	auto	 char	buffer[L_cuserid];
	extern	 char	*cuserid();

	printf("lookup (cuserid=%s)\n", cuserid(buffer));
	printf("uid %s => '%s'\n", show(uid), uid2s(uid));
	printf("uid %s => '%s'\n", show(uid), uid2s(uid));
	printf("invert => %s\n",   show(s2uid(cuserid(buffer))));

	for (j = 1; j < argc; j++) {
		uid = s2uid(argv[j]);
		printf("uid of %s is %s\n", argv[j], show(uid));
	}
	exit(SUCCESS);
}
#endif	TEST
/*  DEC/CMS REPLACEMENT HISTORY, Element UID2S.C */
/*  *3    14-MAR-1989 14:58:15 DICKEY "elaborated on the test-driver to use it as a utility" */
/*  *2     9-JAN-1989 08:37:52 DICKEY "added 's2uid()' function" */
/*  *1    15-DEC-1988 11:42:17 DICKEY "VMS version of uid-to-string converter" */
/*  DEC/CMS REPLACEMENT HISTORY, Element UID2S.C */
