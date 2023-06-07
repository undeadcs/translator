
#include "includes.h"
#include "externals.h"

namespace UdSDK {

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// class CFile

	CFile::CFile( ) {
		m_iReadEnc		= UTF_32;
		m_iInternalEnc	= UTF_32;
		m_iWriteEnc		= UTF_32;
	}

	CFile::CFile( Encoding iReadEnc, Encoding iInternalEnc, Encoding iWriteEnc ) {
		m_iReadEnc		= iReadEnc;
		m_iInternalEnc	= iInternalEnc;
		m_iWriteEnc		= iWriteEnc;
	}

	CFile::CFile( const CFile& objCopy ) {
		m_iReadEnc		= objCopy.m_iReadEnc;
		m_iInternalEnc	= objCopy.m_iInternalEnc;
		m_iWriteEnc		= objCopy.m_iWriteEnc;
	}

	CFile::~CFile( ) {
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// class CFileUtf32

	CFileUtf32::CFileUtf32( ) {
	}

	CFileUtf32::~CFileUtf32( ) {
	}

	udDWord CFileUtf32::Load( string szFileName ) {
		udDWord dwResult = 0;
		udPStdFile pFile = fopen( szFileName.c_str( ), "rb" );
		if ( pFile ) {
			clock_t tStart = clock( );
			if ( this->CheckBOM( pFile ) ) {
				dwResult = this->ReadFile( pFile );
			}
			clock_t tEnd = clock( );
			float fDiff = float( tEnd - tStart ) / float( CLOCKS_PER_SEC );
			char tmp[ 1204 ];
			sprintf( tmp, "loaded file '%s' in %.3f sec\n", szFileName.c_str( ), fDiff );
			if ( g_pLog ) {
				g_pLog->Write( tmp );
			}
			fclose( pFile );
		}
		return dwResult;
	} // udDWord Load

	udDWord CFileUtf32::Length( ) {
		return ( udDWord ) m_szText.length( );
	} // udDWord Length

	udStlStr& CFileUtf32::GetText( ) {
		return m_szText;
	} // udStlStr GetText

	void CFileUtf32::Clear( ) {
		m_szText.clear( );
	} // void Clear

	udDWord CFileUtf32::Flush( string szFileName ) {
		udPStdFile pFile = fopen( szFileName.c_str( ), "wb" );
		if ( pFile ) {
			this->PutBOM( pFile );
			this->PutText( pFile );
			fclose( pFile );
			return 1;
		}
		return 0;
	} // udDWord Flush

	udDWord CFileUtf32::Flush( udPStdFile pFile ) {
		if ( pFile ) {
			this->PutBOM( pFile );
			this->PutText( pFile );
			return 1;
		}
		return 0;
	} // udDWord Flush

	udDWord CFileUtf32::Flush( string szFileName, const udStlStr& szText ) {
		m_szText = szText;
		return this->Flush( szFileName );
	} // udDWord Flush

	void CFileUtf32::SetText( const udStlStr& szText ) {
		m_szText = szText;
	} // void SetText

	udDWord CFileUtf32::CheckBOM( udPStdFile pFile ) {
		int iChar = 0, iIndex = 0;
		int arrBOM[ ] = { 0x00, 0x00, 0xfe, 0xff };
		udDWord iPos;

		if ( !feof( pFile ) ) {
			iChar = fgetc( pFile );
			while ( !feof( pFile ) ) {
				if ( ( iIndex == 4 ) || ( iChar != arrBOM[ iIndex ] ) ) {
					break;
				}
				++iIndex;
				iChar = fgetc( pFile );
			}
		}
		if ( iIndex == 4 ) {
			fgetpos( pFile, ( fpos_t* )&iPos );
			--iPos;
			fsetpos( pFile, ( fpos_t* )&iPos );
			m_dwHasBOM = 1;
			return 1;
		}

		iPos = 0;
		fsetpos( pFile, ( fpos_t* )&iPos );
		m_dwHasBOM = 0;
		return 1;
	} // udDWord CheckBOM

	udDWord CFileUtf32::ReadFile( udPStdFile pFile ) {
		udDWord dwResult = 0, dwCur = 0;
		int iChar = 0, i = 0;
		while ( !feof( pFile ) ) {
			for( i = 0; i < 4; ++i ) {
				iChar = fgetc( pFile );
				if ( iChar != EOF ) {
					if ( i ) {
						dwCur <<= 8; // байты идут от старшего к младшему, значит и заполняем так же
					}
					dwCur |= iChar;
				} else {
					break;
				}
			}
			if ( i == 4 ) {
				m_szText += dwCur;
				++dwResult;
				dwCur = 0;
			}
		}

		return dwResult;
	} // udDWord ReadFile

	void CFileUtf32::PutBOM( udPStdFile pFile ) {
		fputc( 0x00, pFile );
		fputc( 0x00, pFile );
		fputc( 0xfe, pFile );
		fputc( 0xff, pFile );
		/*int arrBOM[ ] = { 0x00, 0x00, 0xfe, 0xff };
		for( int i = 0; i < 4; ++i ) {
			fputc( arrBOM[ i ], pFile );
		}*/
	} // void PutBOM

	void CFileUtf32::PutText( udPStdFile pFile ) {
		for( udStlStr::iterator i = m_szText.begin( ), itEnd = m_szText.end( ); i != itEnd; ++i ) {
			this->PutChar( pFile, *i );
		}
	} // void PutText

	void CFileUtf32::PutChar( udPStdFile pFile, udDWord dwChar ) {
		fputc( BYTE_4( dwChar ), pFile );
		fputc( BYTE_3( dwChar ), pFile );
		fputc( BYTE_2( dwChar ), pFile );
		fputc( BYTE_1( dwChar ), pFile );
	} // void PutChar

} // namespace UdSDK
