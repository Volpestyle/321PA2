#include <stdio.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <ctype.h>
#include "opFunctions.h"
//uint32_t??
u_int8_t memory[4096]; //4096? //1025
u_int8_t stack[512];   //512?? //129
u_int8_t reg[32];

//TYPE I

void addI(int des, int reg1, int val, u_int64_t *regArr)
{
    regArr[des] = regArr[reg1] + val;
}

void subI(int des, int reg1, int val, u_int64_t *regArr)
{
    regArr[des] = regArr[reg1] - val;
}

void andI(int des, int reg1, int val, u_int64_t *regArr)
{
    regArr[des] = regArr[reg1] & val;
}

void orrI(int des, int reg1, int val, u_int64_t *regArr)
{
    regArr[des] = regArr[reg1] | val;
}

void subis(int des, int reg1, int val, u_int64_t *regArr, unsigned int *condFlag)
{
    regArr[des] = regArr[reg1] - val;
}

//TYPE R

void add(int des, int reg1, int reg2, u_int64_t *regArr)
{
    regArr[des] = regArr[reg1] + regArr[reg2];
}

void sub(int des, int reg1, int reg2, u_int64_t *regArr)
{
    regArr[des] = regArr[reg1] - regArr[reg2];
}

void and (int des, int reg1, int reg2, u_int64_t *regArr)
{
    regArr[des] = regArr[reg1] & regArr[reg2];
}
void subs(int des, int reg1, int reg2, u_int64_t *regArr, unsigned int *condFlag)
{
    regArr[des] = regArr[reg1] - regArr[reg2];
    //setConditionals(des, regArr, condFlag);
}
void lsl(int des, int reg1, int offset, u_int64_t *regArr)
{
    regArr[des] = regArr[reg1] << offset;
}
void lsr(int des, int reg1, int offset, u_int64_t *regArr)
{
    regArr[des] = regArr[reg1] >> offset;
}
void eor(int des, int reg1, int reg2, u_int64_t *regArr)
{
    regArr[des] = regArr[reg1] ^ regArr[reg2];
}

void mul(int des, int reg1, int reg2, u_int64_t *regArr)
{
    regArr[des] = regArr[reg1] * regArr[reg2];
}

void dump(u_int64_t *regArr, u_int64_t *memory, u_int64_t *stack, FILE *f)
{
    printf("%s\n", "Registers: ");
    for (int i = 0; i < 32; i++)
    {

        if (i == 16)
        {
            printf("(IPO)  X%d: %ld\n", i, regArr[i]);
        }
        else if (i == 17)
        {
            printf("(IP1)  X%d: %ld\n", i, regArr[i]);
        }
        else if (i == 28)
        {
            printf(" (SP)  X%d: %ld\n", i, regArr[i]);
        }
        else if (i == 29)
        {
            printf(" (FP)  X%d: %ld\n", i, regArr[i]);
        }
        else if (i == 30)
        {
            printf(" (LR)  X%d: %ld\n", i, regArr[i]);
        }
        else if (i == 31)
        {
            printf("(XZR)  X%d: %ld\n", i, regArr[i]);
        }
        else if (i < 10)
        {
            printf("       X%d:  %ld\n", i, regArr[i]);
        }
        else
        {
            printf("       X%d: %ld\n", i, regArr[i]);
        }
    }
    prnl();
    prnl();
    //SP & FP - initialized to the size of the stack
    printf("%s\n", "Stack: ");
    hexdump(f, stack, 64);
    prnl();
    prnl();
    printf("%s\n", "Main Memory: ");
    hexdump(f, memory, 512);
}

void prnl()
{
    printf("\n");
}

//TYPE D
void ldur(int des, int reg1, u_int64_t *memory, int offset, u_int64_t *regArr)
{
    int val = offset / 8; //divides offset by 8
    regArr[des] = memory[val + regArr[reg1]];
}

//OTHER
void hexdump(FILE *f, uint64_t *start, size_t size) //displays contents of registers, memory, and disassembled program
{
    size_t i;

    for (i = 0; i < size - (size % 2); i += 2)
    //for(i = 0; i < size; i++)
    {
        /*
     fprintf(f,
             "%08x "
             " %02hhx %02hhx %02hhx %02hhx %02hhx %02hhx %02hhx %02hhx "
             " %02hhx %02hhx %02hhx %02hhx %02hhx %02hhx %02hhx %02hhx "
             " |%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c|\n",
             (int32_t) i,
             start[i +  0], start[i +  1], start[i +  2], start[i +  3],
             start[i +  4], start[i +  5], start[i +  6], start[i +  7],
             start[i +  8], start[i +  9], start[i + 10], start[i + 11],
             start[i + 12], start[i + 13], start[i + 14], start[i + 15],
             printable_char(start[i +  0]), printable_char(start[i +  1]),
             printable_char(start[i +  2]), printable_char(start[i +  3]),
             printable_char(start[i +  4]), printable_char(start[i +  5]),
             printable_char(start[i +  6]), printable_char(start[i +  7]),
             printable_char(start[i +  8]), printable_char(start[i +  9]),
             printable_char(start[i + 10]), printable_char(start[i + 11]),
             printable_char(start[i + 12]), printable_char(start[i + 13]),
             printable_char(start[i + 14]), printable_char(start[i + 15]));
    */
        printf(
            "%08lx "
            //" %02hhx %02hhx %02hhx %02hhx %02hhx %02hhx %02hhx %02hhx "
            //" %02hhx %02hhx %02hhx %02hhx %02hhx %02hhx %02hhx %02hhx "
            "%lx %lx \n",
            //" |%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c|\n",
            (int32_t)i * 8,
            start[i], start[i + 1]);
        /*(start[i]>>56)&0xf, (start[i]>>48)&0xf, (start[i]>>40)&0xf, (start[i]>>32)&0xf,
                    (start[i]>>24)&0xf, (start[i]>>16)&0xf, (start[i]>>8)&0xf, (start[i])&0xf,
                    (start[i + 1]>>56)&0xf, (start[i + 1]>>48)&0xf, (start[i + 1]>>40)&0xf, (start[i + 1]>>32)&0xf,
                    (start[i + 1]>>24)&0xf, (start[i + 1]>>16)&0xf, (start[i + 1]>>8)&0xf, (start[i + 1])&0xf,
                    printable_char((start[i]>>56)&0xf), printable_char((start[i]>>48)&0xf),
                    printable_char((start[i]>>40)&0xf), printable_char((start[i]>>32)&0xf),
                    printable_char((start[i]>>24)&0xf), printable_char((start[i]>>16)&0xf),
                    printable_char((start[i]>>8)&0xf), printable_char((start[i])&0xf),
                    printable_char((start[i + 1]>>56)&0xf), printable_char((start[i + 1]>>48)&0xf),
                    printable_char((start[i + 1]>>40)&0xf), printable_char((start[i + 1]>>32)&0xf),
                    printable_char((start[i + 1]>>24)&0xf), printable_char((start[i + 1]>>16)&0xf),
                    printable_char((start[i + 1]>>8)&0xf), printable_char((start[i + 1])&0xf));*/
    }
    //printf("Mem at index %d is %d\n", i, start[i]);
    //fprintf(f, "%08x\n", (int32_t) size);
    printf("%08lx\n", (int32_t)size * 16);
}