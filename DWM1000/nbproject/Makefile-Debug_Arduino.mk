#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Environment
MKDIR=mkdir
CP=cp
GREP=grep
NM=nm
CCADMIN=CCadmin
RANLIB=ranlib
CC=gcc
CCC=g++
CXX=g++
FC=gfortran
AS=as

# Macros
CND_PLATFORM=GNU-Linux
CND_DLIB_EXT=so
CND_CONF=Debug_Arduino
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/DWM1000.o

# Test Directory
TESTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}/tests

# Test Files
TESTFILES= \
	${TESTDIR}/TestFiles/f1

# Test Object Files
TESTOBJECTFILES= \
	${TESTDIR}/tests/Basic\ Test.o

# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=-fno-exceptions
CXXFLAGS=-fno-exceptions

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=-L/C/Users/Developer/Documents/arduino-1.6.12/libraries/stlport -L/C/Users/Developer/Documents/arduino-1.6.12/libraries/stlport/stl

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libdwm1000.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libdwm1000.a: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libdwm1000.a
	${AR} -rv ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libdwm1000.a ${OBJECTFILES} 
	$(RANLIB) ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libdwm1000.a

${OBJECTDIR}/DWM1000.o: DWM1000.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../arduino-1.6.12/libraries/stlport -I../../arduino-1.6.12/libraries/stlport/stl -I../../arduino-1.6.12/libraries/stlport/stl/config -I../../arduino-1.6.12/libraries/stlport/stl/debug -I../../arduino-1.6.12/libraries/stlport/stl/pointers -std=c++98 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/DWM1000.o DWM1000.cpp

# Subprojects
.build-subprojects:

# Build Test Targets
.build-tests-conf: .build-tests-subprojects .build-conf ${TESTFILES}
.build-tests-subprojects:

${TESTDIR}/TestFiles/f1: ${TESTDIR}/tests/Basic\ Test.o ${OBJECTFILES:%.o=%_nomain.o}
	${MKDIR} -p ${TESTDIR}/TestFiles
	${LINK.cc}   -o ${TESTDIR}/TestFiles/f1 $^ ${LDLIBSOPTIONS} 


.NO_PARALLEL:${TESTDIR}/tests/Basic\ Test.o
${TESTDIR}/tests/Basic\ Test.o: tests/Basic\ Test.cpp 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../arduino-1.6.12/libraries/stlport -I../../arduino-1.6.12/libraries/stlport/stl -I../../arduino-1.6.12/libraries/stlport/stl/config -I../../arduino-1.6.12/libraries/stlport/stl/debug -I../../arduino-1.6.12/libraries/stlport/stl/pointers -I. -std=c++98 -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/Basic\ Test.o tests/Basic\ Test.cpp


${OBJECTDIR}/DWM1000_nomain.o: ${OBJECTDIR}/DWM1000.o DWM1000.cpp 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/DWM1000.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g -I../../arduino-1.6.12/libraries/stlport -I../../arduino-1.6.12/libraries/stlport/stl -I../../arduino-1.6.12/libraries/stlport/stl/config -I../../arduino-1.6.12/libraries/stlport/stl/debug -I../../arduino-1.6.12/libraries/stlport/stl/pointers -std=c++98 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/DWM1000_nomain.o DWM1000.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/DWM1000.o ${OBJECTDIR}/DWM1000_nomain.o;\
	fi

# Run Test Targets
.test-conf:
	@if [ "${TEST}" = "" ]; \
	then  \
	    ${TESTDIR}/TestFiles/f1 || true; \
	else  \
	    ./${TEST} || true; \
	fi

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libdwm1000.a

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
