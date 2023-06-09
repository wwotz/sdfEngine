#ifndef SDFE_ENGINEINCLUDE_H_
#define SDFE_ENGINEINCLUDE_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include <GL/glew.h>
#include <GL/glu.h>
#include <GL/gl.h>

#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <poll.h>


/* debug */
#define SDFE_DEBUG_STACK_CAPACITY 10
#define SDFE_DEBUG_MESSAGE_LENGTH 1024
#define SDFE_PUSH_ERROR(msg) \
        do { sdfe_debug_stack_push(msg, __FILE__, __LINE__); } while (0)
extern void sdfe_debug_stack_push(const char *msg, const char *file, size_t line);
extern const char *sdfe_debug_stack_pop(void);
extern int sdfe_debug_had_error(void);

/* utils */
extern char *sdfe_utils_fmap(const char *path);

/* window */
#define SDFE_WINDOW_X SDL_WINDOWPOS_UNDEFINED
#define SDFE_WINDOW_Y SDL_WINDOWPOS_UNDEFINED
#define SDFE_WINDOW_W 1200.0
#define SDFE_WINDOW_H 800.0
#define SDFE_WINDOW_FLAGS (SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE)

typedef enum SDFE_WINDOW_ENUM {
        SDFE_WINDOW_NO_ERROR = 0,
        SDFE_WINDOW_SDL_ERROR,
        SDFE_WINDOW_IMG_ERROR,
        SDFE_WINDOW_TTF_ERROR,
        SDFE_WINDOW_WINDOW_ERROR,
        SDFE_WINDOW_CONTEXT_ERROR,
        SDFE_WINDOW_GLEW_ERROR,
        SDFE_WINDOW_ENUM_COUNT
} SDFE_WINDOW_ENUM;

extern GLfloat sdfe_model[16], sdfe_view[16], sdfe_projection[16];
extern int sdfe_window_init(const char *name, int x, int y, int w, int h,
                            int flags);
extern void sdfe_window_viewport(float x, float y, float w, float h);
extern void sdfe_window_get_size(int *w, int *h);
extern void sdfe_window_clear_buffers(GLbitfield buffers);
extern void sdfe_window_swap(void);
extern void sdfe_window_events(void);
extern int sdfe_window_poll_event(void);
extern int sdfe_window_event_type(void);
extern int sdfe_window_event_keycode(void);
extern int sdfe_window_event_window(void);
extern int sdfe_window_get_running(void);
extern GLfloat sdfe_window_get_width(void);
extern GLfloat sdfe_window_get_height(void);
extern void sdfe_window_set_clear_colour(GLuint colour);
extern void sdfe_window_set_running(int running);
extern void sdfe_window_free(void);

/* buffer */
#define SDFE_BUFFER_INITIAL_CAPACITY 8
typedef enum SDFE_BUFFER_ENUM {
        SDFE_BUFFER_NO_ERROR = 0,
        SDFE_BUFFER_NULL_ERROR,
        SDFE_BUFFER_DATA_ERROR,
        SDFE_BUFFER_RESIZE_ERROR,
        SDFE_BUFFER_ENUM_COUNT
} SDFE_BUFFER_ENUM;

typedef struct sdfe_buffer_t {
        size_t capacity;
        size_t size;
        char *data;
} sdfe_buffer_t;

extern int sdfe_buffer_init(sdfe_buffer_t *buffer, const char *data);
extern int sdfe_buffer_append(sdfe_buffer_t *buffer, char c);
extern int sdfe_buffer_write(sdfe_buffer_t *buffer, const char *data);
extern int sdfe_buffer_insert(sdfe_buffer_t *buffer, const char *data, size_t offset);
extern int sdfe_buffer_clear(sdfe_buffer_t *buffer);
extern int sdfe_buffer_length(sdfe_buffer_t *buffer);
extern char sdfe_buffer_get_charat(sdfe_buffer_t *buffer, int index);
extern void sdfe_buffer_set_charat(sdfe_buffer_t *buffer, int index, char c);
extern int sdfe_buffer_resize(sdfe_buffer_t *buffer);
extern void sdfe_buffer_free(sdfe_buffer_t *buffer);

/* colour */
typedef enum sdfe_colour_mask_t {
        SDFE_COLOUR_MASK_R = 0,
        SDFE_COLOUR_MASK_G,
        SDFE_COLOUR_MASK_B,
        SDFE_COLOUR_MASK_A,
        SDFE_COLOUR_MASK_COUNT
} sdfe_colour_mask_t;

