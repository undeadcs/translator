
#include "includes.h"
#include "externals.h"

namespace UdSDK {

	udDWord dwDelCount = 0;

	/**
	 * 	Очищает дерево
	 * 	@param pRoot CTrNode* корень дерева
	 * 	@return void
	 */
	static void ClearTree( CTrNode*& pRoot ) {
		if ( pRoot ) {
			++dwDelCount;

			CTrNode* pNode = pRoot->GetChildren( ), *pTmp = NULL;
			while( pNode ) {
				pTmp = pNode;
				pNode = pNode->GetNextSibling( );

				ClearTree( pTmp );
			}

			delete pRoot;
			pRoot = NULL;
		}
	} // void CLearTree

	/**
	 * 	Подсчитывает количество потомков узла
	 * 	@param pNode CTrNode* узел
	 * 	@return udDWord
	 */
	static udDWord CountChildren( CTrNode* pNode ) {
		udDWord dwCount = 0;
		if ( pNode ) {
			pNode = pNode->GetChildren( );
			while ( pNode ) {
				++dwCount;
				pNode = pNode->GetNextSibling( );
			}
		}
		return dwCount;
	} // udDWord CountChildren

	/**
	 * 	Подсчитывает количество переводов слова
	 * 	@param pNode CTrNode* узел дерева
	 * 	@return udDWord
	 */
	static udDWord CountTranslate( CTrNode* pNode ) {
		udDWord dwCount = 0;
		if ( pNode ) {
			CTrIndex* pIndex = pNode->GetTranslate( );
			while ( pIndex ) {
				++dwCount;
				pIndex = pIndex->GetNext( );
			}
		}
		return dwCount;
	} // udDWord CountTranslate

	/**
	 * 	Обходит дерево и сливает его данные в текст
	 * 	@param pRoot CTrNode* текущий корень дерева
	 * 	@param szDump udStlStr& строка, куда пишутся данные
	 * 	@return void
	 */
	static void WalkDump( CTrNode* pRoot, udStlStr& szDump ) {
		if ( pRoot ) {
			szDump += pRoot->GetId( );
			szDump += pRoot->GetHasWord( );
			szDump += pRoot->GetCode( );
			szDump += CountChildren( pRoot );

			CTrNode* pNode = pRoot->GetChildren( );
			while ( pNode ) {
				szDump += pNode->GetId( );
				pNode = pNode->GetNextSibling( );
			}

			szDump += CountTranslate( pRoot );

			CTrIndex* pIndex = pRoot->GetTranslate( );
			while ( pIndex ) {
				szDump += pIndex->GetTreeNode( )->GetId( );
				pIndex = pIndex->GetNext( );
			}

			pNode = pRoot->GetChildren( );
			while( pNode ) {
				WalkDump( pNode, szDump );
				pNode = pNode->GetNextSibling( );
			}
		}
	} // void WalkDump

	/**
	 * 	Удаление узла перевода
	 * 	@param pNode CTrNode* узел, для которого удаляется перевод
	 * 	@param pDelNode CTrNode* узел, который является переводом
	 * 	@return void
	 */
	static void DelTranslateDict( CTrNode* pNode, CTrNode* pDelNode ) {
		if ( pNode ) {
			pNode->DelTranslate( pDelNode );
			CTrNode* pTmp = pNode->GetChildren( );
			while( pTmp ) {
				DelTranslateDict( pTmp, pDelNode );
				pTmp = pTmp->GetNextSibling( );
			}
		}
	} // void DelTranslateDict

	/**
	 * 	Строит список узлов, из которых можно построить слово
	 * 	@param pRoot CTrNode корень дерева поиска
	 * 	@param vctNode vector набор узлов
	 * 	@return void
	 */
	static void	BuildWordNodeList( CTrNode* pRoot, vector< CTrNode* >& vctNode ) {
		if ( pRoot ) {
			if ( pRoot->GetHasWord( ) ) {
				vctNode.push_back( pRoot );
			}
			CTrNode* pNode = pRoot->GetChildren( );
			while( pNode ) {
				BuildWordNodeList( pNode, vctNode );
				pNode = pNode->GetNextSibling( );
			}
		}
	} // void BuildWordNodeList

	/**
	 * 	Строит слово
	 * 	@param pNode CTrNode конечный узел слова
	 * 	@param szText udStlStr& строка, в которую будет записано слово
	 * 	@return void
	 */
	static void BuildWord( CTrNode* pNode, udStlStr& szText ) {
		udStlStr szTmp;

		while( pNode->GetParent( ) ) {
			szTmp += pNode->GetCode( );
			pNode = pNode->GetParent( );
		}

		for( udStlStr::reverse_iterator i = szTmp.rbegin( ); i != szTmp.rend( ); ++i ) {
			szText += *i;
		}
	} // void BuildWord

