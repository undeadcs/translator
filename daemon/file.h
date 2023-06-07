
#ifndef FILE_H
#define FILE_H

namespace UdSDK {

	/**
	 * 	Файл
	 */
	class CFile {
	public:
		enum Encoding {
			UNKNOWN = 0,	// неизвестная кодировка
			CP1251,			// cp1251
			CP1252,			// cp1252
			EUC_JP,			// EUC-JP
			UTF_8,			// UTF-8
			UTF_16,			// UTF-16BE
			UTF_16_LE,		// UTF-16LE
			UTF_32,			// UTF-32 (UTF-32BE, UCS-4)
			UTF_32_LE,		// UTF-32LE
		};

	private:
		Encoding	m_iReadEnc,		// кодировка чтения файла данных
					m_iInternalEnc,	// кодировка хранения данных файла, внутри объекта
					m_iWriteEnc;	// кодировка при записи

		udPVoid		m_pData;		// текст
		udDWord		m_dwLength;		// длина текста в байтах
		string		m_szFileName;	// имя файла
		udPStdFile	m_pFile;		// указатель на дескриптор файла

	public:
		CFile( );
		CFile( Encoding iReadEnc, Encoding iInternalEnc, Encoding iWriteEnc );
		CFile( const CFile& objCopy );

		~CFile( );

		udDWord	OpenFile( string szFileName );
		udDWord	SetText( string szText );
		udDWord	SetText( udStlStr szText );
		void	SetReadingEncoding( Encoding iReadEncoding );
		void	SetInternalEncoding( Encoding iInternalEncoding );
		void	SetWritingEncoding( Encoding iWriteEncoding );

	private:
		udDWord	SwitchInternalEncoding( Encoding iInternalEncoding );

	}; // class CFile

	/**
	 * 	Файл в формате UTF-32 (BE)
	 */
	class CFileUtf32 {
		udStlStr	m_szText;		// текст файла
		udDWord		m_dwHasBOM;		// был ли в файле BOM

	public:
		CFileUtf32( );

		~CFileUtf32( );

		udDWord 	Load( string szFileName );						// загружает текст из файла
		udDWord 	Length( );										// возвращает количество символов в тексте
		udStlStr&	GetText( );										// возвращает текст
		void		Clear( );										// очищает текст
		udDWord		Flush( string szFileName );						// сливает текст в файл
		udDWord		Flush( FILE* pFile );							// сливает текст в файл
		udDWord		Flush( string szFileName, const udStlStr& szText );
		void		SetText( const udStlStr& szText );

	private:
		udDWord CheckBOM( udPStdFile pFile );		// проверяет файл на содержание сигнатуры
		udDWord ReadFile( udPStdFile pFile );		// чтение файла
		void	PutBOM( udPStdFile pFile );		// пишет BOM в файл
		void	PutText( udPStdFile pFile );		// пишет текст в файл
		void	PutChar( udPStdFile pFile, udDWord dwChar );		// пишет символ в файл

	}; // class CFileUtf32

} // namespace UdSDK

#endif
