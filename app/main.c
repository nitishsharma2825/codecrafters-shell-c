#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

int is_executable(const char* path) {
  return access(path, X_OK) == 0;
}

char *find_in_path(char *commandType) {
  char *path = getenv("PATH");
  char *path_copy = strdup(path);

  char *dir = strtok(path_copy, ":");
  static char full_path[1024];
  while (dir!=NULL) {    
    sprintf(full_path, "%s/%s", dir, commandType);      
    if (is_executable(full_path))
    {
      return full_path;
    }
    dir = strtok(NULL, ":");
  }

  return NULL;
}

void execute_cmd(char **argv) {
  pid_t pid = fork();
  if (pid < 0)
  {
    fprintf(stderr, "fork failed\n");
    exit(1);
  }
  else if (pid == 0)
  {
    execv(argv[0], argv);
    fprintf(stderr, "exec failed!\n");
    exit(1);
  }
  else
  {
    int status;
    waitpid(pid, &status, 0);
  }
}


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

      if (found == 0)
      {
        char *path = find_in_path(commandType); 
        if (path)
        {
          printf("%s is %s\n", commandType, path);
        }
        else
        {
          printf("%s: not found\n", commandType);
        }
      }

    } else {
      char *argv[10];
      int argc = 0;
      
      while (commandType != NULL)
      {
        argv[argc++] = commandType;
        commandType = strtok(NULL, " ");
      }
      argv[argc] = NULL;
      char *path = find_in_path(argv[0]);
      if (path)
      {
        argv[0] = path;
        execute_cmd(argv);
      }
      else
      {
        printf("%s: command not found\n", argv[0]);
      }
    }

    fflush(stdout);

  }

  return 0;
}
