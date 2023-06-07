
#include "includes.h"
#include "externals.h"

namespace UdSDK {

	void SignalHandler( int iSig ) {
		if ( g_pLog ) {
			char msg[ 1204 ];
			sprintf( msg, "got signal %d\n", iSig );
			g_pLog->Write( msg );
		}
		FatalTerminate( );
		exit( EXIT_SUCCESS );
	} // void SignalHandler

	CDaemon::CDaemon( ) {
#ifdef FOR_WIN32
#else
#endif
		m_iPid		= 0;
		m_iSid		= 0;
		m_hSocket	= INVALID_SOCKET;
		m_pLog		= NULL;
		m_pConfig	= NULL;
		m_pTree		= NULL;
	}

	CDaemon::CDaemon( const CDaemon& objCopy ) {
	}

	CDaemon::~CDaemon( ) {
		this->Terminate( );
	}

	CDaemon::DaemonError CDaemon::Proc( int argc, char** argv ) {
		CDaemon::DaemonError iErr = this->Init( "config" );
		if ( iErr != CDaemon::UDERROR_OK ) {
			this->Terminate( );
			return iErr;
		}

		this->Cycle( );

		return CDaemon::UDERROR_OK;
	} // DaemonError Proc

	CDaemon::DaemonError CDaemon::Init( string szConfigPath ) {
#ifdef FOR_WIN32
#else
		m_iPid = fork( );
		if ( m_iPid < 0 ) {
			return CDaemon::UDERROR_FAILED_PID;
		}

		if ( m_iPid > 0 ) {
			return CDaemon::UDERROR_PARENT;
		}

		m_iPid = getpid( );

		umask( 0 );
#endif

		CDaemon::DaemonError iErr = this->InitConfig( szConfigPath.c_str( ) );
		if ( iErr != CDaemon::UDERROR_OK ) {
			return iErr;
		}

		iErr = this->InitLog( );
		if ( iErr != CDaemon::UDERROR_OK ) {
			return iErr;
		}

		iErr = this->InitDump( );
		if ( iErr != CDaemon::UDERROR_OK ) {
			return iErr;
		}

#ifdef FOR_WIN32
#else
		m_iSid = setsid( );
		if ( m_iSid < 0 ) {
			m_pLog->Write( "failed getting session id\n" );
			return CDaemon::UDERROR_FAILED_SID;
		}

		char msg[ 1024 ];
		sprintf( msg, "pid: %i, sid: %i\n", m_iPid, m_iSid );
		m_pLog->Write( msg );

		if ( ( chdir( "/" ) ) < 0 ) {
			return CDaemon::UDERROR_FAILED_DIR;
		}

		close( STDIN_FILENO		);
  		close( STDOUT_FILENO	);
  		close( STDERR_FILENO	);

  		signal( SIGTERM, SignalHandler );
#endif
		iErr = this->InitSocket( );
		if ( iErr != CDaemon::UDERROR_OK ) {
			return iErr;
		}


		m_pLog->Write( "Undead Translator has been started.\n" );
		return CDaemon::UDERROR_OK;
	} // DaemonError Init

	CDaemon::DaemonError CDaemon::InitConfig( string szConfigPath ) {
		m_pConfig = new udConfig;
		CConfig::ConfError iErr = m_pConfig->Load( szConfigPath.c_str( ) );
		if ( iErr != CConfig::UDERROR_OK ) {
			return CDaemon::UDERROR_NO_CONFIG;
		}

		return CDaemon::UDERROR_OK;
	} // DaemonError InitConfig

	CDaemon::DaemonError CDaemon::InitLog( ) {
		m_pLog = new udLog;
		CLog::LogError iLogErr = m_pLog->Open( m_pConfig->GetLogPath( ) );
		if ( iLogErr != CLog::UDERROR_OK ) {
			return CDaemon::UDERROR_NO_LOG;
		}

		g_pLog = m_pLog;

		return CDaemon::UDERROR_OK;
	} // DaemonError InitLog

