
#include <stdio.h>
#include <errno.h>
#include <string>
#include <iostream>
#include <iconv.h>

using namespace std;

#define DEFAULT_SIZE 40000000
#define ICONV_OPEN_OK( x ) (x) != (iconv_t)(-1)

iconv_t hIconv;

void PutBOM( FILE* pFile );
size_t Convert( const char* szIn, size_t& iLen, char* szOut );
void Flush( const char* szFileName, const char* szText, size_t iBytes );
void ShowError( );

int mainX( int argc, char** argv ) {
	hIconv = iconv_open( "UTF-32", "UTF-16LE" );
	if ( ICONV_OPEN_OK( hIconv ) ) {
		size_t iSizeIn = DEFAULT_SIZE, iCurIn = 0;
		char *szTextIn = new char[ iSizeIn ], *tmp = NULL;
		memset( szTextIn, 0, DEFAULT_SIZE );
		FILE* pFileIn = fopen( "warodai.txt", "rb" );

		for( int xyu = 0; xyu < 2; ++xyu ) {
			fgetc( pFileIn );
		}

		char cChar = fgetc( pFileIn );
		while( !feof( pFileIn ) ) {
			if ( iCurIn == iSizeIn ) {
				iSizeIn += iSizeIn;
				tmp = new char[ iSizeIn ];
				memcpy( tmp, szTextIn, iCurIn );
				delete [ ] szTextIn;
				szTextIn = tmp;
				tmp = NULL;
			}
			szTextIn[ iCurIn++ ] = cChar;
			szTextIn[ iCurIn ] = '\0';
			cChar = fgetc( pFileIn );
		}
		fclose( pFileIn );

		char *szTextOut = new char[ DEFAULT_SIZE ];
		memset( szTextOut, 0, DEFAULT_SIZE );

		size_t iResultLen = Convert( szTextIn, iCurIn, szTextOut );
		cout << "result string length: " << iResultLen;
		Flush( "warodai.utf32", szTextOut, iResultLen );

		iconv_close( hIconv );

		delete [ ] szTextOut;
		delete [ ] szTextIn;
	}
	cout << endl << "press any key to exit";
	getchar( );
	return 0;
} // int main

size_t Convert( const char* szIn, size_t& iLen, char* szOut ) {
	size_t j = DEFAULT_SIZE;
	size_t iResult = iconv( hIconv, &szIn, &iLen, &szOut, &j );
	if ( iResult == (size_t)(-1) ) {
		ShowError( );
		return 0;
	} else {
		return ( DEFAULT_SIZE - j );
	}
} // void Convert
void PutBOM( FILE* pFile ) {
	int arrBOM[ ] = { 0x00, 0x00, 0xfe, 0xff };
	for( int i = 0; i < 4; ++i ) {
		fputc( arrBOM[ i ], pFile );
	}
} // void PutBOM

void Flush( const char* szFileName, const char* szText, size_t iBytes ) {
	FILE* pFile = fopen( szFileName, "wb" );
	if ( pFile ) {
		int arrBOM[ ] = { 0x00, 0x00, 0xfe, 0xff };
		for( int j = 0; j < 4; ++j ) {
			fputc( arrBOM[ j ], pFile );
		}
		for( size_t i = 0; i < iBytes; ++i ) {
			fputc( szText[ i ], pFile );
		}
		fclose( pFile );
	} else {
		cout << "can't open file '" << szFileName << '\'';
	}
} // void Flush

void ShowError( ) {
	int err = errno;
	switch( err ) {
		case E2BIG: {
			cout << "not enough memory for output";
		} break;
		case EILSEQ: {
			cout << "invalid multibyte sequence";
		} break;
		case EINVAL: {
			cout << "incomplete multibyte sequence";
		} break;
	}
} // void ShowError


