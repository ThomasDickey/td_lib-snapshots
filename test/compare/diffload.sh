#!/bin/sh
# $Id: diffload.sh,v 12.0 1992/11/18 15:04:24 ste_cm Rel $
ME=diffload
./$ME dummy.txt dummy2.txt | sed -e 's@/usr/tmp.*$@@' >$ME.tmp
