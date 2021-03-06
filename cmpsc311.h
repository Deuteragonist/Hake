/* CMPSC 311, Spring 2012, Project 8
 *
 * Author:   Max DeLiso
 * Email:    jmd5473@psu.edu
 *
 */

#ifndef CMPSC311_H
#define CMPSC311_H

#include <stdbool.h>

//------------------------------------------------------------------------------

// global variables set from the command line

extern char *prog;              // program name
extern int verbose;             // -v option, extra output
                        // -v can be repeated for even more output

// macro definitions

// maximum line length in an input file (buffer size in read_lines)
#define MAXLINE 4096

//------------------------------------------------------------------------------

// utility functions

// guard against null pointer for character string output
const char *safe_string(const char *str);

//------------------------------------------------------------------------------

// check function arguments

#define verify(tf, msg) cmpsc311_verify(tf, msg, __func__, __LINE__)

void cmpsc311_verify(const bool tf, const char *msg, const char *func,
                     const int line);

//------------------------------------------------------------------------------

// check function return values
//   malloc
//   strdup
//   fopen

#define Malloc(size) cmpsc311_malloc(size, __func__, __LINE__)
#define Strdup(s1) cmpsc311_strdup(s1, __func__, __LINE__)
#define Fopen(filename,mode) cmpsc311_fopen(filename, mode, __func__, __LINE__)
#define strndup(s,n) cmpsc311_strndup(s,n, __func__, __LINE__)


void *cmpsc311_malloc(size_t size, const char *func, const int line);
char *cmpsc311_strdup(const char *s1, const char *func, const int line);
FILE *cmpsc311_fopen(const char *restrict filename, const char *restrict mode,
                     const char *func, const int line);
char *cmpsc311_strndup(const char *s, size_t n, const char * func, const int line);

//------------------------------------------------------------------------------

#endif
