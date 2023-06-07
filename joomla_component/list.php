<?php

	/**
	 * 	Список
	 */
	class CTrList {
		const	DEFAULT_PAGE		= 1,
				DEFAULT_PAGE_SIZE	= 50,
				DEFAULT_PAGE_LEN	= 10;
		
		private	$m_iPage			= 1,
				$m_iPageSize		= 0,
				$m_iPageLen			= 0;
				
		/**
		 * 	Конструктор
		 */
		public function __construct( ) {
			$this->m_iPage		= isset( $_GET[ 'page'		] ) ? abs( intval( $_GET[ 'page'		] ) ) : CTrList::DEFAULT_PAGE;
			$this->m_iPageSize	= isset( $_GET[ 'page_len'	] ) ? abs( intval( $_GET[ 'page_len'	] ) ) : CTrList::DEFAULT_PAGE_SIZE;
			$this->m_iPageLen	= isset( $_GET[ 'page_size'	] ) ? abs( intval( $_GET[ 'page_size'	] ) ) : CTrList::DEFAULT_PAGE_LEN;
			
			if ( $this->m_iPage < CTrList::DEFAULT_PAGE ) {
				$this->m_iPage =  CTrList::DEFAULT_PAGE;
			}
			
			if ( $this->m_iPageSize < CTrList::DEFAULT_PAGE_SIZE ) {
				$this->m_iPageSize = CTrList::DEFAULT_PAGE_SIZE;
			}
			
			if ( $this->m_iPageLen < CTrList::DEFAULT_PAGE_LEN ) {
				$this->m_iPageLen = CTrList::DEFAULT_PAGE_LEN;
			}
		} // function __construct
		
		/**
		 * 	Установка номера текущей страницы
		 */
		public function SetPage( $iPage ) {
			if ( !is_scalar( $iPage ) ) {
				return;
			}
			
			$this->m_iPage = abs( intval( $iPage ) );
			
			if ( $this->m_iPage < CTrList::DEFAULT_PAGE ) {
				$this->m_iPage =  CTrList::DEFAULT_PAGE;
			}
		} // function SetPage
		
		/**
		 * 	Установка размера страницы
		 */
		public function SetPageSize( $iPageSize ) {
			if ( !is_scalar( $iPageSize ) ) {
				return;
			}
			
			$this->m_iPageSize = abs( intval( $iPageSize ) );
			
			if ( $this->m_iPageSize < CTrList::DEFAULT_PAGE_SIZE ) {
				$this->m_iPageSize = CTrList::DEFAULT_PAGE_SIZE;
			}
		} // function SetPageSize
		
		/**
		 * 	Установка количества ссылок на страницы
		 */
		public function SetPageLen( $iPageLen ) {
			if ( !is_scalar( $iPageLen ) ) {
				return;
			}
			
			$this->m_iPageLen = abs( intval( $iPageLen ) );
			
			if ( $this->m_iPageLen < CTrList::DEFAULT_PAGE_LEN ) {
				$this->m_iPageLen = CTrList::DEFAULT_PAGE_LEN;
			}
		} // function SetPageLen
		
		/**
		 * 	Обработка формы
		 */
		public function ProcForm( $objTranslator ) {
			return false;
		} // function ProcForm
		
		/**
		 * 	Получение списка в виде HTML
		 * 	@param $objTranslator CTranslator класс переводчика
		 * 	@return string
		 */
		public function GetData( $objTranslator ) {
			$iTotal		= $objTranslator->WordTotalNumber( );
			if ( !$iTotal ) {
				return '';
			}
			
			$arrList	= $objTranslator->GetList( ( ( $this->m_iPage - 1 ) * $this->m_iPageSize ), $this->m_iPageSize );
			
			ob_start( );
			
			?><div class="tr_list"><table>
				<tr><th class="col1"><div>Слово</div></th><th class="col2"><div>Список переводов</div></th><th class="col3"><div>Операции</div></th></tr><?
			
			foreach( $arrList as $v ) {
				$iId = key( $v[ 0 ] );
				
				echo '<tr><td class="col1"><div>';
				$tmp1 = array( );
				foreach( $v[ 0 ] as $i => $w ) {
					$tmp1[ $i ] = $w;
				}
				echo join( '&nbsp;&nbsp; | &nbsp;&nbsp;', $tmp1 );
				echo '</div></td><td class="col2"><div>';
				$tmp1 = array( );
				foreach( $v[ 1 ] as $i => $w ) {
					//$tmp1[ $i ] = '<a href="?mode=deltranslate&amp;word='.urlencode( $v[ 0 ][ 0 ] ).'&amp;translate='.urlencode( $w ).'&amp;'.time( ).'" onclick="return confirm(\'Вы действительно хотите удалить перевод?\');" title="удалить этот перевод">'.$w.'</a>';
					$tmp1[ $i ] = '<a href="?mode=deltranslate&amp;word='.$iId.'&amp;translate='.$i.'&amp;'.time( ).'" onclick="return confirm(\'Вы действительно хотите удалить перевод?\');" title="удалить этот перевод">'.$w.'</a>';
				}
				echo join( '&nbsp;&nbsp; | &nbsp;&nbsp;', $tmp1 );
				echo '</div></td><td class="col3"><div>';
				//echo '<a href="?mode=delword&amp;word='.urlencode( $v[ 0 ][ 0 ] ).'&amp;'.time( ).'" onclick="return confirm(\'Вы действительно хотите удалить слово?\');">Удалить слово</a>';
				echo '<a href="?mode=delword&amp;word='.$iId.'&amp;'.time( ).'" onclick="return confirm(\'Вы действительно хотите удалить слово?\');">Удалить слово</a>';
				echo '</div></td></tr>';
			}
			
			?></table></div><?
			
			$arrPager = GenPagerArray( $this->m_iPage, $iTotal, $this->m_iPageSize, $this->m_iPageLen );
	
			echo '<div class="pager"><div class="clear">&nbsp;</div>';
			if ( isset( $arrPager[ 'prevFrame' ] ) ) {
				echo '<a class="page_prev" href="?page='.$arrPager[ 'prevFrame' ][ 0 ].'" title="предыдущие '.$this->m_iPageLen.' страниц">&lt;</a>';
			}
			if ( isset( $arrPager[ 'prev' ] ) ) {
				if ( $arrPager[ 'prev' ] == 0 ) {
					echo '<span class="page_prev">&laquo;</span>';
				} else {
					echo '<a class="page_prev" href="?page='.$arrPager[ 'prev' ].'" title="предыдущая страница">&laquo;</a>';
				}
			}
			if ( isset( $arrPager[ 'pages' ] ) ) {
				foreach( $arrPager[ 'pages' ] as $v ) {
					if ( $v[ 1 ] ) {
						echo '<span>'.$v[ 0 ].'</span>';
					} else {
						echo '<a href="?page='.$v[ 0 ].'">'.$v[ 0 ].'</a>';
					}
				}
			}
			if ( isset( $arrPager[ 'next' ] ) ) {
				if ( $arrPager[ 'next' ] == 0 ) {
					echo '<span class="page_next">&raquo;</span>';
				} else {
					echo '<a class="page_next" href="?page='.$arrPager[ 'next' ].'" title="следующая страница">&raquo;</a>';
				}
			}
			if ( isset( $arrPager[ 'nextFrame' ] ) ) {
				echo '<a class="page_next" href="?page='.$arrPager[ 'nextFrame' ][ 0 ].'" title="следующие '.$this->m_iPageLen.' страниц">&gt;</a>';
			}
			echo '<div class="clear">&nbsp;</div></div>';
			
			echo '<p>Всего слов: '.$iTotal.'</p>';
			
			$r = ob_get_clean( );
			if ( $r === false ) {
				$r = '';
			}
			return $r;
		} // function GetData
		
	} // class CTrList

?>