/* CMPSC 311, Spring 2012, Project 8
 *
 * Author:   Don Heller
 * Modified: Trevor Sprinkle (project 4)
 * Modified: Max DeLiso (project 8)
 *
 */

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include<stdbool.h>
#include<sys/stat.h>
#include<sys/time.h>

#include "cmpsc311.h"
#include "target.h"
#include "names.h"

struct list_target target_list;

static struct timespec mtime(const char *file)
{
   struct stat s;
   struct timespec t = { 0, 0 };

   if (stat(file, &s) == 0)
#if   defined(MTIME) && MTIME == 1      // Linux
   {
      t = s.st_mtim;
   }
#elif defined(MTIME) && MTIME == 2      // Mac OS X
   {
      t = s.st_mtimespec;
   }
#elif defined(MTIME) && MTIME == 3      // Mac OS X, with some additional settings
   {
      t.tv_sec = s.st_mtime;
      t.tv_nsec = s.st_mtimensec;
   }
#else                           // Solaris
   {
      t.tv_sec = s.st_mtime;
   }
#endif

   return t;
}

static struct target *target_lookup(char *name)
{
   struct target *current = target_list.head;
   do {
      if (strcmp(current->name, name) == 0)
         return current;
      else
         current = current->next;
   } while (current != NULL);
   return NULL;
}

bool check_age(struct target * aim, struct name * dependencies)
{
   struct timespec t1 = mtime(aim->name);       // target
   if (t1.tv_sec == 0){
      if(verbose > 0) printf("target %s does not exist\n",aim->name);
      return true;              // target does not exist
   }
   struct timespec t2 = mtime(dependencies->name);      // source
   if(verbose > 0){
      printf("target %s made at %u.%u; dependency %s made at %u.%u\n",
            aim->name,(unsigned int)t1.tv_sec,(unsigned int)t1.tv_nsec,
            dependencies->name,(unsigned int)t2.tv_sec,(unsigned int)t2.tv_nsec); 
   }
   if (t1.tv_sec < t2.tv_sec    // target is older than source
       || ((t1.tv_sec == t2.tv_sec) && (t1.tv_nsec < t2.tv_nsec)))
      return true;

   return false;
}

void parse_args(struct target *newTarget, char *prereqs)
{
   int startofline = 0;
   char *prereq;
   bool spaceFlag = (prereqs[0] == ' ');
   struct name *name;
   struct name *tail = newTarget->prereqs;
   for (int i = 0; i <= (int)strlen(prereqs); i++) {
      if ((prereqs[i] == ' ' )||(i == (int)strlen(prereqs))){
         if(spaceFlag){
           startofline++;
         }
         else{
            prereq = (char *)malloc(i - startofline + 1);
            for (int j = startofline; j < i; j++) {
               prereq[j - startofline] = prereqs[j];
            }
            name = (struct name *)malloc(sizeof(struct name));
            name->name = prereq;
            startofline = i+1;  
            spaceFlag = true;
            if (tail == NULL) {
               newTarget->prereqs = name;
               tail = name;
            } else {
               tail->next = name;
               tail = name;
            }
         }
      }
      else if(spaceFlag){
         spaceFlag = false;
      }
   }
}

void goal_set(char *name, char *prereqs, int line_num, char *file)
{
   struct target *newTarget = malloc(sizeof(struct target));
   if (target_list.name == NULL)
      target_list.name = strdup(file);
   for(int i = strlen(name)-1; i>=0; i--){
      if(name[i] != ' ')     
        newTarget->name = strndup(name,(size_t)(i+1));
   }
   parse_args(newTarget, prereqs);
   newTarget->line_num = line_num;

   if (target_list.head == NULL) {
      target_list.tail = newTarget;
      target_list.head = newTarget;
   } else {
      target_list.tail->next = newTarget;
      target_list.tail = newTarget;
   }
}

int goal_run(char *goal)
{ 
   struct target *aim = ((goal == NULL) ? target_list.head : target_lookup(goal));
   if(aim == NULL) return 1;
   struct name *prereq = aim->prereqs;

   char in[MAXLINE];
   int rline = 0;
   bool update = true;
   while (prereq != NULL) {
      if (target_lookup(prereq->name) != NULL)
         goal_run(prereq->name);
      else
         update = update && check_age(aim, prereq);
      if (prereq->next == NULL)
         break;
      prereq = prereq->next;
   }
   if (update) {
      FILE *fp = fopen(target_list.name, "r");
      if (fp == NULL)
         return 1;
      for (int i = 0; i < aim->line_num; i++) {
         fgets(in, MAXLINE, fp);
      }
      do {
         fgets(in, MAXLINE, fp);
         printf("Recipe line %d>> %s", rline++, in);
      } while (in[0] == '\t');
   }
   return 0;
}

void target_free(){
   free(target_list.name);
   struct target* t = NULL;
   struct name * n = NULL;
   for (struct target * p = target_list.head; p != NULL; p = p->next) {
      free(t);               // free(NULL) is harmless
      printf("target: %s_\n",p->name);
      free(p->name);
      for(struct name * s = p->prereqs; s != NULL; s = s->next){
         free(n);
         printf("dependency: %s_\n",s->name);
         free(s->name);
         n = s;
      }
      t = p;
   }
   free(t);                  // prev == list->tail
   free(n);
}
