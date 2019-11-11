#include <stdio.h>
#include <stdlib.h>
#include "convert.h"

OpCodeInstr opcodes[] = {
    {5, B, B},        //
    {37, BL, B},      //
    {84, BCOND, CB},  //
    {180, CBZ, CB},   //
    {181, CBNZ, CB},  //
    {580, ADDI, I},   //
    {584, ANDI, I},   //
    {712, ORRI, I},   //
    {836, SUBI, I},   //
    {840, EORI, I},   //
    {964, SUBIS, I},  //
    {448, STURB, D},  // X - don't do
    {450, LDURB, D},  // X
    {960, STURH, D},  // X
    {962, LDURH, D},  // X
    {1104, AND, R},   //
    {1112, ADD, R},   //
    {1238, SDIV, R},  // X
    {1238, UDIV, R},  //
    {1240, MUL, R},   //
    {1242, SMULH, R}, // X
    {1246, UMULH, R}, // X
    {1360, ORR, R},
    {1472, STURW, D},  // X
    {1476, LDURSW, D}, // X
    {1690, LSR, R},    //
    {1691, LSL, R},    //
    {1712, BR, R},     //
    {1616, EOR, R},    //
    {1624, SUB, R},    //
    {1880, SUBS, R},   //
    {1984, STUR, D},   //
    {1986, LDUR, D},   //
    {2044, PRNL, JS},  //
    {2045, PRNT, JS},  //
    {2046, DUMP, JS},  //
    {2047, HALT, JS}}; //

int search(int opcode)
{
    int i;
    //printf("%x\n", opcode);
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
    int mask = 0;
    while (breakout != 1)
    {
        switch (shift)
        {
        case 6:
            mask = 0x3F;
            break;

        case 8:
            mask = 0xFF;
            break;

        case 9:
            mask = 0x1FF;
            break;

        case 10:
            mask = 0x3FF;
            break;

        case 11:
            mask = 0x7FF;
            break;
        }
        foundIndex = search(code >> (32 - shift) & mask);
        if (foundIndex >= 0)
        {
            return opcodes[foundIndex];
        }
        switch (shift)
        {
        case 11:
            shift = -1;
            break;

        case 10:
            shift = 11;
            break;

        case 9:
            shift = 10;
            break;

        case 8:
            shift = 9;
            break;

        case 6:
            shift = 8;
            break;
        }
        if (shift < 0)
        {
            breakout = 1;
        }
    }
}
