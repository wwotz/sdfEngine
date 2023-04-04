#include "../include/sdfE.h"

sdfe_metric_t *sdfe_metric_create(char key, int minx, int maxx,
                                  int miny, int maxy, int advance, int offset)
{
        sdfe_metric_t *metric;
        metric = malloc(sizeof(*metric));
        if (!metric) {
                SDFE_PUSH_ERROR("metric_t structure failed to allocate memory!");
                return NULL;
        }

        metric->key = key;
        metric->next = NULL;
        metric->minx = minx;
        metric->maxx = maxx;
        metric->miny = miny;
        metric->maxy = maxy;
        metric->advance = advance;
        metric->offset = offset;
        return metric;
}

int sdfe_metric_init(sdfe_metric_t *metric, char key, int minx, int maxx,
                     int miny, int maxy, int advance, int offset)
{
        if (!metric) {
                SDFE_PUSH_ERROR("metric_t structure was null!");
                return SDFE_METRIC_NULL_ERROR;
        }

        metric->key = key;
        metric->next = NULL;
        metric->minx = minx;
        metric->maxx = maxx;
        metric->miny = miny;
        metric->maxy = maxy;
        metric->advance = advance;
        metric->offset = offset;
        return SDFE_METRIC_NO_ERROR;
}

sdfe_metric_t *sdfe_metric_insert(sdfe_metric_t *metric, char key, int minx, int maxx,
                                  int miny, int maxy, int advance, int offset)
{
        sdfe_metric_t *node;
        if (!metric) {
                SDFE_PUSH_ERROR("metric_t structure was null!");
                return NULL;
        }

        node = sdfe_metric_create(key, minx, maxx, miny, maxy, advance, offset);
        if (!node) {
                SDFE_PUSH_ERROR("metric_t failed to insert!");
                return metric;
        }

        node->next = metric;
        metric = node;
        return metric;
}

sdfe_metric_t *sdfe_metric_lookup(sdfe_metric_t *metric, char key)
{
        sdfe_metric_t *ptr;
        if (!metric) {
                SDFE_PUSH_ERROR("metric_t structure was null!");
                return NULL;
        }

        ptr = metric;
        while (ptr != NULL && ptr->key != key)
                ptr = ptr->next;
        return (ptr ? ptr : metric);
}

void sdfe_metric_free(sdfe_metric_t *metric)
{
        sdfe_metric_t *ptr;
        if (metric) {
                ptr = metric;
                while (ptr != NULL) {
                        sdfe_metric_t *next = ptr->next;
                        free(ptr);
                        ptr = next;
                }
        }
}

static int sdfe_glyph_hash(char c)
{
        return c % SDFE_METRICTABLE_SIZE;
}

