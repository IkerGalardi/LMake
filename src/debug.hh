#pragma once 

#include <stdio.h>

#define DEBUG(...) printf("[D] "); printf(__VA_ARGS__); printf("\n")

#define ERROR(...) fprintf(stderr, "[E] "); fprintf(stderr, __VA_ARGS__); fprintf(stderr, "\n")