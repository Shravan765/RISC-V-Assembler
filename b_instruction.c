#include <stdio.h>
#include<stdlib.h>
#include<string.h>


/*
Status can be 0 (failure) , 1 (success), 2 or 3 (additional checking, corner cases)
*/
struct parsed_b_command{
    short status;
    char* instr;
    short rs1;
    short rs2;
    short imm;
    char* label;
    short line_number;
};

struct parsed_b_command b_check_register(int instr_space_index, char* command, char* inst){
    char *reg_convention_names_i[] = {"zero", "ra", "sp", "gp", "tp", "t0", "t1", "t2", "s0", "s1", "a0", "a1", "a2", "a3", "a4", "a5", "a6", "a7", "s2", "s3", "s4", "s5", "s6", "s7", "s8", "s9", "s10", "s11", "t3", "t4", "t5", "t6"};
    char *reg_number_names_i[] = {"x0", "x1", "x2", "x3", "x4", "x5", "x6", "x7", "x8", "x9", "x10", "x11", "x12", "x13", "x14", "x15", "x16", "x17", "x18", "x19", "x20", "x21", "x22", "x23", "x24", "x25", "x26", "x27", "x28", "x29", "x30", "x31"};
    
    struct parsed_b_command b_command;
    b_command.instr = malloc(sizeof(char)*(strlen(inst)+1));
    strcpy(b_command.instr, inst);
    b_command.instr[strlen(inst)] = '\0';

    b_command.status =0;

    int command_counter = instr_space_index+1;
    char reg1_temp[5];
    int reg1_size = 0;
    while(command[command_counter] != ','){
        reg1_temp[reg1_size++] = command[command_counter++];
        if (reg1_size>3){
            
            reg1_temp[4] = '\0';
            if (strcmp(reg1_temp, reg_convention_names_i[0]) == 0){
                //puts(reg1_temp);
                if(command[command_counter] != ','){
                    return b_command;
                }
            }
            else{
                return b_command;
            }
        }
    }
    
    char reg1[reg1_size+1];
    strncpy(reg1,reg1_temp,reg1_size);
    reg1[reg1_size] = '\0';
    int found = 0;
    for (int i = 0; i<32; i++){
        if (strcmp(reg1, reg_convention_names_i[i]) == 0){
            found = 1;
            b_command.rs1 = i;
            break;
        }
        else if(strcmp(reg1, reg_number_names_i[i]) == 0){
            found = 1;
            b_command.rs1 = i;
            break;
        }
    }
    if (found == 0){
        if (strcmp(reg1, "fp") == 0){
            b_command.rs1 = 8;
        }
        else{
            return b_command;
        }
    }
    if (command[++command_counter] != ' '){
        return b_command;
    }

    command_counter++;
    
    char reg2_temp[5];
    int reg2_size = 0;
    //printf("%c\n",command[command_counter]);
    while(command[command_counter] != ','){
        //printf("%d\n",command[command_counter]);
        reg2_temp[reg2_size++] = command[command_counter++];
        if (reg2_size>3){
            //puts(reg2_temp);
            reg2_temp[4] = '\0';
            if (strcmp(reg2_temp, reg_convention_names_i[0]) == 0){
                //puts(reg1_temp);
                if(command[command_counter] != ','){
                    return b_command;;
                }
            }
            else{
                //puts("here");
                return b_command;
            }
        }
    }
    //puts(reg3_temp);
    char reg2[reg2_size+1];
    strncpy(reg2,reg2_temp,reg2_size);
    reg2[reg2_size] = '\0';
    //puts(reg3);
    found = 0;
    for (int i = 0; i<32; i++){
        if (strcmp(reg2, reg_convention_names_i[i]) == 0){
            b_command.rs2 = i;
            found = 1;
            break;
        }
        else if(strcmp(reg2, reg_number_names_i[i]) == 0){
            found = 1;
            b_command.rs2 = i;
            break;
        }
    }
    if (found == 0){
        if (strcmp(reg2, "fp") == 0){
            b_command.rs2 = 8;
        }
        else{
            return b_command;
        }
    }

