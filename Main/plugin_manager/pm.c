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
#include <sys/types.h>
#include <dlfcn.h>
#include <dirent.h>
#include <string.h>


#include "../../Headers/cmn_macros.h"
#include "../../Headers/msg_type.h"
#include "../api/pres2pm.h"

#define MAX_PLUGINS 255

extern char program_dir_path[4096];

static size_t plugin_count;

static callback_foo * callback_arr[MSG_COUNT];

static void onExit(void) {
    if(likely(callback_arr[0])) {
        free(callback_arr[0]);
    }
}

static void onGetMessage(enum msg_type type, size_t bodySize, char const body [][bodySize]) {

}

static void onPluginSendMessage(enum msg_type type, size_t bodySize, char const body [][bodySize],
        void (*callback)(int status)) {

    if(callback) {
        callback(-1);
    }
}

static void onUpdate(void) {

    for(size_t i = 0; i < plugin_count; ++i) {
        char tmp_arr[1] = {'\0'};

        callback_foo tmp = callback_arr[MSG_TYPE_UPDATE][i];
        if(tmp) {
            tmp(MSG_TYPE_UPDATE, 1, &tmp_arr, onPluginSendMessage);
        }
    }
}

static int loadPlugins() {

     int attribute(pure) isLibrary(const char * name) {
        int count = 0;

        while(*name) {
            if(*name == '.') {
                count = 1;
            }
            else if(count == 1 && *name == 's') {
                count = 2;
            }
            else if(count == 2 && *name == 'o') {
                count = 3;
            }
            else {
                count = 0;
            }
            ++name;
        }

        if(count == 3) return 1;
        return 0;
    }

    DIR * dir = opendir("Plugins");

    if(unlikely(!dir)) {
        puts("The directory hasn't been found\n");
        return 1;
    }

    void (*fooArr[MAX_PLUGINS + 1])(size_t count, callback_foo arr [][count]);
    size_t counter = 0;

    struct dirent * file;
    while((file = readdir(dir))) {

        if(isLibrary(file->d_name)) {
            char curr_path [sizeof(program_dir_path) + 256 + 10];
            curr_path[0] = '\0';
            strcat(curr_path, program_dir_path);
            strcat(curr_path, "/Plugins/");
            strcat(curr_path, file->d_name);

            void * handle = dlopen(curr_path, RTLD_LAZY);
            if(unlikely(!handle)) {
                puts("dlopen error\n");
            }

            fooArr[counter] = dlsym(handle, "plugin_init");
            ++counter;
            if(unlikely(counter > MAX_PLUGINS)) {
                puts("So many plugins\n.");
                exit(1);
            }
        }
    }

    if(unlikely(!counter)) {
        puts("Plugins haven't been found.\n");
        exit(1);
    }

    plugin_count = counter;
    closedir(dir);

    void * ptr = calloc(counter, sizeof(callback_arr));
    if(unlikely(!ptr)) {
        exit(1);
    }

    char * mem = ptr;

    for(size_t i = 0; i < array_size(callback_arr); ++i) {
        callback_arr[i] = (void *)mem;

        mem += sizeof(callback_foo) * counter;
    }

    for(size_t i = 0; i < counter; ++i) {
        callback_foo tmp_arr [MSG_COUNT];
        memset(tmp_arr, 0, sizeof(tmp_arr));

        (fooArr[i])(MSG_COUNT, &tmp_arr);

        for(size_t j = 0; j < MSG_COUNT; ++j) {
            callback_arr[j][i] = tmp_arr[j];
        }
    }

    return 0;
}

struct {void (*exit)(void); void (*work)(void);} plugin_manager_init(void) {

    subscribe(onGetMessage);

    if(unlikely(loadPlugins())) {
        exit(1);
    }

    return (decltype(plugin_manager_init())) {.exit = onExit, .work = onUpdate};
}
