#ifndef __REG_H__
#define __REG_H__

#include "common.h"

enum { R_EAX, R_ECX, R_EDX, R_EBX, R_ESP, R_EBP, R_ESI, R_EDI };
enum { R_AX, R_CX, R_DX, R_BX, R_SP, R_BP, R_SI, R_DI };
enum { R_AL, R_CL, R_DL, R_BL, R_AH, R_CH, R_DH, R_BH };

/**
 * Re-organize the `CPU_state' structure to match the register
 * encoding scheme in i386 instruction format. For example, if we
 * access cpu.gpr[3]._16, we will get the `bx' register; if we access
 * cpu.gpr[1]._8[1], we will get the 'ch' register. Hint: Use `union'.
 * For more details about the register encoding scheme, see i386 manual.
 */

typedef struct {
	union {
		union {
			uint32_t _32;
			uint16_t _16;
			uint8_t _8[2];
		} gpr[8];

	/* Do NOT change the order of the GPRs' definitions. */
		struct{
			uint32_t eax, ecx, edx, ebx, esp, ebp, esi, edi;
		};
	};
	struct {
 		uint16_t CS, DS, SS, ES, FS, GS;
 	};
	union {
		struct {
			unsigned CF: 1;		//Carry Flag
			unsigned RES1: 1;
			unsigned PF: 1;		//Parity Flag
			unsigned RES2: 1;
			unsigned AF: 1;		//Assistant Flag
			unsigned RES3: 1;
			unsigned ZF: 1;		//Zero Flag
			unsigned SF: 1;		//Singal Flag
			unsigned TF: 1;		//Trap Flag
			unsigned IF: 1;		//Interrupt Enable Flag
			unsigned DF: 1;		//Direction Flag
			unsigned OF: 1;		//Overflow Flag
			unsigned IOPL: 2;	//12 13
			unsigned NT: 1;		//Nested Task Flag
			unsigned RES4: 1;
			unsigned RF: 1;		// Resume Flag
			unsigned VM: 1;		//Virtual-8086 Mode Flag
			unsigned AC: 1;		//Alignment Check
			unsigned VIF: 1;	//Virtual Interrupt Flag
			unsigned VIP: 1;	//Virtual Interrupt Pending Flag
			unsigned ID: 1;		//Identification Flag
			unsigned RES: 10;	//RES INDICATES INTEL RESERVED. DO NOT DEFINE
		};
		uint32_t EFLAGS;
	};
	swaddr_t eip;

} CPU_state;

extern CPU_state cpu;

static inline int check_reg_index(int index) {
	assert(index >= 0 && index < 8);
	return index;
}

#define reg_l(index) (cpu.gpr[check_reg_index(index)]._32)
#define reg_w(index) (cpu.gpr[check_reg_index(index)]._16)
#define reg_b(index) (cpu.gpr[check_reg_index(index) & 0x3]._8[index >> 2])

extern const char* regsl[];
extern const char* regsw[];
extern const char* regsb[];

#endif
