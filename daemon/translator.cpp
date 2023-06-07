
#define TRANSLATOR_CPP

#include "includes.h"
#include "externals.h"

namespace UdSDK {

	CTranslator::CTranslator( ) {
		m_pLog		= NULL;
		m_pConfig	= NULL;
		m_pTree		= NULL;
		m_dwTreeId	= 0;
		m_dwLogId	= 0;
	}

	CTranslator::CTranslator( const CTranslator& objCopy ) {
	}

	CTranslator::~CTranslator( ) {
		this->Destroy( );
	}

	CTranslator::TranslatorError CTranslator::Proc( int argc, char** argv ) {
		CTranslator::TranslatorError iErr = this->Init( "config" );
		if ( iErr != CTranslator::UDERROR_OK ) {
			this->Destroy( );
			return iErr;
		}

		iErr = this->ValidateCmdArg( argc, argv );
		if ( iErr != CTranslator::UDERROR_OK ) {
			this->Destroy( );
			return iErr;
		}

		switch( argv[ 1 ][ 0 ] ) {
			// подгрузка дампа
			case 'l': {
				this->LoadDump( argv[ 2 ] );
			} break;

			// подгрузка композиции
			case 'c': {
				this->LoadComposition( argv[ 2 ] );
			} break;

			// перевод фразы
			case 't': {
				this->Translate( argv[ 2 ], argv[ 3 ] );
			} break;

			// добавление перевода
			case 'a': {
				this->AddTranslate( argv[ 2 ] );
			} break;

			// получение слов и их переводов
			case 's': {
				this->ShowList( argc, argv );
			} break;

			// получение информации о данных переводчика
			case 'i': {
				this->ShowInfo( );
			} break;

			// удаление слова
			case 'd': {
				this->DelWord( argv[ 2 ] );
			} break;

			// удаление перевода
			case 'e': {
				this->DelTranslate( argv[ 2 ] );
			} break;

			// удаление перевода заданного слова
			case 'r': {
				this->DelTargetTranslate( argc, argv );
			} break;

			case 'g': {
				this->ProcIdRequest( argv[ 2 ] );
			} break;
		}

		return CTranslator::UDERROR_OK;
	} // TranslatorError Proc

	CTranslator::TranslatorError CTranslator::BeginLog( const string& szLogPath, udPDWord dwIndex ) {
		udPLog pLog = new CLog;
		CLog::LogError iErr = pLog->Open( szLogPath );
		if ( iErr != CLog::UDERROR_OK ) {
			delete pLog;
			pLog = NULL;
			return CTranslator::UDERROR_NO_LOG;
		}

		*dwIndex = m_dwLogId;
		m_mapLog[ m_dwLogId++ ] = pLog;
		return CTranslator::UDERROR_OK;
	} // TranslatorError BeginLog

	CTranslator::TranslatorError CTranslator::BeginLog( const char* szLogPath, udPDWord dwIndex ) {
		string szTmp( szLogPath );
		return this->BeginLog( szTmp, dwIndex );
	} // TranslatorError BeginLog

	CTranslator::TranslatorError CTranslator::EndLog( udDWord dwIndex ) {
		udMapLog::iterator i = m_mapLog.find( dwIndex );
		if ( i == m_mapLog.end( ) ) {
			return CTranslator::UDERROR_NO_LOG;
		}
		udPLog pLog = i->second;
		if ( pLog ) {
			pLog->Close( );
			delete pLog;
			pLog = NULL;
		}
		m_mapLog.erase( i );
		return CTranslator::UDERROR_OK;
	} // TranslatorError EndLog

	udPLog CTranslator::GetLog( udDWord dwIndex ) {
		udMapLog::iterator i = m_mapLog.find( dwIndex );
		if ( i != m_mapLog.end( ) ) {
			return i->second;
		}
		return NULL;
	} // udPLog GetLog

	udPLog CTranslator::GetLog( ) {
		return m_pLog;
	} // udPLog GetLog

