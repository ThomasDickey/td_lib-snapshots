#!/bin/sh
# $Id: rawgets.sh,v 11.2 1992/11/18 14:06:54 dickey Exp $
OUT=rawgets.tmp
echo '? error' >$OUT
if ( ./rawgets )
then
	if ( ./rawgets -w )
	then	echo '** ok (no output)' >$OUT
	fi
fi