	CDaemon::DaemonError CDaemon::InitDump( ) {
		m_pTree = new udTree;
		if ( !m_pTree->DumpLoad( m_pConfig->GetDumpPath( ) ) ) {
			if ( m_pLog ) {
				m_pLog->Write( "default dump not found\n" );
			}
		}
		/*udPTree pTree = new udTree;
		if ( pTree->DumpLoad( m_pConfig->GetDumpPath( ) ) ) {
			m_mapTree[ string( "default" ) ] = pTree;
		} else {
			m_pLog->Write( "default dump not found\n" );
			delete pTree;
		}

		pTree = NULL;*/
		return CDaemon::UDERROR_OK;
	} // DaemonError InitDump

	CDaemon::DaemonError CDaemon::InitSocket( ) {
		char msg[ 1024 ];
#ifdef FOR_WIN32
		WSAStartup( WINSOCK_VERSION, &m_objWinSockData );
#else
#endif
		m_hSocket = socket( AF_INET, SOCK_STREAM, 0 );
		if ( m_hSocket == INVALID_SOCKET ) {
			sprintf( msg, "failed socket, error code: %d\n", GET_SOCKET_ERROR );
			m_pLog->Write( msg );
			return CDaemon::UDERROR_NO_SOCKET;
		}

		memset( &m_objSockAddr, 0, sizeof( sockaddr_in ) );
		m_objSockAddr.sin_family		= AF_INET;
		m_objSockAddr.sin_addr.s_addr	= htonl( INADDR_ANY );
		m_objSockAddr.sin_port			= htons( 25000 );

		if ( bind( m_hSocket, ( sockaddr* ) &m_objSockAddr, sizeof( sockaddr ) ) == SOCKET_ERROR ) {
			sprintf( msg, "failed binding, error code: %d\n", GET_SOCKET_ERROR );
			m_pLog->Write( msg );
			return CDaemon::UDERROR_NO_BIND;
		}

		if ( listen( m_hSocket, 1 ) == SOCKET_ERROR ) {
			sprintf( msg, "listen failed, error code: %d\n", GET_SOCKET_ERROR );
			m_pLog->Write( msg );
			return CDaemon::UDERROR_SOCKET_FAIL;
		}

		return CDaemon::UDERROR_OK;
	} // DaemonError InitSocket

	void CDaemon::Terminate( ) {
		if ( m_hSocket != INVALID_SOCKET ) {
			CLOSE_SOCKET( m_hSocket );
#ifdef FOR_WIN32
			WSACleanup( );
#else
#endif
		}

		if ( m_pTree ) {
			m_pTree->Clear( );
			delete m_pTree;
			m_pTree = NULL;
		}
		if ( m_pConfig ) {
			delete m_pConfig;
			m_pConfig = NULL;
		}
		if ( m_pLog ) {
			delete m_pLog;
			m_pLog = NULL;
			g_pLog = NULL;
		}
	} // void Terminate

	CDaemon::DaemonError CDaemon::Cycle( ) {
		char msg[ 1204 ];

		SOCKET hSocket;
		udBool bBreak = 0;
		CDaemon::DaemonError iErr = CDaemon::UDERROR_OK;

		while( !bBreak ) {
			hSocket = accept( m_hSocket, NULL, NULL );
			if ( hSocket == INVALID_SOCKET ) {
				sprintf( msg, "accept failed, error code: %d\n", GET_SOCKET_ERROR );
				m_pLog->Write( msg );
				return CDaemon::UDERROR_SOCKET_FAIL;
			}

			//m_pLog->Write( "client connected\n" );

			iErr = this->HandleConnection( hSocket );
			if ( iErr != CDaemon::UDERROR_OK ) {
				sprintf( msg, "error, code: %d\n", iErr );
				m_pLog->Write( msg );
			}

			CLOSE_SOCKET( hSocket );
		}

		return CDaemon::UDERROR_OK;
	} // DaemonError Cycle

