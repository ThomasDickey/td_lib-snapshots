# $Id: Makefile,v 12.2 1994/07/10 21:25:55 tom Exp $
# make-file for TD_LIB support directory

####### (Development) ##########################################################
GET	= checkout
RM	= -rm -f

####### (Command-line Options) #################################################

####### (Standard Lists) #######################################################
SOURCES	= Makefile #td_lib.mk

####### (Standard Productions) #################################################
all\
run_test:	$(SOURCES) $(ALL)
clean:			; $(RM) *.out *.log
clobber:	clean	; $(RM) $(ALL)
destroy:		# will get here last
sources:	$(SOURCES)
lint.out:

####### (Details of Productions) ###############################################
#
$(SOURCES):		; $(GET) $@
