/*
 * Title:	dumptty.c (dump tty settings to file)
 * Author:	T.E.Dickey
 * Created:	16 Jul 1994
 * Modified:
 *
 * Function:	writes the current tty settings to the given file, with
 *		a header message.  The first time it's called, 'dumptty()'
 *		saves the terminal settings for reference. This is used for
 *		debugging curses applications.
 */
#define TRM_PTYPES	/* <termios.h> */
#include <td_curse.h>

MODULE_ID("$Id: dumptty.c,v 12.2 1994/07/16 18:34:23 tom Exp $")

#define	CONTRAST(msg,f) Contrast(msg, (long)(tst.f), (long)(ref.f))
#define	SHOWMASK(t,m)   ShowMasks(t, SIZEOF(t), m)

typedef	struct	{
	long	code;
	char *	name;
	} TABLE;

static	FILE *	the_fp;

#if USING_TERMIOS_H || USING_TERMIO_H
static	char *	CtlChar(	/* c_cc[] characters */
		_AR1(int,	c))
		_DCL(int,	c)
{
	static	TABLE	table[] = {
#ifdef VINTR
		{ VINTR,	"INTR" },
#endif
#ifdef VQUIT
		{ VQUIT,	"QUIT" },
#endif
#ifdef VERASE
		{ VERASE,	"ERASE" },
#endif
#ifdef VKILL
		{ VKILL,	"KILL" },
#endif
#ifdef VEOF
		{ VEOF,		"EOF" },
#endif
#ifdef VTIME
		{ VTIME,	"TIME" },
#endif
#ifdef VMIN
		{ VMIN,		"MIN" },
#endif
#ifdef VSWTC
		{ VSWTC,	"SWTC" },
#endif
#ifdef VSTART
		{ VSTART,	"START" },
#endif
#ifdef VSTOP
		{ VSTOP,	"STOP" },
#endif
#ifdef VSUSP
		{ VSUSP,	"SUSP" },
#endif
#ifdef VEOL
		{ VEOL,		"EOL" },
#endif
#ifdef VREPRINT
		{ VREPRINT,	"REPRINT" },
#endif
#ifdef VDISCARD
		{ VDISCARD,	"DISCARD" },
#endif
#ifdef VWERASE
		{ VWERASE,	"WERASE" },
#endif
#ifdef VLNEXT
		{ VLNEXT,	"LNEXT" },
#endif
#ifdef VEOL2
		{ VEOL2,	"EOL2" },
#endif
	};
	static	char	temp[10];
	register int	n;
	for (n = 0; n < SIZEOF(table); n++)
		if (table[n].code == c)
			return table[n].name;
	FORMAT(temp, "cc[%d]", n);
	return temp;
}

static	TABLE	table_iflag [] = {	/* c_iflag bits */
#ifdef IGNBRK
		{ IGNBRK,	"IGNBRK" },
#endif
#ifdef BRKINT
		{ BRKINT,	"BRKINT" },
#endif
#ifdef IGNPAR
		{ IGNPAR,	"IGNPAR" },
#endif
#ifdef PARMRK
		{ PARMRK,	"PARMRK" },
#endif
#ifdef INPCK
		{ INPCK,	"INPCK" },
#endif
#ifdef ISTRIP
		{ ISTRIP,	"ISTRIP" },
#endif
#ifdef INLCR
		{ INLCR,	"INLCR" },
#endif
#ifdef IGNCR
		{ IGNCR,	"IGNCR" },
#endif
#ifdef ICRNL
		{ ICRNL,	"ICRNL" },
#endif
#ifdef IUCLC
		{ IUCLC,	"IUCLC" },
#endif
#ifdef IXON
		{ IXON,		"IXON" },
#endif
#ifdef IXANY
		{ IXANY,	"IXANY" },
#endif
#ifdef IXOFF
		{ IXOFF,	"IXOFF" },
#endif
#ifdef IMAXBEL
		{ IMAXBEL,	"IMAXBEL" },
#endif
	};

static	TABLE	table_oflag[] = {	/* c_oflag bits */
#ifdef OPOST
	{ OPOST,	"OPOST" },
#endif
#ifdef OLCUC
	{ OLCUC,	"OLCUC" },
#endif
#ifdef ONLCR
	{ ONLCR,	"ONLCR" },
#endif
#ifdef OCRNL
	{ OCRNL,	"OCRNL" },
#endif
#ifdef ONOCR
	{ ONOCR,	"ONOCR" },
#endif
#ifdef ONLRET
	{ ONLRET,	"ONLRET" },
#endif
#ifdef OFILL
	{ OFILL,	"OFILL" },
#endif
#ifdef OFDEL
	{ OFDEL,	"OFDEL" },
#endif
#ifdef NLDLY
	{ NLDLY,	"NLDLY" },
#endif
#ifdef CRDLY
	{ CRDLY,	"CRDLY" },
#endif
#ifdef TABDLY
	{ TABDLY,	"TABDLY" },
#endif
#ifdef BSDLY
	{ BSDLY,	"BSDLY" },
#endif
#ifdef VTDLY
	{ VTDLY,	"VTDLY" },
#endif
#ifdef FFDLY
	{ FFDLY,	"FFDLY" },
#endif
	};

