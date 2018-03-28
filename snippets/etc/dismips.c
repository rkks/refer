/*	dismips.c
 *
 *	Created by Giampiero Caprino
 *	Backer Street Software
 *
 *	This software is part of REC, the reverse engineering compiler.
 *	You are free to use, copy, modify and distribute this software.
 *	If you fix bugs and or extend this software, I
 *	will be happy to include your changes in the
 *	most current version of the source.
 *	just send mail to caprino@netcom.com
 */

struct instr {
	char	*name;
	unsigned long	opcode;
	unsigned long	opcode2;
	int	(*optype)();
	int	(*op1)();
	int	(*op2)();
	int	(*op3)();
};

static	char	opcode[32];	/* opcode mnemonic of current instruction */
static	char	ops[8][32];	/* operands of current instruction */
static	char	*op[8];		/* pointers to operand */
static	char	**opp;		/* pointer to current operand */
static	long	cur_pc;		/* address of current instruction */
extern	char	mneu[];		/* the output string from the disassembler */

static	int	op26(struct instr *ip, unsigned long val)
{
	if((val >> 26) == ip->opcode)
	    return(1);
	return(0);
}

static	int	op32(struct instr *ip, unsigned long val)
{
	if(val == ip->opcode)
	    return(1);
	return(0);
}

static	int	op26_16(struct instr *ip, unsigned long val)
{
	if((val >> 26) == ip->opcode && ((val >> 16) & 0x1F) == ip->opcode2)
	    return(1);
	return(0);
}

static	int	op26__16(struct instr *ip, unsigned long val)
{
	if((val >> 26) == ip->opcode && (val & 0xFFFF) == ip->opcode2)
	    return(1);
	return(0);
}

static	int	op21_6(struct instr *ip, unsigned long val)
{
	if((val >> 21) == ip->opcode && (val & 0x3F) == ip->opcode2)
	    return(1);
	return(0);
}

static	int	op5___11(struct instr *ip, unsigned long val)
{
	if((val >> 26) == ip->opcode && (val & 0x7FF) == ip->opcode2)
	    return(1);
	return(0);
}

static	int	op26_5(struct instr *ip, unsigned long val)
{
	if((val >> 26) == ip->opcode && (val & 0x3F) == ip->opcode2)
	    return(1);
	return(0);
}

static	int	op26_16_11(struct instr *ip, unsigned long val)
{
	if((val >> 26) == ip->opcode && (val & 0x001F07FF) == ip->opcode2)
	    return(1);
	return(0);
}

static	int	op26_21(struct instr *ip, unsigned long val)
{
	if((val >> 26) == ip->opcode && (val & 0x001FFFFF) == ip->opcode2)
	    return(1);
	return(0);
}

static	int	op_r21(struct instr *ip, unsigned long val)
{
	sprintf(*opp, "r%d", (val >> 21) & 0x1F);
	return(1);
}

static	int	op_r16(struct instr *ip, unsigned long val)
{
	sprintf(*opp, "r%d", (val >> 16) & 0x1F);
	return(1);
}

static	int	op_r11(struct instr *ip, unsigned long val)
{
	sprintf(*opp, "r%d", (val >> 11) & 0x1F);
	return(1);
}

static	int	op_i16(struct instr *ip, unsigned long val)
{
	long	lv;

	lv = (long)val;
	lv <<= 16;
	lv >>= 16;
	sprintf(*opp, "#0x%lx", lv);
	return(1);
}

static	int	op_u16(struct instr *ip, unsigned long val)
{
	sprintf(*opp, "#0x%lx", val & 0xffff);
	return(1);
}

static	int	op_i6(struct instr *ip, unsigned long val)
{
	sprintf(*opp, "#%ld", (val >> 6) & 0x1f);
	return(1);
}

static	int	op_h16(struct instr *ip, unsigned long val)
{
	sprintf(*opp, "#0x%lx", val << 16);
	return(1);
}

static	int	op_b21(struct instr *ip, unsigned long val)
{
	long	lv;

	lv = (long)val;
	lv <<= 16;
	lv >>= 16;
	sprintf(*opp, "%ld(r%d)", lv, (val >> 21) & 0x1F);
	return(1);
}

