#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

#define GPIO_PATH "/sys/class/gpio/gpio529"

volatile sig_atomic_t running = 1;

void handle_signal(int signal)
{
    running = 0;
}

int write_gpio(const char *filename, const char *value)
{
    char path[128];

    snprintf(path, sizeof(path), "%s/%s", GPIO_PATH, filename);

    FILE *fp = fopen(path, "w");

    if (fp == NULL) {
        perror(path);
        return -1;
    }

    fprintf(fp, "%s", value);
    fclose(fp);

    return 0;
}

int main(int argc, char *argv[])
{
    int interval = 1000;

    if (argc == 2) {
        interval = atoi(argv[1]);
    }

    if (interval <= 0) {
        printf("Ungueltiges Intervall!\n");
        printf("Verwendung: %s [Millisekunden]\n", argv[0]);
        return 1;
    }

    signal(SIGINT, handle_signal);

    printf("GPIO17 LED gestartet\n");
    printf("Blinkintervall: %d ms\n", interval);

    if (write_gpio("direction", "out") < 0)
        return 1;

    while (running) {

        if (write_gpio("value", "1") < 0)
            break;

        usleep(interval * 1000);

        if (!running)
            break;

        if (write_gpio("value", "0") < 0)
            break;

        usleep(interval * 1000);
    }

    write_gpio("value", "0");

    printf("\nProgramm beendet. GPIO17 = LOW\n");

    return 0;
}