	/**
	 * 	Базовая инициализация переводчика
	 * 	@param szConfigPath string путь до конфига
	 *	@return TranslatroError
	 */
	CTranslator::TranslatorError CTranslator::Init( const char* szConfigPath ) {
		if ( m_pConfig || m_pLog ) {
			this->Destroy( );
		}

		m_pConfig = new CConfig;
		CConfig::ConfError iConfErr = m_pConfig->Load( szConfigPath );
		if ( iConfErr != CConfig::UDERROR_OK ) {
			return CTranslator::UDERROR_NO_CONFIG;
		}

		m_pLog = new CLog;
		CLog::LogError iLogErr = m_pLog->Open( m_pConfig->GetLogPath( ) );
		if ( iLogErr != CLog::UDERROR_OK ) {
			switch( iLogErr ) {
				case CLog::UDERROR_OK: {
				} break;
				case CLog::UDERROR_CANT_OPEN_LOG: {
					cout << "can't open log" << endl;
				} break;
				case CLog::UDERROR_WRITE_LOG_FAILED: {
					cout << "write log failed" << endl;
				} break;
				case CLog::UDERROR_NOT_OPENED: {
					cout << "log not opened" << endl;
				} break;
				case CLog::UDERROR_INVALID_ARG: {
					cout << "invalid arg" << endl;
				} break;
			}
			return CTranslator::UDERROR_NO_LOG;
		}

		//ClearFile( m_pConfig->GetLogPath( ).c_str( ) );

		g_pLog = m_pLog;

		m_pTree = new udTree;
		if ( !m_pTree->DumpLoad( m_pConfig->GetDumpPath( ) ) ) {
			m_pLog->Write( "default dump not found\n" );
		}

		return CTranslator::UDERROR_OK;
	} // TranslatorError Init

	/**
	 * 	Проверка валидности командной строки
	 * 	@return TranslatorError
	 */
	CTranslator::TranslatorError CTranslator::ValidateCmdArg( int argc, char** argv ) {
		if ( argc < 2 ) {
			return CTranslator::UDERROR_INVALID_CMD;
		}

		switch( argv[ 1 ][ 0 ] ) {
			case 'l':
			case 'c':
			case 'a':
			case 's':
			case 'd':
			case 'e':
			case 'r':
			case 'g': {
				if ( argc < 3 ) {
					m_pLog->Write( "invalid command\n" );
					return CTranslator::UDERROR_INVALID_CMD;
				}
			} break;

			case 't': {
				if ( argc < 4 ) {
					m_pLog->Write( "invalid command\n" );
					return CTranslator::UDERROR_INVALID_CMD;
				}
			} break;

			case 'i': {
			} break;

			default: {
				m_pLog->Write( "unknown command\n" );
				return CTranslator::UDERROR_UNKNOWN_CMD;
			} break;
		}

		return CTranslator::UDERROR_OK;
	} // udDWord ValidateCmdArg

	void CTranslator::Destroy( ) {
		if ( m_pConfig ) {
			delete m_pConfig;
			m_pConfig = NULL;
		}
		if ( m_pTree ) {
			m_pTree->Clear( );
			delete m_pTree;
			m_pTree = NULL;
		}
		if ( !m_mapTree.empty( ) ) {
			m_mapTree.clear( );
		}
		if ( !m_mapLog.empty( ) ) {
			udPLog pLog = NULL;
			for( udMapLog::iterator i = m_mapLog.begin( ), j = m_mapLog.end( ); i != j; ++i ) {
				pLog = i->second;
				if ( pLog ) {
					pLog->Close( );
					delete pLog;
					pLog = NULL;
				}
			}
			m_mapLog.clear( );
		}
		if ( m_pLog ) {
			delete m_pLog;
			m_pLog = NULL;
			g_pLog = NULL;
		}

		m_dwTreeId	= 0;
		m_dwLogId	= 0;
	} // void Destroy

	void CTranslator::LoadDump( const char* szDumpPath ) {
		if ( m_pTree->DumpLoad( szDumpPath ) ) {
			m_pTree->Dump( m_pConfig->GetDumpPath( ).c_str( ) );
		} else {
			m_pLog->Write( "loading dump '" );
			m_pLog->Write( szDumpPath, 0 );
			m_pLog->Write( "' failed\n", 0 );
		}
	} // void LoadDump

