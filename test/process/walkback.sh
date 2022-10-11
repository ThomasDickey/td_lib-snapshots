#!/bin/sh
# $Id: walkback.sh,v 12.1 2022/10/11 08:15:03 tom Exp $
./walkback | grep main |wc -l | sed -e 's@ @@g' >walkback.tmp
