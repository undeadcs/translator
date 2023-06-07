
#ifndef TRNODE_H
#define TRNODE_H

namespace UdSDK {

	/**
	 * 	Узел дерева перевода
	 */
	class CTrNode {
		udDWord		m_dwHasWord;	// признак наличия слова
		udDWord		m_dwVisited;	// маркер обхода
		udDWord		m_dwCode;		// индекс потомка (код символа перехода)
		udDWord		m_dwId;			// id узла в дереве
		udPNode		m_pParent;		// указатель на родителя
		udPNode		m_pChild;		// указатель на первого потомка
		udPNode		m_pNextSibling;	// следующий сосед
		udPNode		m_pPrevSibling;	// предыдущий сосед
		udPIndex	m_pTranslate;	// указатели на узлы перевода

	public:
		CTrNode( udDWord dwHasWord = 0L, udDWord dwCode = 0L, CTrNode* pParent = NULL );
		CTrNode( udDWord dwHasWord, udDWord dwCode, udDWord dwIde, CTrNode* pParent = NULL );
		CTrNode( const CTrNode& objNode );

		~CTrNode( );

		void		SetParent( CTrNode* pParent );							// установка родителя
		void		SetHasWord( udDWord dwHasWord );						// установка наличия слова
		CTrNode*	AddChild( udDWord dwCode, udDWord dwHasWord = 0 );		// добавления потомка по индексу
		CTrNode*	GetChildren( );											// получение указателя на потомков
		CTrNode*	GetParent( );											// получение родителя
		CTrNode*	GetChild( udDWord dwCode );								// получение потомка по индексу
		void		SetNextSibling( CTrNode* pNode );						// установка указателя на соседа справа
		void		SetPrevSibling( CTrNode* pNode );						// установка указателя на соседа слева
		CTrNode*	GetNextSibling( );										// получения указателя на соседа справа
		CTrNode*	GetPrevSibling( );										// получение указателя на соседа слева
		void		SetCode( udDWord dwCode );								// установка кода символа
		void		SetVisited( udDWord dwVisited );						// установка значения поля посещения
		void		AddTranslate( CTrNode* pNode );							// добавление узла перевода
		void		SetId( udDWord dwId );									// установка ID узла
		void		PushChild( CTrNode* pNode );
		void		PushTranslate( CTrNode* pNode );
		void		DelTranslate( CTrNode* pNode );
		void		DelTranslate( udDWord dwId );						// удаление перевода по заданному id
		void		ClearTranslate( );										// очистка списка перевода
		void		SetTranslate( CTrIndex* pIndex );

		udDWord		GetHasWord( ) const;									// какой уровень владения словом
		udDWord		GetCode( ) const;										// индекс узла среди потомков
		udDWord		GetVisited( ) const;									// возвращает значение поля посещения
		udDWord		GetId( ) const;											// возвращает id узла

		CTrIndex*	GetTranslate( );

	}; // class CTrNode

} // namespace UdSDK

#endif
