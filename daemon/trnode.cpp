
#include "includes.h"
#include "externals.h"

namespace UdSDK {

	CTrNode::CTrNode( udDWord dwHasWord, udDWord dwCode, CTrNode* pParent ) {
		m_dwHasWord		= dwHasWord;
		m_dwCode		= dwCode;
		m_dwId			= 0;
		m_dwVisited		= 0;
		m_pParent		= pParent;
		m_pChild		= NULL;
		m_pNextSibling	= NULL;
		m_pPrevSibling	= NULL;
		m_pTranslate	= NULL;
	}

	CTrNode::CTrNode( udDWord dwHasWord, udDWord dwCode, udDWord dwId, CTrNode* pParent ) {
		m_dwHasWord		= dwHasWord;
		m_dwCode		= dwCode;
		m_dwId			= dwId;
		m_dwVisited		= 0;
		m_pParent		= pParent;
		m_pChild		= NULL;
		m_pNextSibling	= NULL;
		m_pPrevSibling	= NULL;
		m_pTranslate	= NULL;
	}

	CTrNode::CTrNode( const CTrNode& objNode ) {
		m_dwHasWord		= objNode.m_dwHasWord;
		m_dwCode		= objNode.m_dwCode;
		m_dwId			= objNode.m_dwId;
		m_dwVisited		= objNode.m_dwVisited;
		m_pParent		= objNode.m_pParent;
		m_pChild		= objNode.m_pChild;
		m_pNextSibling	= objNode.m_pNextSibling;
		m_pPrevSibling	= objNode.m_pPrevSibling;
		m_pTranslate	= objNode.m_pTranslate;
	}

	CTrNode::~CTrNode( ) {
		m_dwHasWord		= 0;
		m_dwCode		= 0;
		m_dwId			= 0;
		m_dwVisited		= 0;
		m_pParent		= NULL;
		m_pChild		= NULL;
		m_pNextSibling	= NULL;
		m_pPrevSibling	= NULL;

		this->ClearTranslate( );

		m_pTranslate = NULL;
	}

	void CTrNode::SetParent( CTrNode* pParent ) {
		m_pParent = pParent;
	} // void SetParent

	void CTrNode::SetHasWord( udDWord dwHasWord ) {
		m_dwHasWord = dwHasWord;
	} // void SetHasWord

	CTrNode* CTrNode::AddChild( udDWord dwCode, udDWord dwHasWord ) {
		if ( m_pChild == NULL ) {
			m_pChild = new CTrNode( dwHasWord, dwCode, this );
			return m_pChild;
		} else {
			CTrNode* pNode = m_pChild;
			CTrNode* pTmp = NULL;
			udDWord dwNodeCode = pNode->GetCode( );
			if ( dwCode != dwNodeCode ) {
				if ( dwCode > dwNodeCode ) {
					pTmp = pNode;
					pNode = pNode->GetNextSibling( );
					while( true ) {
						if ( pNode == NULL ) {
							// добавляем новый узел в конец списка
							CTrNode* pNew = new CTrNode( dwHasWord, dwCode, this );
							pNew->SetPrevSibling( pTmp );
							pTmp->SetNextSibling( pNew );
							return pNew;
						}
						dwNodeCode = pNode->GetCode( );
						if ( dwCode < dwNodeCode ) {
							// добавляем новый узел между двумя другими элементами списка
							CTrNode* pNew = new CTrNode( dwHasWord, dwCode, this );
							pNew->SetPrevSibling( pTmp );
							pNew->SetNextSibling( pNode );
							pNode->SetPrevSibling( pNew );
							pTmp->SetNextSibling( pNew );
							return pNew;
						} else if ( dwCode == dwNodeCode ) {
							break;
						} else {
							pTmp = pNode;
							pNode = pNode->GetNextSibling( );
						}
					}
				} else {
					pTmp = pNode;
					pNode = pNode->GetPrevSibling( );
					CTrNode* pNew = new CTrNode( dwHasWord, dwCode, this );
					pNew->SetNextSibling( pTmp );
					pTmp->SetPrevSibling( pNew );
					this->m_pChild = pNew;
					return pNew;
				}
			}
			pNode = NULL;
		}
		return NULL;
	} // void AddChild

