UNAME:=$(shell uname -s)

ifeq ($(UNAME),SunOS)
	ifeq ($(CC),cc)
		CFLAGS?=-O
	else
		CFLAGS?=-Wall -ggdb -O2
	endif
else
	CFLAGS?=-Wall -ggdb -O2
endif

LIB_CPPFLAGS=$(CPPFLAGS) -I. -I./include
LIB_CFLAGS:=$(CFLAGS)
LIB_CXXFLAGS:=$(CXXFLAGS)
LIB_LDFLAGS:=$(LDFLAGS) 
LIB_LIBADD:=$(LIBADD) -lpthread -lmosquittopp 

LIB_CFLAGS:=$(LIB_CFLAGS) -fPIC -fPIE
LIB_CXXFLAGS:=$(LIB_CXXFLAGS) -fPIC -fPIE

ifeq ($(UNAME),Linux)
	LIB_LIBADD:=$(LIB_LIBADD) -lrt
endif

MAIN = util-app
SRCS = src/DataObserver.cpp src/main.cpp

OBJS = $(SRCS:.cpp=.o)

.PHONY: depend clean

all:    $(MAIN)
		@echo  The project has been compiled

$(MAIN): $(OBJS)
		${CROSS_COMPILE}$(CXX) $(LIB_CPPFLAGS) $(LIB_CXXFLAGS) -o $(MAIN) $(OBJS) $(LIB_LIBADD)

.cpp.o:
		${CROSS_COMPILE}$(CXX) $(LIB_CPPFLAGS) $(LIB_CXXFLAGS) -c $<  -o $@

clean:
		@echo  Cleaning the project
		$(RM) *.o *~ $(MAIN)
		@echo  The Project is cleaned
