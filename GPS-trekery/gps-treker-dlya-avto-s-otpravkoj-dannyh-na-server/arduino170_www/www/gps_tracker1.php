<?php

 require_once("mybaza.php");
 $id_avto=$_GET[id_avto];
 $lat=$_GET[lat]/100000;
 $lon=$_GET[lon]/100000;
 $data=date('Y-m-d H:i:s');
 $query1="INSERT into gps_tracker SET
           id_avto=".$id_avto.",lat=".$lat.",lon=".$lon.",
           data='".$data."' ";
   if(!mysql_query($query1))
    echo "error=".$query1;  
  else
    echo "OK";

?>