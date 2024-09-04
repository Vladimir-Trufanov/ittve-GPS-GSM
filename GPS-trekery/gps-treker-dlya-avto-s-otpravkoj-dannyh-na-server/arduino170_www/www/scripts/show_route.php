<?php
// Показать маршрут
// авто, дата1, дата2
function ShowRoute($avto, $datetime1, $datetime2,$page)
  {
	$objResponse = new xajaxResponse();

  //$objResponse->alert("ShowRoute id_avto=".$avto); 
  // подключение к базе данных 
  require_once("my.php");
  require_once("mybaza.php");
  // удаление всех меток
  $script1="map.removeOverlay(polyline1);";
  $objResponse->script($script1);
  //$objResponse->alert($script1);
  $script1="myCollection.removeAll();";
  $objResponse->script($script1);
  // Поиск меток
  $query0="SELECT COUNT(id) FROM gps_tracker WHERE id_avto=".$avto." &&
           data>='".$datetime1."' && data<='".$datetime2."' ";
  //$query0="SELECT COUNT(id) FROM gps_tracker WHERE id_avto=".$avto." ";
  $res0=mysql_query($query0);
  $nn=mysql_result($res0,0);
//$objResponse->alert($query0."  ".$nn);
  if($nn>0)
    {
    $page0=($page-1)*NN1;
    if($nn-$page0<NN1)
      $page0=max(0,$nn-NN1);
    $query1="SELECT * FROM gps_tracker WHERE id_avto=".$avto." ORDER BY id ASC LIMIT ".$page0.", ".NN1." ";
//$objResponse->alert($query1);
    $res1=mysql_query($query1);
    $script1=""; $i=0;$data1="";$data2="";
    $script2="polyline1 = new ymaps.Polyline([";
    while($row1=mysql_fetch_assoc($res1))
      {
      if($i==0)
        $data1=$row1[data];
      $data2=$row1[data];  
      $script1.="metka[".$i."]=new ymaps.Placemark([".$row1[lat].",".$row1[lon]."],{balloonContentBody: ['".$row1[data]."']});";
      $script2.="[".$row1[lat].",".$row1[lon]."],";
      $script1.="metka[".$i."].name='222222';";
      //$content1.="map.addOverlay(metka[".$i."]);";
      $script1.="myCollection.add(metka[".$i."]);";
      $lat=$row1[lat];$lon=$row1[lon];
      $i++;
      }
    $objResponse->script($script1);
    $script2.="]);"; 
    $script2.="myCollection.add(polyline1);";
    //$script2.="map.geoObjects.add(polyline1);";
    $script2.="map.setBounds(polyline1.geometry.getBounds());";
    //$objResponse->alert($script2);
    if($i>1)
       $objResponse->script($script2);
    else
      {
      $script1="map.panTo([".$lat.",".$lon."],{duration:1000});";
      $objResponse->script($script1);
      }
    }
  //$objResponse->alert("ShowRoute ".$avto." ".$datetime1." ".$datetime2);	
  $objResponse->assign("ButtonFormOptions","value", "Смотрим"); 
  //  кнопки
  if($page>1)
     {
     $objResponse->assign("prev","value", "< ".($page-1)*NN1);   
     $objResponse->script("document.getElementById('prev').style.visibility='visible';");
     }
  else
     {
     $objResponse->script("document.getElementById('prev').style.visibility='hidden';");
     }
  if($i>0)
    {
    $objResponse->assign("info","value", $data1." - ".$data2);   
    $objResponse->script("document.getElementById('info').style.visibility='visible';");  
    }
  else
    {
    $objResponse->script("document.getElementById('info').style.visibility='hidden';");  
    }
  if($page*NN1<$nn)
    {  
    $objResponse->script("document.getElementById('next').style.visibility='visible';");
     $objResponse->assign("next","value", " >".($nn-$page*NN1));   
    }
  else
    $objResponse->script("document.getElementById('next').style.visibility='hidden';");
  
  $objResponse->script("flag1=0;"); 
    
  return $objResponse;
  }

?>
