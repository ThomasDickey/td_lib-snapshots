#!/bin/sh
# $Id: abspath.sh,v 12.0 1992/11/18 11:36:27 ste_cm Rel $
WD=`pwd`
cd ../..
TOP=`pwd`
cd /
ROOT=`pwd`
cd $WD
./abspath |\
	sed	-e s@$TOP@TOP@ \
		-e s@$ROOT/@/@ \
		-e s@$ROOT@/@ | fgrep -v node_data >`basename $0 .sh`.tmp
