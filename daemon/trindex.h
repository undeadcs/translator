
#ifndef TRINDEX_H
#define TRINDEX_H

namespace UdSDK {

	/**
	 * 	Индекс слова в дереве
	 */
	class CTrIndex {
		udPNode		m_pNode;
		udPIndex	m_pPrev,
					m_pNext;

	public:
		CTrIndex( udPNode m_pNode = NULL, udPIndex pPrev = NULL, udPIndex pNext = NULL );
		CTrIndex( const udIndex& objIndex );

		~CTrIndex( );

		void	SetNode( udPNode pNode = NULL );
		void	SetPrev( udPIndex pPrev = NULL );
		void	SetNext( udPIndex pNext = NULL );

		udPNode		GetTreeNode( );
		udPIndex	GetPrev( );
		udPIndex	GetNext( );

	}; // class CTrIndex

} // namespace UdSDK

#endif
