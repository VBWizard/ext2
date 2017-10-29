/* 
 * File:   ext2bits.h
 * Author: yogi
 *
 * Created on October 19, 2017, 7:05 AM
 */

#ifndef EXT2BITS_H
#define	EXT2BITS_H

#ifdef	__cplusplus
extern "C" {
#endif

#define BIT8 0x01
#define BIT7 0x02
#define BIT6 0x04
#define BIT5 0x08
#define BIT4 0x10
#define BIT3 0x20
#define BIT2 0x40
#define BIT1 0x80
char bits[8] = {BIT1, BIT2, BIT3, BIT4, BIT5, BIT6, BIT7, BIT8};
//eightBits &= (bits[3] & bits[4]);
#ifdef	__cplusplus
}
#endif

#endif	/* EXT2BITS_H */

