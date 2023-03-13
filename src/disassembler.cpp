#include "common.h"

u16 read_binary(const char *file_name, u8 **data) {
	FILE *f = fopen(file_name, "rb");
	if (!f) {
		*data = 0;
		return 0;
	}

	fseek(f, 0, SEEK_END);
	u16 length = (u16) ftell(f);
	fseek(f, 0, SEEK_SET);

	u8 *content = (u8 * ) malloc(length);
	fread(content, 1, length, f);

	fclose(f);

	*data = content;
	return length;
}

void decode(u16 inst) {
	u8 first = inst & 0xff;
	u8 second = (inst >> 8) & 0xff;

	u8 opcode = (first >> 2);
	u8 dir    = first & 0b10;
	u8 word   = first & 0b1;

	u8 mod = (second >> 6) & 0b11;
	u8 reg = (second >> 3) & 0b00111;
	u8 rm  = second & 0b111;

	u8 dst = dir ? reg : rm;
	u8 src = dir ? rm : reg;

	printf(
			"%s %s, %s",
			opcode_names[opcode],
			register_name(dst, word),
			register_name(src, word)
		);
}

int main() {
	u8 *data;
	u16 len = read_binary("listing_0037_single_register_mov", &data);
	if (data == 0) {
		return 1;
	}

	u16 *ip = (u16 *) data;
	decode(*ip);

	return 0;
}
