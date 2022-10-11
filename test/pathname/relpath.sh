#!/bin/sh
# $Id: relpath.sh,v 12.1 2022/10/11 08:15:03 tom Exp $
WD=`pwd`
cd ../..
TOP2=`pwd`
cd ..
TOP3=`pwd`
cd /
ROOT=`pwd`
cd $WD
./relpath | sed	-e s@\ $TOP2@\ TOP2@g \
		-e s@\ $TOP3@\ TOP3@g \
		-e s@\ $ROOT/@\ /@g |
	grep -v HOME | grep -v //dickey >relpath.tmp