	static CTrNode* GetTranslateNodeById( CTrIndex* pIndex, udDWord dwId ) {
		CTrNode *pNode = NULL;
		while( pIndex ) {
			pNode = pIndex->GetTreeNode( );
			if ( dwId == pNode->GetId( ) ) {
				return pNode;
			}
			pIndex = pIndex->GetNext( );
		}
		return NULL;
	} // CTrNode* GetTranslateNodeById

	/**
	 * 	Сортировка переводов
	 * 	@param pNode CTrNode* узел
	 * 	@return void
	 */
	static void SortNodeTranslate( CTrNode* pNode ) {
		if ( pNode ) {
			CTrIndex *pIndex = pNode->GetTranslate( );
			if ( pIndex ) {
				CTrTree *objTree = new CTrTree;
				CTrNode *pNode1 = NULL, *pTmp = NULL;
				udStlStr szTmp;
				while( pIndex ) {
					pNode1 = pIndex->GetTreeNode( );
					BuildWord( pNode1, szTmp );
					pTmp = objTree->AddWord( szTmp );
					pTmp->SetId( pNode1->GetId( ) );

					szTmp.clear( );
					pIndex = pIndex->GetNext( );
				}

				pNode1 = objTree->GetRoot( );
				vector< CTrNode* > vctNode;
				vector< CTrNode* >::iterator i, j;

				BuildWordNodeList( pNode1, vctNode );
				j = vctNode.end( );

				pIndex = pNode->GetTranslate( );
				pNode->SetTranslate( NULL );

				for( i = vctNode.begin( ); i != j; ++i ) {
					pNode1 = *i;
					pTmp = GetTranslateNodeById( pIndex, pNode1->GetId( ) );
					if ( pTmp ) {
						pNode->PushTranslate( pTmp );
					}
				}

				CTrIndex *pTmpIndex = NULL;
				while( pIndex ) {
					pTmpIndex = pIndex;
					pIndex = pIndex->GetNext( );

					delete pTmpIndex;
					pTmpIndex = NULL;
				}

				objTree->Clear( );
				delete objTree;
				objTree = NULL;
			}
		}
	} // void SortNodeTranslate

	static udPNode GetNodeById( udPNode pRoot, udDWord dwId ) {
		if ( pRoot ) {
			if ( dwId == pRoot->GetId( ) ) {
				return pRoot;
			}
			udNode *pNode = pRoot->GetChildren( ), *pTmp = NULL;
			while( pNode ) {
				pTmp = GetNodeById( pNode, dwId );
				if ( pTmp ) {
					return pTmp;
				}
				pNode = pNode->GetNextSibling( );
			}
		}
		return NULL;
	} // udPNode GetNodeById

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// class CTrTree

	/**
	 * 	Конструктор по умолчанию
	 */
	CTrTree::CTrTree( ) {
		m_pRoot			= new CTrNode;
		m_dwCount		= 0;
		m_dwNodeCount	= 1;
	}

	/**
	 * 	Конструктор копирования
	 */
	CTrTree::CTrTree( const CTrTree& objTree ) {
		if ( m_pRoot ) {
			this->Clear( );
		}

		m_pRoot			= objTree.m_pRoot;
		m_dwCount		= objTree.m_dwCount;
		m_dwNodeCount	= objTree.m_dwNodeCount;
	}

	/**
	 * 	Деструктор
	 */
	CTrTree::~CTrTree( ) {
		if ( m_pRoot ) {
			delete m_pRoot;
		}

		m_pRoot			= NULL;
		m_dwCount		= 0;
		m_dwNodeCount	= 0;
	}

	/**
	 * 	Очистка дерева
	 * 	@return void
	 */
	void CTrTree::Clear( ) {
		if ( m_pRoot ) {
			dwDelCount = 0;
			ClearTree( m_pRoot );
			if ( m_dwNodeCount != dwDelCount ) {
				g_pLog->Write( "delete failed\n" );
			}

			m_pRoot			= new CTrNode;
			m_dwCount		= 0;
			m_dwNodeCount	= 1;
		}
	} // void Clear

	/**
	 * 	Добавление слов в словарный запас
	 * 	@param szText const udStlStr& текст
	 * 	@return void
	 */
	void CTrTree::AddWords( const udStlStr& szText ) {
		udStlStr::const_iterator i = szText.begin( ), j = szText.end( );
		udDWord dwChar;
		udStlStr szWord;

		for( ; i != j; ++i ) {
			dwChar = *i;
			if ( dwChar == CTrTree::SEPARATOR ) {
				this->AddWord( szWord );
				szWord.clear( );
			} else {
				szWord += dwChar;
			}
		}

		if ( !( szWord.empty( ) ) ) {
			this->AddWord( szWord );
			szWord.clear( );
		}

	} // void AddWords

