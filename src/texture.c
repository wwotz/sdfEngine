#include "../include/sdfE.h"

sdfe_texture_info_t sdfe_texture_info_create(SDFE_TEXTURE_TYPE type,
                                             char *path,
                                             GLuint *pixels,
                                             GLuint w,
                                             GLuint h)
{
        return (sdfe_texture_info_t) {
                .type = type,
                .path = path,
                .pixels = pixels,
                .w = w,
                .h = h,
        };
}

GLuint sdfe_texture_create(sdfe_texture_info_t tinfo)
{
        GLuint texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);

        if (tinfo.type == SDFE_TEXTURE_PIXELS) {
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tinfo.w, tinfo.h, 0,
                             GL_RGBA, GL_UNSIGNED_BYTE, tinfo.pixels);
        } else if (tinfo.type == SDFE_TEXTURE_FILE) {
                SDL_Surface *surface;
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

                surface = IMG_Load(tinfo.path);
                if (!surface) {
                        SDFE_PUSH_ERROR(IMG_GetError());
                        glDeleteTextures(1, &texture);
                        return 0;
                }

                switch (surface->format->BytesPerPixel) {
                case 1:
                        fprintf(stderr, "DEBUG: processing 1 byte texture!\n");
                        SDFE_PUSH_ERROR("TODO: cannot load 1 byte textures!\n");
                        SDL_FreeSurface(surface);
                        glDeleteTextures(1, &texture);
                        return 0;
                case 2:
                        fprintf(stderr, "DEBUG: processing 2 byte texture!\n");
                        SDFE_PUSH_ERROR("TODO: cannot load 2 byte textures!\n");
                        SDL_FreeSurface(surface);
                        glDeleteTextures(1, &texture);
                        return 0;
                case 3:
                        fprintf(stderr, "DEBUG: processing 3 byte texture!\n");
                        SDFE_PUSH_ERROR("TODO: cannot load 3 byte textures!\n");
                        SDL_FreeSurface(surface);
                        glDeleteTextures(1, &texture);
                        return 0;
                case 4:
                        fprintf(stderr, "DEBUG: processing 4 byte texture!\n");
                        //SDFE_PUSH_ERROR("TODO: cannot load 4 byte textures!\n");
                        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surface->w, surface->h, 0,
                                     GL_RGBA, GL_UNSIGNED_BYTE, surface->pixels);
                        SDL_FreeSurface(surface);
                        return 0;
                }
        } else {
                SDFE_PUSH_ERROR("Invalid texture type!");
                glDeleteTextures(1, &texture);
                texture = 0;
        }

        return texture;
}
