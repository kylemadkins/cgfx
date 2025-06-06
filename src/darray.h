/*
darray.h is a fork of https://github.com/zauonlok/renderer/blob/master/renderer/core/darray.h
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

#ifndef DARRAY_H
#define DARRAY_H

#define darray_push(darray, value)                                          \
    do {                                                                    \
        (darray) = darray_hold((darray), 1, sizeof(*(darray)));             \
        (darray)[darray_size(darray) - 1] = (value);                        \
    } while (0)

void *darray_hold(void *darray, int count, int item_size);
int darray_size(void *darray);
void darray_free(void *darray);

#endif
