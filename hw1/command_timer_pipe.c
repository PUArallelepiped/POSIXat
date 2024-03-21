
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>

#include <sys/time.h>
#include <unistd.h>

#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
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
    // get time of day
    close(fd[0]);
    struct timeval tv;
    gettimeofday(&tv, NULL);
    printf("Time of day: %ld\n", tv.tv_usec);
    write(fd[1], &tv, sizeof(tv));
    close(fd[1]);

    // run cal

    // print argv[0]
    printf("argv[0]: %s\n", argv[0]);
    printf("argv[1]: %s\n", argv[1]);
    printf("argv[2]: %s\n", argv[2]);
    execvp(argv[1], argv + 1);
    return 0;

  } else {
    int exit_status;
    wait(&exit_status);
    close(fd[1]);
    struct timeval child_tv;
    read(fd[0], &child_tv, sizeof(child_tv));
    close(fd[0]);

    // printf("Child process exited with status %d\n",
    // WEXITSTATUS(exit_status));
    struct timeval tv;
    gettimeofday(&tv, NULL);
    printf("return: %ld\n", tv.tv_usec - child_tv.tv_usec);
  }
  return 0;
}
