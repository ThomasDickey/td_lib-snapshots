#ifndef	NO_IDENT
static	char	Id[] = "$Id: utimes.c,v 12.1 1994/08/21 18:39:51 tom Exp $";
#endif

/*
 * Title:	utimes.c
 * Author:	T.E.Dickey
 * Created:	29 Sep 1988
 * Modified:
 *		01 Dec 1993, ifdefs.
 *		20 Nov 1992, use prototypes
 *
 * Function:	Sets VMS file creation and revision times to correspond with
 *		the unix modification and change times.
 *
 * Bugs:	does not check for success of the modify-time
 */

#include	"port2vms.h"

#ifdef	vms
#include	<rms.h>
#include	<iodef.h>
#include	<errno.h>

#include	<fibdef.h>
#include	<atrdef.h>
/* patch: "acp.h" is up-to-date, except for the following, plus overlay stuff */
#define	FIB	struct fibdef		/* patch */
#define	ATR	struct atrdef		/* patch */

#define	sys(f)	status = f;\
		if (!$VMS_STATUS_SUCCESS(status)) {\
			errno = EVMSERR;\
			vaxc$errno = status;\
			return(-1);\
		}

#define	TIMEVAL	struct	timeval

/*
 * Lookup/Modify attributes using ACP:
 */
int	utimes (
	_ARX(char *,	filespec)
	_AR1(TIMEVAL *,	tv)
		)
	_DCL(char *,	filespec)
	_DCL(TIMEVAL *,	tv)
{
	auto	struct	FAB	fab;
	auto	struct	NAM	nam;
	auto	long		status;
	auto	FIB	fib;
	auto	ATR	atr[3];		/* Size: 1 more than max attributes */
	auto	long	credate[2];	/* File-creation-date	    */
	auto	long	revdate[2];	/* File-revision-date	    */

	auto	char		esa[NAM$C_MAXRSS],
				rsa[NAM$C_MAXRSS];
	auto	long		iosb[2];
	auto	short		chnl;
	auto	int		j	= 0,
				func;

	$DESCRIPTOR(DSC_name,rsa);
	struct	dsc$descriptor	fibDSC;

	fab = cc$rms_fab;
	fab.fab$l_fop = FAB$M_NAM;
	fab.fab$l_nam = &nam;		/* FAB => NAM block	*/
	fab.fab$b_dns = strlen(fab.fab$l_dna = ".;");
	fab.fab$b_fns = strlen(fab.fab$l_fna = filespec);

	nam = cc$rms_nam;
	nam.nam$b_ess = NAM$C_MAXRSS;
	nam.nam$l_esa = esa;
	nam.nam$b_rss = NAM$C_MAXRSS;
	nam.nam$l_rsa = rsa;

	sys(sys$parse(&fab))
	sys(sys$search(&fab))

	DSC_name.dsc$w_length = nam.nam$b_rsl;
	sys(sys$assign (&DSC_name, &chnl, 0, 0))

	fibDSC.dsc$w_length = sizeof(FIB);
	fibDSC.dsc$a_pointer = &fib;
	memset (&fib, 0, sizeof(FIB));
	memcpy (&fib.fib$r_fid_overlay.fib$w_fid[0], &nam.nam$w_fid[0], 6);

#define	SET(type,size,addr) {\
	atr[j].atr$w_type = type;\
	atr[j].atr$w_size = size;\
	atr[j++].atr$l_addr = addr;}

	time2vms(credate, tv[1].tv_sec);    /* "updated" unix time */
	time2vms(revdate, tv[0].tv_sec);    /* "accessed" unix time */

	func = IO$_MODIFY;
	SET(ATR$C_CREDATE,ATR$S_CREDATE,credate)
	SET(ATR$C_REVDATE,ATR$S_REVDATE,revdate)
	atr[j].atr$w_size = atr[j].atr$w_type = 0;

	sys(sys$qiow (0, chnl, func, iosb, 0, 0,
			&fibDSC, 0,0,0, &atr[0],0))
	sys(sys$dassgn(chnl))
	return (0);
}

#ifdef	TEST
/*ARGSUSED*/
_MAIN
{
	static	TIMEVAL	tv[2];
	int	j;
	for (j = 1; j < argc; j++)
		if (utimes(argv[j], tv) < 0)
			perror(argv[j]);
	exit(SUCCESS);
	/*NOTREACHED*/
}
#endif	/* TEST */

#endif	/* vms */
