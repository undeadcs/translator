<?php
	defined( '_JEXEC' ) or die( 'hello world. bye bye.' );
	jimport( 'joomla.application.component.model' );
	
	/**
	 * 	Добавление
	 */
	class addModelAdd extends JModel {
		
		function add( ) {
			$szOriginal		= JRequest::getVar( 'original',		'' );
			$szTranslate	= JRequest::getVar( 'translate',	'' );
			if ( empty( $szOriginal ) ) {
				return false;
			} else {
				$szDir = getcwd( );
				//chdir( JPATH_COMPONENT );
				chdir( '/usr/translator/debug' );
				
				//$objTranslator = new CTranslator( 'Translator.exe', '' );
				$objTranslator = new CTranslator( './translator_exe', JPATH_COMPONENT.DS );
				$bRet = $objTranslator->AddTranslatedWord( $szOriginal, $szTranslate );
				
				chdir( $szDir );
				return $bRet;
			}
			return false;
		} // function act
		
	} // class addModelAdd
	
?>