	/**
	 * 	Добавление слова в словарный запас
	 * 	@param arrWord const udArrWord& набор слов
	 * 	@return void
	 */
	void CTrTree::AddWord( const udArrWord& arrWord ) {
		for( udArrWord::const_iterator i = arrWord.begin( ); i != arrWord.end( ); ++i ) {
			this->AddWord( *i );
		}
	} // void AddWord

	/**
	 * 	Добавление слова в словарный запас
	 * 	@param szWord const udStlStr& слово
	 * 	@return void
	 */
	CTrNode* CTrTree::AddWord( const udStlStr& szWord ) {
		//char tmp[ 1024 ];
		if ( m_pRoot && szWord.size( ) ) {
			CTrNode *pNode = m_pRoot, *pTmp = NULL;
			udDWord dwCur = 0;
			udStlStr::const_iterator j = szWord.end( );

			for( udStlStr::const_iterator i = szWord.begin( ); i != j; ++i ) {
				dwCur = *i;
				pTmp = pNode->GetChild( dwCur );
				if ( pTmp == NULL ) {
					pTmp = pNode->AddChild( dwCur, 0 );
					pTmp->SetId( m_dwNodeCount++ );
				}
				pNode = pTmp;
			}

			if ( pNode && !( pNode->GetHasWord( ) ) ) {
				if ( pNode == m_pRoot ) {
					cout << "wtf? " << szWord.size( ) << ' ' << szWord[ 0 ] << endl;
				}
				++m_dwCount;
				pNode->SetHasWord( 1 );
				//sprintf( tmp, "code: %4lx\n", pNode->GetCode( ) );
				//g_pLog->Write( tmp );
			}

			return pNode;
		}
		return NULL;
	} // void AddWord

	/**
	 * 	Распечатка словарного запаса
	 * 	@param szText udStlStr& текст, куда записывать слова
	 * 	@return udDWord
	 */
	udDWord CTrTree::Print( udStlStr& szText ) {
		vector< CTrNode* > vctNode;
		BuildWordNodeList( m_pRoot, vctNode );

		vector< CTrNode* >::iterator itForEnd = vctNode.end( );
		--itForEnd;

		for( vector< CTrNode* >::iterator i = vctNode.begin( ); i != vctNode.end( ); ++i ) {
			BuildWord( *i, szText );
			if ( i != itForEnd ) {
				szText += CTrTree::CARETRETURN;
				szText += CTrTree::LINEFEED;
			}
		}

		return 1;
	} // udDWord Print

	/**
	 * 	Распечатка словарного запаса
	 * 	@param szFileName const char* имя файла
	 * 	@return udDWord
	 */
	udDWord CTrTree::Print( const char* szFileName ) {
		CFileUtf32 objFile;
		this->Print( objFile.GetText( ) );
		return objFile.Flush( szFileName );
	} // udDWord Print

	/**
	 * 	Распечатка словарного запаса
	 * 	@param pFile FILE* указатель на открытый файл
	 * 	@return udDWord
	 */
	udDWord CTrTree::Print( FILE* pFile ) {
		CFileUtf32 objFile;
		this->Print( objFile.GetText( ) );
		return objFile.Flush( pFile );
	} // udDWord Print

	/**
	 * 	Получение количества слов
	 * 	@return udDWord
	 */
	udDWord CTrTree::GetWordCount( ) {
		return m_dwCount;
	} // udDWord GetWordCount

