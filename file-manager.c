#include "file-manager.h"

void
open_from_file(
    signal_array *signals_array,
    char *path,
    file_ext file_ext
) {
    FILE *file;
    file = fopen(path, "r");
    switch (file_ext)
    {
    case WAV: ;
        
        break;
    case CSV: ;
        
        break;
    case JSON: ;
        
        break;
    }
    fclose(file);
}

void
save_to_file(
    signal_array *signals_array,
    char *path,
    file_ext file_ext
) {
    FILE *file;
    file = fopen(path, "w");
    switch (file_ext)
    {
    case WAV: ;
        unsigned int bytes_per_sample = (signals_array->signals[0].bits_per_sample / 8);
        unsigned int file_size = 36 + bytes_per_sample * signals_array->signals[0].size * signals_array->signals[0].channels;
        unsigned int byte_rate = signals_array->signals[0].sample_rate * signals_array->signals[0].channels * bytes_per_sample;
        unsigned int block_align = bytes_per_sample * signals_array->signals[0].channels;
        unsigned int data_size = bytes_per_sample * signals_array->signals[0].size * signals_array->signals[0].channels;

        // WAV structure https://www.isip.piconepress.com/projects/speech/software/tutorials/production/fundamentals/v1.0/section_02/s02_01_p05.html
        fwrite("RIFF", 1, 4, file); // The ASCII text string "RIFF" 
        fwrite_int_value(file_size, 4, file); // The file size LESS the size of the "RIFF" description (4 bytes) and the size of file description (4 bytes). 
        fwrite("WAVE", 1, 4, file); // The ascii text string "WAVE".
        fwrite("fmt ", 1, 4, file); // The ascii text string "fmt " (note the trailing space).
        fwrite_int_value(16, 4, file); // The size of the WAV type format (2 bytes) + mono/stereo flag (2 bytes) + sample rate (4 bytes) + bytes/sec (4 bytes) + block alignment (2 bytes) + bits/sample (2 bytes). This is usually 16. 
        fwrite_int_value(1, 2, file); // Type of WAV format. This is a PCM header, or a value of 0x01. 
        fwrite_int_value(signals_array->signals[0].channels, 2, file); // mono (0x01) or stereo (0x02) 
        fwrite_int_value(signals_array->signals[0].sample_rate, 4, file); // The sample frequency. 
        fwrite_int_value(byte_rate, 4, file); // The audio data rate in bytes/sec. 
        fwrite_int_value(block_align, 2, file); // The block alignment. 
        fwrite_int_value(8 * bytes_per_sample, 2, file); // The number of bits per sample. 
        fwrite("data", 1, 4, file); // The ascii text string "data". 
        fwrite_int_value(data_size, 4, file); // Number of bytes of data is included in the data section.
        // Write signal samples
        for (unsigned int i = 0; i < signals_array->signals[0].size; i++)
        {
            fwrite_int_value((unsigned int)signals_array->signals_data[i], bytes_per_sample, file);
        }
        break;
    case CSV: ;
        for (unsigned int i = 0; i < signals_array->signals[0].size; i++)
        {
            fprintf(file, "%d", signals_array->signals_data[i]);
            if (i < signals_array->signals[0].size - 1) fprintf(file, "\n");
        }
        break;
    case JSON: ;
        fprintf(file, "[\n");
        for (unsigned int n = 0; n < signals_array->size; n++)
        {
            fprintf(file, "\t{\n");
            fprintf(file, "\t\t\"type\": %d,\n", signals_array->signals[n].signal_type);
            fprintf(file, "\t\t\"frequency\": %d,\n", signals_array->signals[n].frequency);
            fprintf(file, "\t\t\"amplitude\": %d,\n", signals_array->signals[n].amplitude);
            fprintf(file, "\t\t\"phase\": %f,\n", signals_array->signals[n].phase);
            fprintf(file, "\t\t\"duration\": %d,\n", signals_array->signals[n].durarion);
            fprintf(file, "\t\t\"channels\": %d,\n", signals_array->signals[n].channels);
            fprintf(file, "\t\t\"bits\": %d,\n", signals_array->signals[n].bits_per_sample);
            fprintf(file, "\t\t\"rate\": %d,\n", signals_array->signals[n].sample_rate);
            fprintf(file, "\t\t\"size\": %d,\n", signals_array->signals[n].size);
            fprintf(file, "\t\t\"samples\": [");

            unsigned int offset = count_signal_offset(signals_array, n);
            for (unsigned int i = 0; i < signals_array->signals[n].size; i++)
            {
                if (i % 50 == 0) fprintf(file, "\n\t\t\t");
                fprintf(file, "%d", signals_array->signals_data[offset + i]);
                if (i < signals_array->signals[n].size - 1) fprintf(file, ", ");
            }
            fprintf(file, "\n\t\t]");
            fprintf(file, "\n\t}");
            if (n < signals_array->size - 1) fprintf(file, ",\n");
        }
        fprintf(file, "\n]\n");
        break;
    }
    fclose(file);
}

void
fwrite_int_value(
    unsigned int value,
    int size,
    FILE *file
) {
    unsigned int tmp;
    while (size > 0)
    {
        tmp = value & 255;
        fwrite(&tmp, 1, 1, file);
        size--;
        value >>= 8;
    }
}