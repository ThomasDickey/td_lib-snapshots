#!/bin/sh
# $Id: walkback.sh,v 12.0 1992/11/24 14:28:42 ste_cm Rel $
./walkback | fgrep main |wc -l | sed -e 's@ @@g' >walkback.tmp
