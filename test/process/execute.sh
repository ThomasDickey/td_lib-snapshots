#!/bin/sh
# $Id: execute.sh,v 11.2 1992/11/18 14:34:10 dickey Exp $
ME=execute
WD=`pwd`
cd /
ROOT=`pwd`
cd $WD
( ./$ME ./$ME ls -CF ./$ME.sh 2>&1 ) | sed -e s@$ROOT/@/@ >$ME.tmp
