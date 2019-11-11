typedef enum OpType
{
    DEFAULT,
    R,
    I,
    D,
    B,
    CB,
    IW,
    JS
};

typedef enum OpName
{
    BRANCH,
    BL,
    BCOND,
    CBZ,
    CBNZ,
    ADDI,
    ANDI,
    ORRI,
    SUBI,
    EORI,
    SUBIS,
    STURB,
    LDURB,
    STURH,
    LDURH,
    AND,
    ADD,
    SDIV,
    UDIV,
    MUL,
    SMULH,
    UMULH,
    ORR,
    STURW,
    LDURSW,
    LSR,
    LSL,
    BR,
    EOR,
    SUB,
    SUBS,
    STUR,
    LDUR,
    PRNL,
    PRNT,
    DUMP,
    HALT
};

typedef struct OpCodeInstr
{
    int opcode;
    enum OpName opname;
    enum OpType optype;
} OpCodeInstr;

int search(int opcode);

OpCodeInstr convert(int code);
