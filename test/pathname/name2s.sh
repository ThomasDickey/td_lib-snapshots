#!/bin/sh
# $Id: name2s.sh,v 12.0 1992/11/18 12:28:27 ste_cm Rel $
OUT=name2s.tmp
rm -f $OUT
for n in .dot 'Space\ b' 'Tab\	b' '\ Space' README
do
	./name2s "$n" >>$OUT
	./name2s -e "$n" >>$OUT
	./name2s -u "$n" >>$OUT
	./name2s -eu "$n" >>$OUT
done