extern GLfloat sdfe_colour_mask(GLuint colour, sdfe_colour_mask_t mask, GLboolean normalize);

/* timer */
extern void sdfe_timer_start(void);
extern void sdfe_timer_reset(void);
extern double sdfe_timer_get_time(void);

/* linear */
typedef struct {
        float x, y;
} sdfe_vec2_t;

typedef struct {
        float x, y, z;
} sdfe_vec3_t;

typedef struct {
        float x, y, z, w;
} sdfe_vec4_t;

extern sdfe_vec2_t sdfe_vec2_create(float x, float y);
extern float sdfe_vec2_length(sdfe_vec2_t vec);
extern sdfe_vec2_t sdfe_vec2_add(sdfe_vec2_t first, sdfe_vec2_t second);
extern sdfe_vec2_t sdfe_vec2_sub(sdfe_vec2_t first, sdfe_vec2_t second);
extern sdfe_vec2_t sdfe_vec2_mul(sdfe_vec2_t first, sdfe_vec2_t second);
extern sdfe_vec2_t sdfe_vec2_div(sdfe_vec2_t first, sdfe_vec2_t second);
extern float sdfe_vec2_dot(sdfe_vec2_t first, sdfe_vec2_t second);
extern float sdfe_vec2_cross(sdfe_vec2_t first, sdfe_vec2_t second);
extern sdfe_vec2_t sdfe_vec2_normalise(sdfe_vec2_t vec);

extern sdfe_vec3_t sdfe_vec3_create(float x, float y, float z);
extern float sdfe_vec3_length(sdfe_vec3_t vec);
extern sdfe_vec3_t sdfe_vec3_add(sdfe_vec3_t first, sdfe_vec3_t second);
extern sdfe_vec3_t sdfe_vec3_sub(sdfe_vec3_t first, sdfe_vec3_t second);
extern sdfe_vec3_t sdfe_vec3_mul(sdfe_vec3_t first, sdfe_vec3_t second);
extern sdfe_vec3_t sdfe_vec3_div(sdfe_vec3_t first, sdfe_vec3_t second);
extern float sdfe_vec3_dot(sdfe_vec3_t first, sdfe_vec3_t second);
extern sdfe_vec3_t sdfe_vec3_cross(sdfe_vec3_t first, sdfe_vec3_t second);
extern sdfe_vec3_t sdfe_vec3_normalise(sdfe_vec3_t vec);

extern sdfe_vec4_t sdfe_vec4_create(float x, float y, float z, float w);
extern float sdfe_vec4_length(sdfe_vec4_t vec);
extern sdfe_vec4_t sdfe_vec4_add(sdfe_vec4_t first, sdfe_vec4_t second);
extern sdfe_vec4_t sdfe_vec4_sub(sdfe_vec4_t first, sdfe_vec4_t second);
extern sdfe_vec4_t sdfe_vec4_mul(sdfe_vec4_t first, sdfe_vec4_t second);
extern sdfe_vec4_t sdfe_vec4_div(sdfe_vec4_t first, sdfe_vec4_t second);
extern float sdfe_vec4_dot(sdfe_vec4_t first, sdfe_vec4_t second);
extern sdfe_vec4_t sdfe_vec4_normalise(sdfe_vec4_t vec);

extern int sdfe_matrix_multiply(float l[16], float r[16]);
extern void sdfe_matrix_copy(float to[16], float from[16]);
extern int sdfe_matrix_identity(float m[16]);
extern int sdfe_matrix_translate3f(float m[16], float dx, float dy, float dz);
extern int sdfe_matrix_translate3fv(float m[16], sdfe_vec3_t vec);
extern int sdfe_matrix_scale3f(float m[16], float w, float h, float d);
extern int sdfe_matrix_scale3fv(float m[16], sdfe_vec3_t vec);
extern int sdfe_matrix_rotate3f(float m[16], float x, float y, float z, float angle);
extern int sdfe_matrix_rotate3fv(float m[16], sdfe_vec3_t, float angle);
extern int sdfe_matrix_orthographic(float m[16], float t, float r, float b, float l, float n, float f);

/* texture */
typedef enum SDFE_TEXTURE_TYPE {
        SDFE_TEXTURE_FILE = 0,
        SDFE_TEXTURE_PIXELS,
        SDFE_TEXTURE_TYPE_COUNT
} SDFE_TEXTURE_TYPE;

typedef struct sdfe_texture_info_t {
        SDFE_TEXTURE_TYPE type;
        char *path;
        GLuint *pixels;
        GLuint w;
        GLuint h;
} sdfe_texture_info_t;

