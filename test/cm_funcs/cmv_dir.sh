#!/bin/sh
rm -f core
TOP=/mnt1/244/src
if ( make cmv_dir )
then
	cmv_dir $TOP/* $TOP/tools/* $TOP/api/h/*
fi
