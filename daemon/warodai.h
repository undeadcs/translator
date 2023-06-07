
#ifndef WARODAI_H
#define WARODAI_H

namespace UdSDK {

	/**
	 * 	Парсер набора статей warodai
	 */
	class CWarodai {
		string	m_szFileIn,
				m_szFileOut;
		udDWord	m_dwFrameSize;

	public:
		CWarodai( );
		CWarodai( const CWarodai& objCopy );

		~CWarodai( );

		void Proc( );
		void PageSep( );

		void SetFrameSize( udDWord dwFrameSize ) { m_dwFrameSize = dwFrameSize; }
		void SetFileIn( string szFileIn ) { m_szFileIn = szFileIn; }
		void SetFileOut( string szFileOut ) { m_szFileOut = szFileOut; }

	private:
		udStlStr ReadBlock( FILE* pFile );

	}; // class CWarodai

} // namespace UdSDK

#endif
