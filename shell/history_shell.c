#include <ctype.h>
#include <fcntl.h>
#include <linux/limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>
#define MAX_LINE 80

char *strip_string(char *str) {
  // Trim leading whitespace
  while (isspace((unsigned char)*str)) {
    str++;
  }

  // If the entire string is whitespace, str will point to '\0'
  if (*str == '\0') {
    return str;
  }

  // Trim trailing whitespace
  char *end = str + strlen(str) - 1;
  while (end > str && isspace((unsigned char)*end)) {
    end--;
  }

  // Null-terminate the trimmed string
  *(end + 1) = '\0';

  return str;
}

void split(char **arr, char *str, const char *del, int *len) {
  char *s = strtok(str, del);
  *len = 0;
  while (s != NULL) {
    *arr++ = s;
    s = strtok(NULL, del);
    *len += 1;
  }
  arr[*len] = NULL;
}

void run_command(char *command) {
  char *args[MAX_LINE / 2 + 1]; /* command line arguments */
  int len = 0;
  split(args, command, " \n", &len);
  args[len] = NULL;
  execvp(args[0], args);
  perror("execvp");
  exit(EXIT_FAILURE);
}
int wait_run_command(char *command, int is_wait) {
  pid_t pid = fork();
  if (pid == -1) {
    perror("fork");
    exit(EXIT_FAILURE);
  } else if (pid == 0) {
    run_command(command);
  } else {
    if (is_wait) {
      int exit_status;
      wait(&exit_status);
      return exit_status;
    }
  }
  return 0;
}
static char *history_command = NULL;

int main(int argc, char *argv[]) {
  char *args[MAX_LINE / 2 + 1]; /* command line arguments */
  int should_run = 1;
  char input[MAX_LINE];
  char child_input[MAX_LINE];
  while (should_run) {
    printf("osh>");
    fflush(stdout);

    fgets(input, MAX_LINE, stdin);
    //   run_command(input);
    if (strcmp(input, "exit\n") == 0) {
      should_run = 0;
      continue;
    }

    const char *result = strstr(input, "!!");
    if (result != NULL) {
      if (history_command == NULL) {
        printf("No command in history.\n");
      } else {
        printf("history command: %s\n", history_command);
        // do history
        wait_run_command(history_command, 1);
      }
      continue;
    }
    char tmp[MAX_LINE];
    strcpy(tmp, input);
    history_command = tmp;

    char **pipe_command =
        malloc(sizeof(char) * MAX_LINE * MAX_LINE); /* command line arguments */
    int len = 0;
    split(pipe_command, input, "<", &len);
    if (len > 1) {
      pid_t pid = fork();
      if (pid == -1) {
        perror("fork");
        return 1;
      } else if (pid == 0) {
        pipe_command[1] = strip_string(pipe_command[1]);
        int filed = open(pipe_command[1], O_RDONLY, 0666);
        if (dup2(filed, STDIN_FILENO) == -1) {
          perror("dup2");
          exit(EXIT_FAILURE);
        }
        close(filed);
        run_command(pipe_command[0]);

      } else {
        // todo
        wait(NULL);
      }
      free(pipe_command);
      continue;
    }

    split(pipe_command, input, ">", &len);
    if (len > 1) {
      pid_t pid = fork();
      if (pid == -1) {
        perror("fork");
        return 1;
      } else if (pid == 0) {
        pipe_command[1] = strip_string(pipe_command[1]);
        int filed = open(pipe_command[1], O_CREAT | O_WRONLY | O_TRUNC, 0666);
        if (dup2(filed, STDOUT_FILENO) == -1) {
          perror("dup2");
          exit(EXIT_FAILURE);
        }
        close(filed);
        run_command(pipe_command[0]);

      } else {
        // todo
        wait(NULL);
      }
      free(pipe_command);
      continue;
    }

    split(pipe_command, input, "|", &len);
    if (len > 1) {
      // do pipe
      pid_t pid_1 = fork();
      if (pid_1 == -1) {
        perror("fork");
        return 1;
      } else if (pid_1 == 0) {
        int fd[2];
        if (pipe(fd) == -1) {
          perror("pipe");
          return 1;
        }

        pid_t pid = fork();
        if (pid == -1) {
          perror("fork");
          return 1;
        } else if (pid == 0) {

          if (dup2(fd[1], STDOUT_FILENO) == -1) {
            perror("dup2");
            exit(EXIT_FAILURE);
          }
          close(fd[0]);
          close(fd[1]);
          run_command(pipe_command[0]);
        } else {
          wait(NULL);
          if (dup2(fd[0], STDIN_FILENO) == -1) {
            perror("dup2");
            exit(EXIT_FAILURE);
          }
          close(fd[0]);
          close(fd[1]);
          run_command(pipe_command[1]);
        }

      } else {
        wait(NULL);
      }
      // do pipe
      free(pipe_command);
      continue;
    }

    char *backend_command[MAX_LINE / 2 + 1]; /* command line arguments */
    split(backend_command, input, "&", &len);
    if (len > 1) {
      wait_run_command(history_command, 0);
      // do background
      free(pipe_command);
      continue;
    }

    // normal
    wait_run_command(input, 1);
    free(pipe_command);
  }

  //   pid_t pid = fork();
  //   if (pid == -1) {
  //     perror("fork");
  //     return 1;
  //   } else if (pid == 0) {
  //     // run_command(input);
  //     printf("child input: %s\n", input);
  //   } else {
  //     // wait(NULL);
  //     printf("parent input: %s\n", input);
  //   }
  return 0;
}