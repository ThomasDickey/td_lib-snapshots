/*
 * Title:	dyn_argv.c (argv lists using DYN strings)
 * Author:	T.E.Dickey
 * Created:	05 Dec 2019
 * Modified:
 *
 * Function:	Build argv lists, e.g., for "executev()" and "show_argv()".
 */

#define	DYN_INTERNAL
#define	CUR_PTYPES
#define	STR_PTYPES
#include	<ptypes.h>
#include	<dyn_str.h>
#include	<rcsdefs.h>
#include	<sccsdefs.h>
#include	<errno.h>

MODULE_ID("$Id: dyn_argv.c,v 12.6 2019/12/06 01:50:26 tom Exp $")

ARGV *
argv_init(void)
{
    ARGV *result = calloc(1, sizeof(ARGV));
    return result;
}

ARGV *
argv_init1(const char *command)
{
    ARGV *result = argv_init();
    result->the_command = dyn_copy(result->the_command, command);
    argv_append(&result, command);	/* actual argv[0] */
    return result;
}

ARGV *
argv_init2(const char *command, const char *executable)
{
    ARGV *result = argv_init();
    result->the_command = dyn_copy(result->the_command, command);
    argv_append(&result, executable);	/* actual argv[0] */
    return result;
}

void
argv_append(ARGV ** p, const char *value)
{
    ARGV *q;
    size_t actual;
    size_t oldlen;
    size_t newlen;

    if ((q = *p) == NULL)
	*p = q = argv_init();

#define REALLOC(list,old,new) \
	do { \
	    list = realloc(list, (new * sizeof(list[0]))); \
	    memset(&(list)[old], 0, (new - old) * sizeof(list[0])); \
	} while (0)

    actual = q->cur_length;
    oldlen = q->max_length;
    if (actual + 2 >= oldlen) {
	newlen = 32 + (oldlen * 2);
	q->max_length = newlen;
	REALLOC(q->the_strings, oldlen, newlen);
	REALLOC(q->the_array, oldlen, newlen);
    }
    if (value != NULL && *value != EOS) {
	q->the_strings[actual] = dyn_copy(q->the_strings[actual], value);
	q->the_array[actual] = dyn_string(q->the_strings[actual]);
	q->cur_length++;
    }
}

void
argv_append2(ARGV ** p, const char *option, const char *value)
{
    ARGV *q;
    size_t actual;

    if (option != NULL && *option != EOS && value != NULL && *value != EOS) {
	argv_append(p, option);
	if ((q = *p) != NULL && (actual = q->cur_length) != 0) {
	    --actual;
	    q->the_strings[actual] = dyn_append(q->the_strings[actual], value);
	    q->the_array[actual] = dyn_string(q->the_strings[actual]);
	}
    }
}

ARGV *
argv_merge(ARGV ** target, ARGV * source)
{
    ARGV *p = *target;
    size_t n;
    if (p == NULL)
	*target = p = argv_init();
    if (source != NULL) {
	for (n = 0; n < source->cur_length; ++n) {
	    argv_append(target, source->the_array[n]);
	}
    }
    return p;
}

char **
argv_values(ARGV * p)
{
    return p ? p->the_array : NULL;
}

DYN *
argv_flatten(ARGV * args)
{
    DYN *result = NULL;
    char **values = argv_values(args);
    char *source;
    int n;
    int ch;

    dyn_init(&result, BUFSIZ);
    for (n = 0; (source = values[n]) != NULL; ++n) {
	if (n != 0)
	    dyn_append_c(result, ' ');
	while ((ch = UCH(*source++)) != EOS) {
	    if (!valid_shell_char(ch)) {
		dyn_append_c(result, '\\');
	    }
	    dyn_append_c(result, ch);
	}
    }
    return result;
}

void
argv_free(ARGV ** p)
{
    ARGV *q;
    if ((q = *p) != NULL) {
	*p = NULL;
	while (q->cur_length-- != 0) {
	    dyn_free(q->the_strings[q->cur_length]);
	}
	free(q->the_strings);
	free(q->the_array);
	free(q);
    }
}
