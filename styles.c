#include "styles.h"

void
set_style(
    struct nk_context *ctx
) {
    struct nk_color color_table[NK_COLOR_COUNT];
    color_table[NK_COLOR_TEXT] = nk_rgb(234, 234, 234);
    color_table[NK_COLOR_WINDOW] = nk_rgb(0, 0, 0);
    color_table[NK_COLOR_HEADER] = nk_rgb(0, 0, 0);
    color_table[NK_COLOR_BORDER] = nk_rgba(153, 0, 0, 235);
    color_table[NK_COLOR_BUTTON] = nk_rgb(153, 0, 0);
    color_table[NK_COLOR_BUTTON_HOVER] = nk_rgba(153, 0, 0, 245);
    color_table[NK_COLOR_BUTTON_ACTIVE] = nk_rgba(153, 0, 0, 245);
    color_table[NK_COLOR_TOGGLE] = nk_rgba(50, 58, 61, 255);
    color_table[NK_COLOR_TOGGLE_HOVER] = nk_rgba(45, 53, 56, 255);
    color_table[NK_COLOR_TOGGLE_CURSOR] = nk_rgba(48, 83, 111, 255);
    color_table[NK_COLOR_SELECT] = nk_rgba(57, 67, 61, 255);
    color_table[NK_COLOR_SELECT_ACTIVE] = nk_rgba(48, 83, 111, 255);
    color_table[NK_COLOR_SLIDER] = nk_rgba(50, 58, 61, 255);
    color_table[NK_COLOR_SLIDER_CURSOR] = nk_rgb(153, 0, 0);
    color_table[NK_COLOR_SLIDER_CURSOR_HOVER] = nk_rgba(153, 0, 0, 245);
    color_table[NK_COLOR_SLIDER_CURSOR_ACTIVE] = nk_rgba(153, 0, 0, 245);
    color_table[NK_COLOR_PROPERTY] = nk_rgba(50, 58, 61, 255);
    color_table[NK_COLOR_EDIT] = nk_rgb(37, 37, 37);
    color_table[NK_COLOR_EDIT_CURSOR] = nk_rgba(210, 210, 210, 255);
    color_table[NK_COLOR_COMBO] = nk_rgba(50, 58, 61, 255);
    color_table[NK_COLOR_CHART] = nk_rgb(30, 0, 0);
    color_table[NK_COLOR_CHART_COLOR] = nk_rgb(153, 0, 0);
    color_table[NK_COLOR_CHART_COLOR_HIGHLIGHT] = nk_rgb(153, 0, 0);
    color_table[NK_COLOR_SCROLLBAR] = nk_rgba(50, 58, 61, 255);
    color_table[NK_COLOR_SCROLLBAR_CURSOR] = nk_rgba(48, 83, 111, 255);
    color_table[NK_COLOR_SCROLLBAR_CURSOR_HOVER] = nk_rgba(53, 88, 116, 255);
    color_table[NK_COLOR_SCROLLBAR_CURSOR_ACTIVE] = nk_rgba(58, 93, 121, 255);
    color_table[NK_COLOR_TAB_HEADER] = nk_rgb(0, 0, 0);
    
    nk_style_from_table(ctx, color_table);
}