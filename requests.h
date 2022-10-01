
#include <stdbool.h>
#include <sys/types.h>
#include <stdlib.h>
#include <stdint.h>

typedef struct payload {
    uint32_t operation;
    bool error;
    char body[2048];
} payload_t;
