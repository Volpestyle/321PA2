#include <stdio.h>
#include <stdlib.h>
#include "convert.h"

OpCodeInstr opcodes[] = {
    {5, BRANCH, B},
    {37, BL, B},
    {84, BCOND, CB},
    {180, CBZ, CB},
    {181, CBNZ, CB},
    {580, ADDI, I},
    {584, ANDI, I},
    {712, ORRI, I},
    {836, SUBI, I},
    {840, EORI, I},
    {964, SUBIS, I},
    {448, STURB, D},
    {450, LDURB, D},
    {960, STURH, D},
    {962, LDURH, D},
    {1104, AND, R},
    {1112, ADD, R},
    {1238, SDIV, R},
    {1238, UDIV, R},
    {1240, MUL, R},
    {1242, SMULH, R},
    {1246, UMULH, R},
    {1360, ORR, R},
    {1472, STURW, D},
    {1476, LDURSW, D},
    {1690, LSR, R},
    {1691, LSL, R},
    {1712, BR, R},
    {1616, EOR, R},
    {1624, SUB, R},
    {1880, SUBS, R},
    {1984, STUR, D},
    {1986, LDUR, D},
    {2044, PRNL, JS},
    {2045, PRNT, JS},
    {2046, DUMP, JS},
    {2047, HALT, JS}};

int search(int opcode)
{
    int i;
    printf("%d\n", opcode);
    for (i = 0; i < 37; i++)
    {
        if (opcodes[i].opcode == opcode)
        {
            return i;
        }
    }
    return -1;
}

OpCodeInstr convert(int code)
{
    int shift = 6;
    int breakout = 0;
    int foundIndex = 0;
    while (breakout != 1)
    {
        foundIndex = search(code >> (32 - shift));
        printf("%d\n", foundIndex);
        if (foundIndex >= 0)
        {
            return opcodes[foundIndex];
        }
        if (shift < 11)
        {
            shift++;
        }
        else
        {
            breakout = 1;
        }
    }
}