int sdfe_glyph_init(sdfe_glyph_t *glyph, const char *path,
                    const char *sample, GLuint ptsize)
{
        GLuint texture;
        SDL_Surface *src_surface, *dest_surface;
        SDL_Color colour;
        TTF_Font *font;

        if (!glyph) {
                SDFE_PUSH_ERROR("glyph_t structure was null!");
                return SDFE_GLYPH_NULL_ERROR;
        }

        font = TTF_OpenFont(path, ptsize);
        if (!font) {
                SDFE_PUSH_ERROR(TTF_GetError());
                return SDFE_GLYPH_FONT_ERROR;
        }

        colour.r = colour.g = colour.b = colour.a = 0xff;
        src_surface = TTF_RenderText_Blended(font, sample, colour);
        if (!src_surface) {
                SDFE_PUSH_ERROR(TTF_GetError());
                return SDFE_GLYPH_TEXT_ERROR;
        }

        GLuint w = 1, h = 1;
        while (w <= src_surface->w)
                w <<= 1;
        while (h <= src_surface->h)
                h <<= 1;

        dest_surface = SDL_CreateRGBSurface(0, w, h, 32,
                                            src_surface->format->Rmask,
                                            src_surface->format->Gmask,
                                            src_surface->format->Bmask,
                                            src_surface->format->Amask);
        SDL_BlitSurface(src_surface, NULL, dest_surface, NULL);

        sdfe_texture_info_t tinfo = {
                SDFE_TEXTURE_PIXELS,
                0,
                dest_surface->pixels,
                dest_surface->w,
                dest_surface->h,
        };

        texture = sdfe_texture_create(tinfo);

        if (!texture) {
                return SDFE_GLYPH_TEXTURE_ERROR;
        }

        glyph->texture = texture;
        glyph->tw = dest_surface->w;
        glyph->th = dest_surface->h;
        glyph->h = TTF_FontHeight(font);
        glyph->w = 0;

        for (int i = 0; i < SDFE_METRICTABLE_SIZE; i++)
                glyph->table[i] = 0;

        sdfe_metric_t *found;
        for (int i = 0; sample[i] != '\0'; i++) {
                int minx, maxx, miny, maxy, advance;
                int hash = sdfe_glyph_hash(sample[i]);
                TTF_GlyphMetrics(font, sample[i], &minx, &maxx, &miny, &maxy,
                                 &advance);
                if (glyph->table[hash]) {
                        found = sdfe_metric_lookup(glyph->table[hash], sample[i]);
                        if (found && found->key != sample[i]) {
                                glyph->table[hash] = sdfe_metric_insert(glyph->table[hash],
                                                                        sample[i], minx, maxx, miny, maxy, advance, glyph->w);
                        }
                } else {
                        glyph->table[hash] = sdfe_metric_create(sample[i], minx, maxx, miny, maxy, advance, glyph->w);
                }
                glyph->w += advance;
        }

        TTF_CloseFont(font);
        SDL_FreeSurface(src_surface);
        SDL_FreeSurface(dest_surface);
        return SDFE_GLYPH_NO_ERROR;
}

void sdfe_glyph_free(sdfe_glyph_t *glyph)
{
        if (glyph) {
                glDeleteTextures(1, &glyph->texture);
                glyph->texture = 0;
                glyph->w = 0;
                glyph->h = 0;
                for (int i = 0; i < SDFE_METRICTABLE_SIZE; i++) {
                        sdfe_metric_free(glyph->table[i]);
                        glyph->table[i] = NULL;
                }
        }
}

sdfe_text_t *sdfe_text_create(const char *data, sdfe_glyph_t *glyph, GLuint program,
                              GLfloat x, GLfloat y, GLuint colour)
{
        int status;
        sdfe_text_t *text;

        if (!program || !glIsProgram(program))
                return NULL;

        text = malloc(sizeof(*text));
        if (!text) {
                SDFE_PUSH_ERROR("text_t failed to allocate memory!");
                return NULL;
        }

        if ((status = sdfe_buffer_init(&text->buffer, data)) != SDFE_BUFFER_NO_ERROR) {
                sdfe_text_free(text);
                free(text);
                return NULL;
        }

        if ((status = sdfe_rect_init(&text->rect, x, y, 0.0, 0.0, program, glyph->texture, 0x000000ff))
            != SDFE_RECT_NO_ERROR) {
                sdfe_text_free(text);
                free(text);
                return NULL;
        }

        return text;
}

int sdfe_text_init(sdfe_text_t *text,  const char *data, sdfe_glyph_t *glyph,
                   GLuint program, GLfloat x, GLfloat y, GLuint colour)
{
        int status;

        if (!program || !glIsProgram(program))
                return SDFE_TEXT_PROGRAM_ERROR;

        if (!text) {
                SDFE_PUSH_ERROR("text_t structure was null!");
                return SDFE_TEXT_NULL_ERROR;
        }

        if ((status = sdfe_buffer_init(&text->buffer, data)) != SDFE_BUFFER_NO_ERROR)
                return status;

        if ((status = sdfe_rect_init(&text->rect, x, y, 0.0, 0.0, program, glyph->texture,
                                     colour)) != SDFE_RECT_NO_ERROR) {
                sdfe_text_free(text);
                return status;
        }

        return SDFE_TEXT_NO_ERROR;
}

int sdfe_text_resize(sdfe_text_t *text)
{
        return sdfe_buffer_resize(&text->buffer);
}

