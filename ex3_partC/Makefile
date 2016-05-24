#################################################################
#
#				Warning!!!!
#				============
#	
# this make file compiles all the sources with implicit rules
# therefore in order to make sure all the changes were made 
# run it with "make clean all"
#
##################################################################

EXEC = ex4_303055388_aviadle_205914039_mosheperetz

# list of all the sources files
SRCS	=  	ex2c.cpp		\
		Model.cpp 	\
		bimage.cpp  \
		ShaderIO.cpp
HDRS    =  	ex2c.h		\
        	Model.h 	\
        	bimage.h    \
        	ShaderIO.h
SHADS   = shaders/CircleShader.frag \
          shaders/CircleShader.vert \
          shaders/GouraudShader.frag \
          shaders/GouraudShader.vert \
          shaders/PhongShader.frag \
          shaders/PhongShader.vert \
          shaders/RGBShader.frag \
          shaders/RGBShader.vert \


#OpenMesh definitions
CG_HOME = /cs/course/2013/cg
OM_DIR = $(CG_HOME)/OpenMesh
OM_LIBS = -L$(OM_DIR)/lib -lOpenMeshCore
OM_INCLUDE=  -I$(OM_DIR)/include

# compiler
CC	= g++

INCLUDE = -I/usr/include -I/usr/X11R6/include -I/cs/course/current/cg/include

OBJS = $(subst .cpp,.o,$(SRCS)) 		#substituting .cc -> .o

# in order to run in debug mode use the command "make DBG=1"
ifdef DBG

	CFLAGS = 	\
		-Wall 	\
		-Werror \
		-DDEBUG	\
		-O0		\
		-g3		\
		-Wno-deprecated
					
						
else

	CFLAGS = 		\
		-Wall		\
		-DNDEBUG    \
		-O2
				 
endif


# linker
LD	= $(CC)
LDFLAGS	=  $(OM_LIBS)


# the name of the lib to create 
APP = ex4

# each lib should add with -l flag
LIBS =  -L/usr/lib -lm -lGLEW -lglut -lGL -lGLU -ldl -L/usr/X11R6/lib  -L/cs/course/2013/cg/lib -lbimage $(LDFLAGS)


# rule to change the suffix of the sources files
OBJS	= ${SRCS:.cpp=.o}

# clear out all suffixes
.SUFFIXES:
# list only those we use
.SUFFIXES: .o .cpp

# define a suffix rule for .c -> .o
.cpp.o :
	$(CC) $(CFLAGS) $(INCLUDE) $(OM_INCLUDE) -c $<

##################################
# 	default target 		##
##################################
all : $(APP)

$(APP) : $(OBJS)
	$(LD)  $(OBJS) $(LIBS) -o $(APP)

$(OBJS) : $(SRCS)


tar:
	tar cvf $(EXEC).tar Makefile $(SRCS) $(HDRS) $(SHADS) Readme.txt

zip:
	zip -r $(EXEC).zip Makefile $(SRCS) $(HDRS) $(SHADS) Readme.txt

.PHONY:
clean :
	rm -f $(OBJS) $(EXEC) *~ $(APP)
