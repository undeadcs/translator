<?php
	function ShowVar( ) {
		$iNum = func_num_args( );
		if ( $iNum ) {
			$arrArgs = func_get_args( );
			echo '<pre>';
			call_user_func_array( 'var_dump', $arrArgs );
			echo '</pre>';
		}
	}
	
	/**
	 * 	Включает полный режим отладки, вывод всех ошибок
	 */
	function SwitchOn( ) {
		ini_set( 'display_errors', 1 );
		error_reporting( E_ALL );
	} // function SwitchOn
	
	/**
	 * 	Редирект на заданный урл
	 * 	@param $szUrl string адрес, на который сделать редирект
	 * 	@param $iCode int код возврата ( по умолчанию 301 )
	 */
	function Redirect( $szUrl, $iCode = 301 ) {
		if ( empty( $szUrl ) ) {
			$szUrl = '/';
		}
		header( 'Location: '.$szUrl, true, $iCode );
		exit;
	} // function Redirect
	
	function GetWordNum( ) {
		exec( 'Translator.exe i', $tmp );
		return intval( $tmp[ 0 ] );
	} // function GetWordNum
	
	function GenPagerArray( $iCurPage, $iTotal, $iPageSize, $iPageLength ) {
		$arrRet = array( );
		if ( $iCurPage < 1 ) {
			$iCurPage = 1;
		}
		if ( $iPageSize < 1 ) {
			$iPageSize = 1;
		}
		//
		if ( $iTotal && $iPageSize && $iPageLength ) {
			$iMaxPage = intval( ceil( $iTotal / $iPageSize ) );
			$iCurFrame = intval( ceil( $iCurPage / $iPageLength ) );
			$iStart = ( $iCurFrame - 1 ) * $iPageLength;
			$iEnd = $iCurFrame * $iPageLength + 1;
			
			if ( $iStart ) {
				$arrRet[ 'prevFrame' ] = array( $iStart, false );
			}
			++$iStart;
			for( $i = $iStart; $i < $iEnd; ++$i ) {
				if ( $i > $iMaxPage ) {
					break;
				} else {
					if ( $i == $iCurPage ) {
						$arrRet[ 'pages' ][ ] = array( $i, true );
					} else {
						$arrRet[ 'pages' ][ ] = array( $i, false );
					}
				}
			}
			if ( ( $iEnd > $iPageLength ) && ( $iEnd < ( $iMaxPage + 1 ) ) ) {
				$arrRet[ 'nextFrame' ] = array( $iEnd, false );
			}
			if ( $iCurPage > 1 ) {
				$arrRet[ 'prev' ] = $iCurPage - 1;
			}
			if ( $iCurPage < $iMaxPage ) {
				$arrRet[ 'next' ] = $iCurPage + 1;
			}
		}
		return $arrRet;
	} // function GenPagerArray
?>