	CTrNode* CTrNode::GetChildren( ) {
		return m_pChild;
	} // trArrNode GetChildren

	CTrNode* CTrNode::GetParent( ) {
		return m_pParent;
	} // CTrNode* GeParent

	CTrNode* CTrNode::GetChild( udDWord dwCode ) {
		if ( m_pChild ) {
			CTrNode* pNode = m_pChild;
			udDWord dwNodeCode = pNode->GetCode( );
			if ( dwCode == dwNodeCode ) {
				return pNode;
			} else if ( dwCode > dwNodeCode ) {
				pNode = pNode->GetNextSibling( );
				while( true ) {
					if ( pNode == NULL ) {
						break;
					}
					dwNodeCode = pNode->GetCode( );
					if ( dwCode < dwNodeCode ) {
						break;
					} else if ( dwCode == dwNodeCode ) {
						return pNode;
					} else {
						pNode = pNode->GetNextSibling( );
					}
				}
			} else if ( dwCode < dwNodeCode ) {
				pNode = pNode->GetPrevSibling( );
				while( true ) {
					if ( pNode == NULL ) {
						break;
					}
					dwNodeCode = pNode->GetCode( );
					if ( dwCode > dwNodeCode ) {
						break;
					} else if ( dwCode == dwNodeCode ) {
						return pNode;
					} else {
						pNode = pNode->GetPrevSibling( );
					}
				}
			}
			pNode = NULL;
		}
		return NULL;
	} // CTrNode* GetChild

	void CTrNode::SetNextSibling( CTrNode* pNode ) {
		m_pNextSibling = pNode;
	} // void SetNextSibling

	void CTrNode::SetPrevSibling( CTrNode* pNode ) {
		m_pPrevSibling = pNode;
	} // void SetPrevSibling

	CTrNode* CTrNode::GetNextSibling( ) {
		return m_pNextSibling;
	} // CTrNode* GetNextSibling

	CTrNode* CTrNode::GetPrevSibling( ) {
		return m_pPrevSibling;
	} // CTrNode* GetPrevSibling

	void CTrNode::SetCode( udDWord dwCode ) {
		m_dwCode = dwCode;
	} // void SetIndex

	void CTrNode::SetVisited( udDWord dwVisited ) {
		m_dwVisited = dwVisited;
	} // void SetVisited

	void CTrNode::AddTranslate( CTrNode* pNode ) {
		if ( m_pTranslate ) {
			if ( m_pTranslate->GetTreeNode( ) == pNode ) {
				return;
			}
			CTrIndex *pIndex = m_pTranslate, *pTmp = pIndex;
			pIndex = pIndex->GetNext( );
			while( pIndex ) {
				if ( pIndex->GetTreeNode( ) == pNode ) {
					return;
				}
				pTmp = pIndex;
				pIndex = pIndex->GetNext( );
			}
			pIndex = new CTrIndex( pNode, pTmp );
			pTmp->SetNext( pIndex );
			pIndex = NULL;
			pTmp = NULL;
		} else {
			m_pTranslate = new CTrIndex( pNode );
		}
	} // void AddTranslate

	void CTrNode::SetId( udDWord dwId ) {
		m_dwId = dwId;
	} // void SetId

	void CTrNode::PushChild( CTrNode* pNode ) {
		if ( pNode ) {
			pNode->SetParent( this );
			if ( m_pChild ) {
				CTrNode *pPrev = m_pChild, *pNow = m_pChild->GetNextSibling( );
				while( pNow ) {
					pPrev	= pNow;
					pNow	= pNow->GetNextSibling( );
				}

				pPrev->SetNextSibling( pNode );
				pNode->SetPrevSibling( pPrev );

			} else {
				m_pChild = pNode;
			}
		}
	} // void PushChild

