#!/bin/sh
# $Id: diffload.sh,v 12.1 1994/07/02 17:43:59 tom Exp $
ME=diffload
./$ME ../dummy.txt ../dummy2.txt | sed -e 's@/usr/tmp.*$@@' >$ME.tmp
