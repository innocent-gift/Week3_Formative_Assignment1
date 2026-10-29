#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

volatile sig_atomic_t run = 1;
void handle(int sig) { (void)sig; run = 0; }

int main(void) {
    signal(SIGINT, handle);
    printf("Controller started. PID: %d\n", getpid());
    while(run) {
        printf("Running...\n");
        sleep(5);
    }
    printf("Exiting gracefully.\n");
    return 0;
}
