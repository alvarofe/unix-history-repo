#ifndef lint
static char *sccsid ="@(#)table.c	1.30 (Berkeley) %G%";
#endif lint

# include "pass2.h"

# define WPTR TPTRTO|TINT|TLONG|TFLOAT|TDOUBLE|TPOINT|TUNSIGNED|TULONG
# define AWD SNAME|SOREG|SCON|STARNM|STARREG
/* tbl */
# define ANYSIGNED TPOINT|TINT|TLONG|TSHORT|TCHAR
# define ANYUSIGNED TUNSIGNED|TULONG|TUSHORT|TUCHAR
# define ANYFIXED ANYSIGNED|ANYUSIGNED
# define TWORD TINT|TUNSIGNED|TPOINT|TLONG|TULONG
# define NIAWD SNAME|SCON|STARNM
/* tbl */

struct optab  table[] = {

	/* the following entry is to fix a problem with
	   the manner that the first pass handles the
	   type of a shift expression                 */
PCONV,	INAREG|INTAREG,
	SAREG|AWD,	TINT|TUNSIGNED,
	SANY,	TPOINT,
		NAREG|NASL,	RLEFT,
		"",

#if defined(FORT) || defined(SPRECC)
SCONV,	INTAREG|FORCC,
	SAREG|AWD,	TDOUBLE,
	SANY,	TFLOAT,
		NAREG|NASL,	RESC1|RESCC,
		"	cvtdf	AL,A1\n",

SCONV,	INTAREG|FORCC,
	SAREG|AWD,	ANYSIGNED,
	SANY,	TFLOAT,
		NAREG|NASL,	RESC1|RESCC,
		"	cvtZLf	AL,TA1\n",
#endif

/* take care of redundant conversions introduced by reclaim() */
SCONV,	INTAREG,
	STAREG,	TWORD,
	SANY,	TWORD,
		0,	RLEFT,
		"",

SCONV,	INTAREG,
	STAREG,	TDOUBLE,
	SANY,	TDOUBLE,
		0,	RLEFT,
		"",

SCONV,	INTAREG|FORCC,
	SAREG|AWD,	TANY,
	SANY,	TANY,
		NAREG|NASL,	RESC1|RESCC,
		"	ZA\n",

SCONV,	FORARG,
	SAREG|AWD,	TANY,
	SANY,	TANY,
		NAREG|NASL,	RNULL,
		"	ZV\n",

INIT,	FOREFF,
	SCON,	TANY,
	SANY,	TWORD,
		0,	RNOP,
		"	.long	CL\n",

INIT,	FOREFF,
	SCON,	TANY,
	SANY,	TSHORT|TUSHORT,
		0,	RNOP,
		"	.word	CL\n",

INIT,	FOREFF,
	SCON,	TANY,
	SANY,	TCHAR|TUCHAR,
		0,	RNOP,
		"	.byte	CL\n",

#ifdef FORT
	/* for the use of fortran only */

GOTO,	FOREFF,
	SCON,	TANY,
	SANY,	TANY,
		0,	RNOP,
		"	jbr	CL\n",
#endif

GOTO,	FOREFF,
	SNAME|SOREG,	TANY,
	SANY,	TANY,
		0,	RNOP,
		"	jmp	*AL\n",

GOTO,	FOREFF,
	SAREG,	TANY,
	SANY,	TANY,
		0,	RNOP,
		"	jmp	(AL)\n",

STARG,	FORARG,
	SCON|SOREG,	TANY,
	SANY,	TANY,
		0,	RNULL,
		"	subl2	ZT,sp\nZS",

STASG,	FOREFF,
	SNAME|SOREG,	TANY,
	SCON|SAREG,	TANY,
		0,	RNOP,
		"ZS",

STASG,	INAREG,
	SNAME|SOREG,	TANY,
	SCON,	TANY,
		NAREG,	RESC1,
		"ZS	movl	AR,A1\n",

STASG,	INAREG,
	SNAME|SOREG,	TANY,
	SAREG,	TANY,
		0,	RRIGHT,
		"	pushl	AR\nZS	movl	(sp)+,AR\n",

FLD,	INAREG|INTAREG,
	SANY,	TANY,
	SFLD,	ANYSIGNED,
		NAREG|NASR,	RESC1,
		"	extv	$H,$S,AR,A1\n",

FLD,	INAREG|INTAREG,
	SANY,	TANY,
	SFLD,	ANYUSIGNED,
		NAREG|NASR,	RESC1,
		"	extzv	$H,$S,AR,A1\n",

FLD,	FORARG,
	SANY,	TANY,
	SFLD,	ANYSIGNED,
		0,	RNULL,
		"	extv	$H,$S,AR,-(sp)\n",

FLD,	FORARG,
	SANY,	TANY,
	SFLD,	ANYUSIGNED,
		0,	RNULL,
		"	extzv	$H,$S,AR,-(sp)\n",

OPLOG,	FORCC,
	SAREG|AWD,	TWORD,
	SAREG|AWD,	TWORD,
		0,	RESCC,
		"	cmpl	AL,AR\nZP",

OPLOG,	FORCC,
	SAREG|AWD,	TSHORT,
	SAREG|AWD,	TSHORT,
		0,	RESCC,
		"	cmpw	AL,AR\nZP",

OPLOG,	FORCC,
	SAREG|AWD,	TUSHORT,
	SAREG|AWD,	TUSHORT,
		0,	RESCC,
		"	cmpw	AL,AR\nZP",

OPLOG,	FORCC,
	SAREG|AWD,	TCHAR,
	SAREG|AWD,	TCHAR,
		0,	RESCC,
		"	cmpb	AL,AR\nZP",

OPLOG,	FORCC,
	SAREG|AWD,	TUCHAR,
	SAREG|AWD,	TUCHAR,
		0,	RESCC,
		"	cmpb	AL,AR\nZP",

/* optim2() handles degenerate comparisons with constants */
OPLOG,	FORCC,
	SAREG|AWD,	TCHAR|TUCHAR|TSHORT|TUSHORT,
	SCON,	ANYFIXED,
		0,	RESCC,
		"	cmpZL	AL,AR\nZP",

OPLOG,	FORCC,
	SAREG|AWD,	TDOUBLE,
	SAREG|AWD,	TDOUBLE,
		0,	RESCC,
		"	cmpd	AL,AR\nZP",

OPLOG,	FORCC,
	SAREG|AWD,	TFLOAT,
	SAREG|AWD,	TFLOAT,
		0,	RESCC,
		"	cmpf	AL,AR\nZP",

CCODES,	INAREG|INTAREG,
	SANY,	TANY,
	SANY,	TANY,
		NAREG,	RESC1,
		"	movl	$1,A1\nZN",

UNARY CALL,	INTAREG,
	SCON,	TANY,
	SANY,	TWORD|TCHAR|TUCHAR|TSHORT|TUSHORT|TFLOAT|TDOUBLE,
		NAREG|NASL,	RESC1,
		"	calls	ZC,CL\n",

UNARY CALL,	INTAREG,
	SAREG,	TANY,
	SANY,	TWORD|TCHAR|TUCHAR|TSHORT|TUSHORT|TFLOAT|TDOUBLE,
		NAREG|NASL,	RESC1,	/* should be 0 */
		"	calls	ZC,(AL)\n",

UNARY CALL,	INAREG|INTAREG,
	SNAME,	TANY,
	SANY,	TANY,
		NAREG|NASL,	RESC1,	/* really reg 0 */
		"	calls	ZC,*AL\n",

UNARY CALL,	INAREG|INTAREG,
	SSOREG,	TANY,
	SANY,	TANY,
		NAREG|NASL,	RESC1,	/* really reg 0 */
		"	calls	ZC,*AL\n",

ASG RS,	INAREG|FOREFF|FORCC,
	SAREG,	TWORD,
	SCON,	TINT|TUNSIGNED,
		0,	RLEFT|RESCC,
		"	extzv	AR,ZU,AL,AL\n",

ASG RS,	INAREG|FOREFF|FORCC,
	SAREG,	TWORD,
	SAREG,	ANYFIXED,
		NAREG,	RLEFT|RESCC,
		"	subl3	AR,$32,A1\n	extzv	AR,A1,AL,AL\n",

ASG RS,	INAREG|FOREFF|FORCC,
	SAREG,	TWORD,
	SAREG|AWD,	TWORD,
		NAREG,	RLEFT|RESCC,
		"	subl3	AR,$32,A1\n	extzv	AR,A1,AL,AL\n",

RS,	INAREG|INTAREG|FORCC,
	SAREG,	TWORD,
	SCON,	TINT|TUNSIGNED,
		NAREG|NASL,	RESC1|RESCC,
		"	extzv	AR,ZU,AL,A1\n",

ASG LS,	INAREG|FOREFF|FORCC,
	SAREG|AWD,	TWORD,
	SAREG|NIAWD,	ANYSIGNED|ANYUSIGNED,
		0,	RLEFT|RESCC,
		"	ashl	AR,AL,AL\n",

ASG LS,	INAREG|FOREFF|FORCC,
	SAREG|AWD,	TWORD,
	SSOREG,	ANYSIGNED|ANYUSIGNED,
		0,	RLEFT|RESCC,
		"	ashl	AR,AL,AL\n",

ASG LS,	INAREG|FOREFF|FORCC,
	SAREG|AWD,	TWORD,
	SOREG,	ANYSIGNED|ANYUSIGNED,
		NAREG,	RLEFT|RESCC,
		"	ZB	AR,A1\n	ashl	A1,AL,AL\n",

LS,	INAREG|INTAREG|FORCC,
	SAREG|AWD,	TWORD,
	SAREG|NIAWD,	ANYSIGNED|ANYUSIGNED,
		NAREG|NASL|NASR,	RESC1|RESCC,
		"	ashl	AR,AL,A1\n",

LS,	INAREG|INTAREG|FORCC,
	SAREG|AWD,	TWORD,
	SSOREG,	ANYSIGNED|ANYUSIGNED,
		NAREG|NASL|NASR,	RESC1|RESCC,
		"	ashl	AR,AL,A1\n",

LS,	INAREG|INTAREG|FORCC,
	SAREG|AWD,	TWORD,
	SOREG,	ANYSIGNED|ANYUSIGNED,
		NAREG|NASR,	RESC1|RESCC,
		"	ZB	AR,A1\n	ashl	A1,AL,A1\n",

INCR,	FOREFF,
	SAREG|AWD,	TANY,
	SCON|SNAME,	TANY,
		0,	RLEFT,
		"	ZE\n",

DECR,	FOREFF,
	SAREG|AWD,	TANY,
	SCON|SNAME,	TANY,
		0,	RLEFT,
		"	ZE\n",

INCR,	INAREG|INTAREG,
	SAREG|AWD,	TANY,
	SCON|SNAME,	TANY,
		NAREG,	RESC1,
		"	ZD\n",

DECR,	INAREG|INTAREG,
	SAREG|AWD,	TANY,
	SCON|SNAME,	TANY,
		NAREG,	RESC1,
		"	ZD\n",

ASSIGN,	INAREG|FOREFF|FORCC,
	SAREG|AWD,	TFLOAT|TDOUBLE,
	SAREG|AWD,	TUCHAR|TUSHORT,
		NAREG|NASL,	RLEFT|RESCC,
		"	ZA\n",

ASSIGN,	INAREG|FOREFF|FORCC,
	SAREG|AWD,	TANY,
	SAREG|AWD,	TANY,
		0,	RLEFT|RESCC,
		"	ZA\n",

ASSIGN,	FOREFF,
	SFLD,	TANY,
	SAREG|AWD,	TWORD,
		0,	RNOP,
		"	insv	AR,$H,$S,AL\n",

ASSIGN,	INAREG,
	SFLD,	ANYSIGNED,
	SAREG|AWD,	TWORD,
		NAREG,	RESC1,
		"	insv	AR,$H,$S,AL\n	extv	$H,$S,AL,A1\n",

ASSIGN,	INAREG,
	SFLD,	ANYUSIGNED,
	SAREG|AWD,	TWORD,
		NAREG,	RESC1,
		"	insv	AR,$H,$S,AL\n	extzv	$H,$S,AL,A1\n",

ASSIGN,	INAREG|FOREFF|FORCC,
	SAREG|AWD,	TWORD,
	SFLD,	ANYSIGNED,
		0,	RLEFT|RESCC,
		"	extv	$H,$S,AR,AL\n",

ASSIGN,	INAREG|FOREFF|FORCC,
	SAREG|AWD,	TWORD,
	SFLD,	ANYUSIGNED,
		0,	RLEFT|RESCC,
		"	extzv	$H,$S,AR,AL\n",

/* dummy UNARY MUL entry to get U* to possibly match OPLTYPE */
UNARY MUL,	FOREFF,
	SCC,	TANY,
	SCC,	TANY,
		0,	RNULL,
		"	HELP HELP HELP\n",

OREG,	INTEMP,
	SANY,	TANY,
	SOREG,	TDOUBLE,
		2*NTEMP,	RESC1,
		"	movq	AR,A1\n",

OREG,	INTEMP,
	SANY,	TANY,
	SOREG,	TANY,
		NTEMP,	RESC1,
		"	movZF	AR,A1\n",

REG,	INTEMP,
	SANY,	TANY,
	SAREG,	TDOUBLE,
		2*NTEMP,	RESC1,
		"	movq	AR,A1\n",

REG,	INTEMP,
	SANY,	TANY,
	SAREG,	TANY,
		NTEMP,	RESC1,
		"	movZF	AR,A1\n",

#if defined(FORT) || defined(SPRECC)
REG,	FORARG,
	SANY,	TANY,
	SAREG,	TFLOAT,
		0,	RNULL,
		"	cvtfd	AR,-(sp)\n",

REG,	FORARG,
	SANY,	TANY,
	SAREG,	TDOUBLE,
		0,	RNULL,
		"	movq	AR,-(sp)\n",
#endif

OPLEAF,	FOREFF,
	SANY,	TANY,
	SAREG|AWD,	TANY,
		0,	RLEFT,
		"",

OPLTYPE,	INAREG|INTAREG,
	SANY,	TANY,
	SANY,	TANY,
		NAREG|NASR,	RESC1,
		"	ZA\n",

OPLTYPE,	FORCC,
	SANY,	TANY,
	SANY,	TANY,
		0,	RESCC,
		"	tstZR	AR\n",

OPLTYPE,	FORARG,
	SANY,	TANY,
	SANY,	TANY,
		0,	RNULL,
		"	ZV\n",

#if defined(FORT) || defined(SPRECC)
UNARY MINUS,	INTAREG|FORCC,
	SAREG|AWD,	TFLOAT,
	SANY,	TANY,
		NAREG|NASL,	RESC1|RESCC,
		"	mnegZL	TAL,A1\n",

#endif

UNARY MINUS,	INTAREG|FORCC,
	SAREG|AWD,	TWORD|TDOUBLE,
	SANY,	TANY,
		NAREG|NASL,	RESC1|RESCC,
		"	mnegZL	AL,A1\n",

COMPL,	INTAREG|FORCC,
	SAREG|AWD,	TWORD,
	SANY,	TANY,
		NAREG|NASL,	RESC1|RESCC,
		"	mcomZL	AL,A1\n",

AND,	FORCC,
	SAREG|AWD,	TCHAR|TSHORT,
	SCON,	ANYFIXED,
		NAREG|NASL,	RESCC,
		"	ZZ\n",

AND,	FORCC,
	SAREG|AWD,	TWORD|ANYUSIGNED,
	SCON,	ANYFIXED,
		0,	RESCC,
		"	ZZ\n",

ASG AND,	INAREG|FOREFF|FORCC,
	SAREG,	TWORD,
	SCON,	TWORD,
		0,	RLEFT|RESCC,
		"	bicl2	AR,AL\n",

/* General cases for DIV and ASG DIV are handled below with OPMUL */
/* Some special cases are handled in optim2() */

DIV,	INAREG|FOREFF|FORCC,
	SAREG|AWD,	TUNSIGNED|TULONG,
	SCON,	ANYUSIGNED,
		NAREG|NEVEN,	RESC1|RESCC,
		"	movl	AL,A1\n	clrl	U1\n	ediv	AR,A1,A1,U1\n",

ASG DIV,	INAREG|FOREFF|FORCC,
	SAREG|AWD,	TINT|TLONG|TUNSIGNED|TULONG,
	SMCON,	ANYUSIGNED,
		0,	RLEFT|RESCC,
		"	ZJ\n",

ASG DIV,	INAREG|FOREFF|FORCC,
	SAREG|AWD,	TINT|TLONG|TUNSIGNED|TULONG,
	SCON,	ANYUSIGNED,
		NAREG|NEVEN,	RLEFT|RESCC,
		"	movl	AL,A1\n	clrl	U1\n	ediv	AR,A1,AL,U1\n",

MOD,	INAREG|INTAREG,
	SAREG|AWD,	TINT|TLONG,
	SAREG|AWD,	TINT|TLONG,
		NAREG,	RESC1,
		"	divl3	AR,AL,A1\n	mull2	AR,A1\n	subl3	A1,AL,A1\n",

MOD,	INAREG|FOREFF,
	SAREG|AWD,	TUNSIGNED|TULONG,
	SMCON,	ANYUSIGNED,
		NAREG|NASL,	RLEFT|RESC1,
		"	ZJ\n",

MOD,	INAREG|FOREFF,
	SAREG|AWD,	TUNSIGNED|TULONG,
	SCON,	ANYUSIGNED,
		NAREG|NEVEN,	RESC1,
		"	movl	AL,A1\n	clrl	U1\n	ediv	AR,A1,U1,A1\n",

/* should only see UNSIGNED lhs here if converted from UCHAR/USHORT lhs */
ASG MOD,	INAREG|FOREFF|FORCC,
	SAREG|AWD,	TINT|TLONG|TUNSIGNED|TULONG,
	SAREG|AWD,	TINT|TLONG,
		NAREG,	RLEFT|RESCC,
		"	divl3	AR,AL,A1\n	mull2	AR,A1\n	subl2	A1,AL\n",

ASG MOD,	INAREG|FOREFF,
	SAREG|AWD,	TINT|TLONG|TUNSIGNED|TULONG,
	SMCON,	ANYUSIGNED,
		0,	RLEFT,
		"	ZJ\n",

ASG MOD,	INAREG|FOREFF,
	SAREG|AWD,	TINT|TLONG|TUNSIGNED|TULONG,
	SCON,	ANYUSIGNED,
		NAREG|NEVEN,	RLEFT,
		"	movl	AL,A1\n	clrl	U1\n	ediv	AR,A1,A1,AL\n",

ASG OPMUL,	INAREG|FOREFF|FORCC,
	SAREG|AWD,	TINT|TUNSIGNED|TLONG|TULONG,
	SAREG|AWD,	TINT|TUNSIGNED|TLONG|TULONG,
		0,	RLEFT|RESCC,
		"	OL2	AR,AL\n",

OPMUL,	INAREG|INTAREG|FORCC,
	STAREG,	TINT|TUNSIGNED|TLONG|TULONG,
	SAREG|AWD,	TINT|TUNSIGNED|TLONG|TULONG,
		0,	RLEFT|RESCC,
		"	OL2	AR,AL\n",

OPMUL,	INAREG|INTAREG|FORCC,
	SAREG|AWD,	TINT|TUNSIGNED|TLONG|TULONG,
	SAREG|AWD,	TINT|TUNSIGNED|TLONG|TULONG,
		NAREG|NASL|NASR,	RESC1|RESCC,
		"	OL3	AR,AL,A1\n",

ASG PLUS,	INAREG|FOREFF|FORCC,
	SAREG|AWD,	ANYFIXED,
	SONE,	TANY,
		0,	RLEFT|RESCC,
		"	incZL	AL\n",

ASG MINUS,	INAREG|FOREFF|FORCC,
	SAREG|AWD,	ANYFIXED,
	SONE,	TANY,
		0,	RLEFT|RESCC,
		"	decZL	AL\n",

PLUS,	INAREG|INTAREG|FORCC,
	STAREG,	TWORD,
	SONE,	TWORD,
		0,	RLEFT|RESCC,
		"	incZL	AL\n",

MINUS,	INAREG|INTAREG|FORCC,
	STAREG,	TWORD,
	SONE,	TWORD,
		0,	RLEFT|RESCC,
		"	decZL	AL\n",

ASG OPSIMP,	INAREG|FOREFF|FORCC,
	SAREG|AWD,	TWORD,
	SAREG|AWD,	TWORD,
		0,	RLEFT|RESCC,
		"	OL2	AR,AL\n",

ASG OPSIMP,	INAREG|FOREFF|FORCC,
	AWD,	TSHORT|TUSHORT,
	SAREG|SNAME|STARNM,	TSHORT|TUSHORT|TINT|TUNSIGNED|TLONG|TULONG,
		0,	RLEFT|RESCC,
		"	OW2	AR,AL\n",

ASG OPSIMP,	INAREG|FOREFF|FORCC,
	AWD,	TSHORT|TUSHORT,
	SSOREG,	TSHORT|TUSHORT|TINT|TUNSIGNED|TLONG|TULONG,
		0,	RLEFT|RESCC,
		"	OW2	AR,AL\n",

ASG OPSIMP,	INAREG|FOREFF|FORCC,
	AWD,	TSHORT|TUSHORT,
	SSCON,	ANYFIXED,
		0,	RLEFT|RESCC,
		"	OW2	AR,AL\n",

ASG OPSIMP,	INAREG|FOREFF|FORCC,
	AWD,	TSHORT|TUSHORT,
	AWD,	TSHORT|TUSHORT,
		0,	RLEFT|RESCC,
		"	OW2	AR,AL\n",

ASG OPSIMP,	INAREG|FOREFF|FORCC,
	AWD,	TCHAR|TUCHAR,
	SSOREG,	ANYFIXED,
		0,	RLEFT|RESCC,
		"	OB2	AR,AL\n",

ASG OPSIMP,	INAREG|FOREFF|FORCC,
	AWD,	TCHAR|TUCHAR,
	SAREG|SNAME|STARNM,	ANYFIXED,
		0,	RLEFT|RESCC,
		"	OB2	AR,AL\n",

ASG OPSIMP,	INAREG|FOREFF|FORCC,
	AWD,	TCHAR|TUCHAR,
	SCCON,	ANYFIXED,
		0,	RLEFT|RESCC,
		"	OB2	AR,AL\n",

ASG OPSIMP,	INAREG|FOREFF|FORCC,
	AWD,	TCHAR|TUCHAR,
	AWD,	TCHAR|TUCHAR,
		0,	RLEFT|RESCC,
		"	OB2	AR,AL\n",

OPSIMP,	INAREG|INTAREG|FORCC,
	STAREG,	ANYFIXED,
	SAREG|AWD,	TWORD,
		0,	RLEFT|RESCC,
		"	OL2	AR,AL\n",

OPSIMP,	INAREG|INTAREG|FORCC,
	SAREG|AWD,	TWORD,
	SAREG|AWD,	TWORD,
		NAREG|NASL|NASR,	RESC1|RESCC,
		"	OL3	AR,AL,A1\n",

ASG OPFLOAT,	INAREG|FOREFF|FORCC,
	SAREG|AWD,	TDOUBLE,
	SAREG|AWD,	TDOUBLE,
		0,	RLEFT|RESCC,
		"	OD2	AR,AL\n",

ASG OPFLOAT,	INAREG|FOREFF|FORCC,
	SAREG|AWD,	TFLOAT,
	SAREG|AWD,	TFLOAT,
		0,	RLEFT|RESCC,
#if defined(FORT) || defined(SPRECC)
		"	OF2	AR,TAL\n",
#else
		"	OF2	AR,AL\n",
#endif

ASG OPFLOAT,	INAREG|INTAREG|FOREFF|FORCC,
	SAREG|AWD,	TFLOAT,
	SAREG|AWD,	TDOUBLE,
		NAREG,	RLEFT|RESC1|RESCC,
		"	cvtfd	AL,A1\n	OD2	AR,A1\n	cvtdf	A1,AL\n",

ASG OPFLOAT,	INAREG|FOREFF|FORCC,
	SAREG|AWD,	ANYFIXED,
#ifndef SPRECC
	SAREG|AWD,	TDOUBLE,		/* force FLOAT to register */
#else
	SAREG|AWD,	TFLOAT|TDOUBLE,
#endif
		NAREG,	RLEFT|RESCC,	/* usable() knows we need a reg pair */
		"	ZG\n",

OPFLOAT,	INAREG|INTAREG|FORCC,
	STAREG,	TDOUBLE,
	SAREG|AWD,	TDOUBLE,
		0,	RLEFT|RESCC,
		"	OD2	AR,AL\n",

OPFLOAT,	INAREG|INTAREG|FORCC,
	SAREG|AWD,	TDOUBLE,
	SAREG|AWD,	TDOUBLE,
		NAREG|NASL|NASR,	RESC1|RESCC,
		"	OD3	AR,AL,A1\n",

#if defined(FORT) || defined(SPRECC)
OPFLOAT,	INAREG|INTAREG|FORCC,
	STAREG,		TFLOAT,
	SAREG|AWD,	TFLOAT,
		0,	RLEFT|RESCC,
		"	OF2	AR,TAL\n",

OPFLOAT,	INAREG|INTAREG|FORCC,
	SAREG|AWD,	TFLOAT,
	SAREG|AWD,	TFLOAT,
		NAREG|NASL|NASR,	RESC1|RESCC,
		"	OF3	AR,AL,TA1\n",
#endif

#ifdef FORT
/* perform some implicit conversions XXX SHOULD FIX f77 FRONT END */
OPFLOAT,	INAREG|INTAREG|FORCC,
	SAREG|AWD,	TFLOAT,
	SAREG|AWD,	TDOUBLE,
		NAREG|NASL,	RESC1|RESCC,
		"	cvtfd	AL,A1\n	OD2	AR,A1\n",

OPFLOAT,	INAREG|INTAREG|FORCC,
	SAREG|AWD,	TDOUBLE,
	SAREG|AWD,	TFLOAT,
		NAREG|NASR,	RESC1|RESCC,
		"	cvtfd	AR,A1\n	OD3	A1,AL,A1\n",
#endif

	/* Default actions for hard trees ... */

# define DF(x) FORREW,SANY,TANY,SANY,TANY,REWRITE,x,""

UNARY MUL, DF( UNARY MUL ),

INCR, DF(INCR),

DECR, DF(INCR),

ASSIGN, DF(ASSIGN),

STASG, DF(STASG),

FLD, DF(FLD),

OPLEAF, DF(NAME),

OPLOG,	FORCC,
	SANY,	TANY,
	SANY,	TANY,
		REWRITE,	BITYPE,
		"",

OPLOG,	DF(NOT),

COMOP, DF(COMOP),

INIT, DF(INIT),

OPUNARY, DF(UNARY MINUS),

ASG OPANY, DF(ASG PLUS),

OPANY, DF(BITYPE),

FREE,	FREE,	FREE,	FREE,	FREE,	FREE,	FREE,	FREE,	"help; I'm in trouble\n" };
