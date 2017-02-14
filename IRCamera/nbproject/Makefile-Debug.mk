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
CC=gcc-6.2.0
CCC=cpp-6.2.0
CXX=cpp-6.2.0
FC=gfortran
AS=gcc-ar-6.2.0

# Macros
CND_PLATFORM=GNU-6.2.0-Linux
CND_DLIB_EXT=so
CND_CONF=Debug
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/_ext/f30a23f9/Utilities.o \
	${OBJECTDIR}/CameraThread.o \
	${OBJECTDIR}/DWM1000Thread.o \
	${OBJECTDIR}/decadriver/deca_device.o \
	${OBJECTDIR}/decadriver/deca_params_init.o \
	${OBJECTDIR}/main.o

# Test Directory
TESTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}/tests

# Test Files
TESTFILES= \
	${TESTDIR}/TestFiles/f1

# Test Object Files
TESTOBJECTFILES= \
	${TESTDIR}/tests/CameraThreadTest.o

# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=-pthread
CXXFLAGS=-pthread

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=-L/C/SysGCC/Raspberry/arm-linux-gnueabihf/sysroot/usr/local/lib

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/ircamera

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/ircamera: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/ircamera ${OBJECTFILES} ${LDLIBSOPTIONS}

${OBJECTDIR}/_ext/f30a23f9/Utilities.o: ../raspberrypi_omxcam/include/Utilities.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/f30a23f9
	${RM} "$@.d"
	$(COMPILE.cc) -g -I/C/SysGCC/Raspberry/lib/gcc/arm-linux-gnueabihf/4.9/include -I/C/SysGCC/Raspberry/lib/gcc/arm-linux-gnueabihf/4.9/include-fixed -I/C/SysGCC/Raspberry/arm-linux-gnueabihf/include -I/C/SysGCC/Raspberry/arm-linux-gnueabihf/sysroot/usr/include -I/C/SysGCC/Raspberry/arm-linux-gnueabihf/sysroot/usr/include/arm-linux-gnueabihf -I/C/SysGCC/Raspberry/arm-linux-gnueabihf/sysroot/usr/local/include -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/f30a23f9/Utilities.o ../raspberrypi_omxcam/include/Utilities.cpp

${OBJECTDIR}/CameraThread.o: CameraThread.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -I/C/SysGCC/Raspberry/lib/gcc/arm-linux-gnueabihf/4.9/include -I/C/SysGCC/Raspberry/lib/gcc/arm-linux-gnueabihf/4.9/include-fixed -I/C/SysGCC/Raspberry/arm-linux-gnueabihf/include -I/C/SysGCC/Raspberry/arm-linux-gnueabihf/sysroot/usr/include -I/C/SysGCC/Raspberry/arm-linux-gnueabihf/sysroot/usr/include/arm-linux-gnueabihf -I/C/SysGCC/Raspberry/arm-linux-gnueabihf/sysroot/usr/local/include -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CameraThread.o CameraThread.cpp

${OBJECTDIR}/DWM1000Thread.o: DWM1000Thread.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -I/C/SysGCC/Raspberry/lib/gcc/arm-linux-gnueabihf/4.9/include -I/C/SysGCC/Raspberry/lib/gcc/arm-linux-gnueabihf/4.9/include-fixed -I/C/SysGCC/Raspberry/arm-linux-gnueabihf/include -I/C/SysGCC/Raspberry/arm-linux-gnueabihf/sysroot/usr/include -I/C/SysGCC/Raspberry/arm-linux-gnueabihf/sysroot/usr/include/arm-linux-gnueabihf -I/C/SysGCC/Raspberry/arm-linux-gnueabihf/sysroot/usr/local/include -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/DWM1000Thread.o DWM1000Thread.cpp

${OBJECTDIR}/decadriver/deca_device.o: decadriver/deca_device.c
	${MKDIR} -p ${OBJECTDIR}/decadriver
	${RM} "$@.d"
	$(COMPILE.c) -g -I/C/SysGCC/Raspberry/lib/gcc/arm-linux-gnueabihf/4.9/include -I/C/SysGCC/Raspberry/lib/gcc/arm-linux-gnueabihf/4.9/include-fixed -I/C/SysGCC/Raspberry/arm-linux-gnueabihf/include -I/C/SysGCC/Raspberry/arm-linux-gnueabihf/sysroot/usr/include -I/C/SysGCC/Raspberry/arm-linux-gnueabihf/sysroot/usr/include/arm-linux-gnueabihf -I/C/SysGCC/Raspberry/arm-linux-gnueabihf/sysroot/usr/local/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/decadriver/deca_device.o decadriver/deca_device.c

