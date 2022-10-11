#!/bin/sh
# $Id: packdate.sh,v 12.1 2022/10/11 08:15:03 tom Exp $
OUT=packdate.tmp
./packdate |grep : |sed -e s'@^.*: @@' |uniq >$OUT
NUM=`wc -l <$OUT`
if test $NUM = 1
then	echo '** conversion ok' >$OUT
fi
