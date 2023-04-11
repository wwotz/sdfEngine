#include "../include/sdfE.h"

static char *default_vert_src = "#version 330 core\n"
        "uniform mat4 model;\n"
        "uniform mat4 projection;\n"
        "uniform mat4 view;\n"
        "layout (location = 0) in vec2 vertex;\n"
        "layout (location = 1) in vec2 t_vertex;\n"
        "out vec2 tex_vertex;\n"
        "void main()\n"
        "{\n"
        "tex_vertex = t_vertex;\n"
        "gl_Position = projection * view * model * vec4(vertex, 0.0, 1.0);\n"
        "}";

static char *default_frag_src = "#version 330 core\n"
        "uniform sampler2D texture_unit;\n"
        "uniform vec4 colour;\n"
        "in vec2 tex_vertex;\n"
        "uniform float time;\n"
        "void main()\n"
        "{\n"
        "vec2 uv = vec2(tex_vertex - vec2(0.5));\n"
        "vec3 col = smoothstep(0.0, 0.1, vec3(length(uv)));\n"
        "gl_FragColor = vec4(col, 1.0);\n"
        "}";

int main(int argc, char **argv)
{
        //returns new file descriptor to communicate on.
        if (sdfe_server_start() != 0) {
                fprintf(stderr, "%s\n", sdfe_debug_stack_pop());
                exit(EXIT_FAILURE);
        }

        sdfe_window_init("sdfEngine",
                         SDFE_WINDOW_X,
                         SDFE_WINDOW_Y,
                         SDFE_WINDOW_W,
                         SDFE_WINDOW_H,
                         SDFE_WINDOW_FLAGS);
         if (sdfe_debug_had_error()) {
                fprintf(stderr, "%s\n", sdfe_debug_stack_pop());
                exit(EXIT_FAILURE);
        }

        sdfe_program_info_t rect_pinfo = {
                { SDFE_SHADER_SOURCE, GL_VERTEX_SHADER, default_vert_src },
                { SDFE_SHADER_SOURCE, GL_FRAGMENT_SHADER, default_frag_src },
        };
        GLuint rect_program = sdfe_program_create(rect_pinfo);
        if (sdfe_debug_had_error()) {
                fprintf(stderr, "%s\n", sdfe_debug_stack_pop());
                exit(EXIT_FAILURE);
        }

        sdfe_rect_t rect;
        sdfe_rect_init(&rect, 0.0, 0.0, sdfe_window_get_width(),
                       sdfe_window_get_height(),
                       rect_program, 0, 0xffffffff);
        if (sdfe_debug_had_error()) {
                fprintf(stderr, "%s\n", sdfe_debug_stack_pop());
                exit(EXIT_FAILURE);
        }

        sdfe_window_set_clear_colour(0x222222ff);

        sdfe_timer_start();
        while (sdfe_window_get_running()) {
                while (sdfe_window_poll_event()) {
                        sdfe_window_events();
                        switch (sdfe_window_event_type()) {
                                case SDL_WINDOWEVENT:
                                        switch (sdfe_window_event_window()) {
                                                case SDL_WINDOWEVENT_RESIZED:
                                                        sdfe_rect_set_width(&rect, sdfe_window_get_width());
                                                        sdfe_rect_set_height(&rect, sdfe_window_get_height());
                                                        break;
                                        }
                                        break;
                        }
                }
                sdfe_server_code_t server_code = sdfe_server_recv();
                switch (server_code) {
                        case SDFE_SERVER_END:
                                sdfe_window_set_running(0);
                                break;
                        case SDFE_SERVER_READ:
                                break;
                }
                sdfe_window_clear_buffers(GL_COLOR_BUFFER_BIT);
                sdfe_rect_render(&rect);
                sdfe_window_swap();
        }

        sdfe_rect_free(&rect);
        glDeleteProgram(rect_program);
        sdfe_window_free();
        return 0;
}
