# G++ Compiler
CC = g++

# compile-time flags
CFLAGS = -Wall -g

# define any directories containing header files other than /usr/include
INCLUDES =  -I./include

# define library paths in addition to /usr/lib
LFLAGS =  -L./lib -L/usr/lib/x86_64-linux-gnu 

# define any libraries to link into executable:
LIBS = -lpthread -lmosquittopp 

# define the C source files
SRCS = src/DataObserver.cpp src/main.cpp

# define the C object files 
# This uses Suffix Replacement within a macro: $(name:string1=string2)
# For each word in 'name' replace 'string1' with 'string2'
# Below we are replacing the suffix .c of all words in the macro SRCS with the .o suffix

OBJS = $(SRCS:.cpp=.o)

# define the executable file 
MAIN = util-app

#
# The following part of the makefile is generic; it can be used to 
# build any executable just by changing the definitions above and by
# deleting dependencies appended to the file from 'make depend'
#

.PHONY: depend clean

all:    $(MAIN)
		@echo  The project has been compiled

$(MAIN): $(OBJS) 
		$(CC) $(CFLAGS) $(INCLUDES) -o $(MAIN) $(OBJS) $(LFLAGS) $(LIBS)

# this is a suffix replacement rule for building .o's from .c's
# it uses automatic variables $<: the name of the prerequisite of
# the rule(a .c file) and $@: the name of the target of the rule (a .o file) 
# (see the gnu make manual section about automatic variables)
.cpp.o:
		$(CC) $(CFLAGS) $(INCLUDES) -c $<  -o $@

clean:
		@echo  Cleaning the project
		$(RM) *.o *~ $(MAIN)
		@echo  The Project is cleaned

depend: $(SRCS)
		makedepend $(INCLUDES) $^

