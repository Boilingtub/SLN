#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<string.h>
#ifdef _WIN32
#include<windows.h>
#else
#include<unistd.h>
#endif

typedef struct UserData {
    char name[64];  
    char** custom;
} UserData;



void usr_input(char* input, int size) {
    fflush(stdout);
    putchar('>');
    if(fgets(input, size, stdin)) {}
}

void say(char* str, float delay) {
    int i = 0;
    while(str[i] != '\0') {
        putchar(str[i]);
        fflush(stdout);
        
        usleep((int)(delay*1000000));
        i++;
    }
}

bool YesNo() {
    say("Yes or No ?!\n",0.05);
    char input[2] = "\0";
    usr_input(input,sizeof(input));
    switch(input[0]) {
        case 'y': 
        case 'Y': return true;
            break;
        case 'n':
        case 'N': return false;
            break;
        default: return YesNo();
            break;
    }
}

void write_text_file(char* file_path, char* buffer) {
    FILE *file_pointer = fopen(file_path,"w");
    if(!file_pointer) {
        perror(file_path);
        return;
    }
    fprintf(file_pointer,"%s\n",buffer);
    fclose(file_pointer);
}

char* read_text_file(const char* file_path) {
    FILE *file_pointer;
    long File_SIZE = 0;
    char* buffer;

    file_pointer = fopen(file_path, "r");
    if(!file_pointer) {
        perror(file_path);
        exit(1);
    }
    fseek(file_pointer,0L,SEEK_END);
    File_SIZE = ftell(file_pointer)+1;
    rewind(file_pointer);

    buffer = malloc(File_SIZE);
    if(!buffer) {
        fclose(file_pointer);
        fputs("memory alloc failed",stderr);
        exit(1);
    }
    memset(buffer,0,File_SIZE);
    if(1!=fread(buffer, File_SIZE-1, 1 , file_pointer)) {
        fclose(file_pointer);
        free(buffer);
        fputs("entire read failed",stderr);
        exit(1);
    };
    fclose(file_pointer);
    return buffer;
}

void set_userdata_from_file(const char filepath[], UserData *userdata) {
    printf("getting user data form %s\n",filepath);
    if (access(filepath, F_OK) == 0) {        
        char* file_contents = read_text_file(filepath);

        strncpy(userdata->name, file_contents, sizeof(userdata->name));

        free(file_contents);
    } else {
        printf("%s not found!\n",filepath);
        userdata->name[0] = '\0';
    }
    return;
}

int introduce(UserData *userdata, char* memory_file, float say_delay) {
    char input[256];
    set_userdata_from_file(memory_file,userdata);

    if (strlen(userdata->name) == 0) {
        say("hello there !\nMay I ask your name?\n",say_delay);
        usr_input(input,sizeof(input));
        char usr_name[sizeof(char)*strlen(input)+1];
        strcpy(usr_name,input);

        usr_name[strlen(usr_name)-1] = '\0';
            
        write_text_file("memory.txt",usr_name);

        char to_say[40+strlen(usr_name)+1];
        strcpy(to_say,"Oh, it is so nice to meet you ");
        strcat(to_say,usr_name);
        strcat(to_say," !\n");
        say(to_say,say_delay);
    } else {
        char to_say[15+64];
        strcpy(to_say,"welcome back ");
        strncat(to_say, userdata->name,strlen( userdata->name));
        strcat(to_say, " !\n");
        say(to_say,say_delay);
    }
    return 0;
}

int print_word_list() {
  return 0;
}

int main() {
    #define SAYDELAY 0.025
    int state = 0;
    char input[256];
    //char* word_list[] = {"","exit","myname"};
    putchar('\n');
    putchar('\n');

    UserData userdata;
    introduce(&userdata, "memory.txt", SAYDELAY);
    

    if (strlen(userdata.name) == 0) {
        say("hello there !\nMay I ask your name?\n",SAYDELAY);
        usr_input(input,sizeof(input));
        char usr_name[sizeof(char)*strlen(input)+1];
        strcpy(usr_name,input);

        usr_name[strlen(usr_name)-1] = '\0';
            
        write_text_file("memory.txt",usr_name);

        char to_say[40+strlen(usr_name)+1];
        strcpy(to_say,"Oh, it is so nice to meet you ");
        strcat(to_say,usr_name);
        strcat(to_say," !\n");
        say(to_say,SAYDELAY);
    } else {
        char to_say[15+64];
        strcpy(to_say,"welcome back ");
        strncat(to_say, userdata.name,strlen(userdata.name));
        strcat(to_say, " !\n");
        say(to_say,SAYDELAY);
    }
    say("What would you like me to do ?\n",SAYDELAY);
    
    while (state != -1) {
        usr_input(input,sizeof(input));
        input[strlen(input)-1] = '\0';
        

        if(strcmp(input,"exit")==0) state = -1;
        if(strcmp(input,"")) state = 0;
        if(strcmp(input,"saymyname")==0) state = 1;

        switch(state) { 
            case -1 : {
                say("Do you really want to go ?\n",SAYDELAY);
                if(YesNo() == true) {
                    say("Ok then, bye, bye\n",SAYDELAY);
                    return 0;
                }
                else {
                    say("Phew , had me a bit worried\n",SAYDELAY);
                    state = 0;
                }
            }
            break;
            
            case 0: {
                say("I don't know that word\n",SAYDELAY);
                say("These are the words I do know\n",SAYDELAY);
                //print_word_list(word_list,SAYDELAY);
            }
            break;

            case 1: {
                printf("your name is: %s",userdata.name);
            }
            break;

            default: {
            
            }
            break;
        }
    }
}