	CDaemon::DaemonError CDaemon::HandleConnection( SOCKET hSocket ) {
		CDaemon::DaemonError iErr = CDaemon::UDERROR_OK;
		udDWord		dwLen	= 0;
		udPDWord	pData	= NULL;
		char msg[ 1024 ];

		iErr = this->ReadData( hSocket, &pData, &dwLen );
		if ( iErr != CDaemon::UDERROR_OK ) {
			sprintf( msg, "reading data failed, error code: %d\n", iErr );
			m_pLog->Write( msg );
		} else {
			iErr = this->ValidateData( pData, dwLen );
			if ( iErr != CDaemon::UDERROR_OK ) {
				sprintf( msg, "invalid data, error code: %d\n", iErr );
				m_pLog->Write( msg );
			} else {
				iErr = this->ProcData( hSocket, pData, dwLen );
				if ( iErr != CDaemon::UDERROR_OK ) {
					sprintf( msg, "process data failed, error code: %d\n", iErr );
					m_pLog->Write( msg );
				}
			}
		}

		if ( pData ) {
			delete [ ] pData;
			pData = NULL;
		}
		return iErr;
	} // DaemonError HandleConnection

	CDaemon::DaemonError CDaemon::ReadData( SOCKET hSocket, udPPDWord pData, udPDWord pLen ) {
		CDaemon::DaemonError iErr = CDaemon::UDERROR_OK, iTmpErr = CDaemon::UDERROR_OK;
		udDWord dwCapacity = 200000, dwLen = 0, dwCur = 0;
		//udPChar pChar = new udChar[ dwCapacity  ];
		//udChar msg[ 1024 ];//, cChar = 0;
		udInt bDone = 0;//, iRead = 0;

		*pData = new udDWord[ dwCapacity ];
		memset( *pData, 0, dwCapacity );

		//m_pLog->Write( "reading data\n" );

		while( !bDone ) {
			dwCur = 0;
			iTmpErr = this->ReadDWord( hSocket, &dwCur );
			if ( iTmpErr == CDaemon::UDERROR_OK ) {
				if ( dwLen >= dwCapacity ) {
					dwCapacity += dwCapacity;
					udPDWord pTmp = new udDWord[ dwCapacity ];
					memcpy( pTmp, *pData, dwLen );
					delete [ ] *pData;
					*pData = pTmp;
					pTmp = NULL;
				}
				//sprintf( msg, "got dword: %lu\n", dwCur );
				//m_pLog->Write( msg );
				(*pData)[ dwLen++ ] = dwCur;
			} else if ( iTmpErr == CDaemon::UDERROR_NO_SOCKET ) {
				bDone = 1;
				//(*pData)[ dwLen++ ] = dwCur;
				break;
			}
			/*iRead = recv( hSocket, &cChar, 1, 0 );
			switch( iRead ) {
				case 0: {
					//m_pLog->Write( "connection closed by client\n" );
					bDone = 1;
				} break;

				case 1: {
					if ( dwLen >= dwCapacity ) {
						// достигнут предел приема данных
						delete [ ] pChar;
						pChar = NULL;
						return CDaemon::UDERROR_INVALID_DATA;
					}

					pChar[ dwLen++ ] = cChar;
				} break;

				default: {
					return CDaemon::UDERROR_SOCKET_FAIL;
				} break;
			}*/
		}

		//sprintf( msg, "bytes read from socket: %lu\n", dwLen * 4 );
		//m_pLog->Write( msg );

		*pLen	= dwLen;
		//*pData	= ( udPDWord ) pChar;
		//pChar	= NULL;

		//m_pLog->Write( "data read\n" );

		return iErr;
	} // DaemonError ReadData

	CDaemon::DaemonError CDaemon::ValidateData( udPDWord pData, udDWord dwLen ) {
		return CDaemon::UDERROR_OK;
	} // DaemonError ValidateData

