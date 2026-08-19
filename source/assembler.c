#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include "r_instruction.c"
#include "i_instruction.c"
#include "load_instruction.c"
#include "s_instruction.c"
#include "b_instruction.c"
#include "jal_instruction.c"
#include "jalr_instruction.c"
#include "u_instruction.c"
/*
Returns the group of instructions as follows
r -> r type,  i -> i type,  s-> s type, b-> b type,  j-> j type  , u -> u type
n if none of the above types
*/
char group_of_inst(char*text){
    char*r_list[] = {"add", "sub", "and", "or", "xor", "sll", "srl", "sra"};
    char*i_list[] = {"addi",  "andi", "ori", "xori", "slli", "srli", "srai"};
    char*i_l_list[] = {"ld", "lw", "lh", "lb", "lwu", "lhu", "lbu"};
    char*s_list[] = { "sd", "sw", "sh", "sb"};
    char*b_list[] = {"beq", "bne", "blt", "bge", "bltu", "bgeu"};
    char*j_inst = "jal";
    char*jalr = "jalr";
    char*u_inst = "lui";

    if (strcmp(text, j_inst) == 0){
        return 'j';
    }
    else if(strcmp(text, jalr) == 0){
        return 'J';
    }
    else if(strcmp(text, u_inst) == 0){
        return 'u';
    }
    else{
        for (int i = 0; i< sizeof(r_list)/sizeof(r_list[0]); i++){
            if (strcmp(text, r_list[i]) == 0){
                return 'r';
            }
        }
        for (int i = 0; i< sizeof(i_list)/sizeof(i_list[0]); i++){
            if (strcmp(text, i_list[i]) == 0){
                return 'i';
            }
        }
        for (int i = 0; i< sizeof(i_l_list)/sizeof(i_l_list[0]); i++){
            if (strcmp(text, i_l_list[i]) == 0){
                return 'l';
            }
        }
        for (int i = 0; i< sizeof(s_list)/sizeof(s_list[0]); i++){
            if (strcmp(text, s_list[i]) == 0){
                return 's';
            }
        }
        for (int i = 0; i< sizeof(b_list)/sizeof(b_list[0]); i++){
            if (strcmp(text, b_list[i]) == 0){
                return 'b';
            }
        }
    }
    return 'n'; 

}

void hex_writer(char*binary_command[], int size){

    for (int i = 0; i<size; i++){
        //puts(binary_command[i]);
    }
    

    char* binary_4bits[] = {"0000", "0001", "0010", "0011", "0100", "0101", "0110", "0111", "1000", "1001", "1010", "1011", "1100", "1101", "1110", "1111"};
    char hex_digits[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};
    char hex_command[9] = "";
    FILE *fptr;
    fptr = fopen("output.hex", "w");
    for (int i = 0; i<size;i++){
        char hex_command[9] = "";
        char *bin = binary_command[i];
        //puts(bin);
        //converting binary to hex
        char temp[5] ;
        for (int j = 0; j<32; j++){
            //printf("%c ", bin[j]);
            temp[j%4] = bin[j];
            //puts(temp);
            if (j%4 == 3){
                
                temp[4] = '\0';
                for (int k = 0; k<16;k++){
                    if (strcmp(temp, binary_4bits[k]) == 0){
                        hex_command[j/4] = hex_digits[k];
                        break;
                    }
                }
            }

        }
        fputs(hex_command, fptr);
        if(i != size-1){
            fputs("\n", fptr);
        }
    }
    fclose(fptr);
}

struct label{
    char*str;
    int line_no;
};

