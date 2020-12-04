#pragma once 

#if CONFIG_DEBUG
    #include <stdio.h>
    #define DEBUG(x, ...) printf("[D] " x "\n", __VA_ARGS__)
#else
    #define DEBUG(x, ...)
#endif