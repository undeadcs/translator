<?php
	defined( '_JEXEC' ) or die( 'hello world. bye bye.' );
	JFactory::getDocument( )->addStyleSheet( 'components/com_udtranslator/main.css' );
	
?><div class="udtr_wrap"><div class="udtr_list"><table>
	<tr>
		<th class="colId">#</th>
		<th class="colWord">Слово</th>
		<th class="colTranslate">Перевод</th>
		<th class="colAction">Операции</th>
	</tr><?
	
	foreach( $this->list as $v ) {
		$iId = key( $v[ 0 ] );
		
		?><tr>
			<td class="colId"><div><?=$iId?></div></td>
			<td class="colWord"><div><? echo '<a href="?option=com_udtranslator&amp;controller=list&amp;task=delword&amp;word='.$iId.'&amp;time='.time( ).'" onclick="return confirm(\'Вы действительно хотите удалить слово?\');" title="удалить слово">'.current( $v[ 0 ] ).'</a>'; ?></div></td>
			<td class="colTranslate"><div><?
			
			$tmp = array( );
			foreach( $v[ 1 ] as $j => $w ) {
				$tmp[ ] = '<a href="?option=com_udtranslator&amp;controller=list&amp;task=deltranslate&amp;word='.$iId.'&amp;translate='.$j.'&amp;time='.time( ).'" onclick="return confirm(\'Вы действительно хотите удалить перевод?\');" title="удалить этот перевод">'.$w.'</a>';
			}
			echo join( '&nbsp;&nbsp; | &nbsp;&nbsp;', $tmp );
			
			?></div></td>
		</tr><?
	}
	
?></table></div>
<div class="pager"><div class="clear">&nbsp;</div><?

$szUrl = '?option=com_udtranslator&amp;controller=list';

if ( isset( $this->pager[ 'prevFrame' ] ) ) {
	echo '<a class="page_prev" href="?page='.$this->pager[ 'prevFrame' ][ 0 ].'" title="предыдущие '.$this->len.' страниц">&lt;</a>';
}
if ( isset( $this->pager[ 'prev' ] ) ) {
	if ( $this->pager[ 'prev' ] == 0 ) {
		echo '<span class="page_prev">&laquo;</span>';
	} else {
		echo '<a class="page_prev" href="'.$szUrl.'&amp;page='.$this->pager[ 'prev' ].'" title="предыдущая страница">&laquo;</a>';
	}
}
if ( isset( $this->pager[ 'pages' ] ) ) {
	foreach( $this->pager[ 'pages' ] as $v ) {
		if ( $v[ 1 ] ) {
			echo '<span>'.$v[ 0 ].'</span>';
		} else {
			echo '<a href="'.$szUrl.'&amp;page='.$v[ 0 ].'">'.$v[ 0 ].'</a>';
		}
	}
}
if ( isset( $this->pager[ 'next' ] ) ) {
	if ( $this->pager[ 'next' ] == 0 ) {
		echo '<span class="page_next">&raquo;</span>';
	} else {
		echo '<a class="page_next" href="'.$szUrl.'&amp;page='.$this->pager[ 'next' ].'" title="следующая страница">&raquo;</a>';
	}
}
if ( isset( $this->pager[ 'nextFrame' ] ) ) {
	echo '<a class="page_next" href="'.$szUrl.'&amp;page='.$this->pager[ 'nextFrame' ][ 0 ].'" title="следующие '.$this->len.' страниц">&gt;</a>';
}

?><div class="clear">&nbsp;</div></div></div><p>Всего слов:&nbsp;<?=$this->total?></p>