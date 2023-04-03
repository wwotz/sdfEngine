#include "../include/sdfE.h"

GLfloat sdfe_colour_mask(GLuint colour, sdfe_colour_mask_t mask, GLboolean normalize)
{
        GLfloat col;
        switch (mask) {
        case SDFE_COLOUR_MASK_R:
                col = (colour & 0xff000000) >> 24;
                return (normalize ? col / 255.0 : col);
        case SDFE_COLOUR_MASK_G:
                col = (colour & 0x00ff0000) >> 16;
                return (normalize ? col / 255.0 : col);
        case SDFE_COLOUR_MASK_B:
                col = (colour & 0x0000ff00) >> 8;
                return (normalize ? col / 255.0 : col);
        default:
        case SDFE_COLOUR_MASK_A:
                col = (colour & 0x000000ff) >> 0;
                return (normalize ? col / 255.0 : col);
        }
}
