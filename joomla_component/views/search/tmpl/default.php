<?php
	defined( '_JEXEC' ) or die( 'hello world. bye bye.' );
	
?><div class="udtr_wrap"><div class="tr_search"><form name="adminForm" action="index.php" method="get">
	<input type="hidden" name="option" value="com_udtranslator"/>
	<input type="hidden" name="controller" value="search"/>
	<input type="hidden" name="task" value=""/>
	<p><input type="text" name="word" value=""/></p>
</form></div></div><?

	if ( isset( $this->result ) ) {
		echo $this->result;
	}

?>