	/**
	 * 	Загрузка перевода из файлов
	 * 	@param szComposeName const char* префикс композиции ( [prefix]_[ru|jp].utf32 )
	 * 	@return udDWord 0 - не получилось загрузить, 1 - получилось
	 */
	udDWord CTrTree::Load( const char* szComposeName ) {
		CFileUtf32	objFile;
		udStlStr	szText;
		string		szFileName( szComposeName );
		udArrWord	arrWordRu, arrWordJp;
		udStlStr	szWord;

		szFileName += "_ru.utf32";

		if ( objFile.Load( szFileName.c_str( ) ) ) {
			szText = objFile.GetText( );
			udStlStr::iterator i = szText.begin( ), j = szText.end( );
			udDWord dwChar;

			for( ; i != j; ++i ) {
				dwChar = *i;
				if ( dwChar == CTrTree::SEPARATOR ) {
					arrWordRu.push_back( szWord );
					szWord.clear( );
				} else {
					szWord += dwChar;
				}
			}
			if ( !( szWord.empty( ) ) ) {
				arrWordRu.push_back( szWord );
				szWord.clear( );
			}

			objFile.Clear( );
			szText.clear( );
			szFileName = szComposeName;
			szFileName += "_jp.utf32";

			if ( objFile.Load( szFileName.c_str( ) ) ) {
				szText = objFile.GetText( );
				j = szText.end( );
				for( i = szText.begin( ); i != j; ++i ) {
					dwChar = *i;
					if ( dwChar == CTrTree::SEPARATOR ) {
						arrWordJp.push_back( szWord );
						szWord.clear( );
					} else {
						szWord += dwChar;
					}
				}

				if ( !( szWord.empty( ) ) ) {
					arrWordJp.push_back( szWord );
					szWord.clear( );
				}
			}

			if ( arrWordRu.size( ) == arrWordJp.size( ) ) {
				udArrWord::iterator	i = arrWordJp.begin( ),
									j = arrWordJp.end( ),
									u = arrWordRu.begin( ),
									v = arrWordRu.end( );
				while( i != j ) {
					this->AddTranslatedWord( *i, *u );

					++i;
					++u;
				}
				return 1;
			} else {
				cout << "different number of words" << endl;
			}
		} else {
			cout << "failed loading file" << endl;
		}
		return 0;
	} // udDWord Load

	void CTrTree::AddTranslatedWord( const udStlStr& szWord1, const udStlStr& szWord2 ) {
		CTrNode *pNode1 = NULL, *pNode2 = NULL;
		pNode1 = this->AddWord( szWord1 );
		pNode2 = this->AddWord( szWord2 );

		if ( pNode1 && pNode2 && ( pNode1 != pNode2 ) ) {
			pNode1->AddTranslate( pNode2 );
			pNode2->AddTranslate( pNode1 );

			SortNodeTranslate( pNode1 );
			SortNodeTranslate( pNode2 );
		}

		pNode1 = NULL;
		pNode2 = NULL;
	} // void AddTranslatedWord

	udDWord CTrTree::PrintTranslate( const char* szFileName ) {
		udStlStr szText;
		if ( !this->PrintXml( szText ) ) {
			return 0;
		}

		CFileUtf32 objFile;
		return objFile.Flush( szFileName, szText );
	} // udDWord PrintTranslate

	udDWord CTrTree::PrintTranslate( udStlStr& szText, udDWord dwOffset, udDWord dwCount ) {
		if ( m_pRoot ) {
			vector< CTrNode* > vctNode;
			BuildWordNodeList( m_pRoot, vctNode );

			CTrNode* pNode = NULL;
			CTrIndex* pIndex = NULL;
			vector< CTrNode* >::iterator itForEnd = vctNode.end( );
			--itForEnd;

			if ( !dwCount ) {
				dwCount = m_dwCount;
			}

			for( vector< CTrNode* >::iterator i = vctNode.begin( ); i != vctNode.end( ); ++i ) {
				if ( dwOffset ) { // не достигли нужного смещения
					--dwOffset;
				} else {
					if ( dwCount ) {
						--dwCount;
						pNode = *i;
						BuildWord( pNode, szText );
						pIndex = pNode->GetTranslate( );
						if ( pIndex ) {
							szText += SPACE;
							while( pIndex ) {
								BuildWord( pIndex->GetTreeNode( ), szText );
								if ( pIndex->GetNext( ) ) {
									szText += COMMA;
								}
								pIndex = pIndex->GetNext( );
							}
						}

						if ( i != itForEnd ) {
							szText += LF;
						}
					} else { // считали нужное количество слов
						break;
					}
				}
			}

			return 1;
		}
		return 0;
	} // udDWord PrintTranslate

	udArrWord CTrTree::Find( const udStlStr& szWord ) {
		udArrWord arrWord;
		if ( m_pRoot ) {
			CTrNode *pNode = m_pRoot;
			udStlStr::const_iterator i = szWord.begin( ), j = szWord.end( );
			for( ; i != j; ++i ) {
				pNode = pNode->GetChild( *i );
				if ( pNode == NULL ) {
					break;
				}
			}
			if ( pNode ) {
				udStlStr szTmp;
				CTrIndex* pIndex = pNode->GetTranslate( );
				while( pIndex ) {
					BuildWord( pIndex->GetTreeNode( ), szTmp );
					arrWord.push_back( szTmp );
					szTmp.clear( );
					pIndex = pIndex->GetNext( );
				}
				// добавляем перевод в отсортированном виде
				CTrTree objTree;
				udArrWord::iterator i, j = arrWord.end( );
				for( i = arrWord.begin( ); i != j; ++i ) {
					objTree.AddWord( *i );
				}
				arrWord.clear( );
				arrWord = objTree.GetWordList( );
				objTree.Clear( );

				pNode = NULL;
			}
		}
		return arrWord;
	} // udArrWord Find

