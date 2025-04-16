#include <stdio.h>
#include<stdlib.h>
#include<string.h>


/*
Status can be 0 (failure) , 1 (success), 2 or 3 (additional checking, corner cases)
*/
struct parsed_j_command{
    short status;
    char* instr;
    short rd;
    int imm;
    char* label;
    short line_number;
};

struct parsed_j_command j_check_register(int instr_space_index, char* command, char* inst){
    char *reg_convention_names_i[] = {"zero", "ra", "sp", "gp", "tp", "t0", "t1", "t2", "s0", "s1", "a0", "a1", "a2", "a3", "a4", "a5", "a6", "a7", "s2", "s3", "s4", "s5", "s6", "s7", "s8", "s9", "s10", "s11", "t3", "t4", "t5", "t6"};
    char *reg_number_names_i[] = {"x0", "x1", "x2", "x3", "x4", "x5", "x6", "x7", "x8", "x9", "x10", "x11", "x12", "x13", "x14", "x15", "x16", "x17", "x18", "x19", "x20", "x21", "x22", "x23", "x24", "x25", "x26", "x27", "x28", "x29", "x30", "x31"};
    struct parsed_j_command j_command;
    
    j_command.instr = malloc(sizeof(char)*(strlen(inst)+1));
    strcpy(j_command.instr, inst);
    j_command.instr[strlen(inst)] = '\0';

    j_command.status =0;
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
                    return j_command;
                }
            }
            else{
                return j_command;
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
            j_command.rd = i;
            break;
        }
        else if(strcmp(reg1, reg_number_names_i[i]) == 0){
            found = 1;
            j_command.rd = i;
            break;
        }
    }
    if (found == 0){
        if (strcmp(reg1, "fp") == 0){
            j_command.rd = 8;
        }
        else{
            return j_command;
        }
    }
    if (command[++command_counter] != ' '){
        return j_command;
    }
    command_counter++;
    char label_temp[16];
    int label_length = 0;
    while(command[command_counter] != ' ' && command[command_counter] != '\0' && command[command_counter] != '\n' && command[command_counter] != ';'){
        label_temp[label_length++] = command[command_counter++];
    }
    label_temp[label_length] = '\0';
    char label[label_length+1];
    strncpy(label, label_temp,label_length+1);
    //puts(label);
    j_command.label = (char*)malloc(sizeof(char)*(label_length+1));
    strcpy(j_command.label, label);
    //puts(b_command.label);
    //printf("%d\n", command[command_counter]);
    
    //printf("%d ",command[command_counter]);
    while (command[command_counter] != '\n'){
        //printf("%d ",command[command_counter]);
        if (command[command_counter] != ' '){
            if (command[command_counter] == '\0'){
                
                j_command.status = 2;
                
                return j_command;
            }
            else if (command[command_counter] == ';'){
                //puts("comment b");
                j_command.status = 3;
                return j_command;
            }
            else if (command[command_counter] == EOF){
                j_command.status = 1;
                return j_command;
            }
            else{
                //printf("%d\n",command[command_counter]);
                return j_command;
            }   
        }
        command_counter++;
    }

    j_command.status = 1;
    return j_command;
}

void j_command_binary_convert(struct parsed_j_command j_command, char bin[33]){
    
    char*opcode = "1101111";
    //adding opcode
    for (int i = 25; i<32;i++){
        bin[i] = opcode[i-25];
        //printf("%c ", bin[i]);
    }
    int rd = j_command.rd;
    //printf("%d\n", rs2);
    for (int i = 24; i>19;i--){
        bin[i] = rd&1 ? '1' : '0';
        rd>>=1;
        //printf("%c ", bin[i]);
    }
    //printf("%d %s\n", 1, bin);
    //adding imm[4:0]
    short imm = j_command.imm;
    imm>>=1;
    for(int i = 10; i>0;i--){
        bin[i] = imm&1 ? '1' : '0';
        imm>>=1;
        //printf("%c ", bin[i]);
    }
    bin[11] =  imm&1 ? '1' : '0';
    imm>>=1;
    for(int i = 19; i>11;i--){
        bin[i] = imm&1 ? '1' : '0';
        imm>>=1;
        //printf("%c ", bin[i]);
    }
    bin[0] =  imm&1 ? '1' : '0';


    //puts(bin);
    
}