#!/bin/sh
# $Id: rawgets.sh,v 12.0 1992/11/19 09:59:44 ste_cm Rel $
OUT=rawgets.tmp
echo '? error' >$OUT
if ( ./rawgets 2>&1 >/dev/tty 2>&1 )
then
	if ( ./rawgets -w 2>&1 >/dev/tty 2>&1 )
	then	echo '** ok (no output)' >$OUT
	fi
fi
