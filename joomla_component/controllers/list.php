<?php
	defined( '_JEXEC' ) or die( 'hello world. bye bye.' );
	jimport( 'joomla.application.component.controller' );
	
	/**
	 * 	Список
	 */
	class listController extends JController {
		
		function delword( ) {
			$objModel = $this->getModel( );
			if ( $objModel->delword( ) ) {
				$szMsg = JText::_( 'Слово удалено' );
			} else {
				$szMsg = JText::_( 'Ошибка: не удалось удалить слово. Возможно переводчик занят.' );
			}
			$this->setRedirect( 'index.php?option=com_udtranslator&controller=list', $szMsg );
		} // function delword
		
		function deltranslate( ) {
			$objModel = $this->getModel( );
			if ( $objModel->deltranslate( ) ) {
				$szMsg = JText::_( 'Перевод удален' );
			} else {
				$szMsg = JText::_( 'Ошибка: не удалось удалить перевод. Возможно переводчик занят.' );
			}
			$this->setRedirect( 'index.php?option=com_udtranslator&controller=list', $szMsg );
		} // function deltranslate
		
	} // class listController
	
?>