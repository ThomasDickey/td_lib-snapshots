# $Header: /users/source/archives/td_lib.vcs/RCS/Makefile,v 1.1 1989/03/21 16:25:41 dickey Exp $
# Top-level makefile for CM_TOOLS common library
#
# $Log: Makefile,v $
# Revision 1.1  1989/03/21 16:25:41  dickey
# Initial revision
#
#
####### (Development) ##########################################################
GET	= checkout

####### (Standard Lists) #######################################################
FIRST	=\
	lib\
	interface/Makefile\
	src/Makefile

####### (Standard Productions) #################################################
all\
clean\
clobber\
destroy\
sources\
lint.out:	$(FIRST)
	cd interface;	make $(MFLAGS) $@
	cd src;		make $(MFLAGS) $@

####### (Details of Productions) ###############################################
lib:			; mkdir $@
interface/Makefile:	; cd interface;	$(GET) Makefile
src/Makefile:		; cd src;	$(GET) Makefile
