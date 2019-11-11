#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "readFile.h"
#include "convert.h"
#include "opFunctions.h"
#define MAX 1000

void iFormat(int code, int i);
void rFormat(int code, int i);
void dFormat(int code, int i);
void getStats();
void executeInstructions();

struct instrData
{
   int rm, rn, rd, immediate, shamt, destAddress, branchAddress, bcondAddress
};

u_int64_t regArr[32];
u_int64_t memory[512];
u_int64_t stack[64];

int dataHazards = 0;
int controlHazards = 0;
struct instrData instrData[MAX];
int rawInstructions[MAX];
OpCodeInstr instructions[MAX];
int size = 0;
FILE *file;

int main(int argc, char const *argv[])
{
   size = readFile(argc, argv, rawInstructions, file) - 1;
   printf("%d Instructions: \n", size);
   for (int i = 0; i < size; i++)
   {
      printf("%x\n", rawInstructions[i]);
      instrData[i].rm = -1;
      instrData[i].rn = -1;
      instrData[i].rd = -1;
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
      case B:
         bFormat(rawInstructions[index], index);
      case CB:
         cbFormat(rawInstructions[index], index);
      }
   }
   executeInstructions();
   getStats();
}

void iFormat(int code, int i)
{
   //printf("%d\n", code);
   instrData[i].rd = code & 0x1F;
   instrData[i].rn = (code >> 5) & 0x1F;
   instrData[i].immediate = (code >> 10) & 0xfff;
   printf("Instruction Type: I \n");
   printf(" -> Immediate = %d, Rn = %d, Rd = %d\n", instrData[i].immediate, instrData[i].rn, instrData[i].rd);
}

void rFormat(int code, int i)
{
   instrData[i].rd = code & 0x1F;
   instrData[i].rn = (code >> 5) & 0x1F;
   instrData[i].shamt = (code >> 10) & 0x3F;
   instrData[i].rm = (code >> 16) & 0x1F;
   printf("Instruction Type: R \n");
   printf(" -> Rm = %d, Rn = %d, Rd = %d\n", instrData[i].rm, instrData[i].rn, instrData[i].rd);
}

void dFormat(int code, int i)
{
   instrData[i].rd = code & 0x1F;
   instrData[i].rn = (code >> 5) & 0x1F;
   instrData[i].destAddress = (code >> 12) & 0x1ff;
   printf("Instruction Type: D \n");
   printf(" -> DTa = %d, Rn = %d, Rt = %d\n", instrData[i].destAddress, instrData[i].rn, instrData[i].rd);
}

void bFormat(int code, int i)
{
   instrData[i].branchAddress = code & 0x3FFFFFF;
   printf("Instruction Type: B \n");
   printf(" -> Branch Address: %d", instrData[i].branchAddress);
}

void cbFormat(int code, int i)
{
   instrData[i].rd = code & 0x1F;
   instrData[i].bcondAddress = code >> 5 & 0x7FFFF;
}

void executeInstructions()
{
   unsigned int conditionFlags[16];

   for (int i = 0; i < size; i++)
   {
      switch (instructions[i].opcode)
      {

         //
         // TYPE I
         //

      case 580:
         addI(instrData[i].rd, instrData[i].rn, instrData[i].immediate, regArr);
         break;

      case 836:
         subI(instrData[i].rd, instrData[i].rn, instrData[i].immediate, regArr);
         break;

      case 584:
         andI(instrData[i].rd, instrData[i].rn, instrData[i].immediate, regArr);
         break;

      case 712:
         orrI(instrData[i].rd, instrData[i].rn, instrData[i].immediate, regArr);
         break;

      case 964:
         subis(instrData[i].rd, instrData[i].rn, instrData[i].immediate, regArr, conditionFlags);
         break;

      case 840:
         eorI(instrData[i].rd, instrData[i].rn, instrData[i].immediate, regArr);
         break;

      case 1360:
         orr(instrData[i].rd, instrData[i].rn, instrData[i].rm, regArr);
         break;

         //
         // TYPE R
         //

      case 1104:
         and(instrData[i].rd, instrData[i].rn, instrData[i].rm, regArr);
         break;

      case 1112:
         add(instrData[i].rd, instrData[i].rn, instrData[i].rm, regArr);
         break;

      case 1624:
         sub(instrData[i].rd, instrData[i].rn, instrData[i].rm, regArr);
         break;

      case 1880:
         subs(instrData[i].rd, instrData[i].rn, instrData[i].rm, regArr, conditionFlags);
         break;

      case 1691:
         lsl(instrData[i].rd, instrData[i].rn, instrData[i].shamt, regArr);
         break;

      case 1690:
         lsr(instrData[i].rd, instrData[i].rn, instrData[i].shamt, regArr);
         break;

      case 1240:
         mul(instrData[i].rd, instrData[i].rn, instrData[i].rm, regArr);
         break;

      case 1616:
         eor(instrData[i].rd, instrData[i].rn, instrData[i].rm, regArr);
         break;

      case 1238:
         udiv(instrData[i].rd, instrData[i].rn, instrData[i].rm, regArr);
         break;

         //
         // TYPE D
         //

      case 1984:
         stur(instrData[i].rd, memory, instrData[i].rn, instrData[i].destAddress, regArr);
         break;

      case 1986:
         ldur(instrData[i].rd, instrData[i].rn, memory, instrData[i].destAddress, regArr);
         break;

      //*****
      // TYPE B/CB
      //*****

      //BRANCH
      case 5:
         i += instrData[i].branchAddress - 1;
         break;

      //BL
      case 37:
         //set BR register to current address
         regArr[30] = (u_int64_t)i;

         //branch
         i += instrData[i].branchAddress - 1;
         break;

      //BCOND
      case 84:
         if (bCond(instrData[i].rd, regArr, conditionFlags))
         {
            i += instrData[i].bcondAddress - 1;
         }
         break;

      //BR
      case 1712:
         i += instrData[i].rd - 1;
         break;

      //CBZ
      case 180:
         if (instrData[i].rd == 0)
         {
            i += instrData[i].bcondAddress - 1;
         }
         break;

      //CBNZ
      case 181:
         if (instrData[i].rd != 0)
         {
            i += instrData[i].bcondAddress - 1;
         }
         break;

      //*****
      // TYPE JK
      //*****

      //DUMP
      case 2046:
         dump(regArr, memory, stack, file);
         break;

      //HALT
      case 2047:
         halt(regArr, memory, stack, file);
         break;

      //PRNL
      case 2044:
         prnl();
         break;

      //PRNT
      case 1021:
         prnt(instrData[i].rd, regArr);
         break;
      }
   }
}

void getStats()
{
   int j = 0;
   for (int i = 0; i < size; i++)
   {
      j = i;
      while (j < i + 4 && j < size - 1)
      {
         if (instrData[i].rd != -1)
         {
            bool a = instrData[i].rd == instrData[j].rm;
            bool b = instrData[i].rd == instrData[j].rn;
            bool c = instrData[i].rd == instrData[j].rd;
            if (a || b || c)
            {
               dataHazards++;
               //cycles += 5 - (j-i);
            }
         }
      }
   }
   printf("Number of data hazards = %d\n", dataHazards);
}
