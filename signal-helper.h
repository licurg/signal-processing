#pragma once

#define M_PI 3.14159265358979323846

enum 
signal_type{
    Sine,
    Triangle,
    Square,
    Sawtooth,
    Mixed
};

typedef enum signal_type signal_type;

struct
signal {
    signal_type signal_type;
    float phase;
    unsigned int frequency, amplitude, sample_rate, durarion, channels, bits_per_sample, size;
};

typedef struct signal signal;

struct
signal_array {
    unsigned int size;
    signal *signals;
    int *signals_data;
};

typedef struct signal_array signal_array;

void
generate_signal(
    signal_array *signals_array,
    unsigned int index
);

void
add_signal(
    signal_array *signals_array,
    signal_type signal_type,
    float frequency,
    float amplitude,
    float phase,
    int duration,
    int bits_per_sample,
    int sample_rate
);

void
add_signal_file(
    signal_array *signals_array,
    signal_type signal_type,
    float frequency,
    float amplitude,
    float phase,
    int duration,
    int bits_per_sample,
    int sample_rate,
    int *data
);

void
delete_signal(
    signal_array *signals_array,
    unsigned int index
);

void
mix_signals(
    signal_array *signals_array,
    unsigned int duration,
    unsigned int bits_per_sample,
    unsigned int sample_rate
);

unsigned int
count_signal_offset(
    signal_array *signals_array,
    unsigned int signal_index
);

int
scale_sample(
    int sample,
    unsigned int size
);
