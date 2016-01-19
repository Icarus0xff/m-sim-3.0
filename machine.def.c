//This doesn't look like -*- C -*-, but it is!

//alpha.def - Alpha ISA machine definition

/* SimpleScalar(TM) Tool Suite
 * Copyright (C) 1994-2003 by Todd M. Austin, Ph.D. and SimpleScalar, LLC.
 * All Rights Reserved.
 *
 * THIS IS A LEGAL DOCUMENT, BY USING SIMPLESCALAR, YOU ARE AGREEING TO THESE TERMS AND CONDITIONS.
 *
 * No portion of this work may be used by any commercial entity, or for any commercial purpose, without the prior, written permission of SimpleScalar,
 * LLC (info@simplescalar.com). Nonprofit and noncommercial use is permitted as described below.
 *
 * 1. SimpleScalar is provided AS IS, with no warranty of any kind, express or implied. The user of the program accepts full responsibility for the
 * application of the program and the use of any results.
 *
 * 2. Nonprofit and noncommercial use is encouraged. SimpleScalar may be downloaded, compiled, executed, copied, and modified solely for nonprofit,
 * educational, noncommercial research, and noncommercial scholarship purposes provided that this notice in its entirety accompanies all copies.
 * Copies of the modified software can be delivered to persons who use it solely for nonprofit, educational, noncommercial research, and
 * noncommercial scholarship purposes provided that this notice in its entirety accompanies all copies.
 *
 * 3. ALL COMMERCIAL USE, AND ALL USE BY FOR PROFIT ENTITIES, IS EXPRESSLY PROHIBITED WITHOUT A LICENSE FROM SIMPLESCALAR, LLC (info@simplescalar.com).
 *
 * 4. No nonprofit user may place any restrictions on the use of this software, including as modified by the user, by any other authorized user.
 *
 * 5. Noncommercial and nonprofit users may distribute copies of SimpleScalar in compiled or executable form as set forth in Section 2, provided that
 * either: (A) it is accompanied by the corresponding machine-readable source code, or (B) it is accompanied by a written offer, with no time limit, to
 * give anyone a machine-readable copy of the corresponding source code in return for reimbursement of the cost of distribution. This written offer
 * must permit verbatim duplication by anyone, or (C) it is distributed by someone who received only the executable form, and is accompanied by a
 * copy of the written offer of source code.
 *
 * 6. SimpleScalar was developed by Todd M. Austin, Ph.D. The tool suite is currently maintained by SimpleScalar LLC (info@simplescalar.com). US Mail:
 * 2395 Timbercrest Court, Ann Arbor, MI 48105.
 *
 * Copyright (C) 1994-2003 by Todd M. Austin, Ph.D. and SimpleScalar, LLC.
 */


// FIXME: these comments are out-of-date

/* This file defines all aspects of the SimpleScalar instruction set architecture.  Each instruction set in the architecture has a DEFINST()
 * macro call included below.  The contents of a instruction definition are as follows:
 *
 *   DEFINST(<enum>,			<opcode>,
 *	     <opname>,			<operands>,
 *	     <fu_req>,			<iflags>,
 *	     <output deps...>,		<input deps...>,
 *	     <expr>)
 *
 * Where:
 *
 *   <enum>	- is an enumerator that is returned when an instruction is decoded by SS_OP_ENUM()
 *   <opcode>	- is the opcode of this instruction
 *   <opname>   - name of this instruction as a string, used by disassembler
 *   <operands>	- specified the instruction operand fields and their printed order for disassembly, used by disassembler, the recognized
 *		  operand field are (the instruction format is detailed in the header file machine.h):
 *		    J - target field
 *		    j - PC relative target (offset + PC)
 *		    s - S register field
 *		    b - S register field (base register)
 *		    t - T register field
 *		    d - D register field
 *		    S - S register field (FP register)
 *		    T - T register field (FP register)
 *		    D - D register field (FP register)
 *		    o - load address offset (offset)
 *		    i - signed immediate field value
 *		    u - unsigned immediate field value
 *		    U - upper immediate field value
 *		    H - shift amount immediate field value
 *		    B - break code
 *
 *   <fu_req>	- functional unit requirements for this instruction
 *   <iflags>	- instruction flags, accessible via the SS_OP_FLAGS() macro, flags are defined with F_* prefix in machine.h
 *   <output deps...>
 *		- a list of up to two output dependency designators, the following designators are recognized (place an DNA in any
 *		  unused fields:
 *		    DGPR(N)   - general purpose register N
 *		    DGPR_D(N) - double word general purpose register N
 *		    DCGPR(N)  - general purpose register conditional on
 *			        pre/post- increment/decrement mode
 *		    DFPR_L(N) - floating-point register N, as word
 *		    DFPR_F(N) - floating-point reg N, as single-prec float
 *		    DFPR_D(N) - floating-point reg N, as double-prec double
 *		    DHI	      - HI result register
 *		    DLO	      - LO result register
 *		    DFCC      - floating point condition codes
 *		    DCPC      - current PC
 *		    DNPC      - next PC
 *		    DNA	      - no dependence
 *
 *   <input deps...>
 *		- a list of up to three input dependency designators, the designators are defined above (place an DNA in any unused fields.
 *
 *   <expr>	- a C expression that implements the instruction being defined, the expression must modify all architected state
 *		  affected by the instruction's execution, by default, the next PC (NPC) value defaults to the current PC (CPC) plus
 *		  SS_INST_SIZE, as a result, only taken branches need to set NPC
 *
 *		  The following predefined macros are available for use in DEFINST() instruction expressions to access the value of
 *		  instruction operand/opcode field values:
 *
 *		    RS	    - RS register field value
 *		    RT	    - RT register field value
 *		    RD	    - RD register field value
 *		    FS	    - RS register field value
 *		    FT	    - RT register field value
 *		    FD	    - RD register field value
 *		    BS	    - RS register field value
 *		    TARG    - jump target field value
 *		    OFS	    - signed offset field value
 *		    IMM	    - signed offset field value
 *		    UIMM    - unsigned offset field value
 *		    SHAMT   - shift amount field value
 *		    BCODE   - break code field value
 *
 *		  To facilitate the construction of performance simulators (which may want to specialize their architected state
 *		  storage format), all architected register and memory state is accessed through the following macros:
 *
 *		    GPR(N)         - read general purpose register N
 *		    SET_GPR(N,E)   - write general purpose register N with E
 *		    GPR_D(N)       - read double word general purpose reg N
 *		    SET_GPR_D(N,E) - write double word gen purpose reg N w/ E
 *		    FPR_L(N)       - read floating-point register N, as word
 *		    SET_FPR_L(N,E) - floating-point reg N, as word, with E
 *		    FPR_F(N)       - read FP reg N, as single-prec float
 *		    SET_FPR_F(N,E) - write FP reg N, as single-prec float w/ E
 *		    FPR_D(N)       - read FP reg N, as double-prec double
 *		    SET_FPR_D(N,E) - write FP reg N, as double-prec double w/E
 *		    HI	           - read HI result register
 *		    SET_HI(E)      - write HI result register with E
 *		    LO	           - read LO result register
 *		    SET_LO(E)      - write LO result register with E
 *		    FCC	           - read floating point condition codes
 *		    SET_FCC(E)     - write floating point condition codes w/ E
 *		    CPC	           - read current PC register
 *		    NPC	           - read next PC register
 *		    SET_NPC(E)     - write next PC register with E
 *		    TPC	           - read target PC register
 *		    SET_TPC(E)     - write target PC register with E
 *
 *		    READ_SIGNED_BYTE(A)   - read signed byte from address A
 *		    READ_UNSIGNED_BYTE(A) - read unsigned byte from address A
 *		    READ_SIGNED_HALF(A)   - read signed half from address A
 *		    READ_UNSIGNED_HALF(A) - read unsigned half from address A
 *		    READ_WORD(A)          - read word from address A
 *		    WRITE_BYTE(E,A)       - write byte value E to address A
 *		    WRITE_HALF(E,A)       - write half value E to address A
 *		    WRITE_WORD(E,A)       - write word value E to address A
 *
 *		  Finally, the following helper functions are available to assist in the construction of instruction expressions:
 *
 *		    INC_DEC(E,N,S) - execute E and update N as per pre/post-incr/decr addressing sementics for an access of S bytes
 *		    OVER(X,Y)      - check for overflow for X+Y, both signed
 *		    UNDER(X,Y)	   - check for umderflow for X-Y, both signed
 *		    DIV0(N)	   - check for divide by zero, N is denom
 *		    INTALIGN(N)    - check double word int reg N alignment
 *		    FPALIGN(N)	   - check double word FP reg N alignment
 *		    TALIGN(T)	   - check jump target T alignment
 */


#define FIX_DOUBLE(X)			((dfloat_t)(float)(X))

#if 0
/* TOP LEVEL decode table */
DEFLINK(TOPLEV, 0x00, "toplev", 26, 0x3f)
CONNECT(TOPLEV)
#endif

DEFLINK(CALL_PAL, 0x00, "call_pal", 0, 0xff)

#define LDA_IMPL							\
{									\
	SET_GPR(RA, GPR(RB) + SEXT(OFS));				\
}
DEFINST(LDA,			0x08,
	"lda",			"a,o(b)",
	IntALU,			F_ICOMP,
	DGPR(RA), DNA,		DNA, DGPR(RB), DNA)

#define LDAH_IMPL							\
{									\
	SET_GPR(RA, GPR(RB) + SEXT32(65536 * OFS));			\
}
DEFINST(LDAH,			0x09,
	"ldah",			"a,o(b)",
	IntALU,			F_ICOMP,
	DGPR(RA), DNA,		DNA, DGPR(RB), DNA)

/* EV56 BWX extension... */
#define LDBU_IMPL							\
{									\
	byte_t _result;							\
	enum md_fault_type _fault;					\
									\
	_result = READ_BYTE(GPR(RB) + SEXT(OFS), _fault);		\
	if(_fault != md_fault_none)					\
		DECLARE_FAULT(_fault);					\
									\
	SET_GPR(RA, (qword_t)_result);					\
}
DEFINST(LDBU,			0x0a,
	"ldbu",			"a,o(b)",
	RdPort,			F_MEM|F_LOAD|F_DISP,
	DGPR(RA), DNA,		DNA, DGPR(RB), DNA)

#define LDQ_U_IMPL							\
{									\
	qword_t _result;						\
	enum md_fault_type _fault;					\
									\
	_result = READ_QWORD((GPR(RB) + SEXT(OFS)) & ~7, _fault);	\
	if(_fault != md_fault_none)					\
		DECLARE_FAULT(_fault);					\
									\
	SET_GPR(RA, _result);						\
}
DEFINST(LDQ_U,			0x0b,
	"ldq_u",		"a,o(b)",
	RdPort,			F_MEM|F_LOAD|F_DISP,
	DGPR(RA), DNA,		DNA, DGPR(RB), DNA)

/* EV56 BWX extension... */
#define LDWU_IMPL							\
{									\
	half_t _result;							\
	enum md_fault_type _fault;					\
									\
	_result = READ_HALF(GPR(RB) + SEXT(OFS), _fault);		\
	if(_fault != md_fault_none)					\
		DECLARE_FAULT(_fault);					\
									\
	SET_GPR(RA, (qword_t)_result);					\
}
DEFINST(LDWU,			0x0c,
	"ldwu",			"a,o(b)",
	RdPort,			F_MEM|F_LOAD|F_DISP,
	DGPR(RA), DNA,		DNA, DGPR(RB), DNA)

/* EV56 BWX extension... */
#define STW_IMPL							\
{									\
	enum md_fault_type _fault;					\
									\
	WRITE_HALF((half_t)GPR(RA), GPR(RB) + SEXT(OFS), _fault);	\
	if(_fault != md_fault_none)					\
		DECLARE_FAULT(_fault);					\
}
DEFINST(STW,			0x0d,
	"stw",			"a,o(b)",
	WrPort,			F_MEM|F_STORE|F_DISP,
	DNA, DNA,		DGPR(RA), DGPR(RB), DNA)

/* EV56 BWX extension... */
#define STB_IMPL							\
{									\
	enum md_fault_type _fault;					\
									\
	WRITE_BYTE((byte_t)GPR(RA), GPR(RB) + SEXT(OFS), _fault);	\
	if(_fault != md_fault_none)					\
		DECLARE_FAULT(_fault);					\
}
DEFINST(STB,			0x0e,
	"stb",			"a,o(b)",
	WrPort,			F_MEM|F_STORE|F_DISP,
	DNA, DNA,		DGPR(RA), DGPR(RB), DNA)

#define STQ_U_IMPL							\
{									\
	enum md_fault_type _fault;					\
									\
	WRITE_QWORD(GPR(RA), (GPR(RB) + SEXT(OFS)) & ~7, _fault);	\
	if(_fault != md_fault_none)					\
		DECLARE_FAULT(_fault);					\
}
DEFINST(STQ_U,			0x0f,
	"stq_u",		"a,o(b)",
	WrPort,			F_MEM|F_STORE|F_DISP,
	DNA, DNA,		DGPR(RA), DGPR(RB), DNA)

DEFLINK(INTA, 0x10, "inta", 5, 0x7f)

DEFLINK(INTL, 0x11, "intl", 5, 0x7f)

DEFLINK(INTS, 0x12, "ints", 5, 0x7f)

/* changed from 0x7f to 0x3f to allow MUL{Q,L}/V */
DEFLINK(INTM, 0x13, "intm", 5, 0x3f)

/* added 02/27/99, plakal@cecil, from Alpha Arch Handbook (Rev.4, EV6) */

/* FIX extensions */
DEFLINK(ITFP, 0x14, "itfp", 5, 0x3f)

#define FLTV_IMPL							\
{									\
	/* FIXME: unimplemented */					\
	DECLARE_FAULT(md_fault_unimpl);					\
}
DEFINST(FLTV,			0x15,
	"fltv (unimpl)",	"",
	NA,			NA,
	DNA, DNA,		DNA, DNA, DNA)

DEFLINK(FLTI, 0x16, "flti", 5, 0x3f)

DEFLINK(FLTL, 0x17, "fltl", 5, /* FIXME: check this... */0x7f)

/* changed the shift & mask to incorporate new instructions in the group */
DEFLINK(MISC, 0x18, "misc", 8, 0xff)

DEFLINK(JMPJSR, 0x1a, "jmpjsr", 14, 0x03)

/* changed from EXTS to FPTI to include more extensions (FIX,CIX,MVI) */
DEFLINK(FPTI, 0x1c, "fpti", 5, 0x7f)

#define LDF_IMPL							\
{									\
	/* FIXME: unimplemented (VAX) */				\
	DECLARE_FAULT(md_fault_unimpl);					\
}
DEFINST(LDF,			0x20,
	"ldf (unimpl)",		"",
	NA,			NA,
	DNA, DNA,		DNA, DNA, DNA)

#define LDG_IMPL							\
{									\
	/* FIXME: unimplemented (VAX) */				\
	DECLARE_FAULT(md_fault_unimpl);					\
}
DEFINST(LDG,			0x21,
	"ldg (unimpl)",		"",
	NA,			NA,
	DNA, DNA,		DNA, DNA, DNA)

#define LDS_IMPL							\
{									\
	sqword_t _longhold, _e1, _e2;					\
	enum md_fault_type _fault;					\
									\
	_longhold = READ_WORD(GPR(RB) + SEXT(OFS), _fault);		\
	if(_fault != md_fault_none)					\
		DECLARE_FAULT(_fault);					\
									\
	_e1 = _longhold & 0x40000000;					\
	_e2 = (_longhold >> 23) & ULL(0x7f);				\
	if(_e1)								\
	{								\
		if(_e2 == ULL(0x7f))					\
			_e2 = ULL(0x7ff);				\
		else							\
			_e2 |= ULL(0x400);				\
	}								\
	else								\
	{								\
		if(_e2 != 0)						\
			_e2 |= ULL(0x380);				\
	}								\
	SET_FPR_Q(RA, (((_longhold & ULL(0x80000000)) << 32)		\
		| (_e2 << 52) | (_longhold & ULL(0x7fffff)) << 29));	\
}
DEFINST(LDS,			0x22,
	"lds",			"A,o(b)",
	RdPort,			F_MEM|F_LOAD|F_DISP,
	DFPR(RA), DNA,		DNA, DGPR(RB), DNA)

#define LDT_IMPL							\
{									\
	qword_t _result;						\
	enum md_fault_type _fault;					\
									\
	_result = READ_QWORD(GPR(RB) + SEXT(OFS), _fault);		\
	if(_fault != md_fault_none)					\
		DECLARE_FAULT(_fault);					\
									\
	SET_FPR_Q(RA, _result);						\
}
DEFINST(LDT,			0x23,
	"ldt",			"A,o(b)",
	RdPort,			F_MEM|F_LOAD|F_DISP,
	DFPR(RA), DNA,		DNA, DGPR(RB), DNA)

#define STF_IMPL							\
{									\
	/* FIXME: unimplemented (VAX) */				\
	DECLARE_FAULT(md_fault_unimpl);					\
}
DEFINST(STF,			0x24,
	"stf (unimpl)",		"",
	NA,			NA,
	DNA, DNA,		DNA, DNA, DNA)

#define STG_IMPL							\
{									\
	/* FIXME: unimplemented (VAX) */				\
	DECLARE_FAULT(md_fault_unimpl);					\
}
DEFINST(STG,			0x25,
	"stg (unimpl)",		"",
	NA,			NA,
	DNA, DNA,		DNA, DNA, DNA)

#define STS_IMPL							\
{									\
	sqword_t _longhold;						\
	sword_t _inthold;						\
	enum md_fault_type _fault;					\
									\
	_longhold = FPR_Q(RA);						\
	_inthold = (((_longhold >> 32) & ULL(0xc0000000))		\
		| ((_longhold >> 29) & ULL(0x3fffffff)));		\
	WRITE_WORD(_inthold, GPR(RB) + SEXT(OFS), _fault);		\
	if(_fault != md_fault_none)					\
		DECLARE_FAULT(_fault);					\
}
DEFINST(STS,			0x26,
	"sts",			"A,o(b)",
	WrPort,			F_MEM|F_STORE|F_DISP,
	DNA, DNA,		DFPR(RA), DGPR(RB), DNA)

#define STT_IMPL							\
{									\
	enum md_fault_type _fault;					\
									\
	WRITE_QWORD(FPR_Q(RA), GPR(RB) + SEXT(OFS), _fault);		\
	if(_fault != md_fault_none)					\
		DECLARE_FAULT(_fault);					\
}
DEFINST(STT,			0x27,
	"stt",			"A,o(b)",
	WrPort,			F_MEM|F_STORE|F_DISP,
	DNA, DNA,		DFPR(RA), DGPR(RB), DNA)

#define LDL_IMPL							\
{									\
	word_t _result;							\
	enum md_fault_type _fault(md_fault_none);			\
									\
	_result = READ_WORD(GPR(RB) + SEXT(OFS), _fault);		\
	if(_fault != md_fault_none)					\
		DECLARE_FAULT(_fault);					\
									\
	SET_GPR(RA, (sqword_t)((sword_t)_result));			\
}
DEFINST(LDL,			0x28,
	"ldl",			"a,o(b)",
	RdPort,			F_MEM|F_LOAD|F_DISP,
	DGPR(RA), DNA,		DNA, DGPR(RB), DNA)

#define LDQ_IMPL							\
{									\
	qword_t _result;						\
	enum md_fault_type _fault;					\
									\
	_result = READ_QWORD(GPR(RB) + SEXT(OFS), _fault);		\
	if(_fault != md_fault_none)					\
	{								\
		_result = READ_WORD(GPR(RB) + SEXT(OFS) + 0, _fault);	\
		if(_fault != md_fault_none)				\
			DECLARE_FAULT(_fault);				\
		_result <<=32;						\
		_result += READ_WORD(GPR(RB) + SEXT(OFS) + 4, _fault);	\
		addr=GPR(RB)+SEXT(OFS);					\
		if(_fault != md_fault_none)				\
			DECLARE_FAULT(_fault);				\
	}								\
									\
	SET_GPR(RA, _result);						\
}
DEFINST(LDQ,			0x29,
	"ldq",			"a,o(b)",
	RdPort,			F_MEM|F_LOAD|F_DISP,
	DGPR(RA), DNA,		DNA, DGPR(RB), DNA)