	/**
	 * 	Дампит дерево в файл
	 * 	@param szFileName const char* имя файла
	 * 	@return udDWord
	 */
	udDWord CTrTree::Dump( const char* szFileName ) {
		char tmp[ 1024 ];
		clock_t tStart = clock( );
		if ( g_pLog ) {
			sprintf( tmp, "dumping into file '%s'\n", szFileName );
			g_pLog->Write( tmp );
		}
		CFileUtf32 objFile;
		udStlStr& szDump = objFile.GetText( );

		szDump += m_dwCount;
		szDump += m_dwNodeCount;

		WalkDump( m_pRoot, szDump );
		udDWord dwResult = objFile.Flush( szFileName );
		clock_t tEnd = clock( );
		float fDiff = float( tEnd - tStart ) / float( CLOCKS_PER_SEC );
		sprintf( tmp, "time to dump: %.3f sec, flush returned: %lu\n", fDiff, dwResult );
		if ( g_pLog ) {
			g_pLog->Write( tmp );
		}
		return dwResult;
	} // udDWord Dump

	udDWord CTrTree::Dump( udStlStr& szOut ) {
		szOut += m_dwCount;
		szOut += m_dwNodeCount;
		WalkDump( m_pRoot, szOut );
		return szOut.size( );
	} // udDWord Dump

	/**
	 * 	Загружает дерево из дампа
	 * 	@param szFileName string имя файла дампа
	 * 	@return udDWord
	 */
	udDWord CTrTree::DumpLoad( string szFileName ) {
		CFileUtf32 objFile;
		if ( objFile.Load( szFileName ) ) {
			clock_t tStart = clock( );
			udStlStr szText = objFile.GetText( );
			udStlStr::iterator i = szText.begin( ), j = szText.end( );
			udDWord dwCount = 0, dwNodeCount = 0;

			dwCount		= *i; ++i;
			dwNodeCount	= *i; ++i;

			udPPNode		arrNode = new udPNode[ dwNodeCount ];
			udPPNodeInfo	arrInfo = new udPNodeInfo[ dwNodeCount ];
			CTrNodeInfo*	pInfo = NULL;
			CTrNode			*pNode = NULL, *pRoot = NULL;
			udDWord			dwIndex = 0, dwIndex1 = 0, dwNum = 0, dwCur = 0, dwId = 0;
			udDWord*		pTmp;

			while( 1 ) {
				if ( i == j ) {
					break;
				}
				pInfo = this->ReadNode( i, j );
				if ( pInfo ) {
					dwId = pInfo->GetId( );
					arrInfo[ dwId ] = pInfo;
					arrNode[ dwId ] = pInfo->GetTreeNode( );

					pNode = NULL;
					pInfo = NULL;
				}
			}

			for( dwIndex = 0; dwIndex < dwNodeCount; ++dwIndex ) {
				pNode = arrNode[ dwIndex ];
				pInfo = arrInfo[ dwIndex ];
				if ( pNode && pInfo ) {
					dwNum = pInfo->GetChildNum( );
					if ( dwNum ) {
						pTmp = pInfo->GetChildren( );
						for( dwIndex1 = 0; dwIndex1 < dwNum; ++dwIndex1 ) {
							dwCur = pTmp[ dwIndex1 ];
							pNode->PushChild( arrNode[ dwCur ] );
						}
					}

					dwNum = pInfo->GetTranslateNum( );
					if ( dwNum ) {
						pTmp = pInfo->GetTranslate( );
						for( dwIndex1 = 0; dwIndex1 < dwNum; ++dwIndex1 ) {
							dwCur = pTmp[ dwIndex1 ];
							pNode->PushTranslate( arrNode[ dwCur ] );
						}
					}
				}
			}

			for( dwIndex = 0; dwIndex < dwNodeCount; ++dwIndex ) {
				if ( arrNode[ dwIndex ]->GetParent( ) == NULL ) {
					pRoot = arrNode[ dwIndex ];
				}
			}

			for( dwIndex = 0; dwIndex < dwNodeCount; ++dwIndex ) {
				if ( arrNode[ dwIndex ] ) {
					arrNode[ dwIndex ] = NULL;
				}
				if ( arrInfo[ dwIndex ] ) {
					arrInfo[ dwIndex ] = NULL;
				}
			}

			delete [ ] arrNode;
			delete [ ] arrInfo;

			if ( m_pRoot->GetChildren( ) ) {
				MergeTree( m_pRoot, pRoot );
				ClearTree( pRoot );
			} else {
				m_dwCount		= dwCount;
				m_dwNodeCount	= dwNodeCount;
				m_pRoot = pRoot;
				pRoot = NULL;
			}

			objFile.Clear( );
			clock_t tEnd = clock( );
			float fDiff = float( tEnd - tStart ) / float( CLOCKS_PER_SEC );
			char tmp[ 1204 ];
			sprintf( tmp, "loaded dump in %.3f sec\n", fDiff );
			g_pLog->Write( tmp );

			return 1;
		}

		return 0;
	} // udDWord DumpLoad

