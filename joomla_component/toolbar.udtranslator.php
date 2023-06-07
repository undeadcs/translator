<?php
	defined( '_JEXEC' ) or die( 'hello world. bye bye.' );
	
	require_once( JApplicationHelper::getPath( 'toolbar_html' ) );
	
	$szController = JRequest::getVar( 'controller' );
	switch( $szController ) {
		case 'add': {
			toolbarTranslator::task_add( );
		} break;
		case 'search': {
			toolbarTranslator::task_search( );
		} break;
	}
?>