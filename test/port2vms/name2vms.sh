#!/bin/sh
# $Id: name2vms.sh,v 8.0 1993/04/29 13:41:55 ste_cm Rel $
./name2vms |sed -e s@$HOME@{HOME}@g >name2vms.tmp