	/**
	 * 	Возвращает количество узлов дерева
	 * 	@return udDWord
	 */
	udDWord CTrTree::GetNodeCount( ) {
		return m_dwNodeCount;
	} // udDWord GetNodeCount

	/**
	 * 	Удаляет слово
	 */
	udDWord CTrTree::DelWord( const udStlStr& szWord ) {
		CTrNode* pNode = m_pRoot;
		for( udStlStr::const_iterator i = szWord.begin( ), j = szWord.end( ); i != j; ++i ) {
			if ( !pNode ) {
				break;
			}
			pNode = pNode->GetChild( *i );
		}
		if ( pNode && pNode->GetHasWord( ) ) {
			g_pLog->Write( "deleting word from translates\n" );
			DelTranslateDict( m_pRoot, pNode );
			pNode->SetHasWord( 0 );
			pNode->ClearTranslate( );
			--m_dwCount;
			return 1;
		}
		return 0;
	} // udDWord

	udDWord CTrTree::DelWordFromTranslate( const udStlStr& szWord ) {
		udStlStr::const_iterator i = szWord.begin( ), j = szWord.end( );
		CTrNode* pNode = m_pRoot;
		for( ; i != j; ++i ) {
			if ( !pNode ) {
				break;
			}
			pNode = pNode->GetChild( *i );
		}
		if ( pNode && pNode->GetHasWord( ) ) {
			DelTranslateDict( m_pRoot, pNode );
			return 1;
		}
		return 0;
	} // udDWord DelWordFromTranslate

	udDWord CTrTree::DelTranslate( const udStlStr& szWord, const udStlStr& szTrWord ) {
		if ( m_pRoot ) {
			CTrNode *pNode = m_pRoot;
			udStlStr::const_iterator i = szWord.begin( ), j = szWord.end( );
			for( ; i != j; ++i ) {
				pNode = pNode->GetChild( *i );
				if ( pNode == NULL ) {
					break;
				}
			}
			if ( pNode ) {
				// слово найдено
				if ( szTrWord.empty( ) ) {
					pNode->ClearTranslate( );
				} else {
					CTrNode* pTmp = m_pRoot;
					udStlStr::const_iterator i = szTrWord.begin( ), j = szTrWord.end( );
					for( ; i != j; ++i ) {
						pTmp = pTmp->GetChild( *i );
						if ( pTmp == NULL ) {
							break;
						}
					}
					if ( pTmp ) {
						pNode->DelTranslate( pTmp );
					}
				}
			}
		}
		return 0;
	} // udDWord Deltranslate

	/**
	 * 	Распечатка словаря в виде xml
	 * 	@param szFileName const char* имя выходного файла
	 * 	@return udDWord 0 - слито, 1 - не слито
	 */
	udDWord CTrTree::PrintXml( const char* szFileName, udDWord dwShowId ) {
		if ( m_pRoot ) {
			udStlStr szText;

			if ( !this->PrintXml( szText, 0, 0, dwShowId ) ) {
				return 0;
			}

			CFileUtf32 objFile;
			objFile.Flush( szFileName, szText );
		}
		return 0;
	} // udDWord PrintXml

