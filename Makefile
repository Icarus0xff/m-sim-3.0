#
# SimpleScalar(TM) Tool Suite
# Copyright (C) 1994-2003 by Todd M. Austin, Ph.D. and SimpleScalar, LLC.
# All Rights Reserved. 
# 
#
# MODIFIED BY: JASON LOEW - APRIL 2010
# MODIFIED BY: JOSEPH SHARKEY - SEPTEMBER 2005
#
#
#
# THIS IS A LEGAL DOCUMENT, BY USING SIMPLESCALAR,
# YOU ARE AGREEING TO THESE TERMS AND CONDITIONS.
# 
# No portion of this work may be used by any commercial entity, or for any
# commercial purpose, without the prior, written permission of SimpleScalar,
# LLC (info@simplescalar.com). Nonprofit and noncommercial use is permitted
# as described below.
# 
# 1. SimpleScalar is provided AS IS, with no warranty of any kind, express
# or implied. The user of the program accepts full responsibility for the
# application of the program and the use of any results.
# 
# 2. Nonprofit and noncommercial use is encouraged. SimpleScalar may be
# downloaded, compiled, executed, copied, and modified solely for nonprofit,
# educational, noncommercial research, and noncommercial scholarship
# purposes provided that this notice in its entirety accompanies all copies.
# Copies of the modified software can be delivered to persons who use it
# solely for nonprofit, educational, noncommercial research, and
# noncommercial scholarship purposes provided that this notice in its
# entirety accompanies all copies.
# 
# 3. ALL COMMERCIAL USE, AND ALL USE BY FOR PROFIT ENTITIES, IS EXPRESSLY
# PROHIBITED WITHOUT A LICENSE FROM SIMPLESCALAR, LLC (info@simplescalar.com).
# 
# 4. No nonprofit user may place any restrictions on the use of this software,
# including as modified by the user, by any other authorized user.
# 
# 5. Noncommercial and nonprofit users may distribute copies of SimpleScalar
# in compiled or executable form as set forth in Section 2, provided that
# either: (A) it is accompanied by the corresponding machine-readable source
# code, or (B) it is accompanied by a written offer, with no time limit, to
# give anyone a machine-readable copy of the corresponding source code in
# return for reimbursement of the cost of distribution. This written offer
# must permit verbatim duplication by anyone, or (C) it is distributed by
# someone who received only the executable form, and is accompanied by a
# copy of the written offer of source code.
# 
# 6. SimpleScalar was developed by Todd M. Austin, Ph.D. The tool suite is
# currently maintained by SimpleScalar LLC (info@simplescalar.com). US Mail:
# 2395 Timbercrest Court, Ann Arbor, MI 48105.
# 
# Copyright (C) 1994-2003 by Todd M. Austin, Ph.D. and SimpleScalar, LLC.
#


##################################################################
#
# Modify the following definitions to suit your build environment,
# NOTE: most platforms should not require any changes
#
##################################################################

#
# Define below C compiler and flags, machine-specific flags and libraries,
# build tools and file extensions, these are specific to a host environment,
# pre-tested environments follow...
#

##
## vanilla Unix, G++ build
##
## NOTE: the SimpleScalar simulators must be compiled with an ANSI C
## compatible compiler.
##
## tested hosts:
##
##	Ubuntu 10.04.2 LTS x86_64, GNU GCC version Ubuntu 4.4.3-4ubuntu5
##	Debian GNU/Linux 6.0 x86_64, GNU GCC version Debian 4.4.5-8
##
CC = g++
#OFLAGS = -g -O0 -Wall
OFLAGS = -g -O4 -Wall
MFLAGS = `./sysprobe -flags`
MLIBS  = `./sysprobe -libs` -lm
ENDIAN = `./sysprobe -s`
MAKE = make
AR = ar qcv
AROPT =
RANLIB = ranlib
RM = rm -f
RMDIR = rm -f
LN = ln -s
LNDIR = ln -s
DIFF = diff
OEXT = o
LEXT = a
EEXT =
CS = ;
X=/
#setenv MALLOC_CHECK_ 1

