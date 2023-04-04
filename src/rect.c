#include "../include/sdfE.h"

static GLfloat vertices[16] = {
        0.0, 0.0, 0.0, 0.0,
        1.0, 0.0, 1.0, 0.0,
        1.0, 1.0, 1.0, 1.0,
        0.0, 1.0, 0.0, 1.0,
};

static GLuint indices[6] = {
        0, 1, 2,
        2, 3, 0
};

int sdfe_rect_init(sdfe_rect_t *rect, GLfloat x, GLfloat y, GLfloat w, GLfloat h,
                          GLuint program, GLuint texture, GLuint colour)
{
        if (!rect) {
                SDFE_PUSH_ERROR("rect was null!");
                return SDFE_RECT_NULL_ERROR;
        }

        rect->x = x;
        rect->y = y;
        rect->w = w;
        rect->h = h;
        rect->texture = texture;
        rect->colour = colour;

        rect->program = program;
        if (!rect->program) {
                SDFE_PUSH_ERROR("OpenGL program was not given!");
                return SDFE_RECT_PROGRAM_ERROR;
        }

        if (!glIsProgram(program)) {
                SDFE_PUSH_ERROR("OpenGL program was not a program!");
                return SDFE_RECT_PROGRAM_ERROR;
        }

        glGenVertexArrays(1, &rect->vao);
        glBindVertexArray(rect->vao);

        glGenBuffers(1, &rect->vbo);
        glBindBuffer(GL_ARRAY_BUFFER, rect->vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);

        glGenBuffers(1, &rect->ebo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rect->ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        glVertexAttribPointer(SDFE_RECT_VERTEX_LOC, 2, GL_FLOAT,
                              GL_FALSE, 4 * sizeof(*vertices),
                              (const GLvoid *)0);

        glVertexAttribPointer(SDFE_RECT_TEX_VERTEX_LOC, 2, GL_FLOAT,
                              GL_FALSE, 4 * sizeof(*vertices),
                              (const GLvoid *)(2 * sizeof(*vertices)));

        glEnableVertexAttribArray(SDFE_RECT_VERTEX_LOC);
        glEnableVertexAttribArray(SDFE_RECT_TEX_VERTEX_LOC);

        glBindVertexArray(0);
        glUseProgram(0);
        glBindTexture(GL_TEXTURE_2D, 0);
        return SDFE_RECT_NO_ERROR;
}

void sdfe_rect_clip4f(sdfe_rect_t *rect, GLfloat x, GLfloat y, GLfloat w, GLfloat h)
{
        if (rect) {
                vertices[2]  = x;
                vertices[3]  = y;
                vertices[6]  = w;
                vertices[7]  = y;
                vertices[10] = w;
                vertices[11] = h;
                vertices[14] = x;
                vertices[15] = h;

                glBindVertexArray(rect->vao);
                glBindBuffer(GL_ARRAY_BUFFER, rect->vbo);
                glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);
                glBindVertexArray(0);

                vertices[2]  = 0.0f;
                vertices[3]  = 0.0f;
                vertices[6]  = 1.0f;
                vertices[7]  = 0.0f;
                vertices[10] = 1.0f;
                vertices[11] = 1.0f;
                vertices[14] = 0.0f;
                vertices[15] = 1.0f;
        }
}

void sdfe_rect_render(sdfe_rect_t *rect)
{
        if (rect) {
                glUseProgram(rect->program);

                glBindTexture(GL_TEXTURE_2D, rect->texture);

                sdfe_matrix_identity(sdfe_model);
                sdfe_matrix_scale3f(sdfe_model, rect->w, rect->h, 1.0);
                sdfe_matrix_translate3f(sdfe_model, rect->x, rect->y, 0.0);

                glUniformMatrix4fv(glGetUniformLocation(rect->program, "model"), 1, GL_FALSE, sdfe_model);
                glUniformMatrix4fv(glGetUniformLocation(rect->program, "view"), 1, GL_FALSE, sdfe_view);
                glUniformMatrix4fv(glGetUniformLocation(rect->program, "projection"), 1, GL_FALSE, sdfe_projection);
                glUniform1f(glGetUniformLocation(rect->program, "time"), sdfe_timer_get_time());
                glUniform2f(glGetUniformLocation(rect->program, "origin"), rect->x, rect->y);
                glUniform2f(glGetUniformLocation(rect->program, "size"), rect->w, rect->h);

                glUniform4f(glGetUniformLocation(rect->program, "colour"),
                            sdfe_colour_mask(rect->colour, SDFE_COLOUR_MASK_R, GL_TRUE),
                            sdfe_colour_mask(rect->colour, SDFE_COLOUR_MASK_G, GL_TRUE),
                            sdfe_colour_mask(rect->colour, SDFE_COLOUR_MASK_B, GL_TRUE),
                            sdfe_colour_mask(rect->colour, SDFE_COLOUR_MASK_A, GL_TRUE));

                glBindVertexArray(rect->vao);
                glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
                glBindVertexArray(0);
                glBindTexture(GL_TEXTURE_2D, 0);
                glUseProgram(0);
        }
}

GLfloat sdfe_rect_get_x(sdfe_rect_t *rect)
{
        if (rect)
                return rect->x;
        return 0.0;
}

GLfloat sdfe_rect_get_y(sdfe_rect_t *rect)
{
        if (rect)
                return rect->x;
        return 0.0;
}

GLfloat sdfe_rect_get_width(sdfe_rect_t *rect)
{
        if (rect)
                return rect->w;
        return 0.0;
}

GLfloat sdfe_rect_get_height(sdfe_rect_t *rect)
{
        if (rect)
                return rect->h;
        return 0.0;
}

GLuint sdfe_rect_get_colour(sdfe_rect_t *rect)
{
        if (rect)
                return rect->colour;
        return 0;
}

void sdfe_rect_set_x(sdfe_rect_t *rect, GLfloat x)
{
        if (rect)
                rect->x = x;
}

void sdfe_rect_set_y(sdfe_rect_t *rect, GLfloat y)
{
        if (rect)
                rect->y = y;
}

void sdfe_rect_set_width(sdfe_rect_t *rect, GLfloat w)
{
        if (rect)
                rect->w = w;;
}

void sdfe_rect_set_height(sdfe_rect_t *rect, GLfloat h)
{
        if (rect)
                rect->h = h;
}

void sdfe_rect_set_colour(sdfe_rect_t *rect, GLuint colour)
{
        if (rect)
                rect->colour = colour;
}

void sdfe_rect_free(sdfe_rect_t *rect)
{
        if (rect) {
                rect->x = 0.0;
                rect->y = 0.0;
                rect->w = 0.0;
                rect->h = 0.0;
                rect->colour = 0;
                glDeleteBuffers(1, &rect->vbo);
                glDeleteBuffers(1, &rect->ebo);
                glDeleteVertexArrays(1, &rect->vao);
                rect->vao = 0;
                rect->vbo = 0;
                rect->vao = 0;
                rect->texture = 0;
                rect->program = 0;
        }
}
