#!/bin/sh
# $Id: selflink.sh,v 12.1 2004/03/07 15:33:00 tom Exp $
# Ensure that the "td_lib" name points to the top of this directory tree,
# making it simple to build ded, cm_tools and sccs_tools side-by-side.
# A symbolic link is acceptable for this purpose.
CDPATH=
case $PWD in
*/td_lib)
	;;
*)
	test -d ../td_lib	&& rm -f ../td_lib
	test -d ../td_lib 	&& exit 1
	ln -s $PWD ../td_lib	|| exit 1
	;;
esac