#
# Compilation-specific feature flags
#
# -DDEBUG	- turns on debugging features
#
FFLAGS = -DDEBUG

##################################################################
#
# YOU SHOULD NOT NEED TO MODIFY ANYTHING BELOW THIS COMMENT
#
##################################################################

#
# complete flags
#
CFLAGS = $(MFLAGS) $(FFLAGS) $(OFLAGS)

#
# all the sources
#
SRCS =	main.c sim-outorder.c \
	memory.c regs.c cache.c bpred.c ptrace.c \
	resource.c endian.c dlite.c symbol.c eval.c options.c range.c \
	eio.c stats.c endian.c misc.c smt.c power.c\
	regrename.c rob.c cmp.c iq.c dram.c file_table.c \
	bpred_not_taken.c bpred_taken.c bpred_two_level.c bpred_combining.c bpred_bimodal.c btb.c retstack.c \
	pid.c syscall.c syscall_obj.c

HDRS =	syscall.h memory.h regs.h sim.h loader.h cache.h bpred.h ptrace.h \
	resource.h endian.h dlite.h symbol.h eval.h \
	eio.h range.h version.h endian.h misc.h smt.h rob.h regrename.h iq.h power.h\
	cmp.h sim-outorder.h dram.h file_table.h\
	bpred_not_taken.c bpred_taken.c bpred_two_level.c bpred_combining.c bpred_bimodal.c bpreds.h btb.h retstack.h \
	ecoff.h elf.h pid.h syscall_obj.h


#
# common objects
#
OBJS =	main.$(OEXT) syscall.$(OEXT) memory.$(OEXT) regs.$(OEXT) \
	loader.$(OEXT) endian.$(OEXT) dlite.$(OEXT) symbol.$(OEXT) \
	eval.$(OEXT) options.$(OEXT) stats.$(OEXT) eio.$(OEXT)\
	range.$(OEXT) misc.$(OEXT) machine.$(OEXT) power.$(OEXT)\
	dram.$(OEXT) file_table.$(OEXT) \
	bpred_not_taken.$(OEXT) bpred_taken.$(OEXT) bpred_two_level.$(OEXT) bpred_combining.$(OEXT) bpred_bimodal.$(OEXT) btb.$(OEXT) retstack.$(OEXT) \
	pid.$(OEXT) syscall_obj.$(OEXT)


#
# all targets, NOTE: library ordering is important...
#
all: sim-outorder
	@echo "my work is done here..."

dram:
	$(CC) -o dram.exe dram.c -DDRAM_DEBUG -DTEST

#dumper:
#	$(CC) -c code_dump.exe
#	$(CC) smt.o loader.o memory.o code_dump.o machine.o eio.o endian.o misc.o regs.o file_table.o rob.o -o code_dump.exe

sysprobe$(EEXT):	sysprobe.c
	$(CC) $(FFLAGS) -o sysprobe$(EEXT) sysprobe.c
	@echo endian probe results: $(ENDIAN)
	@echo probe flags: $(MFLAGS)
	@echo probe libs: $(MLIBS)

#wattch-interact$(EEXT): sysprobe$(EEXT) eval.o misc.o stats.o power.o wattch-interact.o cacti/libcacti.$(LEXT) cache.o
#	$(CC) eval.o misc.o stats.o power.o wattch-interact.o cacti/libcacti.$(LEXT) cache.o $(CFLAGS) -lc -lm -o wattch-interact

power$(EEXT):           power.c
	$(CC) $(SAFECFLAGS) -c power.c

