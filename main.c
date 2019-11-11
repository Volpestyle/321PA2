#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "readFile.h"
#include "convert.h"
#include "opFunctions.h"
#define MAX 1000

void iFormat(int code, int i);
void rFormat(int code, int i);
void dFormat(int code, int i);
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
FILE *file;

int main(int argc, char const *argv[])
{
   size = readFile(argc, argv, rawInstructions, file) - 1;
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
         rFormat(rawInstructions[index], index);
         break;
      case I:
         iFormat(rawInstructions[index], index);
         break;
      case D:
         dFormat(rawInstructions[index], index);
         break;
      }
   }
   executeInstructions();
}

void iFormat(int code, int i)
{
   //printf("%d\n", code);
   instructionData[i].rd = code & 0x1F;
   instructionData[i].rn = (code >> 5) & 0x1F;
   instructionData[i].immediate = (code >> 10) & 0xfff;
   printf("Instruction Type: R \n");
   printf(" -> Immediate = %d, Rn = %d, Rd = %d\n", instructionData[i].immediate, instructionData[i].rn, instructionData[i].rd);
}

void rFormat(int code, int i)
{
   printf("%d\n", code);
   instructionData[i].rd = code & 0x1F;
   instructionData[i].rd = (code >> 5) & 0x1F;
   instructionData[i].shamt = (code >> 10) & 0x3F;
   instructionData[i].rm = (code >> 16) & 0x1F;
   printf("Instruction Type: I \n");
   printf(" -> Rm = %d, Rn = %d, Rd = %d\n", instructionData[i].rm, instructionData[i].rn, instructionData[i].rd);
}

void dFormat(int code, int i)
{
   instructionData[i].rd = code & 0x1F;
   instructionData[i].rn = (code >> 5) & 0x1F;
   instructionData[i].destAddress = (code >> 12) & 0x1ff;
   printf("Instruction Type: D \n");
   printf(" -> DTa = %d, Rn = %d, Rt = %d\n", instructionData[i].destAddress, instructionData[i].rn, instructionData[i].rd);
}

void executeInstructions()
{
   for (int i = 0; i < size; i++)
   {
      switch (instructions[i].opcode)
      {
      //ADD I
      case 580:
         addI(instructionData[i].rd, instructionData[i].rn, instructionData[i].immediate, regArr);
         break;

      //ADD
      case 1112:
         add(instructionData[i].rd, instructionData[i].rn, instructionData[i].rm, regArr);
         break;

      //SUB
      case 1624:
         sub(instructionData[i].rd, instructionData[i].rn, instructionData[i].rm, regArr);

      //DUMP
      case 2046:
         dump(regArr, memory, stack, file);
         break;

      //HALT
      case 2047:
         halt(regArr, memory, stack, file);
         break;
      }
   }
}
