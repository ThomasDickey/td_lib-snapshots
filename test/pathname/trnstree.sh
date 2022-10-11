#!/bin/sh
# $Id: trnstree.sh,v 12.2 2022/10/11 08:15:03 tom Exp $
ME=trnstree
./$ME -r ../../support 2>&1 | grep -v 'make[.]' >$ME.tmp
