/* CMPSC 311, Spring 2012, Project 8
 *
 * test program for macro.c
 */

//--------------------------------------------------------------------------------

#include <stdio.h>
#include <string.h>

#include "macro.c"

int main(void)
{
   macro_list_print();

   macro_set("A", "a");
   macro_set("B", "bb");
   macro_set("C", "ccc");
   macro_list_print();

   macro_set("B", "dddd");
   macro_list_print();

   macro_set("A", "eeeee");
   macro_list_print();

   printf("%s = '%s' (%d)\n", "A", macro_body("A"), macro_length("A"));
   printf("%s = '%s' (%d)\n", "B", macro_body("B"), macro_length("B"));
   printf("%s = '%s' (%d)\n", "C", macro_body("C"), macro_length("C"));
   printf("%s = '%s' (%d)\n", "D", macro_body("D"), macro_length("D"));

   char in[4096] = "test.${A}.${B}.${C}.${D}.";
   char out[4096];
   printf("len %d\n", macro_expand_length(in));
   macro_expand(in, out);
   printf("%zd %s\n", strlen(in), in);
   printf("%zd %s\n", strlen(out), out);
   printf("len %d\n", macro_expand_length(in));

   macro_set("A", "${B}");
   macro_list_print();
   printf("len %d\n", macro_expand_length(in));
   macro_expand(in, out);
   printf("%zd %s\n", strlen(in), in);
   printf("%zd %s\n", strlen(out), out);
   printf("len %d\n", macro_expand_length(in));

   macro_set("A", "body");
   macro_set("B", "body body");
   macro_set("C", "body body body");
   macro_set("D", "enough already!");
   macro_list_print();
   printf("len %d\n", macro_expand_length(in));
   macro_expand(in, out);
   printf("%zd %s\n", strlen(in), in);
   printf("%zd %s\n", strlen(out), out);
   printf("len %d\n", macro_expand_length(in));

   return 0;
}
