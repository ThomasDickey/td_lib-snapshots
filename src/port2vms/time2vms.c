/*
 * Title:	time2vms.c
 * Author:	T.E.Dickey
 * Created:	29 Sep 1988
 * Modified:
 *		07 Mar 2004, remove K&R support, indent'd.
 *		01 Dec 1993, ifdefs.
 *		21 Oct 1991, converted to ANSI
 *
 * Function:	Converts a unix time (seconds since 1-jan-1970) to a vms
 *		64-bit time (100-nanosecond units since 17-nov-1858).
 */

#include	"port2vms.h"

MODULE_ID("$Id: time2vms.c,v 12.4 2004/03/07 22:03:45 tom Exp $")

#ifdef	vms
#include	<time.h>

#define	VMS_SEC	10000000	/* one second on VMS */
#define	MIN	60
#define	HOUR	(60*MIN)
#define	DAY	(24*HOUR)
#define	MONTH	(30*DAY)

void
time2vms(long *vms_time,
	 time_t unix_time)
{
    static long scale = VMS_SEC;
    static long base = 0;
    static long january[2] =
    {0x4beb4000, 0x7c9567};
    static long two = 2;
    long product[2];

    lib$emul(&unix_time, &scale, &base, product);
    lib$addx(product, january, vms_time, &two);
}

#ifdef	TEST
/*ARGSUSED*/
_MAIN
{
    extern char *ctime();
    char buffer[132];
    short olen;
    long cvtflg = 0;
    static $DESCRIPTOR(date_time, "");
    time_t unix_time = time(0);
    long vms_time[2];
    struct tm mytm;
    int j;

    for (j = 0; j < 12; j += 2) {
	time2vms(vms_time, unix_time);

	date_time.dsc$a_pointer = buffer;
	date_time.dsc$w_length = sizeof(buffer) - 1;
	lib$sys_asctim(&olen, &date_time, vms_time, &cvtflg);
	buffer[olen] = '\0';
	printf("%d months before\n", j);
	printf("\tunix: %s", ctime(&unix_time));
	printf("\tvms:  %s\n", buffer);

	mytm = *localtime(&unix_time);
	printf("\ttm:   %02d/%02d/%02d %02d:%02d:%02d (dst=%d)\n",
	       mytm.tm_year, mytm.tm_mon + 1, mytm.tm_mday,
	       mytm.tm_hour, mytm.tm_min, mytm.tm_sec,
	       mytm.tm_isdst);
	unix_time -= 2 * MONTH;
    }
    exit(SUCCESS);
    /*NOTREACHED */
}
#endif /* TEST */

#endif /* vms */
