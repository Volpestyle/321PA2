#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>
void addI(int des, int reg1, int val, u_int64_t *regArr);
void subI(int des, int reg1, int val, u_int64_t *regArr);
void andI(int des, int reg1, int val, u_int64_t *regArr);
void orrI(int des, int reg1, int val, u_int64_t *regArr);
void subis(int des, int reg1, int val, u_int64_t *regArr, unsigned int *condFlag);

void and (int des, int reg1, int reg2, u_int64_t *regArr);
void add(int des, int reg1, int reg2, u_int64_t *regArr);
void sub(int des, int reg1, int reg2, u_int64_t *regArr);
void subs(int des, int reg1, int reg2, u_int64_t *regArr, unsigned int *condFlag);
void lsl(int des, int reg1, int offset, u_int64_t *regArr);
void lsr(int des, int reg1, int offset, u_int64_t *regArr);
void eor(int des, int reg1, int reg2, u_int64_t *regArr);
void mul(int des, int reg1, int reg2, u_int64_t *regArr);

void ldur(int des, int reg1, u_int64_t *memory, int offset, u_int64_t *regArr);
