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
CND_PLATFORM=GNU-Linux-x86
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
	${OBJECTDIR}/main.o \
	${OBJECTDIR}/source/Client.o \
	${OBJECTDIR}/source/RakVoice.o \
	${OBJECTDIR}/source/Server.o \
	${OBJECTDIR}/source/Video.o \
	${OBJECTDIR}/source/Voice.o


# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=-std=c++11 -O0 -g3 -Wall
CXXFLAGS=-std=c++11 -O0 -g3 -Wall

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=-LLib -Wl,-rpath,Lib -lRakNetDLL -lspeex -lspeexdsp -lportaudio -lopencv_core -lopencv_highgui -lopencv_video

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/raknet-demo

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/raknet-demo: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/raknet-demo ${OBJECTFILES} ${LDLIBSOPTIONS}

${OBJECTDIR}/main.o: main.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -Iinclude -Iinclude/raknet -I/usr/include/c++/4.8.2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/main.o main.cpp

${OBJECTDIR}/source/Client.o: source/Client.cpp 
	${MKDIR} -p ${OBJECTDIR}/source
	${RM} "$@.d"
	$(COMPILE.cc) -g -Iinclude -Iinclude/raknet -I/usr/include/c++/4.8.2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/source/Client.o source/Client.cpp

${OBJECTDIR}/source/RakVoice.o: source/RakVoice.cpp 
	${MKDIR} -p ${OBJECTDIR}/source
	${RM} "$@.d"
	$(COMPILE.cc) -g -Iinclude -Iinclude/raknet -I/usr/include/c++/4.8.2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/source/RakVoice.o source/RakVoice.cpp

${OBJECTDIR}/source/Server.o: source/Server.cpp 
	${MKDIR} -p ${OBJECTDIR}/source
	${RM} "$@.d"
	$(COMPILE.cc) -g -Iinclude -Iinclude/raknet -I/usr/include/c++/4.8.2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/source/Server.o source/Server.cpp

${OBJECTDIR}/source/Video.o: source/Video.cpp 
	${MKDIR} -p ${OBJECTDIR}/source
	${RM} "$@.d"
	$(COMPILE.cc) -g -Iinclude -Iinclude/raknet -I/usr/include/c++/4.8.2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/source/Video.o source/Video.cpp

${OBJECTDIR}/source/Voice.o: source/Voice.cpp 
	${MKDIR} -p ${OBJECTDIR}/source
	${RM} "$@.d"
	$(COMPILE.cc) -g -Iinclude -Iinclude/raknet -I/usr/include/c++/4.8.2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/source/Voice.o source/Voice.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/raknet-demo

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