${OBJECTDIR}/decadriver/deca_params_init.o: decadriver/deca_params_init.c
	${MKDIR} -p ${OBJECTDIR}/decadriver
	${RM} "$@.d"
	$(COMPILE.c) -g -I/C/SysGCC/Raspberry/lib/gcc/arm-linux-gnueabihf/4.9/include -I/C/SysGCC/Raspberry/lib/gcc/arm-linux-gnueabihf/4.9/include-fixed -I/C/SysGCC/Raspberry/arm-linux-gnueabihf/include -I/C/SysGCC/Raspberry/arm-linux-gnueabihf/sysroot/usr/include -I/C/SysGCC/Raspberry/arm-linux-gnueabihf/sysroot/usr/include/arm-linux-gnueabihf -I/C/SysGCC/Raspberry/arm-linux-gnueabihf/sysroot/usr/local/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/decadriver/deca_params_init.o decadriver/deca_params_init.c

${OBJECTDIR}/main.o: main.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -I/C/SysGCC/Raspberry/lib/gcc/arm-linux-gnueabihf/4.9/include -I/C/SysGCC/Raspberry/lib/gcc/arm-linux-gnueabihf/4.9/include-fixed -I/C/SysGCC/Raspberry/arm-linux-gnueabihf/include -I/C/SysGCC/Raspberry/arm-linux-gnueabihf/sysroot/usr/include -I/C/SysGCC/Raspberry/arm-linux-gnueabihf/sysroot/usr/include/arm-linux-gnueabihf -I/C/SysGCC/Raspberry/arm-linux-gnueabihf/sysroot/usr/local/include -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/main.o main.cpp

# Subprojects
.build-subprojects:

# Build Test Targets
.build-tests-conf: .build-tests-subprojects .build-conf ${TESTFILES}
.build-tests-subprojects:

${TESTDIR}/TestFiles/f1: ${TESTDIR}/tests/CameraThreadTest.o ${OBJECTFILES:%.o=%_nomain.o}
	${MKDIR} -p ${TESTDIR}/TestFiles
	${LINK.cc} -o ${TESTDIR}/TestFiles/f1 $^ ${LDLIBSOPTIONS}   


${TESTDIR}/tests/CameraThreadTest.o: tests/CameraThreadTest.cpp 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} "$@.d"
	$(COMPILE.cc) -g -I/C/SysGCC/Raspberry/lib/gcc/arm-linux-gnueabihf/4.9/include -I/C/SysGCC/Raspberry/lib/gcc/arm-linux-gnueabihf/4.9/include-fixed -I/C/SysGCC/Raspberry/arm-linux-gnueabihf/include -I/C/SysGCC/Raspberry/arm-linux-gnueabihf/sysroot/usr/include -I/C/SysGCC/Raspberry/arm-linux-gnueabihf/sysroot/usr/include/arm-linux-gnueabihf -I/C/SysGCC/Raspberry/arm-linux-gnueabihf/sysroot/usr/local/include -I. -std=c++11 -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/CameraThreadTest.o tests/CameraThreadTest.cpp


