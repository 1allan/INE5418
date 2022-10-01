#include <stdlib.h>
#include <sys/types.h>
#include <string.h>

typedef struct Line {
    int number;
    char content[1024];
    struct Line *next;
} line_t;

line_t* search(int number, line_t* file)
{
    line_t *current = file;
    while (current != NULL) {
        if (current->number != number) {
            current = current->next;
        } else {
            return current;
        }
    }
    return current;
}

void new_line(int number, char* content, line_t* file)
{
    line_t new = { .number = number, .content = content };
    file->next = &new;
}

void update_line(int number, char* new_content, line_t* file)
{
    line_t *line = search(number, file);
    if (line == NULL) {
        new_line(number, new_content, file);
    } else {
        strcpy(line->content, new_content);
    }
}

int count_lines(line_t* file)
{
    int count = 0;
    line_t *current = file;
    while (current != NULL) {
        ++count;
    }
    return count;
}
