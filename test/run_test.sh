#!/bin/sh
# $Id: run_test.sh,v 11.4 1992/11/18 11:45:32 dickey Exp $
if test $# != 0
then
	echo '** '`date`
	for n in $*
	do
		N=`basename $n .ref`
		N=`basename $N .sh`
		rm -f $N.tmp
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
