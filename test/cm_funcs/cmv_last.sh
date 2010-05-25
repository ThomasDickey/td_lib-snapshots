#!/bin/sh
rm -f core
TOP=/mnt1/244/src
if ( make cmv_last )
then
	cmv_last $TOP/api/h/* >cmv_last.tmp
fi
