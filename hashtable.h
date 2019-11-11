#ifndef HASHTABLE_H_
#define HASHTABLE_H_

#define SIZE 40

typedef struct OpCodeInstr
{
    char *opcode;
    char *instruction;
} OpCodeInstr;

OpCodeInstr *hashArray[SIZE];
OpCodeInstr *dummyOp;
OpCodeInstr *op;

int hashCode(char *key);

OpCodeInstr *search(int key);

void insert(char *key, char *data);

OpCodeInstr *delete (OpCodeInstr op);

void display();

#endif // HASHTABLE_H_