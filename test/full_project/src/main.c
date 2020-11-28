#include "print.h"

int main(int argc, char** argv) {
    print("Welcome to the test project");

    for(int i = 1; i < argc; i++)
        print(argv[i]);
}