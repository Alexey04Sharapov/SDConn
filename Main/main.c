/*
    MIT License

    Copyright (c) 2020 Alexey Sharapov

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in all
    copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
    SOFTWARE.
 */

// This module is named kernel
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdint.h>
#include <string.h>

#include <unistd.h>

#include "../Headers/cmn_macros.h"

extern void (*presentation_init(void))(void);
extern struct {void (*exit)(void); void (*work)(void);}
    plugin_manager_init(void);

char program_dir_path[4096];

// This module is named kernel
int main(int argc, char *argv[]) {
    printf("ASTEAM !\n");

    if(unlikely(!getcwd(program_dir_path, sizeof(program_dir_path)))) {
        exit(1);
    }

    void (*fp)(void) = presentation_init();
    if(unlikely(fp && atexit(fp))) {
        //something goes wrong
        exit(1);
    }

    decltype(plugin_manager_init()) const fpTuple = plugin_manager_init();
    if(unlikely(fpTuple.exit && atexit(fpTuple.exit))) {
        //something goes wrong
        exit(1);
    }


    while(1) {
        if(likely(fpTuple.work)) {
            fpTuple.work();
        }
        sleep(1);
    }


    return 0;
}
