# $Id: Makefile,v 12.7 1995/10/20 10:50:53 tom Exp $
# MAKE-file for miscellaneous library routines

####### (Environment) ##########################################################
include ../td_defns.mk
include ../../support/td_lib.mk

####### (Standard Lists) #######################################################

CSRC	=\
	addchnst.c \
	beep.c \
	bindkeys.c \
	blip.c \
	cmdch.c \
	dumpchr.c \
	dumptty.c \
	dumpwin.c \
	erasechr.c \
	is_xterm.c \
	killchar.c \
	lnextchr.c \
	logch.c \
	on_winch.c \
	padedit.c \
	rawgets.c \
	rawterm.c \
	resizwin.c \
	savewin.c \
	scr_size.c \
	viewfile.c \
	win2file.c \
	wrepaint.c \
	wresize.c

SOURCES	= Makefile descrip.mms $(CSRC)

OBJS	=\
	$Z(addchnst.o) \
	$Z(beep.o) \
	$Z(bindkeys.o) \
	$Z(blip.o) \
	$Z(cmdch.o) \
	$Z(dumpchr.o) \
	$Z(dumptty.o) \
	$Z(dumpwin.o) \
	$Z(erasechr.o) \
	$Z(is_xterm.o) \
	$Z(killchar.o) \
	$Z(lnextchr.o) \
	$Z(logch.o) \
	$Z(on_winch.o) \
	$Z(padedit.o) \
	$Z(rawgets.o) \
	$Z(rawterm.o) \
	$Z(resizwin.o) \
	$Z(savewin.o) \
	$Z(scr_size.o) \
	$Z(viewfile.o) \
	$Z(win2file.o) \
	$Z(wrepaint.o) \
	$Z(wresize.o)

####### (Standard Productions) #################################################
include ../td_rules.mk

####### (Details of Productions) ###############################################
$Z(beep.o)\
$Z(cmdch.o)\
$Z(dumptty.o)\
$Z(dumpwin.o)\
$Z(erasechr.o)\
$Z(killchar.o)\
$Z(lnextchr.o)\
$Z(logch.o)\
$Z(on_winch.o)\
$Z(rawgets.o)\
$Z(rawterm.o)\
$Z(resizwin.o)\
$Z(savewin.o)\
$Z(viewfile.o)\
$Z(win2file.o)\
$Z(wrepaint.o)\
$Z(wresize.o):	$(CURSES_H)

$Z(rawgets.o): $I/dyn_str.h