	void CTranslator::LoadComposition( const char* szComposeName ) {
		if ( m_pTree->Load( szComposeName ) ) {
			m_pTree->Dump( m_pConfig->GetDumpPath( ).c_str( ) );
		} else {
			m_pLog->Write( "loading composition '" );
			m_pLog->Write( szComposeName, 0 );
			m_pLog->Write( "' failed\n", 0 );
		}
	} // void LoadComposition

	void CTranslator::Translate( const char* szInputFilePath, const char* szOutputFilePath ) {
		CFileUtf32 *objFileIn = new CFileUtf32;
		if ( objFileIn->Load( szInputFilePath ) ) {
			udStlStr &szText = objFileIn->GetText( ), szTextOut;
			udArrWord arrWord = m_pTree->Find( szText );
			CFileUtf32 *objFileOut = new CFileUtf32;
			if ( arrWord.empty( ) ) { // если нет перевода, то перевод равен самому слову
				szTextOut = szText;
				m_pLog->Write( "translation not found\n" );
			} else {
				udArrWord::iterator itLast = arrWord.end( );

				--itLast;

				for( udArrWord::iterator i = arrWord.begin( ), j = arrWord.end( ); i != j; ++i ) {
					szTextOut += *i;
					if ( i != itLast ) {
						szTextOut += udTree::TRSEP;
					}
				}

			}
			if ( !objFileOut->Flush( szOutputFilePath, szTextOut ) ) {
				char tmp[ 1204 ];
				sprintf( tmp, "failed writing to file '%s'\n", szOutputFilePath );
				m_pLog->Write( tmp );
			}

			delete objFileOut;
			objFileOut = NULL;
		} else {
			m_pLog->Write( "loading file '" );
			m_pLog->Write( szInputFilePath, 0 );
			m_pLog->Write( "' failed\n", 0 );
		}

		delete objFileIn;
		objFileIn = NULL;
	} // void Translate

	void CTranslator::AddTranslate( const char* szComposeName ) {
		m_pLog->Write( "-- AddTranslate --\n" );
		char tmp[ 1024 ];

		CFileUtf32 *objFileOrig = new CFileUtf32, *objFileTranslate = new CFileUtf32;
		string szFileNameOrig( szComposeName ), szFileNameTranslate( szComposeName );

		szFileNameOrig		+= "_orig.utf32";
		szFileNameTranslate	+= "_translate.utf32";

		if ( objFileOrig->Load( szFileNameOrig ) ) {
			if ( objFileTranslate->Load( szFileNameTranslate ) ) {
				m_pTree->AddTranslatedWord( objFileOrig->GetText( ), objFileTranslate->GetText( ) );
			} else {
				m_pTree->AddWord( objFileOrig->GetText( ) );
			}

			sprintf( tmp, "dumping tree into '%s'\n", m_pConfig->GetDumpPath( ).c_str( ) );
			m_pLog->Write( tmp );
			m_pTree->Dump( m_pConfig->GetDumpPath( ).c_str( ) );
		} else {
			sprintf( tmp, "loading file '%s' failed\n", szFileNameOrig.c_str( ) );
			m_pLog->Write( tmp );
		}

		delete objFileOrig;
		objFileOrig = NULL;
		delete objFileTranslate;
		objFileTranslate = NULL;

		m_pLog->Write( "------------------\n" );
	} // void AddTranslate

	void CTranslator::ShowList( int argc, char** argv ) {
		udStlStr szText;
		if ( argc == 3 ) {
			// полная распечатка
			m_pTree->PrintXml( szText, 0, 0, 1 );
		} else if ( argc == 4 ) {
			// распечатка определенного количества первых слов
			m_pTree->PrintXml( szText, 0, atol( argv[ 3 ] ), 1 );
		} else {
			// начиная с определенного слова распечатка заданного количества
			m_pTree->PrintXml( szText, atol( argv[ 3 ] ), atol( argv[ 4 ] ), 1 );
		}

		CFileUtf32 *objFile = new CFileUtf32;
		if ( !objFile->Flush( argv[ 2 ], szText ) ) {
			m_pLog->Write( "flushing to file '" );
			m_pLog->Write( argv[ 2 ], 0 );
			m_pLog->Write( "' failed\n", 0 );
		}

		szText.clear( );
		delete objFile;
		objFile = NULL;
	} // void ShowList

	void CTranslator::ShowInfo( ) {
		cout << m_pTree->GetWordCount( ) << endl;
	} // void ShowInfo

