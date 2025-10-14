#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>

int main(){
    char user_command_string[50]; //user input for command
    int state = 1;
    FILE* opener;

    while (state == 1)
    {
        printf("Enter from one of the following: \nhelp\nnew\nedit\ndisplay\ndelete\nexit\n");
        scanf("%49s",user_command_string);

        if (strcmp(user_command_string, "help")==0){
            printf("You selected help.\n");
            printf("Available commands:\n");
            printf("  help    - Show this help message\n");
            printf("  new     - Create a new file (a.txt)\n");
            printf("  edit    - Overwrite contents of a.txt\n");
            printf("  display - Show contents of a.txt\n");
            printf("  delete  - Delete a.txt\n");
            printf("  exit    - Exit the program\n");
        }
        else if (strcmp(user_command_string, "new")==0){
            printf("You selected new.\n");
            opener = fopen("a.txt", "w");    
            if (opener == NULL){
                perror("Error opening file");    //Error handling
            }
            else{
                printf("File 'a.txt has been created.\n");
                fclose(opener);
                printf("File has been closed.\n");
            }
        } 
        else if (strcmp(user_command_string, "edit")==0){
            printf("You selected edit.\n"); 
            printf("Editing 'a.txt'...\n");

            opener = fopen("a.txt", "w");  
            if (opener == NULL) {
                perror("Error opening file");
            } 
            else {
                char text[200];
                printf("Enter new content (one line only): ");
                getchar(); // clear leftover newline from scanf
                fgets(text, sizeof(text), stdin); // read a full line (can include spaces)

                fprintf(opener, "%s", text);
                fclose(opener);
                printf("File updated successfully.\n");
            }   
        }
        else if (strcmp(user_command_string, "display")==0){
            printf("You selected display.\n"); 
            opener = fopen("a.txt", "r");  //using "r" mode for reading
            if (opener == NULL) {
                perror("Error opening file");
            } 
            else {
                printf("--- Contents of 'a.txt' ---\n");
                char ch;
                while ((ch = fgetc(opener)) != EOF) {   //detecting "End-Of-File character
                    putchar(ch);
                }
                printf("--- End of File ---\n");
                fclose(opener); 
            }
        }  
        else if (strcmp(user_command_string, "delete")==0){
            printf("You selected delete.\n"); 
            if (remove("a.txt") == 0) {
                printf("File 'a.txt' deleted successfully.\n");
            } else {
                perror("Error deleting file");
            }   
        } 
        else if (strcmp(user_command_string, "exit")==0){
            printf("Exiting myshell. Goodbye!\n");
            state = 0;    
        } 
    }
    return 0;
}