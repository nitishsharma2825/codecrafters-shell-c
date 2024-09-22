#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main() {

  char *built_in_commands[] = {"echo", "type", "exit"};
  int numOfCommands = sizeof(built_in_commands) / sizeof(built_in_commands[0]);

  while (1)
  {

    printf("$ ");
    fflush(stdout);

    // Wait for user input
    char input[100];
    fgets(input, 100, stdin);

    input[strlen(input) - 1] = '\0';
    char *command = strdup(input);
    char *commandType = strtok(command, " ");

    if (strcmp(input, "exit 0") == 0) {

      exit(0);
    
    } else if (strcmp(commandType, "echo") == 0) {

      printf("%s\n", input+5);

    } else if (strcmp(commandType, "type") == 0) {

      char *commandType = strtok(NULL, " ");
      int found = 0;

      for (size_t i = 0; i < numOfCommands; i++) {
        if (strcmp(commandType, built_in_commands[i]) == 0) {
          found = 1;
          printf("%s is a shell builtin\n", commandType);
          break;
        }
      }
      
      char *path = getenv("PATH");
      char *path_copy = strdup(path);

      char *dir = strtok(path_copy, ":");
      char full_path[1024];
      while (dir!=NULL) {
        if (found == 1)
        {
          break;
        }
        
        sprintf(full_path, "%s/%s", dir, commandType);      
        if (access(full_path, X_OK) == 0 )
        {
          found = 1;
          printf("%s is %s\n", commandType, full_path);
          break;
        }
        dir = strtok(NULL, ":");
      }

      if (!found) {
          printf("%s: not found\n", commandType);
      }

    } else {

      printf("%s: command not found\n", input);

    }

    fflush(stdout);

  }

  return 0;
}
