#include "signal-processing.h"
#include "gui-helper.c"
#include "custom-elements.c"
#include "styles.c"
#include "signal-helper.c"
#include "file-manager.c"

void
gui_begin(
    struct nk_context *ctx,
    signal_array *signals_array,
    unsigned int *signal_plot_scale,
    unsigned int *signal_plot_offset,
    unsigned int *selected_signal,
    int x, 
    int y, 
    int width, 
    int height
) 
{
    if (nk_begin(
        ctx, "", nk_rect(x, y, width, height), NK_WINDOW_NO_SCROLLBAR
    )) {
        struct nk_command_buffer *canvas = nk_window_get_canvas(ctx);

        nk_menubar_begin(ctx);
        nk_layout_row_static(ctx, 30.0f, 60, 3);
        
        static file_ext file_ext;
        static int show_save_file = nk_false;
        static int show_open_file = nk_false;

        if (nk_menu_begin_label(ctx, "Open", NK_TEXT_ALIGN_LEFT | NK_TEXT_ALIGN_MIDDLE, nk_vec2(110.0f, 120.0f))) {
            nk_layout_row_dynamic(ctx, 25, 1);
            if (nk_menu_item_label(ctx, "WAV file", NK_TEXT_ALIGN_LEFT | NK_TEXT_ALIGN_MIDDLE)) {
                file_ext = WAV;
                show_open_file = nk_true;
            }
                
            if (nk_menu_item_label(ctx, "CSV file", NK_TEXT_ALIGN_LEFT | NK_TEXT_ALIGN_MIDDLE)) {
                file_ext = CSV;
                show_open_file = nk_true;
            }
            
            if (nk_menu_item_label(ctx, "JSON file", NK_TEXT_ALIGN_LEFT | NK_TEXT_ALIGN_MIDDLE)) {
                file_ext = JSON;
                show_open_file = nk_true;
            }

            nk_menu_end(ctx);
        }
        if (nk_menu_begin_label(ctx, "Save", NK_TEXT_ALIGN_LEFT | NK_TEXT_ALIGN_MIDDLE, nk_vec2(110.0f, 120.0f))) {
            nk_layout_row_dynamic(ctx, 25, 1);
            if (nk_menu_item_label(ctx, "WAV file", NK_TEXT_ALIGN_LEFT | NK_TEXT_ALIGN_MIDDLE)) {
                file_ext = WAV;
                show_save_file = nk_true;
            }
            if (nk_menu_item_label(ctx, "CSV file", NK_TEXT_ALIGN_LEFT | NK_TEXT_ALIGN_MIDDLE)) {
                file_ext = CSV;
                show_save_file = nk_true;
            }
            if (nk_menu_item_label(ctx, "JSON file", NK_TEXT_ALIGN_LEFT | NK_TEXT_ALIGN_MIDDLE)) {
                file_ext = JSON;
                show_save_file = nk_true;
            }
            nk_menu_end(ctx);
        }

        if (show_open_file)
        {
            struct nk_rect size = {65, 35, WINDOW_WIDTH / 2, 120};
            static int path_length;
            static char path[4097];
            
            if (nk_popup_begin(ctx, NK_POPUP_STATIC, "Open file", NK_WINDOW_CLOSABLE, size))
            {
                nk_layout_row_dynamic(ctx, 30, 1);
                nk_edit_string(ctx, NK_EDIT_SIMPLE, path, &path_length, 4097, nk_filter_default);
                nk_layout_row_dynamic(ctx, 30, 1);
                if(nk_button_label(ctx, "Open")) {
                    nk_popup_close(ctx);
                    show_open_file = nk_false;
                    if (path_length > 0)
                        open_from_file(signals_array, path, file_ext);
                }
                nk_popup_end(ctx);
            } else show_open_file = nk_false;
        }

        if (show_save_file)
        {
            struct nk_rect size = {125, 35, WINDOW_WIDTH / 2, 130};
            static int path_length;
            static char path[4097];
            
            if (nk_popup_begin(ctx, NK_POPUP_STATIC, "Save file", NK_WINDOW_CLOSABLE, size))
            {
                if (signals_array->signals == NULL || signals_array->size < 1) {
                    nk_layout_row_dynamic(ctx, 35, 1);
                    nk_label(ctx, "Please add some signals to save!", NK_TEXT_ALIGN_CENTERED | NK_TEXT_ALIGN_MIDDLE);
                    nk_layout_row_dynamic(ctx, 30, 1);
                    if (nk_button_label(ctx, "Ok!")) {
                        nk_popup_close(ctx);
                        show_save_file = nk_false;
                    }
                }
                else {
                    nk_layout_row_dynamic(ctx, 30, 1);
                    nk_edit_string(ctx, NK_EDIT_SIMPLE, path, &path_length, 4097, nk_filter_default);
                    nk_layout_row_dynamic(ctx, 30, 1);
                    if(nk_button_label(ctx, "Save")) {
                        nk_popup_close(ctx);
                        show_save_file = nk_false;
                        if (path_length > 0)
                            save_to_file(signals_array, path, file_ext);
                    }
                }
                nk_popup_end(ctx);
            } else show_save_file = nk_false;
        }

        static int show_app_about = nk_false;
        if (nk_button_label(ctx, "About"))
            show_app_about = nk_true;

        if (show_app_about)
        {
            struct nk_rect size = {200, 5, 300, 190};
            if (nk_popup_begin(ctx, NK_POPUP_STATIC, "About", NK_WINDOW_CLOSABLE, size))
            {
                nk_layout_row_dynamic(ctx, 20, 1);
                nk_label(ctx, "Signal Processing", NK_TEXT_ALIGN_CENTERED | NK_TEXT_ALIGN_MIDDLE);
                nk_layout_row_dynamic(ctx, 20, 1);
                nk_label(ctx, "By Dmytro Poltoratskyi", NK_TEXT_ALIGN_LEFT | NK_TEXT_ALIGN_MIDDLE);
                nk_layout_row_dynamic(ctx, 20, 1);
                nk_label(ctx, 
                    "Software", 
                    NK_TEXT_ALIGN_LEFT | NK_TEXT_ALIGN_MIDDLE);
                nk_popup_end(ctx);
            } else show_app_about = nk_false;
        }
        
        nk_menubar_end(ctx);
        
        nk_layout_row_dynamic(ctx, 35.0f, 1);
        nk_label(ctx, "Signal Preview", NK_TEXT_ALIGN_CENTERED | NK_TEXT_ALIGN_MIDDLE);

        nk_layout_row_dynamic(ctx, 300.0f, 1);
        if (signals_array->signals != NULL) 
            ui_signal_plot(ctx, signals_array, *signal_plot_scale, *signal_plot_offset, *selected_signal);

        nk_layout_row_dynamic(ctx, 35.0f, 1);
        nk_label(ctx, "Settings", NK_TEXT_ALIGN_CENTERED | NK_TEXT_ALIGN_MIDDLE);

        nk_layout_row_dynamic(ctx, 350.0f, 3);
        nk_group_begin(ctx, "", NK_WINDOW_BORDER);
        if (nk_tree_push(ctx, NK_TREE_TAB, "Signal Management", NK_MAXIMIZED)) {
            static int show_add_signal = nk_false;
            if (nk_button_label(ctx, "Add signal"))
                show_add_signal = nk_true;

            if (show_add_signal)
            {
                static struct nk_rect size = {0, 0, WINDOW_WIDTH / 3 - 18, 348};
                
                if (nk_popup_begin(ctx, NK_POPUP_STATIC, "Add signal", NK_WINDOW_CLOSABLE, size))
                {
                    unsigned int frequency = 1000, amplitude = 10000, duration = 1, bits_per_sample = 16, sample_rate = 44100;
                    float phase = 0;
                    static char text_values[6][11];
                    static int text_values_length[6];
                    static int option = Sine;
                    nk_layout_row_dynamic(ctx, 30, 4);
                    if (nk_button_symbol_label(ctx, (option == Sine)?NK_SYMBOL_CIRCLE_OUTLINE:NK_SYMBOL_CIRCLE_SOLID, "Sine", NK_TEXT_LEFT))
                        option = Sine;
                    if (nk_button_symbol_label(ctx, (option == Triangle)?NK_SYMBOL_CIRCLE_OUTLINE:NK_SYMBOL_CIRCLE_SOLID, "Triangle", NK_TEXT_LEFT))
                        option = Triangle;
                    if (nk_button_symbol_label(ctx, (option == Square)?NK_SYMBOL_CIRCLE_OUTLINE:NK_SYMBOL_CIRCLE_SOLID, "Square", NK_TEXT_LEFT))
                        option = Square;
                    if (nk_button_symbol_label(ctx, (option == Sawtooth)?NK_SYMBOL_CIRCLE_OUTLINE:NK_SYMBOL_CIRCLE_SOLID, "Sawtooth", NK_TEXT_LEFT))
                        option = Sawtooth;

                    nk_layout_row_dynamic(ctx, 30, 2);
                    nk_label(ctx, "Frequency (Hz)", NK_TEXT_ALIGN_LEFT | NK_TEXT_ALIGN_MIDDLE);
                    nk_edit_string(ctx, NK_EDIT_SIMPLE, text_values[0], &text_values_length[0], 11, nk_filter_decimal);
                    
                    nk_layout_row_dynamic(ctx, 30, 2);
                    nk_label(ctx, "Amplitude", NK_TEXT_ALIGN_LEFT | NK_TEXT_ALIGN_MIDDLE);
                    nk_edit_string(ctx, NK_EDIT_SIMPLE, text_values[1], &text_values_length[1], 11, nk_filter_decimal);
                    
                    nk_layout_row_dynamic(ctx, 30, 2);
                    nk_label(ctx, "Phase", NK_TEXT_ALIGN_LEFT | NK_TEXT_ALIGN_MIDDLE);
                    nk_edit_string(ctx, NK_EDIT_SIMPLE, text_values[2], &text_values_length[2], 11, nk_filter_float);

                    nk_layout_row_dynamic(ctx, 30, 2);
                    nk_label(ctx, "Duration (s)", NK_TEXT_ALIGN_LEFT | NK_TEXT_ALIGN_MIDDLE);
                    nk_edit_string(ctx, NK_EDIT_SIMPLE, text_values[3], &text_values_length[3], 11, nk_filter_decimal);
                    
                    nk_layout_row_dynamic(ctx, 30, 2);
                    nk_label(ctx, "Bits per sample (Bits)", NK_TEXT_ALIGN_LEFT | NK_TEXT_ALIGN_MIDDLE);
                    nk_edit_string(ctx, NK_EDIT_SIMPLE, text_values[4], &text_values_length[4], 11, nk_filter_decimal);
                    
                    nk_layout_row_dynamic(ctx, 30, 2);
                    nk_label(ctx, "Sample rate (Hz)", NK_TEXT_ALIGN_LEFT | NK_TEXT_ALIGN_MIDDLE);
                    nk_edit_string(ctx, NK_EDIT_SIMPLE, text_values[5], &text_values_length[5], 11, nk_filter_decimal);

                    nk_layout_row_dynamic(ctx, 30, 1);
                    if (nk_button_label(ctx, "Add")) {
                        nk_popup_close(ctx);
                        show_add_signal = nk_false;
                        frequency = atoi(text_values[0]);
                        amplitude = atoi(text_values[1]);
                        phase = atof(text_values[2]);
                        duration = atoi(text_values[3]);
                        bits_per_sample = atoi(text_values[4]);
                        sample_rate = atoi(text_values[5]);
                        add_signal(signals_array, option, frequency, amplitude, phase, duration, bits_per_sample, sample_rate);
                        *selected_signal = signals_array->size - 1;
                        *signal_plot_offset = 0;
                    }
                    nk_popup_end(ctx);
                } else show_add_signal = nk_false;
            }
            
            nk_layout_row_dynamic(ctx, 30.0f, 1);
            nk_label(ctx, "Signals list", NK_TEXT_ALIGN_CENTERED | NK_TEXT_ALIGN_BOTTOM);
            ui_signal_list(ctx, signals_array, selected_signal);
            nk_tree_pop(ctx);
        }
        nk_group_end(ctx);
        
        nk_group_begin(ctx, "", NK_WINDOW_BORDER);
        if (nk_tree_push(ctx, NK_TREE_TAB, "Plot Management", NK_MAXIMIZED)) {
            nk_layout_row_dynamic(ctx, 30.0f, 1);
            nk_label(ctx, "Preview scale", NK_TEXT_ALIGN_CENTERED | NK_TEXT_ALIGN_BOTTOM);
            nk_slider_int(ctx, 100, signal_plot_scale, 1000, 100);
            nk_layout_row_dynamic(ctx, 30.0f, 1);
            if (signals_array->signals != NULL) {
                nk_label(ctx, "Preview offset", NK_TEXT_ALIGN_CENTERED | NK_TEXT_ALIGN_BOTTOM);
                nk_slider_int(ctx, 0, signal_plot_offset, signals_array->signals[*selected_signal].size - *signal_plot_scale, *signal_plot_scale);
            }
            nk_tree_pop(ctx);
        }
        nk_group_end(ctx);

        static int show_mix_signal = nk_false;
        nk_group_begin(ctx, "", NK_WINDOW_BORDER);
        if (nk_tree_push(ctx, NK_TREE_TAB, "Actions", NK_MAXIMIZED)) {
            if (nk_button_label(ctx, "Mix signals"))
                show_mix_signal = nk_true;
            
            if (show_mix_signal)
            {
                static struct nk_rect size = {0, 0, WINDOW_WIDTH / 3 - 18, 348};
                if (nk_popup_begin(ctx, NK_POPUP_STATIC, "Mix signals", NK_WINDOW_CLOSABLE, size))
                {
                    if (signals_array->signals == NULL) {
                        nk_layout_row_dynamic(ctx, 35, 1);
                        nk_label(ctx, "Please add some signals to mix!", NK_TEXT_ALIGN_CENTERED | NK_TEXT_ALIGN_MIDDLE);
                        nk_layout_row_dynamic(ctx, 30, 1);
                        if (nk_button_label(ctx, "Ok!")) {
                            nk_popup_close(ctx);
                            show_mix_signal = nk_false;
                        }
                    }
                    else if (signals_array->size < 2) {
                        nk_layout_row_dynamic(ctx, 35, 1);
                        nk_label(ctx, "Please add one or more signals to mix!", NK_TEXT_ALIGN_CENTERED | NK_TEXT_ALIGN_MIDDLE);
                        nk_layout_row_dynamic(ctx, 30, 1);
                        if (nk_button_label(ctx, "Ok!")) {
                            nk_popup_close(ctx);
                            show_mix_signal = nk_false;
                        }
                    }
                    else {
                        unsigned int duration = 1, bits_per_sample = 16, sample_rate = 44100;
                        static char text_values[3][11];
                        static int text_values_length[3];

                        nk_layout_row_dynamic(ctx, 30, 2);
                        nk_label(ctx, "Duration (s)", NK_TEXT_ALIGN_LEFT | NK_TEXT_ALIGN_MIDDLE);
                        nk_edit_string(ctx, NK_EDIT_SIMPLE, text_values[0], &text_values_length[0], 11, nk_filter_decimal);
                        
                        nk_layout_row_dynamic(ctx, 30, 2);
                        nk_label(ctx, "Bits per sample (Bits)", NK_TEXT_ALIGN_LEFT | NK_TEXT_ALIGN_MIDDLE);
                        nk_edit_string(ctx, NK_EDIT_SIMPLE, text_values[1], &text_values_length[1], 11, nk_filter_decimal);
                        
                        nk_layout_row_dynamic(ctx, 30, 2);
                        nk_label(ctx, "Sample rate (Hz)", NK_TEXT_ALIGN_LEFT | NK_TEXT_ALIGN_MIDDLE);
                        nk_edit_string(ctx, NK_EDIT_SIMPLE, text_values[2], &text_values_length[2], 11, nk_filter_decimal);

                        nk_layout_row_dynamic(ctx, 30, 1);
                        if (nk_button_label(ctx, "Mix signals")) {
                            nk_popup_close(ctx);
                            show_mix_signal = nk_false;
                            duration = atoi(text_values[0]);
                            bits_per_sample = atoi(text_values[1]);
                            sample_rate = atoi(text_values[2]);
                            mix_signals(signals_array, duration, bits_per_sample, sample_rate);
                            *selected_signal = 0;
                        }
                    }
                    
                    nk_popup_end(ctx);
                } else show_mix_signal = nk_false;
            }
            nk_tree_pop(ctx);
        }
        nk_group_end(ctx);
    }
}

