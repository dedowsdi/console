LIBS = \
-lOIS\
-lboost_system\
-lOgreMain\
-lOgreOverlay\

INCLUDES = \
-Iinclude\
-Isrc\
-I/usr/local/include\
-I/usr/include/OIS\
-I/usr/local/include/OGRE\
-I/usr/local/include/OGRE/Overlay\


SRCDIR = src/
SRCS = src/*.cpp

OBJDIR = obj/
OBJS =\
${OBJDIR}BaseApplication.o\
${OBJDIR}TutorialApplication.o 

UBS = ub/*.cpp
CFLAGS = -Wall -g

#--------------------------------------------------------------------
console: ${OBJS} 
	g++  ${CFLAGS} -o $@ ${OBJS} ${INCLUDES} ${LIBS} 

#--------------------------------------------------------------------
#implicit obj
${OBJDIR}%.o : ${SRCDIR}%.cpp
	g++  ${CFLAGS} -o $@ -c $< ${INCLUDES} ${LIBS} 
#--------------------------------------------------------------------
#build all obj
.PHONY : obj
obj : ${OBJS}
#--------------------------------------------------------------------
.PHONY : clean
clean: 
	rm -f console ${OBJS}
