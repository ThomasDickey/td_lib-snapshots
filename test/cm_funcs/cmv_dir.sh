#!/bin/sh
# $Id: cmv_dir.sh,v 12.2 2010/05/24 23:18:28 tom Exp $
rm -f core
TOP=/mnt1/244/src
if ( make cmv_dir )
then
	cmv_dir $TOP/* $TOP/tools/* $TOP/api/h/* >cmv_dir.tmp
fi
