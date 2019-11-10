#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "readFile.h"
#include "convert.h"
#include "opFunctions.h"
#define MAX 1000

void iFormat(OpCodeInstr op, int i);
void rFormat(OpCodeInstr op, int i);
void dFormat(OpCodeInstr op, int i);
void executeInstructions();

struct instructionData
{
   int rm, rn, rd, immediate, shamt, destAddress, branchAddress, condBranchAddress
};

u_int64_t regArr[32];
u_int64_t memory[512];
u_int64_t stack[64];

struct instructionData instructionData[MAX];
int rawInstructions[MAX];
OpCodeInstr instructions[MAX];
int size = 0;

int main(int argc, char const *argv[])
{
   FILE *file;
   size = readFile(argc, argv, rawInstructions, file);
   printf("%d\n", size);
   for (int i = 0; i < size; i++)
   {
      printf("%x\n", rawInstructions[i]);
   }
   for (int index = 0; index < size; index++)
   {
      OpCodeInstr op = convert(rawInstructions[index]);
      instructions[index] = op;
      switch (op.optype)
      {
      case R:
         rFormat(op, index);
         break;
      case I:
         iFormat(op, index);
         break;
      case D:
         dFormat(op, index);
         break;
      }
      executeInstructions();
   }
}

void iFormat(OpCodeInstr op, int i)
{
   instructionData[i].rd = op.opcode & 0x1F;
   instructionData[i].rn = (op.opcode >> 5) & 0x1F;
   instructionData[i].immediate = (op.opcode >> 10) & 0xfff;
   printf(" -- R");
   printf(" -> Rm = %d, Rn = %d, Rd = %d\n", instructionData[i].rm, instructionData[i].rn, instructionData[i].rd);
}

void rFormat(OpCodeInstr op, int i)
{
   instructionData[i].rd = op.opcode & 0x1F;
   instructionData[i].rd = (op.opcode >> 5) & 0x1F;
   instructionData[i].shamt = (op.opcode >> 10) & 0x3F;
   instructionData[i].rm = (op.opcode >> 16) & 0x1F;
   printf(" -- I");
   printf(" -> Immediate = %d, Rn = %d, Rd = %d\n", instructionData[i].immediate, instructionData[i].rn, instructionData[i].rd);
}

void dFormat(OpCodeInstr op, int i)
{
   instructionData[i].rd = op.opcode & 0x1F;
   instructionData[i].rn = (op.opcode >> 5) & 0x1F;
   instructionData[i].destAddress = (op.opcode >> 12) & 0x1ff;
   printf(" -- D");
   printf(" -> DTa = %d, Rn = %d, Rt = %d\n", instructionData[i].destAddress, instructionData[i].rn, instructionData[i].rd);
}

void executeInstructions()
{
   for (int i = 0; i < size; i++)
   {
      switch (instructions[i].opcode)
      {
      case 580:
         addI(instructionData[i].rd, instructionData[i].rn, instructionData[i].immediate, regArr);
         break;
      }
   }
}
