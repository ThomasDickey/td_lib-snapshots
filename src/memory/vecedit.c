/*
 * Title:	vecedit.c
 * Author:	T.E.Dickey
 * Created:	16 Sep 1991
 * Modified:
 *		07 Mar 2004, remove K&R support, indent'd.
 *		29 Oct 1993, ifdef-ident
 *		21 Sep 1993, gcc-warnings
 *		10 Oct 1991, corrected off-by-one error in 'e_at' variable.
 *
 * Function:	Processes two lists of pointers to strings.  The first list
 *		represents a file to be edited; the second a list of editing
 *		commands generated by the UNIX 'diff -n' command.
 *
 * Returns:	A new list, representing the edited file.  All strings in the
 *		list are computed by copying pointers from the input-arguments.
 */

#include <ptypes.h>

MODULE_ID("$Id: vecedit.c,v 12.5 2004/03/07 22:03:45 tom Exp $")

char **
vecedit(char **text, char **script)
{
    unsigned len_in = veclen(text), len_out = len_in, use_in = 0, use_out = 0;
    unsigned e_at, e_len;
    char e_type, **result;

    register unsigned j;

    static char *fmt = "%c%d %d\n";

    /*
     * First, determine the resulting length of the edited "file"
     */
    for (j = 0; script[j]; j++) {
	if ((sscanf(script[j], fmt, &e_type, &e_at, &e_len) != 3)
	    || (e_type != 'a' && e_type != 'd')
	    || (e_at < 1 || e_at > len_in)) {
	    FPRINTF(stderr, "illegal edit:%s", script[j]);
	    continue;
	}
	if (e_type == 'a') {
	    j += e_len;
	    len_out += e_len;
	} else {
	    len_out -= e_len;
	}
    }
    result = vecalloc(len_out + 3);	/* patch */

    /*
     * Now, compute the contents of the edited-file
     */
    for (j = 0; script[j]; j++) {
	if ((sscanf(script[j], fmt, &e_type, &e_at, &e_len) != 3)
	    || (e_type != 'a' && e_type != 'd')
	    || (e_at < 1 || e_at > len_in)) {
	    continue;		/* already complained once! */
	}

	if (e_type != 'a')	/* assume delete before add! */
	    e_at--;		/* convert to array-index */

	while (use_in < e_at)
	    result[use_out++] = text[use_in++];

	if (e_type == 'a') {
	    while (e_len-- > 0)
		result[use_out++] = script[++j];
	} else {
	    use_in += e_len;
	}
    }

    while (use_in <= len_in)
	result[use_out++] = text[use_in++];

    result[len_out] = 0;
    return (result);
}

#ifdef	TEST
void
show_delta(char *buffer)
{
}

_MAIN
{
    FILE *pp;
    int len2, len4;
    char **vec1, **vec2, **vec3, **vec4;
    char command[BUFSIZ];

    register int j;

    if (argc != 3) {
	FPRINTF(stderr, "need two files\n");
	exit(FAIL);
	/*NOTREACHED */
    }
    (void) file2argv(argv[1], &vec1);
    len2 = file2argv(argv[2], &vec2);

    FORMAT(command, "diff -n %s %s", argv[1], argv[2]);
    if ((pp = popen(command, "r")) != NULL) {
	(void) fp2argv(pp, &vec3, show_delta);
	(void) pclose(pp);
	vec4 = vecedit(vec1, vec3);
	len4 = veclen(vec4);
	if (len2 != len4) {
	    FPRINTF(stderr, "lengths: %d vs %d\n", len2, len4);
	    exit(FAIL);
	    /*NOTREACHED */
	}
	for (j = 0; vec2[j] && vec4[j]; j++) {
	    if (strcmp(vec2[j], vec4[j])) {
		FPRINTF(stderr, "mismatch at %d:\n%s\n%s\n",
			j + 1, vec2[j], vec4[j]);
		exit(FAIL);
		/*NOTREACHED */
	    }
	}
	PRINTF("** ok (%s, %s)\n", argv[1], argv[2]);
    }
    exit(SUCCESS);
    /*NOTREACHED */
}
#endif