	void CTrNode::PushTranslate( CTrNode* pNode ) {
		if ( m_pTranslate ) {
			CTrIndex *pPrev = m_pTranslate, *pNow = m_pTranslate->GetNext( );
			while( pNow ) {
				pPrev = pNow;
				pNow = pNow->GetNext( );
			}
			pNow = new CTrIndex( pNode, pPrev );
			pPrev->SetNext( pNow );
		} else {
			m_pTranslate = new CTrIndex( pNode );
		}
	} // void PushTranslate

	void CTrNode::DelTranslate( CTrNode* pNode ) {
		if ( m_pTranslate ) {
			CTrNode* pTmp = NULL;
			CTrIndex *pIndex = m_pTranslate, *pPrev = NULL, *pNext = NULL;
			while( pIndex ) {
				pTmp = pIndex->GetTreeNode( );
				if ( pTmp == pNode ) {
					//g_pLog->Write( "deleting node\n" );
					pPrev = pIndex->GetPrev( );
					pNext = pIndex->GetNext( );
					if ( pPrev ) {
						if ( pNext ) {
							// удаляется узел в середине списка
							pPrev->SetNext( pNext );
							pNext->SetPrev( pPrev );
							delete pIndex;
							pIndex = pPrev;
						} else {
							// удаляется узел с конца списка
							pPrev->SetNext( );
							delete pIndex;
							break;
						}
					} else {
						if ( pNext ) {
							// удаляется узел из начала списка
							pNext->SetPrev( );
							delete pIndex;
							pIndex = pNext;
							m_pTranslate = pNext;
							continue;
						} else {
							// единственный узел перевода
							delete pIndex;
							m_pTranslate = NULL;
							break;
						}
					}
				}
				pIndex = pIndex->GetNext( );
			}
		}
	} // void DelTranslate

	void CTrNode::DelTranslate( udDWord dwId ) {
		CTrNode* pTmp = NULL;
		CTrIndex *pIndex = m_pTranslate, *pPrev = NULL, *pNext = NULL;
		while( 1 ) {
			if ( !pIndex ) {
				break;
			}
			pTmp = pIndex->GetTreeNode( );
			if ( pTmp->GetId( ) == dwId ) {
				pPrev = pIndex->GetPrev( );
				pNext = pIndex->GetNext( );
				if ( pPrev ) {
					if ( pNext ) {
						// удаляется узел в середине списка
						pPrev->SetNext( pNext );
						pNext->SetPrev( pPrev );
						delete pIndex;
						pIndex = pPrev;
					} else {
						// удаляется узел с конца списка
						pPrev->SetNext( );
						delete pIndex;
						break;
					}
				} else {
					if ( pNext ) {
						// удаляется узел из начала списка
						pNext->SetPrev( );
						delete pIndex;
						pIndex = pNext;
						m_pTranslate = pNext;
						continue;
					} else {
						// единственный узел перевода
						delete pIndex;
						m_pTranslate = NULL;
						break;
					}
				}
			}
			pIndex = pIndex->GetNext( );
		}
	} // udDWord DelTranslate

	void CTrNode::ClearTranslate( ) {
		if ( m_pTranslate ) {
			CTrIndex* pIndex = m_pTranslate, *pTmp = NULL;
			while( pIndex ) {
				pTmp = pIndex;
				pIndex = pIndex->GetNext( );

				delete pTmp;
				pTmp = NULL;
			}
			m_pTranslate = NULL;
		}
	} // void ClearTranslate

	void CTrNode::SetTranslate( CTrIndex* pIndex ) {
		m_pTranslate = pIndex;
	} // void SetTranslate

	udDWord CTrNode::GetHasWord( ) const {
		return m_dwHasWord;
	} // udDWord GetHasWord

	udDWord CTrNode::GetCode( ) const {
		return m_dwCode;
	} // udDWord GetIndex

	udDWord CTrNode::GetVisited( ) const {
		return m_dwVisited;
	} // udDWord GetVisited

	udDWord CTrNode::GetId( ) const {
		return m_dwId;
	} // udDWord GetId

	CTrIndex* CTrNode::GetTranslate( ) {
		return m_pTranslate;
	} // CTrIndex* GetTranslate

} // namespace UdSDK

