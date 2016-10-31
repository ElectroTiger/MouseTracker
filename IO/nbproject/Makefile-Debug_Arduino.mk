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
CC=avr-gcc
CCC=avr-g++
CXX=avr-g++
FC=gfortran
AS=avr-as

# Macros
CND_PLATFORM=Arduino_AVR-Windows
CND_DLIB_EXT=dll
CND_CONF=Debug_Arduino
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES=

# Test Directory
TESTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}/tests

# Test Files
TESTFILES= \
	${TESTDIR}/TestFiles/f2 \
	${TESTDIR}/TestFiles/f1

# Test Object Files
TESTOBJECTFILES= \
	${TESTDIR}/wiringPiHeaders/tests/RpiGPIOTest.o \
	${TESTDIR}/wiringPiHeaders/tests/RpiSPITest.o

# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=
CXXFLAGS=

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libio.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libio.a: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libio.a
	${AR} -rv ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libio.a ${OBJECTFILES} 
	$(RANLIB) ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libio.a

# Subprojects
.build-subprojects:

# Build Test Targets
.build-tests-conf: .build-tests-subprojects .build-conf ${TESTFILES}
.build-tests-subprojects:

${TESTDIR}/TestFiles/f2: ${TESTDIR}/wiringPiHeaders/tests/RpiGPIOTest.o ${OBJECTFILES:%.o=%_nomain.o}
	${MKDIR} -p ${TESTDIR}/TestFiles
	${LINK.cc}   -o ${TESTDIR}/TestFiles/f2 $^ ${LDLIBSOPTIONS} `cppunit-config --libs`   

${TESTDIR}/TestFiles/f1: ${TESTDIR}/wiringPiHeaders/tests/RpiSPITest.o ${OBJECTFILES:%.o=%_nomain.o}
	${MKDIR} -p ${TESTDIR}/TestFiles
	${LINK.cc}   -o ${TESTDIR}/TestFiles/f1 $^ ${LDLIBSOPTIONS} `cppunit-config --libs`   


${TESTDIR}/wiringPiHeaders/tests/RpiGPIOTest.o: wiringPiHeaders/tests/RpiGPIOTest.cpp 
	${MKDIR} -p ${TESTDIR}/wiringPiHeaders/tests
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../../arduino-1.6.12/libraries/stlport/stl -I../../../arduino-1.6.12/libraries/stlport -I../../../arduino-1.6.12/libraries/stlport/stl/config -I../../../arduino-1.6.12/libraries/stlport/stl/debug -I../../../arduino-1.6.12/libraries/stlport/stl/pointers -I. -std=c++11 `cppunit-config --cflags` -MMD -MP -MF "$@.d" -o ${TESTDIR}/wiringPiHeaders/tests/RpiGPIOTest.o wiringPiHeaders/tests/RpiGPIOTest.cpp


${TESTDIR}/wiringPiHeaders/tests/RpiSPITest.o: wiringPiHeaders/tests/RpiSPITest.cpp 
	${MKDIR} -p ${TESTDIR}/wiringPiHeaders/tests
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../../arduino-1.6.12/libraries/stlport/stl -I../../../arduino-1.6.12/libraries/stlport -I../../../arduino-1.6.12/libraries/stlport/stl/config -I../../../arduino-1.6.12/libraries/stlport/stl/debug -I../../../arduino-1.6.12/libraries/stlport/stl/pointers -I. -std=c++11 `cppunit-config --cflags` -MMD -MP -MF "$@.d" -o ${TESTDIR}/wiringPiHeaders/tests/RpiSPITest.o wiringPiHeaders/tests/RpiSPITest.cpp


# Run Test Targets
.test-conf:
	@if [ "${TEST}" = "" ]; \
	then  \
	    ${TESTDIR}/TestFiles/f2 || true; \
	    ${TESTDIR}/TestFiles/f1 || true; \
	else  \
	    ./${TEST} || true; \
	fi

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libio.a

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
