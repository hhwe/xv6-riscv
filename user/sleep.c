#include "kernel/types.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  int i;

  if(argc <= 1){
    fprintf(2, "sleep: missing operand\n");
    exit(0);
  }

  int seconds = 0;

  for(i = 1; i < argc; i++){
    int t = atoi(argv[i]);
    if(t <= 0){
      fprintf(2, "sleep: invalid time interval %s\n", argv[i]);
      exit(1);
    }
    seconds += t;
  }
  sleep(seconds);
  exit(0);
}
