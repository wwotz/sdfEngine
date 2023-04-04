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
#define SDFE_WINDOW_FLAGS (SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL)

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
extern void sdfe_window_clear_buffers(GLbitfield buffers);
extern void sdfe_window_swap(void);
extern void sdfe_window_events(void);
extern int sdfe_window_poll_event(void);
extern int sdfe_window_event_type(void);
extern int sdfe_window_event_keycode(void);
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
extern int sdfe_buffer_write(sdfe_buffer_t *buffer, const char *data);
extern int sdfe_buffer_insert(sdfe_buffer_t *buffer, const char *data, size_t offset);
extern int sdfe_buffer_length(sdfe_buffer_t *buffer);
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
extern void sdfe_rect_render(sdfe_rect_t *rect);
extern GLfloat sdfe_rect_get_x(sdfe_rect_t *rect);
extern GLfloat sdfe_rect_get_y(sdfe_rect_t *rect);
extern GLfloat sdfe_rect_get_width(sdfe_rect_t *rect);
extern GLfloat sdfe_rect_get_height(sdfe_rect_t *rect);
extern GLuint sdfe_rect_get_colour(sdfe_rect_t *rect);
extern void sdfe_rect_set_x(sdfe_rect_t *rect, GLfloat x);
extern void sdfe_rect_set_y(sdfe_rect_t *rect, GLfloat y);
extern void sdfe_rect_set_width(sdfe_rect_t *rect, GLfloat w);
extern void sdfe_rect_set_height(sdfe_rect_t *rect, GLfloat h);
extern void sdfe_rect_set_colour(sdfe_rect_t *rect, GLuint colour);
extern void sdfe_rect_free(sdfe_rect_t *rect);

#endif // SDFE_ENGINEINCLUDE_H_
