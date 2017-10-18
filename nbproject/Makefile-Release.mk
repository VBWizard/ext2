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
CND_PLATFORM=cross-Linux
CND_DLIB_EXT=so
CND_CONF=Release
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/src/ext2Init.o \
	${OBJECTDIR}/src/ext2Main.o \
	${OBJECTDIR}/src/ext2blockgroup.o \
	${OBJECTDIR}/src/ext2dir.o \
	${OBJECTDIR}/src/ext2inode.o \
	${OBJECTDIR}/src/ext2read.o \
	${OBJECTDIR}/src/utility.o


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
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libext2.${CND_DLIB_EXT}

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libext2.${CND_DLIB_EXT}: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.c} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libext2.${CND_DLIB_EXT} ${OBJECTFILES} ${LDLIBSOPTIONS} -shared -fPIC

${OBJECTDIR}/src/ext2Init.o: src/ext2Init.c 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -O2 -Iinclude -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/ext2Init.o src/ext2Init.c

${OBJECTDIR}/src/ext2Main.o: src/ext2Main.c 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -O2 -Iinclude -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/ext2Main.o src/ext2Main.c

${OBJECTDIR}/src/ext2blockgroup.o: src/ext2blockgroup.c 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -O2 -Iinclude -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/ext2blockgroup.o src/ext2blockgroup.c

${OBJECTDIR}/src/ext2dir.o: src/ext2dir.c 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -O2 -Iinclude -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/ext2dir.o src/ext2dir.c

${OBJECTDIR}/src/ext2inode.o: src/ext2inode.c 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -O2 -Iinclude -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/ext2inode.o src/ext2inode.c

${OBJECTDIR}/src/ext2read.o: src/ext2read.c 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -O2 -Iinclude -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/ext2read.o src/ext2read.c

${OBJECTDIR}/src/utility.o: src/utility.c 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -O2 -Iinclude -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/utility.o src/utility.c

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libext2.${CND_DLIB_EXT}

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
