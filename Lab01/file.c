#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 1000
#define FILE_NAME "file.txt"
#define T_FILE_NAME "file.tmp"

int get(int line_number, char *s)
{
    char path[] = FILE_NAME;
    char buffer[BUFFER_SIZE];

    FILE *file = fopen(path, "r");

    if (file == NULL) {
        return 0;
    }

    int curr_line = 0;
    while (fgets(buffer, BUFFER_SIZE, file) != NULL) {
        if (++curr_line == line_number) {
            strncpy(s, buffer, BUFFER_SIZE);
            break;
        }
    }
    fclose(file);
    return 1;
}

int put(int line, char *content)
{
    char path[] = FILE_NAME;
    char buffer[BUFFER_SIZE];

    FILE *file = fopen(path, "r");
    FILE *temp = fopen(T_FILE_NAME, "w");

    if (file == NULL || temp == NULL) {
        return 0;
    }

    int curr_line = 0;
    char *cursor = fgets(buffer, BUFFER_SIZE, file);
    while (curr_line <= line || cursor != NULL) {
        if (++curr_line != line) {
            if (cursor == NULL) {
                fputs("\n", temp);
            } else {
                fputs(buffer, temp);
            }
        } else {
            fputs(content, temp);
            fputs("\n", temp); // off-by-one goes brrrrr
        }
        cursor = fgets(buffer, BUFFER_SIZE, file);
    }

    fclose(file);
    fclose(temp);
    remove(path);
    rename(T_FILE_NAME, path);

    return 1;
}
