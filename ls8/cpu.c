#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include "cpu.h"
#include "handlers.h"

#define DEBUG 0

/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */
void throwHandsUpAndGiveUp(void)
{
  perror("Error");
  exit(1);
}

void cpu_load(struct cpu *cpu, char *argv[])
{
  FILE *fp = fopen(argv[1], "r");
  char *strBuff = 0;
  char *token;
  long length;
  int address;

  printf("\n"); // print a new line so I better distinguish output in terminal

  /* Open file and copy to string buffer strBuff */
  // Shamelessly copied from 
  // https://stackoverflow.com/questions/174531/how-to-read-the-content-of-a-file-to-a-string-in-c
  if (  !fp  )                          throwHandsUpAndGiveUp(); // Error if FP is null

  if (  fseek(fp, 0, SEEK_END)   != 0)  throwHandsUpAndGiveUp(); 
        length = ftell(fp);
  if (  length == -1 )                  throwHandsUpAndGiveUp();
  if (  fseek(fp, 0, SEEK_SET)   != 0)  throwHandsUpAndGiveUp();

  strBuff = malloc(length + 1);

  if (strBuff) fread(strBuff, 1, length, fp);

  fclose(fp);
  strBuff[length] = '\0';

  /* Tokenize and parse string in strBuff */
  if (strBuff)
  {
    // start to process your data / extract strings here...
    token = strtok(strBuff, " \r\n");
    address = 0;

    while (token != NULL) {
      if (token[0] == '0' || token[0] == '1') {
        unsigned char numberedToken = strtoul(token, NULL, 2);
        #if DEBUG
        // Print instructions being written to memory
        printf("token at address %i:\t%s | HEX: %02x\n", address, token,numberedToken);
        #endif
        cpu->ram[address++] = numberedToken;
      }
      token = strtok(NULL, " \r\n");
    }
  }
}

/**
 * ALU
 */
void alu(struct cpu *cpu, enum alu_op op)
{
  int regA;
  int regB;

  switch (op) {
    case ALU_ADD:
      regA = cpu_ram_read(cpu, cpu->PC + 1);
      regB = cpu_ram_read(cpu, cpu->PC + 2);
      handleALU_ADD(cpu, regA, regB);
      break;
    case ALU_MUL:
      // TODO
      regA = cpu_ram_read(cpu, cpu->PC + 1);
      regB = cpu_ram_read(cpu, cpu->PC + 2);
      handleALU_MUL(cpu, regA, regB);
      break;

    // TODO: implement more ALU ops
  }
}

/**
 * Run the CPU
 */
void cpu_run(struct cpu *cpu)
{
  int running = 1; // True until we get a HLT instruction
  unsigned char IR;

  while (running) {
    // TODO
    // 1. Get the value of the current instruction (in address PC).
    // 2. switch() over it to decide on a course of action.
    // 3. Do whatever the instruction should do according to the spec.
    // 4. Move the PC to the next instruction.

    IR = cpu_ram_read(cpu, cpu->PC);

    switch (IR) {
      case LDI:
        handleLDI(cpu);
        break;
      case PRN:
        handlePRN(cpu);
        break;
      case ADD:
        alu(cpu, ALU_ADD);
        break;
      case MUL:
        alu(cpu, ALU_MUL);
        break;
      case PUSH:
        handlePUSH(cpu);
        break;
      case POP:
        handlePOP(cpu);
        break;
      case CALL:
        handleCALL(cpu);
        break;
      case RET:
        handleRET(cpu);
        break;
      case HLT:
        running = 0;
        break;
      default:
        fprintf(stderr, "Unkown instruction at %i: %02x\n", cpu->PC, IR);
        exit(2);
    }
  }

  #if DEBUG
  // Print first 16 positions of the stack in memory
  for (int q = 0xF3; q > (0xF3 - 0x10); q--) {
    printf("stack @ addr:%i: %i\n", q, cpu_ram_read(cpu, q));
  }
  // Print current stack pointer
  // At 0th place, SP should be 0xF4/244;
  printf("current stack pointer: %i\n", cpu->SP);
  #endif
}

/**
 * Initialize a CPU struct
 */
void cpu_init(struct cpu *cpu)
{
  // TODO: Initialize the PC and other special registers
  cpu->PC = 0;
  cpu->SP = 0xF4;
  // TODO: Zero registers and RAM
  cpu->registers = (unsigned char *) calloc(8, sizeof(unsigned char));
  cpu->ram = (unsigned char *) calloc(256, sizeof(unsigned char));
}
