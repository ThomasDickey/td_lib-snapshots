# $Id: Makefile,v 11.1 1992/10/09 11:38:59 dickey Exp $
# make-file for CM_LIBRARY support directory

####### (Development) ##########################################################
GET	= checkout

####### (Command-line Options) #################################################

####### (Standard Lists) #######################################################
SOURCES	= Makefile cm_library.mk

####### (Standard Productions) #################################################
all:	sources $(ALL)
clean:			; rm -f *.out
clobber:	clean	; rm -f $(ALL)
destroy:
	sh -c 'for i in *;do case $$i in RCS);; *) rm -f $$i;;esac;done;exit 0'
sources:	$(SOURCES)
lint.out:

####### (Details of Productions) ###############################################
#
$(SOURCES):		; $(GET) $@
