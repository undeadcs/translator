<?php
	defined( '_JEXEC' ) or die( 'hello world. bye bye.' );
	
	class toolbarTranslator {
		
		static function task_add( ) {
			JToolBarHelper::save( 'add', 'Добавить' );
		} // function task_add
		
		static function task_search( ) {
			JToolBarHelper::apply( 'find', 'Найти' );
		} // function task_search
		
	}
?>