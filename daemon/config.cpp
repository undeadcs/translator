
#include "includes.h"
#include "externals.h"

namespace UdSDK {

	CConfig::CConfig( ) {
	}

	CConfig::CConfig( const CConfig& objCopy ) {
	}

	CConfig::~CConfig( ) {
	}

	CConfig::ConfError CConfig::Load( const string& szPath ) {
		if ( szPath.empty( ) ) {
			return CConfig::UDERROR_INVALID_ARG;
		}

		udPStdFile pFile = fopen( szPath.c_str( ), "rb" );
		if ( !pFile ) {
			return CConfig::UDERROR_CANT_OPEN;
		}

		if ( fscanf( pFile, "%lu", &m_dwLogLevel ) == EOF ) {
			fclose( pFile );
			return CConfig::UDERROR_NOT_LOADED;
		}

		char tmp[ 256 ];

		if ( fscanf( pFile, "%s", tmp ) == EOF ) {
			fclose( pFile );
			return CConfig::UDERROR_NOT_LOADED;
		}

		m_szLogPath = tmp;

		if ( fscanf( pFile, "%s", tmp ) == EOF ) {
			fclose( pFile );
			return CConfig::UDERROR_NOT_LOADED;
		}

		m_szDumpPath = tmp;

		fclose( pFile );

		return CConfig::UDERROR_OK;
	} // ConfError Load

} // namespace UdSDK
