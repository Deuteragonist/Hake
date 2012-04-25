/* CMPSC 311, Spring 2012, Project 8
 *
 * Author:   Don Heller
 * Modified: Trevor Sprinkle (project 4)
 * Modified: Max DeLiso (project 8)
 *
 */

//--------------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "cmpsc311.h"
#include "macro.h"
//-----------------------------------------------------------------------

char *Strndup(char *str, int len)
{
   char *retval = (char *)malloc(len + 1);
   assert(retval != NULL);
   strncpy(retval, str, len);
   retval[len] = '\0';
   return retval;
}

//--------------------------------------------------------------------------------

// list of known macros, initially empty

struct macro {
   struct macro *next;
   char *macro;
   char *result;
};

static struct macro macro_list = {
   .next = NULL,
   .macro = NULL,
   .result = NULL
};

struct macro nixed;
//--------------------------------------------------------------------------------

void macro_print(struct macro *current)
{
   (current->next) == NULL ? (1 + 1) : (macro_print(current->next));
   printf("\t%s = %s (%i)\n", current->macro, current->result,
          strlen(current->result));
}

void macro_list_print(void)
{
   printf("Macro list\n");
   struct macro *current = &(macro_list);
   (current->macro == NULL) ? (1 + 1) : (macro_print(current));
   printf("\t-- end of list\n");
}

static struct macro *macro_lookup(char *name)
{

   struct macro *current = &(macro_list);
   do {
      if (strcmp(current->macro, name) == 0)
         return current;
      else
         current = current->next;
   } while (current != NULL);
   return NULL;
}

char *macro_body(char *name)
{
   struct macro *current = &(macro_list);
   do {
      if (strcmp(current->macro, name) == 0)
         return current->result;
      else
         current = current->next;
   } while (current != NULL);
   return "";
}

int macro_length(char *name)
{
   struct macro *current = &(macro_list);
   do {
      if (strcmp(current->macro, name) == 0)
         return strlen(current->result);
      else
         current = current->next;
   } while (current != NULL);
   return 0;
}

void macro_set(char *name, char *body)
{
   struct macro *current = &(macro_list);

   if (current->macro == NULL) {        //first one
      current->macro = (char *)malloc(strlen(name));
      strcpy(current->macro, name);
      current->result = (char *)malloc(strlen(body));
      strcpy(current->result, body);
      return;
   }

   while (true) {
      if (strcmp(current->macro, name) == 0) {
         free(current->result);
         current->result = (char *)malloc(strlen(body));
         strcpy(current->result, body);
         break;
      } else if (current->next == NULL) {
         current->next = (struct macro *)malloc(sizeof(struct macro));
         (current->next)->next = NULL;
         (current->next)->macro = (char *)malloc(strlen(name));
         strcpy((current->next)->macro, name);
         (current->next)->result = (char *)malloc(strlen(body));
         strcpy((current->next)->result, body);
         break;
      } else {
         current = current->next;
      }
   }
}

// assume in[] is constructed properly
// assume out[] is large enough
void macro_expand(char *in, char *out)
{
   int i = 0, j = 0, len = 0;
   char *temp = NULL;
   char *expand = NULL;
   out[0] = '\0';
   for (i = 0; i < (int)strlen(in); i++) {

      if (*(in + i) == '$') {
         for (j = 1; j < (int)strlen(in) - i; j++) {

            if (*(in + i + j) == '}') {
               temp = Strndup(in + i + 2, j - 2);       //char*,sizeof
               expand = macro_body(temp);
               free(temp);
               temp = NULL;

               if (expand == NULL) {
                  if (true) {
                     printf("Macro undefined: %s\n", temp);
                  }
               } else {
                  strcat(out, expand);
               }

               i = i + j + 1;
               break;
            } else if (*(in + i + j) == '\0') { //Ending brace not found: abort!
               strcat(out, in + i);
               if (true) {
                  printf
                      ("Ending brace not found, assuming no macro expansions exist\n");
               }
               i = i + j + 1;
               break;
            }
         }
      }
      len = strlen(out);
      out[len] = *(in + i);
      out[len + 1] = '\0';
   }
}

// assume in[] is constructed properly
// returns length of the expansion
int macro_expand_length(char *in)
{
   int i = 0, j = 0, len = 0;
   char *temp = NULL;
   char *expand = NULL;
   for (i = 0; i < (int)strlen(in); i++) {

      if (*(in + i) == '$') {
         for (j = 1; j < (int)strlen(in) - i; j++) {

            if (*(in + i + j) == '}') {
               temp = Strndup(in + i + 2, j - 2);       //char*,sizeof
               expand = macro_body(temp);
               free(temp);
               temp = NULL;

               if (expand == NULL) {
                  if (true) {
                     printf("Macro undefined: %s\n", temp);
                  }
               } else {
                  len += strlen(expand);
               }

               i = i + j + 1;
               break;
            } else if (*(in + i + j) == '\0') { //Ending brace not found: abort!
               len += j;
               if (true) {
                  printf
                      ("Ending brace not found, assuming no macro expansions exist\n");
               }
               i = i + j + 1;
               break;
            }
         }
      }
      len++;
   }
   return len;
}

//--------------------------------------------------------------------------------
