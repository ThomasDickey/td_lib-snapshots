#!/bin/sh
# $Id: gethome.sh,v 12.0 1992/11/18 11:49:46 ste_cm Rel $
WD=$HOME
HOME=/;export HOME
./gethome | sed -e s@$WD@HOME@ >gethome.tmp
