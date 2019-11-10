
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <endian.h>

void parseHexCode(__uint32_t);
void execute_i_format(__uint32_t code, char* operation);
void execute_r_format(__uint32_t code, char* operation);
void setFlags(__uint32_t val);

__uint32_t memory[4096];
__uint32_t reg[64];
__uint32_t stack[512];
int pc = 0;
int n_flag;
int z_flag;
int v_flag;
int c_flag;

int main () {
   FILE *fp;
   int sz;
   //char c[] = "this is tutorialspoint";
   

   /* Open file for both reading and writing     */
   char file[] = "i.machine";
   fp = fopen(file, "r");

   /* Write data to the file */
   //fwrite(c, strlen(c) + 1, 1, fp);

   /* Seek to the beginning of the file */
    struct stat st;
    stat(file, &st);
    sz = st.st_size;
    //printf("%d\n", sz);
    __uint32_t buffer[sz];


   /* Read and display data */
   fread(buffer, 4, sz, fp);
   
   for(pc = 0; pc < sz/4; pc++){
      __uint32_t val = buffer[pc];
      val = htobe32(val);
      parseHexCode(val);
      //printf("%x\n", val);
      
    
   }
   fclose(fp);
   
   return(0);
}
void parseHexCode(__uint32_t val){
   __uint32_t opcode = val >> 21;
   switch(opcode)
   {
   case 0b10001011000:
      printf("ADD Instruction\n");
      execute_r_format(val, "add");
      return;
   
   case 0b10001010000:
      printf("AND Instruction\n");
      execute_r_format(val, "and");
      return;
   
   case 0b11001010000:
      printf("EOR Instruction\n");
      return;

   case 0b11001011000:
      printf("SUB Instruction\n");
      execute_r_format(val, "sub");
      return;

   case 0b11111000000:
      printf("STUR Instruction\n");
      return;
   
   case 0b11111000010:
      printf("LDUR Instruction\n");
      return;

   case 0b11010011011:
      printf("LSL Instruction\n");
      execute_r_format(val, "lsl");
      return;
   
   case 0b11010011010:
      printf("LSR Instruction\n");
      execute_r_format(val, "lsr");
      return;

   case 0b11010110000:
      printf("BR Instruction\n");
      execute_r_format(val, "br");
      return;

   case 0b11101011000:
      printf("SUBS Instruction\n");
      execute_r_format(val, "subs");
      return;
   
   case 0b10011011000:
      printf("MUL Instruction\n");
      execute_r_format(val, "mul");
   }
   opcode = val >> 22;
   switch (opcode)
   {
   case 0b1001000100:
      printf("ADDI instruction\n");
      execute_i_format(val, "addi");
      return;
   
   case 0b1101000100:
      printf("SUBI Instruction\n");
      execute_i_format(val, "subi");

      return;
   
   case 0b1001001000:
      printf("ANDI Instruction\n");
      execute_i_format(val, "andi");
      return;
   
   case 0b1011001000:
      printf("ORRI Instruction\n");
      execute_i_format(val, "orri");
      break;

   case 0b1111000100:
      printf("SUBIS Instruction\n");
      execute_i_format(val, "subis");
      return;
   }
   
}

void execute_i_format(__uint32_t code, char* operation){
   int rd = code & 0x1F;
   int rn = (code >> 5) & 0x1F;
   int immediate = (code >> 10) & 0xfff;
   printf("%d ", reg[rd]);
   //printf("%x\n%x\n%d\n", rd, rn, immediate);
   if(operation == "addi"){
      reg[rd] = reg[rn] + immediate;
   }
   else if(operation == "subi"){
      reg[rd] = reg[rn] - immediate;
   }
   else if(operation == "andi"){
      reg[rd] = reg[rn] & immediate;
   }
   else if(operation == "orri"){
      reg[rd] = reg[rn] | immediate;
   }
   else if(operation == "subis"){
      reg[rd] = reg[rn] - immediate;
      setFlags(reg[rd]);
   }
   printf("%d\n", reg[rd]);
}

void execute_r_format(__uint32_t code, char* operation){
   int rd = code & 0x1F;
   int rn = (code >> 5) & 0x1F;
   int shamt = (code >> 10) & 0x3F;
   int rm = (code >> 16) & 0x1F;
   //TODO implement shamt
   //printf("%d ", reg[rd]);
   if(operation == "add"){
      reg[rd] = reg[rn] + reg[rm];
   }
   else if(operation == "sub"){
      reg[rd] = reg[rn] - reg[rm];
   }
   else if(operation == "and"){
      reg[rd] = reg[rn] & reg[rm];
   }
   else if(operation == "subs"){
      reg[rd] = reg[rn] - reg[rm];
      //TODO: set flags
   }
   else if(operation == "lsl"){
      reg[rd] = reg[rn] << shamt;
   }
   else if(operation == "lsr"){
      reg[rd] = reg[rn] >> shamt;
   }
   else if(operation == "eor"){
      reg[rd] = reg[rn] ^ reg[rm];
   }
   else if(operation == "br"){
      //printf("%d %d %d", rd, rm, rn);
      pc = reg[rn];
   }
   else if(operation == "mul"){
      reg[rd] = reg[rn] * reg[rm];
   }
   
   //printf("%d\n", reg[rd]);
}

void execute_d_format(__uint32_t code, char* operation){
   int rt = code & 0x1F;
   int rn = (code >> 5) & 0x1F; 
   int op2 = (code >> 10) & 0x3;
   int address = (code >> 12) & 0x1ff;
   if(operation == "ldur"){
      if(rt == 29 || rt == 30){
         reg[rt] = stack[rn + address/8]; 
      }
      else{
         reg[rt] = memory[rn + address/8]; 
      }
   }
   else if(operation == "stur"){
      if(rt == 29 || rt == 30){
         reg[rt] = stack[rn + address/8]; 
      }
      else{
         memory[rn + address/8] = rt;
      }
   }
}


void setFlags(__uint32_t val){ 
   if(val == 0){
      z_flag = 1;
   }
   else if(val < 0){
      n_flag = 1;
   }
   else if(val > 0){
      n_flag = 0;
   }
}
