#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "log.h"

unsigned int log_entry_number = 0;

char *get_current_timestamp() {
    char *timestamp = malloc(100 * sizeof(char));
    time_t rawtime;
    struct tm *timeinfo;

    time(&rawtime);
    timeinfo = localtime(&rawtime);

    strftime(timestamp, 100, "%d-%m-%y %H:%M:%S", timeinfo);
    return timestamp;
}

char *get_log_line(char *timestamp, char *process_name, char *entry) {
    char *line = calloc(sizeof(char), strlen(timestamp) + strlen(process_name) + strlen(entry) + 75);

    // Generate log entries
    sprintf(line, "[%s] [%s] #%d: %s\n", timestamp, process_name, ++log_entry_number, entry);

    return line;
}
