#ifndef	NO_IDENT
static	char	Id[] = "$Id: rmsio.c,v 12.1 1994/08/21 18:39:51 tom Exp $";
#endif

/*
 * Title:	rmsio.c
 * Author:	T.E.Dickey
 * Created:	03 Nov 1988 (from earlier version w/o unix-style interface)
 * Modified:
 *		01 Dec 1993, ifdefs.
 *		20 Nov 1992, use prototypes
 *		22 Feb 1989, to permit user to append to existing file.
 *
 * Function:	Provides unix-style interface to VAX/RMS I/O.
 *
 * Entry:	rmsio_open:	Open an RMS-file, returning pointer to buffers
 *		rmsio_open2:	Open a file, providing default-specification.
 *		rmsio_read:	Read a record from RMS-file
 *		rmsio_write:	Write a record to RMS-file
 *		rmsio_close:	Close an RMS-file, releasing I/O buffers
 *		rmsio_perror:	Print message for last error
 *		rmsio_size:	Returns size of largest record of input.
 */

#include	"port2vms.h"
#include	<ctype.h>
#include	<errno.h>

#ifdef	vms
#include	<rms.h>
#include	<ssdef.h>

#define	c$$translate(sts)	errno = EVMSERR; vaxc$errno = sts

#define	REG	register

#define DEFAULT_RSIZE	512

#define sys(f)		status = (f);\
			if (status && !$VMS_STATUS_SUCCESS(status))
#define save_st		rmsio_errno = status
#define latch		if (!rmsio_errno) save_st

#define CHECK(f)	sys(f)	{save_st; goto failed;}
#define CHECK2(f)	sys(f)	{latch; goto failed;}

typedef struct	_rfile {
	struct	_rfile	*link;
	int		fd;
	struct	RAB	rab;
	struct	FAB	fab;
	struct	NAM	nam;
	struct	XABFHC	xabfhc;
	char	esa[NAM$C_MAXRSS],	/* expanded by SYS$PARSE	*/
		rsa[NAM$C_MAXRSS];	/* result from SYS$SEARCH	*/
	} RFILE;

/*
 * Module-level data:
 */
static	RFILE	*fd_list;
static	long	rmsio_errno = 0;
static	char	rmsio$nam[NAM$C_MAXRSS] = "";	/* Last-parsed filename */

#define zFAB	z->fab
#define zNAM	z->nam
#define zRAB	z->rab

#define BLOCKED (blocked ? FAB$M_BRO : 0)
#define isBLOCK (zFAB.fab$b_fac & FAB$M_BRO)

/************************************************************************
 *	private procedures						*
 ************************************************************************/

	/* allocate a new RFILE structure */
static	int	alloc_fd()
	{
		auto	RFILE	*z = calloc(1, sizeof(RFILE));
		static	int	unique_fd;

		unique_fd++;
		z->link = fd_list;
		fd_list = z;
		z->fd = unique_fd;
		return (unique_fd);
	}

	/* locate an existing RFILE structure */
static	RFILE	*find_fd(fd)
		int	fd;
	{
		REG	RFILE	*z = fd_list;

		while (z) {
			if (z->fd == fd)
				return (z);
			z = z->link;
		}
		return (0);
	}

	/* release storage associated with an RFILE structure */
static	void	free_fd(fd)
		int	fd;
	{
		REG	RFILE	*z, *x = 0;

		for (z = fd_list; z; z = z->link) {
			if (z->fd == fd) {
				auto	char	*ubf_	= zRAB.rab$l_ubf;
				if (x)		x->link = z->link;
				else		fd_list = z->link;
				if (ubf_)	cfree(ubf_);
				cfree(z);
				return;
			}
			x = z;
		}
	}

/************************************************************************
 *	public entrypoints						*
 ************************************************************************/

/* <rmsio_open>:
 * Open a file for record I/O:
 *
 * mode_:
 *	"r" - read, record
 *	"w" - write, record
 *	"a" - append, record
 *	"R" - read, block (512)
 *	"W" - write, block
 *	"A" - append, block
 * For output files, 'mode_' may be suffixed with the record-length.  The
 * file will be assumed fixed in this case.
 */
int	rmsio_open (
	_ARX(char *,	name_)
	_AR1(char *,	mode_)
		)
	_DCL(char *,	name_)
	_DCL(char *,	mode_)
{
	return (rmsio_open2(name_, 0, mode_));
}

/* <rmsio_open2>:
 * Open a file for record I/O, giving a default file specification.
 *
 *	name_	=> filename
 *	dft_	=> default specification
 *	mode_	=> Unix-style (record/block) mode (see: 'rmsio_open')
 */
