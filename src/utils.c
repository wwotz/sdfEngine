#include "../include/sdfE.h"

static size_t sdfe_utils_fsize(FILE *fd)
{
        GLuint size;
        fseek(fd, 0, SEEK_END);
        size = ftell(fd);
        fseek(fd, 0, SEEK_SET);
        return size;
}

char *sdfe_utils_fmap(const char *path)
{
        FILE *fd;
        char *buffer;
        size_t buffer_size, nread;

        fd = fopen(path, "r");
        if (!fd) {
                SDFE_PUSH_ERROR("Invalid file path!");
                return NULL;
        }

        buffer_size = sdfe_utils_fsize(fd);
        buffer = malloc(buffer_size + 1);
        while ((nread = fread(buffer, sizeof(*buffer), buffer_size, fd)) > 0)
                ;
        buffer[buffer_size] = '\0';

        if (ferror(fd)) {
                SDFE_PUSH_ERROR("There was an error reading the file!");
                free(buffer);
                return NULL;
        }

        fclose(fd);
        return buffer;
}
