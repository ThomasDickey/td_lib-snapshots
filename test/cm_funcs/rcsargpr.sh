#!/bin/sh
# $Id: rcsargpr.sh,v 11.2 1992/11/18 13:53:15 dickey Exp $
RCS_DIR=FOO; export RCS_DIR
(
./rcsargpair xxx
./rcsargpair xxx,v
./rcsargpair xxx xxx,v
./rcsargpair xxx xxx,v yyy
./rcsargpair xxx yyy
./rcsargpair xxx,v xxx yyy
) >rcsargpair.tmp
