#!/bin/sh
# $Id: vms2name.sh,v 8.0 1992/11/20 10:03:48 ste_cm Rel $
WD=`pwd`
./vms2name | sed -e s@$WD@WD@ >vms2name.tmp
