<?php

//Параметры MySQL
$location="localhost";
$user="root";
$pass="";
$db_name="gps_tracker";

//Соединение с базой 
$db=mysql_connect($location,$user,$pass);
mysql_select_db($db_name,$db);
mysql_query("SET CHARACTER SET cp1251");


?>
