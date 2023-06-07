<?php
	/**
	 * 	Класс добавления элементов перевода
	 */
	class CTrAdd {
		
		/**
		 * 	Конструктор
		 */
		public function __construct( ) {
		} // function __construct
		
		/**
		 * 	Обработка формы
		 * 	@param $objTranslator CTranslator класс работы с переводчиком через API
		 * 	@return mixed
		 */
		public function ProcForm( $objTranslator ) {
			if ( count( $_POST ) ) {
				$szOriginal		= ( isset( $_POST[ 'original'	] ) && is_string( $_POST[ 'original'	] ) ) ? $_POST[ 'original'	] : '';
				$szTranslate	= ( isset( $_POST[ 'translate'	] ) && is_string( $_POST[ 'translate'	] ) ) ? $_POST[ 'translate'	] : '';
				if ( empty( $szOriginal ) ) {
					return false;
				} else {
					$tmp = $objTranslator->AddTranslatedWord( $szOriginal, $szTranslate );
					if ( $tmp == false ) {
						echo 'переводчик занят, подождите и потом нажмите F5. авось освободится.';
						exit;
					}
					return true;
				}
			}
			return false;
		} // function ProcForm
		
		/**
		 * 	Форма добавления
		 */
		public function GetForm( ) {
			ob_start( );
			
			?><div class="tr_add"><form action="./?mode=add" method="post"><table>
				<tr><td>Оригинал</td><td><textarea name="original"></textarea></td></tr>
				<tr><td>Перевод</td><td><textarea name="translate"></textarea></td></tr>
				<tr><td>&nbsp;</td><td><input type="submit" value="Добавить"/></td></tr>
			</table></form></div><?
			
			$r = ob_get_clean( );
			if ( $r === false ) {
				$r = '';
			}
			return $r;
		} // function GetForm
		
	} // class CTrAdd
?>