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

//TYPE D
void ldur(int des, int reg1, u_int64_t *memory, int offset, u_int64_t *regArr)
{
    int val = offset / 8; //divides offset by 8
    regArr[des] = memory[val + regArr[reg1]];
}