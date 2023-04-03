#include "../include/sdfE.h"

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

        return sdfe_buffer_write(buffer, data);
}

int sdfe_buffer_write(sdfe_buffer_t *buffer, const char *data)
{
        return sdfe_buffer_insert(buffer, data, 0);
}

int sdfe_buffer_insert(sdfe_buffer_t *buffer, const char *data, size_t offset)
{
        size_t data_len;
        int status;
        if (!buffer) {
                SDFE_PUSH_ERROR("buffer was null!");
                return SDFE_BUFFER_NULL_ERROR;
        }

        data_len = strlen(data);
        while (buffer->capacity <= buffer->size + data_len + offset) {
                if ((status = sdfe_buffer_resize(buffer)) != SDFE_BUFFER_NO_ERROR)
                        return status;
        }

        memcpy(buffer->data+offset, data, data_len);
        buffer->data[offset+data_len] = '\0';
        return SDFE_BUFFER_NO_ERROR;
}

int sdfe_buffer_length(sdfe_buffer_t *buffer)
{
        if (buffer)
                return buffer->size;
        return 0;
}

int sdfe_buffer_resize(sdfe_buffer_t *buffer)
{
        if (!buffer) {
                SDFE_PUSH_ERROR("buffer was null!");
                return SDFE_BUFFER_NULL_ERROR;
        }

        char *new_data = realloc(buffer->data, buffer->capacity * 2);
        if (!new_data) {
                SDFE_PUSH_ERROR("buffer failed to resize!");
                return SDFE_BUFFER_RESIZE_ERROR;
        }

        buffer->data = new_data;
        buffer->capacity *= 2;
        return SDFE_BUFFER_NO_ERROR;
}

void sdfe_buffer_free(sdfe_buffer_t *buffer)
{
        if (buffer) {
                if (buffer->data) {
                        free(buffer->data);
                        buffer->data = NULL;
                }

                buffer->size = 0;
                buffer->capacity = 0;
        }
}
