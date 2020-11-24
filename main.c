#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/resource.h>
#include <signal.h>
#include <stdlib.h>
#include <fcntl.h>


#define MAX_LENGTH 10 //ten arguments on one line
#define MAX_CHAR 50   //50 letters in one line
#define DELIMETER " " //defines my delimeter


int fg_flag = 0;


 char *log_file = "logFile.txt";
 FILE * fptr;

void read_line(char line[]) {

    char* returnValue = fgets(line, MAX_CHAR, stdin); //reads from console

int j = 0;
    int i = 0;                //removing the new line character
    while (line[i] != '\n')
        i++;
    line[i] = '\0';

   if (strcasecmp(line,"exit")==0 || returnValue == NULL)
      exit(0);

    while(line[j] !='\0'){         //checks for foreground token
        if(line[j]== '&'){
            fg_flag = 1;
            line[j]='\0';
           }
        j++;
    }


}

int parse(char* str_tmp[],char line[]){


    int i = 0;
    str_tmp[i]=strtok(line,DELIMETER);

    if(str_tmp[i]==NULL) {
        printf("No Command Written\n");
        return 1;
    }


    while(str_tmp[i]!= NULL){
        str_tmp[++i]=strtok(NULL,DELIMETER);
    }


    return 1;

}





int parse_input(char * str_args[], char line[]){  //returns true if it was able to read and parse line


    read_line(line);  //reads input and stores it in array of characters

    parse(str_args,line); //read array of characters and tokenizes it




    return 1;
}
void signalHandler(int signal)
{

    if (signal==SIGCHLD) {
        fptr = fopen(log_file, "a");
        fputs("Child Process was Terminated\n", fptr);
        printf("child terminated \n");
       // wait(NULL);
    }

    }





int main() {

    signal(SIGCHLD,signalHandler);

   char * str_args[MAX_LENGTH];
   char line[MAX_CHAR];

   while(parse_input(str_args,line)) {
       pid_t child_pid = fork();

       if (fg_flag == 1 && child_pid != 0)
           execvp(str_args[0], str_args); //foreground priority


       else {
           if (child_pid == 0) {  //checks to see if currently in parent or child process




               execvp(str_args[0], str_args);
           } else {


               wait(child_pid);


           }

       }
       fg_flag = 0;

   }


    return 0;
}

