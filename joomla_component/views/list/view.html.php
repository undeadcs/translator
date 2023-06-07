<?php
	defined( '_JEXEC' ) or die( 'hello world. bye bye.' );
	jimport( 'joomla.application.component.view');
	
	/**
	 * 	Список
	 */
	class listViewList extends JView {
		
		function display( $tpl = NULL ) {
			$objModel = $this->getModel( );
			$arrData = $objModel->getData( );
			$this->assignRef( 'list',	$arrData[ 'list'	] );
			$this->assignRef( 'pager',	$arrData[ 'pager'	] );
			$this->assignRef( 'len',	$arrData[ 'len'		] );
			$this->assignRef( 'total',	$arrData[ 'total'	] );
			parent::display( $tpl );
		} // function display
		
	} // class listViewList
	
?>