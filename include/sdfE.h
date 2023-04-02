#ifndef SDFE_ENGINEINCLUDE_H_
#define SDFE_ENGINEINCLUDE_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include <GL/glew.h>
#include <GL/glu.h>
#include <GL/gl.h>*/


/* debug */
#define SDFE_DEBUG_STACK_CAPACITY 10
#define SDFE_DEBUG_MESSAGE_LENGTH 1024
#define SDFE_PUSH_ERROR(msg) \
        do { sdfe_debug_stack_push(msg, __FILE__, __LINE__); } while (0)
extern int sdfe_debug_stack_push(const char *msg, const char *file, size_t line);
extern const char *sdfe_debug_stack_pop(void);
extern int sdfe_debug_had_error(void);

/* window */
extern int sdfe_window_init(const char *name, int x, int y, int w, int h,
                            int flags);
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
extern int sdfe_buffer_resize(sdfe_buffer_t *buffer);
extern void sdfe_buffer_free(sdfe_buffer_t *buffer);

#endif // SDFE_ENGINEINCLUDE_H_