extern sdfe_texture_info_t sdfe_texture_info_create(SDFE_TEXTURE_TYPE type,
                                                    char *path,
                                                    GLuint *pixels,
                                                    GLuint w,
                                                    GLuint h);
extern GLuint sdfe_texture_create(sdfe_texture_info_t tinfo);

/* shader */
typedef enum SDFE_SHADER_TYPE {
        SDFE_SHADER_FILE = 0,
        SDFE_SHADER_SOURCE,
        SDFE_SHADER_TYPE_COUNT
} SDFE_SHADER_TYPE;

typedef struct sdfe_shader_info_t {
        SDFE_SHADER_TYPE data_type;
        GLuint shader_type;
        char *data;
} sdfe_shader_info_t;

typedef struct sdfe_program_info_t {
        sdfe_shader_info_t vert;
        sdfe_shader_info_t frag;
} sdfe_program_info_t;

extern sdfe_shader_info_t sdfe_shader_info_create(SDFE_SHADER_TYPE data_type,
                                                  GLuint shader_type,
                                                  char *data);
extern sdfe_program_info_t sdfe_program_info_create(sdfe_shader_info_t vert,
                                                    sdfe_shader_info_t frag);
extern GLuint sdfe_shader_create(sdfe_shader_info_t info);
extern GLuint sdfe_program_create(sdfe_program_info_t info);

/* rect */
typedef enum sdfe_rect_locs_t {
        SDFE_RECT_VERTEX_LOC = 0,
        SDFE_RECT_TEX_VERTEX_LOC,
        SDFE_RECT_LOC_COUNT
} sdfe_rect_locs_t;

typedef enum SDFE_RECT_ENUM {
        SDFE_RECT_NO_ERROR = 0,
        SDFE_RECT_NULL_ERROR,
        SDFE_RECT_PROGRAM_ERROR,
        SDFE_RECT_ENUM_COUNT
} SDFE_RECT_ENUM;

typedef struct sdfe_rect_t {
        GLuint vbo, vao, ebo;
        GLfloat x;
        GLfloat y;
        GLfloat w;
        GLfloat h;
        GLuint program;
        GLuint colour;
        GLuint texture;
} sdfe_rect_t;

extern int sdfe_rect_init(sdfe_rect_t *rect, GLfloat x, GLfloat y, GLfloat w, GLfloat h,
                          GLuint program, GLuint texture, GLuint colour);
extern void sdfe_rect_clip4f(sdfe_rect_t *rect, GLfloat x, GLfloat y, GLfloat w, GLfloat h);
extern void sdfe_rect_render(sdfe_rect_t *rect);
extern GLfloat sdfe_rect_get_x(sdfe_rect_t *rect);
extern GLfloat sdfe_rect_get_y(sdfe_rect_t *rect);
extern GLfloat sdfe_rect_get_width(sdfe_rect_t *rect);
extern GLfloat sdfe_rect_get_height(sdfe_rect_t *rect);
extern GLuint sdfe_rect_get_colour(sdfe_rect_t *rect);
extern void sdfe_rect_set_program(sdfe_rect_t *rect, GLuint program);
extern void sdfe_rect_set_x(sdfe_rect_t *rect, GLfloat x);
extern void sdfe_rect_set_y(sdfe_rect_t *rect, GLfloat y);
extern void sdfe_rect_set_width(sdfe_rect_t *rect, GLfloat w);
extern void sdfe_rect_set_height(sdfe_rect_t *rect, GLfloat h);
extern void sdfe_rect_set_colour(sdfe_rect_t *rect, GLuint colour);
extern void sdfe_rect_free(sdfe_rect_t *rect);

/* text */
#define SDFE_METRICTABLE_SIZE 16

typedef enum SDFE_GLYPH_ENUM {
        SDFE_GLYPH_NO_ERROR = 0,
        SDFE_GLYPH_NULL_ERROR,
        SDFE_GLYPH_FONT_ERROR,
        SDFE_GLYPH_TEXT_ERROR,
        SDFE_GLYPH_TEXTURE_ERROR,
        SDFE_GLYPH_ENUM_COUNT
} SDFE_GLYPH_ENUM;

typedef enum SDFE_METRIC_ENUM {
        SDFE_METRIC_NO_ERROR = 0,
        SDFE_METRIC_NULL_ERROR,
        SDFE_METRIC_ENUM_COUNT
} SDFE_METRIC_ENUM;

typedef struct sdfe_metric_t {
        char key;
        int minx;
        int maxx;
        int miny;
        int maxy;
        int advance;
        int offset;
        struct sdfe_metric_t *next;
} sdfe_metric_t;

