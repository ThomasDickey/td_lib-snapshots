#!/bin/sh
# $Id: cutoff.sh,v 12.0 1992/11/18 11:40:51 ste_cm Rel $
OUT=cutoff.tmp
./cutoff -c89/12/1 12:13:14 dummy  >$OUT
./cutoff -c89/6/1  12:13:14 dummy >>$OUT
./cutoff -c89.12.1 12.13.14 dummy >>$OUT
./cutoff -c891201121314     dummy >>$OUT