	void CTranslator::DelWord( const char* szInputPath ) {
		m_pLog->Write( "DelWord\n" );

		CFileUtf32 *objFile = new CFileUtf32;
		if ( objFile->Load( szInputPath ) ) {
			m_pTree->DelWord( objFile->GetText( ) );
			m_pTree->Dump( m_pConfig->GetDumpPath( ).c_str( ) );
		} else {
			m_pLog->Write( "loading file '" );
			m_pLog->Write( szInputPath, 0 );
			m_pLog->Write( "' failed\n", 0 );
		}

		delete objFile;
		objFile = NULL;
	} // void DelWord

	void CTranslator::DelTranslate( const char* szInputPath ) {
		CFileUtf32 *objFile = new CFileUtf32;
		if ( objFile->Load( szInputPath ) ) {
			m_pTree->DelWordFromTranslate( objFile->GetText( ) );
			m_pTree->Dump( m_pConfig->GetDumpPath( ).c_str( ) );
		} else {
			m_pLog->Write( "loading file '" );
			m_pLog->Write( szInputPath, 0 );
			m_pLog->Write( "' failed", 0 );
		}

		delete objFile;
		objFile = NULL;
	} // void DelTranslate

	void CTranslator::DelTargetTranslate( int argc, char** argv ) {
		CFileUtf32 *objFileIn = new CFileUtf32;
		if ( objFileIn->Load( argv[ 2 ] ) ) {
			if ( argc == 3 ) {
				udStlStr szWord;
				szWord.clear( );
				m_pTree->DelTranslate( objFileIn->GetText( ), szWord );
				m_pTree->Dump( m_pConfig->GetDumpPath( ).c_str( ) );
			} else {
				CFileUtf32 *objFileIn2 = new CFileUtf32;
				if ( objFileIn2->Load( argv[ 3 ] ) ) {
					m_pTree->DelTranslate( objFileIn->GetText( ), objFileIn2->GetText( ) );
					m_pTree->Dump( m_pConfig->GetDumpPath( ).c_str( ) );
				} else {
					m_pLog->Write( "loading file '" );
					m_pLog->Write( argv[ 3 ], 0 );
					m_pLog->Write( "' failed\n", 0 );
				}

				delete objFileIn2;
				objFileIn2 = NULL;
			}
		} else {
			m_pLog->Write( "loading file '" );
			m_pLog->Write( argv[ 2 ], 0 );
			m_pLog->Write( "' failed\n", 0 );
		}

		delete objFileIn;
		objFileIn = NULL;
	} // void DelTargetTranslate

	void CTranslator::ProcIdRequest( const char* szFilePath ) {
		FILE *pFile = fopen( szFilePath, "rb" );
		if ( pFile ) {
			string szText;
			udDWord dwId = 0;
			udStlStr arrId;
			bool bWasSpace = false, bTranslate = false;
			char cChar = fgetc( pFile );
			while( !feof( pFile ) ) {
				if ( cChar == ' ' ) {
					if ( bWasSpace && bTranslate ) {
						arrId.push_back( atol( szText.c_str( ) ) );
						szText.clear( );
					} else {
						dwId = atol( szText.c_str( ) );
						szText.clear( );
						bWasSpace = true;
					}
				} else if ( cChar == 't' ) {
					if ( bWasSpace ) {
						bTranslate = true;
					} else {
						break;
					}
				} else {
					szText += cChar;
				}
				cChar = fgetc( pFile );
			}

			if ( !szText.empty( ) ) {
				if ( bTranslate ) {
					arrId.push_back( atol( szText.c_str( ) ) );
				} else {
					dwId = atol( szText.c_str( ) );
				}
				szText.clear( );
			}

			if ( bTranslate ) {
				dwId = m_pTree->DelTranslateById( dwId, arrId );
			} else {
				dwId = m_pTree->DelWordById( dwId );
			}

			fclose( pFile );
			m_pTree->Dump( m_pConfig->GetDumpPath( ).c_str( ) );
		} else {
			m_pLog->Write( "opening file '" );
			m_pLog->Write( szFilePath, 0 );
			m_pLog->Write( "' failed\n", 0 );
		}
	} // void ProcIdRequest

} // namespace UdSDK