#define LDL_L_IMPL							\
{									\
	enum md_fault_type _fault;					\
									\
	word_t _result = READ_WORD(GPR(RB) + SEXT(OFS), _fault);	\
	if(_fault != md_fault_none)					\
		DECLARE_FAULT(_fault);					\
									\
	cores[core_num].lock_flag = 1;					\
	cores[core_num].locked_physical_address = GPR(RB) + SEXT(OFS);	\
	SET_GPR(RA, (sqword_t)((sword_t)_result));			\
}
DEFINST(LDL_L,			0x2a,
	"ldl_l (unimpl)",	"a,o(b)",
	RdPort,			F_MEM|F_LOAD|F_DISP,
	DGPR(RA), DNA,		DNA, DGPR(RB), DNA)

#define LDQ_L_IMPL							\
{									\
	enum md_fault_type _fault;					\
									\
	qword_t _result = READ_QWORD(GPR(RB) + SEXT(OFS), _fault);	\
	if(_fault != md_fault_none)					\
		DECLARE_FAULT(_fault);					\
									\
	cores[core_num].lock_flag = 1;					\
	cores[core_num].locked_physical_address = GPR(RB) + SEXT(OFS);	\
	SET_GPR(RA, _result);						\
}
DEFINST(LDQ_L,			0x2b,
	"ldq_l (unimpl)",	"a,o(b)",
	RdPort,			F_MEM|F_LOAD|F_DISP,
	DGPR(RA), DNA,		DNA, DGPR(RB), DNA)

//FIXME: _src masking should not be necessary since WRITE_WORD should handle it (although, we should cast GPR(RA) as word_t.
#define STL_IMPL							\
{									\
	word_t _src;							\
	enum md_fault_type _fault;					\
									\
	_src = (word_t)(GPR(RA) & ULL(0xffffffff));			\
	WRITE_WORD(_src, GPR(RB) + SEXT(OFS), _fault);			\
	if(_fault != md_fault_none)					\
		DECLARE_FAULT(_fault);					\
}
DEFINST(STL,			0x2c,
	"stl",			"a,o(b)",
	WrPort,			F_MEM|F_STORE|F_DISP,
	DNA, DNA,		DGPR(RA), DGPR(RB), DNA)

#define STQ_IMPL							\
{									\
	enum md_fault_type _fault;					\
									\
	WRITE_QWORD(GPR(RA), GPR(RB) + SEXT(OFS), _fault);		\
	if(_fault != md_fault_none)					\
	{								\
		word_t top = GPR(RA)>>32;				\
		WRITE_WORD(top, GPR(RB) + SEXT(OFS) + 0, _fault);	\
		if(_fault != md_fault_none)				\
			DECLARE_FAULT(_fault);				\
		top = GPR(RA) & 0xffffffff;				\
		WRITE_WORD(top, GPR(RB) + SEXT(OFS) + 4, _fault);	\
		addr=GPR(RB)+SEXT(OFS);					\
		if(_fault != md_fault_none)				\
			DECLARE_FAULT(_fault);				\
	}								\
}
DEFINST(STQ,			0x2d,
	"stq",			"a,o(b)",
	WrPort,			F_MEM|F_STORE|F_DISP,
	DNA, DNA,		DGPR(RA), DGPR(RB), DNA)

//FIXME: The address must occur within an aligned 16 byte block, we check for exact matches here.
//FIXME: CALL_PAL rei, rti and rfe clear the lock_flag.
#define STL_C_IMPL											\
{													\
	enum md_fault_type _fault;									\
													\
	word_t _src = (word_t)(GPR(RA) & ULL(0xffffffff));						\
	SET_GPR(RA, 0);											\
	if(cores[core_num].lock_flag && (GPR(RB)+SEXT(OFS))==cores[core_num].locked_physical_address)	\
	{												\
		WRITE_WORD(_src, GPR(RB) + SEXT(OFS), _fault);						\
		if(_fault != md_fault_none)								\
			DECLARE_FAULT(_fault);								\
		SET_GPR(RA, 1);										\
	}												\
	else												\
	{												\
		SET_GPR(RA, 0);										\
	}												\
	cores[core_num].lock_flag = 0;									\
}
DEFINST(STL_C,			0x2e,
	"stl_c (unimpl)",	"a,o(b)",
	WrPort,			F_MEM|F_STORE|F_DISP,
	DGPR(RA), DNA,		DGPR(RA), DGPR(RB), DNA)

//FIXME: The address must occur within an aligned 16 byte block, we check for exact matches here.
#define STQ_C_IMPL											\
{													\
	enum md_fault_type _fault;									\
													\
	if(cores[core_num].lock_flag && (GPR(RB)+SEXT(OFS))==cores[core_num].locked_physical_address)	\
	{												\
		WRITE_QWORD(GPR(RA), GPR(RB) + SEXT(OFS), _fault);					\
		if(_fault != md_fault_none)								\
			DECLARE_FAULT(_fault);								\
		SET_GPR(RA, 1);										\
	}												\
	else												\
	{												\
		SET_GPR(RA, 0);										\
	}												\
	cores[core_num].lock_flag = 0;									\
}
DEFINST(STQ_C,			0x2f,
	"stq_c (unimpl)",	"a,o(b)",
	WrPort,			F_MEM|F_STORE|F_DISP,
	DGPR(RA), DNA,		DGPR(RA), DGPR(RB), DNA)

#define BR_IMPL								\
{									\
	SET_TPC(CPC + (SEXT21(TARG) << 2) + 4);				\
	SET_NPC(CPC + (SEXT21(TARG) << 2) + 4);				\
	SET_GPR(RA, CPC + 4);						\
}
DEFINST(BR,			0x30,
	"br",			"a,J",
	IntALU,			F_CTRL|F_UNCOND|F_DIRJMP,
	DGPR(RA), DNA,		DNA, DNA, DNA)

#define FBEQ_IMPL							\
{									\
	SET_TPC(CPC + (SEXT21(TARG) << 2) + 4);				\
	if(FPR(RA) == 0.0)						\
		SET_NPC(CPC + (SEXT21(TARG) << 2) + 4);			\
}
DEFINST(FBEQ,			0x31,
	"fbeq",			"A,j",
	FloatADD,		F_CTRL|F_COND|F_DIRJMP,
	DNA, DNA,		DFPR(RA), DNA, DNA)

#define FBLT_IMPL							\
{									\
	SET_TPC(CPC + (SEXT21(TARG) << 2) + 4);				\
	if(FPR(RA) < 0.0)						\
		SET_NPC(CPC + (SEXT21(TARG) << 2) + 4);			\
}
DEFINST(FBLT,			0x32,
	"fblt",			"A,j",
	FloatADD,		F_CTRL|F_COND|F_DIRJMP,
	DNA, DNA,		DFPR(RA), DNA, DNA)

#define FBLE_IMPL							\
{									\
	SET_TPC(CPC + (SEXT21(TARG) << 2) + 4);				\
	if(FPR(RA) <= 0.0)						\
		SET_NPC(CPC + (SEXT21(TARG) << 2) + 4);			\
}
DEFINST(FBLE,			0x33,
	"fble",			"A,j",
	FloatADD,		F_CTRL|F_COND|F_DIRJMP,
	DNA, DNA,		DFPR(RA), DNA, DNA)

/* NOTE: this is semantically equivalent to BR, the different opcode tips
   off the predictor to use the return address stack... */
#define BSR_IMPL							\
{									\
	SET_TPC(CPC + (SEXT21(TARG) << 2) + 4);				\
	SET_NPC(CPC + (SEXT21(TARG) << 2) + 4);				\
	SET_GPR(RA, CPC + 4);						\
}
DEFINST(BSR,			0x34,
	"bsr",			"a,J",
	IntALU,			F_CTRL|F_UNCOND|F_DIRJMP,
	DGPR(RA), DNA,		DNA, DNA, DNA)

#define FBNE_IMPL							\
{									\
	SET_TPC(CPC + (SEXT21(TARG) << 2) + 4);				\
	if(FPR(RA) != 0.0)						\
		SET_NPC(CPC + (SEXT21(TARG) << 2) + 4);			\
}
DEFINST(FBNE,			0x35,
	"fbne",			"A,j",
	FloatADD,		F_CTRL|F_COND|F_DIRJMP,
	DNA, DNA,		DFPR(RA), DNA, DNA)

#define FBGE_IMPL							\
{									\
	SET_TPC(CPC + (SEXT21(TARG) << 2) + 4);				\
	if(FPR(RA) >= 0.0)						\
		SET_NPC(CPC + (SEXT21(TARG) << 2) + 4);			\
}
DEFINST(FBGE,			0x36,
	"fbge",			"A,j",
	FloatADD,		F_CTRL|F_COND|F_DIRJMP,
	DNA, DNA,		DFPR(RA), DNA, DNA)

#define FBGT_IMPL							\
{									\
	SET_TPC(CPC + (SEXT21(TARG) << 2) + 4);				\
	if(FPR(RA) > 0.0)						\
		SET_NPC(CPC + (SEXT21(TARG) << 2) + 4);			\
}
DEFINST(FBGT,			0x37,
	"fbgt",			"A,j",
	FloatADD,		F_CTRL|F_COND|F_DIRJMP,
	DNA, DNA,		DFPR(RA), DNA, DNA)

#define BLBC_IMPL							\
{									\
	SET_TPC(CPC + (SEXT21(TARG) << 2) + 4);				\
	if(!(GPR(RA) & 1))						\
		SET_NPC(CPC + (SEXT21(TARG) << 2) + 4);			\
}
DEFINST(BLBC,			0x38,
	"blbc",			"a,j",
	IntALU,			F_CTRL|F_COND|F_DIRJMP,
	DNA, DNA,		DGPR(RA), DNA, DNA)

#define BEQ_IMPL							\
{									\
	SET_TPC(CPC + (SEXT21(TARG) << 2) + 4);				\
	if(GPR(RA) == ULL(0))						\
		SET_NPC(CPC + (SEXT21(TARG) << 2) + 4);				\
}
DEFINST(BEQ,			0x39,
	"beq",			"a,j",
	IntALU,			F_CTRL|F_COND|F_DIRJMP,
	DNA, DNA,		DGPR(RA), DNA, DNA)

#define BLT_IMPL							\
{									\
	SET_TPC(CPC + (SEXT21(TARG) << 2) + 4);				\
	if((sqword_t)GPR(RA) < LL(0))					\
		SET_NPC(CPC + (SEXT21(TARG) << 2) + 4);			\
}
DEFINST(BLT,			0x3a,
	"blt",			"a,j",
	IntALU,			F_CTRL|F_COND|F_DIRJMP,
	DNA, DNA,		DGPR(RA), DNA, DNA)

#define BLE_IMPL							\
{									\
	SET_TPC(CPC + (SEXT21(TARG) << 2) + 4);				\
	if((sqword_t)GPR(RA) <= LL(0))					\
		SET_NPC(CPC + (SEXT21(TARG) << 2) + 4);			\
}
DEFINST(BLE,			0x3b,
	"ble",			"a,j",
	IntALU,			F_CTRL|F_COND|F_DIRJMP,
	DNA, DNA,		DGPR(RA), DNA, DNA)

#define BLBS_IMPL							\
{									\
	SET_TPC(CPC + (SEXT21(TARG) << 2) + 4);				\
	if(GPR(RA) & 1)							\
		SET_NPC(CPC + (SEXT21(TARG) << 2) + 4);			\
}
DEFINST(BLBS,			0x3c,
	"blbs",			"a,j",
	IntALU,			F_CTRL|F_COND|F_DIRJMP,
	DNA, DNA,		DGPR(RA), DNA, DNA)

#define BNE_IMPL							\
{									\
	SET_TPC(CPC + (SEXT21(TARG) << 2) + 4);				\
	if(GPR(RA) != ULL(0))						\
		SET_NPC(CPC + (SEXT21(TARG) << 2) + 4);			\
}
DEFINST(BNE,			0x3d,
	"bne",			"a,j",
	IntALU,			F_CTRL|F_COND|F_DIRJMP,
	DNA, DNA,		DGPR(RA), DNA, DNA)

#define BGE_IMPL							\
{									\
	SET_TPC(CPC + (SEXT21(TARG) << 2) + 4);				\
	if((sqword_t)GPR(RA) >= LL(0))					\
		SET_NPC(CPC + (SEXT21(TARG) << 2) + 4);			\
}
DEFINST(BGE,			0x3e,
	"bge",			"a,j",
	IntALU,			F_CTRL|F_COND|F_DIRJMP,
	DNA, DNA,		DGPR(RA), DNA, DNA)

#define BGT_IMPL							\
{									\
	SET_TPC(CPC + (SEXT21(TARG) << 2) + 4);				\
	if((sqword_t)GPR(RA) > LL(0))					\
		SET_NPC(CPC + (SEXT21(TARG) << 2) + 4);			\
}
DEFINST(BGT,			0x3f,
	"bgt",			"a,j",
	IntALU,			F_CTRL|F_COND|F_DIRJMP,
	DNA, DNA,		DGPR(RA), DNA, DNA)


CONNECT(CALL_PAL)

#define PAL_WRENT_IMPL							\
{									\
	PALCALL(inst);							\
}
DEFINST(PAL_WRENT,		0x34,
	"call_pal wrent (unimpl)","",
	NA,			F_TRAP,
	DNA, DNA,		DGPR(RA), DNA, DNA)

#define PAL_WRKGP_IMPL							\
{									\
	PALCALL(inst);							\
}
DEFINST(PAL_WRKGP,		0x37,
	"call_pal wrkgp",	"",
	NA,			F_TRAP,
	DGPR(MD_REG_GP), DNA,	DGPR(RA), DNA, DNA)

#define PAL_CALLSYS_IMPL						\
{									\
	SYSCALL(inst);							\
}
DEFINST(PAL_CALLSYS,		0x83,
	"call_pal callsys",	"",
	NA,			F_TRAP,
	DNA, DNA,		DNA, DNA, DNA)

#define PAL_IMB_IMPL							\
{									\
	PALCALL(inst);							\
}
DEFINST(PAL_IMB,		0x86,
	"call_pal imb",		"",
	NA,			F_TRAP,
	DNA, DNA,		DNA, DNA, DNA)

#define PAL_RDUNIQ_IMPL							\
{									\
	PALCALL(inst);							\
}
DEFINST(PAL_RDUNIQ,		0x9e,
	"call_pal rduniq",	"",
	NA,			F_TRAP,
	DGPR(/* v0 */0), DNA,	DUNIQ, DNA, DNA)

#define PAL_WRUNIQ_IMPL							\
{									\
	PALCALL(inst);							\
}
DEFINST(PAL_WRUNIQ,		0x9f,
	"call_pal wruniq",	"",
	NA,			F_TRAP,
	DUNIQ, DNA,		DGPR(/* a0 */16), DNA, DNA)

CONNECT(INTA)

DEFLINK(ADDL_LINK, 0x00, "addl_link", 12, 1)

DEFLINK(S4ADDL_LINK, 0x02, "s4addl_link", 12, 1)

DEFLINK(SUBL_LINK, 0x09, "subl_link", 12, 1)

DEFLINK(S4SUBL_LINK, 0x0b, "s4subl_link", 12, 1)

DEFLINK(CMPBGE_LINK, 0x0f, "cmpbge_link", 12, 1)

DEFLINK(S8ADDL_LINK, 0x12, "s8addl_link", 12, 1)

DEFLINK(S8SUBL_LINK, 0x1b, "s8subl_link", 12, 1)

DEFLINK(CMPULT_LINK, 0x1d, "cmpult_link", 12, 1)

DEFLINK(ADDQ_LINK, 0x20, "addq_link", 12, 1)

DEFLINK(S4ADDQ_LINK, 0x22, "s4addq_link", 12, 1)

DEFLINK(SUBQ_LINK, 0x29, "subq_link", 12, 1)

DEFLINK(S4SUBQ_LINK, 0x2b, "s4subq_link", 12, 1)

DEFLINK(CMPEQ_LINK, 0x2d, "cmpeq_link", 12, 1)

DEFLINK(S8ADDQ_LINK, 0x32, "s8addq_link", 12, 1)

DEFLINK(S8SUBQ_LINK, 0x3b, "s8subq_link", 12, 1)

DEFLINK(CMPULE_LINK, 0x3d, "cmpule_link", 12, 1)

DEFLINK(ADDLV_LINK, 0x40, "addlv_link", 12, 1)

DEFLINK(SUBLV_LINK, 0x49, "sublv_link", 12, 1)

DEFLINK(CMPLT_LINK, 0x4d, "cmplt_link", 12, 1)

DEFLINK(ADDQV_LINK, 0x60, "addqv_link", 12, 1)

DEFLINK(SUBQV_LINK, 0x69, "subqv_link", 12, 1)

DEFLINK(CMPLE_LINK, 0x6d, "cmple_link", 12, 1)


CONNECT(ADDL_LINK)

#define ADDL_IMPL							\
{									\
	SET_GPR(RC, SEXT32((GPR(RA) + GPR(RB)) & ULL(0xffffffff)));	\
}
DEFINST(ADDL,			0x00,
	"addl",			"a,b,c",
	IntALU,			F_ICOMP,
	DGPR(RC), DNA,		DGPR(RA), DGPR(RB), DNA)

#define ADDLI_IMPL							\
{									\
	SET_GPR(RC, SEXT32((GPR(RA) + IMM) & ULL(0xffffffff)));		\
}
DEFINST(ADDLI,			0x01,
	"addl",			"a,i,c",
	IntALU,			F_ICOMP|F_IMM,
	DGPR(RC), DNA,		DGPR(RA), DNA, DNA)
	

CONNECT(S4ADDL_LINK)

#define S4ADDL_IMPL								\
{										\
	SET_GPR(RC, SEXT32(((GPR(RA) << 2) + GPR(RB)) & ULL(0xffffffff)));	\
}
DEFINST(S4ADDL,			0x00,
	"s4addl",		"a,b,c",
	IntALU,			F_ICOMP,
	DGPR(RC), DNA,		DGPR(RA), DGPR(RB), DNA)

#define S4ADDLI_IMPL							\
{									\
	SET_GPR(RC, SEXT32(((GPR(RA) << 2) + IMM) & ULL(0xffffffff)));	\
}
DEFINST(S4ADDLI,		0x01,
	"s4addl",		"a,i,c",
	IntALU,			F_ICOMP|F_IMM,
	DGPR(RC), DNA,		DGPR(RA), DNA, DNA)
	

CONNECT(SUBL_LINK)

#define SUBL_IMPL							\
{									\
	SET_GPR(RC, SEXT32((GPR(RA) - GPR(RB)) & ULL(0xffffffff)));	\
}
DEFINST(SUBL,			0x00,
	"subl",			"a,b,c",
	IntALU,			F_ICOMP,
	DGPR(RC), DNA,		DGPR(RA), DGPR(RB), DNA)

#define SUBLI_IMPL							\
{									\
	SET_GPR(RC, SEXT32((GPR(RA) - IMM) & ULL(0xffffffff)));		\
}
DEFINST(SUBLI,			0x01,
	"subl",			"a,i,c",
	IntALU,			F_ICOMP|F_IMM,
	DGPR(RC), DNA,		DGPR(RA), DNA, DNA)


CONNECT(S4SUBL_LINK)

#define S4SUBL_IMPL								\
{										\
	SET_GPR(RC, SEXT32(((GPR(RA) << 2) - GPR(RB)) & ULL(0xffffffff)));	\
}
DEFINST(S4SUBL,			0x00,
	"s4subl",		"a,b,c",
	IntALU,			F_ICOMP,
	DGPR(RC), DNA,		DGPR(RA), DGPR(RB), DNA)

