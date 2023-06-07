
#ifndef CONVERTER_H
#define CONVERTER_H

namespace UdSDK {

	/**
	 * 	Преобразователь текста
	 */
	class CConverter {
		iconv_t	m_hIconv;

	public:
		CConverter( );
		CConverter( const CConverter& objCopy );

		~CConverter( );

		udDWord Create( string szInpEnc, string szOutEnc );
		udDWord Destroy( );
		udDWord Convert(
#ifdef FOR_WIN32
		const char* szIn,
#else
		char* szIn,
#endif
		size_t& iInputLen, char* szOut, size_t& iOutputLen );
		udDWord ConvertFile( const char* szFileIn, const char* szFileOut );

	}; // class CConverter

} // namespace UdSDK

#endif
