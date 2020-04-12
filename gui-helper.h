#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define NK_PRIVATE
#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_DEFAULT_ALLOCATOR // Allow to use nk_init_default()
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT
#define NK_IMPLEMENTATION // Enable implementation mode
#include "./nuklear/nuklear.h"

#define UNUSED(a) (void)a
#define NK_SHADER_VERSION "#version 150\n"
#define MAX_VERTEX_MEMORY 512 * 1024
#define MAX_ELEMENT_MEMORY 128 * 1024

struct 
device {
    struct nk_buffer cmds;
    struct nk_draw_null_texture null;
    GLuint vbo, vao, ebo;
    GLuint prog;
    GLuint vert_shdr;
    GLuint frag_shdr;
    GLint attrib_pos;
    GLint attrib_uv;
    GLint attrib_col;
    GLint uniform_tex;
    GLint uniform_proj;
    GLuint font_tex;
};

struct 
nk_glfw_vertex {
    float position[2];
    float uv[2];
    nk_byte col[4];
};

void
die(
    const char *fmt, 
    ...
);

void
device_init(
    struct device *dev
);

void
device_upload_atlas(
    struct device *dev, 
    const void *image, 
    int width, 
    int height
);

void
device_shutdown(
    struct device *dev
);

void
device_draw(
    struct device *dev, 
    struct nk_context *ctx, 
    int width, 
    int height, 
    enum nk_anti_aliasing AA
);

void 
error_callback(
    int e, 
    const char *d
);

void 
text_input(
    GLFWwindow *win, 
    unsigned int codepoint
);

void 
scroll_input(
    GLFWwindow *win, 
    double _, 
    double yoff
);

void
pump_input(
    struct nk_context *ctx, 
    GLFWwindow *win
);