#define S4SUBLI_IMPL							\
{									\
	SET_GPR(RC, SEXT32(((GPR(RA) << 2) - IMM) & ULL(0xffffffff)));	\
}
DEFINST(S4SUBLI,		0x01,
	"s4subl",		"a,i,c",
	IntALU,			F_ICOMP|F_IMM,
	DGPR(RC), DNA,		DGPR(RA), DNA, DNA)


CONNECT(CMPBGE_LINK)

#define CMPBGE_IMPL							\
{									\
	int _i;								\
	qword_t _rav, _rbv;						\
									\
	_rav = GPR(RA);							\
	_rbv = GPR(RB);							\
	SET_GPR(RC, 0);							\
									\
	for(_i = 56; _i >= 0; _i -= 8)					\
	{								\
		SET_GPR(RC, GPR(RC) << 1);				\
		SET_GPR(RC, (GPR(RC)					\
			| ((_rav >> _i & (sqword_t)0xff) >=		\
			(_rbv >> _i & (sqword_t)0xff))));		\
	}								\
}
DEFINST(CMPBGE,			0x00,
	"cmpbge",		"a,b,c",
	IntALU,			F_ICOMP,
	DGPR(RC), DNA,		DGPR(RA), DGPR(RB), DNA)

#define CMPBGEI_IMPL							\
{									\
	int _i;								\
	qword_t _rav, _rbv;						\
									\
	_rav = GPR(RA);							\
	_rbv = IMM;							\
	SET_GPR(RC, 0);							\
									\
	for(_i = 56; _i >= 0; _i -= 8)					\
	{								\
		SET_GPR(RC, GPR(RC) << 1);				\
		SET_GPR(RC, (GPR(RC)					\
			| ((_rav >> _i & (sqword_t)0xff) >=		\
			(_rbv >> _i & (sqword_t)0xff))));		\
	}								\
}
DEFINST(CMPBGEI,		0x01,
	"cmpbge",		"a,i,c",
	IntALU,			F_ICOMP|F_IMM,
	DGPR(RC), DNA,		DGPR(RA), DNA, DNA)


CONNECT(S8ADDL_LINK)

#define S8ADDL_IMPL								\
{										\
	SET_GPR(RC, SEXT32(((GPR(RA) << 3) + GPR(RB)) & ULL(0xffffffff)));	\
}
DEFINST(S8ADDL,			0x00,
	"s8addl",		"a,b,c",
	IntALU,			F_ICOMP,
	DGPR(RC), DNA,		DGPR(RA), DGPR(RB), DNA)
	
#define S8ADDLI_IMPL							\
{									\
	SET_GPR(RC, SEXT32(((GPR(RA) << 3) + IMM) & ULL(0xffffffff)));	\
}
DEFINST(S8ADDLI,		0x01,
	"s8addl",		"a,i,c",
	IntALU,			F_ICOMP|F_IMM,
	DGPR(RC), DNA,		DGPR(RA), DNA, DNA)
	

CONNECT(S8SUBL_LINK)

#define S8SUBL_IMPL								\
{										\
	SET_GPR(RC, SEXT32(((GPR(RA) << 3) - GPR(RB)) & ULL(0xffffffff)));	\
}
DEFINST(S8SUBL,			0x00,
	"s8subl",		"a,b,c",
	IntALU,			F_ICOMP,
	DGPR(RC), DNA,		DGPR(RA), DGPR(RB), DNA)
	
#define S8SUBLI_IMPL							\
{									\
	SET_GPR(RC, SEXT32(((GPR(RA) << 3) - IMM) & ULL(0xffffffff)));	\
}
DEFINST(S8SUBLI,		0x01,
	"s8subl",		"a,i,c",
	IntALU,			F_ICOMP|F_IMM,
	DGPR(RC), DNA,		DGPR(RA), DNA, DNA)


CONNECT(CMPULT_LINK)

#define CMPULT_IMPL							\
{									\
	SET_GPR(RC, (qword_t)GPR(RA) < (qword_t)GPR(RB));		\
}
DEFINST(CMPULT,			0x00,
	"cmpult",		"a,b,c",
	IntALU,			F_ICOMP,
	DGPR(RC), DNA,		DGPR(RA), DGPR(RB), DNA)

#define CMPULTI_IMPL							\
{									\
	SET_GPR(RC, (qword_t)GPR(RA) < (qword_t)IMM);			\
}
DEFINST(CMPULTI,		0x01,
	"cmpult",		"a,i,c",
	IntALU,			F_ICOMP|F_IMM,
	DGPR(RC), DNA,		DGPR(RA), DNA, DNA)


CONNECT(ADDQ_LINK)

#define ADDQ_IMPL							\
{									\
	SET_GPR(RC, GPR(RA) + GPR(RB));					\
}
DEFINST(ADDQ,			0x00,
	"addq",			"a,b,c",
	IntALU,			F_ICOMP,
	DGPR(RC), DNA,		DGPR(RA), DGPR(RB), DNA)
	
#define ADDQI_IMPL							\
{									\
	SET_GPR(RC, GPR(RA) + IMM);					\
}
DEFINST(ADDQI,			0x01,
	"addq",			"a,i,c",
	IntALU,			F_ICOMP|F_IMM,
	DGPR(RC), DNA,		DGPR(RA), DNA, DNA)


CONNECT(S4ADDQ_LINK)

#define S4ADDQ_IMPL							\
{									\
	SET_GPR(RC, (GPR(RA) << 2) + GPR(RB));				\
}
DEFINST(S4ADDQ,			0x00,
	"s4addq",		"a,b,c",
	IntALU,			F_ICOMP,
	DGPR(RC), DNA,		DGPR(RA), DGPR(RB), DNA)

#define S4ADDQI_IMPL							\
{									\
	SET_GPR(RC, (GPR(RA) << 2) + IMM);				\
}
DEFINST(S4ADDQI,		0x01,
	"s4addq",		"a,i,c",
	IntALU,			F_ICOMP|F_IMM,
	DGPR(RC), DNA,		DGPR(RA), DNA, DNA)


CONNECT(SUBQ_LINK)

#define SUBQ_IMPL							\
{									\
	SET_GPR(RC, GPR(RA) - GPR(RB));					\
}
DEFINST(SUBQ,			0x00,
	"subq",			"a,b,c",
	IntALU,			F_ICOMP,
	DGPR(RC), DNA,		DGPR(RA), DGPR(RB), DNA)

#define SUBQI_IMPL							\
{									\
	SET_GPR(RC, GPR(RA) - IMM);					\
}
DEFINST(SUBQI,			0x01,
	"subq",			"a,i,c",
	IntALU,			F_ICOMP|F_IMM,
	DGPR(RC), DNA,		DGPR(RA), DNA, DNA)


CONNECT(S4SUBQ_LINK)

#define S4SUBQ_IMPL							\
{									\
	SET_GPR(RC, (GPR(RA) << 2) - GPR(RB));				\
}
DEFINST(S4SUBQ,			0x00,
	"s4subq",		"a,b,c",
	IntALU,			F_ICOMP,
	DGPR(RC), DNA,		DGPR(RA), DGPR(RB), DNA)

#define S4SUBQI_IMPL							\
{									\
	SET_GPR(RC, (GPR(RA) << 2) - IMM);				\
}
DEFINST(S4SUBQI,		0x01,
	"s4subq",		"a,i,c",
	IntALU,			F_ICOMP|F_IMM,
	DGPR(RC), DNA,		DGPR(RA), DNA, DNA)


CONNECT(CMPEQ_LINK)

#define CMPEQ_IMPL							\
{									\
	SET_GPR(RC, GPR(RA) == GPR(RB));				\
}
DEFINST(CMPEQ,			0x00,
	"cmpeq",		"a,b,c",
	IntALU,			F_ICOMP,
	DGPR(RC), DNA,		DGPR(RA), DGPR(RB), DNA)

#define CMPEQI_IMPL							\
{									\
	SET_GPR(RC, GPR(RA) == (qword_t)IMM);				\
}
DEFINST(CMPEQI,			0x01,
	"cmpeq",		"a,i,c",
	IntALU,			F_ICOMP|F_IMM,
	DGPR(RC), DNA,		DGPR(RA), DNA, DNA)


CONNECT(S8ADDQ_LINK)

#define S8ADDQ_IMPL							\
{									\
	SET_GPR(RC, (GPR(RA) << 3) + GPR(RB));				\
}
DEFINST(S8ADDQ,			0x00,
	"s8addq",		"a,b,c",
	IntALU,			F_ICOMP,
	DGPR(RC), DNA,		DGPR(RA), DGPR(RB), DNA)

#define S8ADDQI_IMPL							\
{									\
	SET_GPR(RC, (GPR(RA) << 3) + IMM);				\
}
DEFINST(S8ADDQI,		0x01,
	"s8addq",		"a,i,c",
	IntALU,			F_ICOMP|F_IMM,
	DGPR(RC), DNA,		DGPR(RA), DNA, DNA)


CONNECT(S8SUBQ_LINK)

#define S8SUBQ_IMPL							\
{									\
	SET_GPR(RC, (GPR(RA) << 3) - GPR(RB));				\
}
DEFINST(S8SUBQ,			0x00,
	"s8subq",		"a,b,c",
	IntALU,			F_ICOMP,
	DGPR(RC), DNA,		DGPR(RA), DGPR(RB), DNA)

#define S8SUBQI_IMPL							\
{									\
	SET_GPR(RC, (GPR(RA) << 3) - IMM);				\
}
DEFINST(S8SUBQI,		0x01,
	"s8subq",		"a,i,c",
	IntALU,			F_ICOMP|F_IMM,
	DGPR(RC), DNA,		DGPR(RA), DNA, DNA)


CONNECT(CMPULE_LINK)

#define CMPULE_IMPL							\
{									\
	SET_GPR(RC, (qword_t)GPR(RA) <= (qword_t)GPR(RB));		\
}
DEFINST(CMPULE,			0x00,
	"cmpule",		"a,b,c",
	IntALU,			F_ICOMP,
	DGPR(RC), DNA,		DGPR(RA), DGPR(RB), DNA)

#define CMPULEI_IMPL							\
{									\
	SET_GPR(RC, (qword_t)GPR(RA) <= (qword_t)IMM);			\
}
DEFINST(CMPULEI,		0x01,
	"cmpule",		"a,i,c",
	IntALU,			F_ICOMP|F_IMM,
	DGPR(RC), DNA,		DGPR(RA), DNA, DNA)


CONNECT(ADDLV_LINK)

#define ADDLV_IMPL							\
{									\
	/* FIXME: not checking for overflows... */			\
	SET_GPR(RC, SEXT32((GPR(RA) + GPR(RB)) & ULL(0xffffffff)));	\
}
DEFINST(ADDLV,			0x00,
	"addl/v (unimpl)",	"a,b,c",
	IntALU,			F_ICOMP,
	DGPR(RC), DNA,		DGPR(RA), DGPR(RB), DNA)

#define ADDLVI_IMPL							\
{									\
	/* FIXME: not checking for overflows... */			\
	SET_GPR(RC, SEXT32((GPR(RA) + IMM) & ULL(0xffffffff)));		\
}
DEFINST(ADDLVI,			0x01,
	"addl/v (unimpl)",	"a,i,c",
	IntALU,			F_ICOMP|F_IMM,
	DGPR(RC), DNA,		DGPR(RA), DNA, DNA)


CONNECT(SUBLV_LINK)

#define SUBLV_IMPL							\
{									\
	SET_GPR(RC, SEXT32((GPR(RA) - GPR(RB)) & ULL(0xffffffff)));	\
}
DEFINST(SUBLV,			0x00,
	"subl/v (unimpl)",	"a,b,c",
	IntALU,			F_ICOMP,
	DGPR(RC), DNA,		DGPR(RA), DGPR(RB), DNA)

#define SUBLVI_IMPL							\
{									\
	SET_GPR(RC, SEXT32((GPR(RA) - IMM) & ULL(0xffffffff)));		\
}
DEFINST(SUBLVI,			0x01,
	"subl/v (unimpl)",	"a,i,c",
	IntALU,			F_ICOMP|F_IMM,
	DGPR(RC), DNA,		DGPR(RA), DNA, DNA)


CONNECT(CMPLT_LINK)

#define CMPLT_IMPL							\
{									\
	SET_GPR(RC, (sqword_t)GPR(RA) < (sqword_t)GPR(RB));		\
}
DEFINST(CMPLT,			0x00,
	"cmplt",		"a,b,c",
	IntALU,			F_ICOMP,
	DGPR(RC), DNA,		DGPR(RA), DGPR(RB), DNA)

#define CMPLTI_IMPL							\
{									\
	SET_GPR(RC, (sqword_t)GPR(RA) < (sqword_t)IMM);			\
}
DEFINST(CMPLTI,		0x01,
	"cmplt",		"a,i,c",
	IntALU,			F_ICOMP|F_IMM,
	DGPR(RC), DNA,		DGPR(RA), DNA, DNA)


CONNECT(ADDQV_LINK)

#define ADDQV_IMPL							\
{									\
	/* FIXME: not checking for overflows... */			\
	SET_GPR(RC, GPR(RA) + GPR(RB));					\
}
DEFINST(ADDQV,			0x00,
	"addq/v (unimpl)",	"a,b,c",
	IntALU,			F_ICOMP,
	DGPR(RC), DNA,		DGPR(RA), DGPR(RB), DNA)

#define ADDQVI_IMPL							\
{									\
	/* FIXME: not checking for overflows... */			\
	SET_GPR(RC, GPR(RA) + IMM);					\
}
DEFINST(ADDQVI,			0x01,
	"addq/v (unimpl)",	"a,i,c",
	IntALU,			F_ICOMP|F_IMM,
	DGPR(RC), DNA,		DGPR(RA), DNA, DNA)


CONNECT(SUBQV_LINK)

#define SUBQV_IMPL							\
{									\
	/* FIXME: not checking for overflows... */			\
	SET_GPR(RC, GPR(RA) - GPR(RB));					\
}
DEFINST(SUBQV,			0x00,
	"subq/v (unimpl)",	"a,b,c",
	IntALU,			F_ICOMP,
	DGPR(RC), DNA,		DGPR(RA), DGPR(RB), DNA)

#define SUBQVI_IMPL							\
{									\
	/* FIXME: not checking for overflows... */			\
	SET_GPR(RC, GPR(RA) - IMM);					\
}
DEFINST(SUBQVI,			0x01,
	"subq/v (unimpl)",	"a,b,c",
	IntALU,			F_ICOMP|F_IMM,
	DGPR(RC), DNA,		DGPR(RA), DNA, DNA)


CONNECT(CMPLE_LINK)

#define CMPLE_IMPL							\
{									\
	SET_GPR(RC, (sqword_t)GPR(RA) <= (sqword_t)GPR(RB));		\
}
DEFINST(CMPLE,			0x00,
	"cmple",		"a,b,c",
	IntALU,			F_ICOMP,
	DGPR(RC), DNA,		DGPR(RA), DGPR(RB), DNA)

#define CMPLEI_IMPL							\
{									\
	SET_GPR(RC, (sqword_t)GPR(RA) <= (sqword_t)IMM);		\
}
DEFINST(CMPLEI,		0x01,
	"cmple",		"a,i,c",
	IntALU,			F_ICOMP|F_IMM,
	DGPR(RC), DNA,		DGPR(RA), DNA, DNA)


CONNECT(INTL)

DEFLINK(AND_LINK, 0x00, "and_link", 12, 1)

DEFLINK(BIC_LINK, 0x08, "bic_link", 12, 1)  /* FIXME: PRM says 0x11.0x00 ?! */

DEFLINK(CMOVLBS_LINK, 0x14, "cmovlbs_link", 12, 1)

DEFLINK(CMOVLBC_LINK, 0x16, "cmovlbc_link", 12, 1)

/* FIXME: BetaDyn recodes some of these for SMT insts (NOP technology)... */
DEFLINK(BIS_LINK, 0x20, "bis_link", 12, 1)

DEFLINK(CMOVEQ_LINK, 0x24, "cmoveq_link", 12, 1)

DEFLINK(CMOVNE_LINK, 0x26, "cmovne_link", 12, 1)

DEFLINK(ORNOT_LINK, 0x28, "ornot_link", 12, 1)

DEFLINK(XOR_LINK, 0x40, "xor_link", 12, 1)

DEFLINK(CMOVLT_LINK, 0x44, "cmovlt_link", 12, 1)

DEFLINK(CMOVGE_LINK, 0x46, "cmovge_link", 12, 1)

DEFLINK(EQV_LINK, 0x48, "eqv_link", 12, 1)

/* added 02/27/99, plakal@cecil, from Alpha Arch Handbook (Rev.4, EV6) */
DEFLINK(AMASK_LINK, 0x61, "amask_link", 12, 1)

DEFLINK(CMOVLE_LINK, 0x64, "cmovle_link", 12, 1)

DEFLINK(CMOVGT_LINK, 0x66, "cmovgt_link", 12, 1)

/* added 02/27/99, plakal@cecil, from Alpha Arch Handbook (Rev.4, EV6) */
 
//major implementation version of processor, used for code-scheduling decisions
//rather than ISA decisions. Makes sense to return EV6 -- for sim-outorder
#define IMPLVER_IMPL							\
{									\
	SET_GPR(RC, ULL(2));					        \
}
DEFINST(IMPLVER,		0x6c,
	"implver",		"c",
	NA,			NA,
	DGPR(RC), DNA,		DNA, DNA, DNA)


CONNECT(AND_LINK)

#define AND_IMPL							\
{									\
	SET_GPR(RC, GPR(RA) & GPR(RB));					\
}
DEFINST(AND,			0x00,
	"and",			"a,b,c",
	IntALU,			F_ICOMP,
	DGPR(RC), DNA,		DGPR(RA), DGPR(RB), DNA)

#define ANDI_IMPL							\
{									\
	SET_GPR(RC, GPR(RA) & IMM);					\
}
DEFINST(ANDI,			0x01,
	"and",			"a,i,c",
	IntALU,			F_ICOMP|F_IMM,
	DGPR(RC), DNA,		DGPR(RA), DNA, DNA)


CONNECT(BIC_LINK)

#define BIC_IMPL							\
{									\
	SET_GPR(RC, GPR(RA) & ~GPR(RB));				\
}
DEFINST(BIC,			0x00,
	"bic",			"a,b,c",
	IntALU,			F_ICOMP,
	DGPR(RC), DNA,		DGPR(RA), DGPR(RB), DNA)

#define BICI_IMPL							\
{									\
	SET_GPR(RC, GPR(RA) & ~IMM);					\
}
DEFINST(BICI,			0x01,
	"bic",			"a,i,c",
	IntALU,			F_ICOMP|F_IMM,
	DGPR(RC), DNA,		DGPR(RA), DNA, DNA)


CONNECT(CMOVLBS_LINK)

#define CMOVLBS_IMPL							\
{									\
	if(GPR(RA) & 1)							\
		SET_GPR(RC, GPR(RB));					\
}
DEFINST(CMOVLBS,		0x00,
	"cmovlbs",		"a,b,c",
	IntALU,			F_ICOMP,
	DGPR(RC), DNA,		DGPR(RA), DGPR(RB), DNA)

#define CMOVLBSI_IMPL							\
{									\
	if(GPR(RA) & 1)							\
		SET_GPR(RC, IMM);					\
}
DEFINST(CMOVLBSI,		0x01,
	"cmovlbs",		"a,i,c",
	IntALU,			F_ICOMP|F_IMM,
	DGPR(RC), DNA,		DGPR(RA), DNA, DNA)


CONNECT(CMOVLBC_LINK)

#define CMOVLBC_IMPL							\
{									\
	if((GPR(RA) & 1) == 0)						\
		SET_GPR(RC, GPR(RB));					\
}
DEFINST(CMOVLBC,		0x00,
	"cmovlbc",		"a,b,c",
	IntALU,			F_ICOMP,
	DGPR(RC), DNA,		DGPR(RA), DGPR(RB), DNA)

#define CMOVLBCI_IMPL							\
{									\
	if((GPR(RA) & 1) == 0)						\
		SET_GPR(RC, IMM);					\
}
DEFINST(CMOVLBCI,		0x01,
	"cmovlbc",		"a,i,c",
	IntALU,			F_ICOMP|F_IMM,
	DGPR(RC), DNA,		DGPR(RA), DNA, DNA)