int main(){
    FILE *fptr;
    fptr = fopen("input.s", "r");
    char command[50] = ""; //can take only 50 characters of each line
    const char delimitor = ',';
    
    //array of binary commands
    char**binary_cmd_array = malloc(sizeof(char*)); //dynamic allocation
    int binary_cmd_array_size = 0;
    struct label *label_array = (struct label*)malloc(sizeof(struct label));
    //puts("noyt");
    int label_number = 0;

    //beq/jalinstruction with label
    struct parsed_b_command *b_array = (struct parsed_b_command*)malloc(sizeof(struct parsed_b_command));
    int b_array_size = 0;
    struct parsed_j_command *j_array = (struct parsed_j_command*)malloc(sizeof(struct parsed_j_command));
    int j_array_size = 0;

    int line_number = 1;
    while (fgets(command, 49, fptr)){
        //printf("%d , %s \n", line_number, command);
        //printf("%c\n",command[0]);
        if(command[0] == '\n'){
            printf("Line number %d ", line_number);
            puts("Empty line detected!\n");
            line_number--;
        }
        else if(command[0] == ';'){
            int cmd_ctr = 0;
            for (int i = 0; i<50;i++){
                                if (command[i] == '\0'){
                                    //comment doesnt spill over
                                    cmd_ctr = i;
                                    break;
                                }
                            }
                            //printf("%d\n", cmd_ctr);
                            //getting file to start from next line number
                            if (cmd_ctr>=48){
                                char ch = fgetc(fptr);
                                while(ch != '\n' && ch != EOF){
                                    ch = fgetc(fptr);
                                }
                            }
            //comment
            line_number--;
        }
        else
        {
            char binary_rep [33] = ""; //Used at end

            int word1_end_index = 0;
            char word1[50] = "";
            int key_for_space = 0;
            int key_for_colon = 0;
            while(command[word1_end_index] != ' '  && command[word1_end_index] != '\0' && command[word1_end_index] != ':'){
                word1[word1_end_index] = command[word1_end_index];
                word1_end_index++;
                if(command[word1_end_index] == ' '){
                    key_for_space = 1;
                }
                else if(command[word1_end_index] == ':'){
                    key_for_colon = 1;
                } 
            }
            if(key_for_colon == 0 && key_for_space == 0){
                printf("Syntax error at line number %d !\n", line_number);
                printf(" => %s\n", command);
                puts("Terminating" );
                binary_cmd_array_size = -1;
                break;
            }


            else 
            {//instruction or label
                int cmd_end_index = 0;
                if(key_for_colon == 1){
                    //store label and go ahead
                    //puts("here");
                    struct label lb;
                    lb.str = (char *)malloc((word1_end_index + 1) * sizeof(char));
                    strncpy(lb.str, word1, word1_end_index);
                    lb.str[word1_end_index] = '\0';
                    lb.line_no = line_number;
                    label_array[label_number] = lb;
                    label_number++;
                    //puts("here1");
                    label_array = (struct label*)(realloc(label_array,sizeof(struct label)*(label_number+1))); 
                    //puts("here2"); 
                    //printf("%d Label  %s: \n", line_number, lb.str);
                    //puts(word1);
                    word1_end_index++;
                    if (command[word1_end_index] != ' '){
                        printf("Syntax error at line number %d !\n", line_number);
                        printf(" => %s\n", command);
                        puts("Terminating" );
                        binary_cmd_array_size = -1;
                        break;
                    }
                    word1_end_index++;
                    //once again finding command
                    memset(word1, 0, 50);
                    int cmd_found = 0;
                    while(command[word1_end_index] != ' '  && command[word1_end_index] != '\0'){
                                word1[cmd_end_index] = command[word1_end_index];
                                word1_end_index++;
                                cmd_end_index++;
                                if(command[word1_end_index] == ' '){
                                    cmd_found = 1;
                                } 
                    }
                    if (cmd_found == 0){
                        printf("Syntax error at line number %d !\n", line_number);
                        printf(" => %s\n", command);
                        puts("Terminating" );
                        binary_cmd_array_size = -1;
                        break;
                    }
                    /*
                    label: add 
                    cmd_end_index would be at 3 (add ) and word1_end_index at 10
                    */
                }
                else if (key_for_space == 1){
                    cmd_end_index = word1_end_index;
                }
                
                if (cmd_end_index > 4){
                        printf("Syntax error at line number %d !\n", line_number);
                        printf(" => %s\n", command);
                        puts("Terminating" );
                        binary_cmd_array_size = -1;
                        break;
                }
                    
                char inst[cmd_end_index] ;
                strncpy(inst, word1, cmd_end_index+1);
                inst[cmd_end_index] = '\0';
                
                //puts(inst);

                char type_of_inst = group_of_inst(inst);
                if (type_of_inst == 'n'){
                    printf("Syntax error at line number %d !\n", line_number);
                    printf(" => %s\n", command);
                    puts("Terminating" );
                    binary_cmd_array_size = -1;
                    break;
                }
                else if(type_of_inst == 'r')
                {
                    //puts("here");
                    struct parsed_r_command rcmd = r_check_register(word1_end_index,command);
                    //make a struct, check for more than 50 char stuff
                    
                    if (rcmd.status == 1){
                        rcmd.instr = inst;
                    }
                    else if (rcmd.status == 2){
                            //getting file to start from next line number
                            char ch = fgetc(fptr);
                            while(ch != '\n' && ch != EOF){
                                if(ch != ' '){
                                    if (ch == ';'){ //comment
                                        while(ch != '\n' && ch != EOF){
                                             ch = fgetc(fptr);
                                        }
                                        break;
                                    }
                                    printf("Unrecognised char found somewhere %C \n", ch);
                                    rcmd.status = 0;
                                    binary_cmd_array_size = -1;
                                    //printf("size %d", binary_cmd_array_size);
                                    break;
                                }
                                ch = fgetc(fptr);
                            }
                            if(rcmd.status != 0){
                                rcmd.instr = inst;
                            }
                    }
                    else if (rcmd.status == 3){
                            //puts("here");
                            int cmd_ctr = 0;
                            for (int i = 0; i<50;i++){
                                if (command[i] == '\0'){
                                    //comment doesnt spill over
                                    cmd_ctr = i;
                                    break;
                                }
                            }
                            //printf("%d\n", cmd_ctr);
                            //getting file to start from next line number
                            if (cmd_ctr>=48){
                                char ch = fgetc(fptr);
                                while(ch != '\n' && ch != EOF){
                                    ch = fgetc(fptr);
                                }
                            }
                            rcmd.instr = inst;
                    }
                    if(rcmd.status == 0){
                                    printf("Syntax error at line number %d !\n", line_number);
                                    printf("(first 50 characters) => %s\n", command);
                                    puts("Terminating" );
                                    binary_cmd_array_size = -1;
                                    break;
                    }
                    else{
                        r_command_binary_convert(rcmd, binary_rep);    
                    }
                }
                else if(type_of_inst == 'i')
                {
                    struct parsed_i_command icmd = i_check_register(word1_end_index,command,inst);
                    //printf("status %s %d %d %d %d\n",icmd.instr, icmd.status, icmd.rd, icmd.rs1, icmd.imm);
                    if (icmd.status == 1){
                    }
                    else if (icmd.status == 2){
                            //getting file to start from next line number
                            char ch = fgetc(fptr);
                            while(ch != '\n' && ch != EOF){
                                if(ch != ' '){
                                    if (ch == ';'){ //comment
                                        while(ch != '\n' && ch != EOF){
                                             ch = fgetc(fptr);
                                        }
                                        break;
                                    }
                                    printf("Unrecognised char found somewhere %C \n", ch);
                                    icmd.status = 0;
                                    binary_cmd_array_size = -1;
                                    //printf("size %d", binary_cmd_array_size);
                                    break;
                                }
                                ch = fgetc(fptr);
                            }
                            if(icmd.status != 0){
                                
                            }
                    }
                    else if (icmd.status == 3){
                            int cmd_ctr = 0;
                            for (int i = 0; i<50;i++){
                                if (command[i] == '\0'){
                                    //comment doesnt spill over
                                    cmd_ctr = i;
                                    break;
                                }
                            }
                            //printf("%d\n", cmd_ctr);
                            //getting file to start from next line number
                            if (cmd_ctr>=48){
                                char ch = fgetc(fptr);
                                while(ch != '\n' && ch != EOF){
                                    ch = fgetc(fptr);
                                }
                            }
                    }
                    if(icmd.status == 0){
                                    printf("Syntax error at line number %d !\n", line_number);
                                    printf("(first 50 characters) => %s\n", command);
                                    puts("Terminating" );
                                    binary_cmd_array_size = -1;
                                    break;
                    }
                    else{
                        //printf("status %s %d %d %d %d\n",icmd.instr, icmd.status, icmd.rd, icmd.rs1, icmd.imm);
                        i_command_binary_convert(icmd, binary_rep); 
                    }
                }
                else if (type_of_inst == 'l')
                {
                    struct parsed_l_command lcmd =  l_check_register(word1_end_index,command,inst);
                    //printf("status %s %d %d %d %d\n",lcmd.instr, lcmd.status, lcmd.rd, lcmd.rs1, lcmd.imm);
                    if (lcmd.status == 1){
                    }
                    else if (lcmd.status == 2){
                            //getting file to start from next line number
                            char ch = fgetc(fptr);
                            while(ch != '\n' && ch != EOF){
                                if(ch != ' '){
                                    if (ch == ';'){ //comment
                                        while(ch != '\n' && ch != EOF){
                                             ch = fgetc(fptr);
                                        }
                                        break;
                                    }
                                    printf("Unrecognised char found somewhere %C \n", ch);
                                    lcmd.status = 0;
                                    binary_cmd_array_size = -1;
                                    //printf("size %d", binary_cmd_array_size);
                                    break;
                                }
                                ch = fgetc(fptr);
                            }
                            if(lcmd.status != 0){
                                
                            }
                    }
                    else if (lcmd.status == 3){
                            int cmd_ctr = 0;
                            for (int i = 0; i<50;i++){
                                if (command[i] == '\0'){
                                    //comment doesnt spill over
                                    cmd_ctr = i;
                                    break;
                                }
                            }
                            //printf("%d\n", cmd_ctr);
                            //getting file to start from next line number
                            if (cmd_ctr>=48){
                                char ch = fgetc(fptr);
                                while(ch != '\n' && ch != EOF){
                                    ch = fgetc(fptr);
                                }
                            }
                    }
                    if(lcmd.status == 0){
                                    printf("Syntax error at line number %d !\n", line_number);
                                    printf("(first 50 characters) => %s\n", command);
                                    puts("Terminating" );
                                    binary_cmd_array_size = -1;
                                    break;
                    }
                    else{
                        //printf("status %s %d %d %d %d\n",icmd.instr, icmd.status, icmd.rd, icmd.rs1, icmd.imm);
                        l_command_binary_convert(lcmd, binary_rep); 
                    }

                }
                else if (type_of_inst == 's')
                {
                    struct parsed_s_command scmd =  s_check_register(word1_end_index,command,inst);
                    if (scmd.status == 1){
                    }
                    else if (scmd.status == 2){
                            //getting file to start from next line number
                            char ch = fgetc(fptr);
                            while(ch != '\n' && ch != EOF){
                                if(ch != ' '){
                                    if (ch == ';'){ //comment
                                        while(ch != '\n' && ch != EOF){
                                             ch = fgetc(fptr);
                                        }
                                        break;
                                    }
                                    printf("Unrecognised char found somewhere %C \n", ch);
                                    scmd.status = 0;
                                    binary_cmd_array_size = -1;
                                    //printf("size %d", binary_cmd_array_size);
                                    break;
                                }
                                ch = fgetc(fptr);
                            }
                            if(scmd.status != 0){
                                
                            }
                    }
                    else if (scmd.status == 3){
                            int cmd_ctr = 0;
                            for (int i = 0; i<50;i++){
                                if (command[i] == '\0'){
                                    //comment doesnt spill over
                                    cmd_ctr = i;
                                    break;
                                }
                            }
                            //printf("%d\n", cmd_ctr);
                            //getting file to start from next line number
                            if (cmd_ctr>=48){
                                char ch = fgetc(fptr);
                                while(ch != '\n' && ch != EOF){
                                    ch = fgetc(fptr);
                                }
                            }
                    }
                    if(scmd.status == 0){
                                    printf("Syntax error at line number %d !\n", line_number);
                                    printf("(first 50 characters) => %s\n", command);
                                    puts("Terminating" );
                                    binary_cmd_array_size = -1;
                                    break;
                    }
                    else{
                        //printf("status %s %d %d %d %d\n",scmd.instr, scmd.status, scmd.rs1, scmd.rs2, scmd.imm);
                        s_command_binary_convert(scmd, binary_rep); 
                        //puts(binary_rep);
                    }
                }
                else if(type_of_inst == 'b')
                {
                    struct parsed_b_command bcmd =  b_check_register(word1_end_index,command,inst);
                    if (bcmd.status == 1){
                    }
                    else if (bcmd.status == 2){
                            //getting file to start from next line number
                            char ch = fgetc(fptr);
                            while(ch != '\n' && ch != EOF){
                                if(ch != ' '){
                                    if (ch == ';'){ //comment
                                        while(ch != '\n' && ch != EOF){
                                             ch = fgetc(fptr);
                                        }
                                        break;
                                    }
                                    printf("Unrecognised char found somewhere %c\n", ch);
                                    bcmd.status = 0;
                                    binary_cmd_array_size = -1;
                                    //printf("size %d", binary_cmd_array_size);
                                    break;
                                }
                                ch = fgetc(fptr);
                            }
                            if(bcmd.status != 0){
                                
                            }
                    }
                    else if (bcmd.status == 3){
                            int cmd_ctr = 0;
                            for (int i = 0; i<50;i++){
                                if (command[i] == '\0'){
                                    //comment doesnt spill over
                                    cmd_ctr = i;
                                    break;
                                }
                            }
                            //printf("%d\n", cmd_ctr);
                            //getting file to start from next line number
                            if (cmd_ctr>=48){
                                char ch = fgetc(fptr);
                                while(ch != '\n' && ch != EOF){
                                    ch = fgetc(fptr);
                                }
                            }
                    }
                    if(bcmd.status == 0){
                                    printf("Syntax error at line number %d !\n", line_number);
                                    printf("(first 50 characters) => %s\n", command);
                                    puts("Terminating" );
                                    binary_cmd_array_size = -1;
                                    break;
                    }
                    else{
                        bcmd.line_number = line_number;
                        //printf("status %s %d %d %d %s %d\n",bcmd.instr, bcmd.status, bcmd.rs1, bcmd.rs2, bcmd.label, bcmd.line_number);
                        b_array[b_array_size] = bcmd;
                        b_array_size++;
                        //puts("here1");
                        b_array = (struct parsed_b_command*)(realloc(b_array,sizeof(struct parsed_b_command)*(b_array_size+1)));
                        binary_rep[0] = 'b';
                        //puts(binary_rep);
                    }
                }
                else if(type_of_inst == 'j')
                {
                    //puts("j");
                    struct parsed_j_command jcmd =  j_check_register(word1_end_index,command,inst);
                    if (jcmd.status == 1){
                    }
                    else if (jcmd.status == 2){
                            //getting file to start from next line number
                            char ch = fgetc(fptr);
                            while(ch != '\n' && ch != EOF){
                                if(ch != ' '){
                                    if (ch == ';'){ //comment
                                        while(ch != '\n' && ch != EOF){
                                             ch = fgetc(fptr);
                                        }
                                        break;
                                    }
                                    printf("Unrecognised char found somewhere %c\n", ch);
                                    jcmd.status = 0;
                                    binary_cmd_array_size = -1;
                                    //printf("size %d", binary_cmd_array_size);
                                    break;
                                }
                                ch = fgetc(fptr);
                            }
                            if(jcmd.status != 0){
                                
                            }
                    }
                    else if (jcmd.status == 3){
                            int cmd_ctr = 0;
                            for (int i = 0; i<50;i++){
                                if (command[i] == '\0'){
                                    //comment doesnt spill over
                                    cmd_ctr = i;
                                    break;
                                }
                            }
                            //printf("%d\n", cmd_ctr);
                            //getting file to start from next line number
                            if (cmd_ctr>=48){
                                char ch = fgetc(fptr);
                                while(ch != '\n' && ch != EOF){
                                    ch = fgetc(fptr);
                                }
                            }
                    }
                    if(jcmd.status == 0){
                                    printf("Syntax error at line number %d !\n", line_number);
                                    printf("(first 50 characters) => %s\n", command);
                                    puts("Terminating" );
                                    binary_cmd_array_size = -1;
                                    break;
                    }
                    else{
                        jcmd.line_number = line_number;
                        //printf("status %s %d %d %d %s %d\n",bcmd.instr, bcmd.status, bcmd.rs1, bcmd.rs2, bcmd.label, bcmd.line_number);
                        j_array[j_array_size] = jcmd;
                        j_array_size++;
                        //puts("here1");
                        j_array = (struct parsed_j_command*)(realloc(j_array,sizeof(struct parsed_j_command)*(j_array_size+1)));
                        binary_rep[0] = 'j';
                        //puts(binary_rep);
                    }
                }
                else if(type_of_inst == 'J')
                {
                    struct parsed_jalr_command jcmd = jalr_check_register(word1_end_index,command,inst);
                    if (jcmd.status == 1){
                    }
                    else if (jcmd.status == 2){
                            //getting file to start from next line number
                            char ch = fgetc(fptr);
                            while(ch != '\n' && ch != EOF){
                                if(ch != ' '){
                                    if (ch == ';'){ //comment
                                        while(ch != '\n' && ch != EOF){
                                             ch = fgetc(fptr);
                                        }
                                        break;
                                    }
                                    printf("Unrecognised char found somewhere %C \n", ch);
                                    jcmd.status = 0;
                                    binary_cmd_array_size = -1;
                                    //printf("size %d", binary_cmd_array_size);
                                    break;
                                }
                                ch = fgetc(fptr);
                            }
                            if(jcmd.status != 0){
                                
                            }
                    }
                    else if (jcmd.status == 3){
                            int cmd_ctr = 0;
                            for (int i = 0; i<50;i++){
                                if (command[i] == '\0'){
                                    //comment doesnt spill over
                                    cmd_ctr = i;
                                    break;
                                }
                            }
                            //printf("%d\n", cmd_ctr);
                            //getting file to start from next line number
                            if (cmd_ctr>=48){
                                char ch = fgetc(fptr);
                                while(ch != '\n' && ch != EOF){
                                    ch = fgetc(fptr);
                                }
                            }
                    }
                    if(jcmd.status == 0){
                                    printf("Syntax error at line number %d !\n", line_number);
                                    printf("(first 50 characters) => %s\n", command);
                                    puts("Terminating" );
                                    binary_cmd_array_size = -1;
                                    break;
                    }
                    else{
                        jalr_command_binary_convert(jcmd, binary_rep); 
                    }
                }
                else if (type_of_inst == 'u')
                {
                    struct parsed_u_command ucmd = u_check_register(word1_end_index,command,inst);
                    if (ucmd.status == 1){
                    }
                    else if (ucmd.status == 2){
                            //getting file to start from next line number
                            char ch = fgetc(fptr);
                            while(ch != '\n' && ch != EOF){
                                if(ch != ' '){
                                    if (ch == ';'){ //comment
                                        while(ch != '\n' && ch != EOF){
                                             ch = fgetc(fptr);
                                        }
                                        break;
                                    }
                                    printf("Unrecognised char found somewhere %C \n", ch);
                                    ucmd.status = 0;
                                    binary_cmd_array_size = -1;
                                    //printf("size %d", binary_cmd_array_size);
                                    break;
                                }
                                ch = fgetc(fptr);
                            }
                            if(ucmd.status != 0){
                                
                            }
                    }
                    else if (ucmd.status == 3){
                        //puts("h");
                            int cmd_ctr = 0;
                            for (int i = 0; i<50;i++){
                                if (command[i] == '\0'){
                                    //comment doesnt spill over
                                    cmd_ctr = i;
                                    break;
                                }
                            }
                            
                            //getting file to start from next line number
                            if (cmd_ctr>=48){
                                char ch = fgetc(fptr);
                                while(ch != '\n' && ch != EOF){
                                    ch = fgetc(fptr);
                                }
                            }
                    }
                    if(ucmd.status == 0){
                                    printf("Syntax error at line number %d !\n", line_number);
                                    printf("(first 50 characters) => %s\n", command);
                                    puts("Terminating" );
                                    binary_cmd_array_size = -1;
                                    break;
                    }
                    else{
                        u_command_binary_convert(ucmd, binary_rep); 
                    }
                }
            }
            /*binary_cmd_array[binary_cmd_array_size] = binary_rep;
                        The issue with this is that all elements point to the ADDRESS OFbinary_rep
                        so whenever binary_rep changes (as it will in next lines),
                        the previous lines get deleted 
                        SOLUTION : USE STRCPY
            */
            binary_cmd_array[binary_cmd_array_size] = malloc(33*sizeof(char));
            strcpy(binary_cmd_array[binary_cmd_array_size], binary_rep);
            binary_cmd_array_size++;
            binary_cmd_array = realloc(binary_cmd_array, (binary_cmd_array_size+1)*sizeof(char*));
        }
        //printf("%d\n", line_number);
        line_number++;
    }
    //printf("size %d", binary_cmd_array_size);
    
    if (binary_cmd_array_size != -1){
        for (int i = 0; i<label_number;i++){
                for (int j = i+1; j<label_number;j++){
                    if (strcmp(label_array[i].str, label_array[j].str) == 0){
                        puts("Syntax error : two labels with same name!");
                        printf("Label name => %s ; Line number %d & %d\n", label_array[i].str, label_array[i].line_no, label_array[j].line_no);
                        puts("Terminating" );
                        binary_cmd_array_size = -1;
                    }
                }
            }
        for (int i = 0; i<b_array_size;i++){
            int index = b_array[i].line_number - 1;
            //printf("%d b\n", index);
            char*label = b_array[i].label;
            short found = 0;
            for (int j = 0; j<label_number; j++){
                //printf("%s %s %d\n", label, label_array[j].str ,label_array[j].line_no);
                if (strcmp(label, label_array[j].str) == 0){
                    found = 1;
                    short offset = 4*(label_array[j].line_no - b_array[i].line_number);
                    b_array[i].imm = offset;
                    char binary_rep[33] = "";
                    b_command_binary_convert(b_array[i], binary_rep);
                    binary_cmd_array[index] = malloc(33*sizeof(char));
                    strcpy(binary_cmd_array[index], binary_rep);
                    break;
                }
            }
            if (found == 0){
                puts("Syntax error : labels in branch statement not found");
                printf("Line number => %d, Label name => %s/\n", b_array[i].line_number, b_array[i].label);
                puts("Terminating" );
                binary_cmd_array_size = -1;
            }
        }
        for (int i = 0; i<j_array_size;i++){
            int index = j_array[i].line_number - 1;
            //printf("%d j\n", index);
            char*label = j_array[i].label;
            short found = 0;
            for (int j = 0; j<label_number; j++){
                //printf("%s %s %d\n", label, label_array[j].str ,label_array[j].line_no);
                if (strcmp(label, label_array[j].str) == 0){
                    found = 1;
                    short offset = 4*(label_array[j].line_no - j_array[i].line_number);
                    j_array[i].imm = offset;
                    char binary_rep[33] = "";
                    j_command_binary_convert(j_array[i], binary_rep);
                    binary_cmd_array[index] = malloc(33*sizeof(char));
                    strcpy(binary_cmd_array[index], binary_rep);
                    break;
                }
            }
            if (found == 0){
                puts("Syntax error : labels in branch statement not found");
                printf("Line number => %d, Label name => %s/\n", j_array[i].line_number, j_array[i].label);
                puts("Terminating" );
                binary_cmd_array_size = -1;
            }
        }

    }
    if (binary_cmd_array_size != -1){
         //no errors found, can create hex file
        hex_writer(binary_cmd_array, binary_cmd_array_size);
    }
    fclose(fptr);
    for (int i = 0; i<binary_cmd_array_size;i++){
        free(binary_cmd_array[i]);
    }
    free(binary_cmd_array);


}
