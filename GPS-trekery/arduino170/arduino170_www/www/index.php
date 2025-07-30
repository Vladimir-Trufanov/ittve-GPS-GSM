<?php
require_once('index.common.php');
require_once('my.php');
require_once('mybaza.php');
?>

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="ru" lang="ru">


<head>
<meta http-equiv="Content-Type" content="text/html; charset=windows-1251" />
<title>Arduino - GPS - Tracker</title>
<link href="style.css" rel="stylesheet" type="text/css" />
<script src="https://api-maps.yandex.ru/2.1/?lang=ru_RU" type="text/javascript"></script>
<script type="text/javascript">
// флаг xajax-запроса к серверу
var flag1=0;

var map;
var metka=new Array();
var polyline1;
var myCollection;

ymaps.ready(ini);

function ini()
  { 
  // создать карту
    map = new ymaps.Map("ymap", {
            center: [44.04069, 43.05122],
            zoom: 17,
            controls: ['zoomControl', 'typeSelector',"fullscreenControl"]
        });



  myCollection = new ymaps.GeoObjectCollection({}, {
       preset: 'islands#redIcon', //все метки красные
       draggable: false // и их можно перемещать
    });

<?php
$query1="SELECT * FROM gps_tracker WHERE id>0";
$res1=mysql_query($query1);
$content1=""; $i=0;
while($row1=mysql_fetch_assoc($res1))
  {
  $content1.="metka[".$i."]=new ymaps.Placemark([".$row1[lat].",".$row1[lon]."],{balloonContentBody: ['".$row1[data]."']});";
  $content1.="metka[".$i."].name='222222';";
  $content1.="myCollection.add(metka[".$i."]);";
  $i++;
  }
  //echo $content1; */
?>   

  map.geoObjects.add(myCollection);

  update_point();  
  }
// функции
 
// ******* смена данных в форме 
function change_data(dpage)
  {
  xajax.$('ButtonFormOptions').value='Загрузка...';
  var d1=document.getElementById('datetime1');
  var d2=document.getElementById('datetime2');
  var tt=document.getElementById('typeview');
  var tc=document.getElementById('cars');
  var tpage=document.getElementById('tekpage');
  tpage.value=parseInt(tpage.value)+dpage;
  if(tt.value==1)
    {document.getElementById('datetime1').style.visibility='hidden';
     document.getElementById('datetime2').style.visibility='hidden';
     flag1=1;
     xajax_ShowPoint(tc.value); 
     }
  else 
    {document.getElementById('datetime1').style.visibility='visible';
     document.getElementById('datetime2').style.visibility='visible';
     flag1=1;
     xajax_ShowRoute(tc.value,d1.value,d2.value,tpage.value); 
    }
  } 
// обновление текущей позиции   
function update_point()
  {
  var tt=document.getElementById('typeview');
  var tc=document.getElementById('cars');
  if(tt.value==1)
    {
    if(flag1<1)
      xajax_ShowPoint(tc.value); 
    }
  setTimeout("update_point();",5000);
  }
  
</script>
<?php $xajax->printJavascript(''); ?>

</head>
<body onload=';'>
    <div id=zag>
    <b>Yandex-API Такси</b><br>
    Служба Такси
    </div>
    <div id=options>
      <form id='FormOptions' id='FormOptions' action='javascript:void(null);' onsubmit='javascript:void(null);' >  
          <select id=cars name=cars onchange='document.getElementById("tekpage").value=1;change_data(0);'> 
             <?php
               $query1="SELECT * FROM cars WHERE id>0 and status='yes' ";
               $res1=mysql_query($query1);
               $content1=""; $i=0;
               while($row1=mysql_fetch_assoc($res1))
                 {
                  if($i<1)
                    $content1.="<option value=".$row1[id]." selected>".$row1[mark]." ".$row1[number];
                  else
                    $content1.="<option value=".$row1[id].">".$row1[mark]." ".$row1[number];
                  $i++;
                  }
               echo $content1;
            ?>
          </select> 
          <select id=typeview name=typeview onchange='change_data(0);'> 
            <option value=1 selected >Текущее положение                
            <option value=2 >Маршрут  
          </select>
          <input type="submit" id="ButtonFormOptions" value='Смотрим'>     
          <br><input type='datetime' id='datetime1' name='datetime1' value='<?php  echo date("Y-m-d H:i:s",strtotime('now 00:00:00')); ?>' style="visibility:hidden" onchange='change_data(0);'>      
          <input type='datetime' id='datetime2' name='datetime2' value='<?php  echo date("Y-m-d H:i:s"); ?>' style="visibility:hidden" onchange='change_data(0);'> 
  
                   <br>
                   <input type=button id='prev' name='prev' value='<' style="visibility:hidden" onclick="change_data(-1);">      
                   <input type=button id='info' name='info' value='Загрузка карты' style="visibility:visible">      
                   <input type=button id='next' name='next' value='>' style="visibility:hidden"  onclick="change_data(1);">      
                   <input type=hidden id='tekpage' name='tekpage' value='1'>      
      
       </form>          
    </div>
    

    
    <div id="ymap" >
        
    </div>

    
</body>
</html>