CONNECT(BIS_LINK)

#define BIS_IMPL							\
{									\
	SET_GPR(RC, GPR(RA) | GPR(RB));					\
}
DEFINST(BIS,			0x00,
	"bis",			"a,b,c",
	IntALU,			F_ICOMP,
	DGPR(RC), DNA,		DGPR(RA), DGPR(RB), DNA)

#define BISI_IMPL							\
{									\
	SET_GPR(RC, GPR(RA) | IMM);					\
}
DEFINST(BISI,			0x01,
	"bis",			"a,i,c",
	IntALU,			F_ICOMP|F_IMM,
	DGPR(RC), DNA,		DGPR(RA), DNA, DNA)


CONNECT(CMOVEQ_LINK)

#define CMOVEQ_IMPL							\
{									\
	if(GPR(RA) == 0)						\
		SET_GPR(RC, GPR(RB));					\
}
DEFINST(CMOVEQ,			0x00,
	"cmoveq",		"a,b,c",
	IntALU,			F_ICOMP,
	DGPR(RC), DNA,		DGPR(RA), DGPR(RB), DNA)

#define CMOVEQI_IMPL							\
{									\
	if(GPR(RA) == 0)						\
		SET_GPR(RC, IMM);					\
}
DEFINST(CMOVEQI,		0x01,
	"cmoveq",		"a,i,c",
	IntALU,			F_ICOMP|F_IMM,
	DGPR(RC), DNA,		DGPR(RA), DNA, DNA)


CONNECT(CMOVNE_LINK)

#define CMOVNE_IMPL							\
{									\
	if(GPR(RA) != 0)						\
		SET_GPR(RC, GPR(RB));					\
}
DEFINST(CMOVNE,			0x00,
	"cmovne",		"a,b,c",
	IntALU,			F_ICOMP,
	DGPR(RC), DNA,		DGPR(RA), DGPR(RB), DNA)

#define CMOVNEI_IMPL							\
{									\
	if(GPR(RA) != 0)						\
		SET_GPR(RC, IMM);					\
}
DEFINST(CMOVNEI,		0x01,
	"cmovne",		"a,i,c",
	IntALU,			F_ICOMP|F_IMM,
	DGPR(RC), DNA,		DGPR(RA), DNA, DNA)


CONNECT(ORNOT_LINK)

#define ORNOT_IMPL							\
{									\
	SET_GPR(RC, GPR(RA) | ~GPR(RB));				\
}
DEFINST(ORNOT,			0x00,
	"ornot",		"a,b,c",
	IntALU,			F_ICOMP,
	DGPR(RC), DNA,		DGPR(RA), DGPR(RB), DNA)

#define ORNOTI_IMPL							\
{									\
	SET_GPR(RC, GPR(RA) | ~IMM);					\
}
DEFINST(ORNOTI,			0x01,
	"ornot",		"a,i,c",
	IntALU,			F_ICOMP|F_IMM,
	DGPR(RC), DNA,		DGPR(RA), DNA, DNA)


CONNECT(XOR_LINK)

#define XOR_IMPL							\
{									\
	SET_GPR(RC, GPR(RA) ^ GPR(RB));					\
}
DEFINST(XOR,			0x00,
	"xor",			"a,b,c",
	IntALU,			F_ICOMP,
	DGPR(RC), DNA,		DGPR(RA), DGPR(RB), DNA)

#define XORI_IMPL							\
{									\
	SET_GPR(RC, GPR(RA) ^ IMM);					\
}
DEFINST(XORI,			0x01,
	"xor",			"a,i,c",
	IntALU,			F_ICOMP|F_IMM,
	DGPR(RC), DNA,		DGPR(RA), DNA, DNA)


CONNECT(CMOVLT_LINK)

#define CMOVLT_IMPL							\
{									\
	if((sqword_t)GPR(RA) < LL(0))					\
		SET_GPR(RC, GPR(RB));					\
}
DEFINST(CMOVLT,			0x00,
	"cmovlt",		"a,b,c",
	IntALU,			F_ICOMP,
	DGPR(RC), DNA,		DGPR(RA), DGPR(RB), DNA)

#define CMOVLTI_IMPL							\
{									\
	if((sqword_t)GPR(RA) < LL(0))					\
		SET_GPR(RC, IMM);					\
}
DEFINST(CMOVLTI,		0x01,
	"cmovlt",		"a,i,c",
	IntALU,			F_ICOMP|F_IMM,
	DGPR(RC), DNA,		DGPR(RA), DNA, DNA)


CONNECT(CMOVGE_LINK)

#define CMOVGE_IMPL							\
{									\
	if((sqword_t)GPR(RA) >= LL(0))					\
		SET_GPR(RC, GPR(RB));					\
}
DEFINST(CMOVGE,			0x00,
	"cmovge",		"a,b,c",
	IntALU,			F_ICOMP,
	DGPR(RC), DNA,		DGPR(RA), DGPR(RB), DNA)

#define CMOVGEI_IMPL							\
{									\
	if((sqword_t)GPR(RA) >= LL(0))					\
		SET_GPR(RC, IMM);					\
}
DEFINST(CMOVGEI,		0x01,
	"cmovge",		"a,i,c",
	IntALU,			F_ICOMP|F_IMM,
	DGPR(RC), DNA,		DGPR(RA), DNA, DNA)


CONNECT(EQV_LINK)

#define EQV_IMPL							\
{									\
	SET_GPR(RC, GPR(RA) ^ ~GPR(RB));				\
}
DEFINST(EQV,			0x00,
	"eqv",			"a,b,c",
	IntALU,			F_ICOMP,
	DGPR(RC), DNA,		DGPR(RA), DGPR(RB), DNA)

#define EQVI_IMPL							\
{									\
	SET_GPR(RC, GPR(RA) ^ ~IMM);					\
}
DEFINST(EQVI,			0x01,
	"eqv",			"a,i,c",
	IntALU,			F_ICOMP|F_IMM,
	DGPR(RC), DNA,		DGPR(RA), DNA, DNA)


/* added 02/27/99, plakal@cecil, from Alpha Arch Handbook (Rev.4, EV6) */
CONNECT(AMASK_LINK)

/* 	AMASK queries support for ISA extensions, currently we support:
	This represents (NOT CPU_feature_mask). The cleared bits listed here reflect set bits.
	BWX (clear bit 0)
	FIX (clear bit 1) 
	CIX (clear bit 2)
	MVI (clear bit 8)
	Should this be 0x303? (Pass 2?)
	Bit 9: Precise arithmetic trap reporting in hardware.
*/
#define AMASK_IMPL							\
{									\
	SET_GPR(RC, GPR(RB) & ULL(0xfffffffffffffef8));			\
}
DEFINST(AMASK,			0x00,
	"amask",		"b,c",
	NA,			NA,
	DGPR(RC), DNA,		DGPR(RB), DNA, DNA)

#define AMASKI_IMPL							\
{									\
	SET_GPR(RC, IMM & ULL(0xfffffffffffffef8));			\
}
DEFINST(AMASKI,			0x01,
	"amask",		"i,c",
	NA,			F_IMM,
	DGPR(RC), DNA,		DNA, DNA, DNA)


CONNECT(CMOVLE_LINK)

#define CMOVLE_IMPL							\
{									\
	if((sqword_t)GPR(RA) <= LL(0))					\
		SET_GPR(RC, GPR(RB));					\
}
DEFINST(CMOVLE,			0x00,
	"cmovle",		"a,b,c",
	IntALU,			F_ICOMP,
	DGPR(RC), DNA,		DGPR(RA), DGPR(RB), DNA)

#define CMOVLEI_IMPL							\
{									\
	if((sqword_t)GPR(RA) <= LL(0))					\
		SET_GPR(RC, IMM);					\
}
DEFINST(CMOVLEI,		0x01,
	"cmovle",		"a,i,c",
	IntALU,			F_ICOMP|F_IMM,
	DGPR(RC), DNA,		DGPR(RA), DNA, DNA)


CONNECT(CMOVGT_LINK)

#define CMOVGT_IMPL							\
{									\
	if((sqword_t)GPR(RA) > LL(0))					\
		SET_GPR(RC, GPR(RB));					\
}
DEFINST(CMOVGT,			0x00,
	"cmovgt",		"a,b,c",
	IntALU,			F_ICOMP,
	DGPR(RC), DNA,		DGPR(RA), DGPR(RB), DNA)

#define CMOVGTI_IMPL							\
{									\
	if((sqword_t)GPR(RA) > LL(0))					\
		SET_GPR(RC, IMM);					\
}
DEFINST(CMOVGTI,		0x01,
	"cmovgt",		"a,i,c",
	IntALU,			F_ICOMP|F_IMM,
	DGPR(RC), DNA,		DGPR(RA), DNA, DNA)


CONNECT(INTS)

DEFLINK(MSKBL_LINK, 0x02, "mskbl_link", 12, 1)

DEFLINK(EXTBL_LINK, 0x06, "extbl_link", 12, 1)

DEFLINK(INSBL_LINK, 0x0b, "insbl_link", 12, 1)

DEFLINK(MSKWL_LINK, 0x12, "mskwl_link", 12, 1)

DEFLINK(EXTWL_LINK, 0x16, "extwl_link", 12, 1)

DEFLINK(INSWL_LINK, 0x1b, "inswl_link", 12, 1)

DEFLINK(MSKLL_LINK, 0x22, "mskll_link", 12, 1)

DEFLINK(EXTLL_LINK, 0x26, "extll_link", 12, 1)

DEFLINK(INSLL_LINK, 0x2b, "insll_link", 12, 1)

DEFLINK(ZAP_LINK, 0x30, "zap_link", 12, 1)

DEFLINK(ZAPNOT_LINK, 0x31, "zapnot_link", 12, 1)

DEFLINK(MSKQL_LINK, 0x32, "mskql_link", 12, 1)

DEFLINK(SRL_LINK, 0x34, "srl_link", 12, 1)

DEFLINK(EXTQL_LINK, 0x36, "extql_link", 12, 1)

DEFLINK(SLL_LINK, 0x39, "sll_link", 12, 1)

DEFLINK(INSQL_LINK, 0x3b, "insql_link", 12, 1)

DEFLINK(SRA_LINK, 0x3c, "sra_link", 12, 1)

DEFLINK(MSKWH_LINK, 0x52, "mskwh_link", 12, 1)

DEFLINK(INSWH_LINK, 0x57, "inswh_link", 12, 1)

DEFLINK(EXTWH_LINK, 0x5a, "extwh_link", 12, 1)

DEFLINK(MSKLH_LINK, 0x62, "msklh_link", 12, 1)

DEFLINK(INSLH_LINK, 0x67, "inslh_link", 12, 1)

DEFLINK(EXTLH_LINK, 0x6a, "extlh_link", 12, 1)

DEFLINK(MSKQH_LINK, 0x72, "mskqh_link", 12, 1)

DEFLINK(INSQH_LINK, 0x77, "insqh_link", 12, 1)

DEFLINK(EXTQH_LINK, 0x7a, "extqh_link", 12, 1)


CONNECT(MSKBL_LINK)

#define MSKBL_IMPL							\
{									\
	SET_GPR(RC, GPR(RA) & ~(ULL(0xff) << ((GPR(RB) & 0x7) * 8)));	\
}
DEFINST(MSKBL,			0x00,
	"mskbl",		"a,b,c",
	IntALU,			F_ICOMP,
	DGPR(RC), DNA,		DGPR(RA), DGPR(RB), DNA)

#define MSKBLI_IMPL							\
{									\
	SET_GPR(RC, GPR(RA) & ~(ULL(0xff) << ((IMM & 0x7) * 8)));	\
}
DEFINST(MSKBLI,			0x01,
	"mskbl",		"a,i,c",
	IntALU,			F_ICOMP|F_IMM,
	DGPR(RC), DNA,		DGPR(RA), DNA, DNA)


CONNECT(EXTBL_LINK)

#define EXTBL_IMPL							\
{									\
	SET_GPR(RC, (GPR(RA) >> ((GPR(RB) & 0x7) * 8)) & LL(0xff));		\
}
DEFINST(EXTBL,			0x00,
	"extbl",		"a,b,c",
	IntALU,			F_ICOMP,
	DGPR(RC), DNA,		DGPR(RA), DGPR(RB), DNA)

#define EXTBLI_IMPL							\
{									\
	SET_GPR(RC, (GPR(RA) >> ((IMM & 0x7) * 8)) & LL(0xff));		\
}
DEFINST(EXTBLI,			0x01,
	"extbl",		"a,i,c",
	IntALU,			F_ICOMP|F_IMM,
	DGPR(RC), DNA,		DGPR(RA), DNA, DNA)


CONNECT(INSBL_LINK)

#define INSBL_IMPL							\
{									\
	SET_GPR(RC, ((GPR(RA) << ((GPR(RB) & 0x7) * 8))			\
		& (ULL(0xff) << ((GPR(RB) & 0x7) * 8))));		\
}
DEFINST(INSBL,			0x00,
	"insbl",		"a,b,c",
	IntALU,			F_ICOMP,
	DGPR(RC), DNA,		DGPR(RA), DGPR(RB), DNA)

#define INSBLI_IMPL							\
{									\
	SET_GPR(RC, ((GPR(RA) << ((IMM & 0x7) * 8))			\
		& (ULL(0xff) << ((IMM & 0x7) * 8))));			\
}
DEFINST(INSBLI,			0x01,
	"insbl",		"a,i,c",
	IntALU,			F_ICOMP|F_IMM,
	DGPR(RC), DNA,		DGPR(RA), DNA, DNA)


CONNECT(MSKWL_LINK)

#define MSKWL_IMPL							\
{									\
	SET_GPR(RC, GPR(RA) & ~(ULL(0xffff) << ((GPR(RB) & 0x7) * 8)));	\
}
DEFINST(MSKWL,			0x00,
	"mskwl",		"a,b,c",
	IntALU,			F_ICOMP,
	DGPR(RC), DNA,		DGPR(RA), DGPR(RB), DNA)

#define MSKWLI_IMPL							\
{									\
	SET_GPR(RC, GPR(RA) & ~(ULL(0xffff) << ((IMM & 0x7) * 8)));	\
}
DEFINST(MSKWLI,			0x01,
	"mskwl",		"a,i,c",
	IntALU,			F_ICOMP|F_IMM,
	DGPR(RC), DNA,		DGPR(RA), DNA, DNA)


CONNECT(EXTWL_LINK)

#define EXTWL_IMPL							\
{									\
	SET_GPR(RC, (GPR(RA) >> ((GPR(RB) & 0x7) * 8)) & LL(0xffff));	\
}
DEFINST(EXTWL,			0x00,
	"extwl",		"a,b,c",
	IntALU,			F_ICOMP,
	DGPR(RC), DNA,		DGPR(RA), DGPR(RB), DNA)

#define EXTWLI_IMPL							\
{									\
	SET_GPR(RC, (GPR(RA) >> ((IMM & 0x7) * 8)) & LL(0xffff));	\
}
DEFINST(EXTWLI,			0x01,
	"extwl",		"a,i,c",
	IntALU,			F_ICOMP|F_IMM,
	DGPR(RC), DNA,		DGPR(RA), DNA, DNA)


CONNECT(INSWL_LINK)

#define INSWL_IMPL							\
{									\
	SET_GPR(RC, ((GPR(RA) << ((GPR(RB) & 0x7) * 8))			\
		& (ULL(0xffff) << ((GPR(RB) & 0x7) * 8))));		\
}
DEFINST(INSWL,			0x00,
	"inswl",		"a,b,c",
	IntALU,			F_ICOMP,
	DGPR(RC), DNA,		DGPR(RA), DGPR(RB), DNA)

#define INSWLI_IMPL							\
{									\
	SET_GPR(RC, ((GPR(RA) << ((IMM & 0x7) * 8))			\
		& (ULL(0xffff) << ((IMM & 0x7) * 8))));			\
}
DEFINST(INSWLI,			0x01,
	"inswl",		"a,i,c",
	IntALU,			F_ICOMP|F_IMM,
	DGPR(RC), DNA,		DGPR(RA), DNA, DNA)


CONNECT(MSKLL_LINK)

#define MSKLL_IMPL								\
{										\
	SET_GPR(RC, GPR(RA) & ~(ULL(0xffffffff) << ((GPR(RB) & 0x7) * 8)));	\
}
DEFINST(MSKLL,			0x00,
	"mskll",		"a,b,c",
	IntALU,			F_ICOMP,
	DGPR(RC), DNA,		DGPR(RA), DGPR(RB), DNA)

#define MSKLLI_IMPL							\
{									\
	SET_GPR(RC, GPR(RA) & ~(ULL(0xffffffff) << ((IMM & 0x7) * 8)));	\
}
DEFINST(MSKLLI,			0x01,
	"mskll",		"a,i,c",
	IntALU,			F_ICOMP|F_IMM,
	DGPR(RC), DNA,		DGPR(RA), DNA, DNA)


CONNECT(EXTLL_LINK)

#define EXTLL_IMPL								\
{										\
	SET_GPR(RC, (GPR(RA) >> ((GPR(RB) & 0x7) * 8)) & LL(0xffffffff));	\
}
DEFINST(EXTLL,			0x00,
	"extll",		"a,b,c",
	IntALU,			F_ICOMP,
	DGPR(RC), DNA,		DGPR(RA), DGPR(RB), DNA)

#define EXTLLI_IMPL							\
{									\
	SET_GPR(RC, (GPR(RA) >> ((IMM & 0x7) * 8)) & LL(0xffffffff));	\
}
DEFINST(EXTLLI,			0x01,
	"extll",		"a,i,c",
	IntALU,			F_ICOMP|F_IMM,
	DGPR(RC), DNA,		DGPR(RA), DNA, DNA)


CONNECT(INSLL_LINK)

#define INSLL_IMPL							\
{									\
	SET_GPR(RC, ((GPR(RA) << ((GPR(RB) & 0x7) * 8))			\
		& (ULL(0xffffffff) << ((GPR(RB) & 0x7) * 8))));		\
}
DEFINST(INSLL,			0x00,
	"insll",		"a,b,c",
	IntALU,			F_ICOMP,
	DGPR(RC), DNA,		DGPR(RA), DGPR(RB), DNA)

#define INSLLI_IMPL							\
{									\
	SET_GPR(RC, ((GPR(RA) << ((IMM & 0x7) * 8))			\
		& (ULL(0xffffffff) << ((IMM & 0x7) * 8))));		\
}
DEFINST(INSLLI,			0x01,
	"insll",		"a,i,c",
	IntALU,			F_ICOMP|F_IMM,
	DGPR(RC), DNA,		DGPR(RA), DNA, DNA)


CONNECT(ZAP_LINK)

#define ZAP_IMPL							\
{									\
	int _i;								\
	qword_t _temp = LL(0xff);					\
	qword_t _rav = GPR(RA);						\
	qword_t _rbv = GPR(RB);						\
									\
	SET_GPR(RC, 0);							\
	for(_i = 1; _i < 0x100; _i = _i << 1)				\
	{								\
		if(_i & ~_rbv)						\
			SET_GPR(RC, GPR(RC) | (_rav & _temp));		\
		_temp = _temp << 8;					\
	}								\
}
DEFINST(ZAP,			0x00,
	"zap",			"a,b,c",
	IntALU,			F_ICOMP,
	DGPR(RC), DNA,		DGPR(RA), DGPR(RB), DNA)

#define ZAPI_IMPL							\
{									\
	int _i;								\
	qword_t _temp = LL(0xff);					\
	qword_t _rav = GPR(RA);						\
	qword_t _rbv = IMM;						\
									\
	SET_GPR(RC, 0);							\
	for(_i = 1; _i < 0x100; _i = _i << 1)				\
	{								\
		if(_i & ~_rbv)						\
			SET_GPR(RC, GPR(RC) | (_rav & _temp));		\
		_temp = _temp << 8;					\
	}								\
}
DEFINST(ZAPI,			0x01,
	"zap",			"a,i,c",
	IntALU,			F_ICOMP|F_IMM,
	DGPR(RC), DNA,		DGPR(RA), DNA, DNA)


