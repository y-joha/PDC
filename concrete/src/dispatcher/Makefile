

# make clean/all CONFIG=Debug/Release ARCH=linux64/raspi ECHO=0/1

# the parrent dir of arch dir
ROOT_DIR = ../../../../..

PROJ_SRCS = $(wildcard *.cpp)
#PROJ_SRCS = $(foreach D,$(DIR),$(wildcard $(D)/*.cpp))

# list of directories containing header files
PROJ_INC_DIR = ../../include ../../../framework/include
#PROJ_INC_DIR =

# the executable file
PROJ_OUT = dispatcherTest.out

# list of ADDITIONAL compiler flags (prepended with - sign)
#PROJ_CFLAGS = -Wunused-but-set-variable
PROJ_CFLAGS = 
PROJ_CPPFLAGS = -std=c++11

#PROJ_MACROS = _BSD_SOURCE
PROJ_MACROS =

# additional object files (created by other projects)
PROJ_ADD_OBJS = command.obj threadpool.obj semaphore.obj request_host.obj  nbd_comm.o transmitter.obj

# additional libraries
PROJ_LIBS =

# linker flags
PROJ_LDFLAGS =

# list of projects (makefiles) this project depends on
PROJ_DEPENDS  = ../command/Makefile ../request_host/Makefile ../../../framework/src/threadpool/Makefile ../GARY/Makefile  ../../../framework/src/semaphore/Makefile  ../../../framework/src/factory/Makefile ../../../framework/src/transmitter/Makefile
include $(ROOT_DIR)/make/Project.mak

