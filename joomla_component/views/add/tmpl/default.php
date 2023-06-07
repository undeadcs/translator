<?php
	defined( '_JEXEC' ) or die( 'hello world. bye bye.' );
	JFactory::getDocument( )->addStyleSheet( 'components/com_udtranslator/main.css' );
	
?><div class="udtr_wrap"><div class="tr_add"><form name="adminForm" action="index.php" method="post"><table>
	<tr><td>Оригинал</td><td><textarea name="original"></textarea></td></tr>
	<tr><td>Перевод</td><td><textarea name="translate"></textarea></td></tr>
	<input type="hidden" name="option" value="com_udtranslator"/>
	<input type="hidden" name="controller" value="add"/>
	<input type="hidden" name="task" value=""/>
</table></form></div></div>