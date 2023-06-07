
#ifndef TRINFO_H
#define TRINFO_H

namespace UdSDK {

	/**
	 * 	Информация об узле дерева ( для дампа )
	 */
	class CTrNodeInfo {
		udDWord		m_dwId;				// id узла
		udDWord		m_dwHasWord;		// значения поля HasWord узла
		udDWord		m_dwCode;			// код символа
		udDWord		m_dwChildNum;		// количество потомков
		udDWord		m_dwTranslateNum;	// количество переводов
		udPDWord	m_pChildren;		// массив индексов узлов потомков
		udPDWord	m_pTranslate;		// массив индексов узлов перевода

	public:
		CTrNodeInfo( );
		CTrNodeInfo( udDWord dwId, udDWord dwHasWord, udDWord dwCode, udDWord dwChildNum, udDWord dwTranslateNum );
		CTrNodeInfo( const CTrNodeInfo& objInfo );

		~CTrNodeInfo( );

		void		Create( udDWord dwId, udDWord dwHasWord, udDWord dwCode, udDWord dwChildNum, udDWord dwTranslateNum );
		void		Clear( );

		udPNode		GetTreeNode( );
		udDWord		GetId( );
		udDWord		GetChildNum( );
		udDWord		GetTranslateNum( );
		udPDWord	GetChildren( );
		udPDWord	GetTranslate( );

		void		SetChild( udDWord dwPos, udDWord dwValue );
		void		SetTranslate( udDWord dwPos, udDWord dwValue );
		void		SetChildNum( udDWord dwChildNum );
		void		SetTranslateNum( udDWord dwTranslateNum );

	}; // class CTrNodeInfo

} // namespace UdSDK

#endif