sim-outorder$(EEXT):	sysprobe$(EEXT) sim-outorder.$(OEXT) smt.$(OEXT) rob.$(OEXT) cmp.$(OEXT) cache.$(OEXT) iq.$(OEXT) bpred.$(OEXT) regrename.$(OEXT) resource.$(OEXT) ptrace.$(OEXT) $(OBJS) cacti/libcacti.$(LEXT)
	$(CC) -o sim-outorder$(EEXT) $(CFLAGS) sim-outorder.$(OEXT) smt.$(OEXT) rob.$(OEXT) cmp.$(OEXT) cache.$(OEXT) iq.$(OEXT) bpred.$(OEXT) regrename.$(OEXT) resource.$(OEXT) ptrace.$(OEXT) $(OBJS) cacti/libcacti.$(LEXT) $(MLIBS)

cacti cacti/libcacti.$(LEXT): sysprobe$(EEXT)
	cd cacti $(CS) \
	$(MAKE) "MAKE=$(MAKE)" "CC=$(CC)" "AR=$(AR)" "AROPT=$(AROPT)" "RANLIB=$(RANLIB)" "CFLAGS=$(MFLAGS) $(FFLAGS) $(SAFEOFLAGS)" "OEXT=$(OEXT)" "LEXT=$(LEXT)" "EEXT=$(EEXT)" "X=$(X)" "RM=$(RM)" libcacti.$(LEXT)


.c.$(OEXT):
	$(CC) $(CFLAGS) -c $*.c

filelist:
	@echo $(SRCS) $(HDRS) Makefile

