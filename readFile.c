
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <endian.h>

void parseHexCode(__uint32_t);
void execute_i_format(__uint32_t code, char* operation);
void execute_r_format(__uint32_t code, char* operation);

__uint32_t memory[64];
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
   
   for(int i = 0; i < sz/4; i++){
      __uint32_t val = buffer[i];
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
      printf("LSR Instruction");
      execute_r_format(val, "lsr");
      return;

   case 0b11010110000:
      printf("BR Instruction\n");
      return;

   case 0b11101011000:
      printf("SUBS Instruction\n");
      execute_r_format(val, "subs");
      return;
   
   
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
   printf("%d ", memory[rd]);
   //printf("%x\n%x\n%d\n", rd, rn, immediate);
   if(operation == "addi"){
      memory[rd] = memory[rn] + immediate;
   }
   else if(operation == "subi"){
      memory[rd] = memory[rn] - immediate;
   }
   else if(operation == "andi"){
      memory[rd] = memory[rn] & immediate;
   }
   else if(operation == "orri"){
      memory[rd] = memory[rn] | immediate;
   }
   else if(operation == "subis"){
      memory[rd] = memory[rn] - immediate;
      //TODO: Set Flags
   }
   printf("%d\n", memory[rd]);
}

void execute_r_format(__uint32_t code, char* operation){
   int rd = code & 0x1F;
   int rn = (code >> 5) & 0x1F;
   int shamt = (code >> 10) & 0x3F;
   int rm = (code >> 16) & 0x1F;
   //TODO implement shamt
   printf("%d ", memory[rd]);
   if(operation = "add"){
      memory[rd] = memory[rn] + memory[rm];
   }
   else if(operation = "sub"){
      memory[rd] = memory[rn] - memory[rm];
   }
   else if(operation = "and"){
      memory[rd] = memory[rn] & memory[rm];
   }
   else if(operation = "subs"){
      memory[rd] = memory[rn] - memory[rm];
      //TODO: set flags
   }
   else if(operation = "lsl"){
      memory[rd] = memory[rn] << shamt;
   }
   else if(operation = "lsr"){
      memory[rd] = memory[rn] >> shamt;
   }
   printf("%d\n", memory[rd]);
}