
#define LOG_CPP

#include "includes.h"
#include "externals.h"

namespace UdSDK {

	CLog::CLog( ) {
		m_szPath	= "";
		m_pFile		= NULL;
	}

	CLog::CLog( const CLog& objCopy ) {
	}

	CLog::~CLog( ) {
		m_szPath.clear( );

		if ( m_pFile ) {
			fclose( m_pFile );
			m_pFile = NULL;
		}
	}

	CLog::LogError CLog::Open( const string& szPath ) {
		if ( szPath.empty( ) ) {
			return CLog::UDERROR_INVALID_ARG;
		}

		m_pFile = fopen( szPath.c_str( ), "ab" );
		if ( m_pFile ) {
			m_szPath = szPath;
			return CLog::UDERROR_OK;
		}

		return CLog::UDERROR_CANT_OPEN_LOG;
	} // LogError Open

	CLog::LogError CLog::Close( ) {
		if ( m_pFile ) {
			fclose( m_pFile );
			m_pFile = NULL;
			m_szPath.clear( );
			return CLog::UDERROR_OK;
		}

		return CLog::UDERROR_NOT_OPENED;
	} // LogErorr Close

	CLog::LogError CLog::Write( const string& szText, udDWord dwShowTime ) {
		if ( szText.empty( ) ) {
			return CLog::UDERROR_INVALID_ARG;
		}
		if ( !m_pFile ) {
			return CLog::UDERROR_NOT_OPENED;
		}

		if ( dwShowTime ) {
			time_t t;
			tm* pTime;
			time( &t );
			pTime = localtime( &t );
			char buf[ 80 ];
			strftime( buf, 80, "[%Y.%m.%d %H:%M:%S] ", pTime );
			fwrite( buf, 1, strlen( buf ), m_pFile );
		}

		size_t iSize = szText.size( ), iResult = 0;

		iResult = fwrite( szText.c_str( ), 1, iSize, m_pFile );
		if ( iResult != iSize ) {
			return CLog::UDERROR_WRITE_LOG_FAILED;
		}

		fflush( m_pFile );

		return CLog::UDERROR_OK;
	} // LogError Write

} // namespace UdSDK