static	int	op_p16(struct instr *ip, unsigned long val)
{
	long	lv;

	lv = (long)val;
	lv <<= 16;
	lv >>= 14;
	lv += cur_pc + 4;
	sprintf(*opp, "0x%lx", lv);
	return(1);
}

static	int	op_p26(struct instr *ip, unsigned long val)
{
	long	lv;

	lv = (long)val & 0x03FFFFFF;
	lv <<= 2;
	lv |= ((cur_pc + 4) & 0xF0000000);
	sprintf(*opp, "0x%lx", lv);
	return(1);
}

static	int	op_null(struct instr *ip, unsigned long val)
{
	return(0);
}

struct	instr	itab[] = {
	{ "add",	0, 0x20,	op5___11, op_r11, op_r21, op_r16 },
	{ "addi",	8, 0x00,	op26, op_r16, op_r21, op_i16 },
	{ "addiu",	9, 0x00,	op26, op_r16, op_r21, op_i16 },
	{ "addu",	0, 0x21,	op5___11, op_r11, op_r21, op_r16 },
	{ "and",	0, 0x24,	op5___11, op_r11, op_r21, op_r16 },
	{ "andi",	0xc, 0, 	op26, op_r16, op_r21, op_u16 },
	{ "beq",	0x4, 0, 	op26, op_r21, op_r16, op_p16 },
	{ "beql",	0x14, 0, 	op26, op_r21, op_r16, op_p16 },
	{ "bgez",	0x1, 0x1, 	op26_16, op_r21, op_p16, op_null },
	{ "bgezal",	0x1, 0x11, 	op26_16, op_r21, op_p16, op_null },
	{ "bgezall",	0x1, 0x13, 	op26_16, op_r21, op_p16, op_null },
	{ "bgezl",	0x1, 0x03, 	op26_16, op_r21, op_p16, op_null },
	{ "bgtz",	0x7, 0x0, 	op26_16, op_r21, op_p16, op_null },
	{ "bgtzl",	0x17, 0x0, 	op26_16, op_r21, op_p16, op_null },
	{ "blez",	0x6, 0x0, 	op26_16, op_r21, op_p16, op_null },
	{ "blezl",	0x16, 0x0, 	op26_16, op_r21, op_p16, op_null },
	{ "bltz",	0x1, 0x0, 	op26_16, op_r21, op_p16, op_null },
	{ "bltzal",	0x1, 0x10, 	op26_16, op_r21, op_p16, op_null },
	{ "bltzall",	0x1, 0x12, 	op26_16, op_r21, op_p16, op_null },
	{ "bne",	0x5, 0x0, 	op26, op_r21, op_r16, op_p16 },
	{ "bnel",	0x15, 0x0, 	op26, op_r21, op_r16, op_p16 },
	/* { "break",	0x15, 0x0, 	op26_16, op_r21, op_r16, op_p16 }, */
	/* { "cache",	0x15, 0x0, 	op26_16, op_r21, op_r16, op_p16 }, */
	{ "div",	0, 0x1a,	op26__16, op_r21, op_r16, op_null },
	{ "divu",	0, 0x1b,	op26__16, op_r21, op_r16, op_null },
	{ "j",		2, 0,		op26, op_p26, op_null, op_null },
	{ "jal",	3, 0,		op26, op_p26, op_null, op_null },
	{ "jalr",	0, 0x9,		op26_16_11, op_r11, op_r21, op_null },
	{ "jr",		0, 0x8,		op26_21, op_r21, op_null, op_null },
	{ "lb",		0x20, 0,	op26, op_r16, op_b21, op_null },
	{ "lbu",	0x24, 0,	op26, op_r16, op_b21, op_null },
	{ "lh",		0x21, 0,	op26, op_r16, op_b21, op_null },
	{ "lhu",	0x25, 0,	op26, op_r16, op_b21, op_null },
	{ "ll",		0x30, 0,	op26, op_r16, op_b21, op_null },
	{ "lui",	0x0F, 0,	op26, op_r16, op_h16, op_null },
	{ "lw",		0x23, 0,	op26, op_r16, op_b21, op_null },
	{ "lwu",	0x2F, 0,	op26, op_r16, op_b21, op_null },
	{ "lwl",	0x22, 0,	op26, op_r16, op_b21, op_null },
	{ "lwr",	0x26, 0,	op26, op_r16, op_b21, op_null },
	{ "mult",	0, 0x18,	op26__16, op_r21, op_r16, op_null },
	{ "multu",	0, 0x19,	op26__16, op_r21, op_r16, op_null },
	{ "nor",	0, 0x27,	op5___11, op_r11, op_r21, op_r16 },
	{ "or",		0, 0x25,	op5___11, op_r11, op_r21, op_r16 },
	{ "ori",	0xd, 0,		op26, op_r16, op_r21, op_u16 },
	{ "sb",		0x28, 0,	op26, op_r16, op_b21, op_null },
	{ "sc",		0x38, 0, 	op26, op_r16, op_b21, op_null },
	{ "sh",		0x29, 0,	op26, op_r16, op_b21, op_null },
	{ "sll",	0, 0,		op26_5, op_r11, op_r16, op_i6 },
	{ "sllv",	0, 4,		op5___11, op_r11, op_r16, op_r21 },
	{ "slt",	0, 0x2a,	op5___11, op_r11, op_r21, op_r16 },
	{ "slti",	0xa, 0,		op26, op_r16, op_r21, op_i16 },
	{ "sltiu",	0xb, 0,		op26, op_r16, op_r21, op_i16 },
	{ "sltu",	0, 0x2b,	op5___11, op_r11, op_r21, op_r16 },
	{ "sra",	0, 3,		op21_6, op_r11, op_r16, op_i6 },
	{ "srav",	0, 7,		op5___11, op_r11, op_r16, op_r21 },
	{ "srl",	0, 2,		op21_6, op_r11, op_r16, op_i6 },
	{ "srlv",	0, 6,		op5___11, op_r11, op_r16, op_r21 },
	{ "sub",	0, 0x22,	op5___11, op_r11, op_r21, op_r16 },
	{ "subu",	0, 0x23,	op5___11, op_r11, op_r21, op_r16 },
	{ "sw",		0x2b, 0,	op26, op_r16, op_b21, op_null },
	{ "swl",	0x2a, 0,	op26, op_r16, op_b21, op_null },
	{ "swr",	0x2e, 0,	op26, op_r16, op_b21, op_null },
	{ "sync",	0xf, 0,		op32, op_null, op_null, op_null },
	{ "syscall",	0xc, 0,		op32, op_null, op_null, op_null },
	{ "xor",	0, 0x26,	op5___11, op_r11, op_r21, op_r16 },
	{ "xori",	0x0e, 0,	op26, op_r16, op_r21, op_u16 },
	{ 0 }
};

char	*mips_disass(unsigned long addr, unsigned long val)
{
	struct	instr	*ip;
	int	i;
	char	*sep;

	for(i = 0; i < 8; ++i) {
	    ops[i][0] = 0;
	    op[i] = ops[i];
	}
	opp = op;
	cur_pc = addr;
	for(ip = itab; ip->name; ++ip) {
	    if(ip->optype && ip->optype(ip, val)) {
		strcpy(opcode, ip->name);
		if(ip->op1(ip, val))
		    ++opp;
		if(ip->op2(ip, val))
		    ++opp;
		if(ip->op3(ip, val))
		    ++opp;
		break;
	    }
	}
	*op = 0;		/* end of arguments */
	if(!ip->name) {
	    sprintf(opcode, "Unknown opcode");
	    sprintf(ops[0], "0x%08lx", val);
	    ops[1][0] = 0;
	}
	strcpy(mneu, opcode);
	sep = " ";
	for(i = 0; ops[i][0]; ++i) {
	    strcat(mneu, sep);
	    strcat(mneu, ops[i]);
	    sep = ",";
	}
	return(mneu);
}

