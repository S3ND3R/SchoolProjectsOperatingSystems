#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

// https://genius.com/Walt-disney-records-circle-of-life-lyrics
char *lyrics[] = {
  "Nants ingonyama bagithi baba",
  "Sithi uhm ingonyama",
  "Nants ingonyama bagithi baba",
  "Sithi uhm ingonyama",
  "Ingonyama",
  "Siyo Nqoba",
  "Ingonyama Ingonyama nengw' enamabala ",
  "From the day we arrive on the planet",
  "And blinking, step into the Sun",
  "There's more to be seen than can ever be seen",
  "More to do than can ever be done",
  "Some say eat or be eaten",
  "Some say live and let live",
  "But all are agreed as they join the stampede",
  "You should never take more than you give",
  "In the circle of life",
  "It's the wheel of fortune",
  "It's the leap of faith",
  "It's the band of hope",
  "Till we find our place",
  "On the path unwinding",
  "In the circle, the circle of life",
  "Some of us fall by the wayside",
  "And some of us soar to the stars",
  "And some of us sail through our troubles",
  "And some have to live with the scars",
  "There's far too much to take in here",
  "More to find than can ever be found",
  "But the Sun rolling high through the sapphire sky",
  "Keeps great and small on the endless round",
  NULL
};


void int_handler(int sig_num){
  printf("THE CIRCLE OF LIFE CONTINUES ON...\n");
  pid_t pid = fork();
  if(pid == 0) {  // in child
    if (execlp("./circle_of_life", "./circle_of_life") == -1) {
      perror("exec failed");
      exit(1);
    };
  }
}

int main(){
  struct sigaction my_sa = {};
  sigemptyset(&my_sa.sa_mask);
  my_sa.sa_flags = SA_RESTART;
  my_sa.sa_handler = int_handler;
  sigaction(SIGINT, &my_sa, NULL);
  sigaction(SIGTERM, &my_sa, NULL);
  int index = 0;
  while(lyrics[index] != NULL){
    printf("%s\n",lyrics[index]);
    fflush(stdout);
    index++;
    sleep(1);
  }
  exit(0);
}