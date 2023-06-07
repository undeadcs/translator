<?php
	// обработка текстов
	
	$tmpRu = file_get_contents( 'text1_ru' );
	$tmpJp = file_get_contents( 'text1_jp' );
	
	$tmpRu = explode( '.', $tmpRu );
	$tmpJp = explode( '。', $tmpJp );
	
	foreach( $tmpRu as $i => $v ) {
		$tmpRu[ $i ] = preg_replace( '/\s+$|^\s+/', '', $v );
	}
	foreach( $tmpJp as $i => $v ) {
		$tmpJp[ $i ] = preg_replace( '/\s+$|^\s+/', '', $v );
	}
	
	ShowVar( $tmpRu, $tmpJp );
?>