#!/bin/sh
# $Id: editfile.sh,v 8.0 1992/11/20 11:01:11 ste_cm Rel $
#
ME=editfile
rm -f *.tst *.bak
trap "rm -f *.tst *.bak" 0 1 2 5 15
for i in ../support/*.mms
do	j=`basename $i .mms`
	cp $i ./$j.tst
	cp $i ./$j.bak
	chmod u+w $j.bak
	echo LAST >>$j.bak
done
#
./editfile ../test/*.tst 2>&1 | sed -e 's@"file.*"@TMP@' >$ME.tmp
#
for i in *.tst
do
	j=`basename $i .tst`
	if (cmp -s $j.tst $j.bak)
	then	echo 'ok:'$j >>$ME.tmp
	fi
done
