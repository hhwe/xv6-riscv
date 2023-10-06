#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/fcntl.h"
#include "kernel/param.h"

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        fprintf(2, "xargs: missing operand\n");
        exit(1);
    }

    char *ex_argv[MAXARG];
    int ex_argc = 0;

    int i;
    for (i = 1; i < argc; i++)
    {
        ex_argv[ex_argc++] = argv[i];
    }

    char buf[512] = {0};
    int len;
    int k = 0;
    while (((len = read(0, &buf[k], (sizeof(buf) - k))) > 0) && (ex_argc < MAXARG))
    {
        printf("buf: %d %d %s", len, k, &buf[k]);
        int m = k;
        for (i = k; i < k + len; i++)
        {
            if (buf[i] == '\n')
            {
                ex_argv[ex_argc++] = &buf[m];
                ex_argv[ex_argc] = 0;
                ex_argc = argc - 1;
                buf[i] = 0;
                m = i + 1;
                if (fork() == 0)
                {
                    exec(argv[1], ex_argv);
                    exit(0);
                }
                else
                {
                    wait(0);
                }
            }
            else if (buf[i] == ' ')
            {
                ex_argv[ex_argc++] = &buf[m];
                buf[i] = 0;
                // printf("args: %s\n", &buf[m]);
                m = i + 1;
            }
        }
        k = i;
    }

    exit(0);
}
