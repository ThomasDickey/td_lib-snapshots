#!/bin/sh
# $Id: strucmp.sh,v 12.0 1992/11/18 13:24:18 ste_cm Rel $
OUT=strucmp.tmp
./strucmp aaa AAA aAa >>$OUT
./strucmp baa bAA aAa bAa caa >>$OUT
