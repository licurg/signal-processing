#include "gui-helper.c"
#include "styles.c"
#include "custom-elements.c"

#define WINDOW_WIDTH 1200
#define WINDOW_HEIGHT 800

static void
gui_begin(struct nk_context *ctx, int x, int y, int width, int height) {
    if (nk_begin(
        ctx, "", nk_rect(x, y, width, height), NK_WINDOW_NO_SCROLLBAR
    )) {
        struct nk_command_buffer *canvas = nk_window_get_canvas(ctx);

        nk_menubar_begin(ctx);
        nk_layout_row_static(ctx, 30.0f, 50, 3);

        if (nk_menu_begin_label(ctx, "Open", NK_TEXT_ALIGN_LEFT | NK_TEXT_ALIGN_MIDDLE, nk_vec2(110.0f, 120.0f))) {
            nk_layout_row_dynamic(ctx, 25, 1);
            if (nk_menu_item_label(ctx, "WAW file", NK_TEXT_ALIGN_LEFT | NK_TEXT_ALIGN_MIDDLE))
                printf("WAW open\n");
            if (nk_menu_item_label(ctx, "CSV file", NK_TEXT_ALIGN_LEFT | NK_TEXT_ALIGN_MIDDLE))
                printf("CSV open\n");
            nk_menu_end(ctx);
        }
        if (nk_menu_begin_label(ctx, "Save", NK_TEXT_ALIGN_LEFT | NK_TEXT_ALIGN_MIDDLE, nk_vec2(110.0f, 120.0f))) {
            nk_layout_row_dynamic(ctx, 25, 1);
            if (nk_menu_item_label(ctx, "WAW file", NK_TEXT_ALIGN_LEFT | NK_TEXT_ALIGN_MIDDLE))
                printf("WAW save\n");
            if (nk_menu_item_label(ctx, "CSV file", NK_TEXT_ALIGN_LEFT | NK_TEXT_ALIGN_MIDDLE))
                printf("CSV save\n");
            nk_menu_end(ctx);
        }
        if (nk_button_label(ctx, "About"))
            printf("About\n");
        nk_menubar_end(ctx);
        
        nk_layout_row_dynamic(ctx, 35.0f, 1);
        nk_label(ctx, "Signal Preview", NK_TEXT_ALIGN_CENTERED | NK_TEXT_ALIGN_MIDDLE);

        nk_layout_row_dynamic(ctx, 400.0f, 1);
        ui_line_chart(ctx);

        nk_layout_row_dynamic(ctx, 35.0f, 1);
        nk_label(ctx, "Settings", NK_TEXT_ALIGN_CENTERED | NK_TEXT_ALIGN_MIDDLE);

        nk_layout_row_dynamic(ctx, 300.0f, 3);
        ui_piemenu(ctx, canvas, 30.0f, 530.0f, 240);
    }
}

static void
gui_end(struct nk_context *ctx) {
    nk_end(ctx);
}

int main(int argc, char *argv[])
{
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

    while (!glfwWindowShouldClose(win)) {
        pump_input(&ctx, win);
        gui_begin(&ctx, 0, 0, width, height);
        
        gui_end(&ctx);
        /* Draw */
        glfwGetWindowSize(win, &width, &height);
        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
        device_draw(&device, &ctx, width, height, NK_ANTI_ALIASING_ON);
        glfwSwapBuffers(win);
    }

    nk_font_atlas_clear(&atlas);
    nk_free(&ctx);
    device_shutdown(&device);
    glfwTerminate();

    return 0;
}