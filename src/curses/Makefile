# $Id: Makefile,v 12.1 1994/07/02 14:43:51 tom Exp $
# MAKE-file for miscellaneous library routines

####### (Environment) ##########################################################
include ../td_defns.mk

####### (Standard Lists) #######################################################

CSRC	=\
	beep.c \
	bindkeys.c \
	blip.c \
	cmdch.c \
	dumpchr.c \
	dumpwin.c \
	erasechr.c \
	killchar.c \
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
	wrepaint.c

SOURCES	= Makefile descrip.mms $(CSRC)

OBJS	=\
	$Z(beep.o) \
	$Z(bindkeys.o) \
	$Z(blip.o) \
	$Z(cmdch.o) \
	$Z(dumpchr.o) \
	$Z(dumpwin.o) \
	$Z(erasechr.o) \
	$Z(killchar.o) \
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
	$Z(wrepaint.o)

####### (Standard Productions) #################################################
include ../td_rules.mk

####### (Details of Productions) ###############################################
$Z(beep.o)\
$Z(cmdch.o)\
$Z(dumpwin.o)\
$Z(erasechr.o)\
$Z(killchar.o)\
$Z(logch.o)\
$Z(on_winch.o)\
$Z(rawgets.o)\
$Z(rawterm.o)\
$Z(resizwin.o)\
$Z(savewin.o)\
$Z(viewfile.o)\
$Z(win2file.o)\
$Z(wrepaint.o):	$(CURSES_H)

$Z(rawgets.o): $I/dyn_str.h
