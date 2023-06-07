
#include "includes.h"
#include "externals.h"

namespace UdSDK {

	CWarodai::CWarodai( ) {
		m_szFileIn		= "";
		m_szFileOut		= "";
		m_dwFrameSize	= 1000;
	}

	CWarodai::CWarodai( const CWarodai& objCopy ) {
		m_szFileIn		= objCopy.m_szFileIn;
		m_szFileOut		= objCopy.m_szFileOut;
		m_dwFrameSize	= objCopy.m_dwFrameSize;
	}

	CWarodai::~CWarodai( ) {
	}

	void CWarodai::Proc( ) {
	} // void Proc

	void CWarodai::PageSep( ) {
		if ( !m_dwFrameSize ) {
			return;
		}

		FILE* pFileIn = fopen( m_szFileIn.c_str( ), "rb" );
		if ( pFileIn ) {
			// проскакиваем BOM
			udDWord i;
			int iChar = 0;
			for( i = 0; i < 4; ++i ) {
				iChar = fgetc( pFileIn );
			}

			CFileUtf32 objFile;
			udStlStr szTmp;
			const char szPageNameFormat[ ] = "warodai/page%02d_warodai.utf32";
			char szPageName[ 256 ];

			for( i = 1; i < m_dwFrameSize; ++i ) {
				szTmp += this->ReadBlock( pFileIn );
				if ( feof( pFileIn ) ) {
					break;
				}
				szTmp += LF;
				szTmp += LF;
			}

			if ( feof( pFileIn ) ) {
				sprintf( szPageName, szPageNameFormat, 1 );
				objFile.Flush( szPageName, szTmp );
				objFile.Clear( );
				szTmp.clear( );
			} else {
				sprintf( szPageName, szPageNameFormat, 1 );
				objFile.Flush( szPageName, szTmp );
				objFile.Clear( );
				szTmp.clear( );
				int iCurPage = 2;

				while( !feof( pFileIn ) ) {
					for( i = 1; i < m_dwFrameSize; ++i ) {
						szTmp += this->ReadBlock( pFileIn );
						if ( feof( pFileIn ) ) {
							break;
						}
						szTmp += LF;
						szTmp += LF;
					}

					sprintf( szPageName, szPageNameFormat, iCurPage++ );
					objFile.Flush( szPageName, szTmp );
					objFile.Clear( );
					szTmp.clear( );
				}
			}

			fclose( pFileIn );
		} else {
			cout << "can't open file " << m_szFileIn;
		}
	} // void PageSep

	udStlStr CWarodai::ReadBlock( FILE* pFile ) {
		udStlStr szRet;
		udDWord dwChar = 0, dwPre = 0;
		int iChar = 0, i = 0;

		while ( !feof( pFile ) ) {
			for( i = 0; i < 4; ++i ) {
				iChar = fgetc( pFile );
				if ( iChar != EOF ) {
					if ( i ) {
						dwChar <<= 8; // байты идут от старшего к младшему, значит и заполняем так же
					}
					dwChar |= iChar;
				} else {
					break;
				}
			}
			if ( i == 4 ) {
				if ( dwChar == LF ) {
					if ( dwPre == LF ) {
						break;
					}
				} else {
					if ( dwPre == LF ) {
						szRet += LF;
					}
					szRet += dwChar;
				}
				dwPre = dwChar;
				dwChar = 0;
			}
		}

		return szRet;
	} // udStlStr

} // namespace UdSDK

