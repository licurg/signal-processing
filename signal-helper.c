#include "signal-helper.h"

void
generate_signal(
    signal_array *signals_array,
    unsigned int index
)
{
    unsigned int offset = count_signal_offset(signals_array, index);
    unsigned int size = signals_array->signals[index].size;
    float frequency = signals_array->signals[index].frequency;
    float amplitude = signals_array->signals[index].amplitude;
    float phase = signals_array->signals[index].phase;
    int sample_rate = signals_array->signals[index].sample_rate;
    int bits_per_sample = signals_array->signals[index].bits_per_sample;
    signal_type signal_type = signals_array->signals[index].signal_type;
    
    for (unsigned int i = 0; i < size; i++)
    {
        double value = (M_PI * 2 * frequency * i) / sample_rate + phase;
        switch (signal_type)
        {
            case Sine:
                signals_array->signals_data[offset + i] = scale_sample(
                    (int)(amplitude * sin(value)), bits_per_sample);
                break;
            case Triangle:
                signals_array->signals_data[offset + i] = scale_sample(
                    (int)(amplitude * asin(sin(value))), bits_per_sample);
                break;
            case Square:
                signals_array->signals_data[offset + i] = scale_sample(
                    (int)(amplitude * (atan(sin(value)) + atan(1 / sin(value)))), bits_per_sample);
                break;
            case Sawtooth:
                signals_array->signals_data[offset + i] = scale_sample(
                    (int)(amplitude * (-1) * atan(1 / tan(value))), bits_per_sample);
                break;
        }
    }
}

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
)
{
    unsigned int index = 0;
    unsigned int signal_size = (duration * sample_rate) * sizeof(int);
    if (signals_array->signals == NULL) {
        signals_array->signals = (signal*)malloc(sizeof(signal));
        signals_array->signals_data = (int*)malloc(signal_size);
    }
    else {
        unsigned int length = signals_array->size;
        for (unsigned int i = 0; i < length; i++)
        {
            signal_size += signals_array->signals[i].size * sizeof(int);
        }
        signals_array->signals = (signal*)realloc(signals_array->signals, (length + 1) * sizeof(signal));
        signals_array->signals_data = (int*)realloc(signals_array->signals_data, signal_size);
        index = length;
    }

    signals_array->signals[index].signal_type = signal_type;
    signals_array->signals[index].frequency = frequency;
    signals_array->signals[index].amplitude = amplitude;
    signals_array->signals[index].phase = phase;
    signals_array->signals[index].durarion = duration;
    signals_array->signals[index].sample_rate = sample_rate;
    signals_array->signals[index].channels = 1;
    signals_array->signals[index].bits_per_sample = bits_per_sample;
    signals_array->signals[index].size = (duration * sample_rate);
    signals_array->size++;

    generate_signal(signals_array, index);
}

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
) {
    
}

void
delete_signal(
    signal_array *signals_array,
    unsigned int index
) {
    signal *signals_buffer;
    int *data_buffer;
    unsigned int offset_to_delete = count_signal_offset(signals_array, index);
    unsigned int new_size = 0, old_size = 0;
    for (unsigned int i = 0; i < signals_array->size; i++)
    {
        if (i != index) new_size += signals_array->signals[i].size;
        old_size += signals_array->signals[i].size;
    }
    
    signals_buffer = (signal*)malloc((signals_array->size - 1) * sizeof(signal));
    data_buffer = (int*)malloc(new_size * sizeof(int));

    if (index != 0) {
        nk_memcopy(signals_buffer, signals_array->signals, index * sizeof(signal));
        nk_memcopy(data_buffer, signals_array->signals_data, offset_to_delete * sizeof(int));
    }

    if (index != (signals_array->size - 1)) {
        nk_memcopy(signals_buffer+index, signals_array->signals+index+1, (signals_array->size - index - 1) * sizeof(signal));
        nk_memcopy(data_buffer+offset_to_delete, 
            signals_array->signals_data+offset_to_delete+(old_size - new_size),
            (old_size - offset_to_delete - (old_size - new_size)) * sizeof(int));
    }

    signals_array->size--;
    free(signals_array->signals);
    signals_array->signals = (signal*)malloc(signals_array->size * sizeof(signal));
    free(signals_array->signals_data);
    signals_array->signals_data = (int*)malloc(new_size * sizeof(int));

    nk_memcopy(signals_array->signals, signals_buffer, signals_array->size * sizeof(signal));
    nk_memcopy(signals_array->signals_data, data_buffer, new_size * sizeof(int));
    
    free(signals_buffer);
    free(data_buffer);
}

void
mix_signals(
    signal_array *signals_array,
    unsigned int duration,
    unsigned int bits_per_sample,
    unsigned int sample_rate
) {
    unsigned int size = duration * sample_rate;
    int *new_signal_data = (int*)malloc(size * sizeof(int));
    signal new_signal;
    
    for (unsigned int i = 0; i < size; i++)
    {
        int sample = 0;
        for (unsigned int n = 0; n < signals_array->size; n++) {
            sample += signals_array->signals_data[n * size + i];
        }
        new_signal_data[i] = scale_sample(sample, bits_per_sample);
    }

    new_signal.signal_type = Mixed;
    new_signal.amplitude = signals_array->signals[0].amplitude;
    new_signal.frequency = signals_array->signals[0].frequency;
    new_signal.phase = signals_array->signals[0].phase;
    new_signal.durarion = duration;
    new_signal.channels = 1;
    new_signal.bits_per_sample = bits_per_sample;
    new_signal.sample_rate = sample_rate;
    new_signal.size = size;

    signals_array->size = 1;
    signals_array->signals = (signal*)malloc(sizeof(signal));
    nk_memcopy(signals_array->signals, &new_signal, sizeof(signal));
    signals_array->signals_data = (int*)malloc(size * sizeof(int));
    nk_memcopy(signals_array->signals_data, new_signal_data, size * sizeof(int));

    free(new_signal_data);
}

unsigned int
count_signal_offset(
    signal_array *signals_array,
    unsigned int signal_index
) {
    unsigned int offset = 0;
    for (unsigned int i = 0; i < signal_index; i++) {
        offset += signals_array->signals[i].size;
    }
    return offset;
}

int
scale_sample(
    int sample,
    unsigned int size
) {
    if (size == 32) return sample;
    float min = (-1) * powf(2, size - 1);
    float max = powf(2, size - 1) - 1;
    if (sample > max) return max;
    if (sample < min) return min;
    return sample;
}