# $Id: old-td_lib.mk,v 11.9 1993/04/26 15:36:49 dickey Exp $
# common definitions for makefiles built over TD_LIB library.

####### (Environment) ##########################################################
TD_LIB	= td_lib
# TOP	= ../../.. -- must define

B	= ../bin
I	= $(TOP)/$(TD_LIB)/include
L	= $(TOP)/$(TD_LIB)/lib

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

LIBS	= $L/$(TD_LIB).a
DATE	= echo '** '`date` >> $@

LINTLIBS= -ltd
LINTOPTS= $(CPP_OPTS) $(LINTLIBS)

####### (Standard Lists) #######################################################
CLEAN	= *.[oai] *.lint *.bak *.log *.out *.tst .nfs* core
DESTROY	=sh -c 'for i in *;do case $$i in RCS);; *) rm -f $$i;;esac;done;exit 0'
RUN_TESTS=sh -c '$@.sh 2>&1 | tee -a $@.out'

PTYPES_H =	$I/ptypes.h	$I/td_lib.h
CURSES_H =	$(PTYPES_H)	$I/td_curse.h
QSORT_H =	$(PTYPES_H)	$I/td_qsort.h
RCSDEFS_H =	$(PTYPES_H)	$I/rcsdefs.h	$I/deltree.h
SCCSDEFS_H =	$(PTYPES_H)	$I/sccsdefs.h

####### (Development) ##########################################################
CPROTO	= cproto -e -i -fo'\\n\\t\\t' -fp'\\n\\t\\t'

LINT_EACH = sh -c 'for i in $?;do echo $$i:; tdlint $(LINTOPTS) $$i >>$@;done'

.SUFFIXES: .c .o .a

.c.o:		; $(CC) $(CPP_OPTS) $(CFLAGS) -c $<
.c.a:
	@echo compile $<
	@$(CC) $(CFLAGS) $(CPP_OPTS) -c $*.c
	@ar rv $@ $*.o
	@rm -f $*.o

.SUFFIXES: .i .proto .lint .tst

.c.i:		; $(CC) $(CPP_OPTS) -E -C $< >$@
.c.proto:	; $(CPROTO) $(CPP_OPTS) $< >$@
.c.lint:	; tdlint $(LINTOPTS) $< >$@

.c.tst:		; $(CC) -o $@ -DTEST $(CFLAGS) $(CPP_OPTS) $< $Z $(LIBS)

.SUFFIXES: .man .cat

.man.cat:
	rm -f $@
	nroff -man $? >$@
