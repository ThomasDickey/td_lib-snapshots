# $Id: Makefile,v 11.3 1992/12/04 11:53:08 ste_cm Exp $
# make-file for CM_LIBRARY support directory

####### (Development) ##########################################################
GET	= checkout

####### (Command-line Options) #################################################

####### (Standard Lists) #######################################################
SOURCES	= Makefile cm_library.mk

####### (Standard Productions) #################################################
all\
run_tests:	$(SOURCES) $(ALL)
clean:			; rm -f *.out
clobber:	clean	; rm -f $(ALL)
destroy:		# will get here last
sources:	$(SOURCES)
lint.out:

####### (Details of Productions) ###############################################
#
$(SOURCES):		; $(GET) $@
