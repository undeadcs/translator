
#ifndef CONFIG_H
#define CONFIG_H

namespace UdSDK {

	/**
	 * 	Конфиг
	 */
	class CConfig {
	public:
		enum ConfError {
			UDERROR_OK = 0,
			UDERROR_NOT_LOADED,
			UDERROR_CANT_OPEN,
			UDERROR_INVALID_ARG
		};

	private:
		udDWord	m_dwLogLevel;
		string	m_szLogPath,
				m_szDumpPath;

	public:
		CConfig( );
		CConfig( const CConfig& objCopy );

		~CConfig( );

		ConfError	Load( const string& szPath );

		string		GetDumpPath( ) const { return m_szDumpPath; }
		string		GetLogPath( ) const { return m_szLogPath; }

	}; // calss CConfig

} // namespace UdSDK

#endif
