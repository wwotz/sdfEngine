#include "../include/sdfE.h"

int main(int argc, char **argv)
{
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
                { SDFE_SHADER_FILE, GL_VERTEX_SHADER, "../shaders/yours.vert" },
                { SDFE_SHADER_FILE, GL_FRAGMENT_SHADER, "../shaders/yours.frag" },
        };
        GLuint rect_program = sdfe_program_create(rect_pinfo);
        if (sdfe_debug_had_error()) {
                fprintf(stderr, "%s\n", sdfe_debug_stack_pop());
                exit(EXIT_FAILURE);
        }

        sdfe_rect_t rect;
        sdfe_rect_init(&rect, 0.0, 0.0, sdfe_window_get_width(), sdfe_window_get_height(),
                       rect_program, 0, 0xffffffff);
        if (sdfe_debug_had_error()) {
                fprintf(stderr, "%s\n", sdfe_debug_stack_pop());
                exit(EXIT_FAILURE);
        }

        while (sdfe_window_get_running()) {
                while (sdfe_window_poll_event()) {
                        sdfe_window_events();
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