CONNECT(ZAPNOT_LINK)

#define ZAPNOT_IMPL							\
{									\
	int _i;								\
	qword_t _temp = LL(0xff);					\
	qword_t _rav = GPR(RA);						\
	qword_t _rbv = GPR(RB);						\
									\
	SET_GPR(RC, 0);							\
	for(_i = 1; _i < 0x100; _i = _i << 1)				\
	{								\
		if(_i & _rbv)						\
			SET_GPR(RC, GPR(RC) | (_rav & _temp));		\
		_temp = _temp << 8;					\
	}								\
}
DEFINST(ZAPNOT,			0x00,
	"zapnot",		"a,b,c",
	IntALU,			F_ICOMP,
	DGPR(RC), DNA,		DGPR(RA), DGPR(RB), DNA)

#define ZAPNOTI_IMPL							\
{									\
	int _i;								\
	qword_t _temp = LL(0xff);					\
	qword_t _rav = GPR(RA);						\
	qword_t _rbv = IMM;						\
									\
	SET_GPR(RC, 0);							\
	for(_i = 1; _i < 0x100; _i = _i << 1)				\
	{								\
		if(_i & _rbv)						\
			SET_GPR(RC, GPR(RC) | (_rav & _temp));		\
		_temp = _temp << 8;					\
	}								\
}
DEFINST(ZAPNOTI,		0x01,
	"zapnot",		"a,i,c",
	IntALU,			F_ICOMP|F_IMM,
	DGPR(RC), DNA,		DGPR(RA), DNA, DNA)


CONNECT(MSKQL_LINK)

#define MSKQL_IMPL									\
{											\
	SET_GPR(RC, (GPR(RA) & ~(ULL(0xffffffffffffffff) << ((GPR(RB) & 0x7) * 8))));	\
}
DEFINST(MSKQL,			0x00,
	"mskql",		"a,b,c",
	IntALU,			F_ICOMP,
	DGPR(RC), DNA,		DGPR(RA), DGPR(RB), DNA)

#define MSKQLI_IMPL									\
{											\
	SET_GPR(RC, (GPR(RA) & ~(ULL(0xffffffffffffffff) << ((IMM & 0x7) * 8))));	\
}
DEFINST(MSKQLI,			0x01,
	"mskql",		"a,i,c",
	IntALU,			F_ICOMP|F_IMM,
	DGPR(RC), DNA,		DGPR(RA), DNA, DNA)


CONNECT(SRL_LINK)

#define SRL_IMPL							\
{									\
	SET_GPR(RC, GPR(RA) >> (GPR(RB) & 0x3f));			\
}
DEFINST(SRL,			0x00,
	"srl",			"a,b,c",
	IntALU,			F_ICOMP,
	DGPR(RC), DNA,		DGPR(RA), DGPR(RB), DNA)

#define SRLI_IMPL							\
{									\
	SET_GPR(RC, GPR(RA) >> (IMM & 0x3f));				\
}
DEFINST(SRLI,			0x01,
	"srl",		"a,i,c",
	IntALU,			F_ICOMP|F_IMM,
	DGPR(RC), DNA,		DGPR(RA), DNA, DNA)


CONNECT(EXTQL_LINK)

#define EXTQL_IMPL							\
{									\
	SET_GPR(RC, GPR(RA) >> ((GPR(RB) & 0x7) * 8));			\
}
DEFINST(EXTQL,			0x00,
	"extql",		"a,b,c",
	IntALU,			F_ICOMP,
	DGPR(RC), DNA,		DGPR(RA), DGPR(RB), DNA)

#define EXTQLI_IMPL							\
{									\
	SET_GPR(RC, GPR(RA) >> ((IMM & 0x7) * 8));			\
}
DEFINST(EXTQLI,			0x01,
	"extql",		"a,i,c",
	IntALU,			F_ICOMP|F_IMM,
	DGPR(RC), DNA,		DGPR(RA), DNA, DNA)


CONNECT(SLL_LINK)

#define	SLL_IMPL							\
{									\
	SET_GPR(RC, GPR(RA) << (GPR(RB) & 0x3f));			\
}
DEFINST(SLL,			0x00,
	"sll",			"a,b,c",
	IntALU,			F_ICOMP,
	DGPR(RC), DNA,		DGPR(RA), DGPR(RB), DNA)

#define SLLI_IMPL							\
{									\
	SET_GPR(RC, GPR(RA) << (IMM & 0x3f));				\
}
DEFINST(SLLI,			0x01,
	"sll",			"a,i,c",
	IntALU,			F_ICOMP|F_IMM,
	DGPR(RC), DNA,		DGPR(RA), DNA, DNA)


CONNECT(INSQL_LINK)

#define INSQL_IMPL							\
{									\
	SET_GPR(RC, ((GPR(RA) << ((GPR(RB) & 0x7) * 8))			\
		& (ULL(0xffffffffffffffff) << ((GPR(RB) & 0x7) * 8))));	\
}
DEFINST(INSQL,			0x00,
	"insql",		"a,b,c",
	IntALU,			F_ICOMP,
	DGPR(RC), DNA,		DGPR(RA), DGPR(RB), DNA)

#define INSQLI_IMPL							\
{									\
	SET_GPR(RC, ((GPR(RA) << ((IMM & 0x7) * 8))			\
		& (ULL(0xffffffffffffffff) << ((IMM & 0x7) * 8))));	\
}
DEFINST(INSQLI,			0x01,
	"insql",		"a,i,c",
	IntALU,			F_ICOMP|F_IMM,
	DGPR(RC), DNA,		DGPR(RA), DNA, DNA)

CONNECT(SRA_LINK)

#define SRA_IMPL							\
{									\
	SET_GPR(RC, (sqword_t)GPR(RA) >> (GPR(RB) & 0x3f));		\
}
DEFINST(SRA,			0x00,
	"sra",			"a,b,c",
	IntALU,			F_ICOMP,
	DGPR(RC), DNA,		DGPR(RA), DGPR(RB), DNA)

#define SRAI_IMPL							\
{									\
	SET_GPR(RC, (sqword_t)GPR(RA) >> (IMM & 0x3f));			\
}
DEFINST(SRAI,			0x01,
	"sra",			"a,i,c",
	IntALU,			F_ICOMP|F_IMM,
	DGPR(RC), DNA,		DGPR(RA), DNA, DNA)


CONNECT(MSKWH_LINK)

#define MSKWH_IMPL									\
{											\
	if((GPR(RB) & 0x7) != 0)							\
		SET_GPR(RC, GPR(RA) & ~(ULL(0xffff) >> ((8 - (GPR(RB) & 0x7)) * 8)));	\
	else										\
		SET_GPR(RC, GPR(RA));							\
}
DEFINST(MSKWH,			0x00,
	"mskwh",		"a,b,c",
	IntALU,			F_ICOMP,
	DGPR(RC), DNA,		DGPR(RA), DGPR(RB), DNA)

#define MSKWHI_IMPL									\
{											\
	if((IMM & 0x7) != 0)								\
		SET_GPR(RC, GPR(RA) & ~(ULL(0xffff) >> ((8 - (IMM & 0x7)) * 8)));	\
	else										\
		SET_GPR(RC, GPR(RA));							\
}
DEFINST(MSKWHI,			0x01,
	"mskwh",		"a,i,c",
	IntALU,			F_ICOMP|F_IMM,
	DGPR(RC), DNA,		DGPR(RA), DNA, DNA)


CONNECT(INSWH_LINK)

#define INSWH_IMPL								\
{										\
	if((GPR(RB) & 0x7) != 0)						\
		SET_GPR(RC, ((GPR(RA) >> (63 - (GPR(RB) & 0x7) * 8)) >> 1	\
			& (ULL(0xffff) >> ((8 - (GPR(RB) & 0x7)) * 8))));	\
	else									\
		SET_GPR(RC, 0);							\
}
DEFINST(INSWH,			0x00,
	"inswh",		"a,b,c",
	IntALU,			F_ICOMP,
	DGPR(RC), DNA,		DGPR(RA), DGPR(RB), DNA)

#define INSWHI_IMPL							\
{									\
	if((IMM & 0x7) != 0)						\
		SET_GPR(RC, ((GPR(RA) >> (63 - (IMM & 0x7) * 8)) >> 1	\
			& (ULL(0xffff) >> ((8 - (IMM & 0x7)) * 8))));	\
	else								\
		SET_GPR(RC, 0);						\
}
DEFINST(INSWHI,			0x01,
	"inswh",		"a,i,c",
	IntALU,			F_ICOMP|F_IMM,
	DGPR(RC), DNA,		DGPR(RA), DNA, DNA)


CONNECT(EXTWH_LINK)

#define EXTWH_IMPL									\
{											\
	SET_GPR(RC, (GPR(RA) << ((64 - (GPR(RB)&0x7) * 8) & 0x3f)) & LL(0xffff));	\
}
DEFINST(EXTWH,			0x00,
	"extwh",		"a,b,c",
	IntALU,			F_ICOMP,
	DGPR(RC), DNA,		DGPR(RA), DGPR(RB), DNA)

#define EXTWHI_IMPL								\
{										\
	SET_GPR(RC, (GPR(RA) << ((64 - (IMM & 0x7) * 8) & 0x3f)) & LL(0xffff));	\
}
DEFINST(EXTWHI,			0x01,
	"extwh",		"a,i,c",
	IntALU,			F_ICOMP|F_IMM,
	DGPR(RC), DNA,		DGPR(RA), DNA, DNA)


CONNECT(MSKLH_LINK)

#define MSKLH_IMPL										\
{												\
	if((GPR(RB) & 0x7) != 0)								\
		SET_GPR(RC, GPR(RA) & ~(ULL(0xffffffff) >> ((8 - (GPR(RB) & 0x7)) * 8)));	\
	else											\
		SET_GPR(RC, GPR(RA));								\
}
DEFINST(MSKLH,			0x00,
	"msklh",		"a,b,c",
	IntALU,			F_ICOMP,
	DGPR(RC), DNA,		DGPR(RA), DGPR(RB), DNA)

#define MSKLHI_IMPL									\
{											\
	if((IMM & 0x7) != 0)								\
		SET_GPR(RC, GPR(RA) & ~(ULL(0xffffffff) >> ((8 - (IMM & 0x7)) * 8)));	\
	else										\
		SET_GPR(RC, GPR(RA));							\
}
DEFINST(MSKLHI,			0x01,
	"msklh",		"a,i,c",
	IntALU,			F_ICOMP|F_IMM,
	DGPR(RC), DNA,		DGPR(RA), DNA, DNA)


CONNECT(INSLH_LINK)

#define INSLH_IMPL								\
{										\
	if((GPR(RB) & 0x7) != 0)						\
		SET_GPR(RC, ((GPR(RA) >> (63 - (GPR(RB) & 0x7) * 8)) >> 1	\
			& (ULL(0xffffffff) >> ((8 - (GPR(RB) & 0x7)) * 8))));	\
	else									\
		SET_GPR(RC, 0);							\
}
DEFINST(INSLH,			0x00,
	"inslh",		"a,b,c",
	IntALU,			F_ICOMP,
	DGPR(RC), DNA,		DGPR(RA), DGPR(RB), DNA)

#define INSLHI_IMPL								\
{										\
	if((IMM & 0x7) != 0)							\
		SET_GPR(RC, ((GPR(RA) >> (63 - (IMM & 0x7) * 8)) >> 1		\
			& (ULL(0xffffffff) >> ((8 - (IMM & 0x7)) * 8))));	\
	SET_GPR(RC, 0);								\
}
DEFINST(INSLHI,			0x01,
	"inslh",		"a,i,c",
	IntALU,			F_ICOMP|F_IMM,
	DGPR(RC), DNA,		DGPR(RA), DNA, DNA)


CONNECT(EXTLH_LINK)

#define EXTLH_IMPL									\
{											\
	SET_GPR(RC, (GPR(RA) << ((64 - (GPR(RB) & 0x7) * 8) & 0x3f)) & LL(0xffffffff));	\
}
DEFINST(EXTLH,			0x00,
	"extlh",		"a,b,c",
	IntALU,			F_ICOMP,
	DGPR(RC), DNA,		DGPR(RA), DGPR(RB), DNA)

#define EXTLHI_IMPL									\
{											\
	SET_GPR(RC, (GPR(RA) << ((64 - (IMM&0x7) * 8) & 0x3f)) & LL(0xffffffff));	\
}
DEFINST(EXTLHI,			0x01,
	"extlh",		"a,i,c",
	IntALU,			F_ICOMP|F_IMM,
	DGPR(RC), DNA,		DGPR(RA), DNA, DNA)


CONNECT(MSKQH_LINK)

#define MSKQH_IMPL											\
{													\
	if((GPR(RB) & 0x7) != 0)									\
		SET_GPR(RC, (GPR(RA) & ~(ULL(0xffffffffffffffff) >> ((8 - (GPR(RB) & 0x7)) * 8))));	\
	else												\
		SET_GPR(RC, GPR(RA));									\
}
DEFINST(MSKQH,			0x00,
	"mskqh",		"a,b,c",
	IntALU,			F_ICOMP,
	DGPR(RC), DNA,		DGPR(RA), DGPR(RB), DNA)

#define MSKQHI_IMPL										\
{												\
	if((IMM & 0x7) != 0)									\
		SET_GPR(RC, (GPR(RA) & ~(ULL(0xffffffffffffffff) >> ((8 - (IMM & 0x7)) * 8))));	\
	else											\
		SET_GPR(RC, GPR(RA));								\
}
DEFINST(MSKQHI,			0x01,
	"mskqh",		"a,i,c",
	IntALU,			F_ICOMP|F_IMM,
	DGPR(RC), DNA,		DGPR(RA), DNA, DNA)


CONNECT(INSQH_LINK)

#define INSQH_IMPL									\
{											\
	if((GPR(RB) & 0x7) != 0)							\
		SET_GPR(RC, ((GPR(RA) >> (63 - (GPR(RB) & 0x7) * 8)) >> 1		\
			& (ULL(0xffffffffffffffff) >> ((8 - (GPR(RB) & 0x7)) * 8))));	\
	else										\
		SET_GPR(RC, 0);								\
}
DEFINST(INSQH,			0x00,
	"insqh",		"a,b,c",
	IntALU,			F_ICOMP,
	DGPR(RC), DNA,		DGPR(RA), DGPR(RB), DNA)

#define INSQHI_IMPL									\
{											\
	if((IMM & 0x7) != 0)								\
		SET_GPR(RC, ((GPR(RA) >> (63 - (IMM & 0x7) * 8)) >> 1			\
			& (ULL(0xffffffffffffffff) >> ((8 - (IMM & 0x7)) * 8))));	\
	else										\
		SET_GPR(RC, 0);								\
}
DEFINST(INSQHI,			0x01,
	"insqh",		"a,i,c",
	IntALU,			F_ICOMP|F_IMM,
	DGPR(RC), DNA,		DGPR(RA), DNA, DNA)


CONNECT(EXTQH_LINK)

#define EXTQH_IMPL							\
{									\
	SET_GPR(RC, (GPR(RA) << ((64 - (GPR(RB) & 0x7) * 8) & 0x3f)));	\
}
DEFINST(EXTQH,			0x00,
	"extqh",		"a,b,c",
	IntALU,			F_ICOMP,
	DGPR(RC), DNA,		DGPR(RA), DGPR(RB), DNA)

#define EXTQHI_IMPL							\
{									\
	SET_GPR(RC, (GPR(RA) << ((64 - (IMM & 0x7) * 8) & 0x3f)));	\
}
DEFINST(EXTQHI,			0x01,
	"extqh",		"a,i,c",
	IntALU,			F_ICOMP|F_IMM,
	DGPR(RC), DNA,		DGPR(RA), DNA, DNA)


CONNECT(INTM)

/* FIXME: changed mask in DEFLINK(INTM ... above so that MUL{Q,L}/V
   now map to MUL{Q,L}, i.e. no overflow checking (earlier they
   generated unimplemented-faults). I think this is more symmetrical
   because {ADD,SUB}{L,Q}/V also do not check for overflow.  */

DEFLINK(MULL_LINK, 0x00, "mull_link", 12, 1)

DEFLINK(MULQ_LINK, 0x20, "mulq_link", 12, 1)

DEFLINK(UMULH_LINK, 0x30, "umulh_link", 12, 1)


CONNECT(MULL_LINK)

#define MULL_IMPL							\
{									\
	SET_GPR(RC, SEXT32((GPR(RA) * GPR(RB)) & ULL(0xffffffff)));	\
}
DEFINST(MULL,			0x00,
	"mull",			"a,b,c",
	IntMULT,		F_ICOMP,
	DGPR(RC), DNA,		DGPR(RA), DGPR(RB), DNA)

#define MULLI_IMPL							\
{									\
	SET_GPR(RC, SEXT32((GPR(RA) * IMM) & ULL(0xffffffff)));		\
}
DEFINST(MULLI,			0x01,
	"mull",			"a,i,c",
	IntMULT,		F_ICOMP|F_IMM,
	DGPR(RC), DNA,		DGPR(RA), DNA, DNA)


CONNECT(MULQ_LINK)

#define MULQ_IMPL							\
{									\
	SET_GPR(RC, GPR(RA) * GPR(RB));					\
}
DEFINST(MULQ,			0x00,
	"mulq",			"a,b,c",
	IntMULT,		F_ICOMP,
	DGPR(RC), DNA,		DGPR(RA), DGPR(RB), DNA)

#define MULQI_IMPL							\
{									\
	SET_GPR(RC, GPR(RA) * IMM);					\
}
DEFINST(MULQI,			0x01,
	"mulq",			"a,i,c",
	IntMULT,		F_ICOMP|F_IMM,
	DGPR(RC), DNA,		DGPR(RA), DNA, DNA)


CONNECT(UMULH_LINK)

#define UMULH_IMPL									\
{											\
	sqword_t _v1 = GPR(RA);								\
	sqword_t _v2 = GPR(RB);								\
	qword_t _result;								\
	qword_t _a, _b, _c, _d;								\
	qword_t _bd, _ad, _cb, _ac;							\
	qword_t _mid, _mid2, _carry_mid = 0;						\
											\
	_a = (_v1 >> 32) & LL(0xffffffff);						\
	_b = _v1 & LL(0xffffffff);							\
	_c = (_v2 >> 32) & LL(0xffffffff);						\
	_d = _v2 & LL(0xffffffff);							\
	/*myfprintf(stderr, "%n: %p %p %p %p\n", sim_num_insn, _a, _b, _c, _d);*/	\
											\
	_bd = _b * _d;									\
	_ad = _a * _d;									\
	_cb = _c * _b;									\
	_ac = _a * _c;									\
	/*myfprintf(stderr, "    %p %p %p %p\n", _bd, _ad, _cb, _ac);*/			\
											\
	_mid = _ad + _cb;								\
	if(ARITH_OVFL(_mid, _ad, _cb))							\
		_carry_mid = 1;								\
											\
	_mid2 = _mid + ((_bd >> 32) & LL(0xffffffff));					\
	if(ARITH_OVFL(_mid2, _mid, ((_bd >> 32) & LL(0xffffffff))))			\
		_carry_mid += 1;							\
	_result = _ac + (_carry_mid << 32) + ((_mid2 >> 32) & LL(0xffffffff));		\
	/*myfprintf(stderr, "    %p %p %p %p\n", _mid,_mid2,_carry_mid,_result);*/	\
											\
	SET_GPR(RC, _result);								\
}
DEFINST(UMULH,			0x00,
	"umulh",		"a,b,c",
	IntMULT,		F_ICOMP,
	DGPR(RC), DNA,		DGPR(RA), DGPR(RB), DNA)

