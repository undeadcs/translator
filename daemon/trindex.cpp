
#include "includes.h"
#include "externals.h"

namespace UdSDK {

	CTrIndex::CTrIndex( CTrNode* pNode, CTrIndex* pPrev, CTrIndex* pNext ) {
		m_pNode	= pNode;
		m_pPrev	= pPrev;
		m_pNext	= pNext;
	}

	CTrIndex::CTrIndex( const CTrIndex& objIndex ) {
		m_pNode = objIndex.m_pNode;
		m_pPrev = objIndex.m_pPrev;
		m_pNext = objIndex.m_pNext;
	}

	CTrIndex::~CTrIndex( ) {
		m_pNode	= NULL;
		m_pPrev	= NULL;
		m_pNext	= NULL;
	}

	void CTrIndex::SetNode( CTrNode* pNode ) {
		m_pNode = pNode;
	} // void SetNode

	void CTrIndex::SetPrev( CTrIndex* pPrev ) {
		m_pPrev = pPrev;
	} // void SetPrev

	void CTrIndex::SetNext( CTrIndex* pNext ) {
		m_pNext = pNext;
	} // void SetNext

	CTrNode* CTrIndex::GetTreeNode( ) {
		return m_pNode;
	} // CTrNode* GetTreeNode

	CTrIndex* CTrIndex::GetPrev( ) {
		return m_pPrev;
	} // CTrNode* GetPrev

	CTrIndex* CTrIndex::GetNext( ) {
		return m_pNext;
	} // CTrNode* GetNext

}
