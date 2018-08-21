#ifndef _CPU_H_
#define _CPU_H_

// Holds all information about the CPU
struct cpu {
  // TODO
  // PC
  // registers (array)
  // ram (array)
  int PC;
  int SP;
  unsigned char *registers;
  unsigned char *ram;
};

// Instructions

// These use binary literals. If these aren't available with your compiler, hex
// literals should be used.

#define LDI  0b10000010
#define PRN  0b01000111
#define HLT  0b00000001
#define ADD  0b10100000
#define MUL  0b10100010
#define PUSH 0b01000101
#define POP  0b01000110
#define CALL 0b01010000
#define RET  0b00010001

/* ALU Operations */
enum alu_op {
  ALU_ADD,
  // ALU_SUB,
  ALU_MUL,
  // ALU_DIV,
  // ALU_INC,
  // ALU_DEC,
  // ALU_CMP
};

// TODO: more instructions here. These can be used in cpu_run().

// Function declarations

extern void cpu_load(struct cpu *cpu, char *argv[]);
extern void cpu_init(struct cpu *cpu);
extern void cpu_run(struct cpu *cpu);

#endif
