#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void ping_pong(void)
{
    int fds1[2];
    if (pipe(fds1))
    {
        fprintf(2, "error pipe\n");
        exit(1);
    }
    int fds2[2];
    if (pipe(fds2))
    {
        fprintf(2, "error pipe\n");
        exit(1);
    }

    int pid = fork();
    if (pid == 0)
    {
        char buf;
        close(fds1[1]);
        close(fds2[0]);
        while (read(fds1[0], &buf, sizeof(buf)))
        {
            printf("child: %d\n", buf);
            sleep(10);
            write(fds2[1], &buf, sizeof(buf));
        }
        close(fds1[0]);
        close(fds2[1]);
    }
    else if (pid > 0)
    {
        char buf = 0;
        close(fds1[0]);
        close(fds2[1]);
        while (buf++ < 10)
        {
            sleep(10);
            write(fds1[1], &buf, sizeof(buf));
            read(fds2[0], &buf, sizeof(buf));
            printf("parent: %d\n", buf);
        }
        close(fds1[1]);
        close(fds2[0]);
        wait(0);
    }
    else
    {
        fprintf(2, "error fork\n");
        exit(1);
    }
}

void free_mem(void)
{
    printf("lsmem %d\n", lsmem());
}

int main(int argc, char *argv[])
{
    int id;

    if (argc <= 1)
    {
        fprintf(2, "usage: ex id [file ...]\n");
        exit(1);
    }

    id = atoi(argv[1]);
    switch (id)
    {
    case 1:
        // 1.1. Write a program that uses UNIX system calls to “ping-pong” a byte between two processes
        // over a pair of pipes, one for each direction. Measure the program’s performance, in exchanges per second.
        ping_pong();
        break;

    case 2:
        // 2.1. Add a system call to xv6 that returns the amount of free memory available.
        free_mem();
        break;

    case 3:
        // 1. Parse RISC-V’s device tree to find the amount of physical memory the computer has.
        // 2. Write a user program that grows its address space by one byte by calling sbrk(1). Run
        // the program and investigate the page table for the program before the call to sbrk and after
        // the call to sbrk. How much space has the kernel allocated? What does the PTE for the new
        // memory contain?
        // 3. Modify xv6 to use super pages for the kernel.
        // 4. Unix implementations of exec traditionally include special handling for shell scripts. If the
        // file to execute begins with the text #!, then the first line is taken to be a program to run to
        // interpret the file. For example, if exec is called to run myprog arg1 and myprog ’s first
        // line is #!/interp, then exec runs /interp with command line /interp myprog arg1.
        // Implement support for this convention in xv6.
        // 5. Implement address space layout randomization for the kernel.
        ;
        break;
    default:
        fprintf(2, "error id\n");
        exit(1);
        break;
    }

    exit(0);
}
