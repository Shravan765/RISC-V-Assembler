#include <stdio.h>
#include<string.h>



/*
Status can be 0 (failure) , 1 (success), 2 or 3 (additional checking, corner cases)
*/
struct parsed_r_command{
    short  status;
    char* instr;
    short  rd;
    short  rs1;
    short  rs2;
};


/*
Returns 0 if any error
*/
struct parsed_r_command r_check_register(int instr_space_index, char* command){
    char *reg_convention_names_r[] = {"zero", "ra", "sp", "gp", "tp", "t0", "t1", "t2", "s0", "s1", "a0", "a1", "a2", "a3", "a4", "a5", "a6", "a7", "s2", "s3", "s4", "s5", "s6", "s7", "s8", "s9", "s10", "s11", "t3", "t4", "t5", "t6"};
    char *reg_number_names_r[] = {"x0", "x1", "x2", "x3", "x4", "x5", "x6", "x7", "x8", "x9", "x10", "x11", "x12", "x13", "x14", "x15", "x16", "x17", "x18", "x19", "x20", "x21", "x22", "x23", "x24", "x25", "x26", "x27", "x28", "x29", "x30", "x31"};

    struct parsed_r_command r_command;
    r_command.status =0;
    int command_counter = instr_space_index+1;
    char reg1_temp[5];
    //standard : sll x14, x31, x1
    int reg1_size = 0;
    while(command[command_counter] != ','){
        reg1_temp[reg1_size++] = command[command_counter++];
        if (reg1_size>3){
            
            reg1_temp[4] = '\0';
            if (strcmp(reg1_temp, reg_convention_names_r[0]) == 0){
                //puts(reg1_temp);
                if(command[command_counter] != ','){
                    return r_command;
                }
            }
            else{
                return r_command;
            }
        }
    }
    
    char reg1[reg1_size+1];
    strncpy(reg1,reg1_temp,reg1_size);
    reg1[reg1_size] = '\0';
    //puts(reg1);
    int found = 0;
    for (int i = 0; i<32; i++){
        if (strcmp(reg1, reg_convention_names_r[i]) == 0){
            found = 1;
            r_command.rd = i;
            break;
        }
        else if(strcmp(reg1, reg_number_names_r[i]) == 0){
            found = 1;
            r_command.rd = i;
            break;
        }
    }
    if (found == 0){
        if (strcmp(reg1, "fp") == 0){
            r_command.rd = 8;
        }
        else{
                    return r_command;
        }
    }
    //puts(reg1);
    //puts("here");
    if (command[++command_counter] != ' '){
        return r_command;
    }
    command_counter++;
    char reg2_temp[5];
    int reg2_size = 0;
    while(command[command_counter] != ','){
        reg2_temp[reg2_size++] = command[command_counter++];
        if (reg2_size>3){
            
            reg2_temp[4] = '\0';
            if (strcmp(reg2_temp, reg_convention_names_r[0]) == 0){
                //puts(reg1_temp);
                if(command[command_counter] != ','){
                    return r_command;
                }
            }
            else{
                return r_command;
            }
        }
    }
    
    char reg2[reg2_size+1];
    strncpy(reg2,reg2_temp,reg2_size);
    reg2[reg2_size] = '\0';
    //puts(reg2);
    found = 0;
    for (int i = 0; i<32; i++){
        if (strcmp(reg2, reg_convention_names_r[i]) == 0){
            r_command.rs1 = i;
            found = 1;
            break;
        }
        else if(strcmp(reg2, reg_number_names_r[i]) == 0){            
            r_command.rs1 = i;
            found = 1;
            break;
        }
    }
    if (found == 0){
        if (strcmp(reg2, "fp") == 0){
            r_command.rs1 = 8;
        }
        else{
                    return r_command;
        }
    }
    