${OBJECTDIR}/_ext/f30a23f9/Utilities_nomain.o: ${OBJECTDIR}/_ext/f30a23f9/Utilities.o ../raspberrypi_omxcam/include/Utilities.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/f30a23f9
	@NMOUTPUT=`${NM} ${OBJECTDIR}/_ext/f30a23f9/Utilities.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g -I/C/SysGCC/Raspberry/lib/gcc/arm-linux-gnueabihf/4.9/include -I/C/SysGCC/Raspberry/lib/gcc/arm-linux-gnueabihf/4.9/include-fixed -I/C/SysGCC/Raspberry/arm-linux-gnueabihf/include -I/C/SysGCC/Raspberry/arm-linux-gnueabihf/sysroot/usr/include -I/C/SysGCC/Raspberry/arm-linux-gnueabihf/sysroot/usr/include/arm-linux-gnueabihf -I/C/SysGCC/Raspberry/arm-linux-gnueabihf/sysroot/usr/local/include -std=c++11 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/f30a23f9/Utilities_nomain.o ../raspberrypi_omxcam/include/Utilities.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/_ext/f30a23f9/Utilities.o ${OBJECTDIR}/_ext/f30a23f9/Utilities_nomain.o;\
	fi

${OBJECTDIR}/CameraThread_nomain.o: ${OBJECTDIR}/CameraThread.o CameraThread.cpp 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/CameraThread.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g -I/C/SysGCC/Raspberry/lib/gcc/arm-linux-gnueabihf/4.9/include -I/C/SysGCC/Raspberry/lib/gcc/arm-linux-gnueabihf/4.9/include-fixed -I/C/SysGCC/Raspberry/arm-linux-gnueabihf/include -I/C/SysGCC/Raspberry/arm-linux-gnueabihf/sysroot/usr/include -I/C/SysGCC/Raspberry/arm-linux-gnueabihf/sysroot/usr/include/arm-linux-gnueabihf -I/C/SysGCC/Raspberry/arm-linux-gnueabihf/sysroot/usr/local/include -std=c++11 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CameraThread_nomain.o CameraThread.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/CameraThread.o ${OBJECTDIR}/CameraThread_nomain.o;\
	fi

${OBJECTDIR}/DWM1000Thread_nomain.o: ${OBJECTDIR}/DWM1000Thread.o DWM1000Thread.cpp 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/DWM1000Thread.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g -I/C/SysGCC/Raspberry/lib/gcc/arm-linux-gnueabihf/4.9/include -I/C/SysGCC/Raspberry/lib/gcc/arm-linux-gnueabihf/4.9/include-fixed -I/C/SysGCC/Raspberry/arm-linux-gnueabihf/include -I/C/SysGCC/Raspberry/arm-linux-gnueabihf/sysroot/usr/include -I/C/SysGCC/Raspberry/arm-linux-gnueabihf/sysroot/usr/include/arm-linux-gnueabihf -I/C/SysGCC/Raspberry/arm-linux-gnueabihf/sysroot/usr/local/include -std=c++11 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/DWM1000Thread_nomain.o DWM1000Thread.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/DWM1000Thread.o ${OBJECTDIR}/DWM1000Thread_nomain.o;\
	fi

${OBJECTDIR}/decadriver/deca_device_nomain.o: ${OBJECTDIR}/decadriver/deca_device.o decadriver/deca_device.c 
	${MKDIR} -p ${OBJECTDIR}/decadriver
	@NMOUTPUT=`${NM} ${OBJECTDIR}/decadriver/deca_device.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.c) -g -I/C/SysGCC/Raspberry/lib/gcc/arm-linux-gnueabihf/4.9/include -I/C/SysGCC/Raspberry/lib/gcc/arm-linux-gnueabihf/4.9/include-fixed -I/C/SysGCC/Raspberry/arm-linux-gnueabihf/include -I/C/SysGCC/Raspberry/arm-linux-gnueabihf/sysroot/usr/include -I/C/SysGCC/Raspberry/arm-linux-gnueabihf/sysroot/usr/include/arm-linux-gnueabihf -I/C/SysGCC/Raspberry/arm-linux-gnueabihf/sysroot/usr/local/include -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/decadriver/deca_device_nomain.o decadriver/deca_device.c;\
	else  \
	    ${CP} ${OBJECTDIR}/decadriver/deca_device.o ${OBJECTDIR}/decadriver/deca_device_nomain.o;\
	fi

${OBJECTDIR}/decadriver/deca_params_init_nomain.o: ${OBJECTDIR}/decadriver/deca_params_init.o decadriver/deca_params_init.c 
	${MKDIR} -p ${OBJECTDIR}/decadriver
	@NMOUTPUT=`${NM} ${OBJECTDIR}/decadriver/deca_params_init.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.c) -g -I/C/SysGCC/Raspberry/lib/gcc/arm-linux-gnueabihf/4.9/include -I/C/SysGCC/Raspberry/lib/gcc/arm-linux-gnueabihf/4.9/include-fixed -I/C/SysGCC/Raspberry/arm-linux-gnueabihf/include -I/C/SysGCC/Raspberry/arm-linux-gnueabihf/sysroot/usr/include -I/C/SysGCC/Raspberry/arm-linux-gnueabihf/sysroot/usr/include/arm-linux-gnueabihf -I/C/SysGCC/Raspberry/arm-linux-gnueabihf/sysroot/usr/local/include -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/decadriver/deca_params_init_nomain.o decadriver/deca_params_init.c;\
	else  \
	    ${CP} ${OBJECTDIR}/decadriver/deca_params_init.o ${OBJECTDIR}/decadriver/deca_params_init_nomain.o;\
	fi

${OBJECTDIR}/main_nomain.o: ${OBJECTDIR}/main.o main.cpp 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/main.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g -I/C/SysGCC/Raspberry/lib/gcc/arm-linux-gnueabihf/4.9/include -I/C/SysGCC/Raspberry/lib/gcc/arm-linux-gnueabihf/4.9/include-fixed -I/C/SysGCC/Raspberry/arm-linux-gnueabihf/include -I/C/SysGCC/Raspberry/arm-linux-gnueabihf/sysroot/usr/include -I/C/SysGCC/Raspberry/arm-linux-gnueabihf/sysroot/usr/include/arm-linux-gnueabihf -I/C/SysGCC/Raspberry/arm-linux-gnueabihf/sysroot/usr/local/include -std=c++11 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/main_nomain.o main.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/main.o ${OBJECTDIR}/main_nomain.o;\
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

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
