#ifndef COMPRESSO_LOG_H
#define COMPRESSO_LOG_H

char* get_current_timestamp();
char* get_log_line(char* timestamp, char* process_name, char* entry);

#endif //COMPRESSO_LOG_H
