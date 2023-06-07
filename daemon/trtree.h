
#ifndef TRTREE_H
#define TRTREE_H

namespace UdSDK {

	/**
	 * 	Дерево перевода
	 */
	class CTrTree {
	public:
		enum TreeError {
			UDERROR_OK = 0
		};

		enum SpecCodes {
			SEPARATOR	= 0x0000007cL, // разделитель слов |
			CARETRETURN	= 0x0000000dL, // возврат каретки \r
			LINEFEED	= 0x0000000aL, // перевод строки \n
			WORDSEP		= 0x000f0001L, // разделитель вариантов перевода
			TRSEP		= 0x000f0002L, // разделитель соответствия перевода
			REGIONSEP	= 0x000f0003L, // разделитель областей соответствия
		};

	private:
		CTrNode*	m_pRoot;		// корень дерева
		udDWord		m_dwCount;		// количество слов
		udDWord		m_dwNodeCount;	// количество узлов дерева

	public:
		CTrTree( );
		CTrTree( const CTrTree& objTree );

		~CTrTree( );

		void		Clear( );
		void		AddWords( const udStlStr& szText );											// добавление слов из текста
		void		AddWord( const udArrWord& arrWord );										// добавление слов из набора
		CTrNode*	AddWord( const udStlStr& szWord );											// добавление слова
		udDWord		Print( udStlStr& szText );													// распечатка словарного запаса в текст
		udDWord		Print( const char* szFileName );											// распечатка словарного запаса в файл
		udDWord		Print( FILE* pFile );														// распечатка словарного запаса в файл
		udDWord		GetWordCount( );															// получение числа слов
		udDWord		Load( const char* szComposeName );											// загрузка перевода
		void		AddTranslatedWord( const udStlStr& szWord1, const udStlStr& szWord2 );		// добавление переведенного слова
		udDWord		PrintTranslate(	const char* szFileName );									// распечатка слов с переводом
		udDWord		PrintTranslate( udStlStr& szText, udDWord dwOffset = 0, udDWord dwCount = 0 );
		udArrWord	Find( const udStlStr& szWord );												// возвращает набор вариантов перевода слова
		udDWord		Dump( const char* szFileName );												// дампит дерево в файл
		udDWord		Dump( udStlStr& szOut );
		udDWord		DumpLoad( string szFileName );												// грузит дерево из дампа
		udDWord		GetNodeCount( );															// получение числа узлов
		udDWord		DelWord( const udStlStr& szWord );											// удаляет слово из словаря
		udDWord		DelWordFromTranslate( const udStlStr& szWord );								// удаляет слово из переводов других слов
		udDWord		DelTranslate( const udStlStr& szWord, const udStlStr& szTrWord );			// удаляет переводы заданного слова
		udDWord		PrintXml( const char* szFileName, udDWord dwShowId = 0 );
		udDWord		PrintXml( udStlStr& szText, udDWord dwOffset = 0, udDWord dwCount = 0, udDWord dwShowId = 0 );
		udArrWord	GetWordList( );
		udDWord		DelWordById( udDWord dwId );
		udDWord		DelTranslateById( udDWord dwWordId, const udStlStr& arrId );

		void		Translate( const char* szFileIn, const char* szFileOut );

		CTrNode*	GetRoot( ) { return m_pRoot; }

	private:
		CTrNodeInfo*	ReadNode( udStlStr::iterator& itCurrent, udStlStr::iterator& itEnd );
		void			MergeTree( udPNode pRootLeft, udPNode pRootRight );
		void			GetSortedTranslate( udPNode pNode, udStlStr& szText, udDWord dwShowId = 0 );
		void			LogStats( );
		void			WriteId( udStlStr& szText, udDWord dwId );

	}; // class CTrTree

} // namespace UdSDK

#endif
