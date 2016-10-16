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
CND_PLATFORM=Arduino-Windows
CND_DLIB_EXT=dll
CND_CONF=Debug_Arduino
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/DWM1000.o \
	${OBJECTDIR}/main.o


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
LDLIBSOPTIONS=-LC/Users/Developer/Documents/arduino-1.6.12/libraries/stlport -LC/Users/Developer/Documents/arduino-1.6.12/libraries/stlport/stl

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/dwm1000.exe

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/dwm1000.exe: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/dwm1000 ${OBJECTFILES} ${LDLIBSOPTIONS}

${OBJECTDIR}/DWM1000.o: DWM1000.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../arduino-1.6.12/libraries/stlport -I../../arduino-1.6.12/libraries/stlport/stl -I../../arduino-1.6.12/libraries/stlport/stl/config -I../../arduino-1.6.12/libraries/stlport/stl/debug -I../../arduino-1.6.12/libraries/stlport/stl/pointers -std=c++98 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/DWM1000.o DWM1000.cpp

${OBJECTDIR}/main.o: main.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../../arduino-1.6.12/libraries/stlport -I../../arduino-1.6.12/libraries/stlport/stl -I../../arduino-1.6.12/libraries/stlport/stl/config -I../../arduino-1.6.12/libraries/stlport/stl/debug -I../../arduino-1.6.12/libraries/stlport/stl/pointers -std=c++98 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/main.o main.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/dwm1000.exe

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
