#!/bin/sh
# $Id: trnstree.sh,v 8.0 1993/04/30 11:20:22 ste_cm Rel $
ME=trnstree
./$ME -r ../support 2>&1 | fgrep -v make. >$ME.tmp
