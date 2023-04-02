#ifndef SDFE_ENGINEINCLUDE_H_
#define SDFE_ENGINEINCLUDE_H_

/* debug */
#define SDFE_PUSH_ERROR(msg) \
        do { sdfe_debug_stack_push(msg, __FILE__, __LINE__); } while (0)
extern int sdfe_debug_stack_push(const char *msg, const char *file, size_t line);
extern const char *sdfe_debug_stack_pop(void);

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
