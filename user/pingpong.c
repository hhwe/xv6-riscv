#include "kernel/types.h"
#include "user/user.h"

int main(void)
{
    char ping[] = "ping";
    char pong[] = "pong";
    char buf[10] = {0};
    int fds[2][2];

    pipe(fds[0]);
    pipe(fds[1]);
    // printf("000\r\n");
    if (fork() == 0) // child processer
    {
        close(fds[0][1]);
        close(fds[1][0]);
        // while (1)
        // {
        read(fds[0][0], buf, sizeof(buf));
        printf("%d: received ping\n", getpid());
        // sleep(1);
        write(fds[1][1], pong, sizeof(pong));
        // }
        close(fds[0][0]);
        close(fds[1][1]);
        exit(0);
    }
    else
    {
        close(fds[0][0]);
        close(fds[1][1]);
        // while (1)
        // {
        write(fds[0][1], ping, sizeof(ping));
        read(fds[1][0], buf, sizeof(buf));
        printf("%d: received pong\n", getpid());
        // sleep(1);
        // }
        close(fds[0][1]);
        close(fds[1][0]);
        exit(0);
    }

    return 0;
}