int	rmsio_open2 (
	_ARX(char *,	name_)
	_ARX(char *,	dft_)
	_AR1(char *,	mode_)
		)
	_DCL(char *,	name_)
	_DCL(char *,	dft_)
	_DCL(char *,	mode_)
{
	auto	int	fd = alloc_fd();
	REG	RFILE	*z = find_fd(fd);
	auto	long	status;
	auto	int	newfile = (_tolower(*mode_) != 'r'),
			append	= (_tolower(*mode_) == 'a'),
			blocked = (isupper(*mode_)),
			no_old	= FALSE,
			recsize = DEFAULT_RSIZE,
			len;
	auto	char	*s_	= mode_ + 1;

	if (z == 0)
		return (-1);

	zFAB = cc$rms_fab;

	zFAB.fab$l_fop = FAB$M_NAM;
	zFAB.fab$l_nam = &zNAM;			/* FAB => NAM block	*/
	if (dft_) {
		zFAB.fab$l_dna = dft_;		/* Default-selection	*/
		zFAB.fab$b_dns = strlen(dft_);
	}
	if (name_) {
		zFAB.fab$l_fna = name_;
		zFAB.fab$b_fns = strlen(name_);
	}

	zNAM = cc$rms_nam;
	zNAM.nam$b_ess = NAM$C_MAXRSS;
	zNAM.nam$l_esa = &z->esa;
	zNAM.nam$b_rss = NAM$C_MAXRSS;
	zNAM.nam$l_rsa = &z->rsa;

	zFAB.fab$l_xab = &z->xabfhc;
	z->xabfhc = cc$rms_xabfhc;

	zRAB = cc$rms_rab;
	zRAB.rab$l_fab = &zFAB;

	/*
	 * Open the file.  We assume that if it is a new file, then no
	 * version number is given.  Exploit this to obtain the format
	 * of any previously-existing file of that name.
	 */
	CHECK(sys$parse(&zFAB));
	strncpy (rmsio$nam, z->esa, len = zNAM.nam$b_esl);
	rmsio$nam[len] = '\0';

	if (newfile) {
		zFAB.fab$b_fac = FAB$M_GET | BLOCKED;
		sys(sys$search(&zFAB))
			no_old = TRUE;
		else {
			sys(sys$open(&zFAB))
				no_old	= TRUE;
			else
				sys$close(&zFAB);
		}
		if (isdigit(*s_)) {
			recsize = 0;	/* Assume the argument is ok!! */
			while (*s_)
				recsize = (recsize * 10) + *s_++ - '0';
			zFAB.fab$b_rfm = FAB$C_FIX;
			zFAB.fab$b_rat = (recsize == 512) ? FAB$M_BLK : 0;
			zFAB.fab$w_mrs = recsize;
		} else if (no_old) {
			zFAB.fab$b_rfm = FAB$C_VAR;
			zFAB.fab$b_rat |= FAB$M_CR;
		}
		if (append && !no_old) {
			zFAB.fab$b_fac = FAB$M_PUT | BLOCKED;
			CHECK(sys$open(&zFAB));
			zRAB.rab$l_rop |= RAB$M_EOF;
		} else {
			zFAB.fab$b_fac = FAB$M_PUT | BLOCKED;
			CHECK(sys$create(&zFAB));
		}
	} else {
		zFAB.fab$b_fac |= FAB$M_GET | BLOCKED;
		CHECK(sys$search(&zFAB));
		CHECK(sys$open(&zFAB));
	}

	/*
	 * Allocate a buffer big enough for the biggest record:
	 */
	if (!newfile)	recsize = rmsio_size(z->fd);
	zRAB.rab$l_ubf = calloc (1, zRAB.rab$w_usz = recsize);

	/*
	 * Connect an I/O stream to the file:
	 */
	CHECK(sys$connect(&zRAB));
	return (z->fd);

failed:
	c$$translate(rmsio_errno);
	rmsio_close (z);
	return (-1);
}

/* <rmsio_read>:
 * Read a record, returning the number of bytes read.
 * Note that with RMS, we may have zero-length records; test for a negative
 * code to end the file.
 */
int	rmsio_read (
	_ARX(int,	fd)		/* file-descriptor pointer	*/
	_ARX(char *,	bfr)		/* buffer to load		*/
	_AR1(int,	maxbfr)		/* ...its size			*/
		)
	_DCL(int,	fd)		/* file-descriptor pointer	*/
	_DCL(char *,	bfr)		/* buffer to load		*/
	_DCL(int,	maxbfr)		/* ...its size			*/
{
	REG	RFILE	*z	= find_fd(fd);
	auto	long	status;
	auto	int	len;
	auto	char	*s_;

	if (z == 0)
		return (-1);

	zRAB.rab$b_rac = RAB$C_SEQ;
	if (isBLOCK) {		/* return zero if error or EOF	*/
		CHECK(sys$read(&zRAB));
	} else {
		CHECK(sys$get(&zRAB));
	}

	len = zRAB.rab$w_rsz;		/* limit length of returned buffer */
	if (len > maxbfr) len = maxbfr; /* ...to caller's 'maxbfr' size */

	memcpy (bfr, (char *) zRAB.rab$l_rbf, len);
	return (len);

failed:
	c$$translate(rmsio_errno);
	return (-1);
}

