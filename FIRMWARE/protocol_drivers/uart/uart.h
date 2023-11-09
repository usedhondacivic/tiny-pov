#pragma once

#include <stdio.h>

#undef FDEV_SETUP_STREAM
#define FDEV_SETUP_STREAM(p, g, f) \
    {                              \
        .buf = NULL,               \
        .unget = 0,                \
        .flags = f,                \
        .size = 0,                 \
        .len = 0,                  \
        .put = p,                  \
        .get = g,                  \
        .udata = 0                 \
    }

void USART0_init(void);