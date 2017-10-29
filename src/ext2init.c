#include "ext2.h"
#include "ext2session.h"

void ext2init(void* mallocFunction, void* freeFunction)
{
    mallocF=mallocFunction;
    freeF=freeFunction;
    fileHandles=mallocF(FILE_HANDLE_MAX_COUNT*sizeof(sFile));
    memset(fileHandles,0,FILE_HANDLE_MAX_COUNT*sizeof(sFile));
    sessions=mallocF(SESSION_MAX_COUNT * sizeof(sExt2Session));
    memset(sessions,0,sizeof(sExt2Session)*SESSION_MAX_COUNT);
    globalBlockBuffer=NULL;
    globalBlockBufferStartBlock=-1;
    libInitialized=true;
}

void ext2deinit(void)
{
    freeF(fileHandles);
    freeF(sessions);
    if (globalBlockBuffer!=NULL)
        freeF(globalBlockBuffer);
    libInitialized=false;
}
