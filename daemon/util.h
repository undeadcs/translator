
#ifndef UTIL_H
#define UTIL_H

namespace UdSDK {

	udStlStr	StringStdToOur( const char* szString );
	string		OurStringToStd( const udStlStr& szString );
	udArrWord	ExplodeOne( const udStlStr& szString, udDWord dwSymbol );
	char*		LongToString( udDWord dwNum );
	void		ShowMemoryInfo( const char* szLabel );
	udDWord		ClearFile( const char* szFilePath );
	void		FatalTerminate( );

} // namespace UdSDK

#endif
