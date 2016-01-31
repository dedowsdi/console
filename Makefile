LIBS = \
-lboost_regex\

INCLUDES = \
-Iinclude\
-Isrc\
-I/usr/local/include\


SRCDIR = src/
SRCS = src/*.cpp

OBJDIR = obj/
OBJS =\
${OBJDIR}pacAbsDir.o\
${OBJDIR}pacArgHandler.o\
${OBJDIR}pacCommand.o\
${OBJDIR}pacConsole.o\
${OBJDIR}pacException.o\
${OBJDIR}pacIntrinsicArgHandler.o\
${OBJDIR}pacIntrinsicCmd.o\
${OBJDIR}pacStdUtil.o\
${OBJDIR}pacStringInterface.o\
${OBJDIR}pacStringUtil.o\
${OBJDIR}pacUiConsole.o

UBS = ub/*.cpp
CFLAGS = -Wall -g -std=c++11 -fPIC

#--------------------------------------------------------------------
#build shared lib
.PHONY : console
console : obj
	gcc -shared -fPIC -o bin/${@}.so ${OBJS} 

#--------------------------------------------------------------------
#implicit obj
${OBJDIR}%.o : ${SRCDIR}%.cpp
	g++  ${CFLAGS} -o $@ -c $< ${INCLUDES} ${LIBS} 
#--------------------------------------------------------------------
#build all obj
.PHONY : obj
obj : ${OBJS}
#--------------------------------------------------------------------
#--------------------------------------------------------------------
.PHONY : clean
clean: 
	rm -f console ${OBJS}
