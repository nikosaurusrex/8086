#include <stdio.h>
#include <stdlib.h>

typedef unsigned char u8;
typedef unsigned short u16;

enum : u8 {
	REG_AL = 0b000,
	REG_CL = 0b001,
	REG_DL = 0b010,
	REG_BL = 0b011,
	REG_AH = 0b100,
	REG_CH = 0b101,
	REG_DH = 0b110,
	REG_BH = 0b111,
};

enum : u8 {
	REG_AX = 0b000,
	REG_CX = 0b001,
	REG_DX = 0b010,
	REG_BX = 0b011,
	REG_SP = 0b100,
	REG_BP = 0b101,
	REG_SI = 0b110,
	REG_DI = 0b111,
};

enum {
	DISPLACE_0 = 0b00,
	DISPLACE_8 = 0b01,
	DISPLACE_16 = 0b10,
	DISPLACE_REG = 0b11,
};

enum : u8 {
	OP_MOV = 0b100010
};

const char *register_names[] = {
	"al", "cl", "dl", "bl", "ah", "ch", "dh", "bh",
	"ax", "cx", "dx", "bx", "sp", "bp", "si", "di"
};

const char *opcode_names[] = {
	[OP_MOV] = "mov"
};

void print_u8(u8 b) {
	for (int i = 8 - 1; i >= 0; --i) {
		u8 x = b & (1 << i);
		putc(x ? '1' : '0', stdout);
	}
	putc('\n', stdout);
}

void print_u16(u16 b) {
	for (int i = 16 - 1; i >= 0; --i) {
		u8 x = b & (1 << i);
		putc(x ? '1' : '0', stdout);
	}
	putc('\n', stdout);
}

const char *register_name(u8 reg, u8 w) {
	return register_names[reg + w * 8];
}