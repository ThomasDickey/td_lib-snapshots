#!/bin/sh
# $Id: execute.sh,v 12.0 1992/11/24 13:34:07 ste_cm Rel $
ME=execute
WD=`pwd`
cd /
ROOT=`pwd`
cd $WD
( ./$ME ./$ME ls -CF ./$ME.sh 2>&1 ) |\
	sed	-e s@$ROOT/@/@ \
		-e s@/usr/bin@/bin@ >$ME.tmp
