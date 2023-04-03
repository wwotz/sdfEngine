#include "../include/sdfE.h"

sdfe_vec2_t sdfe_vec2_create(float x, float y)
{
        return (sdfe_vec2_t) { x, y };
}

float sdfe_vec2_length(sdfe_vec2_t vec)
{
        return sqrtf((vec.x * vec.x) + (vec.y * vec.y));
}

sdfe_vec2_t sdfe_vec2_add(sdfe_vec2_t first, sdfe_vec2_t second)
{
        return sdfe_vec2_create(first.x + second.x,
                                first.y + second.y);
}

sdfe_vec2_t sdfe_vec2_sub(sdfe_vec2_t first, sdfe_vec2_t second)
{
        return sdfe_vec2_create(first.x - second.x,
                                first.y - second.y);
}

sdfe_vec2_t sdfe_vec2_mul(sdfe_vec2_t first, sdfe_vec2_t second)
{
        return sdfe_vec2_create(first.x * second.x,
                                first.y * second.y);
}

sdfe_vec2_t sdfe_vec2_div(sdfe_vec2_t first, sdfe_vec2_t second)
{
        return sdfe_vec2_create(first.x / second.x,
                                first.y / second.y);
}

float sdfe_vec2_dot(sdfe_vec2_t first, sdfe_vec2_t second)
{
        return first.x * second.x + first.y * second.y;
}

float sdfe_vec2_cross(sdfe_vec2_t first, sdfe_vec2_t second)
{
        return first.x * second.y - first.y * second.x;
}

sdfe_vec2_t sdfe_vec2_normalise(sdfe_vec2_t vec)
{
        float length = sdfe_vec2_length(vec);
        return (sdfe_vec2_t) { vec.x / length, vec.y / length };
}

sdfe_vec3_t sdfe_vec3_create(float x, float y, float z)
{
        return (sdfe_vec3_t) { x, y, z };
}

