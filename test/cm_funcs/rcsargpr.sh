#!/bin/sh
# $Id: rcsargpr.sh,v 12.0 1993/04/27 08:04:52 ste_cm Rel $
ME=rcsargpr
RCS_DIR=FOO; export RCS_DIR
(
./$ME xxx
./$ME xxx,v
./$ME xxx xxx,v
./$ME xxx xxx,v yyy
./$ME xxx yyy
./$ME xxx,v xxx yyy
) >$ME.tmp