int sdfe_text_append(sdfe_text_t *text, char c)
{
        return sdfe_buffer_append(&text->buffer, c);
}

int sdfe_text_write(sdfe_text_t *text, const char *data)
{
        return sdfe_buffer_write(&text->buffer, data);
}

int sdfe_text_clear(sdfe_text_t *text)
{
        return sdfe_buffer_clear(&text->buffer);
}

GLfloat sdfe_text_get_height(sdfe_text_t *text, sdfe_glyph_t *glyph)
{
        return (GLfloat) glyph->h;
}

GLfloat sdfe_text_get_width(sdfe_text_t *text, sdfe_glyph_t *glyph)
{
        GLfloat width = 0.0;
        char c;
        sdfe_metric_t *found;

        if (text && glyph) {
                for (int i = 0; (c = text->buffer.data[i]) != '\0'; i++) {
                        found = sdfe_metric_lookup(glyph->table[sdfe_glyph_hash(c)], c);
                        if (!found) return 0;
                        width += found->advance;
                }
        }

        return (GLfloat) width;
}

GLfloat sdfe_text_get_x(sdfe_text_t *text)
{
        if (text)
                return sdfe_rect_get_x(&text->rect);
        return 0.0;
}

GLfloat sdfe_text_get_y(sdfe_text_t *text)
{
        if (text)
                return sdfe_rect_get_y(&text->rect);
        return 0.0;
}

size_t sdfe_text_get_length(sdfe_text_t *text)
{
        if (text)
                return sdfe_buffer_length(&text->buffer);
        return 0;
}

char sdfe_text_get_charat(sdfe_text_t *text, int index)
{
        if (text)
                return sdfe_buffer_get_charat(&text->buffer, index);
        return '\0';
}

GLuint sdfe_text_get_colour(sdfe_text_t *text)
{
        if (text)
                return sdfe_rect_get_colour(&text->rect);
        return 0;
}

void sdfe_text_set_charat(sdfe_text_t *text, int index, char c)
{
        if (text)
                sdfe_buffer_set_charat(&text->buffer, index, c);
}

void sdfe_text_set_colour(sdfe_text_t *text, GLuint colour)
{
        if (text)
                sdfe_rect_set_colour(&text->rect, colour);
}

void sdfe_text_set_x(sdfe_text_t *text, GLfloat x)
{
        if (text)
                sdfe_rect_set_x(&text->rect, x);
}

void sdfe_text_set_y(sdfe_text_t *text, GLfloat y)
{
        if (text)
                sdfe_rect_set_y(&text->rect, y);
}

void sdfe_text_render(sdfe_text_t *text, sdfe_glyph_t *glyph)
{
        float sav_x, sav_y;
        char c;
        sdfe_metric_t *found;
        if (text && glyph) {
                sav_x = sdfe_rect_get_x(&text->rect);
                sav_y = sdfe_rect_get_y(&text->rect);
                for (int i = 0; (c = sdfe_text_get_charat(text, i)) != '\0'; i++) {
                        found = sdfe_metric_lookup(glyph->table[sdfe_glyph_hash(c)], c);
                        if (found) {
                                sdfe_rect_clip4f(&text->rect,
                                                 (GLfloat) found->offset/glyph->tw,
                                                 (GLfloat) 0.0,
                                                 (GLfloat) (found->offset + found->advance)/glyph->tw,
                                                 (GLfloat) glyph->h/glyph->th);
                                sdfe_rect_set_width(&text->rect, found->advance);
                                sdfe_rect_set_height(&text->rect, glyph->h);
                                sdfe_rect_render(&text->rect);
                                sdfe_text_set_x(text, sdfe_text_get_x(text) + found->advance);
                        }
                }
                sdfe_text_set_x(text, sav_x);
                sdfe_text_set_y(text, sav_y);
        }
}

void sdfe_text_free(sdfe_text_t *text)
{
        if (text) {
                sdfe_buffer_free(&text->buffer);
                sdfe_rect_free(&text->rect);
        }
}
