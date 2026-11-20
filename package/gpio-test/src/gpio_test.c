#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <gpiod.h>

#define GPIO_CHIP "/dev/gpiochip0"
#define GPIO_OFFSET 17

volatile sig_atomic_t running = 1;

void handle_signal(int signal)
{
    running = 0;
}

int main(int argc, char *argv[])
{
    int interval = 1000;
    int exit_status = 0;
    int line_requested = 0;

    struct gpiod_chip *chip = NULL;
    struct gpiod_line *line = NULL;

    /* Blinkintervall einlesen */
    if (argc == 2) {
        interval = atoi(argv[1]);
    }

    if (interval <= 0) {
        printf("Ungueltiges Intervall!\n");
        printf("Verwendung: %s [Millisekunden]\n", argv[0]);
        return 1;
    }

    /* GPIO-Chip öffnen */
    chip = gpiod_chip_open(GPIO_CHIP);

    if (chip == NULL) {
        perror("GPIO chip open");
        return 1;
    }

    /* GPIO17 auswählen */
    line = gpiod_chip_get_line(chip, GPIO_OFFSET);

    if (line == NULL) {
        perror("GPIO line get");
        exit_status = 1;
        goto cleanup;
    }

    /* GPIO17 als Ausgang reservieren, initial LOW */
    if (gpiod_line_request_output(line, "gpio-test", 0) < 0) {
        perror("GPIO line request");
        exit_status = 1;
        goto cleanup;
    }

    line_requested = 1;

    /* CTRL+C behandeln */
    signal(SIGINT, handle_signal);

    printf("GPIO17 LED gestartet\n");
    printf("Blinkintervall: %d ms\n", interval);

    /* Endlosschleife */
    while (running) {

        if (gpiod_line_set_value(line, 1) < 0) {
            perror("GPIO HIGH");
            exit_status = 1;
            break;
        }

        usleep((useconds_t)interval * 1000U);

        if (!running)
            break;

        if (gpiod_line_set_value(line, 0) < 0) {
            perror("GPIO LOW");
            exit_status = 1;
            break;
        }

        usleep((useconds_t)interval * 1000U);
    }

    printf("\nProgramm beendet. GPIO17 = LOW\n");

cleanup:

    /* GPIO auf LOW setzen und Leitung freigeben */
    if (line_requested) {
        if (gpiod_line_set_value(line, 0) < 0) {
            perror("GPIO cleanup");
            exit_status = 1;
        }

        gpiod_line_release(line);
    }

    /* GPIO-Chip schliessen */
    if (chip != NULL)
        gpiod_chip_close(chip);

    return exit_status;
}
