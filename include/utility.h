/* 
 * File:   utility.h
 * Author: yogi
 *
 * Created on October 16, 2017, 6:34 AM
 */

#ifndef UTILITY_H
#define	UTILITY_H

#ifdef	__cplusplus
extern "C" {
#endif

#include "ext2.h"
    
    int sectorsForByteCount(int bytes);
    bool invalidFileHandle(int FILE);

#ifdef	__cplusplus
}
#endif

#endif	/* UTILITY_H */

