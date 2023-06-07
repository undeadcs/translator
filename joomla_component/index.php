<?php
	session_start( );
	header( 'Content-Type: text/html; charset=UTF-8', true, 200 );

	require( 'lib.php'		);
	require( 'trapi.php'	);
	
	SwitchOn( );
	
	$time = microtime( );
	$time = explode( ' ', $time );
	$time = array_map( 'floatval', $time );
	$time = $time[ 0 ] + $time[ 1 ];

	$szTitle	= 'Translator';
	$szMode		= ( isset( $_GET[ 'mode' ] ) && is_string( $_GET[ 'mode' ] ) ) ? $_GET[ 'mode' ] : '';
	
	$objTranslator = new CTranslator( 'Translator.exe', '' );
	//$objTranslator->SetHost( '192.168.0.2' );
	
	ob_start( );
	
	require( 'menu.php' );
	
	switch( $szMode ) {
		case 'add': {
			$szTitle .= ' / Добавление';
			
			require( 'add.php' );
			
			$objAdd = new CTrAdd( );
			
			$tmp = $objAdd->ProcForm( $objTranslator );
			if ( $tmp == false ) {
				echo $objAdd->GetForm( );
			} else {
				Redirect( '.' );
			}
		} break;
		
		case 'delword': {
			$szWord = ( isset( $_GET[ 'word' ] ) && is_string( $_GET[ 'word' ] ) ) ? urldecode( $_GET[ 'word' ] ) : '';
			if ( !empty( $szWord ) ) {
				if ( $objTranslator->DelById( intval( $szWord ) ) == false ) {
					echo 'переводчик занят, подождите и потом нажмите F5. авось освободится.';
					exit;
				}
				/*if ( $objTranslator->DelWord( $szWord ) == false ) {
					echo 'переводчик занят, подождите и потом нажмите F5. авось освободится.';
					exit;
				}*/
			}
			Redirect( '.' );
		} break;
		
		case 'deltranslate': {
			$szWord			= ( isset( $_GET[ 'word'		] ) && is_string( $_GET[ 'word'			] ) ) ? urldecode( $_GET[ 'word'		] ) : '';
			$szTranslate	= ( isset( $_GET[ 'translate'	] ) && is_string( $_GET[ 'translate'	] ) ) ? urldecode( $_GET[ 'translate'	] ) : '';
			if ( !empty( $szWord ) && !empty( $szTranslate ) ) {
				if ( $objTranslator->DelTranslateById( intval( $szWord ), array( intval( $szTranslate ) ) ) == false ) {
					echo 'переводчик занят, подождите и потом нажмите F5. авось освободится.';
					exit;
				}
				/*$tmp = $objTranslator->DelTargetTranslate( $szWord, $szTranslate );
				if ( $tmp == false ) {
					echo 'переводчик занят, подождите и потом нажмите F5. авось освободится.';
					exit;
				}*/
			}
			Redirect( '.' );
		} break;
		
		case 'search': {
			$szTitle .= ' / Поиск слов';
			
			?><div class="tr_search"><form action="." method="get">
				<p><input type="text" name="word" value=""/>&nbsp;<input type="submit" value="найти"/></p>
				<input type="hidden" name="mode" value="search"/>
			</form></div><?
			
			if ( isset( $_GET[ 'word' ] ) ) {
				$szWord = urldecode( $_GET[ 'word' ] );
				$tmp = NULL;
				if ( $objTranslator->Translate( $szWord, $tmp ) ) {
					echo '<h4>'.$szWord.'</h4>';
					if ( is_string( $tmp ) ) {
						echo '<p>'.$tmp.'</p>';
					} elseif ( is_array( $tmp ) ) {
						echo '<ol>';
						foreach( $tmp as $v ) {
							echo '<li>'.$v.'</li>';
						}
						echo '</ol>';
					} else {
						ShowVar( $tmp );
					}
				} else {
					echo 'переводчик занят, подождите и потом нажмите F5. авось освободится.';
					exit;
				}
			}
		} break;
		
		case 'text': {
			require( 'text.php' );
		} break; 
		
		default: {
			$szTitle .= ' / Список';
			
			require( 'list.php' );
			
			$objList = new CTrList( );
			echo $objList->GetData( $objTranslator );
		} break;
	}
	
	$szBody = ob_get_clean( );
	
	$time1 = microtime( );
	$time1 = explode( ' ', $time1 );
	$time1 = array_map( 'floatval', $time1 );
	$time1 = $time1[ 0 ] + $time1[ 1 ];

?><!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="en" lang="en">
<head>
	<meta http-equiv="Content-Type" content="text/html; charset=UTF-8"/>
	<title><?=$szTitle?></title>
	<link rel="stylesheet" type="text/css" href="main.css"/>
</head>
<body><div class="wrap"><?=$szBody?><p><? echo 'время работы: '.( round( $time1 - $time, 3 ) * 1000 ).'мс'; ?></p></div></body>
</html>