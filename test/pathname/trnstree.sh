#!/bin/sh
# $Id: trnstree.sh,v 12.1 1994/08/21 21:55:19 tom Exp $
ME=trnstree
./$ME -r ../../support 2>&1 | fgrep -v make. >$ME.tmp
