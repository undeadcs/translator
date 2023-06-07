
#include "includes.h"
#include "externals.h"

namespace UdSDK {

	CTrNodeInfo::CTrNodeInfo( ) {
		m_dwId				= 0;
		m_dwHasWord			= 0;
		m_dwCode			= 0;
		m_dwChildNum		= 0;
		m_dwTranslateNum	= 0;
		m_pChildren			= NULL;
		m_pTranslate		= NULL;
	}

	CTrNodeInfo::CTrNodeInfo( udDWord dwId, udDWord dwHasWord, udDWord dwCode, udDWord dwChildNum, udDWord dwTranslateNum ) {
		m_dwId				= dwId;
		m_dwHasWord			= dwHasWord;
		m_dwCode			= dwCode;
		m_dwChildNum		= dwChildNum;
		m_dwTranslateNum	= dwTranslateNum;
		m_pChildren			= NULL;
		m_pTranslate		= NULL;

		if ( m_dwChildNum ) {
			m_pChildren = new udDWord[ m_dwChildNum	];
		}

		if ( m_dwTranslateNum ) {
			m_pTranslate = new udDWord[ m_dwTranslateNum ];
		}
	}

	CTrNodeInfo::CTrNodeInfo( const CTrNodeInfo& objInfo ) {
	}

	CTrNodeInfo::~CTrNodeInfo( ) {
		if ( m_pChildren ) {
			delete [ ] m_pChildren;
		}
		if ( m_pTranslate ) {
			delete [ ] m_pTranslate;
		}

		m_dwId				= 0;
		m_dwHasWord			= 0;
		m_dwCode			= 0;
		m_dwChildNum		= 0;
		m_dwTranslateNum	= 0;
		m_pChildren			= NULL;
		m_pTranslate		= NULL;
	}

	void CTrNodeInfo::Create( udDWord dwId, udDWord dwHasWord, udDWord dwCode, udDWord dwChildNum, udDWord dwTranslateNum ) {
		m_dwId				= dwId;
		m_dwHasWord			= dwHasWord;
		m_dwCode			= dwCode;
		m_dwChildNum		= dwChildNum;
		m_dwTranslateNum	= dwTranslateNum;

		if ( m_pChildren ) {
			delete [ ] m_pChildren;
			m_pChildren = NULL;
		}
		if ( m_pTranslate ) {
			delete [ ] m_pTranslate;
			m_pTranslate = NULL;
		}

		if ( m_dwChildNum ) {
			m_pChildren = new udDWord[ m_dwChildNum	];
		}

		if ( m_dwTranslateNum ) {
			m_pTranslate = new udDWord[ m_dwTranslateNum ];
		}
	} // void Create

	void CTrNodeInfo::Clear( ) {
		if ( m_pChildren ) {
			delete [ ] m_pChildren;
		}
		if ( m_pTranslate ) {
			delete [ ] m_pTranslate;
		}

		m_dwId				= 0;
		m_dwHasWord			= 0;
		m_dwCode			= 0;
		m_dwChildNum		= 0;
		m_dwTranslateNum	= 0;
		m_pChildren			= NULL;
		m_pTranslate		= NULL;
	} // void Clear

	udPNode CTrNodeInfo::GetTreeNode( ) {
		return new CTrNode( m_dwHasWord, m_dwCode, m_dwId );
	} // udPNode GetTreeNode

	udDWord CTrNodeInfo::GetId( ) {
		return m_dwId;
	} // udDWord GetId

	udDWord CTrNodeInfo::GetChildNum( ) {
		return m_dwChildNum;
	} // udDWord GetChildNum

	udDWord CTrNodeInfo::GetTranslateNum( ) {
		return m_dwTranslateNum;
	} // udDWord GetTranslateNum

	udPDWord CTrNodeInfo::GetChildren( ) {
		return m_pChildren;
	} // udPDWord GetChildren

	udPDWord CTrNodeInfo::GetTranslate( ) {
		return m_pTranslate;
	} // udPDWord GetTranslate

	void CTrNodeInfo::SetChild( udDWord dwPos, udDWord dwValue ) {
		if ( dwPos < m_dwChildNum ) {
			m_pChildren[ dwPos ] = dwValue;
		}
	} // void SetChild

	void CTrNodeInfo::SetTranslate( udDWord dwPos, udDWord dwValue ) {
		if ( dwPos < m_dwTranslateNum ) {
			m_pTranslate[ dwPos ] = dwValue;
		}
	} // void SetTranslate

	void CTrNodeInfo::SetChildNum( udDWord dwChildNum ) {
		if ( m_pChildren ) {
			delete [ ] m_pChildren;
			m_pChildren = NULL;
		}

		m_dwChildNum	= dwChildNum;

		if ( m_dwChildNum ) {
			m_pChildren = new udDWord[ m_dwChildNum ];
		}
	} // void SetChildNum

	void CTrNodeInfo::SetTranslateNum( udDWord dwTranslateNum ) {
		if ( m_pTranslate ) {
			delete [ ] m_pTranslate;
			m_pTranslate = NULL;
		}

		m_dwTranslateNum	= dwTranslateNum;

		if ( m_dwTranslateNum ) {
			m_pTranslate = new udDWord[ m_dwTranslateNum ];
		}
	} // void SetTranslateNum

} // namespace UdSDK