diffs:
	-rcsdiff RCS/*
	-cd config; rcsdiff RCS/*

clean:
	-$(RM) *.o *.obj *.exe core *~ MAKE.log Makefile.bak sysprobe$(EEXT) sim-outorder
	cd cacti $(CS) $(MAKE) "RM=$(RM)" "CS=$(CS)" clean $(CS) cd ..

depend:
	makedepend.local -n -x $(BINUTILS_INC) $(SRCS)


# DO NOT DELETE THIS LINE -- make depend depends on it.

main.$(OEXT): host.h misc.h machine.h machine.def.c endian.h version.h dlite.h
main.$(OEXT): regs.h memory.h options.h stats.h eval.h loader.h sim.h smt.h rob.h
sim-outorder.$(OEXT): host.h misc.h machine.h machine.def.c regs.h memory.h
sim-outorder.$(OEXT): options.h stats.h eval.h cache.h iq.h loader.h syscall.h
sim-outorder.$(OEXT): bpred.h regrename.h resource.h ptrace.h range.h dlite.h
sim-outorder.$(OEXT): sim.h smt.h iq.h regrename.h rob.h cache.h
sim-outorder.$(OEXT): inflightq.h cmp.h sim-outorder.h dram.h bpreds.h pid.h
dram.$(OEXT): dram.h host.h machine.h
memory.$(OEXT): host.h misc.h machine.h machine.def.c options.h stats.h eval.h
memory.$(OEXT): memory.h
regs.$(OEXT): host.h misc.h machine.h machine.def.c loader.h regs.h memory.h
regs.$(OEXT): options.h stats.h eval.h
smt.$(OEXT): smt.h regs.h host.h misc.h machine.h loader.h rob.h bpred.h fetchtorename.h
smt.$(OEXT): regrename.h bpreds.h file_table.h
cmp.$(OEXT): smt.h iq.h power.h inflightq.h resource.h ptrace.h rob.h dram.h bpreds.h
cache.$(OEXT): host.h misc.h machine.h machine.def.c cache.h memory.h options.h
cache.$(OEXT): stats.h eval.h
iq.$(OEXT): iq.h
bpred.$(OEXT): host.h misc.h machine.h machine.def.c bpred.h stats.h eval.h
rob.$(OEXT): bpred.h regs.h rob.h bpreds.h
regrename.$(OEXT): rob.h
ptrace.$(OEXT): host.h misc.h machine.h machine.def.c range.h ptrace.h
resource.$(OEXT): host.h misc.h resource.h
endian.$(OEXT): endian.h loader.h host.h misc.h machine.h machine.def.c regs.h
endian.$(OEXT): memory.h options.h stats.h eval.h
dlite.$(OEXT): host.h misc.h machine.h machine.def.c version.h eval.h regs.h
dlite.$(OEXT): memory.h options.h stats.h sim.h symbol.h loader.h range.h
dlite.$(OEXT): dlite.h
symbol.$(OEXT): host.h misc.h ecoff.h loader.h machine.h
symbol.$(OEXT): machine.def.c regs.h memory.h options.h stats.h eval.h symbol.h
eval.$(OEXT): host.h misc.h eval.h machine.h machine.def.c
options.$(OEXT): host.h misc.h options.h
range.$(OEXT): host.h misc.h machine.h machine.def.c symbol.h loader.h regs.h
range.$(OEXT): memory.h options.h stats.h eval.h range.h
eio.$(OEXT): host.h misc.h machine.h machine.def.c regs.h memory.h options.h
eio.$(OEXT): stats.h eval.h loader.h host.h misc.h machine.h
eio.$(OEXT): syscall.h sim.h endian.h eio.h
stats.$(OEXT): host.h misc.h machine.h machine.def.c eval.h stats.h
endian.$(OEXT): endian.h loader.h host.h misc.h machine.h machine.def.c regs.h
endian.$(OEXT): memory.h options.h stats.h eval.h
misc.$(OEXT): host.h misc.h machine.h machine.def.c
power.$(OEXT): host.h misc.h machine.h machine.def.c cache.h memory.h options.h
power.$(OEXT): stats.h eval.h power.h
loader.$(OEXT): host.h misc.h machine.h machine.def.c endian.h regs.h memory.h
loader.$(OEXT): options.h stats.h eval.h sim.h eio.h loader.h
loader.$(OEXT): elf.h ecoff.h
syscall.$(OEXT): host.h misc.h machine.h machine.def.c regs.h memory.h
syscall.$(OEXT): options.h stats.h eval.h loader.h sim.h eio.h endian.h
syscall.$(OEXT): syscall.h smt.h file_table.h syscall_obj.h
syscall_obj.$(OEXT): syscall_obj.h
symbol.$(OEXT): host.h misc.h ecoff.h loader.h machine.h
symbol.$(OEXT): machine.def.c regs.h memory.h options.h stats.h eval.h symbol.h
alpha.$(OEXT): host.h misc.h machine.h machine.def.c eval.h regs.h
loader.$(OEXT): host.h misc.h machine.h machine.def.c endian.h regs.h memory.h
loader.$(OEXT): options.h stats.h eval.h sim.h eio.h loader.h
loader.$(OEXT): ecoff.h 
syscall.$(OEXT): host.h misc.h machine.h machine.def.c regs.h memory.h
syscall.$(OEXT): options.h stats.h eval.h loader.h sim.h eio.h endian.h
syscall.$(OEXT): syscall.h pid.h
symbol.$(OEXT): host.h misc.h loader.h machine.h machine.def.c regs.h memory.h
symbol.$(OEXT): options.h stats.h eval.h symbol.h ecoff.h
bpred_taken.$(OEXT): bpred_taken.c bpred_taken.h bpred.h
bpred_not_taken.$(OEXT): bpred_taken.c bpred_taken.h bpred.h
bpred_not_taken.$(OEXT): bpred_not_taken.c bpred_not_taken.h bpred.h
bpred_bimodal.$(OEXT): bpred_bimodal.c bpred_bimodal.h bpred.h
bpred_two_level.$(OEXT): bpred_two_level.c bpred_two_level.h bpred.h
bpred_combining.$(OEXT): bpred_combining.c bpred_combining.h bpred.h bpred_bimodal.h bpred_two_level.h
btb.$(OEXT): btb.c btb.h
retstack.$(OEXT): retstack.c retstack.h
file_table.$(OEXT): file_table.h file_table.c machine.h
pid.$(OEXT): pid.h pid.c
