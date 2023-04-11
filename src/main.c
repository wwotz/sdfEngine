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

static GLuint canvas_program = 0;
static sdfe_rect_t canvas;
static char *canvas_path = NULL;

static void sdfe_response_exit(void)
{
        if (sdfe_debug_had_error()) {
                fprintf(stderr, "%s\n", sdfe_debug_stack_pop());
        }
        sdfe_window_set_running(0);
}

static void sdfe_response_load_shader(void)
{
        GLuint new_program;
        char *buf = sdfe_server_get_message();

        sdfe_program_info_t new_pinfo = {
                { SDFE_SHADER_SOURCE, GL_VERTEX_SHADER, default_vert_src },
                { SDFE_SHADER_FILE, GL_FRAGMENT_SHADER, buf }
        };
        new_program = sdfe_program_create(new_pinfo);
        if (sdfe_debug_had_error()) {
                fprintf(stderr, "%s\n", sdfe_debug_stack_pop());
        } else {
                if (!canvas_path) {
                        free(canvas_path);
                }
                canvas_path = strdup(buf);
                glDeleteProgram(canvas_program);
                canvas_program = new_program;
                sdfe_rect_set_program(&canvas, canvas_program);
        }
}

static void sdfe_response_refresh_shader(void)
{
        GLuint new_program;
        if (!canvas_path)
                return;

        sdfe_program_info_t new_pinfo = {
                { SDFE_SHADER_SOURCE, GL_VERTEX_SHADER, default_vert_src },
                { SDFE_SHADER_FILE, GL_FRAGMENT_SHADER, canvas_path }
        };
        new_program = sdfe_program_create(new_pinfo);
        if (sdfe_debug_had_error()) {
                fprintf(stderr, "%s\n", sdfe_debug_stack_pop());
        } else {
                glDeleteProgram(canvas_program);
                canvas_program = new_program;
                sdfe_rect_set_program(&canvas, canvas_program);
        }
}

static void sdfe_response_reset_timer(void)
{
        sdfe_timer_reset();
}

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

        sdfe_program_info_t canvas_pinfo = {
                { SDFE_SHADER_SOURCE, GL_VERTEX_SHADER, default_vert_src },
                { SDFE_SHADER_SOURCE, GL_FRAGMENT_SHADER, default_frag_src },
        };
        canvas_program = sdfe_program_create(canvas_pinfo);

        if (sdfe_debug_had_error()) {
                fprintf(stderr, "%s\n", sdfe_debug_stack_pop());
                exit(EXIT_FAILURE);
        }

        sdfe_rect_init(&canvas, 0.0, 0.0, sdfe_window_get_width(),
                       sdfe_window_get_height(),
                       canvas_program, 0, 0xffffffff);
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
                                                        sdfe_rect_set_width(&canvas, sdfe_window_get_width());
                                                        sdfe_rect_set_height(&canvas, sdfe_window_get_height());
                                                        break;
                                        }
                                        break;
                        }
                }

                sdfe_server_code_t server_code = sdfe_server_recv();
                switch (server_code) {
                case SDFE_SERVER_CODE_END:
                        sdfe_response_exit();
                        break;
                case SDFE_SERVER_CODE_LOAD_SHADER:
                        sdfe_response_load_shader();
                        break;
                case SDFE_SERVER_CODE_REFRESH_SHADER:
                        sdfe_response_refresh_shader();
                        break;
                case SDFE_SERVER_CODE_RESET_TIMER:
                        sdfe_response_reset_timer();
                        break;
                default:
                        break;
                }

                sdfe_window_clear_buffers(GL_COLOR_BUFFER_BIT);
                sdfe_rect_render(&canvas);
                sdfe_window_swap();
        }

        sdfe_server_close();
        sdfe_rect_free(&canvas);
        glDeleteProgram(canvas_program);
        sdfe_window_free();
        return 0;
}
