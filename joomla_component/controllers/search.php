<?php
	defined( '_JEXEC' ) or die( 'hello world. bye bye.' );
	jimport( 'joomla.application.component.controller' );
	
	/**
	 * 	Поиск
	 */
	class searchController extends JController {
		
		function find( ) {
			$objModel = $this->getModel( );
			$tmp = $objModel->find( );
			if ( $tmp === false ) {
			} else {
				require_once( 'components/com_udtranslator/views/search/view.html.php' );
				$objView = $this->getView( );
				$objView->assignRef( 'result', $tmp );
			}
			parent::display( );
		} // function find
		
		function display( ) {
			parent::display( );
		}
		
	} // class searchController
	
?>