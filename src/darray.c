/*
darray.c is a fork of https://github.com/zauonlok/renderer/blob/master/renderer/core/darray.c
darray.c is under the following license:
------------------------------------------------------------------------------
MIT License

Copyright (c) 2020 Zhou Le

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

#include <assert.h>
#include <stdlib.h>
#include "darray.h"

/*
 * for type-safe dynamic array, see
 * https://github.com/nothings/stb/blob/master/stretchy_buffer.h
 */

#define DARRAY_RAW_DATA(darray) ((int*)(darray) - 2)
#define DARRAY_CAPACITY(darray) (DARRAY_RAW_DATA(darray)[0])
#define DARRAY_OCCUPIED(darray) (DARRAY_RAW_DATA(darray)[1])

int darray_size(void *darray) {
    return darray != NULL ? DARRAY_OCCUPIED(darray) : 0;
}

void darray_free(void *darray) {
    if (darray != NULL) {
        free(DARRAY_RAW_DATA(darray));
    }
}

void *darray_hold(void *darray, int count, int item_size) {
    int header_size = sizeof(int) * 2;
    assert(count > 0 && item_size > 0);
    if (darray == NULL) {
        int raw_size = header_size + item_size * count;
        int *base = (int*)malloc(raw_size);
        base[0] = count;  /* capacity */
        base[1] = count;  /* occupied */
        return base + 2;
    } else if (DARRAY_OCCUPIED(darray) + count <= DARRAY_CAPACITY(darray)) {
        DARRAY_OCCUPIED(darray) += count;
        return darray;
    } else {
        int needed_size = DARRAY_OCCUPIED(darray) + count;
        int double_curr = DARRAY_CAPACITY(darray) * 2;
        int capacity = needed_size > double_curr ? needed_size : double_curr;
        int occupied = needed_size;
        int raw_size = header_size + item_size * capacity;
        int *base = (int*)realloc(DARRAY_RAW_DATA(darray), raw_size);
        base[0] = capacity;
        base[1] = occupied;
        return base + 2;
    }
}
