#include "../include/sdfE.h"

static struct timeval sdfe_start, sdfe_curr;

void sdfe_timer_start(void)
{
        sdfe_timer_reset();
}

void sdfe_timer_reset(void)
{
        gettimeofday(&sdfe_start, NULL);
        gettimeofday(&sdfe_curr, NULL);
}

double sdfe_timer_get_time(void)
{
        gettimeofday(&sdfe_curr, NULL);
        double time = (sdfe_curr.tv_sec - sdfe_start.tv_sec)
                + (double) (sdfe_curr.tv_usec - sdfe_start.tv_usec)/1000000.0;
        return time;
}
