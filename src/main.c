#include "../include/sdfE.h"

int main(int argc, char **argv)
{
        char *title = malloc(128);
        snprintf(title, 128, "sdfEngine %d", getpid());
        sdfe_window_init(title,
                         SDFE_WINDOW_X,
                         SDFE_WINDOW_Y,
                         SDFE_WINDOW_W,
                         SDFE_WINDOW_H,
                         SDFE_WINDOW_FLAGS);
        free(title);
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
        sdfe_rect_init(&rect, 50.0, 50.0, sdfe_window_get_width() * 0.4,
                       sdfe_window_get_height() * 0.4,
                       rect_program, 0, 0xffffffff);
        if (sdfe_debug_had_error()) {
                fprintf(stderr, "%s\n", sdfe_debug_stack_pop());
                exit(EXIT_FAILURE);
        }

        sdfe_window_set_clear_colour(0x222222ff);

        while (sdfe_window_get_running()) {
                while (sdfe_window_poll_event()) {
                        sdfe_window_events();
                        switch (sdfe_window_event_type()) {
                                case SDL_KEYDOWN:
                                        switch (sdfe_window_event_keycode()) {
                                                case SDLK_F1:
                                                        glDeleteProgram(rect_program);
                                                        rect_program = sdfe_program_create(rect_pinfo);
                                                        rect.program = rect_program;
                                                        break;
                                        }
                                        break;
                        }
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
