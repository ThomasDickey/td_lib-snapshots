#!/bin/sh
# $Id: trnstree.sh,v 7.1 1992/11/20 10:36:08 dickey Exp $
./transtree -r ../support 2>&1 | fgrep -v make. >transtree.tmp
