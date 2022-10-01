
#include <stdbool.h>
#include <sys/types.h>
#include <stdlib.h>
#include <stdint.h>

typedef struct body {
    int line;
    char content[2048];
} body_t;

typedef struct payload {
    char operation[4];
    bool error;
    body_t body;
} payload_t;
