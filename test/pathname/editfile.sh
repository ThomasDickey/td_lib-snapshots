#!/bin/sh
# $Id: editfile.sh,v 12.2 2024/04/21 20:24:18 tom Exp $
#
ME=editfile
rm -f *.tst *.bak
trap "rm -f *.tst *.bak; exit 1" HUP INT QUIT TERM
trap "rm -f *.tst *.bak" EXIT
for i in ../../support/*.mms
do	j=`basename $i .mms`
	cp $i ./$j.tst
	cp $i ./$j.bak
	chmod u+w $j.bak
	echo LAST >>$j.bak
done
#
./editfile ../pathname/*.tst 2>&1 | sed -e 's@"file.*"@TMP@' >$ME.tmp
#
for i in *.tst
do
	j=`basename $i .tst`
	if (cmp -s $j.tst $j.bak)
	then	echo 'ok:'$j >>$ME.tmp
	else	diff $j.tst $j.bak >>$ME.tmp
	fi
done
