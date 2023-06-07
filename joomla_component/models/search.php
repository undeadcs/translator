<?php
	defined( '_JEXEC' ) or die( 'hello world. bye bye.' );
	jimport( 'joomla.application.component.model' );
	
	/**
	 * 	Поиск
	 */
	class searchModelSearch extends JModel {
		
		function find( ) {
			$szWord = JRequest::getVar( 'word', '' );
			if ( empty( $szWord ) ) {
				return false;
			}
			
			$szDir = getcwd( );
			//chdir( JPATH_COMPONENT );
			chdir( '/usr/translator/debug' );
		
			//$objTranslator = new CTranslator( 'Translator.exe', '' );
			$objTranslator = new CTranslator( './translator_exe', JPATH_COMPONENT.DS );
			$tmp = NULL;
			$bRet = $objTranslator->Translate( $szWord, $tmp );
			if ( $bRet ) {
				ob_start( );
				echo '<h4>'.$szWord.'</h4>';
				if ( is_string( $tmp ) ) {
					echo '<p>'.$tmp.'</p>';
				} elseif ( is_array( $tmp ) ) {
					echo '<ol>';
					foreach( $tmp as $v ) {
						echo '<li>'.$v.'</li>';
					}
					echo '</ol>';
				}
				$bRet = ob_get_clean( );
			}
			
			chdir( $szDir );
			
			return $bRet;
		} // function find
		
	} // class searchModelSearch
	
?>