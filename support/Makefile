# $Id: Makefile,v 12.1 1994/07/04 23:53:53 tom Exp $
# make-file for TD_LIB support directory

####### (Development) ##########################################################
GET	= checkout
RM	= -rm -f

####### (Command-line Options) #################################################

####### (Standard Lists) #######################################################
SOURCES	= Makefile td_lib.mk

####### (Standard Productions) #################################################
all\
run_test:	$(SOURCES) $(ALL)
clean:			; $(RM) *.out
clobber:	clean	; $(RM) $(ALL)
destroy:		# will get here last
sources:	$(SOURCES)
lint.out:

####### (Details of Productions) ###############################################
#
$(SOURCES):		; $(GET) $@
