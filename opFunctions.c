#include <stdio.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <ctype.h>
#include "opFunctions.h"
u_int8_t memory[4096];
u_int8_t stack[512];
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

void subis(int des, int reg1, int val, u_int64_t *regArr, unsigned int *conditionFlags)
{
    regArr[des] = regArr[reg1] - val;
    conditionsHandler(des, regArr, conditionFlags);
}

void eorI(int des, int reg1, int val, u_int64_t *regArr)
{
    regArr[des] = regArr[reg1] ^ val;
}

void orr(int des, int reg1, int reg2, u_int64_t *regArr)
{
    regArr[des] = regArr[reg1] | regArr[reg2];
}

//TYPE R
void and (int des, int reg1, int reg2, u_int64_t *regArr)
{
    regArr[des] = regArr[reg1] & regArr[reg2];
}

void add(int des, int reg1, int reg2, u_int64_t *regArr)
{
    regArr[des] = regArr[reg1] + regArr[reg2];
}

void sub(int des, int reg1, int reg2, u_int64_t *regArr)
{
    regArr[des] = regArr[reg1] - regArr[reg2];
}

void subs(int des, int reg1, int reg2, u_int64_t *regArr, unsigned int *conditionFlags)
{
    regArr[des] = regArr[reg1] - regArr[reg2];
    conditionsHandler(des, regArr, conditionFlags);
}

void lsl(int des, int reg1, int offset, u_int64_t *regArr)
{
    regArr[des] = regArr[reg1] << offset;
}

void lsr(int des, int reg1, int offset, u_int64_t *regArr)
{
    regArr[des] = regArr[reg1] >> offset;
}

void mul(int des, int reg1, int reg2, u_int64_t *regArr)
{
    regArr[des] = regArr[reg1] * regArr[reg2];
}

void eor(int des, int reg1, int reg2, u_int64_t *regArr)
{
    regArr[des] = regArr[reg1] ^ regArr[reg2];
}

void udiv(int des, int reg1, int reg2, u_int64_t *regArr) //??
{
    regArr[des] = regArr[reg1] / regArr[reg2];
}

//TYPE D
void ldur(int des, int reg1, u_int64_t *memory, int offset, u_int64_t *regArr)
{
    int val = offset / 8; //divides offset by 8
    regArr[des] = memory[val + regArr[reg1]];
}

void stur(int des, u_int64_t *memory, int reg, int offset, u_int64_t *regArr)
{
    int val = offset / 8;
    memory[regArr[reg]] = regArr[des];
}

//TYPE B
int bCond(int reg, u_int64_t *regArr, unsigned int *conditionFlags)
{
    if (conditionFlags[reg] == 1)
    {
        return 1;
    }

    return 0;
}

//TYPE JK
void hexdump(FILE *f, uint64_t *start, size_t size) //displays contents of registers, memory, and disassembled program
{
    size_t i;
    for (i = 0; i < size - (size % 2); i += 2)
    {

        printf("%08x "
               "%lx %lx \n",
               (int32_t)i * 8, start[i], start[i + 1]);
    }
    printf("%08x\n", (int32_t)size * 16);
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
    printf("%s\n", "Stack: ");
    hexdump(f, stack, 64);
    prnl();
    prnl();
    printf("%s\n", "Main Memory: ");
    hexdump(f, memory, 512);
}

void halt(u_int64_t *regArr, u_int64_t *memory, u_int64_t *stack, FILE *f)
{
    dump(regArr, memory, stack, f);
    exit(0);
}

void prnl()
{
    printf("\n");
}

void prnt(int reg, u_int64_t *regArr)
{
    printf("Register: %d --> Decimal: %ld", reg, regArr[reg]);
}

//OTHER
void conditionsHandler(int des, u_int64_t *regArr, unsigned int *conditionFlags)
{
    //Loop through enum
    for (int i = EQ; i <= LE; i++)
    {
        switch (i)
        {
        case EQ:
            conditionFlags[i] = ((int64_t)regArr[des] == 0) ? 1 : 0;
            break;

        case NE:
            conditionFlags[i] = ((int64_t)regArr[des] != 0) ? 1 : 0;
            break;

        case HS:
            conditionFlags[i] = ((int64_t)regArr[des] >= 0) ? 1 : 0;
            break;

        case LO:
            conditionFlags[i] = ((int64_t)regArr[des] < 0) ? 1 : 0;
            break;

        case MI:
            conditionFlags[i] = ((int64_t)regArr[des] < 0) ? 1 : 0;
            break;

        case PL:
            conditionFlags[i] = ((int64_t)regArr[des] >= 0) ? 1 : 0;
            break;

        case VS:
            conditionFlags[i] = 0;
            break;

        case VC:
            conditionFlags[i] = 0;
            break;

        case HI:
            conditionFlags[i] = ((int64_t)regArr[des] > 0) ? 1 : 0;
            break;

        case LS:
            conditionFlags[i] = ((int64_t)regArr[des] <= 0) ? 1 : 0;
            break;

        case GE:
            conditionFlags[i] = ((int64_t)regArr[des] >= 0) ? 1 : 0;
            break;

        case LT:
            conditionFlags[i] = ((int64_t)regArr[des] < 0) ? 1 : 0;
            break;

        case GT:
            conditionFlags[i] = ((int64_t)regArr[des] > 0) ? 1 : 0;
            break;

        case LE:
            conditionFlags[i] = ((int64_t)regArr[des] <= 0) ? 1 : 0;
            break;
        }
    }
}