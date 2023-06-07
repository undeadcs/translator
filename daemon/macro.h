
#ifndef MACRO_H
#define MACRO_H

// байты справа налево 4321
#define BYTE_1( tmp )		( ( tmp << 24 ) >> 24 )
#define BYTE_2( tmp )		( ( tmp << 16 ) >> 24 )
#define BYTE_3( tmp )		( ( tmp << 8  ) >> 24 )
#define BYTE_4( tmp )		( tmp >> 24 )
// x - число от 4 до 1
#define BYTE_X( tmp, x )	( ( tmp << ( ( 4 - x ) * 8 ) ) >> 24 )

#endif
