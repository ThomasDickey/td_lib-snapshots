# $Id: Makefile,v 11.2 1992/10/16 12:25:20 dickey Exp $
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
destroy:		# will get here last
sources:	$(SOURCES)
lint.out:

####### (Details of Productions) ###############################################
#
$(SOURCES):		; $(GET) $@
