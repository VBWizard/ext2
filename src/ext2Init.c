#include "ext2.h"

void libInit(void* mallocFunction, void* freeFunction)
{
    mallocF=mallocFunction;
    freeF=freeFunction;
    fileHandles=mallocF(FILE_HANDLE_MAX_COUNT*sizeof(sFile));
    memset(fileHandles,0,FILE_HANDLE_MAX_COUNT*sizeof(sFile));
    sessions=mallocF(SESSION_MAX_COUNT * sizeof(sExt2Session));
    memset(sessions,0,sizeof(sExt2Session)*SESSION_MAX_COUNT);
    readBuffer=NULL;
    readBufferStartBlock=-1;
    libInitialized=true;
}

void libClose(void)
{
    freeF(fileHandles);
    freeF(sessions);
    if (readBuffer!=NULL)
        freeF(readBuffer);
    libInitialized=false;
}

