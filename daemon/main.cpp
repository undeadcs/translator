
#define MAIN_CPP

#include "includes.h"

namespace UdSDK {

	udPLog			g_pLog			= NULL;
	udPTranslator	g_pTranslator	= NULL;
	udPDaemon		g_pDaemon		= NULL;

} // namespace UdSDK

using namespace UdSDK;

#ifdef FOR_TRANSLATOR

int main( int argc, char** argv ) {
	clock_t tStart = clock( );
	g_pTranslator = new udTranslator;
	udTranslator::TranslatorError iErr = g_pTranslator->Proc( argc, argv );
	if ( iErr != udTranslator::UDERROR_OK ) {
		cout << "error: " << iErr << endl << "       ";
		switch( iErr ) {
			case udTranslator::UDERROR_OK: {
			} break;

			case udTranslator::UDERROR_INVALID_CMD: {
				cout << "invalid command";
			} break;

			case udTranslator::UDERROR_UNKNOWN_CMD: {
				cout << "unknown command";
			} break;

			case udTranslator::UDERROR_NO_CONFIG: {
				cout << "no config";
			} break;

			case udTranslator::UDERROR_NO_LOG: {
				cout << "no log";
			} break;

			case udTranslator::UDERROR_NO_DUMP: {
				cout << "no dump";
			} break;
		}
		cout << endl;
	}
	clock_t tEnd = clock( );
	char tmp[ 1024 ];
	float fDiff = float( tEnd - tStart ) / float( CLOCKS_PER_SEC );
	sprintf( tmp, "working time: %.3f sec\n", fDiff );
	if ( g_pLog ) {
		g_pLog->Write( tmp );
	} else {
		cout << tmp << endl;
	}
	delete g_pTranslator;
	return iErr;
} // int main

#else

int main( int argc, char** argv ) {
	try {
		g_pDaemon = new udDaemon;
		CDaemon::DaemonError iErr = g_pDaemon->Proc( argc, argv );
		if ( iErr != CDaemon::UDERROR_OK ) {
			switch( iErr ) {
				case CDaemon::UDERROR_PARENT: {
					cout << "parent process" << endl;
				} break;

				case CDaemon::UDERROR_FAILED_PID: {
					cout << "failed getting PID" << endl;
				} break;

				case CDaemon::UDERROR_FAILED_SID: {
					cout << "failed getting SID" << endl;
				} break;

				case CDaemon::UDERROR_FAILED_DIR: {
					cout << "failed choosing dir" << endl;
				} break;

				case CDaemon::UDERROR_NO_LOG: {
					int iErrNo = errno;
					cout << "failed opening log " << iErrNo << endl;
				} break;

				case CDaemon::UDERROR_NO_SOCKET: {
					cout << "failed opening socket" << endl;
				} break;

				case CDaemon::UDERROR_NO_BIND: {
					cout << "failed binding socket" << endl;
				} break;

				case CDaemon::UDERROR_NO_CONFIG: {
					cout << "no config" << endl;
				} break;

				case CDaemon::UDERROR_INVALID_DATA: {
					cout << "invalid data" << endl;
				} break;

				case CDaemon::UDERROR_SOCKET_FAIL: {
					cout << "failed socket operation" << endl;
				} break;

				default: {
					cout << "unknown error: " << iErr << endl;
				} break;
			}
		}
		if ( g_pDaemon ) {
			delete g_pDaemon;
			g_pDaemon = NULL;
		}

		return iErr;
	}

	catch( ... ) {
		if ( g_pLog ) {
			g_pLog->Write( "got exception\n" );
		}
		FatalTerminate( );
		exit( EXIT_FAILURE );
	}

	return EXIT_SUCCESS;
} // int main

#endif

