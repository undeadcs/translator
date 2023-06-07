
#include "includes.h"
#include "externals.h"

namespace UdSDK {

	udStlStr StringStdToOur( const char* szString ) {
		udStlStr szRet;
		int iIndex = 0;
		char chCur = szString[ iIndex++ ];
		while( chCur != '\0' ) {
			szRet += ( udDWord ) chCur;
			chCur = szString[ iIndex++ ];
		}
		return szRet;
	} // udStlStr

	string OurStringToStd( const udStlStr& szString ) {
		string szRet;
		for( udStlStr::const_iterator i = szString.begin( ), j = szString.end( ); i != j; ++i ) {
			szRet += BYTE_4( *i );
			szRet += BYTE_3( *i );
			szRet += BYTE_2( *i );
			szRet += BYTE_1( *i );
			/*fputc( BYTE_4( dwChar ), pFile );
			fputc( BYTE_3( dwChar ), pFile );
			fputc( BYTE_2( dwChar ), pFile );
			fputc( BYTE_1( dwChar ), pFile );*/
		}
		return szRet;
	} // string OurStringToStd

	udArrWord ExplodeOne( const udStlStr& szString, udDWord dwSymbol ) {
		udArrWord arrWord;
		udStlStr::const_iterator i, j, k, itEnd = szString.end( );
		i = szString.begin( );
		j = i;
		k = i;
		for( ; i != itEnd; ++i ) {
			if ( dwSymbol == *i ) {
				k = i;
				--k;
				arrWord.push_back( udStlStr( j, k ) );
				j = i;
				++j;
			}
		}
		arrWord.push_back( udStlStr( j, k ) );
		return arrWord;
	} // udArrWord

	char* LongToString( udDWord dwNum ) {
		char* szRet = new char[ 20 ];
		int iCur = 0;
		szRet[ iCur ] = '\0';
		if ( dwNum ) {
			stack< int > objStack;
			//int iTmp;
			while( dwNum > 0 ) {
				//iTmp = dwNum % 10;
				//objStack.push( iTmp );
				objStack.push( dwNum % 10 );
				dwNum /= 10;
			}
			while( !objStack.empty( ) ) {
				//iTmp = objStack.top( );
				//szRet[ iCur++ ] = '0' + iTmp;
				szRet[ iCur++ ] = '0' + objStack.top( );
				szRet[ iCur ] = '\0';
				objStack.pop( );
			}
		} else {
			szRet[ iCur++ ] = '0';
			szRet[ iCur ] = '\0';
		}
		return szRet;
	} // char* LongToString

	void ShowMemoryInfo( const char* szLabel ) {
		g_pLog->Write( "\n*** (", 0 );
		g_pLog->Write( szLabel, 0 );
		g_pLog->Write( ") writing memory usage information ***\n", 0 );
#ifdef FOR_WIN32
		PROCESS_MEMORY_COUNTERS pmc;
		char tmp[ 1024 ];

		if ( GetProcessMemoryInfo( GetCurrentProcess( ), &pmc, sizeof( pmc ) ) ) {
			sprintf( tmp, "  PageFaultCount:             %lu\n",	pmc.PageFaultCount				);	g_pLog->Write( tmp, 0 );
			sprintf( tmp, "  PeakWorkingSetSize:         %lu\n",	pmc.PeakWorkingSetSize			);	g_pLog->Write( tmp, 0 );
			sprintf( tmp, "  WorkingSetSize:             %lu\n",	pmc.WorkingSetSize				);	g_pLog->Write( tmp, 0 );
			sprintf( tmp, "  QuotaPeakPagedPoolUsage:    %lu\n",	pmc.QuotaPeakPagedPoolUsage		);	g_pLog->Write( tmp, 0 );
			sprintf( tmp, "  QuotaPagedPoolUsage:        %lu\n",	pmc.QuotaPagedPoolUsage			);	g_pLog->Write( tmp, 0 );
			sprintf( tmp, "  QuotaPeakNonPagedPoolUsage: %lu\n",	pmc.QuotaPeakNonPagedPoolUsage	);	g_pLog->Write( tmp, 0 );
			sprintf( tmp, "  PagefileUsage:              %lu\n",	pmc.PagefileUsage				);	g_pLog->Write( tmp, 0 );
			sprintf( tmp, "  PeakPagefileUsage:          %lu\n",	pmc.PeakPagefileUsage			);	g_pLog->Write( tmp, 0 );
		} else {
			DWORD dwCode = GetLastError( );
			g_pLog->Write( "failed GetProcessMemoryInfo\n", 0 );
			g_pLog->Write( "error code: ", 0 );
			char *p = LongToString( dwCode );
			g_pLog->Write( p, 0 );
			g_pLog->Write( "\n", 0 );
			delete [ ] p;
		}
#else
		FILE *pFile = fopen( "/proc/self/statm", "r" );
		if ( pFile ) {
			unsigned long vm = 0;
			fscanf( pFile, "%lu", &vm );
			fclose( pFile );

			udDWord dwSize = ( size_t ) vm * getpagesize( );
			char *p = LongToString( dwSize );
			g_pLog->Write( "VmSize: " );
			g_pLog->Write( p, 0 );
			g_pLog->Write( "\n", 0 );
			delete [ ] p;
		} else {
			g_pLog->Write( "couldn't open /proc/self/statm\n" );
		}
#endif
		g_pLog->Write( "*** finished writing memory usage information ***\n\n", 0 );
	} // void ShowMemoryInfo

	udDWord ClearFile( const char* szFilePath ) {
		FILE *pFile = fopen( szFilePath, "w" );
		if ( pFile ) {
			fclose( pFile );
			return 1;
		}
		return 0;
	} // udDWord ClearFile

	void FatalTerminate( ) {
		if ( g_pTranslator ) {
			delete g_pTranslator;
			g_pTranslator = NULL;
		}
		if ( g_pDaemon ) {
			delete g_pDaemon;
			g_pDaemon = NULL;
		}
	} // void FatalTerminate

} // namespace UdSDK
