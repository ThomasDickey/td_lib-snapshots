# $Id: Makefile,v 12.0 1993/04/26 16:35:58 ste_cm Rel $
# make-file for TD_LIB support directory

####### (Development) ##########################################################
GET	= checkout

####### (Command-line Options) #################################################

####### (Standard Lists) #######################################################
SOURCES	= Makefile td_lib.mk

####### (Standard Productions) #################################################
all\
run_test:	$(SOURCES) $(ALL)
clean:			; rm -f *.out
clobber:	clean	; rm -f $(ALL)
destroy:		# will get here last
sources:	$(SOURCES)
lint.out:

####### (Details of Productions) ###############################################
#
$(SOURCES):		; $(GET) $@
