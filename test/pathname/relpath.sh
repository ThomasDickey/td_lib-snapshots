#!/bin/sh
# $Id: relpath.sh,v 12.0 1992/11/18 13:49:20 ste_cm Rel $
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
	fgrep -v HOME | fgrep -v //dickey >relpath.tmp