#define UMULHI_IMPL								\
{										\
	sqword_t _v1 = GPR(RA);							\
	sqword_t _v2 = IMM;							\
	qword_t _result;							\
	qword_t _a, _b, _c, _d;							\
	qword_t _bd, _ad, _cb, _ac;						\
	qword_t _mid, _mid2, _carry_mid = 0;					\
										\
	_a = (_v1 >> 32) & LL(0xffffffff);					\
	_b = _v1 & LL(0xffffffff);						\
	_c = (_v2 >> 32) & LL(0xffffffff);					\
	_d = _v2 & LL(0xffffffff);						\
										\
	_bd = _b * _d;								\
	_ad = _a * _d;								\
	_cb = _c * _b;								\
	_ac = _a * _c;								\
										\
	_mid = _ad + _cb;							\
	if(ARITH_OVFL(_mid, _ad, _cb))						\
		_carry_mid = 1;							\
										\
	_mid2 = _mid + ((_bd >> 32) & LL(0xffffffff));				\
	if(ARITH_OVFL(_mid2, _mid, ((_bd >> 32) & LL(0xffffffff))))		\
		_carry_mid += 1;						\
	_result =								\
		_ac + (_carry_mid << 32) + ((_mid2 >> 32) & LL(0xffffffff));	\
										\
	SET_GPR(RC, _result);							\
}
DEFINST(UMULHI,			0x01,
	"umulh",		"a,i,c",
	IntMULT,		F_ICOMP|F_IMM,
	DGPR(RC), DNA,		DGPR(RA), DNA, DNA)


/* added 02/27/99, plakal@cecil, from Alpha Arch Handbook (Rev.4, EV6) */
/* FIX extensions */
CONNECT(ITFP)

#define ITOFS_IMPL							\
{									\
	sqword_t _longhold, _e1, _e2;					\
									\
	_longhold = GPR(RA) & ULL(0xffffffff);				\
									\
	_e1 = _longhold & 0x40000000;					\
	_e2 = (_longhold >> 23) & ULL(0x7f);				\
	if(_e1)								\
	{								\
		if(_e2 == ULL(0x7f))					\
			_e2 = ULL(0x7ff);				\
		else							\
			_e2 |= ULL(0x400);				\
	}								\
	else								\
	{								\
		if(_e2 != 0)						\
			_e2 |= ULL(0x380);				\
	}								\
	SET_FPR_Q(RC, (((_longhold & ULL(0x80000000)) << 32)		\
		| (_e2 << 52) | ((_longhold & ULL(0x7fffff)) << 29)));	\
}
DEFINST(ITOFS,			0x04,
	"itofs",	        "a,C",
	FloatCVT,	        F_FCOMP,  /* FIXME: are these flags correct? */
	DFPR(RC), DNA,		DGPR(RA), DNA, DNA)

#define SQRTF_IMPL							\
{									\
	/* FIXME: unimplemented (VAX) */				\
	DECLARE_FAULT(md_fault_unimpl);					\
}
DEFINST(SQRTF,			0x0a,
	"sqrtf (unimpl)",	"B,C",
	NA,			NA,
	DNA, DNA,		DNA, DNA, DNA)

//FIXME: too much precision here
#define SQRTS_IMPL								\
{										\
	if (FPR(RB) < 0.0)							\
		DECLARE_FAULT(md_fault_invalid);				\
	SET_FPR(RC, (dfloat_t)FIX_DOUBLE(sqrt((double)FIX_DOUBLE(FPR(RB)))));	\
}

DEFINST(SQRTS,			0x0b,
	"sqrts",	        "B,C",
	FloatSQRT,		F_FCOMP,
	DFPR(RC), DNA,		DFPR(RB), DNA, DNA)

#define ITOFF_IMPL							\
{									\
	/* FIXME: unimplemented (VAX) */				\
	DECLARE_FAULT(md_fault_unimpl);					\
}
DEFINST(ITOFF,			0x14,
	"itoff (unimpl)",	"a,C",
	NA,			NA,
	DNA, DNA,		DNA, DNA, DNA)

#define ITOFT_IMPL							\
{									\
	SET_FPR_Q(RC, GPR(RA));                                             \
}
DEFINST(ITOFT,			0x24,
	"itoft",	        "a,C",
	FloatCVT,	        F_FCOMP,  /* FIXME: are these flags correct? */
	DFPR(RC), DNA,		DGPR(RA), DNA, DNA)

#define SQRTG_IMPL							\
{									\
	/* FIXME: unimplemented (VAX) */				\
	DECLARE_FAULT(md_fault_unimpl);					\
}
DEFINST(SQRTG,			0x2a,
	"sqrtg (unimpl)",	"B,C",
	NA,			NA,
	DNA, DNA,		DNA, DNA, DNA)

#define SQRTT_IMPL							\
{									\
	if(FPR(RB) < 0.0)						\
		DECLARE_FAULT(md_fault_invalid);			\
									\
	SET_FPR(RC, (dfloat_t)sqrt((double)FPR(RB)));			\
}
DEFINST(SQRTT,			0x2b,
	"sqrtt",	        "B,C",
	NA,			NA,
	DFPR(RC), DNA,		DFPR(RB), DNA, DNA)


CONNECT(FLTI)

#define ADDS_IMPL									\
{											\
	/* FIXME: too much precision here... */						\
/*	SET_FPR(RC, FPR(RA) + FPR(RB));		*/					\
	SET_FPR(RC, FIX_DOUBLE((FIX_DOUBLE(FPR(RA)) + FIX_DOUBLE(FPR(RB)))));		\
}
DEFINST(ADDS,			0x00,
	"adds",			"A,B,C",
	FloatADD,		F_FCOMP,
	DFPR(RC), DNA,		DFPR(RA), DFPR(RB), DNA)

#define SUBS_IMPL								\
{										\
	/* FIXME: too much precision here... */					\
/*	SET_FPR(RC, FPR(RA) - FPR(RB));			*/			\
	SET_FPR(RC, FIX_DOUBLE((FIX_DOUBLE(FPR(RA)) - FIX_DOUBLE(FPR(RB)))));	\
}
DEFINST(SUBS,			0x01,
	"subs",			"A,B,C",
	FloatADD,		F_FCOMP,
	DFPR(RC), DNA,		DFPR(RA), DFPR(RB), DNA)

#define MULS_IMPL								\
{										\
	/* FIXME: too much precision here... */					\
/*	SET_FPR(RC, FPR(RA) * FPR(RB));		*/				\
	SET_FPR(RC, FIX_DOUBLE((FIX_DOUBLE(FPR(RA)) * FIX_DOUBLE(FPR(RB)))));	\
}
DEFINST(MULS,			0x02,
	"muls",			"A,B,C",
	FloatMULT,		F_FCOMP,
	DFPR(RC), DNA,		DFPR(RA), DFPR(RB), DNA)

#define DIVS_IMPL								\
{										\
	/* FIXME: too much precision here... */					\
/*	SET_FPR(RC, FPR(RA) / FPR(RB));			*/			\
	SET_FPR(RC, FIX_DOUBLE((FIX_DOUBLE(FPR(RA)) / FIX_DOUBLE(FPR(RB)))));	\
}
DEFINST(DIVS,			0x03,
	"divs",			"A,B,C",
	FloatDIV,		F_FCOMP,
	DFPR(RC), DNA,		DFPR(RA), DFPR(RB), DNA)

#define ADDT_IMPL							\
{									\
	SET_FPR(RC, FPR(RA) + FPR(RB));					\
}
DEFINST(ADDT,			0x20,
	"addt",			"A,B,C",
	FloatADD,			F_FCOMP,
	DFPR(RC), DNA,		DFPR(RA), DFPR(RB), DNA)

#define SUBT_IMPL							\
{									\
	SET_FPR(RC, FPR(RA) - FPR(RB));					\
}
DEFINST(SUBT,			0x21,
	"subt",			"A,B,C",
	FloatADD,			F_FCOMP,
	DFPR(RC), DNA,		DFPR(RA), DFPR(RB), DNA)

#define MULT_IMPL							\
{									\
	SET_FPR(RC, FPR(RA) * FPR(RB));					\
}
DEFINST(MULT,			0x22,
	"mult",			"A,B,C",
	FloatMULT,		F_FCOMP,
	DFPR(RC), DNA,		DFPR(RA), DFPR(RB), DNA)

#define DIVT_IMPL							\
{									\
	SET_FPR(RC, FPR(RA) / FPR(RB));					\
}
DEFINST(DIVT,			0x23,
	"divt",			"A,B,C",
	FloatDIV,		F_FCOMP,
	DFPR(RC), DNA,		DFPR(RA), DFPR(RB), DNA)

#define CMPTUN_IMPL									\
{											\
	SET_FPR(RC, (IS_IEEEFP_DBL_NAN(FPR_Q(RA)) || IS_IEEEFP_DBL_NAN(FPR_Q(RB)))	\
		? 2.0 : 0.0);								\
}
DEFINST(CMPTUN,			0x24,
	"cmptun",	        "A,B,C",
	FloatCMP,		F_FCOMP,
	DFPR(RC), DNA,		DFPR(RA), DFPR(RB), DNA)

#define CMPTEQ_IMPL								\
{										\
	SET_FPR(RC, (((FPR_Q(RA) == FPR_Q(RB))					\
		|| (FPR_Q(RA) << 1 == ULL(0) && FPR_Q(RB) << 1 == ULL(0)))	\
			? 2.0 : 0.0));						\
}
DEFINST(CMPTEQ,			0x25,
	"cmpteq",		"A,B,C",
	FloatADD,		F_FCOMP,
	DFPR(RC), DNA,		DFPR(RA), DFPR(RB), DNA)

#define CMPTLT_IMPL							\
{									\
	SET_FPR(RC, (FPR(RA) < FPR(RB)) ? 2.0 : 0.0);			\
}
DEFINST(CMPTLT,			0x26,
	"cmptlt",		"A,B,C",
	FloatADD,		F_FCOMP,
	DFPR(RC), DNA,		DFPR(RA), DFPR(RB), DNA)

#define CMPTLE_IMPL							\
{									\
	SET_FPR(RC, (FPR(RA) <= FPR(RB)) ? 2.0 : 0.0);			\
}
DEFINST(CMPTLE,			0x27,
	"cmptle",		"A,B,C",
	FloatADD,		F_FCOMP,
	DFPR(RC), DNA,		DFPR(RA), DFPR(RB), DNA)

#define CVTTS_IMPL							\
{									\
	SET_FPR(RC, (float)(FPR(RB)));					\
}
DEFINST(CVTTS,			0x2c,
	"cvtts",		"B,C",
	FloatADD,		F_FCOMP,
	DFPR(RC), DNA,		DFPR(RB), DNA, DNA)

#define CVTTQ_IMPL							\
{									\
	SET_FPR_Q(RC, (sqword_t)FPR(RB));				\
}
DEFINST(CVTTQ,			0x2f,
	"cvttq",		"B,C",
	FloatADD,		F_FCOMP,
	DFPR(RC), DNA,		DFPR(RB), DNA, DNA)

#define CVTQS_IMPL							\
{									\
	SET_FPR(RC, FIX_DOUBLE((sqword_t)(FPR_Q(RB))));			\
}
DEFINST(CVTQS,			0x3c,
	"cvtqs",		"B,C",
	FloatADD,		F_FCOMP,
	DFPR(RC), DNA,		DFPR(RB), DNA, DNA)

#define CVTQT_IMPL							\
{									\
	SET_FPR(RC, (dfloat_t)(sqword_t)FPR_Q(RB));			\
}
DEFINST(CVTQT,			0x3e,
	"cvtqt",		"B,C",
	FloatADD,		F_FCOMP,
	DFPR(RC), DNA,		DFPR(RB), DNA, DNA)


CONNECT(FLTL)

#define CVTLQ_IMPL							\
{									\
	sqword_t _longhold;						\
	sword_t _inthold;						\
									\
	_longhold = FPR_Q(RB);						\
	_inthold = (((_longhold >> 32) & 0xc0000000)			\
		| ((_longhold >> 29) & 0x3fffffff));			\
	SET_FPR_Q(RC, (sqword_t)_inthold);				\
}
DEFINST(CVTLQ,			0x10,
	"cvtlq",		"B,C",
	FloatADD,		F_FCOMP,
	DFPR(RC), DNA,		DFPR(RB), DNA, DNA)

#define CPYS_IMPL							\
{									\
	SET_FPR_Q(RC, ((FPR_Q(RA) & (ULL(1) << 63))			\
		| (FPR_Q(RB) & LL(0x7fffffffffffffff))));		\
}
DEFINST(CPYS,			0x20,
	"cpys",			"A,B,C",
	FloatADD,		F_FCOMP,
	DFPR(RC), DNA,		DFPR(RA), DFPR(RB), DNA)

#define CPYSN_IMPL							\
{									\
	SET_FPR_Q(RC, (((FPR_Q(RA) >> 63) ^ 1) << 63			\
		| (FPR_Q(RB) & LL(0x7fffffffffffffff))));		\
}
DEFINST(CPYSN,			0x21,
	"cpysn",		"A,B,C",
	FloatADD,		F_FCOMP,
	DFPR(RC), DNA,		DFPR(RA), DFPR(RB), DNA)

#define CPYSE_IMPL							\
{									\
	SET_FPR_Q(RC, ((FPR_Q(RA) & ULL(0xfff) << 52)			\
		| (FPR_Q(RB) & ULL(0xfffffffffffff))));			\
}
DEFINST(CPYSE,			0x22,
	"cpyse",		"A,B,C",
	FloatADD,		F_FCOMP,
	DFPR(RC), DNA,		DFPR(RA), DFPR(RB), DNA)

#define MT_FPCR_IMPL								\
{										\
	/* FIXED, 02/26/99, plakal@cecil, Glew's fix, read FP reg as qword */	\
	SET_FPCR(FPR_Q(RA));							\
}
DEFINST(MT_FPCR,		0x24,
	"mt_fpcr",		"A",
	FloatADD,		F_FCOMP,
	DFPCR, DNA,		DFPR(RA), DNA, DNA)

#define MF_FPCR_IMPL									\
{											\
	/* FIXME: qword_t to double conversion not implemented in MSC */		\
	/* FIXED, 02/26/99, plakal@cecil, using Glew's fix, set FP reg as qword */	\
	SET_FPR_Q(RA, FPCR);								\
}
DEFINST(MF_FPCR,		0x25,
	"mf_fpcr",		"A",
	FloatADD,		F_FCOMP,
	DFPR(RA), DNA,		DFPCR, DNA, DNA)

#define FCMOVEQ_IMPL							\
{									\
	if((FPR_Q(RA) << 1) == ULL(0))					\
		SET_FPR(RC, FPR(RB));					\
}
DEFINST(FCMOVEQ,		0x2a,
	"fcmoveq",		"A,B,C",
	FloatADD,		F_FCOMP,
	DFPR(RC), DNA,		DFPR(RA), DFPR(RB), DNA)

#define FCMOVNE_IMPL							\
{									\
	if((FPR_Q(RA) << 1) != ULL(0))					\
		SET_FPR(RC, FPR(RB));					\
}
DEFINST(FCMOVNE,		0x2b,
	"fcmovne",		"A,B,C",
	FloatADD,		F_FCOMP,
	DFPR(RC), DNA,		DFPR(RA), DFPR(RB), DNA)

#define FCMOVLT_IMPL							\
{									\
	if(((FPR_Q(RA) << 1) != ULL(0)) && (FPR_Q(RA) >> 63))		\
		SET_FPR(RC, FPR(RB));					\
}
DEFINST(FCMOVLT,		0x2c,
	"fcmovlt",		"A,B,C",
	FloatADD,		F_FCOMP,
	DFPR(RC), DNA,		DFPR(RA), DFPR(RB), DNA)

#define FCMOVGE_IMPL							\
{									\
	if(((FPR_Q(RA) << 1) == ULL(0)) || (FPR_Q(RA) >> 63 == ULL(0)))	\
		SET_FPR(RC, FPR(RB));					\
}
DEFINST(FCMOVGE,		0x2d,
	"fcmovge",		"A,B,C",
	FloatADD,		F_FCOMP,
	DFPR(RC), DNA,		DFPR(RA), DFPR(RB), DNA)

#define FCMOVLE_IMPL							\
{									\
	if(((FPR_Q(RA) << 1) == ULL(0)) || (FPR_Q(RA) >> 63))		\
		SET_FPR(RC, FPR(RB));					\
}
DEFINST(FCMOVLE,		0x2e,
	"fcmovle",		"A,B,C",
	FloatADD,		F_FCOMP,
	DFPR(RC), DNA,		DFPR(RA), DFPR(RB), DNA)

#define FCMOVGT_IMPL							\
{									\
	if(((FPR_Q(RA) << 1) != ULL(0)) && (FPR_Q(RA) >> 63 == ULL(0)))	\
		SET_FPR(RC, FPR(RB));					\
}
DEFINST(FCMOVGT,		0x2f,
	"fcmovgt",		"A,B,C",
	FloatADD,		F_FCOMP,
	DFPR(RC), DNA,		DFPR(RA), DFPR(RB), DNA)

/* FIXME: CVTQLV and CVTQLSV should map to here... */
#define CVTQL_IMPL							\
{									\
	sqword_t longhold = FPR_Q(RB);					\
	SET_FPR_Q(RC, (((longhold & 0xc0000000) << 32)			\
		| (longhold & LL(0x3fffffff)) << 29));			\
}
DEFINST(CVTQL,			0x30,
	"cvtql",		"B,C",
	FloatADD,		F_FCOMP,
	DFPR(RC), DNA,		DFPR(RB), DNA, DNA)


CONNECT(MISC)

/* Note: some DEFINSTs below have different MSK values because the
   mask & shifts in the MISC link above have changed */

#define TRAPB_IMPL						\
{								\
	/* Nothing, and this is fine */				\
}
DEFINST(TRAPB,			0x00,
	"trapb",		"",
	NA,			F_TRAP,
	DNA, DNA,		DNA, DNA, DNA)

/* added 02/27/99, plakal@cecil, from Alpha Arch Handbook (Rev.4, EV6) */
//Nothing to do actually. This is a barrier (as an F_TRAP it will cause the ROB to drain)
#define EXCB_IMPL						\
{								\
	/* Nothing, and this is fine */				\
}
DEFINST(EXCB,			0x04,
	"excb",		        "",
	NA,			F_TRAP,
	DNA, DNA,		DNA, DNA, DNA)

//From Handbook
//	Guarantees that all subsequent loads and stores will not access memory until
//	after all previous loads and stores have accessed memory, as observed by other processors
#define MB_IMPL							\
{								\
	/* This is fine */					\
}
DEFINST(MB,			0x40, /* -- Changed from 0x04 */
	"mb",			"",
	NA,			F_TRAP,
	DNA, DNA,		DNA, DNA, DNA)

/* added 02/27/99, plakal@cecil, from Alpha Arch Handbook (Rev.4, EV6) */

//Allows ordering of writes, generally, just a trap. 
#define WMB_IMPL							\
{									\
	/* FIXME: not supported... */					\
}
DEFINST(WMB,			0x44,
	"wmb",			"",
	NA,			F_TRAP,
	DNA, DNA,		DNA, DNA, DNA)

//See handbook, this is optional and "unlikely to help"
#define FETCH_IMPL						\
{								\
	/* Nothing, and this is fine */				\
}
DEFINST(FETCH,			0x80, /* -- Changed from 0x08 */
	"fetch",	        "0(b)",
	NA,			NA,
	DNA, DNA,		DNA, DNA, DNA)

//See handbook, this is optional and "unlikely to help"
#define FETCH_M_IMPL						\
{								\
	/* Nothing, and this is fine */				\
}
DEFINST(FETCH_M,		0xa0, /* -- Changed from 0x0a */
	"fetch_m",	        "0(b)",
	NA,			NA,
	DNA, DNA,		DNA, DNA, DNA)

//Assumes that PCC_OFF is unimplemented
#define RPCC_IMPL						\
{								\
	SET_GPR(RA, sim_cycle & LL(0xffffffff));		\
}
DEFINST(RPCC,			0xc0, /* -- Changed from 0x0c */
	"rpcc",          	"a",
	NA,			NA,
	DGPR(RA), DNA,		DNA, DNA, DNA)

