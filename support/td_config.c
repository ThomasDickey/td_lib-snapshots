/*
 * $Id: td_config.c,v 12.2 1997/09/11 23:57:50 tom Exp $
 *
 * Convert a set of symbols substituted by autoconf for @DEFS@ into a series
 * of C #define statements.
 */

#include <stdio.h>

/* skip over a C-style comment so we don't get confused by its contents */
static int comment(fp, c)
	FILE *fp;
	int c;
{
	int d = 0;

	do {
		putchar(c);
		d = c;
		c = fgetc(fp);
	} while (c != '/' || d != '*');
	return c;
}

static int quoted(fp, c)
	FILE *fp;
	int c;
{
	int delim = c;
	int d;
	do {
		d = c;
		if (c != '\\')
			putchar(c);
		c = fgetc(fp);
	} while (c != delim);
	return c;
}

static int escape(fp, c)
	FILE *fp;
	int c;
{
	int d = c;

	c = fgetc(fp);
	if (c == '"') {
		c = quoted(fp, c);
	}
	return c;
}

static int td_config(fp)
	FILE *fp;
{
	int c = 0;
	int d = 0;

	while ((c = fgetc(fp)) != EOF && !feof(fp) && !ferror(fp)) {
		int ignore = 0;
		switch (c) {
		case '*':
			if (d == '/')
				c = comment(fp, c);
			break;
		case '\\':
			c = escape(fp, c);
			break;
		case ' ':
		case '-':
			ignore = 1;
			break;
		case 'D':
			if (d == '-') {
				fputs("\n#define", stdout);
				c = ' ';
			}
			break;
		case '=':
			c = '\t';
			break;
		}
		if (!ignore)
			putchar(c);
		d = c;
	}
}

int main(argc, argv)
	int argc;
	char *argv[];
{
	int n;
	if (argc > 1) {
		for (n = 1; n < argc; n++) {
			FILE *fp = fopen(argv[n], "r");
			if (fp == 0) {
				perror(argv[n]);
				exit(1);
			}
			td_config(fp);
			fclose(fp);
		}
	} else {
		td_config(stdin);
	}
	return 0;
}
