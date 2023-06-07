<?php
	defined( '_JEXEC' ) or die( 'hello world. bye bye.' );
	
	require( 'lib.php'			);
	require( 'trapi.php'		);
	require( 'controller.php'	);
	
	$szController = JRequest::getVar( 'controller' );
	if ( empty( $szController ) ) {
		$szController = 'list';
	}
	$szPath = JPATH_COMPONENT.DS.'controllers'.DS.$szController.'.php';
	if ( file_exists( $szPath ) ) {
		require( $szPath );
	}
	
	$szClass = $szController.'Controller';
	$objController = new $szClass( );
	$objController->execute( JRequest::getVar( 'task' ) );
	$objController->redirect( );
?>