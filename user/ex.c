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

void grow_1byte(void)
{
    sbrk(1);
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
        grow_1byte();
        // 3. Modify xv6 to use super pages for the kernel.
        // 4. Unix implementations of exec traditionally include special handling for shell scripts. If the
        // file to execute begins with the text #!, then the first line is taken to be a program to run to
        // interpret the file. For example, if exec is called to run myprog arg1 and myprog ’s first
        // line is #!/interp, then exec runs /interp with command line /interp myprog arg1.
        // Implement support for this convention in xv6.
        // 5. Implement address space layout randomization for the kernel.
        break;
    case 4:
        // 1. The functions copyin and copyinstr walk the user page table in software. Set up
        // the kernel page table so that the kernel has the user program mapped, and copyin and
        // copyinstr can use memcpy to copy system call arguments into kernel space, relying on
        // the hardware to do the page table walk.
        // 2. Implement lazy memory allocation.
        // 3. Implement COW fork.
        // 4. Is there a way to eliminate the special TRAPFRAME page mapping in every user address
        // space? For example, could uservec be modified to simply push the 32 user registers onto
        // the kernel stack, or store them in the proc structure?
        // 5. Could xv6 be modified to eliminate the special TRAMPOLINE page mapping?
        break;
    case 5:
        // 1. Modify uart.c to not use interrupts at all. You may need to modify console.c as well.
        // 2. Add a driver for an Ethernet card
        break;
    case 6:
        // 1. Comment out the calls to acquire and release in kalloc (kernel/kalloc.c:69). This seems
        // like it should cause problems for kernel code that calls kalloc; what symptoms do you
        // expect to see? When you run xv6, do you see these symptoms? How about when running
        // usertests? If you don’t see a problem, why not? See if you can provoke a problem by
        // inserting dummy loops into the critical section of kalloc.
        // 2. Suppose that you instead commented out the locking in kfree (after restoring locking
        // in kalloc). What might now go wrong? Is lack of locks in kfree less harmful than in
        // kalloc?
        // 3. If two CPUs call kalloc at the same time, one will have to wait for the other, which is bad
        // for performance. Modify kalloc.c to have more parallelism, so that simultaneous calls to
        // kalloc from different CPUs can proceed without waiting for each other.
        // 4. Write a parallel program using POSIX threads, which is supported on most operating systems. For example, implement a parallel hash table and measure if the number of puts/gets
        // scales with increasing number of cores.
        // 5. Implement a subset of Pthreads in xv6. That is, implement a user-level thread library so that
        // a user process can have more than 1 thread and arrange that these threads can run in parallel
        // on different CPUs. Come up with a design that correctly handles a thread making a blocking
        // system call and changing its shared address space.
        break;
    case 7:
        // 1. Sleep has to check lk != &p->lock to avoid a deadlock Suppose the special case were
        // eliminated by replacing
        // if(lk != &p->lock){
        // acquire(&p->lock);
        // release(lk);
        // }
        // with
        // release(lk);
        // acquire(&p->lock);
        // Doing this would break sleep. How?
        // 2. Implement semaphores in xv6 without using sleep and wakeup (but it is OK to use spin
        // locks). Replace the uses of sleep and wakeup in xv6 with semaphores. Judge the result.
        // 3. Fix the race mentioned above between kill and sleep, so that a kill that occurs after
        // the victim’s sleep loop checks p->killed but before it calls sleep results in the victim
        // abandoning the current system call.
        // 4. Design a plan so that every sleep loop checks p->killed so that, for example, a process
        // that is in the virtio driver can return quickly from the while loop if it is killed by another
        // process.
        // 5. Modify xv6 to use only one context switch when switching from one process’s kernel thread
        // to another, rather than switching through the scheduler thread. The yielding thread will need
        // to select the next thread itself and call swtch. The challenges will be to prevent multiple
        // cores from executing the same thread accidentally; to get the locking right; and to avoid
        // deadlocks.
        // 6. Modify xv6’s scheduler to use the RISC-V WFI (wait for interrupt) instruction when no
        // processes are runnable. Try to ensure that, any time there are runnable processes waiting to
        // run, no cores are pausing in WFI.
        break;
    default:
        fprintf(2, "error id\n");
        exit(1);
        break;
    }

    exit(0);
}
