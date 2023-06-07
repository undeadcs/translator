<?php
	defined( '_JEXEC' ) or die( 'hello world. bye bye.' );
	jimport( 'joomla.application.component.model' );
	
	/**
	 * 	Список
	 */
	class listModelList extends JModel {
		const	DEFAULT_PAGE		= 1,
				DEFAULT_PAGE_SIZE	= 50,
				DEFAULT_PAGE_LEN	= 10;
		
		private	$m_iPage			= 1,
				$m_iPageSize		= 0,
				$m_iPageLen			= 0;
				
		function delword( ) {
			$iWord = JRequest::getVar( 'word', 0, 'default', 'int' );
			if ( $iWord ) {
				$szDir = getcwd( );
				//chdir( JPATH_COMPONENT );
				chdir( '/usr/translator/debug' );
			
				//$objTranslator = new CTranslator( 'Translator.exe', '' );
				$objTranslator = new CTranslator( './translator_exe', JPATH_COMPONENT.DS );
				$bRet = $objTranslator->DelById( $iWord );
				
				chdir( $szDir );
				return $bRet;
			}
			return false;
		} // function delword
		
		function deltranslate( ) {
			$iWord		= JRequest::getVar( 'word', 0, 'default', 'int' );
			$iTranslate	= JRequest::getVar( 'translate', 0, 'default', 'int' );
			if ( $iWord && $iTranslate ) {
				$szDir = getcwd( );
				//chdir( JPATH_COMPONENT );
				chdir( '/usr/translator/debug' );
			
				//$objTranslator = new CTranslator( 'Translator.exe', '' );
				$objTranslator = new CTranslator( './translator_exe', JPATH_COMPONENT.DS );
				$bRet = $objTranslator->DelTranslateById( $iWord, array( $iTranslate ) );
				
				chdir( $szDir );
				return $bRet;
			}
			return false;
		} // function deltranslate
		
		/**
		 * 	Список
		 */
		function getData( ) {
			$arrRet = array( 'list' => NULL, 'pager' => NULL, 'len' => NULL, 'total' => NULL );
			
			$this->m_iPage		= isset( $_GET[ 'page'		] ) ? abs( intval( $_GET[ 'page'		] ) ) : listModelList::DEFAULT_PAGE;
			$this->m_iPageSize	= isset( $_GET[ 'page_len'	] ) ? abs( intval( $_GET[ 'page_len'	] ) ) : listModelList::DEFAULT_PAGE_SIZE;
			$this->m_iPageLen	= isset( $_GET[ 'page_size'	] ) ? abs( intval( $_GET[ 'page_size'	] ) ) : listModelList::DEFAULT_PAGE_LEN;
			
			if ( $this->m_iPage < listModelList::DEFAULT_PAGE ) {
				$this->m_iPage =  listModelList::DEFAULT_PAGE;
			}
			
			if ( $this->m_iPageSize < listModelList::DEFAULT_PAGE_SIZE ) {
				$this->m_iPageSize = listModelList::DEFAULT_PAGE_SIZE;
			}
			
			if ( $this->m_iPageLen < listModelList::DEFAULT_PAGE_LEN ) {
				$this->m_iPageLen = listModelList::DEFAULT_PAGE_LEN;
			}
			
			$szDir = getcwd( );
			//chdir( JPATH_COMPONENT );
			chdir( '/usr/translator/debug' );
			
			//$objTranslator = new CTranslator( 'Translator.exe', '' );
			$objTranslator = new CTranslator( './translator_exe', JPATH_COMPONENT.DS );
			$iTotal	= $objTranslator->WordTotalNumber( );
			$arrRet[ 'list'		] = $objTranslator->GetList( ( ( $this->m_iPage - 1 ) * $this->m_iPageSize ), $this->m_iPageSize );
			$arrRet[ 'pager'	] = GenPagerArray( $this->m_iPage, $iTotal, $this->m_iPageSize, $this->m_iPageLen );
			$arrRet[ 'len'		] = $this->m_iPageLen;
			$arrRet[ 'total'	] = $iTotal;
			
			chdir( $szDir );
			
			return $arrRet;
		} // function getData
		
	} // class listModelList
	
?>