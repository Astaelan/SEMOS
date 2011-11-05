#ifndef _PORTIO_H
#define _PORTIO_H

#define PORTIO(bwl, bw, type)						\
static inline void out##bwl(int port, unsigned type value)		\
{									\
	__asm volatile("out" #bwl " %" #bw "0, %w1"			\
		     : : "a"(value), "Nd"(port));			\
}									\
									\
static inline unsigned type in##bwl(int port)				\
{									\
	unsigned type value;						\
	__asm volatile("in" #bwl " %w1, %" #bw "0"			\
		     : "=a"(value) : "Nd"(port));			\
	return value;							\
}									\
									\
static inline void out##bwl##_p(int port, unsigned type value)		\
{									\
	out##bwl(port, value);						\
}									\
									\
static inline unsigned type in##bwl##_p(int port)			\
{									\
	unsigned type value = in##bwl(port);				\
	return value;							\
}									\
									\
static inline void outs##bwl(int port, const void *addr, unsigned long count) \
{									\
	__asm volatile("rep; outs" #bwl					\
		     : "+S"(addr), "+c"(count) : "d"(port));		\
}									\
									\
static inline void ins##bwl(int port, void *addr, unsigned long count)	\
{									\
	__asm volatile("rep; ins" #bwl					\
		     : "+D"(addr), "+c"(count) : "d"(port));		\
}

PORTIO(b, b, char)
PORTIO(w, w, short)
PORTIO(l, , int)

#define IOWAIT()		__asm volatile( "outb %%al, $0x80" : : "a"(0) )

#endif