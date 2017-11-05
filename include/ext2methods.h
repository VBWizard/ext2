/* 
 * File:   ext2methods.h
 * Author: yogi
 *
 * Created on October 29, 2017, 2:49 PM
 */

#ifndef EXT2METHODS_H
#define	EXT2METHODS_H

#ifdef	__cplusplus
extern "C" {
#endif

#include "ext2file.h"

__attribute__((visibility("default"))) int ext2open (int sess, char* fileName);
__attribute__((visibility("default"))) int ext2close(int FILE);
__attribute__((visibility("default"))) int ext2seek (int FILE, int pos, int whence);
__attribute__((visibility("default"))) void ext2init(void* mallocFunction, void* freeFunction);
__attribute__((visibility("default"))) void ext2deinit(void);
__attribute__((visibility("default"))) int ext2initsession(void* readFunction, void* writeFunction);
__attribute__((visibility("default"))) int ext2write (int FILE, char* buffer, int bytes);
__attribute__((visibility("default"))) int ext2read(int FILE, char* buffer, int bytes);


#ifdef	__cplusplus
}
#endif

#endif	/* EXT2METHODS_H */

