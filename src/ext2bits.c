/*Parts from http://www.mathcs.emory.edu/~cheung/Courses/255/Syllabus/1-C-intro/bit-array.html*/

#define ClearBit(Array,bit)   ( Array[(bit/32)] &= ~(1 << (bit%32)) ) 
#define TestBit(Array,bit)    ( Array[(bit/32)] & (1 << (bit%32)) ) 
#define SetBit(Array,bit)     ( Array[(bit/32)] |= (1 << (bit%32)) )
