#!/bin/sh
# $Id: which.sh,v 12.0 1992/11/18 12:44:47 ste_cm Rel $
./which $0 ./which | sed -e s@`pwd`@PWD@ >which.tmp
