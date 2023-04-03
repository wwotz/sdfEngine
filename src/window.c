#include "../include/sdfE.h"

GLfloat sdfe_model[16], sdfe_view[16], sdfe_projection[16];

static SDL_Window *sdfe_window;
static SDL_GLContext *sdfe_context;
static int sdfe_window_w, sdfe_window_h;
static int sdfe_running;

int sdfe_window_init(const char *name, int x, int y, int w, int h,
                     int flags)
{
        int img_flags;
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

        return SDFE_WINDOW_NO_ERROR;
}

void sdfe_window_free(void)
{

}
