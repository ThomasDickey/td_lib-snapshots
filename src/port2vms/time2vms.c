#ifndef	lint
static	char	what[] = "$Header: /users/source/archives/td_lib.vcs/src/port2vms/RCS/time2vms.c,v 4.0 1988/11/30 13:41:43 ste_cm Rel $";
#endif	lint

/*
 * Title:	time2vms.c
 * Author:	T.E.Dickey
 * Created:	29 Sep 1988
 *
 * Function:	Converts a unix time (seconds since 1-jan-1970) to a vms
 *		64-bit time (100-nanosecond units since 17-nov-1858).
 */

#include	"portunix.h"
#include	<time.h>

#define	VMS_SEC	10000000	/* one second on VMS */
#define	MIN	60
#define	HOUR	(60*MIN)
#define	DAY	(24*HOUR)
#define	MONTH	(30*DAY)

time2vms(vms_time, unix_time)
long	vms_time[2];
time_t	unix_time;
{
	static	long	scale	= VMS_SEC;
	static	long	base	= 0;
	static	long	january[2] = { 0x4beb4000, 0x7c9567 };
	static	long	two	= 2;
	auto	long	product[2];

	lib$emul(&unix_time, &scale, &base, product);
	lib$addx(product, january, vms_time, &two);
}

#ifdef	TEST
main()
{
	extern	char	*ctime();
	auto	char	buffer[132];
	auto	short	olen;
	auto	long	cvtflg = 0;
	static	$DESCRIPTOR(date_time,"");
	auto	time_t	unix_time = time(0);
	auto	long	vms_time[2];
	struct	tm	mytm;
	register int	j;

	for (j = 0; j < 12; j += 2) {
		time2vms(vms_time, unix_time);

		date_time.dsc$a_pointer = buffer;
		date_time.dsc$w_length  = sizeof(buffer) - 1;
		lib$sys_asctim (&olen, &date_time, vms_time, &cvtflg);
		buffer[olen] = '\0';
		printf("%d months before\n", j);
		printf("\tunix: %s", ctime(&unix_time));
		printf("\tvms:  %s\n", buffer);

		mytm = *localtime(&unix_time);
		printf("\ttm:   %02d/%02d/%02d %02d:%02d:%02d (dst=%d)\n",
			mytm.tm_year, mytm.tm_mon+1, mytm.tm_mday,
			mytm.tm_hour, mytm.tm_min,   mytm.tm_sec,
			mytm.tm_isdst);
		unix_time -= 2 * MONTH;
	}
}
#endif	TEST