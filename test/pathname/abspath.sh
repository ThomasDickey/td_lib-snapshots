#!/bin/sh
# $Id: abspath.sh,v 12.1 2022/10/11 08:15:03 tom Exp $
WD=`pwd`
cd ../..
TOP=`pwd`
cd /
ROOT=`pwd`
cd $WD
./abspath |\
	sed	-e s@$TOP@TOP@ \
		-e s@$ROOT/@/@ \
		-e s@$ROOT@/@ | grep -v node_data >`basename $0 .sh`.tmp