void
gui_end(
    struct nk_context *ctx
) 
{
    nk_end(ctx);
}

int 
main(
    int argc, 
    char *argv[]
)
{
    signal_array signals_array;
    unsigned int signal_plot_scale = 500, signal_plot_offset = 0;
    unsigned int selected_signal = 0;

    static GLFWwindow *win;
    int width = 0, height = 0;
    const void *image; 
    int w, h;

    struct device device;
    struct nk_font_atlas atlas;
    struct nk_context ctx;
    struct nk_font *font;

    /* GLFW */
    glfwSetErrorCallback(error_callback);
    if (!glfwInit()) {
        fprintf(stdout, "[GFLW] failed to init!\n");
        exit(1);
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    win = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Signal Processing", NULL, NULL);
    glfwMakeContextCurrent(win);
    glfwSetWindowUserPointer(win, &ctx);
    glfwSetCharCallback(win, text_input);
    glfwSetScrollCallback(win, scroll_input);
    glfwGetWindowSize(win, &width, &height);

    /* OpenGL */
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    glewExperimental = 1;
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to setup GLEW\n");
        exit(1);
    }

    device_init(&device);

    nk_font_atlas_init_default(&atlas);
    nk_font_atlas_begin(&atlas);
    font = nk_font_atlas_add_from_file(&atlas, "./assets/fonts/Roboto-Regular.ttf", 18, 0);
    image = nk_font_atlas_bake(&atlas, &w, &h, NK_FONT_ATLAS_RGBA32);
    device_upload_atlas(&device, image, w, h);
    nk_font_atlas_end(&atlas, nk_handle_id((int)device.font_tex), &device.null);

    nk_init_default(&ctx, &font->handle);

    glEnable(GL_TEXTURE_2D);

    set_style(&ctx);

    while (!glfwWindowShouldClose(win)) {
        pump_input(&ctx, win);
        gui_begin(&ctx, &signals_array, &signal_plot_scale, &signal_plot_offset, &selected_signal, 0, 0, width, height);
        
        gui_end(&ctx);
        /* Draw */
        glfwGetWindowSize(win, &width, &height);
        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
        device_draw(&device, &ctx, width, height, NK_ANTI_ALIASING_ON);
        glfwSwapBuffers(win);
    }

    free(signals_array.signals);
    free(signals_array.signals_data);
    nk_font_atlas_clear(&atlas);
    nk_free(&ctx);
    device_shutdown(&device);
    glfwTerminate();

    return 0;
}