static	TABLE	table_cflag[] = {	/* c_cflag bit meaning */
#ifdef CBAUD
	{ CBAUD,	"CBAUD" },
#endif
#ifdef CSIZE
	{ CSIZE,	"CSIZE" },
#endif
#ifdef CSTOPB
	{ CSTOPB,	"CSTOPB" },
#endif
#ifdef CREAD
	{ CREAD,	"CREAD" },
#endif
#ifdef PARENB
	{ PARENB,	"PARENB" },
#endif
#ifdef PARODD
	{ PARODD,	"PARODD" },
#endif
#ifdef HUPCL
	{ HUPCL,	"HUPCL" },
#endif
#ifdef CLOCAL
	{ CLOCAL,	"CLOCAL" },
#endif
	};

static	TABLE	table_lflag[] = {	/* c_lflag bits */
#ifdef ISIG
	{ ISIG,		"ISIG" },
#endif
#ifdef ICANON
	{ ICANON,	"ICANON" },
#endif
#ifdef XCASE
	{ XCASE,	"XCASE" },
#endif
#ifdef ECHO
	{ ECHO,		"ECHO" },
#endif
#ifdef ECHOE
	{ ECHOE,	"ECHOE" },
#endif
#ifdef ECHOK
	{ ECHOK,	"ECHOK" },
#endif
#ifdef ECHONL
	{ ECHONL,	"ECHONL" },
#endif
#ifdef NOFLSH
	{ NOFLSH,	"NOFLSH" },
#endif
#ifdef TOSTOP
	{ TOSTOP,	"TOSTOP" },
#endif
#ifdef ECHOCTL
	{ ECHOCTL,	"ECHOCTL" },
#endif
#ifdef ECHOPRT
	{ ECHOPRT,	"ECHOPRT" },
#endif
#ifdef ECHOKE
	{ ECHOKE,	"ECHOKE" },
#endif
#ifdef FLUSHO
	{ FLUSHO,	"FLUSHO" },
#endif
#ifdef PENDIN
	{ PENDIN,	"PENDIN" },
#endif
#ifdef IEXTEN
	{ IEXTEN,	"IEXTEN" },
#endif
	};

static
void	ShowMasks(
	_ARX(TABLE *,	table)
	_ARX(int,	length)
	_AR1(long,	mask)
		)
	_DCL(TABLE *,	table)
	_DCL(int,	length)
	_DCL(long,	mask)
{
	register int	n;
	for (n = 0; n < length; n++) {
		if (mask & table[n].code) {
			FPRINTF(the_fp, "\t=>%s (%#lo)\n",
				table[n].name,
				table[n].code);
			mask &= ~(table[n].code);
		}
	}
	if (mask != 0)
		FPRINTF(the_fp, "\t=>? (%#lo)\n", mask);
}
#endif	/* #if USING_TERMIOS_H || USING_TERMIO_H */

static
long	Contrast (
	_ARX(char *,	msg)
	_ARX(long,	tst)
	_AR1(long,	ref)
	)
	_DCL(char *,	msg)
	_DCL(long,	tst)
	_DCL(long,	ref)
{
	long	diff	= 0;

	FPRINTF(the_fp, "\t%8s %#lo", msg, tst);
	if (tst != ref) {
		FPRINTF(the_fp, " (was %#lo)", ref);
		diff = tst ^ ref;
	}
	(void)fputc('\n', the_fp);
	return diff;
}

void	dumptty (
	_ARX(FILE *,	fp)
	_AR1(char *,	msg)
	)
	_DCL(FILE *,	fp)
	_DCL(char *,	msg)
{
#if USING_TERMIOS_H || USING_TERMIO_H
	auto	int	n;
	auto	long	mask;
#endif
	static	TermioT ref;
	static	int	initialized;
	auto	TermioT tst;

	the_fp = fp;
	GetTerminal(&tst);
	FPRINTF(fp, "%s: \n", msg);
	if (!initialized) {
		ref = tst;
		initialized = TRUE;
	}
#if USING_TERMIOS_H || USING_TERMIO_H
	if ((mask = CONTRAST("iflag", c_iflag)) != 0)
		SHOWMASK(table_iflag, mask);
	if ((mask = CONTRAST("oflag", c_oflag)) != 0)
		SHOWMASK(table_oflag, mask);
	if ((mask = CONTRAST("cflag", c_cflag)) != 0)
		SHOWMASK(table_cflag, mask);
	if ((mask = CONTRAST("lflag", c_lflag)) != 0)
		SHOWMASK(table_lflag, mask);
	for (n = 0; n < NCCS; n++) {	/* control chars */
		if (tst.c_cc[n] != 0 || ref.c_cc[n] != 0)
			(void)CONTRAST(CtlChar(n), c_cc[n]);
	}
#else
	(void)CONTRAST("ispeed", sg_ispeed);	/* input speed */
	(void)CONTRAST("ospeed", sg_ospeed);	/* output speed */
	(void)CONTRAST("erase",  sg_erase);	/* erase character */
	(void)CONTRAST("kill",   sg_kill);	/* kill character */
	(void)CONTRAST("flags",  sg_flags);	/* mode flags */
#endif
	FFLUSH(fp);	/* just in case the program dies! */
}