    if (command[++command_counter] != ' '){
        return b_command;
    }

    command_counter++;

    //parsing label

    char label_temp[16];
    int label_length = 0;
    while(command[command_counter] != ' ' && command[command_counter] != '\0' && command[command_counter] != '\n' && command[command_counter] != ';'){
        label_temp[label_length++] = command[command_counter++];
    }
    label_temp[label_length] = '\0';
    char label[label_length+1];
    strncpy(label, label_temp,label_length+1);
    //puts(label);
    b_command.label = (char*)malloc(sizeof(char)*(label_length+1));
    strcpy(b_command.label, label);
    //puts(b_command.label);
    //printf("%d\n", command[command_counter]);
    
    //printf("%d ",command[command_counter]);
    while (command[command_counter] != '\n'){
        //printf("%d ",command[command_counter]);
        if (command[command_counter] != ' '){
            if (command[command_counter] == '\0'){
                
                b_command.status = 2;
                
                return b_command;
            }
            else if (command[command_counter] == ';'){
                //puts("comment b");
                b_command.status = 3;
                return b_command;
            }
            else if (command[command_counter] == EOF){
                b_command.status = 1;
                return b_command;
            }
            else{
                //printf("%d\n",command[command_counter]);
                return b_command;
            }   
        }
        command_counter++;
    }

    b_command.status = 1;
    return b_command;
}

void b_command_binary_convert(struct parsed_b_command b_command, char bin[33]){
    
    char*opcode = "1100011";
    //adding opcode
    for (int i = 25; i<32;i++){
        bin[i] = opcode[i-25];
        //printf("%c ", bin[i]);
    }
    //printf("%d %s\n", 1, bin);
    //adding imm[4:1|11], imm[12|10:5]
    
    short imm = b_command.imm;
    //printf("%d\n",imm);
    imm>>=1;//discarding [0]
    for(int i = 23; i>19;i--){
        bin[i] = imm&1 ? '1' : '0';
        imm>>=1;
        //printf("%c ", bin[i]);
    }
    for(int i = 6; i>0;i--){
        bin[i] = imm&1 ? '1' : '0';
        imm>>=1;
        //printf("%c ", bin[i]);
    }
    //[1:10] done
    bin[24] = imm&1 ? '1' : '0'; //[11]
    bin[0] = imm&1 ? '1' : '0' ; //[12]
    
    struct cmd_f3{
        char* cmd;
        char* funct3_bin;
    };
    struct cmd_f3 beq = {.cmd = "beq" , .funct3_bin = "000"};
    struct cmd_f3 bne = {.cmd = "bne" , .funct3_bin = "001"};
    struct cmd_f3 blt = {.cmd = "blt" , .funct3_bin = "100"};
    struct cmd_f3 bge = {.cmd = "bge" , .funct3_bin = "101"};
    struct cmd_f3 bltu = {.cmd = "bltu" , .funct3_bin = "110"};
    struct cmd_f3 bgeu = {.cmd = "bgeu" , .funct3_bin = "111"};

    struct cmd_f3 array [6] = {beq, bne, blt, bge, bltu, bgeu};

    //finding funct3
    char* funct3;
    for (int i = 0; i<6;i++){
        if (strcmp(b_command.instr ,array[i].cmd) == 0){
            funct3 = array[i].funct3_bin;
            break;
        }
    }
    //adding funct3
    for (int i = 17; i<20; i++){
        bin[i] = funct3[i-17];
        //printf("%c ", bin[i]);
    }
    //adding rs1 & rs2;
    int rs1 = b_command.rs1;
    for (int i = 16; i>11;i--){
        bin[i] = rs1&1 ? '1' : '0';
        rs1>>=1;
        //printf("%c ", bin[i]);
    }
    int rs2 = b_command.rs2;
    //printf("%d\n", rs2);
    for (int i = 11; i>6;i--){
        bin[i] = rs2&1 ? '1' : '0';
        rs2>>=1;
        //printf("%c ", bin[i]);
    }

    //puts(bin);
    
}
