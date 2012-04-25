/* CMPSC 311, Spring 2012, Project 8
 *
 * Author:   Don Heller
 *
 */

#ifndef CMPSC311_TARGET_H
#define CMPSC311_TARGET_H

struct target {
   struct name *prereqs;        // NULL indicates end-of-list
   char *name;                  // from strdup()
   int line_num;
   struct target *next;
};

struct list_target {
   struct target *head;         // NULL indicates empty list
   struct target *tail;
   char *name;
};

extern struct list_target target_list;

void goal_set(char *name, char *prereqs, int line_num, char *file);
void parse_args(struct target *newTarget, char *prereqs);
int goal_run(char *name);

#endif