/* added 02/27/99, plakal@cecil, from Alpha Arch Handbook (Rev.4, EV6) */
#define _RC_IMPL						        \
{									\
	/* FIXME: not supported (VAX) */				\
	DECLARE_FAULT(md_fault_unimpl);					\
}
DEFINST(_RC,			0xe0,  
	"rc",		        "a",
	NA,			NA,
	DNA, DNA,		DNA, DNA, DNA)

/* added 02/27/99, plakal@cecil, from Alpha Arch Handbook (Rev.4, EV6) */
//Evict cache block at RB, this is a performance hint. We'll ignore it for now.
#define ECB_IMPL							\
{									\
	/* FIXME: nada... */						\
}
DEFINST(ECB,			0xe8,
	"ecb",		        "(b)",
	NA,			NA,
	DNA, DNA,		DNA, DNA, DNA)

#define _RS_IMPL							\
{									\
	/* FIXME: not supported (VAX) */					\
	DECLARE_FAULT(md_fault_unimpl);					\
}
DEFINST(_RS,			0xf0,  /* -- Changed from 0x0f */
	"rs",		        "a",
	NA,			NA,
	DNA, DNA,		DNA, DNA, DNA)

/* added 02/27/99, plakal@cecil, from Alpha Arch Handbook (Rev.4, EV6) */
//This is a performance hint and is ignored
#define WH64_IMPL							\
{									\
	/* FIXME: nada... */						\
}
DEFINST(WH64,			0xf8,
	"wh64",		        "(b)",
	NA,			NA,
	DNA, DNA,		DNA, DNA, DNA)


CONNECT(JMPJSR)

#define JMP_IMPL							\
{									\
	SET_TPC(GPR(RB) & ~3);						\
	SET_NPC(GPR(RB) & ~3);						\
	SET_GPR(RA, CPC + 4);						\
}
DEFINST(JMP,			0x00,
	"jmp",			"a,(b)",
	IntALU,			F_CTRL|F_UNCOND|F_INDIRJMP,
	DGPR(RA), DNA,		DGPR(RB), DNA, DNA)

#define JSR_IMPL							\
{									\
	SET_TPC(GPR(RB) & ~3);						\
	SET_NPC(GPR(RB) & ~3);						\
	SET_GPR(RA, CPC + 4);						\
}
DEFINST(JSR,			0x01,
	"jsr",			"a,(b)",
	IntALU,			F_CTRL|F_UNCOND|F_INDIRJMP,
	DGPR(RA), DNA,		DGPR(RB), DNA, DNA)

#define RETN_IMPL							\
{									\
	SET_TPC(GPR(RB) & ~3);						\
	SET_NPC(GPR(RB) & ~3);						\
	SET_GPR(RA, CPC + 4);						\
}
DEFINST(RETN,			0x02,
	"ret",			"a,(b)",
	IntALU,			F_CTRL|F_UNCOND|F_INDIRJMP,
	DGPR(RA), DNA,		DGPR(RB), DNA, DNA)

#define JSR_COROUTINE_IMPL						\
{									\
	SET_TPC(GPR(RB) & ~3);						\
	SET_NPC(GPR(RB) & ~3);						\
	SET_GPR(RA, CPC + 4);						\
}
DEFINST(JSR_COROUTINE,		0x03,
	"jsr_coroutine",	"a,(b)",
	IntALU,			F_CTRL|F_UNCOND|F_INDIRJMP,
	DGPR(RA), DNA,		DGPR(RB), DNA, DNA)


/* changed from EXTS to FPTI to include more extensions (FIX,CIX,MVI) */
CONNECT(FPTI)

/* EV56 BWX extension... */
DEFLINK(SEXTB_LINK, 0x00, "sextb_link", 12, 1)

/* EV56 BWX extension... */
DEFLINK(SEXTW_LINK, 0x01, "sextw_link", 12, 1)

/* added 02/27/99, plakal@cecil, from Alpha Arch Handbook (Rev.4, EV6) */
 
/* CIX extensions */

/* FIXME: could write a faster version of 1-bit-counting:
   i.e., count = 0; while (n) { n = n & (n-1); count++; } */
#define CTPOP_IMPL							\
{									\
	int _temp, _i;							\
	qword_t _qwordhold = GPR(RB);				        \
									\
	_temp = 0;							\
	for(_i = 0; _i <= 63; _i++)					\
		if(_qwordhold & (ULL(1) << _i))				\
			_temp++;					\
									\
	SET_GPR(RC, (qword_t)_temp);					\
}
DEFINST(CTPOP,			0x30,  
	"ctpop",		"b,c",
	IntALU,			F_ICOMP,
	DGPR(RC), DNA,		DGPR(RB), DNA, DNA)

/* MVI extensions */
#define PERR_IMPL							\
{									\
	int _i;								\
	qword_t _sum_diffs, _qwordhold_a, _qwordhold_b;			\
									\
	_qwordhold_a = GPR(RA);						\
	_qwordhold_b = GPR(RB);						\
	_sum_diffs = 0;							\
									\
	for(_i = 0; _i <= 7; _i++)					\
	{								\
		byte_t _bytehold_a, _bytehold_b;			\
									\
		_bytehold_a = (_qwordhold_a >> (_i*8)) & 0xff;		\
		_bytehold_b = (_qwordhold_b >> (_i*8)) & 0xff;		\
		if(_bytehold_a >= _bytehold_b)				\
			_sum_diffs += (_bytehold_a - _bytehold_b);	\
		else							\
			_sum_diffs += (_bytehold_b - _bytehold_a);	\
	}								\
	SET_GPR(RC, _sum_diffs);					\
}
DEFINST(PERR,			0x31,
	"perr",			"a,b,c",
	IntALU,			F_ICOMP,
	DGPR(RC), DNA,		DGPR(RA), DGPR(RB), DNA)


/* CIX extensions */
#define CTLZ_IMPL							\
{									\
	int _temp, _i;							\
	qword_t _qwordhold = GPR(RB);					\
									\
	_temp = 0;							\
	for(_i = 63; _i >= 0; _i--)					\
	{								\
		if(_qwordhold & (ULL(1) << _i))				\
			break;						\
		_temp++;						\
	}								\
	SET_GPR(RC, (qword_t)_temp);					\
}
DEFINST(CTLZ,			0x32,  
	"ctlz",		        "b,c",
	IntALU,			F_ICOMP,
	DGPR(RC), DNA,		DGPR(RB), DNA, DNA)

#define CTTZ_IMPL							\
{									\
	int _temp, _i;							\
	qword_t _qwordhold = GPR(RB);					\
									\
	_temp = 0;							\
	for(_i = 0; _i <= 63; _i++)					\
	{								\
		if(_qwordhold & (ULL(1) << _i))				\
			break;						\
		_temp++;						\
	}								\
	SET_GPR(RC, (qword_t)_temp);					\
}
DEFINST(CTTZ,			0x33,  
	"cttz",		        "b,c",
	IntALU,			F_ICOMP,
	DGPR(RC), DNA,		DGPR(RB), DNA, DNA)


/* MVI extensions */
#define UNPKBW_IMPL							\
{									\
	qword_t _temp, _qwordhold;					\
									\
	_temp = 0;							\
	_qwordhold = GPR(RB);						\
									\
	_temp |= (_qwordhold & 0xff);					\
	_temp |= (((_qwordhold >> 8) & 0xff) << 16);			\
	_temp |= (((_qwordhold >> 16) & 0xff) << 32);			\
	_temp |= (((_qwordhold >> 24) & 0xff) << 48);			\
									\
	SET_GPR(RC, _temp);						\
}
DEFINST(UNPKBW,			0x34,
	"unpkbw",		"b,c",
	IntALU,			F_ICOMP,
	DGPR(RC), DNA,		DGPR(RB), DNA, DNA)

#define UNPKBL_IMPL							\
{									\
	qword_t _temp, _qwordhold;					\
									\
	_temp = 0;							\
	_qwordhold = GPR(RB);						\
									\
	_temp |= (_qwordhold & 0xff);					\
	_temp |= (((_qwordhold >> 8) & 0xff) << 32);			\
									\
	SET_GPR(RC, _temp);						\
}
DEFINST(UNPKBL,			0x35,
	"unpkbl",		"b,c",
	IntALU,			F_ICOMP,
	DGPR(RC), DNA,		DGPR(RB), DNA, DNA)

#define PKWB_IMPL							\
{									\
	qword_t _temp, _qwordhold;					\
									\
	_temp = 0;							\
	_qwordhold = GPR(RB);						\
									\
	_temp |= (_qwordhold & 0xff);					\
	_temp |= (((_qwordhold >> 16) & 0xff) << 8);			\
	_temp |= (((_qwordhold >> 32) & 0xff) << 16);			\
	_temp |= (((_qwordhold >> 48) & 0xff) << 24);			\
									\
	SET_GPR(RC, _temp);						\
}
DEFINST(PKWB,			0x36,
	"pkwb",			"b,c",
	IntALU,			F_ICOMP,
	DGPR(RC), DNA,		DGPR(RB), DNA, DNA)

#define PKLB_IMPL							\
{									\
	qword_t _temp, _qwordhold;					\
									\
	_temp = 0;							\
	_qwordhold = GPR(RB);						\
									\
	_temp |= (_qwordhold & 0xff);					\
	_temp |= (((_qwordhold >> 32) & 0xff) << 8);			\
									\
	SET_GPR(RC, _temp);						\
}
DEFINST(PKLB,			0x37,
	"pklb",			"b,c",
	IntALU,			F_ICOMP,
	DGPR(RC), DNA,		DGPR(RB), DNA, DNA)

/* following MVI entries were script-generated :) */

DEFLINK(MINSB8_LINK, 0x38, "minsb8_link", 12, 1)

DEFLINK(MINSW4_LINK, 0x39, "minsw4_link", 12, 1)

DEFLINK(MINUB8_LINK, 0x3a, "minub8_link", 12, 1)

DEFLINK(MINUW4_LINK, 0x3b, "minuw4_link", 12, 1)

DEFLINK(MAXUB8_LINK, 0x3c, "maxub8_link", 12, 1)

DEFLINK(MAXUW4_LINK, 0x3d, "maxuw4_link", 12, 1)

DEFLINK(MAXSB8_LINK, 0x3e, "maxsb8_link", 12, 1)

DEFLINK(MAXSW4_LINK, 0x3f, "maxsw4_link", 12, 1)

/* FIX extensions */
#define FTOIT_IMPL							\
{									\
	SET_GPR(RC, FPR_Q(RA));						\
}
DEFINST(FTOIT,			0x70,
	"ftoit",	        "A,c",
	FloatCVT,	        F_FCOMP,  /* FIXME: are these flags correct? */
	DGPR(RC), DNA,		DFPR(RA), DNA, DNA)

#define FTOIS_IMPL							\
{									\
	sqword_t _longhold;						\
	sword_t _inthold;						\
									\
	_longhold = FPR_Q(RA);						\
	_inthold = (((_longhold >> 32) & ULL(0xc0000000))		\
		| ((_longhold >> 29) & ULL(0x3fffffff)));		\
									\
	SET_GPR(RC, (SEXT32(_longhold >> 63) << 32) | _inthold);	\
}
DEFINST(FTOIS,			0x78,
	"ftois",	        "A,c",
	FloatCVT,	        F_FCOMP,  /* FIXME: are these flags correct? */
	DGPR(RC), DNA,		DFPR(RA), DNA, DNA)


/* MVI extensions (contd) */
CONNECT(MINSB8_LINK)

#define MINSB8_IMPL									\
{											\
	int _i;										\
	qword_t _qwordhold_a, _qwordhold_b, _qwordhold_c;				\
											\
	_qwordhold_a = GPR(RA);								\
	_qwordhold_b = GPR(RB);								\
	_qwordhold_c = 0;								\
											\
	for(_i = 0; _i <= 7; _i++)							\
	{										\
		sbyte_t _bytehold_a, _bytehold_b, _bytehold_c;				\
											\
		_bytehold_a = (_qwordhold_a >> (_i * 8)) & 0xff;			\
		_bytehold_b = (_qwordhold_b >> (_i * 8)) & 0xff;			\
		_bytehold_c = MIN(_bytehold_a, _bytehold_b);				\
											\
		_qwordhold_c |= (((qword_t)(byte_t)_bytehold_c & 0xff) << (_i*8));	\
	}										\
	SET_GPR(RC, _qwordhold_c);							\
}
DEFINST(MINSB8,			0x00,
	"minsb8",		"a,b,c",
	IntALU,			F_ICOMP,
	DGPR(RC), DNA,		DGPR(RA), DGPR(RB), DNA)

#define MINSB8I_IMPL									\
{											\
	int _i;										\
	qword_t _qwordhold_a, _qwordhold_b, _qwordhold_c;				\
											\
	_qwordhold_a = GPR(RA);								\
	_qwordhold_b = IMM;								\
	_qwordhold_c = 0;								\
											\
	for(_i = 0; _i <= 7; _i++)							\
	{										\
		sbyte_t _bytehold_a, _bytehold_b, _bytehold_c;				\
											\
		_bytehold_a = (_qwordhold_a >> (_i * 8)) & 0xff;			\
		_bytehold_b = (_qwordhold_b >> (_i * 8)) & 0xff;			\
		_bytehold_c = MIN(_bytehold_a, _bytehold_b);				\
											\
		_qwordhold_c |= (((qword_t)(byte_t)_bytehold_c & 0xff) << (_i*8));	\
	}										\
	SET_GPR(RC, _qwordhold_c);							\
}
DEFINST(MINSB8I,		0x01,
	"minsb8",		"a,i,c",
	IntALU,			F_ICOMP|F_IMM,
	DGPR(RC), DNA,		DGPR(RA), DNA, DNA)


CONNECT(MINSW4_LINK)

#define MINSW4_IMPL									\
{											\
	int _i;										\
	qword_t _qwordhold_a, _qwordhold_b, _qwordhold_c;				\
											\
	_qwordhold_a = GPR(RA);								\
	_qwordhold_b = GPR(RB);								\
	_qwordhold_c = 0;								\
											\
	for(_i = 0; _i <= 3; _i++)							\
	{										\
		shalf_t _halfhold_a, _halfhold_b, _halfhold_c;				\
											\
		_halfhold_a = (_qwordhold_a >> (_i * 16)) & 0xffff;			\
		_halfhold_b = (_qwordhold_b >> (_i * 16)) & 0xffff;			\
		_halfhold_c = MIN(_halfhold_a, _halfhold_b);				\
											\
		_qwordhold_c |= (((qword_t)(half_t)_halfhold_c & 0xffff) << (_i*16));	\
	}										\
	SET_GPR(RC, _qwordhold_c);							\
}
DEFINST(MINSW4,			0x00,
	"minsw4",		"a,b,c",
	IntALU,			F_ICOMP,
	DGPR(RC), DNA,		DGPR(RA), DGPR(RB), DNA)

#define MINSW4I_IMPL									\
{											\
	int _i;										\
	qword_t _qwordhold_a, _qwordhold_b, _qwordhold_c;				\
											\
	_qwordhold_a = GPR(RA);								\
	_qwordhold_b = IMM;								\
	_qwordhold_c = 0;								\
											\
	for(_i = 0; _i <= 3; _i++)							\
	{										\
		shalf_t _halfhold_a, _halfhold_b, _halfhold_c;				\
											\
		_halfhold_a = (_qwordhold_a >> (_i * 16)) & 0xffff;			\
		_halfhold_b = (_qwordhold_b >> (_i * 16)) & 0xffff;			\
		_halfhold_c = MIN(_halfhold_a, _halfhold_b);				\
											\
		_qwordhold_c |= (((qword_t)(half_t)_halfhold_c & 0xffff) << (_i*16));	\
	}										\
	SET_GPR(RC, _qwordhold_c);							\
}
DEFINST(MINSW4I,		0x01,
	"minsw4",		"a,i,c",
	IntALU,			F_ICOMP|F_IMM,
	DGPR(RC), DNA,		DGPR(RA), DNA, DNA)


CONNECT(MINUB8_LINK)

#define MINUB8_IMPL									\
{											\
	int _i;										\
	qword_t _qwordhold_a, _qwordhold_b, _qwordhold_c;				\
											\
	_qwordhold_a = GPR(RA);								\
	_qwordhold_b = GPR(RB);								\
	_qwordhold_c = 0;								\
											\
	for(_i = 0; _i <= 7; _i++)							\
	{										\
		byte_t _bytehold_a, _bytehold_b, _bytehold_c;				\
											\
		_bytehold_a = (_qwordhold_a >> (_i * 8)) & 0xff;			\
		_bytehold_b = (_qwordhold_b >> (_i * 8)) & 0xff;			\
		_bytehold_c = MIN(_bytehold_a, _bytehold_b);				\
											\
		_qwordhold_c |= (((qword_t)(byte_t)_bytehold_c & 0xff) << (_i*8));	\
	}										\
	SET_GPR(RC, _qwordhold_c);							\
}
DEFINST(MINUB8,			0x00,
	"minub8",		"a,b,c",
	IntALU,			F_ICOMP,
	DGPR(RC), DNA,		DGPR(RA), DGPR(RB), DNA)

#define MINUB8I_IMPL									\
{											\
	int _i;										\
	qword_t _qwordhold_a, _qwordhold_b, _qwordhold_c;				\
											\
	_qwordhold_a = GPR(RA);								\
	_qwordhold_b = IMM;								\
	_qwordhold_c = 0;								\
											\
	for(_i = 0; _i <= 7; _i++)							\
	{										\
		byte_t _bytehold_a, _bytehold_b, _bytehold_c;				\
											\
		_bytehold_a = (_qwordhold_a >> (_i * 8)) & 0xff;			\
		_bytehold_b = (_qwordhold_b >> (_i * 8)) & 0xff;			\
		_bytehold_c = MIN(_bytehold_a, _bytehold_b);				\
											\
		_qwordhold_c |= (((qword_t)(byte_t)_bytehold_c & 0xff) << (_i*8));	\
	}										\
	SET_GPR(RC, _qwordhold_c);							\
}
DEFINST(MINUB8I,		0x01,
	"minub8",		"a,i,c",
	IntALU,			F_ICOMP|F_IMM,
	DGPR(RC), DNA,		DGPR(RA), DNA, DNA)


CONNECT(MINUW4_LINK)

#define MINUW4_IMPL									\
{											\
	int _i;										\
	qword_t _qwordhold_a, _qwordhold_b, _qwordhold_c;				\
											\
	_qwordhold_a = GPR(RA);								\
	_qwordhold_b = GPR(RB);								\
	_qwordhold_c = 0;								\
											\
	for(_i = 0; _i <= 3; _i++)							\
	{										\
		half_t _halfhold_a, _halfhold_b, _halfhold_c;				\
											\
		_halfhold_a = (_qwordhold_a >> (_i * 16)) & 0xffff;			\
		_halfhold_b = (_qwordhold_b >> (_i * 16)) & 0xffff;			\
		_halfhold_c = MIN(_halfhold_a, _halfhold_b);				\
											\
		_qwordhold_c |= (((qword_t)(half_t)_halfhold_c & 0xffff) << (_i*16));	\
	}										\
	SET_GPR(RC, _qwordhold_c);							\
}
DEFINST(MINUW4,			0x00,
	"minuw4",		"a,b,c",
	IntALU,			F_ICOMP,
	DGPR(RC), DNA,		DGPR(RA), DGPR(RB), DNA)

#define MINUW4I_IMPL									\
{											\
	int _i;										\
	qword_t _qwordhold_a, _qwordhold_b, _qwordhold_c;				\
											\
	_qwordhold_a = GPR(RA);								\
	_qwordhold_b = IMM;								\
	_qwordhold_c = 0;								\
											\
	for(_i = 0; _i <= 3; _i++)							\
	{										\
		half_t _halfhold_a, _halfhold_b, _halfhold_c;				\
											\
		_halfhold_a = (_qwordhold_a >> (_i * 16)) & 0xffff;			\
		_halfhold_b = (_qwordhold_b >> (_i * 16)) & 0xffff;			\
		_halfhold_c = MIN(_halfhold_a, _halfhold_b);				\
											\
		_qwordhold_c |= (((qword_t)(half_t)_halfhold_c & 0xffff) << (_i*16));	\
	}										\
	SET_GPR(RC, _qwordhold_c);							\
}
DEFINST(MINUW4I,		0x01,
	"minuw4",		"a,i,c",
	IntALU,			F_ICOMP|F_IMM,
	DGPR(RC), DNA,		DGPR(RA), DNA, DNA)


