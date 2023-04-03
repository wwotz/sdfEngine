#include "../include/sdfE.h"

GLfloat sdfe_model[16], sdfe_view[16], sdfe_projection[16];

static SDL_Window *sdfe_window = NULL;
static SDL_GLContext *sdfe_context = NULL;
static SDL_Event sdfe_event = {0};
static int sdfe_window_w = 0, sdfe_window_h = 0;
static int sdfe_running = 0;

int sdfe_window_init(const char *name, int x, int y, int w, int h,
                     int flags)
{
        int img_flags, status;
        if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
                SDFE_PUSH_ERROR(SDL_GetError());
                return SDFE_WINDOW_SDL_ERROR;
        }

        img_flags = IMG_INIT_PNG | IMG_INIT_JPG;
        if (IMG_Init(img_flags) != img_flags) {
                SDFE_PUSH_ERROR(IMG_GetError());
                return SDFE_WINDOW_IMG_ERROR;
        }

        if (TTF_Init() != 0) {
                SDFE_PUSH_ERROR(TTF_GetError());
                return SDFE_WINDOW_TTF_ERROR;
        }

        sdfe_window = SDL_CreateWindow(name, x, y, w, h, flags);
        if (!sdfe_window) {
                SDFE_PUSH_ERROR("Failed to initialise window!");
                return SDFE_WINDOW_WINDOW_ERROR;
        }

        sdfe_context = SDL_GL_CreateContext(sdfe_window);
        if (!sdfe_context) {
                SDFE_PUSH_ERROR("Failed to initialise context!");
                return SDFE_WINDOW_CONTEXT_ERROR;
        }

        status = glewInit();
        if (status != GLEW_OK) {
                SDFE_PUSH_ERROR("Failed to initialise GLEW!");
                return SDFE_WINDOW_GLEW_ERROR;
        }

        glClearColor(0.0, 0.0, 0.0, 1.0);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glEnable(GL_TEXTURE_2D);

        SDL_GetWindowSize(sdfe_window, &sdfe_window_w, &sdfe_window_h);

        sdfe_matrix_identity(sdfe_model);
        sdfe_matrix_identity(sdfe_view);
        sdfe_matrix_orthographic(sdfe_projection, 0.0, sdfe_window_get_width(), sdfe_window_get_height(), 0.0, 10.0, -10.0);

        glViewport(0.0, 0.0, sdfe_window_get_width(), sdfe_window_get_height());

        sdfe_running = 1;
        return SDFE_WINDOW_NO_ERROR;
}

void sdfe_window_events(void)
{
        switch (sdfe_window_event_type()) {
                case SDL_QUIT:
                        sdfe_window_set_running(0);
                        break;
        }
}

void sdfe_window_clear_buffers(GLbitfield buffers)
{
        glClear(buffers);
}

void sdfe_window_swap(void)
{
        SDL_GL_SwapWindow(sdfe_window);
}

int sdfe_window_running(void)
{
        return sdfe_running;
}

int sdfe_window_poll_event(void)
{
        return SDL_PollEvent(&sdfe_event);
}

int sdfe_window_event_type(void)
{
        return sdfe_event.type;
}

int sdfe_window_get_running(void)
{
        return sdfe_running;
}

GLfloat sdfe_window_get_width(void)
{
        return (GLfloat) sdfe_window_w;
}

GLfloat sdfe_window_get_height(void)
{
        return (GLfloat) sdfe_window_h;
}

void sdfe_window_set_running(int running)
{
        sdfe_running = running;
}

void sdfe_window_free(void)
{
        SDL_DestroyWindow(sdfe_window);
        SDL_GL_DeleteContext(sdfe_context);

        sdfe_window = NULL;
        sdfe_context = NULL;
        sdfe_window_w = 0;
        sdfe_window_h = 0;
        sdfe_running = 0;

        SDL_Quit();
        TTF_Quit();
        IMG_Quit();
}
