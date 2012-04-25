/* CMPSC 311, Spring 2012, Project 8
 *
 * Author:   Don Heller
 * Modified: Trevor Sprinkle (project 4)
 * Modified: Max DeLiso (project 8)
 *
 */

/* base library for CMPSC 311 projects
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <errno.h>
#include <string.h>

#include "cmpsc311.h"

//------------------------------------------------------------------------------

// global variables set from the command line

char *prog = "[no name]";       // program name
int verbose = 0;                // -v option, extra output

//------------------------------------------------------------------------------

// utility functions

// guard against null pointer for character string output
const char *safe_string(const char *str)
{
   if (str == NULL)
      return "(null)";
   else
      return str;
}

//------------------------------------------------------------------------------

// check function arguments

// #define verify(tf, msg) cmpsc311_verify(tf, msg, __func__, __LINE__)

void cmpsc311_verify(const bool tf, const char *msg, const char *func,
                     const int line)
{
   if (tf == false) {
      fprintf(stderr, "%s: %s() at line %d failed: %s\n", prog, func, line,
              msg);
      exit(EXIT_FAILURE);
   }
}

//------------------------------------------------------------------------------

// check function return values
//   malloc
//   strdup
//   fopen

// #define Malloc(size) cmpsc311_malloc(size, __func__, __LINE__)

void *cmpsc311_malloc(const size_t size, const char *func, const int line)
{
   void *p = malloc(size);
   if (p == NULL) {
      fprintf(stderr, "%s: %s() at line %d failed: malloc(): %s\n", prog, func,
              line, strerror(errno));
      exit(EXIT_FAILURE);
   }

   if (verbose) {
      fprintf(stderr, "%s: malloc(%zd) at %p from %s line %d\n", prog, size, p,
              func, line);
   }

   return p;
}

// #define Strdup(s) cmpsc311_strdup(s, __func__, __LINE__)

char *cmpsc311_strdup(const char *s, const char *func, const int line)
{
   char *p = strdup(s);
   if (p == NULL) {
      fprintf(stderr, "%s: %s() at line %d failed: strdup(): %s\n", prog, func,
              line, strerror(errno));
      exit(EXIT_FAILURE);
   }

   if (verbose) {
      fprintf(stderr, "%s: strdup(%zd) at %p from %s line %d\n", prog,
              strlen(s) + 1, p, func, line);
   }

   return p;
}

// #define Fopen(filename,mode) cmpsc311_fopen(filename, mode, __func__, __LINE__)

FILE *cmpsc311_fopen(const char *restrict filename, const char *restrict mode,
                     const char *func, const int line)
{
   FILE *f = fopen(filename, mode);
   if (f == NULL) {
      fprintf(stderr, "%s: %s() at line %d failed: fopen(): %s\n", prog, func,
              line, strerror(errno));
      exit(EXIT_FAILURE);
   }

   return f;
}

//------------------------------------------------------------------------------
