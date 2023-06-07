<?php
	/**
	 * 	Класс для взаимодействия с переводчиком
	 * 	Входные строки в UTF-8
	 */
	class CTranslator {
		private	$m_szBinary			= '',	// путь к бинарнику
				$m_szTmpFile		= 'in_list',		// имя временного файла для списка
				$m_szComposeName	= 'in_pair',		// имя временной композиции
				$m_szDelFileName	= 'delword',		// имя временного файла для удаления слова
				$m_szInWord			= 'in_word',		// имя временного файла для заданного слова
				$m_szInTranslate	= 'in_translate',	// имя временного файла для заданного перевода
				$m_szTrInWord		= 'trword',			// имя временного файла ввода слова для перевода
				$m_szTrOutWord		= 'trwordout',		// имя временного файла вывода перевода
				$m_szIdIn			= 'wordid',			// имя временного файла ввода id слов
				$m_szTmpFilePath	= '',
				$m_szBinaryDir		= ''
				;
		
		/**
		 * 	Конструктор
		 */
		public function __construct( $szBinary, $szTmpFilePath ) {
			$this->m_szBinary		= $szBinary;
			$this->m_szTmpFilePath	= $szTmpFilePath;
			
			$this->m_szTmpFile		= $szTmpFilePath.$this->m_szTmpFile;
			$this->m_szComposeName	= $szTmpFilePath.$this->m_szComposeName;
			$this->m_szDelFileName	= $szTmpFilePath.$this->m_szDelFileName;
			$this->m_szInWord		= $szTmpFilePath.$this->m_szInWord;
			$this->m_szInTranslate	= $szTmpFilePath.$this->m_szInTranslate;
			$this->m_szTrInWord		= $szTmpFilePath.$this->m_szTrInWord;
			$this->m_szTrOutWord	= $szTmpFilePath.$this->m_szTrOutWord;
			$this->m_szIdIn			= $szTmpFilePath.$this->m_szIdIn;
		} // function __construct
		
		/**
		 * 	Подсчитывает количество слов
		 */
		public function WordTotalNumber( ) {
			exec( $this->m_szBinary.' i', $tmp );
			return intval( $tmp[ 0 ] );
		} // function WordTotalNumber
		
		/**
		 * 	Получение списка слов и их переводов
		 * 	@param $iOffset int смещение относительно начала
		 * 	@param $iCount int количество слов
		 * 	@return array
		 */
		public function GetList( $iOffset, $iCount ) {
			if ( file_exists( $this->m_szTmpFile ) ) {
				return false;
			}
			
			$arrRet = array( );
			exec( $this->m_szBinary.' s '.$this->m_szTmpFile.' '.$iOffset.' '.$iCount, $out, $ret );
			
			$szText = file_get_contents( $this->m_szTmpFile );
			$szText = iconv( 'UTF-32BE', 'UTF-8', $szText );
			$szText = preg_replace( '/ encoding="[^"]*"/suU', '', $szText, 1 );
			
			if ( file_exists( $this->m_szTmpFile ) ) {
				unlink( $this->m_szTmpFile );
			}
			
			$objDom = new DOMDocument( );
			
			if ( @$objDom->loadXML( $szText ) ) {
				$objXpath = new DOMXPath( $objDom );
				$szQuery = '//dict/f';
				$objNodeList = $objXpath->query( $szQuery );
				
				foreach( $objNodeList as $v ) {
					$arrOrig = array( );
					$tmp = $v->getElementsByTagName( 'o' );
					foreach( $tmp as $w ) {
						$iId = intval( $w->getAttribute( 'id' ) );
						$arrOrig[ $iId ] = $w->nodeValue;
					}
					$arrTr = array( );
					$tmp = $v->getElementsByTagName( 't' );
					foreach( $tmp as $w ) {
						$iId = intval( $w->getAttribute( 'id' ) );
						$arrTr[ $iId ] = $w->nodeValue;
					}
					//sort( $arrTr );
					$arrRet[ ] = array( $arrOrig, $arrTr );
				}
			}
			
			return $arrRet;
		} // function GetList
		
		/**
		 * 	Добавление пары оригинал - перевод.
		 * 	@param $szOriginal string оригинал слова
		 * 	@param $szTranslate string перевод слова
		 * 	@return void
		 */
		public function AddTranslatedWord( $szOriginal, $szTranslate ) {
			$szFileNameOrig			= $this->m_szComposeName.'_orig.utf32';
			$szFileNameTranslate	= $this->m_szComposeName.'_translate.utf32';
			
			if ( file_exists( $szFileNameOrig ) || file_exists( $szFileNameTranslate ) ) {
				return false;
			}
			
			$szOriginal		= iconv( 'UTF-8', 'UTF-32BE', $szOriginal	);
			$szTranslate	= iconv( 'UTF-8', 'UTF-32BE', $szTranslate	);
			
			// сливаем текст во временные файлы
			file_put_contents( $szFileNameOrig,			$szOriginal		);
			file_put_contents( $szFileNameTranslate,	$szTranslate	);
			
			exec( $this->m_szBinary.' a '.$this->m_szComposeName );
			
			// сносим файлы, чтоб переводчик не был занят
			unlink( $szFileNameOrig			);
			unlink( $szFileNameTranslate	);
			
			return true;
		} // function AddTranslatedWord
		
		/**
		 * 	Удаляет слово из словаря
		 * 	@param $szWord string слово
		 */
		public function DelWord( $szWord ) {
			if ( file_exists( $this->m_szDelFileName ) ) {
				return false;
			}
			
			$szWord = iconv( 'UTF-8', 'UTF-32BE', $szWord );
			
			file_put_contents( $this->m_szDelFileName, $szWord );
			exec( $this->m_szBinary.' d '.$this->m_szDelFileName );
			unlink( $this->m_szDelFileName );
			
			return true;
		} // function DelWord
		
		/**
		 * 	Удаление заданного перевода для заданного слова
		 * 	@param $szWord string слово
		 * 	@param $szTranslate string перевод, который нужно удалить для слова
		 */
		public function DelTargetTranslate( $szWord, $szTranslate ) {
			if ( file_exists( $this->m_szInWord ) || file_exists( $this->m_szInTranslate ) ) {
				return false;
			}
			
			$szWord			= iconv( 'UTF-8', 'UTF-32BE', $szWord		);
			$szTranslate	= iconv( 'UTF-8', 'UTF-32BE', $szTranslate	);
			
			file_put_contents( $this->m_szInWord,		$szWord			);
			file_put_contents( $this->m_szInTranslate,	$szTranslate	);
			
			exec( $this->m_szBinary.' r '.$this->m_szInWord.' '. $this->m_szInTranslate );
			
			unlink( $this->m_szInWord		);
			unlink( $this->m_szInTranslate	);
			
			return true;
		} // function DelTargetTranslate
		
		/**
		 * 	Перевод слова
		 */
		public function Translate( $szWord, &$mxdData ) {
			if ( file_exists( $this->m_szTrInWord ) || file_exists( $this->m_szTrOutWord ) ) {
				return false;
			}
			
			$szWord = iconv( 'UTF-8', 'UTF-32BE', $szWord );
			
			file_put_contents( $this->m_szTrInWord, $szWord );
			exec( $this->m_szBinary.' t '.$this->m_szTrInWord.' '.$this->m_szTrOutWord );
			
			$tmp = file_get_contents( $this->m_szTrOutWord );
			$tmp = iconv( 'UTF-32BE', 'UTF-8', $tmp );
			$tmp = urlencode( $tmp );
			$szSep = '%F3%B0%80%82';
			$tmp = explode( $szSep, $tmp );
			$tmp = array_map( 'urldecode', $tmp );
			if ( count( $tmp ) == 1 ) {
				$mxdData = $tmp[ 0 ];
			} else {
				$mxdData = $tmp;
			}
			
			unlink( $this->m_szTrInWord		);
			unlink( $this->m_szTrOutWord	);
			
			return true;
		} // function Translate
		
		/**
		 * 	Удаление слова по id
		 */
		public function DelById( $iId ) {
			if ( file_exists( $this->m_szIdIn ) ) {
				return false;
			}
			
			file_put_contents( $this->m_szIdIn, $iId.'' );
			exec( $this->m_szBinary.' g '.$this->m_szIdIn );
			unlink( $this->m_szIdIn );
			
			return true;
		} // function DelById
		
		/**
		 * 	Удаление перевода слова по их id
		 */
		public function DelTranslateById( $iWordId, $arrTrId ) {
			if ( file_exists( $this->m_szIdIn ) ) {
				return false;
			}
			
			file_put_contents( $this->m_szIdIn, $iWordId.' t '.join( ' ', $arrTrId ) );
			exec( $this->m_szBinary.' g '.$this->m_szIdIn );
			unlink( $this->m_szIdIn );
			
			return true;
		} // function DelTranslateById
		
	} // class CTranslator
	
	
?>