	CDaemon::DaemonError CDaemon::ReadDWord( SOCKET hSocket, udPDWord dwOut ) {
		if ( !dwOut ) {
			return CDaemon::UDERROR_INVALID_DATA;
		}

		udDWord	dwCur = 0;
		udInt	iRead = 0;
		udByte	cChar = 0;

		for ( int i = 0; i < 4; ++i ) {
			iRead = recv( hSocket, ( char* ) &cChar, 1, 0 );
			switch( iRead ) {
				case SOCKET_ERROR: { // ошибка
					return CDaemon::UDERROR_SOCKET_FAIL;
				} break;

				case 0: { // закрыли соединение
					*dwOut = dwCur;
					return CDaemon::UDERROR_NO_SOCKET;
				} break;

				default: { // считали байт
					//char msg[ 256 ];
					//sprintf( msg, "%d, 0x%02x\n", cChar, cChar );
					//m_pLog->Write( msg );
					if ( i ) {
						dwCur <<= 8;
					}
					dwCur |= cChar;
				} break;
			}
			cChar = 0;
		}

		*dwOut = dwCur;

		return CDaemon::UDERROR_OK;
	} // DaemonError ReadDWord

	CDaemon::DaemonError CDaemon::ProcData( SOCKET hSocket, udPDWord pData, udDWord dwLen ) {
		char msg[ 1024 ];
		//sprintf( msg, "data len: %lu\n", dwLen );
		//m_pLog->Write( msg );
		udDWord dwCmd = pData[ 0 ];
		switch( dwCmd ) {
			case CDaemon::UDCMD_LIST: {
				this->ShowList( hSocket, pData, dwLen );
			} break;

			case CDaemon::UDCMD_INFO: {
				this->ShowInfo( hSocket, pData, dwLen );
			} break;

			case CDaemon::UDCMD_DEL_BY_ID: {
				this->DelById( hSocket, pData, dwLen );
			} break;

			case CDaemon::UDCMD_ADD_TANSLATE: {
				this->AddTranslate( hSocket, pData, dwLen );
			} break;

			case CDaemon::UDCMD_DEL_WORD: {
				this->DelWord( hSocket, pData, dwLen );
			} break;

			case CDaemon::UDCMD_DEL_TRANSLATE: {
				this->DelTranslate( hSocket, pData, dwLen );
			} break;

			case CDaemon::UDCMD_DEL_WORD_TRANSLATE: {
				this->DelWordTranslate( hSocket, pData, dwLen );
			} break;

			case CDaemon::UDCMD_GET_TRANSLATE: {
				this->GetTranslate( hSocket, pData, dwLen );
			} break;

			default: {
				sprintf( msg, "unknown command code, dword: %lu, hex: 0x%04lx\n", dwCmd, dwCmd );
				m_pLog->Write( msg );
				return CDaemon::UDERROR_UNKNOWN_CMD;
			} break;
		}

		return CDaemon::UDERROR_OK;
	} // DaemonError ProcData

	void CDaemon::ShowInfo( SOCKET hSocket, udPDWord pData, udDWord dwLen ) {
		//m_pLog->Write( "ShowInfo\n" );

		char msg[ 1024 ];
		udDWord dwCount = m_pTree->GetWordCount( );
		udStlStr szOut;

		szOut += ( udDWord ) 0;
		szOut += dwCount;

		string szData = OurStringToStd( szOut );
		//clock_t tStart = clock( );
		udInt iSent = send( hSocket, szData.data( ), szData.length( ), 0 );
		if ( iSent == SOCKET_ERROR ) {
			sprintf( msg, "error, code: %d\n", GET_SOCKET_ERROR );
			m_pLog->Write( msg );
		} else {
			//sprintf( msg, "sent bytes: %d\n", iSent );
		}
		shutdown( hSocket, 1 );
		//m_pLog->Write( msg );
		/*clock_t tEnd = clock( );
		float fDiff = float( tEnd - tStart ) / float( CLOCKS_PER_SEC );
		sprintf( msg, "sending time: %.3f sec\n", fDiff );
		m_pLog->Write( msg );*/
		szOut.clear( );
		szData.clear( );
	} // void ShowInfo

