<?php
// Показать последнюю позицию
// авто 
function ShowPoint($avto)
  {
	$objResponse = new xajaxResponse();

  //$objResponse->alert("ShowPoint ".$avto);
  // подключение к базе данных 
  require_once("mybaza.php");
  // удаление всех меток
  $script1="map.removeOverlay(polyline1);";
  $objResponse->script($script1);
  $script1="myCollection.removeAll();";
  $objResponse->script($script1);
  // Поиск меток
  $lat="";$lon="";$data="";
  $query1="SELECT * FROM gps_tracker WHERE id_avto=".$avto." ORDER BY id DESC LIMIT 0, 1 ";
  $res1=mysql_query($query1);
  $script1=""; $i=0;
  while($row1=mysql_fetch_assoc($res1))
    {
    $script1.="metka[".$i."]=new ymaps.Placemark([".$row1[lat].",".$row1[lon]."],{balloonContentBody: ['".$row1[data]."']});";
    $script1.="metka[".$i."].name='222222';";
    //$content1.="map.addOverlay(metka[".$i."]);";
    $script1.="myCollection.add(metka[".$i."]);";
    $lat=$row1[lat];$lon=$row1[lon];$data=$row1[data];
    $i++;
    }
  $objResponse->script($script1);
  if($i>0)
  	  {
      $script1="map.setZoom(16);";
      $script1.="map.panTo([".$lat.",".$lon."],{duration:500});";
      $objResponse->script($script1);
      }
  $objResponse->assign("ButtonFormOptions","value", "Смотрим"); 
  //  кнопки
  $objResponse->script("document.getElementById('prev').style.visibility='hidden';");
  if($i>0)
    {
    $objResponse->assign("info","value", $data);   
    $objResponse->script("document.getElementById('info').style.visibility='visible';");  
    }
  else
    {
    $objResponse->script("document.getElementById('info').style.visibility='hidden';");  
    } 
  $objResponse->script("document.getElementById('next').style.visibility='hidden';");
  $objResponse->assign("tekpage","value", 1);   
  // флаг=0
  $objResponse->script("flag1=0;"); 
  
    
  return $objResponse;
  }


?>
