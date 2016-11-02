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
CC=arm-linux-gnueabihf-gcc
CCC=arm-linux-gnueabihf-g++
CXX=arm-linux-gnueabihf-g++
FC=gfortran
AS=arm-linux-gnueabihf-as

# Macros
CND_PLATFORM=SysGCC_RPi-Windows
CND_DLIB_EXT=dll
CND_CONF=Debug
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/cameraThread.o \
	${OBJECTDIR}/main.o


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
LDLIBSOPTIONS=-L/C/SysGCC/Raspberry/arm-linux-gnueabihf/sysroot/usr/local/lib

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/ircamera.exe

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/ircamera.exe: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/ircamera ${OBJECTFILES} ${LDLIBSOPTIONS}

${OBJECTDIR}/cameraThread.o: cameraThread.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -I/C/SysGCC/Raspberry/lib/gcc/arm-linux-gnueabihf/4.9/include -I/C/SysGCC/Raspberry/lib/gcc/arm-linux-gnueabihf/4.9/include-fixed -I/C/SysGCC/Raspberry/arm-linux-gnueabihf/include -I/C/SysGCC/Raspberry/arm-linux-gnueabihf/sysroot/usr/include -I/C/SysGCC/Raspberry/arm-linux-gnueabihf/sysroot/usr/include/arm-linux-gnueabihf -I/C/SysGCC/Raspberry/arm-linux-gnueabihf/sysroot/usr/local/include -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/cameraThread.o cameraThread.cpp

${OBJECTDIR}/main.o: main.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -I/C/SysGCC/Raspberry/lib/gcc/arm-linux-gnueabihf/4.9/include -I/C/SysGCC/Raspberry/lib/gcc/arm-linux-gnueabihf/4.9/include-fixed -I/C/SysGCC/Raspberry/arm-linux-gnueabihf/include -I/C/SysGCC/Raspberry/arm-linux-gnueabihf/sysroot/usr/include -I/C/SysGCC/Raspberry/arm-linux-gnueabihf/sysroot/usr/include/arm-linux-gnueabihf -I/C/SysGCC/Raspberry/arm-linux-gnueabihf/sysroot/usr/local/include -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/main.o main.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/ircamera.exe

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
