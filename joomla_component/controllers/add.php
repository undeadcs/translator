<?php
	defined( '_JEXEC' ) or die( 'hello world. bye bye.' );
	jimport( 'joomla.application.component.controller' );
	
	/**
	 * 	Добавление
	 */
	class addController extends JController {
		
		function add( ) {
			$szController = 'add';
			$objModel = $this->getModel( );
			if ( $objModel->add( ) ) {
				$szController = 'list';
				$szMsg = JText::_( 'Слово добавлено' );
			} else {
				$szMsg = JText::_( 'Ошибка: не удалось добавить слово. Возможно переводчик занят.' );
			}
			$this->setRedirect( 'index.php?option=com_udtranslator&controller='.$szController, $szMsg );
		} // function act
		
		function display( ) {
			parent::display( );
		}
		
	} // class addController
	
?>