/* <rmsio_write>:
 * Write a record, given the buffer address and number of characters to
 * write.
 */
int	rmsio_write (
	_ARX(int,	fd)		/* file-descriptor pointer	*/
	_ARX(char *,	bfr)		/* buffer to load		*/
	_AR1(int,	maxbfr)		/* ...its size			*/
		)
	_DCL(int,	fd)		/* file-descriptor pointer	*/
	_DCL(char *,	bfr)		/* buffer to load		*/
	_DCL(int,	maxbfr)		/* ...its size			*/
{
	REG	RFILE	*z	= find_fd(fd);
	auto	long	status;

	if (z == 0)
		return (-1);

	zRAB.rab$b_rac = RAB$C_SEQ;

	zRAB.rab$w_rsz = maxbfr;	/* set length of buffer		*/
	zRAB.rab$l_rbf = bfr;		/* ...and copy buffer-pointer	*/
	if (isBLOCK) {			/* return nonzero if error	*/
		CHECK2(sys$write(&zRAB));
	} else {
		CHECK2(sys$put(&zRAB));
	}
	return (maxbfr);		/* assume we wrote it all	*/

failed:
	c$$translate(rmsio_errno);
	if (status)	rmsio_perror("write");
	return (-1);
}

/* <rmsio_close>:
 * Close the file, returning negative status iff an error occurs.
 */
int	rmsio_close (
	_AR1(int,	fd))
	_DCL(int,	fd)
{
	REG	RFILE	*z	= find_fd(fd);
	auto	long	status;

	CHECK2(sys$disconnect(&zRAB));
	CHECK2(sys$close(&zFAB));
	free_fd(fd);
	return (0);

failed:
	c$$translate(rmsio_errno);
	free_fd(fd);
	return (-1);
}

/* <rmsio_perror>:
 * Display the cause of the last error condition, like 'perror'
 */
void	rmsio_perror (
	_AR1(char *,	s))
	_DCL(char *,	s)
{
	auto	char	msg[80+NAM$C_MAXRSS];
	$DESCRIPTOR(DSCxmsg,msg);
	auto	short	retlen	= 0;
	auto	int	flags	= 9;	/* Show component + text only	*/

	lib$sys_getmsg (&rmsio_errno, &retlen, &DSCxmsg, &flags);
	if (retlen >= sizeof(msg))	retlen = sizeof(msg)-2;
	msg[retlen] = '\0';

	if (rmsio_errno == RMS$_FNF)
		sprintf (&msg[strlen(msg)], ": %.80s", rmsio$nam);
	printf ("%.80s", msg);
	rmsio_errno = 0;
}

/* <rmsio_size>:
 * Return the length of the longest record in (an input) file:
 */
int	rmsio_size (
	_AR1(int,	fd))
	_DCL(int,	fd)
{
	REG	RFILE	*z	= find_fd(fd);
	auto	struct	XABFHC	*xab_;
	auto	int	size;

	if (z == 0)
		return (-1);

	if (isBLOCK)	return (512);
	if (xab_ = zFAB.fab$l_xab) {
		while (xab_) {
			if (xab_->xab$b_cod == XAB$C_FHC) {
				if (size = xab_->xab$w_lrl)
					return (size);
			}
			xab_ = xab_->xab$l_nxt;
		}
	}
	return (DEFAULT_RSIZE);
}

/*
 * Test this module by using it to copy files
 */
#ifdef	TEST
/*ARGSUSED*/
_MAIN
{
	auto	int	src, dst, len, size;
	auto	int	records	= 0;
	auto	char	*buffer;

	if (argc <= 2) {
		printf("usage: rmsio src-file dst-file\n");
		exit(FAIL);
	}
	if (src = rmsio_open(argv[1], "r")) {
		printf("src-fd  = %d\n", src);
		printf("...size = %d\n", size = rmsio_size(src));
		buffer = calloc(1, size);
		if (dst = rmsio_open(argv[2], "w")) {
			printf("dst-fd  = %d\n", dst);
			while ((len = rmsio_read(src, buffer, size)) >= 0) {
				rmsio_write(dst, buffer, len);
				records++;
			}
			rmsio_close(dst);
			rmsio_close(src);
			printf("copied %d records\n", records);
		}
	}
	exit (SUCCESS);
	/*NOTREACHED*/
}
#endif	/* TEST */

#endif	/* vms */
