# $Id: old-td_lib.mk,v 11.4 1992/10/15 08:58:02 dickey Exp $
# common definitions for makefiles built over CM_TOOLS library.

####### (Environment) ##########################################################
CM_LIB	= cm_library
# TOP	= ../../.. -- must define

B	= ../bin
I	= $(TOP)/$(CM_LIB)/interface
L	= $(TOP)/$(CM_LIB)/lib

GET	= checkout
COPY	= cp -p
PUT	= rm -f $@; $(COPY) $? $@

MAKE	= make $(MFLAGS) -k$(MAKEFLAGS)	CFLAGS="$(CFLAGS)" COPY="$(COPY)"

INSTALL_BIN = $(TOP)/install_bin
INSTALL_LIB = $(TOP)/install_lib
INSTALL_MAN = $(TOP)/install_man

####### (Command-line Options) #################################################
INCLUDES= -I. -I$I
CPP_OPTS= $(DEFINES) $(INCLUDES)

LIBS	= $L/$(CM_LIB).a
DATE	= echo '** '`date` >> $@

LINTLIBS= -ltd
LINTOPTS= $(CPP_OPTS) $(LINTLIBS)

####### (Standard Lists) #######################################################
CLEAN	= *.[oai] *.bak *.log *.out *.tst .nfs* core
DESTROY	=sh -c 'for i in *;do case $$i in RCS);; *) rm -f $$i;;esac;done;exit 0'
RUN_TESTS=sh -c '$@.sh 2>&1 | tee -a $@.out'

PTYPES_H =	$I/ptypes.h	$I/common.h
RCSDEFS_H =	$(PTYPES_H)	$I/rcsdefs.h	$I/deltree.h
SCCSDEFS_H =	$(PTYPES_H)	$I/sccsdefs.h

####### (Development) ##########################################################
CPROTO	= cproto -e -i -fo'\\n\\t\\t' -fp'\\n\\t\\t'

LINT_EACH = sh -c 'for i in $?;do echo $$i:; tdlint $(LINTOPTS) $$i >>$@;done'

.SUFFIXES: .c .i .o .a .proto .lint .tst

.c.o:		; $(CC) $(CPP_OPTS) $(CFLAGS) -c $<
.c.a:
	@echo compile $<
	@$(CC) $(CFLAGS) $(CPP_OPTS) -c $*.c
	@ar rv $@ $*.o
	@rm -f $*.o
.c.i:		; $(CC) $(CPP_OPTS) -E -C $< >$@
.c.proto:	; $(CPROTO) $(CPP_OPTS) $< >$@
.c.lint:	; tdlint $(LINTOPT) $< >$@

.c.tst:		; $(CC) -o $@ -DTEST $(CFLAGS) $(CPP_OPTS) $< $Z $(LIBS)
