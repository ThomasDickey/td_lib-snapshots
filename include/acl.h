/* $Id: acl.h,v 12.1 1994/06/25 14:13:51 tom Exp $ */

#ifndef	ACL_H
#define	ACL_H

/* apollo sr10.1 acl bits, returned in 'stat()' in the st_rfu[2] field */

	/* user */
/* unassigned:	020000000000 */
#define	ACL_U_K	010000000000	/* keep */
#define	ACL_U_I	004000000000	/* inherit user */
#define	ACL_U_N	002000000000 	/* not-effective  (ignored) */
#define	ACL_U_P	001000000000
#define	ACL_U_R	000400000000
#define	ACL_U_W	000200000000
#define	ACL_U_X	000100000000

	/* group */
/* unassigned:	000040000000 */
#define	ACL_G_K	000020000000	/* keep */
#define	ACL_G_I	000010000000	/* inherit group */
#define	ACL_G_N	000004000000	/* not-effective (ignored) */
#define	ACL_G_P	000002000000
#define	ACL_G_R	000001000000
#define	ACL_G_W	000000400000
#define	ACL_G_X	000000200000

	/* project (organization) */
/* unassigned:	000000100000 */
#define	ACL_P_K	000000040000	/* keep */
/* unassigned:	000000020000 */
#define	ACL_P_N	000000010000	/* not-effective (ignored) */
#define	ACL_P_P	000000004000
#define	ACL_P_R	000000002000
#define	ACL_P_W	000000001000
#define	ACL_P_X	000000000400

	/* other */
/* unassigned:	000000000200 */
#define	ACL_O_K	000000000100	/* keep */
#define	ACL_O_T	000000000040	/* load-text */
#define	ACL_O_N	000000000020	/* not-effective (ignored) */
#define	ACL_O_P	000000000010
#define	ACL_O_R	000000000004
#define	ACL_O_W	000000000002
#define	ACL_O_X	000000000001

#define	EXTENDED_ACLS	(ACL_U_K | ACL_G_K | ACL_G_P | ACL_O_K | ACL_O_P)

#define	is_EXTENDED_ACL(rfu4) \
	((rfu4[2] & EXTENDED_ACLS)\
	|| !(rfu4[2] & ACL_P_N)\
	|| (rfu4[3] != 0))

#endif	/* ACL_H */
