#include <cstdio>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

#include "common.h"

#define LOW(x) (x & 0b11111111)
#define HIGH(x) ((x >> 8) & 0b11111111)

static FILE *f;

void write(u8 byte) {
	fputc(byte, f);
}

void encode_inst_modrm(u8 inst, u8 mod, u8 reg, u8 rm) {
	u8 op = 0b00000000;
	op |= mod << 6;
	op |= reg << 3;
	op |= rm;

	write(inst);
	write(op);
}

void encode_mov_rm(u8 dir, u8 wide, u8 mod, u8 reg, u8 rm) {
	u8 mov = 0b10001000;
	mov |= dir << 1;
	mov |= wide;

	encode_inst_modrm(mov, mod, reg, rm);
}

void encode_mov_reg_reg(u8 src, u8 dst, u8 wide) {
	encode_mov_rm(0, wide, DISPLACE_REG, src, dst);
}

void encode_mov_reg_mem(u8 dir, u8 wide, u8 reg, u8 rm) {
	encode_mov_rm(dir, wide, DISPLACE_0, reg, rm);
}

void encode_mov_reg_mem8(u8 dir, u8 wide, u8 reg, u8 rm, u8 displace) {
	encode_mov_rm(dir, wide, DISPLACE_8, reg, rm);
	write(displace);
}

void encode_mov_reg_mem16(u8 dir, u8 wide, u8 reg, u8 rm, u16 displace) {
	encode_mov_rm(dir, wide, DISPLACE_16, reg, rm);
	write(LOW(displace));
	write(HIGH(displace));
}

void encode_mov_mem8_imm8(u8 rm, u8 displace, u8 data) {
	u8 mov = 0b11000110;

	encode_inst_modrm(mov, DISPLACE_8, 0b000, rm);

	write(displace);
	write(data);
}

void encode_mov_mem16_imm8(u8 rm, u16 displace, u8 data) {
	u8 mov = 0b11000110;

	encode_inst_modrm(mov, DISPLACE_16, 0b000, rm);

	write(LOW(displace));
	write(HIGH(displace));
	write(data);
}

void encode_mov_mem8_imm16(u8 rm, u8 displace, u16 data) {
	u8 mov = 0b11000111;

	encode_inst_modrm(mov, DISPLACE_8, 0b000, rm);

	write(displace);
	write(LOW(data));
	write(HIGH(data));
}

void encode_mov_mem16_imm16(u8 rm, u16 displace, u16 data) {
	u8 mov = 0b11000111;

	encode_inst_modrm(mov, DISPLACE_16, 0b000, rm);

	write(LOW(displace));
	write(HIGH(displace));
	write(LOW(data));
	write(HIGH(data));
}

void encode_mov_reg_imm8(u8 reg, u8 imm) {
	u8 mov = 0b10110000;
	mov |= reg;

	write(mov);
	write(imm);
}

void encode_mov_reg_imm16(u8 reg, u16 imm) {
	u8 mov = 0b10111000;
	mov |= reg;

	write(mov);
	write(LOW(imm));
	write(HIGH(imm));
}

void encode_push_reg(u8 reg) {
	u8 push = 0b01010000;
	push |= reg;

	write(push);
}

void encode_push_mem(u8 rm) {
	u8 push = 0b11111111;
	
	encode_inst_modrm(push, DISPLACE_0, 0b110, rm);
}

void encode_push_mem8(u8 rm, u8 displace) {
	u8 push = 0b11111111;

	encode_inst_modrm(push, DISPLACE_8, 0b110, rm);

	write(displace);
}

void encode_push_mem16(u8 rm, u16 displace) {
	u8 push = 0b11111111;

	encode_inst_modrm(push, DISPLACE_16, 0b110, rm);

	write(LOW(displace));
	write(HIGH(displace));
}

void encode_pop_reg(u8 reg) {
	u8 pop = 0b01011000;
	pop |= reg;

	write(pop);
}

void encode_pop_mem(u8 rm) {
	u8 pop = 0b10001111;
	
	encode_inst_modrm(pop, DISPLACE_0, 0b000, rm);
}

void encode_pop_mem8(u8 rm, u8 displace) {
	u8 pop = 0b10001111;

	encode_inst_modrm(pop, DISPLACE_8, 0b000, rm);

	write(displace);
}

void encode_pop_mem16(u8 rm, u16 displace) {
	u8 pop = 0b10001111;

	encode_inst_modrm(pop, DISPLACE_16, 0b000, rm);

	write(LOW(displace));
	write(HIGH(displace));
}

int main() {
	const char *input = "mov ax, bx";

	f = fopen("out", "wb");

	encode_mov_reg_mem8(0, 1, REG_AX, 0b110, 4);
	encode_push_reg(REG_AX);
	encode_push_mem8(0b110, 8);

	fclose(f);

	return 0;
}