	void CDaemon::ShowList( SOCKET hSocket, udPDWord pData, udDWord dwLen ) {
		//m_pLog->Write( "ShowList\n" );

		char msg[ 1024 ];
		udDWord dwOffset = 0, dwCount = 0, dwMsgLen = pData[ 1 ];
		udStlStr szOut;

		szOut += dwLen;
		szOut += dwLen;

		if ( dwMsgLen == 4 ) {
			dwCount = pData[ 2 ];
		} else if ( dwMsgLen == 8 ) {
			dwOffset	= pData[ 2 ];
			dwCount		= pData[ 3 ];
		}

		/*for( udDWord i = 0; i < dwLen; ++i ) {
			sprintf( msg, "0x%08lx\n", pData[ i ] );
			m_pLog->Write( msg );
		}*/

		//sprintf( msg, "offset: %lu, count: %lu, len: %lu, msg_len: %lu\n", dwOffset, dwCount, dwLen, dwMsgLen );
		//m_pLog->Write( msg );

		m_pTree->PrintXml( szOut, dwOffset, dwCount, 1 );
		dwLen = ( udDWord ) szOut.size( );
		szOut[ 1 ] = dwLen * 4 ;

		string szData = OurStringToStd( szOut );
		//clock_t tStart = clock( );
		udInt iSent = send( hSocket, szData.data( ), szData.length( ), 0 );
		if ( iSent == SOCKET_ERROR ) {
			sprintf( msg, "error, code: %d\n", GET_SOCKET_ERROR );
			m_pLog->Write( msg );
		} else {
			//sprintf( msg, "sent bytes: %d\n", iSent );
		}
		shutdown( hSocket, 1 );
		//m_pLog->Write( msg );
		/*clock_t tEnd = clock( );
		float fDiff = float( tEnd - tStart ) / float( CLOCKS_PER_SEC );
		sprintf( msg, "sending time: %.3f sec\n", fDiff );
		m_pLog->Write( msg );*/
		szOut.clear( );
		szData.clear( );
	} // void ShowList

	void CDaemon::DelById( SOCKET hSocket, udPDWord pData, udDWord dwLen ) {
		//m_pLog->Write( "DelById\n" );

		udDWord dwWordId = pData[ 1 ], dwDelTranslate = pData[ 2 ], dwTrLen = 0;
		udStlStr arrTranslate;

		if ( dwDelTranslate ) {
			dwTrLen = pData[ 3 ];
			for( udDWord i = 4, j = 0; ( j < dwTrLen ) && ( i < dwLen ) ; ++i, ++j ) {
				arrTranslate += pData[ i ];
			}
			m_pTree->DelTranslateById( dwWordId, arrTranslate );
			arrTranslate.clear( );
		} else {
			m_pTree->DelWordById( dwWordId );
		}
	} // void DelWord

	void CDaemon::AddTranslate( SOCKET hSocket, udPDWord pData, udDWord dwLen ) {
		//m_pLog->Write( "AddWord\n" );

		udDWord dwOrigLen = pData[ 1 ], dwTrLen = pData[ 2 ];
		udStlStr szOrig, szTranslate;

		for( udDWord i = 3, j = 0; ( i < dwLen ) && ( j < dwOrigLen ); ++i, ++j ) {
			szOrig += pData[ i ];
		}

		if ( dwTrLen ) {
			for( udDWord i = ( 3 + dwOrigLen ), j = 0; ( i < dwLen ) && ( j < dwTrLen ); ++i, ++j ) {
				szTranslate += pData[ i ];
			}
			m_pTree->AddTranslatedWord( szOrig, szTranslate );
		} else {
			m_pTree->AddWord( szOrig );
		}
	} // void AddWord