float sdfe_vec3_length(sdfe_vec3_t vec)
{
        return sqrtf(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
}

sdfe_vec3_t sdfe_vec3_add(sdfe_vec3_t first, sdfe_vec3_t second)
{
        return sdfe_vec3_create(first.x + second.x,
                                first.y + second.y,
                                first.z + second.z);
}

sdfe_vec3_t sdfe_vec3_sub(sdfe_vec3_t first, sdfe_vec3_t second)
{
        return sdfe_vec3_create(first.x - second.x,
                                first.y - second.y,
                                first.z - second.z);
}

sdfe_vec3_t sdfe_vec3_mul(sdfe_vec3_t first, sdfe_vec3_t second)
{
        return sdfe_vec3_create(first.x * second.x,
                                first.y * second.y,
                                first.z * second.z);
}

sdfe_vec3_t sdfe_vec3_div(sdfe_vec3_t first, sdfe_vec3_t second)
{
        return sdfe_vec3_create(first.x / second.x,
                                first.y / second.y,
                                first.z / second.z);
}

float sdfe_vec3_dot(sdfe_vec3_t first, sdfe_vec3_t second)
{
        return first.x * second.x + first.y * second.y + first.z * second.z;
}

sdfe_vec3_t sdfe_vec3_cross(sdfe_vec3_t first, sdfe_vec3_t second)
{
        return sdfe_vec3_create(first.y * second.z - first.z * second.y,
                                first.z * second.x - first.x * second.z,
                                first.x * second.y - first.y * second.x);
}

sdfe_vec3_t sdfe_vec3_normalise(sdfe_vec3_t vec)
{
        float length = sdfe_vec3_length(vec);
        return (sdfe_vec3_t) { vec.x / length, vec.y / length, vec.z / length };
}

sdfe_vec4_t sdfe_vec4_create(float x, float y, float z, float w)
{
        return (sdfe_vec4_t) { x, y, z, w };
}

float sdfe_vec4_length(sdfe_vec4_t vec)
{
        return sqrtf(vec.x * vec.x + vec.y * vec.y
                     + vec.z * vec.z + vec.w * vec.w);
}

sdfe_vec4_t sdfe_vec4_add(sdfe_vec4_t first, sdfe_vec4_t second)
{
        return sdfe_vec4_create(first.x + second.x,
                                first.y + second.y,
                                first.z + second.z,
                                first.w + second.w);
}

sdfe_vec4_t sdfe_vec4_sub(sdfe_vec4_t first, sdfe_vec4_t second)
{
        return sdfe_vec4_create(first.x - second.x,
                                first.y - second.y,
                                first.z - second.z,
                                first.w - second.w);
}

sdfe_vec4_t sdfe_vec4_mul(sdfe_vec4_t first, sdfe_vec4_t second)
{
        return sdfe_vec4_create(first.x * second.x,
                                first.y * second.y,
                                first.z * second.z,
                                first.w * second.w);
}

sdfe_vec4_t sdfe_vec4_div(sdfe_vec4_t first, sdfe_vec4_t second)
{
        return sdfe_vec4_create(first.x / second.x,
                                first.y / second.y,
                                first.z / second.z,
                                first.w / second.w);
}

float sdfe_vec4_dot(sdfe_vec4_t first, sdfe_vec4_t second)
{
        return first.x * second.x + first.y * second.y
                + first.z * second.z + first.w * second.w;
}

sdfe_vec4_t sdfe_vec4_normalise(sdfe_vec4_t vec)
{
        float length = sdfe_vec4_length(vec);
        return (sdfe_vec4_t) {
                vec.x / length,
                vec.y / length,
                vec.z / length,
                vec.w / length
        };
}

int sdfe_matrix_multiply(float l[16], float r[16])
{
        float final[16];
        for (int i = 0; i < 4; i++) {
                for (int j = 0; j < 4; j++) {
                        float sum = 0.0;
                        for (int k = 0; k < 4; k++)
                                sum += l[j * 4 + k] * r[i * 4 + k];
                        final[j * 4 + i] = sum;
                }
        }

        sdfe_matrix_copy(l, final);
        return 0;
}

void sdfe_matrix_copy(float to[16], float from[16])
{
        for (int i = 0; i < 16; i++)
                to[i] = from[i];
}

int sdfe_matrix_identity(float m[16])
{
        for (int i = 0; i < 16; i++)
                m[i] = 0;
        m[0]  = 1;
        m[5]  = 1;
        m[10] = 1;
        m[15] = 1;
        return 0;
}

int sdfe_matrix_translate3f(float m[16], float dx, float dy, float dz)
{
        float other[16] = {
                1.0, 0.0, 0.0, dx,
                0.0, 1.0, 0.0, dy,
                0.0, 0.0, 1.0, dz,
                0.0, 0.0, 0.0, 1.0,
        };

        return sdfe_matrix_multiply(m, other);
}

int sdfe_matrix_translate3fv(float m[16], sdfe_vec3_t vec)
{
        return sdfe_matrix_translate3f(m, vec.x, vec.y, vec.z);
}

int sdfe_matrix_scale3f(float m[16], float w, float h, float d)
{
        float other[16] = {
                w, 0, 0, 0,
                0, h, 0, 0,
                0, 0, d, 0,
                0, 0, 0, 1,
        };

        return sdfe_matrix_multiply(m, other);
}

int sdfe_matrix_scale3fv(float m[16], sdfe_vec3_t vec)
{
        return sdfe_matrix_scale3f(m, vec.x, vec.y, vec.z);
}

int sdfe_matrix_rotate3f(float m[16], float x, float y, float z, float theta)
{
        float other[16] = {
                cos(theta) + x*x*(1-cos(theta)),   x*y*(1-cos(theta)) - z*sin(theta), x*z*(1-cos(theta)) + y*sin(theta), 0.0,
                y*x*(1-cos(theta)) + z*sin(theta), cos(theta) + y*y*(1-cos(theta)),   y*z*(1-cos(theta)) - x*sin(theta), 0.0,
                z*x*(1-cos(theta)) - y*sin(theta), y*z*(1-cos(theta)) + x*sin(theta), cos(theta) + x*x*(1-cos(theta)),   0.0,
                0.0,                               0.0,                               0.0,                               1.0,
        };

        return sdfe_matrix_multiply(m, other);
}

int sdfe_matrix_rotate3fv(float m[16], sdfe_vec3_t vec, float theta)
{
        return sdfe_matrix_rotate3f(m, vec.x, vec.y, vec.z, theta);
}

int sdfe_matrix_orthographic(float m[16], float t, float r, float b, float l, float n, float f)
{
        float other[16] = {
                2.0/(r-l), 0.0,       0.0,        -(r+l)/(r-l),
                0.0,       2.0/(t-b), 0.0,        -(t+b)/(t-b),
                0.0,       0.0,       -2.0/(f-n), -(f+n)/(f-n),
                0.0,       0.0,       0.0,        1.0
        };

        sdfe_matrix_identity(m);
        return sdfe_matrix_multiply(m, other);
}
