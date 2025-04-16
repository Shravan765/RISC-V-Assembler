#include <stdio.h>
#include<stdlib.h>
#include<string.h>


/*
Status can be 0 (failure) , 1 (success), 2 or 3 (additional checking, corner cases)
*/
struct parsed_i_command{
    short status;
    char* instr;
    short rd;
    short rs1;
    short imm;
};

struct parsed_i_command i_check_register(int instr_space_index, char* command, char* inst){
    char *reg_convention_names_i[] = {"zero", "ra", "sp", "gp", "tp", "t0", "t1", "t2", "s0", "s1", "a0", "a1", "a2", "a3", "a4", "a5", "a6", "a7", "s2", "s3", "s4", "s5", "s6", "s7", "s8", "s9", "s10", "s11", "t3", "t4", "t5", "t6"};
    char *reg_number_names_i[] = {"x0", "x1", "x2", "x3", "x4", "x5", "x6", "x7", "x8", "x9", "x10", "x11", "x12", "x13", "x14", "x15", "x16", "x17", "x18", "x19", "x20", "x21", "x22", "x23", "x24", "x25", "x26", "x27", "x28", "x29", "x30", "x31"};

    struct parsed_i_command i_command;
    i_command.status =0;
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
                    return i_command;
                }
            }
            else{
                return i_command;
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
            i_command.rd = i;
            break;
        }
        else if(strcmp(reg1, reg_number_names_i[i]) == 0){
            found = 1;
            i_command.rd = i;
            break;
        }
    }
    if (found == 0){
        if (strcmp(reg1, "fp") == 0){
            i_command.rd = 8;
        }
        else{
            return i_command;
        }
    }
    if (command[++command_counter] != ' '){
        return i_command;
    }
    command_counter++;
    char reg2_temp[5];
    int reg2_size = 0;
    while(command[command_counter] != ','){
        reg2_temp[reg2_size++] = command[command_counter++];
        if (reg2_size>3){
            
            reg2_temp[4] = '\0';
            if (strcmp(reg2_temp, reg_convention_names_i[0]) == 0){
                if(command[command_counter] != ','){
                    return i_command;
                }
            }
            else{
                return i_command;
            }
        }
    }
    
    char reg2[reg2_size+1];
    strncpy(reg2,reg2_temp,reg2_size);
    reg2[reg2_size] = '\0';
    found = 0;
    for (int i = 0; i<32; i++){
        if (strcmp(reg2, reg_convention_names_i[i]) == 0){
            i_command.rs1 = i;
            found = 1;
            break;
        }
        else if(strcmp(reg2, reg_number_names_i[i]) == 0){            
            i_command.rs1 = i;
            found = 1;
            break;
        }
    }
    if (found == 0){
        if (strcmp(reg2, "fp") == 0){
            i_command.rs1 = 8;
        }
        else{
                    return i_command;
        }
    }
    //puts("here");
    if (command[++command_counter] != ' '){
        return i_command;
    }
    command_counter++;
    //printf("%c\n", command[command_counter]);
    char num[6] = "";
    int i = 0;
    if (command[command_counter] == '+' || command[command_counter] == '-' ){ //handling + and -
        num[i] = command[command_counter];
        i++;
        command_counter++;
    }
    //printf("%c\n", command[command_counter]);
    while(command[command_counter] != ' ' && command[command_counter] != '\0' && command[command_counter] != '\n' && command[command_counter] != ';'){
        //printf("%c\n", command[command_counter]);
        if (command[command_counter] -'0' >9 || command[command_counter] -'0' <0 ){
            //printf("%d\n", command[command_counter]);
            return i_command;
        }
        num[i] = command[command_counter];
       if(i >4){
            return i_command;
        }
        i++;
        command_counter++;
    }
    //puts(num);
    if (i == 0){
        return i_command; //no number
    }
    short val = strtol(num, NULL, 10);
    //printf("%d\n", val);
    //checking the bounds of immediate
    if (inst[0] == 's' ){
        if (val<0 || val>63){
            return i_command;
        }
    }
    else{
        if (val < -2048 || val>2047){
            return i_command;
        }
    }
    i_command.imm = val;

    //printf("%d\n", command[command_counter]);
    i_command.instr = malloc(sizeof(char)*(strlen(inst)+1));
    strcpy(i_command.instr, inst);
    i_command.instr[strlen(inst)] = '\0';
    //printf("%d ",command[command_counter]);
    while (command[command_counter] != '\n'){
        //printf("%d ",command[command_counter]);
        if (command[command_counter] != ' '){
            if (command[command_counter] == '\0'){
                i_command.status = 2;
                return i_command;
            }
            else if (command[command_counter] == ';'){
                i_command.status = 3;
                return i_command;
            }
            else if (command[command_counter] == EOF){
                i_command.status = 1;
                return i_command;
            }
            else{
                //printf("%d\n",command[command_counter]);
                return i_command;
            }   
        }
        command_counter++;
    }

    i_command.status = 1;
    return i_command;
}

void i_command_binary_convert(struct parsed_i_command i_command, char bin[33]){
    
    char*opcode = "0010011";
    //adding opcode
    for (int i = 25; i<32;i++){
        bin[i] = opcode[i-25];
        //printf("%c ", bin[i]);
    }
    //printf("%d %s\n", 1, bin);
    //adding rd
    int rd = i_command.rd;
    for (int i = 24; i>19;i--){
        bin[i] = rd&1 ? '1' : '0';
        //printf("%c ", bin[i]);
        rd>>=1;
    } 
    
    struct cmd_f3{
        char* cmd;
        char* funct3_bin;
    };
    struct cmd_f3 addi = {.cmd = "addi" , .funct3_bin = "000"};
    struct cmd_f3 xori = {.cmd = "xori" , .funct3_bin = "100"};
    struct cmd_f3 ori = {.cmd = "ori" , .funct3_bin = "110"};
    struct cmd_f3 andi = {.cmd = "andi" , .funct3_bin = "111"};
    struct cmd_f3 slli = {.cmd = "slli" , .funct3_bin = "001"};
    struct cmd_f3 srli = {.cmd = "srli" , .funct3_bin = "101"};
    struct cmd_f3 srai = {.cmd = "srai" , .funct3_bin = "101"};

    struct cmd_f3 array [7] = {addi, xori, ori, andi, slli, srli, srai};

    //finding funct3
    char* funct3;
    for (int i = 0; i<7;i++){
        if (strcmp(i_command.instr ,array[i].cmd) == 0){
            funct3 = array[i].funct3_bin;
            break;
        }
    }
    //adding funct3
    for (int i = 17; i<20; i++){
        bin[i] = funct3[i-17];
        //printf("%c ", bin[i]);
    }
    //adding rs1;
    int rs1 = i_command.rs1;
    for (int i = 16; i>11;i--){
        bin[i] = rs1&1 ? '1' : '0';
        rs1>>=1;
        //printf("%c ", bin[i]);
    }
    //adding imm
    short imm = i_command.imm;
    if (strcmp(i_command.instr , "srai") == 0){
        //handlins srai's 0x10 ie 010000
        for(int i = 11; i>5;i--){
            bin[i] = imm&1 ? '1' : '0';
            imm>>=1;
        //printf("%c ", bin[i]);
        }
        char* srai_6_11 = "000010" ;
        for (int i = 5; i>-1; i-- ){
            bin[i] = srai_6_11[5-i];
        }
    }
    else{
        for(int i = 11; i>-1;i--){
            bin[i] = imm&1 ? '1' : '0';
            imm>>=1;
        }
    }
    //puts(bin);
    
}