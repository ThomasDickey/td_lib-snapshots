# $Id: Makefile,v 12.6 1997/09/11 21:54:04 tom Exp $
# make-file for TD_LIB support directory

####### (Development) ##########################################################
RM	= -rm -f

####### (Command-line Options) #################################################

####### (Standard Lists) #######################################################
VMS_FILES = \
	lib_top.mms \
	linkopts.opt \
	prog_top.mms \
	td_lib.mms \
	td_progs.mms

SOURCES	= Makefile $(VMS_FILES)

ALL =

####### (Standard Productions) #################################################
all\
run_test:	$(SOURCES) $(ALL)
	@echo '** made $@'
clean:			; $(RM) *.out *.log
clobber:	clean	; $(RM) $(ALL)
destroy:		# will get here last
sources:	$(SOURCES)
lint.out:
