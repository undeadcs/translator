
#ifndef DAEMON_H
#define DAEMON_H

namespace UdSDK {

	/**
	 *	Класс демон/сервис
	 */
	class CDaemon {
	public:
		enum DaemonError {
			UDERROR_OK,					// нет ошибки
			UDERROR_FAILED_PID,			// не удалось выполнить fork
			UDERROR_FAILED_SID,			// не удалось получить id сессии
			UDERROR_FAILED_DIR,			// не удалось выбрать директорию
			UDERROR_NO_LOG,				// отсутствует лог
			UDERROR_PARENT,				// родительский процесс
			UDERROR_NO_SOCKET,			// не удалось создать сокет
			UDERROR_NO_BIND,			// не удалось связать сокет портом
			UDERROR_SOCKET_FAIL,		// провалилась операция работы с сокетом
			UDERROR_INVALID_DATA,		// неверная данная
			UDERROR_NO_CONFIG,			// отсутствует конфиг
			UDERROR_UNKNOWN_CMD			// не известная команда
		};

		enum DaemonCommand {
			UDCMD_LOAD_DUMP,			// подгрузка дампа
			UDCMD_LOAD_COMPOSITION,		// подгрузка композиции
			UDCMD_GET_TRANSLATE,		// получение перевода
			UDCMD_ADD_TANSLATE,			// добавление перевода
			UDCMD_LIST,					// получение списка
			UDCMD_INFO,					// получение информации
			UDCMD_DEL_WORD,				// удаление слова
			UDCMD_DEL_TRANSLATE,		// удаление перевода
			UDCMD_DEL_WORD_TRANSLATE,	// удаление перевода выбранного слова
			UDCMD_DEL_BY_ID				// удаление по id
		};

	private:
#ifdef FOR_WIN32
		WSADATA			m_objWinSockData;
#else
#endif
		pid_t			m_iPid,
						m_iSid;
		SOCKET			m_hSocket;
		sockaddr_in		m_objSockAddr;
		udPConfig		m_pConfig;
		udPLog			m_pLog;
		udPTree			m_pTree;
		//udStrMapTree	m_mapTree;

	public:
		CDaemon( );
		CDaemon( const CDaemon& objCopy );

		~CDaemon( );

		DaemonError	Proc( int argc, char** argv );

	private:
		DaemonError	Init( string szConfigPath );
		DaemonError	InitConfig( string szConfigPath );
		DaemonError InitLog( );
		DaemonError InitDump( );
		DaemonError	InitSocket( );
		void		Terminate( );
		DaemonError	Cycle( );
		DaemonError	HandleConnection( SOCKET hSocket );
		DaemonError	ReadData( SOCKET hSocket, udPPDWord pData, udPDWord pLen );
		DaemonError	ValidateData( udPDWord pData, udDWord dwLen );
		DaemonError ReadDWord( SOCKET hSocket, udPDWord dwOut );
		DaemonError	ProcData( SOCKET hSocket, udPDWord pData, udDWord dwLen );

		void		ShowInfo( SOCKET hSocket, udPDWord pData, udDWord dwLen );
		void		ShowList( SOCKET hSocket, udPDWord pData, udDWord dwLen );
		void		DelById( SOCKET hSocket, udPDWord pData, udDWord dwLen );
		void		AddTranslate( SOCKET hSocket, udPDWord pData, udDWord dwLen );
		void		DelWord( SOCKET hSocket, udPDWord pData, udDWord dwLen );
		void		DelTranslate( SOCKET hSocket, udPDWord pData, udDWord dwLen );
		void		DelWordTranslate( SOCKET hSocket, udPDWord pData, udDWord dwLen );
		void		GetTranslate( SOCKET hSocket, udPDWord pData, udDWord dwLen );

	}; // class CDaemon

} // namespace UdSDK

#endif