typedef struct sdfe_glyph_t {
        GLuint texture;
        GLuint tw;
        GLuint th;
        GLuint w;
        GLuint h;
        sdfe_metric_t *table[SDFE_METRICTABLE_SIZE];
} sdfe_glyph_t;

typedef enum {
        SDFE_TEXT_NO_ERROR = 0,
        SDFE_TEXT_NULL_ERROR,
        SDFE_TEXT_PROGRAM_ERROR,
        SDFE_TEXT_ENUM_COUNT
} SDFE_TEXT_ENUM;

typedef struct {
        sdfe_buffer_t buffer;
        sdfe_rect_t rect;
} sdfe_text_t;

extern sdfe_metric_t *sdfe_metric_create(char key, int minx, int maxx,
                                         int miny, int maxy, int advance, int offset);
extern int sdfe_metric_init(sdfe_metric_t *metric, char key, int minx, int maxx,
                            int miny, int maxy, int advance, int offset);
extern sdfe_metric_t *sdfe_metric_insert(sdfe_metric_t *metric, char key, int minx, int maxx,
                                         int miny, int maxy, int advance, int offset);
extern sdfe_metric_t *sdfe_metric_lookup(sdfe_metric_t *metric, char key);
extern void sdfe_metric_free(sdfe_metric_t *metric);
extern int sdfe_glyph_init(sdfe_glyph_t *glyph, const char *path,
                                      const char *sample, GLuint ptsize);
extern void sdfe_glyph_free(sdfe_glyph_t *glyph);

extern sdfe_text_t *sdfe_text_create(const char *data, sdfe_glyph_t *glyph, GLuint program,
                                     GLfloat x, GLfloat y, GLuint colour);
extern int sdfe_text_init(sdfe_text_t *text,  const char *data, sdfe_glyph_t *glyph,
                          GLuint program, GLfloat x, GLfloat y, GLuint colour);
extern int sdfe_text_resize(sdfe_text_t *text);
extern int sdfe_text_append(sdfe_text_t *text, char c);
extern int sdfe_text_write(sdfe_text_t *text, const char *data);
extern int sdfe_text_clear(sdfe_text_t *text);
extern GLfloat sdfe_text_get_height(sdfe_text_t *text, sdfe_glyph_t *glyph);
extern GLfloat sdfe_text_get_width(sdfe_text_t *text, sdfe_glyph_t *glyph);
extern GLfloat sdfe_text_get_x(sdfe_text_t *text);
extern GLfloat sdfe_text_get_y(sdfe_text_t *text);
extern size_t sdfe_text_get_length(sdfe_text_t *text);
extern char sdfe_text_get_charat(sdfe_text_t *text, int index);
extern GLuint sdfe_text_get_colour(sdfe_text_t *text);
extern void sdfe_text_set_charat(sdfe_text_t *text, int index, char c);
extern void sdfe_text_set_colour(sdfe_text_t *text, GLuint colour);
extern void sdfe_text_set_x(sdfe_text_t *text, GLfloat x);
extern void sdfe_text_set_y(sdfe_text_t *text, GLfloat y);
extern void sdfe_text_render(sdfe_text_t *text, sdfe_glyph_t *glyph);
extern void sdfe_text_free(sdfe_text_t *text);

/* server */
#define SDFE_SERVER_PORT "3490"
#define SDFE_SERVER_BUFSIZE 256

typedef enum SDFE_SERVER_ENUM {
        SDFE_SERVER_NO_ERROR = 0,
        SDFE_SERVER_REUSE_ERROR,
        SDFE_SERVER_BIND_ERROR,
        SDFE_SERVER_LISTEN_ERROR,
        SDFE_SERVER_ACCEPT_ERROR,
        SDFE_SERVER_ENUM_COUNT
} SDFE_SERVER_ENUM;

typedef enum sdfe_server_code_t {
        SDFE_SERVER_CODE_NONE = 0,
        SDFE_SERVER_CODE_END,
        SDFE_SERVER_CODE_LOAD_SHADER,
        SDFE_SERVER_CODE_REFRESH_SHADER,
        SDFE_SERVER_CODE_RESET_TIMER,
} sdfe_server_code_t;

extern int sdfe_server_start(void);
extern char *sdfe_server_get_message(void);
extern sdfe_server_code_t sdfe_server_recv(void);
extern void sdfe_server_send_message(const char *msg);
extern void sdfe_server_close(void);

#endif // SDFE_ENGINEINCLUDE_H_
