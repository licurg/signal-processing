#pragma once

#include "signal-helper.h"

void
ui_piemenu(
    struct nk_context *ctx,
    struct nk_command_buffer *output,
    float x,
    float y,
    int d
);

void
ui_signal_plot(
    struct nk_context *ctx,
    signal_array *signals_array,
    unsigned int scale,
    unsigned int plot_offset,
    unsigned int index
);

void
ui_signal_list(
    struct nk_context *ctx,
    signal_array *signals_array,
    unsigned int *selected_signal
);