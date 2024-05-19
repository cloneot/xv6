#include "types.h"
#include "stat.h"
#include "user.h"

#define NUM_THREAD 5

void *spin_print(void *arg)
{
  int val = (int)arg;
  while(1)
  {
    printf(1, "Thread %d\n", val);
    sleep(100);
  }
}

void *thread_main(void *arg)
{
  int val = (int)arg;
  printf(1, "Thread %d start\n", val);
  if (arg == 0) {
    sleep(100);
    printf(1, "Exiting...\n");
    exit();
  }
  else {
    sleep(200);
  }

  printf(1, "This code shouldn't be executed!!\n");
  exit();
  return 0;
}

thread_t thread[NUM_THREAD];

int main(int argc, char *argv[])
{
  int i;
  printf(1, "Thread exit test start\n");
  for (i = 0; i < NUM_THREAD; i++) {
    thread_create(&thread[i], spin_print, (void *)i);
  }
  sleep(300);
  char *pname = "/hello_thread";
  char *args[2] = {pname, 0};
  printf(1, "Executing...\n");
  exec(pname, args);
  printf(1, "This code shouldn't be executed!!\n");
  exit();
}
