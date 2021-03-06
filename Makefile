# CMPSC 311, Spring 2012, Project 8
#
# Author:   Don Heller
# Email:    dheller@cse.psu.edu
#
# Modified: Max DeLiso


SRC = hake.c
LIB = cmpsc311.c names.c macro.c target.c
INC = cmpsc311.h names.h macro.h target.h

dummy:
	@echo targets available: hake-sun hake-gcc hake-mac hake-lint clean

hake-sun: $(SRC) $(LIB) $(INC)
	c99 -D_XOPEN_SOURCE=600 -v -o hake $(SRC) $(LIB)

hake-gcc: $(SRC) $(LIB) $(INC)
	gcc -std=c99 -g -D_XOPEN_SOURCE=600 -Wall -Wextra -o hake $(SRC) $(LIB) # use solaris method

hake-mac: $(SRC) $(LIB) $(INC)
	gcc -std=c99 -Wall -DMTIME=3 -Wextra -o hake $(SRC) $(LIB)

hake-lint: $(SRC) $(LIB) $(INC)
	lint -Xc99 $(SRC) $(LIB)

clean:
	rm -f hake a.out *.o

