
#define CONVERTER_CPP

#include "includes.h"
#include "externals.h"

namespace UdSDK {

	CConverter::CConverter( ) {
		m_hIconv = NULL;
	}

	CConverter::CConverter( const CConverter& objCopy ) {
	}

	CConverter::~CConverter( ) {
		if ( m_hIconv ) {
			iconv_close( m_hIconv );
		}
	}

	udDWord CConverter::Create( string szInpEnc, string szOutEnc ) {
		m_hIconv = iconv_open( szInpEnc.c_str( ), szOutEnc.c_str( ) );
		if ( m_hIconv == ( iconv_t ) ( -1 ) ) {
			return 0;
		}
		return 1;
	} // udDWord Create

	udDWord CConverter::Destroy( ) {
		if ( m_hIconv ) {
			int iRes = iconv_close( m_hIconv );
			if ( iRes == -1 ) {
				return 0;
			}
			return 1;
		}
		return 2;
	} // udDWord Destroy

	udDWord CConverter::ConvertFile( const char* szFileIn, const char* szFileOut ) {
		FILE* pFileIn = fopen( szFileIn, "rb" );
		if ( pFileIn ) {
			FILE* pFileOut = fopen( szFileOut, "wb" );
			if ( pFileOut ) {
				string szInputText;
				char cChar = fgetc( pFileIn );
				while( !feof( pFileIn ) ) {
					szInputText += cChar;
					cChar = fgetc( pFileIn );
				}

				size_t iLen1 = szInputText.size( ), iLen2 = iLen1 * 4, iInBytes = iLen1, iOutBytes = iLen2;
				udDWord dwResult = 0;
				char *szText = new char[ iLen1 ], *szOut = new char[ iLen2 ];

				memcpy( szText, szInputText.c_str( ), iLen1 );
				dwResult = this->Convert( szText, iInBytes, szOut, iOutBytes );
				if ( dwResult ) {
					size_t iBytes = iLen2 - iOutBytes;
					for( size_t i = 0; i < iBytes; ++i ) {
						fputc( szOut[ i ], pFileOut );
					}
				}

				delete [ ] szOut;
				delete [ ] szText;

				fclose( pFileOut );

				if ( !dwResult ) {
					fclose( pFileIn );
					return 0;
				}
			} else {
				fclose( pFileIn );
				return 0;
			}
			fclose( pFileIn );
			return 1;
		}
		return 0;
	} // udDWord ConvertFile

	udDWord CConverter::Convert(
#ifdef FOR_WIN32
		const char* szIn,
#else
		char* szIn,
#endif
		size_t& iInputLen, char* szOut, size_t& iOutputLen ) {
		if ( !m_hIconv ) {
			return 0;
		}
		size_t iRes = iconv( m_hIconv, &szIn, &iInputLen, &szOut, &iOutputLen );
		if ( iRes == ( size_t ) ( -1 ) ) {
			return 0;
		}
		return 1;
	} // udDWord Convert

} // namespace UdSDK