	/**
	 * 	Распечатка списка слов в xml формат
	 * 	@param szText udStlStr& текст для распечатки
	 * 	@param dwOffset udDWord начальный номер слова
	 * 	@param dwCount udDWord количество слов
	 */
	udDWord CTrTree::PrintXml( udStlStr& szText, udDWord dwOffset, udDWord dwCount, udDWord dwShowId ) {
		if ( m_pRoot ) {
			szText += StringStdToOur( "<?xml version=\"1.0\" encoding=\"UTF-32\"?>" );
			szText += LF;
			szText += StringStdToOur( "<dict total=\"" );

			char *p = LongToString( m_dwCount );
			szText += StringStdToOur( p );
			delete [ ] p;
			p = NULL;

			szText += StringStdToOur( "\">" );
			szText += LF;

			vector< CTrNode* > vctNode;
			BuildWordNodeList( m_pRoot, vctNode );

			CTrNode* pNode = NULL;
			vector< CTrNode* >::iterator itLast = vctNode.end( );

			--itLast;

			if ( !dwCount ) {
				dwCount = m_dwCount;
			}

			for( vector< CTrNode* >::iterator i = vctNode.begin( ); i != vctNode.end( ); ++i ) {
				if ( dwOffset ) { // не достигли нужного смещения
					--dwOffset;
				} else {
					if ( dwCount ) {
						--dwCount;
						pNode = *i;

						szText += LT;
						szText += 0x66;
						szText += GT;
						szText += LF;
						szText += TAB;
						szText += LT;
						szText += 0x6f;

						if ( dwShowId ) {
							this->WriteId( szText, pNode->GetId( ) );
						}

						szText += GT;

						BuildWord( pNode, szText );

						szText += LT;
						szText += 0x2f;
						szText += 0x6f;
						szText += GT;
						szText += LF;

						this->GetSortedTranslate( pNode, szText, dwShowId );

						szText += LT;
						szText += 0x2f;
						szText += 0x66;
						szText += GT;
						szText += LF;
					} else { // считали нужное количество узлов
						break;
					}
				}
			}

			vctNode.clear( );
			szText += StringStdToOur( "</dict>" );

			return 1;
		}
		return 0;
	} // udDWord PrintXml

	/**
	 *	Получение списка слов без их перевода
	 */
	udArrWord CTrTree::GetWordList( ) {
		udArrWord arrWord;
		udStlStr szTmp;
		vector< CTrNode* > vctNode;
		vector< CTrNode* >::iterator i, j;
		BuildWordNodeList( m_pRoot, vctNode );

		j = vctNode.end( );

		for( i = vctNode.begin( ); i != j; ++i ) {
			BuildWord( *i, szTmp );
			arrWord.push_back( szTmp );
			szTmp.clear( );
		}

		return arrWord;
	} // udArrWord GetWordList

	/**
	 *	Удаление слова по id
	 *	@param dwId udDWord id слова
	 *	@return udDWord
	 */
	udDWord CTrTree::DelWordById( udDWord dwId ) {
		CTrNode *pNode = GetNodeById( m_pRoot, dwId );
		if ( pNode ) {
			DelTranslateDict( m_pRoot, pNode );
			pNode->SetHasWord( 0 );
			pNode->ClearTranslate( );
			--m_dwCount;
			return 1;
		}
		return 0;
	} // udDWord DelWordById

	/**
	 * 	Удаление перевода слова по id
	 * 	@param dwWordId udDWord id слова
	 * 	@param arrId udStlStr массив id переводов
	 * 	@return udDWord
	 */
	udDWord CTrTree::DelTranslateById( udDWord dwWordId, const udStlStr& arrId ) {
		CTrNode *pNode = GetNodeById( m_pRoot, dwWordId );
		if ( pNode ) {
			if ( arrId.empty( ) ) {
				pNode->ClearTranslate( );
			} else {
				for( udStlStr::const_iterator i = arrId.begin( ), j = arrId.end( ); i != j; ++i ) {
					pNode->DelTranslate( *i );
				}
			}
			return 1;
		}
		return 0;
	} // udDWord DelTranslateById

	/**
	 * 	Перевод одного файла и запись результата в другой
	 * 	@param szFileIn const char* имя входного файла
	 * 	@param szFileOut const char* имя выходного файла
	 * 	@return void
	 */
	void CTrTree::Translate( const char* szFileIn, const char* szFileOut ) {
		CFileUtf32 objFile;
		if ( objFile.Load( szFileIn ) ) {
			CFileUtf32 objFileOut;
			udStlStr &szTextOut = objFileOut.GetText( ), &szText = objFile.GetText( ), szTmp;
			CTrNode *pNode = m_pRoot, *pTmp = NULL;
			CTrIndex *pIndex = NULL;
			udStlStr::iterator i, j, k;

			i = szText.begin( );
			j = szText.end( );

			for( ; i != j; ++i ) {
				if ( pNode ) {
					pTmp = pNode->GetChild( *i );
					if ( pTmp ) {
						if ( pTmp->GetHasWord( ) ) {
							pIndex = pTmp->GetTranslate( );
							BuildWord( pIndex->GetTreeNode( ), szTextOut );
							pIndex = pIndex->GetNext( );
							if ( pIndex ) {
								szTextOut += RBRACE_L;
								while( pIndex ) {
									BuildWord( pIndex->GetTreeNode( ), szTextOut );
									pIndex = pIndex->GetNext( );
								}
								szTextOut += RBRACE_R;
							}
							szTextOut += SPACE;
							pNode = m_pRoot;
							szTmp.clear( );
						} else {
							szTmp += *i;
							pNode = pTmp;
						}
					} else {
						if ( !szTmp.empty( ) ) {
							szTextOut += szTmp;
							szTmp.clear( );
						}
						szTextOut += *i;
					}
				}
			}
			objFileOut.Flush( szFileOut );
		}
	} // void Translate

