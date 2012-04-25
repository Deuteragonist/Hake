/* CMPSC 311, Spring 2012, Project 8
 *
 * Author:   Don Heller
 * Modified: Trevor Sprinkle (project 4)
 * Modified: Max DeLiso (project 8)
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <errno.h>
#include <string.h>

#include "cmpsc311.h"
#include "names.h"

//------------------------------------------------------------------------------

void list_names_init(struct list_names *list, const char *listname)
{
   verify(list != NULL, "null arg list");
   verify(listname != NULL, "null arg listname");
   verify(listname[0] != '\0', "empty arg listname");

   list->name = Strdup(listname);
   list->head = list->tail = NULL;
}

//------------------------------------------------------------------------------

struct list_names *list_names_allocate(const char *listname)
{
   verify(listname != NULL, "null arg listname");
   verify(listname[0] != '\0', "empty arg listname");

   struct list_names *list = Malloc(sizeof(struct list_names));

   list->name = Strdup(listname);
   list->head = list->tail = NULL;

   return list;
}

//------------------------------------------------------------------------------

void list_names_deallocate(struct list_names *list)
{
   verify(list != NULL, "null arg list");

   free(list->name);
   struct name *prev = NULL;
   for (struct name * p = list->head; p != NULL; p = p->next) {
      free(prev);               // free(NULL) is harmless
      free(p->name);
      prev = p;
   }
   free(prev);                  // prev == list->tail
   free(list);
}

//------------------------------------------------------------------------------

void list_names_print(struct list_names *list)
{
   verify(list != NULL, "null arg list");

   printf("list of names: %s\n", safe_string(list->name));

   if (list->head == NULL) {
      printf("  <empty>\n");
   } else {
      for (struct name * p = list->head; p != NULL; p = p->next) {
         printf("  %s\n", p->name);
      }
   }
}

//------------------------------------------------------------------------------

void list_names_append(struct list_names *list, const char *name)
{
   verify(list != NULL, "null arg list");
   verify(name != NULL, "null arg name");
   verify(name[0] != '\0', "empty arg name");

   struct name *fn = Malloc(sizeof(struct name));

   fn->next = NULL;
   fn->name = Strdup(name);

   if (list->head == NULL)      // empty list, list->tail is also NULL
   {
      list->head = list->tail = fn;
   } else {
      list->tail->next = fn;
      list->tail = fn;
   }
}

//------------------------------------------------------------------------------

// if (name is on the list already) { return 1 }
// else { put name on the list and return 0 }

int list_names_append_if_new(struct list_names *list, const char *name)
{
   verify(list != NULL, "null arg list");
   verify(name != NULL, "null arg name");
   verify(name[0] != '\0', "empty arg name");

   for (struct name * p = list->head; p != NULL; p = p->next) {
      if (strcmp(p->name, name) == 0)
         return 1;              // name is on the list already
   }

   list_names_append(list, name);

   return 0;
}

//------------------------------------------------------------------------------

void list_names_append_from_file(struct list_names *list, const char *filename)
{
   verify(list != NULL, "null arg list");
   verify(filename != NULL, "null arg filename");
   verify(filename[0] != '\0', "empty arg filename");

   FILE *infile;

   if (strcmp(filename, "-") == 0) {
      infile = stdin;
   } else {
      infile = fopen(filename, "r");
      if (infile == NULL) {
         fprintf(stderr, "%s: failed: could not open file %s: %s\n", prog,
                 filename, strerror(errno));
         exit(EXIT_FAILURE);
      }
   }

   char buffer[256];
   char whsp[] = " \t\n\v\f\r"; // whitespace characters
   char *p;

   while (fgets(buffer, sizeof(buffer), infile) != NULL) {
      /* Note that buffer has a newline at the end if the input line
       *   was short enough to fit.  Line-too-long is probably an error,
       *   to be caught later.
       * NULL from fgets() indicates end-of-file or error, so we just quit.
       */
      p = strchr(buffer, '\n'); // points to the newline character, if there is one
      if (p != NULL) {
         *p = '\0';
      }                         // remove the newline

      p = strchr(buffer, '#');  // a comment starts with #
      if (p != NULL) {
         *p = '\0';
      }                         // remove the comment

      int m = strspn(buffer, whsp);     // index of first non-whitespace character

      // remove trailing whitespace
      int n = 0;
      while (buffer[n] != '\0') {
         int n1 = strspn(&buffer[n], whsp);     // buffer[n .. n+n1-1] is whitespace
         int n2 = strcspn(&buffer[n + n1], whsp);       // buffer[n+n1 .. n+n1+n2-1] is not
         if (n2 == 0) {
            buffer[n] = '\0';
            break;
         }                      // remove trailing whitespace
         n += n1 + n2;
      }

      if (buffer[m] == '\0')
         continue;              // empty line

      list_names_append(list, &buffer[m]);
   }

   if (infile != stdin && fclose(infile) != 0) {
      fprintf(stderr, "%s: failed: could not close input file %s: %s\n", prog,
              filename, strerror(errno));
      exit(EXIT_FAILURE);
   }
}

//------------------------------------------------------------------------------

void list_names_iterate(struct list_names *list, void *func(const char *))
{
   verify(list != NULL, "null arg list");
   verify(func != NULL, "null arg func");

   for (struct name * p = list->head; p != NULL; p = p->next) {
      printf("calling func on %s\n", p->name);
   }
}

//------------------------------------------------------------------------------
