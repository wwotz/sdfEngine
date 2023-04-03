#include "../include/sdfE.h"

static GLboolean sdfe_shader_had_error(GLuint shader)
{
        if (glIsShader(shader)) {
                GLint status;
                glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
                return status == GL_FALSE;
        }
        return 0;
}

static char *sdfe_shader_get_error(GLuint shader)
{
        if (glIsShader(shader)) {
                GLchar *msg;
                GLint msg_len, max_len;
                glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &max_len);
                msg = malloc(max_len);
                if (!msg) {
                        return NULL;
                }
                glGetShaderInfoLog(shader, max_len, &msg_len, msg);
                if (msg_len > 0)
                        return msg;
                free(msg);
        }
        return NULL;
}

static GLboolean sdfe_program_had_error(GLuint program)
{
        if (glIsProgram(program)) {
                GLint status;
                glGetProgramiv(program, GL_LINK_STATUS, &status);
                return status == GL_FALSE;
        }
        return 0;
}

static char *sdfe_program_get_error(GLuint program)
{
        if (glIsProgram(program)) {
                GLchar *msg;
                GLint msg_len, max_len;
                glGetProgramiv(program, GL_INFO_LOG_LENGTH, &max_len);
                msg = malloc(max_len);
                if (!msg) {
                        return NULL;
                }
                glGetProgramInfoLog(program, max_len, &msg_len, msg);
                if (msg_len > 0)
                        return msg;
                free(msg);
        }
        return NULL;
}

sdfe_shader_info_t sdfe_shader_info_create(SDFE_SHADER_TYPE data_type,
                                           GLuint shader_type,
                                           char *data)
{
        return (sdfe_shader_info_t) {
                .data_type = data_type,
                .shader_type = shader_type,
                .data = data,
        };
}

sdfe_program_info_t sdfe_program_info_create(sdfe_shader_info_t vert,
                                             sdfe_shader_info_t frag)
{
        return (sdfe_program_info_t) {
                .vert = vert,
                .frag = frag,
        };
}

/* creates an OpenGL shader */
GLuint sdfe_shader_create(sdfe_shader_info_t info)
{
        GLuint shader;
        char *buffer, *msg;

        shader = glCreateShader(info.shader_type);
        if (!shader) {
                if (glGetError() != GL_INVALID_ENUM) {
                        SDFE_PUSH_ERROR("Invalid shader type!");
                        return 0;
                }
        }

        switch (info.data_type) {
        case SDFE_SHADER_FILE:
                buffer = sdfe_utils_fmap(info.data);
                if (!buffer)
                        return 0;
                break;
        default:
        case SDFE_SHADER_SOURCE:
                buffer = info.data;
                if (!buffer)
                        return 0;
                break;
        }

        glShaderSource(shader, 1, (const GLchar **)&buffer, 0);
        glCompileShader(shader);
        if (sdfe_shader_had_error(shader)) {
                msg = sdfe_shader_get_error(shader);
                SDFE_PUSH_ERROR(msg);
                free(msg);
                glDeleteShader(shader);
                shader = 0;
        }

        if (info.data_type == SDFE_SHADER_FILE) {
                free(buffer);
                buffer = NULL;
        }
        return shader;
}

/* creates an OpenGL program */
GLuint sdfe_program_create(sdfe_program_info_t info)
{
        char *msg;
        GLuint vert_shader, frag_shader, program;
        vert_shader = sdfe_shader_create(info.vert);
        if (!vert_shader)
                return 0;
        frag_shader = sdfe_shader_create(info.frag);
        if (!frag_shader)
                return 0;
        program = glCreateProgram();
        if (!program) {
                SDFE_PUSH_ERROR("Failed to create program!");
                return 0;
        }

        glAttachShader(program, vert_shader);
        glAttachShader(program, frag_shader);
        glLinkProgram(program);
        if (sdfe_program_had_error(program)) {
                msg = sdfe_program_get_error(program);
                SDFE_PUSH_ERROR(msg);
                free(msg);
                glDeleteProgram(program);
                program = 0;
        }

        glDeleteShader(vert_shader);
        glDeleteShader(frag_shader);
        return program;
}
