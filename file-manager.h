#pragma once
#include <stdio.h>
//#include <assert.h>
#include "signal-helper.h"
//#include <dirent.h>
//#include <unistd.h>

// comming soon:)

enum
file_ext {
    CSV,
    WAV,
    JSON
};

typedef enum file_ext file_ext;

void
open_from_file(
    signal_array *signals_array,
    char *path,
    file_ext file_ext
);

void
save_to_file(
    signal_array *signals_array,
    char *path,
    file_ext file_ext
);

void
fwrite_int_value(
    unsigned int value,
    int size,
    FILE *file
);