    if (command[++command_counter] != ' '){
        
        return r_command;
    }
    //puts("here");
    command_counter++;
    char reg3_temp[5];
    int reg3_size = 0;
    //printf("%c\n",command[command_counter]);
    while(command[command_counter] != '\n' && command[command_counter] != ' ' && command[command_counter] != '\0' && command[command_counter] != ';'){
        //printf("%d\n",command[command_counter]);
        reg3_temp[reg3_size++] = command[command_counter++];
        //puts(reg3_temp);
        if (reg3_size>3){
            
            reg3_temp[4] = '\0';
            if (strcmp(reg3_temp, reg_convention_names_r[0]) == 0){
                //puts(reg1_temp);
                if(command[command_counter] != ' '){
                    return r_command;;
                }
            }
            else{
                //puts("here");
                return r_command;
            }
        }
    }
    //puts(reg3_temp);
    char reg3[reg3_size+1];
    strncpy(reg3,reg3_temp,reg3_size);
    reg3[reg3_size] = '\0';
    //puts(reg3);
    found = 0;
    for (int i = 0; i<32; i++){
        if (strcmp(reg3, reg_convention_names_r[i]) == 0){
            r_command.rs2 = i;
            found = 1;
            break;
        }
        else if(strcmp(reg3, reg_number_names_r[i]) == 0){
            found = 1;
            r_command.rs2 = i;
            break;
        }
    }
    if (found == 0){
        if (strcmp(reg3, "fp") == 0){
            r_command.rs2 = 8;
        }
        else{
            return r_command;
        }
    }

    /*
    checking after finding all three registers for unwanted char (or ; then we'll ignore)
    */ 
    //printf("%c\n",command[command_counter]);
    //printf("%d\n",command[command_counter]);
    while (command[command_counter] != '\n'){
        //printf("%c ",command[command_counter]);
        if (command[command_counter] != ' '){
            if (command[command_counter] == '\0'){
                r_command.status = 2;
                return r_command;
            }
            else if (command[command_counter] == ';'){
                r_command.status = 3;
                return r_command;
            }
            else if (command[command_counter] == EOF){
                r_command.status = 1;
                return r_command;
            }
            else{
                //printf("%d\n",command[command_counter]);
                return r_command;
            }   
        }
        command_counter++;
    }
    r_command.status = 1;
    return r_command;
}

//returns binary representation of the instruction
void r_command_binary_convert(struct parsed_r_command r_command, char bin[33]){
    
    char*opcode = "0110011";
    //adding opcode
    for (int i = 25; i<32;i++){
        bin[i] = opcode[i-25];
        //printf("%c ", bin[i]);
    }
    //printf("%d %s\n", 1, bin);
    //adding rd
    int rd = r_command.rd;
    for (int i = 24; i>19;i--){
        bin[i] = rd&1 ? '1' : '0';
        //printf("%c ", bin[i]);
        rd>>=1;
    } 
    
    struct cmd_f3_f7{
        char* cmd;
        char* funct3_bin;
        char* funct7_bin;
    };
    struct cmd_f3_f7 add = {.cmd = "add" , .funct3_bin = "000", .funct7_bin = "0000000"};
    struct cmd_f3_f7 sub = {.cmd = "sub" , .funct3_bin = "000", .funct7_bin = "0100000"};
    struct cmd_f3_f7 xor_ = {.cmd = "xor" , .funct3_bin = "100", .funct7_bin = "0000000"};
    struct cmd_f3_f7 or_ = {.cmd = "or" , .funct3_bin = "110", .funct7_bin = "0000000"};
    struct cmd_f3_f7 and_ = {.cmd = "and" , .funct3_bin = "111", .funct7_bin = "0000000"};
    struct cmd_f3_f7 sll = {.cmd = "sll" , .funct3_bin = "001", .funct7_bin = "0000000"};
    struct cmd_f3_f7 srl = {.cmd = "srl" , .funct3_bin = "101", .funct7_bin = "0000000"};
    struct cmd_f3_f7 sra = {.cmd = "sra" , .funct3_bin = "101", .funct7_bin = "0100000"};

    struct cmd_f3_f7 array [8] = {add, sub, xor_, or_, and_, sll, srl, sra};

    //finding funct3 and funct7
    char* funct3;
    char* funct7;
    for (int i = 0; i<8;i++){
        if (strcmp(r_command.instr ,array[i].cmd) == 0){
            funct3 = array[i].funct3_bin;
            funct7 = array[i].funct7_bin;
            break;
        }
    }
    //adding funct3
    for (int i = 17; i<20; i++){
        bin[i] = funct3[i-17];
        //printf("%c ", bin[i]);
    }
    //adding rs1 and rs2;
    int rs1 = r_command.rs1;
    for (int i = 16; i>11;i--){
        bin[i] = rs1&1 ? '1' : '0';
        rs1>>=1;
    }
    int rs2 = r_command.rs2;
    for (int i = 11; i>6;i--){
        bin[i] = rs2&1 ? '1' : '0';
        rs2>>=1;
    }
    //adding funct7
    for(int i = 0; i<7;i++){
        bin[i] = funct7[i];
    }
    //puts(bin);
    
}