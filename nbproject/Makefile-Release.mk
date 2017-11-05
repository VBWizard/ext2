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
CND_CONF=Release
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/src/ext2bits.o \
	${OBJECTDIR}/src/ext2block.o \
	${OBJECTDIR}/src/ext2blockgroup.o \
	${OBJECTDIR}/src/ext2dir.o \
	${OBJECTDIR}/src/ext2file.o \
	${OBJECTDIR}/src/ext2init.o \
	${OBJECTDIR}/src/ext2inode.o \
	${OBJECTDIR}/src/ext2main.o \
	${OBJECTDIR}/src/ext2read.o \
	${OBJECTDIR}/src/ext2session.o \
	${OBJECTDIR}/src/ext2write.o \
	${OBJECTDIR}/src/extutility.o


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
	${LINK.c} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libext2.${CND_DLIB_EXT} ${OBJECTFILES} ${LDLIBSOPTIONS} -fvisibility=hidden -shared -fPIC

${OBJECTDIR}/src/ext2bits.o: src/ext2bits.c 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -O2 -Iinclude -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/ext2bits.o src/ext2bits.c

${OBJECTDIR}/src/ext2block.o: src/ext2block.c 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -O2 -Iinclude -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/ext2block.o src/ext2block.c

${OBJECTDIR}/src/ext2blockgroup.o: src/ext2blockgroup.c 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -O2 -Iinclude -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/ext2blockgroup.o src/ext2blockgroup.c

${OBJECTDIR}/src/ext2dir.o: src/ext2dir.c 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -O2 -Iinclude -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/ext2dir.o src/ext2dir.c

${OBJECTDIR}/src/ext2file.o: src/ext2file.c 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -O2 -Iinclude -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/ext2file.o src/ext2file.c

${OBJECTDIR}/src/ext2init.o: src/ext2init.c 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -O2 -Iinclude -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/ext2init.o src/ext2init.c

${OBJECTDIR}/src/ext2inode.o: src/ext2inode.c 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -O2 -Iinclude -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/ext2inode.o src/ext2inode.c

${OBJECTDIR}/src/ext2main.o: src/ext2main.c 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -O2 -Iinclude -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/ext2main.o src/ext2main.c

${OBJECTDIR}/src/ext2read.o: src/ext2read.c 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -O2 -Iinclude -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/ext2read.o src/ext2read.c

${OBJECTDIR}/src/ext2session.o: src/ext2session.c 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -O2 -Iinclude -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/ext2session.o src/ext2session.c

${OBJECTDIR}/src/ext2write.o: src/ext2write.c 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -O2 -Iinclude -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/ext2write.o src/ext2write.c

${OBJECTDIR}/src/extutility.o: src/extutility.c 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -O2 -Iinclude -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/extutility.o src/extutility.c

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
