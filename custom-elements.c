#include "custom-elements.h"

void
ui_piemenu(
    struct nk_context *ctx,
    struct nk_command_buffer *output,
    float x,
    float y,
    int d
)
{
    nk_fill_circle(output, nk_rect(x, y, d, d), nk_rgb(50,50,50));
}

void
ui_signal_plot(
    struct nk_context *ctx,
    signal_array *signals_array,
    unsigned int scale,
    unsigned int plot_offset,
    unsigned int index
) {
    float min = (-1) * powf(2, signals_array->signals[index].bits_per_sample - 1);
    float max = powf(2, signals_array->signals[index].bits_per_sample - 1) - 1;
    nk_chart_begin(ctx, NK_CHART_LINES, scale, min, max);
    nk_chart_add_slot_colored(ctx, NK_CHART_LINES, nk_rgb(0,0,0), nk_rgb(0,0,0), scale, min, max);
    if (signals_array->size > 0) {
        unsigned int offset = count_signal_offset(signals_array, index);
        for (int i = plot_offset; i < (plot_offset + scale); i++) {
            nk_chart_push_slot(ctx, signals_array->signals_data[offset + i], 0);
        }
    }
    nk_chart_end(ctx);
}

void
ui_signal_list(
    struct nk_context *ctx,
    signal_array *signals_array,
    unsigned int *selected_signal
) {
    static int show_edit_signal = nk_false;
    static int show_delete_signal = nk_false;
    static int selected_to_edit = 0;
    static int selected_to_delete = 0;
    for (unsigned int i = 0; i < signals_array->size; i++)
    {
        char *n;
        nk_itoa(n, (i + 1));
        nk_layout_row_dynamic(ctx, 30.0f, 5);
        nk_label(ctx, n, NK_TEXT_ALIGN_CENTERED);
        nk_label(ctx, n, NK_TEXT_ALIGN_CENTERED);
        if (nk_button_label(ctx, "Show"))
            *selected_signal = i;

        if (nk_button_label(ctx, "Edit")) {
            selected_to_edit = i;
            show_edit_signal = nk_true;
        }

        if (nk_button_label(ctx, "Delete")) {
            selected_to_delete = i;
            show_delete_signal = nk_true;
        }
    }
    if (show_edit_signal)
    {
        static struct nk_rect size = {0, 0, WINDOW_WIDTH / 3 - 18, 348};
        static char popup_title[24];
        sprintf(popup_title, "Edit signal: %d", selected_to_edit + 1);
        if (nk_popup_begin(ctx, NK_POPUP_STATIC, popup_title, NK_WINDOW_CLOSABLE, size))
        {
            unsigned int frequency = signals_array->signals[selected_to_edit].frequency, 
            amplitude = signals_array->signals[selected_to_edit].amplitude, 
            duration = signals_array->signals[selected_to_edit].durarion, 
            bits_per_sample = signals_array->signals[selected_to_edit].bits_per_sample, 
            sample_rate = signals_array->signals[selected_to_edit].sample_rate;

            float phase = signals_array->signals[selected_to_edit].phase;
            static char text_values[6][11];
            static int text_values_length[6];

            static char frequency_title[26];
            sprintf(frequency_title, "Frequency (%d Hz)", frequency);
            nk_layout_row_dynamic(ctx, 30, 2);
            nk_label(ctx, frequency_title, NK_TEXT_ALIGN_LEFT | NK_TEXT_ALIGN_MIDDLE);
            nk_edit_string(ctx, NK_EDIT_SIMPLE, text_values[0], &text_values_length[0], 11, nk_filter_decimal);
            
            static char amplitude_title[23];
            sprintf(amplitude_title, "Amplitude (%d)", amplitude);
            nk_layout_row_dynamic(ctx, 30, 2);
            nk_label(ctx, amplitude_title, NK_TEXT_ALIGN_LEFT | NK_TEXT_ALIGN_MIDDLE);
            nk_edit_string(ctx, NK_EDIT_SIMPLE, text_values[1], &text_values_length[1], 11, nk_filter_decimal);

            static char phase_title[19];
            sprintf(phase_title, "Phase (%8.2f)", phase);
            nk_layout_row_dynamic(ctx, 30, 2);
            nk_label(ctx, phase_title, NK_TEXT_ALIGN_LEFT | NK_TEXT_ALIGN_MIDDLE);
            nk_edit_string(ctx, NK_EDIT_SIMPLE, text_values[2], &text_values_length[2], 11, nk_filter_float);

            static char duration_title[24];
            sprintf(duration_title, "Duration (%d s)", duration);   
            nk_layout_row_dynamic(ctx, 30, 2);
            nk_label(ctx, duration_title, NK_TEXT_ALIGN_LEFT | NK_TEXT_ALIGN_MIDDLE);
            nk_edit_string(ctx, NK_EDIT_SIMPLE, text_values[3], &text_values_length[3], 11, nk_filter_decimal);

            static char bits_per_sample_title[33];
            sprintf(bits_per_sample_title, "Bits per sample (%d bit)", bits_per_sample);     
            nk_layout_row_dynamic(ctx, 30, 2);
            nk_label(ctx, bits_per_sample_title, NK_TEXT_ALIGN_LEFT | NK_TEXT_ALIGN_MIDDLE);
            nk_edit_string(ctx, NK_EDIT_SIMPLE, text_values[4], &text_values_length[4], 11, nk_filter_decimal);

            static char sample_rate_title[28];
            sprintf(sample_rate_title, "Sample rate (%d Hz)", sample_rate);  
            nk_layout_row_dynamic(ctx, 30, 2);
            nk_label(ctx, sample_rate_title, NK_TEXT_ALIGN_LEFT | NK_TEXT_ALIGN_MIDDLE);
            nk_edit_string(ctx, NK_EDIT_SIMPLE, text_values[5], &text_values_length[5], 11, nk_filter_decimal);

            nk_layout_row_dynamic(ctx, 30, 1);
            if (nk_button_label(ctx, "Edit")) {
                nk_popup_close(ctx);
                show_edit_signal = nk_false;
            }
            nk_popup_end(ctx);
        } else show_edit_signal = nk_false;
    }

    if (show_delete_signal)
    {
        static struct nk_rect size = {0, 0, WINDOW_WIDTH / 3 - 18, 348};
        static char popup_title[26];
        sprintf(popup_title, "Delete signal: %d", selected_to_delete + 1);
        if (nk_popup_begin(ctx, NK_POPUP_STATIC, popup_title, NK_WINDOW_CLOSABLE, size))
        {
            nk_layout_row_dynamic(ctx, 35, 1);
            nk_label(ctx, "Do you really want to delete signal?", NK_TEXT_ALIGN_CENTERED | NK_TEXT_ALIGN_MIDDLE);

            nk_layout_row_dynamic(ctx, 30, 2);
            if (nk_button_label(ctx, "Yes!")) {
                delete_signal(signals_array, selected_to_delete);
                *selected_signal = 0;
                nk_popup_close(ctx);
                show_delete_signal = nk_false;
            }
            if (nk_button_label(ctx, "No!")) {
                nk_popup_close(ctx);
                show_delete_signal = nk_false;
            }
            nk_popup_end(ctx);
        } else show_delete_signal = nk_false;
    }
}