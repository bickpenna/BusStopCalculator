#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STOPS 100
#define MAX_LINE_NAME 50

void adjust_minutes(int *hour, int *min, int adjustment) {
    *min += adjustment;
    while (*min >= 60) {
        *min -= 60;
        *hour += 1;
    }
    while (*min < 0) {
        *min += 60;
        *hour -= 1;
    }
}

int time_to_minutes(int hour, int min) {
    return hour * 60 + min;
}

void print_schedule(int start_h, int start_m, int end_h, int end_m, int frequency, int known_stop_h, int known_stop_m, const char *line_name) {
    int hour = start_h, min = start_m;
    int end_minutes = time_to_minutes(end_h, end_m);
    int previous_times[MAX_STOPS], count = 0;

    char filename[MAX_LINE_NAME + 4];
    sprintf(filename, "%s.txt", line_name);

    FILE *file = fopen(filename, "w");
    if (!file) {
        printf("Errore nell'aprire il file!\n");
        return;
    }

    if (known_stop_h == -1 || known_stop_m == -1) {
        fprintf(file, "Orari delle corse per la linea %s:\n", line_name);
        while (time_to_minutes(hour, min) <= end_minutes) {
            fprintf(file, "%02d:%02d\n", hour, min);
            adjust_minutes(&hour, &min, frequency);
        }
    } else {
        fprintf(file, "Orari alla fermata specifica (nota: %02d:%02d):\n", known_stop_h, known_stop_m);
        
        hour = known_stop_h;
        min = known_stop_m;
        while (time_to_minutes(hour, min) >= time_to_minutes(start_h, start_m) && count < MAX_STOPS) {
            previous_times[count++] = time_to_minutes(hour, min);
            adjust_minutes(&hour, &min, -frequency);
        }

        for (int i = count - 1; i >= 0; i--) {
            hour = previous_times[i] / 60;
            min = previous_times[i] % 60;
            fprintf(file, "%02d:%02d\n", hour, min);
        }

        hour = known_stop_h;
        min = known_stop_m;
        adjust_minutes(&hour, &min, frequency);
        while (time_to_minutes(hour, min) <= end_minutes) {
            fprintf(file, "%02d:%02d\n", hour, min);
            adjust_minutes(&hour, &min, frequency);
        }
    }

    fclose(file);
    printf("Gli orari sono stati salvati nel file: %s\n", filename);
}

int main() {
     int start_h, start_m, end_h, end_m, frequency;
    int known_stop_h = -1, known_stop_m = -1;
    char line_name[MAX_LINE_NAME];

    printf("Inserisci il nome della linea: ");
    fgets(line_name, MAX_LINE_NAME, stdin);
    line_name[strcspn(line_name, "\n")] = 0;

    printf("Inserisci l'orario di inizio corsa (hh:mm): ");
    scanf("%d:%d", &start_h, &start_m);

    printf("Inserisci l'orario di fine corsa (hh:mm): ");
    scanf("%d:%d", &end_h, &end_m);

    printf("Inserisci la frequenza delle corse (mm): ");
    scanf("%d", &frequency);

    printf("Inserisci un orario certo quando il bus passa alla fermata specifica (hh:mm) (-1 se non applicabile): ");
    scanf("%d:%d", &known_stop_h, &known_stop_m);

    print_schedule(start_h, start_m, end_h, end_m, frequency, known_stop_h, known_stop_m, line_name);

    return 0;
}