#ifndef	lint
static	char	Id[] = "$Id: shoarg.c,v 9.1 1991/06/20 08:25:19 dickey Exp $";
#endif

/*
 * Title:	shoarg.c (display argv-array)
 * Created:	20 Jun 1991
 * Modified:
 *
 * Function:	Displays a command + argument-string constructed by 'catarg()'
 *
 * Arguments:	fp	- file-pointer to which to write result
 *		command	- command-verb
 *		string	- the command-string to display
 */

#include	"ptypes.h"
#include	<ctype.h>

shoarg (fp, command, string)
FILE	*fp;
char	*command, *string;
{
	char	temp[BUFSIZ];
	register int	j;
	for (j = 0; (j < (sizeof(temp)-1)) && (temp[j] = *string++); j++)
		if (!isascii(temp[j])) {
			temp[j+1] = toascii(temp[j]);
			temp[j++] = '\\';
		}
	temp[j] = EOS;

	FPRINTF(fp, "%% %s %s\n", command, temp);
}
