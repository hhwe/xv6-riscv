#include "kernel/types.h"
#include "user/user.h"

void primes(int fd)
{
    int buf = 0;
    int fds[2];

    int first_prime;
    if (read(fd, &first_prime, sizeof(first_prime)) <= 0)
    {
        close(fd);
        exit(0);
        return;
    }
    printf("prime %d\n", first_prime);

    pipe(fds);
    if (fork() == 0) // child processer
    {
        close(fds[1]);
        primes(fds[0]);
        close(fds[0]);
        exit(0);
    }
    else
    {
        close(fds[0]);
        while (read(fd, &buf, sizeof(buf)) > 0)
        {
            if (buf % first_prime != 0)
            {
                write(fds[1], &buf, sizeof(buf));
            }
        }
        close(fds[1]);
        close(fd);
        wait((int *) (0));
        exit(0);
    }

    return;
}

int main(void)
{
    int fds[2];
    pipe(fds);
    for (int i = 2; i < 36; i++)
    {
        write(fds[1], &i, sizeof(i));
    }
    close(fds[1]);
    primes(fds[0]);
    return 0;
}