	/**
	 * 	Считывает узел из текста
	 * 	@param itCurrent iterator текущая позиция в строке
	 * 	@param itEnd iterator последняя позиция в строке
	 * 	@return CTrNodeInfo*
	 */
	CTrNodeInfo* CTrTree::ReadNode( udStlStr::iterator& itCurrent, udStlStr::iterator& itEnd ) {
		udDWord dwId = 0, dwHasWord = 0, dwCode = 0;
		dwId = *itCurrent;
		++itCurrent;
		dwHasWord = *itCurrent;
		++itCurrent;
		dwCode = *itCurrent;
		++itCurrent;

		udDWord dwNum = 0, dwChar = 0, dwIndex = 0;
		CTrNodeInfo* pInfo = new CTrNodeInfo( dwId, dwHasWord, dwCode, 0, 0 );

		dwNum = *itCurrent;
		++itCurrent;
		if ( dwNum ) {
			pInfo->SetChildNum( dwNum );
			while( dwNum ) {
				dwChar = *itCurrent;
				++itCurrent;
				pInfo->SetChild( dwIndex++, dwChar );
				--dwNum;
			}
		}

		dwNum = *itCurrent;
		++itCurrent;
		if ( dwNum ) {
			dwChar = 0;
			dwIndex = 0;
			pInfo->SetTranslateNum( dwNum );
			while( dwNum ) {
				dwChar = *itCurrent;
				++itCurrent;
				pInfo->SetTranslate( dwIndex++, dwChar );
				--dwNum;
			}
		}

		return pInfo;
	} // void ReadNode

	/**
	 * 	Сливает 2 дерева в одно, к левому дереву клеится правое
	 * 	@param pRootLeft CTrNode* корень левого дерева
	 * 	@param pRootRight CTrNode* корень правого дерева
	 * 	@return void
	 */
	void CTrTree::MergeTree( CTrNode* pRootLeft, CTrNode* pRootRight ) {
		if ( pRootLeft && pRootRight ) {
			vector< CTrNode* > vctNode;
			BuildWordNodeList( pRootRight, vctNode );
			vector< CTrNode* >::iterator i, j = vctNode.end( );
			CTrIndex* pIndex = NULL;
			CTrNode* pTmp = NULL;
			udStlStr szWord, szTmp;
			for( i = vctNode.begin( ); i != j; ++i ) {
				pTmp = *i;
				BuildWord( pTmp, szWord );
				pIndex = pTmp->GetTranslate( );
				while( pIndex ) {
					BuildWord( pIndex->GetTreeNode( ), szTmp );
					this->AddTranslatedWord( szWord, szTmp );
					szTmp.clear( );
					pIndex = pIndex->GetNext( );
				}
				szWord.clear( );
			}
		}
	} // void MergeTree

	/**
	 *
	 */
	void CTrTree::GetSortedTranslate( CTrNode* pNode, udStlStr& szText, udDWord dwShowId ) {
		CTrIndex* pIndex = pNode->GetTranslate( );
		if ( pIndex ) {
			CTrNode* pNode = NULL;
			while( pIndex ) {
				pNode = pIndex->GetTreeNode( );
				if ( pNode ) {
					szText += TAB;
					szText += LT;
					szText += 0x74;

					if ( dwShowId ) {
						this->WriteId( szText, pNode->GetId( ) );
					}

					szText += GT;
					BuildWord( pNode, szText );
					szText += LT;
					szText += 0x2f;
					szText += 0x74;
					szText += GT;
					szText += LF;
				}

				pIndex = pIndex->GetNext( );
			}
		}
	} // void GetSortedTranslate

	void CTrTree::LogStats( ) {
		char *p = NULL;
		g_pLog->Write( "word count: " );
		p = LongToString( m_dwCount );
		g_pLog->Write( p );
		g_pLog->Write( "\n" );
		delete [ ] p;
		p = NULL;
		g_pLog->Write( "node count: " );
		p = LongToString( m_dwNodeCount );
		g_pLog->Write( p );
		g_pLog->Write( "\n" );
		delete [ ] p;
		p = NULL;
	} // void LogStats

	void CTrTree::WriteId( udStlStr& szText, udDWord dwId ) {
		char *p = LongToString( dwId );

		szText += StringStdToOur( " id=\"" );
		szText += StringStdToOur( p );

		delete [ ] p;
		p = NULL;

		szText += StringStdToOur( "\"" );
	} // void WriteId

} // namespace UdSDK

