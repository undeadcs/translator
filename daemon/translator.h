/**
 * Команды:
 * l dump_path
 * c compose_path
 * t input_path output_path
 * a compose_path
 * s output_path [begin] [amount]
 * i
 * d input_path
 * e input_path
 * r input_path [input_path]
 * g in_path
 *
 * Коды возврата:
 * 0 - все прошло успешно
 * 1 - не достаточно аргументов
 * 2 - не известная команда
 */

#ifndef TRANSLATOR_H
#define TRANSLATOR_H

namespace UdSDK {

	/**
	 * 	Переводчик
	 */
	class CTranslator {
	public:
		enum TranslatorError {
			UDERROR_OK = 0,				// ошибки нет
			UDERROR_INVALID_CMD,		// корявая команда
			UDERROR_UNKNOWN_CMD,		// не известная команда
			UDERROR_NO_CONFIG,			// отсутствует конфиг
			UDERROR_NO_LOG,				// отсутствует лог
			UDERROR_NO_DUMP				// отсутствует основной дамп
		};

	private:
		udPConfig	m_pConfig;
		udPLog		m_pLog;
		udMapLog	m_mapLog;
		udDWord		m_dwLogId;
		udMapTree	m_mapTree;
		udDWord		m_dwTreeId;
		udPTree		m_pTree;

	public:
		CTranslator( );
		CTranslator( const CTranslator& objCopy );

		~CTranslator( );

		TranslatorError	Proc( int argc, char** argv );
		TranslatorError	BeginLog( const string& szLogPath, udPDWord dwId );
		TranslatorError	BeginLog( const char* szLogPath, udPDWord dwId );
		TranslatorError	EndLog( udDWord dwId );
		udPLog			GetLog( udDWord dwId );
		udPLog			GetLog( );

	private:
		TranslatorError	Init( const char* szConfigPath );
		TranslatorError	ValidateCmdArg( int argc, char** argv );
		void			Destroy( );
		void			LoadDump( const char* szDumpPath );
		void			LoadComposition( const char* szComposeName );
		void			Translate( const char* szInputFilePath, const char* szOutpuFilePath );
		void			AddTranslate( const char* szComposeName );
		void			ShowList( int argc, char** argv );
		void			ShowInfo( );
		void			DelWord( const char* szInputPath );
		void			DelTranslate( const char* szInputPath );
		void			DelTargetTranslate( int argc, char** argv );
		void			ProcIdRequest( const char* szFilePath );

	}; // class CTranslator

} // namespace UdSDK

#endif
