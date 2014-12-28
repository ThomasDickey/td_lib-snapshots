#!/bin/sh
# $Id: run_test.sh,v 12.2 2014/12/28 01:42:43 tom Exp $
if test $# != 0
then
	echo '** '`date`
	rm -f *.tmp
	for n in $*
	do
		N=`basename $n .ref`
		N=`basename $N .sh`
		LOG=../$N.purelog
		rm -f $N.tmp $LOG
		if ( ./$N.sh )
		then
			if test -f $N.tmp
			then
				if test -f $N.ref
				then
					if ( cmp -s $N.tmp $N.ref )
					then	echo '** ok:  '$N
						rm -f $N.tmp
					else	echo '?? fail:'$N
						diff $N.ref $N.tmp
					fi
				else
					mv $N.tmp $N.ref
				fi
			fi
		fi
	done
else
	$0 *.ref
fi
