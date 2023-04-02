#include "buffer.h"

int sdfe_buffer_init(sdfe_buffer_t *buffer, const char *data)
{
        if (!buffer) {
                SDFE_PUSH_ERROR("buffer was null!");
                return SDFE_BUFFER_NULL_ERROR;
        }

        buffer->size = 0;
        buffer->capacity = SDFE_BUFFER_INITIAL_CAPACITY;
        buffer->data = malloc(SDFE_BUFFER_INITIAL_CAPACITY);
        if (!buffer->data) {
                sdfe_buffer_free(buffer);
                SDFE_PUSH_ERROR("buffer failed to malloc data!");
                return SDFE_BUFFER_DATA_ERROR;
        }

        return SDFE_BUFFER_NO_ERROR;
}

int sdfe_buffer_write(sdfe_buffer_t *buffer, const char *data)
{

}

int sdfe_buffer_insert(sdfe_buffer_t *buffer, const char *data, size_t offset)
{

}

int sdfe_buffer_resize(sdfe_buffer_t *buffer)
{

}

void sdfe_buffer_free(sdfe_buffer_t *buffer)
{

}
