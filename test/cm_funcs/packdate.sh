#!/bin/sh
# $Id: packdate.sh,v 12.0 1992/11/18 12:39:32 ste_cm Rel $
OUT=packdate.tmp
./packdate |fgrep : |sed -e s'@^.*: @@' |uniq >$OUT
NUM=`wc -l <$OUT`
if test $NUM = 1
then	echo '** conversion ok' >$OUT
fi
