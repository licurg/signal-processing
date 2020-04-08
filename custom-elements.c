static void
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

static void
ui_line_chart(
    struct nk_context *ctx
) {
    int i;
    float id;
    const float step = (2*3.141592654f) / 32;
    nk_chart_begin(ctx, NK_CHART_LINES, 2, 0, 100);
    nk_chart_add_slot_colored(ctx, NK_CHART_LINES, nk_rgb(0,0,255), nk_rgb(0,0,150),32, -1.0f, 1.0f);
    nk_chart_add_slot_colored(ctx, NK_CHART_LINES, nk_rgb(0,255,0), nk_rgb(0,150,0), 32, -1.0f, 1.0f);
    for (id = 0, i = 0; i < 32; ++i) {
        nk_chart_push_slot(ctx, (float)fabs(sin(id)), 0);
        nk_chart_push_slot(ctx, (float)cos(id), 1);
        nk_chart_push_slot(ctx, (float)sin(id), 2);
        id += step;
    }
    nk_chart_end(ctx);
}