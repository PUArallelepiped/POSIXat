
#include <fcntl.h>
#include <linux/limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>
#define MAX_LINE 80 /* 80 chars per line, per command, should be enough. */
#define MAX_HISTORY 10
struct history {
  char command[MAX_LINE];
  struct history *next;
};

void add_history(struct history **head, char *command) {
  struct history *new_node = (struct history *)malloc(sizeof(struct history));
  strcpy(new_node->command, command);
  new_node->next = *head;
  *head = new_node;
}

void print_history(struct history *head) {
  struct history *current = head;
  while (current != NULL) {
    printf("%s\n", current->command);
    current = current->next;
  }
}

int history_length(struct history *head) {
  struct history *current = head;
  int length = 0;
  while (current != NULL) {
    length++;
    current = current->next;
  }
  return length;
}

void free_history(struct history *head) {
  struct history *current = head;
  while (current != NULL) {
    struct history *temp = current;
    current = current->next;
    free(temp);
  }
}
void free_history_tail(struct history *head) {
  struct history *current = head;
  struct history *prev = NULL;
  while (current->next != NULL) {
    prev = current;
    current = current->next;
  }
  free(current);
  prev->next = NULL;
}

char *GetNode(int n, struct history *head) {
  struct history *current = head;
  int i = 0;
  while (current != NULL) {
    if (i == n) {
      printf("%s\n", current->command);
      return current->command;
    }
    i++;
    current = current->next;
  }
  return "";
}

int main(int argc, char *argv[]) {
  char *args[MAX_LINE / 2 + 1]; /* command line arguments */
  struct history *head = NULL;

  while (1) {
    printf("Sus > ");
    char input[MAX_LINE];
    char checked_input[MAX_LINE];
    fgets(input, MAX_LINE, stdin);
    if (strlen(input) >= 2 && input[0] == '$') {
      int n = atoi(&input[1]);
      strcpy(checked_input, GetNode(n, head));
      if (strcmp(checked_input, "") == 0) {
        printf("No such command in history\n");
        continue;
      }
    } else {
      if (strcmp(input, "") == 0) {
        continue;
      }
      strcpy(checked_input, input);
      add_history(&head, checked_input);
    }
    print_history(head);
    if (history_length(head) > MAX_HISTORY) {
      free_history_tail(head);
    }
    // printf("checked_input: %s\n", checked_input);
    // printf("input: %s\n", input);

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
      read(fd[0], &input, sizeof(input));

      char *token = strtok(input, " \n");
      int i = 0;
      while (token != NULL) {
        args[i++] = token;
        token = strtok(NULL, " \n");
      }
      args[i] = NULL; // Null-terminate the argument list

      close(fd[0]);
      close(fd[1]);
      if (strcmp(args[0], "exit") == 0) {
        return 1;
      } else {
        execvp(args[0], args);
        perror("execvp");
        return 0;
      }
    } else {
      write(fd[1], &checked_input, sizeof(checked_input));
      int exit_status;
      wait(&exit_status);
      close(fd[1]);
      close(fd[0]);
      printf("exit status: %d\n", exit_status);
      if (exit_status != 0) {
        printf("end shell\n");
        return 0;
      }
    }
  }
  return 0;
}