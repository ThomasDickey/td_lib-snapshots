#!/bin/sh
# $Id: pathhead.sh,v 12.1 1994/07/02 18:33:45 tom Exp $
ME=pathhead
THIS=pathname
./$ME ./$ME \
	../$THIS/$ME \
	. \
	.. \
	../ \
	../Makefile \
	../$THIS/$ME \
	../src \
	../.. >$ME.tmp
