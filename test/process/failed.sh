#!/bin/sh
# $Id: failed.sh,v 12.0 1992/11/18 14:37:22 ste_cm Rel $
ME=failed
if ( ./$ME 2>&1 >$ME.tmp 2>&1)
then echo >/dev/null
else rm -f $ME.tmp
fi
