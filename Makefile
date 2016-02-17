LIBS = \
-lboost_regex\
-lboost_log\
-lboost_log_setup\
-lboost_system\
-lboost_thread

INCLUDES = \
-Iinclude\
-Isrc\
-I/usr/local/include\


SRCDIR = src/
BIN = bin/

OBJDIR = obj/
OBJS =\
${OBJDIR}pacAbsDir.o\
${OBJDIR}pacArgHandler.o\
${OBJDIR}pacCommand.o\
${OBJDIR}pacConsole.o\
${OBJDIR}pacConsolePattern.o\
${OBJDIR}pacException.o\
${OBJDIR}pacIntrinsicArgHandler.o\
${OBJDIR}pacIntrinsicCmd.o\
${OBJDIR}pacCmdHistory.o\
${OBJDIR}pacStdUtil.o\
${OBJDIR}pacStringInterface.o\
${OBJDIR}pacStringUtil.o\
${OBJDIR}pacUiConsole.o\
${OBJDIR}pacLogger.o

TESTSRCDIR = test/
TESTSRCS = \
${TESTSRCDIR}testAbsDir.hpp\
${TESTSRCDIR}testArgHandler.hpp\
${TESTSRCDIR}testCommand.hpp\
${TESTSRCDIR}testConsole.hpp\
${TESTSRCDIR}testConsolePattern.hpp\
${TESTSRCDIR}test.cpp\
${TESTSRCDIR}testIntrinsicCmd.hpp\
${TESTSRCDIR}testCmdHistory.hpp\
${TESTSRCDIR}testSingleton.hpp\
${TESTSRCDIR}testStdUtil.hpp\
${TESTSRCDIR}testStringInterface.hpp\
${TESTSRCDIR}testStringUtil.hpp\
${TESTSRCDIR}testUiConsole.hpp

TESTBIN = test/bin/


UBS = ub/*.cpp
MACRO = -DBOOST_LOG_DYN_LINK 
CFLAGS = -Wall -g -std=c++11 ${MACRO}

#--------------------------------------------------------------------
#all
.PHONY : console
console: shared static test

#--------------------------------------------------------------------
#build shared lib
.PHONY : shared
shared : obj
	gcc -shared -fPIC -o bin/libconsole.so ${OBJS} 

#--------------------------------------------------------------------
#build static lib
.PHONY : static
static : obj
	ar rcs bin/libconsole.a ${OBJS}

#--------------------------------------------------------------------
#implicit obj
${OBJDIR}%.o : ${SRCDIR}%.cpp
	g++ ${CFLAGS} -fPIC -o $@ -c $< ${INCLUDES} ${LIBS} 
#--------------------------------------------------------------------
#build all obj
.PHONY : obj
obj : ${OBJS}
#--------------------------------------------------------------------
#build all test obj
.PHONY : testobj
testobj : ${TESTOBJS}
#--------------------------------------------------------------------
#build test 
.PHONY : test 
test : ${TESTSRCS}
	g++ ${CFLAGS} -pthread -o ${TESTBIN}/$@ test/test.cpp /usr/local/lib/libgtest.a\
		bin/libconsole.a ${INCLUDES} ${LIBS}
#--------------------------------------------------------------------
.PHONY : clean
clean: 
	rm -f ${BIN}/* ${TESTBIN}/* ${OBJS} 