	void CDaemon::DelWord( SOCKET hSocket, udPDWord pData, udDWord dwLen ) {
		//m_pLog->Write( "DelWord\n" );

		udDWord dwWordLen = pData[ 1 ];
		udStlStr szWord;

		for( udDWord i = 2, j = 0; ( i < dwLen ) && ( j < dwWordLen ); ++i, ++j ) {
			szWord += pData[ i ];
		}

		m_pTree->DelWord( szWord );
		szWord.clear( );
	} // void DelWord

	void CDaemon::DelTranslate( SOCKET hSocket, udPDWord pData, udDWord dwLen ) {
		//m_pLog->Write( "DelTranslate\n" );

		udDWord dwWordLen = pData[ 1 ];
		udStlStr szWord;

		for( udDWord i = 2, j = 0; ( i < dwLen ) && ( j < dwWordLen ); ++i, ++j ) {
			szWord += pData[ i ];
		}

		m_pTree->DelWordFromTranslate( szWord );
		szWord.clear( );
	} // void DelTranslate

	void CDaemon::DelWordTranslate( SOCKET hSocket, udPDWord pData, udDWord dwLen ) {
		//m_pLog->Write( "DelWordTranslate\n" );

		udDWord dwOrigLen = pData[ 1 ], dwTrLen = pData[ 2 ];
		udStlStr szOrig, szTranslate;

		for( udDWord i = 3, j = 0; ( i < dwLen ) && ( j < dwOrigLen ); ++i, ++j ) {
			szOrig += pData[ i ];
		}

		if ( dwTrLen ) {
			for( udDWord i = ( 3 + dwOrigLen ), j = 0; ( i < dwLen ) && ( j < dwTrLen ); ++i, ++j ) {
				szTranslate += pData[ i ];
			}
		}

		m_pTree->DelTranslate( szOrig, szTranslate );
	} // void DelWordTranslate

	void CDaemon::GetTranslate( SOCKET hSocket, udPDWord pData, udDWord dwLen ) {
		//m_pLog->Write( "GetTranslate\n" );

		char msg[ 1024 ];
		udDWord dwWordLen = pData[ 1 ];
		udStlStr szWord, szOut;

		for( udDWord i = 2, j = 0; ( i < dwLen ) && ( j < dwWordLen ); ++i, ++j ) {
			szWord += pData[ i ];
		}

		udArrWord arrWord = m_pTree->Find( szWord );
		if ( arrWord.empty( ) ) {
			szOut += ( udDWord ) 0;
			szOut += ( udDWord ) 1;
			szOut += szWord.length( );
			szOut += szWord;

			//m_pLog->Write( "word not found\n" );
		} else {
			udDWord dwCount = 0;
			szOut += ( udDWord ) 0;	// код ошибки
			szOut += ( udDWord ) 0;	// число слов
			string szTmp;
			for( udArrWord::iterator i = arrWord.begin( ), j = arrWord.end( ); i != j; ++i ) {
				++dwCount;
				szOut += i->length( );
				szOut += *i;
			}

			szOut[ 1 ] = dwCount;

			//sprintf( msg, "word found, translate num: %lu\n", dwCount );
			//m_pLog->Write( msg );
		}

		szWord.clear( );
		string szData = OurStringToStd( szOut );
		//clock_t tStart = clock( );
		udInt iSent = send( hSocket, szData.data( ), szData.length( ), 0 );
		if ( iSent == SOCKET_ERROR ) {
			sprintf( msg, "error, code: %d\n", GET_SOCKET_ERROR );
			m_pLog->Write( msg );
		} else {
			//sprintf( msg, "sent bytes: %d\n", iSent );
		}
		shutdown( hSocket, 1 );
		/*m_pLog->Write( msg );
		clock_t tEnd = clock( );
		float fDiff = float( tEnd - tStart ) / float( CLOCKS_PER_SEC );
		sprintf( msg, "sending time: %.3f sec\n", fDiff );
		if ( g_pLog ) {
			g_pLog->Write( msg );
		}*/
		szOut.clear( );
		szData.clear( );
	} // void GetTranslate

} // namespace UdSDK