CONNECT(MAXUB8_LINK)

#define MAXUB8_IMPL									\
{											\
	int _i;										\
	qword_t _qwordhold_a, _qwordhold_b, _qwordhold_c;				\
											\
	_qwordhold_a = GPR(RA);								\
	_qwordhold_b = GPR(RB);								\
	_qwordhold_c = 0;								\
											\
	for(_i = 0; _i <= 7; _i++)							\
	{										\
		byte_t _bytehold_a, _bytehold_b, _bytehold_c;				\
											\
		_bytehold_a = (_qwordhold_a >> (_i * 8)) & 0xff;			\
		_bytehold_b = (_qwordhold_b >> (_i * 8)) & 0xff;			\
		_bytehold_c = MAX(_bytehold_a, _bytehold_b);				\
											\
		_qwordhold_c |= (((qword_t)(byte_t)_bytehold_c & 0xff) << (_i*8));	\
	}										\
	SET_GPR(RC, _qwordhold_c);							\
}
DEFINST(MAXUB8,			0x00,
	"maxub8",		"a,b,c",
	IntALU,			F_ICOMP,
	DGPR(RC), DNA,		DGPR(RA), DGPR(RB), DNA)

#define MAXUB8I_IMPL									\
{											\
	int _i;										\
	qword_t _qwordhold_a, _qwordhold_b, _qwordhold_c;				\
											\
	_qwordhold_a = GPR(RA);								\
	_qwordhold_b = IMM;								\
	_qwordhold_c = 0;								\
											\
	for(_i = 0; _i <= 7; _i++)							\
	{										\
		byte_t _bytehold_a, _bytehold_b, _bytehold_c;				\
											\
		_bytehold_a = (_qwordhold_a >> (_i * 8)) & 0xff;			\
		_bytehold_b = (_qwordhold_b >> (_i * 8)) & 0xff;			\
		_bytehold_c = MAX(_bytehold_a, _bytehold_b);				\
											\
		_qwordhold_c |= (((qword_t)(byte_t)_bytehold_c & 0xff) << (_i*8));	\
	}										\
	SET_GPR(RC, _qwordhold_c);							\
}
DEFINST(MAXUB8I,		0x01,
	"maxub8",		"a,i,c",
	IntALU,			F_ICOMP|F_IMM,
	DGPR(RC), DNA,		DGPR(RA), DNA, DNA)


CONNECT(MAXUW4_LINK)

#define MAXUW4_IMPL									\
{											\
	int _i;										\
	qword_t _qwordhold_a, _qwordhold_b, _qwordhold_c;				\
											\
	_qwordhold_a = GPR(RA);								\
	_qwordhold_b = GPR(RB);								\
	_qwordhold_c = 0;								\
											\
	for(_i = 0; _i <= 3; _i++)							\
	{										\
		half_t _halfhold_a, _halfhold_b, _halfhold_c;				\
											\
		_halfhold_a = (_qwordhold_a >> (_i * 16)) & 0xffff;			\
		_halfhold_b = (_qwordhold_b >> (_i * 16)) & 0xffff;			\
		_halfhold_c = MAX(_halfhold_a, _halfhold_b);				\
											\
		_qwordhold_c |= (((qword_t)(half_t)_halfhold_c & 0xffff) << (_i*16));	\
	}										\
	SET_GPR(RC, _qwordhold_c);							\
}
DEFINST(MAXUW4,			0x00,
	"maxuw4",		"a,b,c",
	IntALU,			F_ICOMP,
	DGPR(RC), DNA,		DGPR(RA), DGPR(RB), DNA)

#define MAXUW4I_IMPL									\
{											\
	int _i;										\
	qword_t _qwordhold_a, _qwordhold_b, _qwordhold_c;				\
											\
	_qwordhold_a = GPR(RA);								\
	_qwordhold_b = IMM;								\
	_qwordhold_c = 0;								\
											\
	for(_i = 0; _i <= 3; _i++)							\
	{										\
		half_t _halfhold_a, _halfhold_b, _halfhold_c;				\
											\
		_halfhold_a = (_qwordhold_a >> (_i * 16)) & 0xffff;			\
		_halfhold_b = (_qwordhold_b >> (_i * 16)) & 0xffff;			\
		_halfhold_c = MAX(_halfhold_a, _halfhold_b);				\
											\
		_qwordhold_c |= (((qword_t)(half_t)_halfhold_c & 0xffff) << (_i*16));	\
	}										\
	SET_GPR(RC, _qwordhold_c);							\
}
DEFINST(MAXUW4I,		0x01,
	"maxuw4",		"a,i,c",
	IntALU,			F_ICOMP|F_IMM,
	DGPR(RC), DNA,		DGPR(RA), DNA, DNA)


CONNECT(MAXSB8_LINK)

#define MAXSB8_IMPL									\
{											\
	int _i;										\
	qword_t _qwordhold_a, _qwordhold_b, _qwordhold_c;				\
											\
	_qwordhold_a = GPR(RA);								\
	_qwordhold_b = GPR(RB);								\
	_qwordhold_c = 0;								\
											\
	for(_i = 0; _i <= 7; _i++)							\
	{										\
		sbyte_t _bytehold_a, _bytehold_b, _bytehold_c;				\
											\
		_bytehold_a = (_qwordhold_a >> (_i * 8)) & 0xff;			\
		_bytehold_b = (_qwordhold_b >> (_i * 8)) & 0xff;			\
		_bytehold_c = MAX(_bytehold_a, _bytehold_b);				\
											\
		_qwordhold_c |= (((qword_t)(byte_t)_bytehold_c & 0xff) << (_i*8));	\
	}										\
	SET_GPR(RC, _qwordhold_c);							\
}
DEFINST(MAXSB8,			0x00,
	"maxsb8",		"a,b,c",
	IntALU,			F_ICOMP,
	DGPR(RC), DNA,		DGPR(RA), DGPR(RB), DNA)

#define MAXSB8I_IMPL									\
{											\
	int _i;										\
	qword_t _qwordhold_a, _qwordhold_b, _qwordhold_c;				\
											\
	_qwordhold_a = GPR(RA);								\
	_qwordhold_b = IMM;								\
	_qwordhold_c = 0;								\
											\
	for(_i = 0; _i <= 7; _i++)							\
	{										\
		sbyte_t _bytehold_a, _bytehold_b, _bytehold_c;				\
											\
		_bytehold_a = (_qwordhold_a >> (_i * 8)) & 0xff;			\
		_bytehold_b = (_qwordhold_b >> (_i * 8)) & 0xff;			\
		_bytehold_c = MAX(_bytehold_a, _bytehold_b);				\
											\
		_qwordhold_c |= (((qword_t)(byte_t)_bytehold_c & 0xff) << (_i*8));	\
	}										\
	SET_GPR(RC, _qwordhold_c);							\
}
DEFINST(MAXSB8I,		0x01,
	"maxsb8",		"a,i,c",
	IntALU,			F_ICOMP|F_IMM,
	DGPR(RC), DNA,		DGPR(RA), DNA, DNA)


CONNECT(MAXSW4_LINK)

#define MAXSW4_IMPL									\
{											\
	int _i;										\
	qword_t _qwordhold_a, _qwordhold_b, _qwordhold_c;				\
											\
	_qwordhold_a = GPR(RA);								\
	_qwordhold_b = GPR(RB);								\
	_qwordhold_c = 0;								\
											\
	for(_i = 0; _i <= 3; _i++)							\
	{										\
		shalf_t _halfhold_a, _halfhold_b, _halfhold_c;				\
											\
		_halfhold_a = (_qwordhold_a >> (_i * 16)) & 0xffff;			\
		_halfhold_b = (_qwordhold_b >> (_i * 16)) & 0xffff;			\
		_halfhold_c = MAX(_halfhold_a, _halfhold_b);				\
											\
		_qwordhold_c |= (((qword_t)(half_t)_halfhold_c & 0xffff) << (_i*16));	\
	}										\
	SET_GPR(RC, _qwordhold_c);							\
}
DEFINST(MAXSW4,			0x00,
	"maxsw4",		"a,b,c",
	IntALU,			F_ICOMP,
	DGPR(RC), DNA,		DGPR(RA), DGPR(RB), DNA)

#define MAXSW4I_IMPL									\
{											\
	int _i;										\
	qword_t _qwordhold_a, _qwordhold_b, _qwordhold_c;				\
											\
	_qwordhold_a = GPR(RA);								\
	_qwordhold_b = IMM;								\
	_qwordhold_c = 0;								\
											\
	for(_i = 0; _i <= 3; _i++)							\
	{										\
		shalf_t _halfhold_a, _halfhold_b, _halfhold_c;				\
											\
		_halfhold_a = (_qwordhold_a >> (_i * 16)) & 0xffff;			\
		_halfhold_b = (_qwordhold_b >> (_i * 16)) & 0xffff;			\
		_halfhold_c = MAX(_halfhold_a, _halfhold_b);				\
											\
		_qwordhold_c |= (((qword_t)(half_t)_halfhold_c & 0xffff) << (_i*16));	\
	}										\
	SET_GPR(RC, _qwordhold_c);							\
}
DEFINST(MAXSW4I,		0x01,
	"maxsw4",		"a,i,c",
	IntALU,			F_ICOMP|F_IMM,
	DGPR(RC), DNA,		DGPR(RA), DNA, DNA)


/* EV56 BWX extension... */
CONNECT(SEXTB_LINK)

/* EV56 BWX extension... */
#define SEXTB_IMPL							\
{									\
	SET_GPR(RC, (qword_t)(sqword_t)(sbyte_t)(GPR(RB) & 0xff));	\
}
DEFINST(SEXTB,			0x00,
	"sextb",		"b,c",
	IntALU,			F_ICOMP,
	DGPR(RC), DNA,		DGPR(RB), DNA, DNA)

/* EV56 BWX extension... */
#define SEXTBI_IMPL							\
{									\
	SET_GPR(RC, (qword_t)(sqword_t)(sbyte_t)(IMM & 0xff));		\
}
DEFINST(SEXTBI,			0x01,
	"sextb",		"i,c",
	IntALU,			F_ICOMP|F_IMM,
	DGPR(RC), DNA,		DNA, DNA, DNA)


/* EV56 BWX extension... */
CONNECT(SEXTW_LINK)

/* EV56 BWX extension... */
#define SEXTW_IMPL							\
{									\
	SET_GPR(RC, (qword_t)(sqword_t)(shalf_t)(GPR(RB) & 0xffff));	\
}
DEFINST(SEXTW,			0x00,
	"sextw",		"b,c",
	IntALU,			F_ICOMP,
	DGPR(RC), DNA,		DGPR(RB), DNA, DNA)

/* EV56 BWX extension... */
#define SEXTWI_IMPL							\
{									\
	SET_GPR(RC, (qword_t)(sqword_t)(shalf_t)(IMM & 0xffff));	\
}
DEFINST(SEXTWI,			0x01,
	"sextw",		"i,c",
	IntALU,			F_ICOMP|F_IMM,
	DGPR(RC), DNA,		DNA, DNA, DNA)


//clean up all definitions...
#undef LDA_IMPL
#undef LDAH_IMPL
#undef LDBU_IMPL
#undef LDQ_U_IMPL
#undef LDWU_IMPL
#undef STW_IMPL
#undef STB_IMPL
#undef STQ_U_IMPL
#undef FLTV_IMPL
#undef LDF_IMPL
#undef LDG_IMPL
#undef LDS_IMPL
#undef LDT_IMPL
#undef STF_IMPL
#undef STG_IMPL
#undef STS_IMPL
#undef STT_IMPL
#undef LDL_IMPL
#undef LDQ_IMPL
#undef LDL_L_IMPL
#undef LDQ_L_IMPL
#undef STL_IMPL
#undef STQ_IMPL
#undef STL_C_IMPL
#undef STQ_C_IMPL
#undef BR_IMPL
#undef FBEQ_IMPL
#undef FBLT_IMPL
#undef FBLE_IMPL
#undef BSR_IMPL
#undef FBNE_IMPL
#undef FBGE_IMPL
#undef FBGT_IMPL
#undef BLBC_IMPL
#undef BEQ_IMPL
#undef BLT_IMPL
#undef BLE_IMPL
#undef BLBS_IMPL
#undef BNE_IMPL
#undef BGE_IMPL
#undef BGT_IMPL
#undef PAL_CALLSYS_IMPL
#undef PAL_RDUNIQ_IMPL
#undef PAL_WRUNIQ_IMPL
#undef PAL_WRKGP_IMPL
#undef PAL_WRENT_IMPL
#undef PAL_IMB_IMPL
#undef ADDL_IMPL
#undef ADDLI_IMPL
#undef S4ADDL_IMPL
#undef S4ADDLI_IMPL
#undef SUBL_IMPL
#undef SUBLI_IMPL
#undef S4SUBL_IMPL
#undef S4SUBLI_IMPL
#undef CMPBGE_IMPL
#undef CMPBGEI_IMPL
#undef S8ADDL_IMPL
#undef S8ADDLI_IMPL
#undef S8SUBL_IMPL
#undef S8SUBLI_IMPL
#undef CMPULT_IMPL
#undef CMPULTI_IMPL
#undef ADDQ_IMPL
#undef ADDQI_IMPL
#undef S4ADDQ_IMPL
#undef S4ADDQI_IMPL
#undef SUBQ_IMPL
#undef SUBQI_IMPL
#undef S4SUBQ_IMPL
#undef S4SUBQI_IMPL
#undef CMPEQ_IMPL
#undef CMPEQI_IMPL
#undef S8ADDQ_IMPL
#undef S8ADDQI_IMPL
#undef S8SUBQ_IMPL
#undef S8SUBQI_IMPL
#undef CMPULE_IMPL
#undef CMPULEI_IMPL
#undef ADDLV_IMPL
#undef ADDLVI_IMPL
#undef SUBLV_IMPL
#undef SUBLVI_IMPL
#undef CMPLT_IMPL
#undef CMPLTI_IMPL
#undef ADDQV_IMPL
#undef ADDQVI_IMPL
#undef SUBQV_IMPL
#undef SUBQVI_IMPL
#undef CMPLE_IMPL
#undef CMPLEI_IMPL
#undef AND_IMPL
#undef ANDI_IMPL
#undef BIC_IMPL
#undef BICI_IMPL
#undef CMOVLBS_IMPL
#undef CMOVLBSI_IMPL
#undef CMOVLBC_IMPL
#undef CMOVLBCI_IMPL
#undef BIS_IMPL
#undef BISI_IMPL
#undef CMOVEQ_IMPL
#undef CMOVEQI_IMPL
#undef CMOVNE_IMPL
#undef CMOVNEI_IMPL
#undef ORNOT_IMPL
#undef ORNOTI_IMPL
#undef XOR_IMPL
#undef XORI_IMPL
#undef CMOVLT_IMPL
#undef CMOVLTI_IMPL
#undef CMOVGE_IMPL
#undef CMOVGEI_IMPL
#undef EQV_IMPL
#undef EQVI_IMPL
#undef AMASK_IMPL
#undef AMASKI_IMPL
#undef CMOVLE_IMPL
#undef CMOVLEI_IMPL
#undef CMOVGT_IMPL
#undef CMOVGTI_IMPL
#undef IMPLVER_IMPL
#undef MSKBL_IMPL
#undef MSKBLI_IMPL
#undef EXTBL_IMPL
#undef EXTBLI_IMPL
#undef INSBL_IMPL
#undef INSBLI_IMPL
#undef MSKWL_IMPL
#undef MSKWLI_IMPL
#undef EXTWL_IMPL
#undef EXTWLI_IMPL
#undef INSWL_IMPL
#undef INSWLI_IMPL
#undef MSKLL_IMPL
#undef MSKLLI_IMPL
#undef EXTLL_IMPL
#undef EXTLLI_IMPL
#undef INSLL_IMPL
#undef INSLLI_IMPL
#undef ZAP_IMPL
#undef ZAPI_IMPL
#undef ZAPNOT_IMPL
#undef ZAPNOTI_IMPL
#undef MSKQL_IMPL
#undef MSKQLI_IMPL
#undef SRL_IMPL
#undef SRLI_IMPL
#undef EXTQL_IMPL
#undef EXTQLI_IMPL
#undef SLL_IMPL
#undef SLLI_IMPL
#undef INSQL_IMPL
#undef INSQLI_IMPL
#undef SRA_IMPL
#undef SRAI_IMPL
#undef MSKWH_IMPL
#undef MSKWHI_IMPL
#undef INSWH_IMPL
#undef INSWHI_IMPL
#undef EXTWH_IMPL
#undef EXTWHI_IMPL
#undef MSKLH_IMPL
#undef MSKLHI_IMPL
#undef INSLH_IMPL
#undef INSLHI_IMPL
#undef EXTLH_IMPL
#undef EXTLHI_IMPL
#undef MSKQH_IMPL
#undef MSKQHI_IMPL
#undef INSQH_IMPL
#undef INSQHI_IMPL
#undef EXTQH_IMPL
#undef EXTQHI_IMPL
#undef MULL_IMPL
#undef MULLI_IMPL
#undef MULQ_IMPL
#undef MULQI_IMPL
#undef UMULH_IMPL
#undef UMULHI_IMPL
#undef ITOFS_IMPL
#undef SQRTF_IMPL
#undef SQRTS_IMPL
#undef ITOFF_IMPL
#undef ITOFT_IMPL
#undef SQRTG_IMPL
#undef SQRTT_IMPL
#undef ADDS_IMPL
#undef SUBS_IMPL
#undef MULS_IMPL
#undef DIVS_IMPL
#undef ADDT_IMPL
#undef SUBT_IMPL
#undef MULT_IMPL
#undef DIVT_IMPL
#undef CMPTUN_IMPL
#undef CMPTEQ_IMPL
#undef CMPTLT_IMPL
#undef CMPTLE_IMPL
#undef CVTTS_IMPL
#undef CVTTQ_IMPL
#undef CVTQS_IMPL
#undef CVTQT_IMPL
#undef CVTLQ_IMPL
#undef CPYS_IMPL
#undef CPYSN_IMPL
#undef CPYSE_IMPL
#undef MT_FPCR_IMPL
#undef MF_FPCR_IMPL
#undef FCMOVEQ_IMPL
#undef FCMOVNE_IMPL
#undef FCMOVLT_IMPL
#undef FCMOVGE_IMPL
#undef FCMOVLE_IMPL
#undef FCMOVGT_IMPL
#undef CVTQL_IMPL
#undef TRAPB_IMPL
#undef EXCB_IMPL
#undef MB_IMPL
#undef WMB_IMPL
#undef FETCH_IMPL
#undef FETCH_M_IMPL
#undef RPCC_IMPL
#undef _RC_IMPL
#undef ECB_IMPL
#undef _RS_IMPL
#undef WH64_IMPL
#undef JMP_IMPL
#undef JSR_IMPL
#undef RETN_IMPL
#undef JSR_COROUTINE_IMPL
#undef SEXTB_IMPL
#undef SEXTBI_IMPL
#undef SEXTW_IMPL
#undef SEXTWI_IMPL
#undef CTPOP_IMPL
#undef PERR_IMPL
#undef CTLZ_IMPL
#undef CTTZ_IMPL
#undef UNPKBW_IMPL
#undef UNPKBL_IMPL
#undef PKWB_IMPL
#undef PKLB_IMPL
#undef MINSB8_IMPL
#undef MINSW4_IMPL
#undef MINUB8_IMPL
#undef MINUW4_IMPL
#undef MAXUB8_IMPL
#undef MAXUW4_IMPL
#undef MAXSB8_IMPL
#undef MAXSW4_IMPL
#undef FTOIT_IMPL
#undef FTOIS_IMPL

#undef DEFINST
#undef DEFLINK
#undef CONNECT
