<?php
//// Перевод  utf --> win-1251
function utftowin($s)
{
    $s=str_replace(chr(hexdec(D0)).chr(hexdec(B0)),"а",$s);  $s=str_replace(chr(hexdec(D0)).chr(hexdec(90)),"А",$s);
    $s=str_replace(chr(hexdec(D0)).chr(hexdec(B1)),"б",$s);  $s=str_replace(chr(hexdec(D0)).chr(hexdec(91)),"Б",$s);
    $s=str_replace(chr(hexdec(D0)).chr(hexdec(B2)),"в",$s);  $s=str_replace(chr(hexdec(D0)).chr(hexdec(92)),"В",$s);
    $s=str_replace(chr(hexdec(D0)).chr(hexdec(B3)),"г",$s);  $s=str_replace(chr(hexdec(D0)).chr(hexdec(93)),"Г",$s);
    $s=str_replace(chr(hexdec(D0)).chr(hexdec(B4)),"д",$s);  $s=str_replace(chr(hexdec(D0)).chr(hexdec(94)),"Д",$s);
    $s=str_replace(chr(hexdec(D0)).chr(hexdec(B5)),"е",$s);  $s=str_replace(chr(hexdec(D0)).chr(hexdec(95)),"Е",$s);
    $s=str_replace(chr(hexdec(D1)).chr(hexdec(x91)),"ё",$s);  $s=str_replace(chr(hexdec(D0)).chr(hexdec(x81)),"Ё",$s);
    $s=str_replace(chr(hexdec(D0)).chr(hexdec(B6)),"ж",$s);  $s=str_replace(chr(hexdec(D0)).chr(hexdec(96)),"Ж",$s);
    $s=str_replace(chr(hexdec(D0)).chr(hexdec(B7)),"з",$s);  $s=str_replace(chr(hexdec(D0)).chr(hexdec(97)),"З",$s);
    $s=str_replace(chr(hexdec(D0)).chr(hexdec(B8)),"и",$s);  $s=str_replace(chr(hexdec(D0)).chr(hexdec(98)),"И",$s);
    $s=str_replace(chr(hexdec(D0)).chr(hexdec(B9)),"й",$s);  $s=str_replace(chr(hexdec(D0)).chr(hexdec(99)),"Й",$s);
    $s=str_replace(chr(hexdec(D0)).chr(hexdec(BA)),"к",$s);  $s=str_replace(chr(hexdec(D0)).chr(hexdec(x9A)),"К",$s);
    $s=str_replace(chr(hexdec(D0)).chr(hexdec(BB)),"л",$s);  $s=str_replace(chr(hexdec(D0)).chr(hexdec(x9B)),"Л",$s);
    $s=str_replace(chr(hexdec(D0)).chr(hexdec(BC)),"м",$s);  $s=str_replace(chr(hexdec(D0)).chr(hexdec(x9C)),"М",$s);
    $s=str_replace(chr(hexdec(D0)).chr(hexdec(BD)),"н",$s);  $s=str_replace(chr(hexdec(D0)).chr(hexdec(x9D)),"Н",$s);
    $s=str_replace(chr(hexdec(D0)).chr(hexdec(BE)),"о",$s);  $s=str_replace(chr(hexdec(D0)).chr(hexdec(x9E)),"О",$s);
    $s=str_replace(chr(hexdec(D0)).chr(hexdec(BF)),"п",$s);  $s=str_replace(chr(hexdec(D0)).chr(hexdec(x9F)),"П",$s);
    $s=str_replace(chr(hexdec(D1)).chr(hexdec(80)),"р",$s);  $s=str_replace(chr(hexdec(D0)).chr(hexdec(A0)),"Р",$s);
    $s=str_replace(chr(hexdec(D1)).chr(hexdec(81)),"с",$s);  $s=str_replace(chr(hexdec(D0)).chr(hexdec(A1)),"С",$s);
    $s=str_replace(chr(hexdec(D1)).chr(hexdec(82)),"т",$s);  $s=str_replace(chr(hexdec(D0)).chr(hexdec(A2)),"Т",$s);
    $s=str_replace(chr(hexdec(D1)).chr(hexdec(83)),"у",$s);  $s=str_replace(chr(hexdec(D0)).chr(hexdec(A3)),"У",$s);
    $s=str_replace(chr(hexdec(D1)).chr(hexdec(84)),"ф",$s);  $s=str_replace(chr(hexdec(D0)).chr(hexdec(A4)),"Ф",$s);
    $s=str_replace(chr(hexdec(D1)).chr(hexdec(85)),"х",$s);  $s=str_replace(chr(hexdec(D0)).chr(hexdec(A5)),"Х",$s);
    $s=str_replace(chr(hexdec(D1)).chr(hexdec(86)),"ц",$s);  $s=str_replace(chr(hexdec(D0)).chr(hexdec(A6)),"Ц",$s);
    $s=str_replace(chr(hexdec(D1)).chr(hexdec(87)),"ч",$s);  $s=str_replace(chr(hexdec(D0)).chr(hexdec(A7)),"Ч",$s);
    $s=str_replace(chr(hexdec(D1)).chr(hexdec(88)),"ш",$s);  $s=str_replace(chr(hexdec(D0)).chr(hexdec(A8)),"Ш",$s);
    $s=str_replace(chr(hexdec(D1)).chr(hexdec(89)),"щ",$s);  $s=str_replace(chr(hexdec(D0)).chr(hexdec(A9)),"Щ",$s);
    $s=str_replace(chr(hexdec(D1)).chr(hexdec(x8A)),"ъ",$s);  $s=str_replace(chr(hexdec(D0)).chr(hexdec(AA)),"Ъ",$s);
    $s=str_replace(chr(hexdec(D1)).chr(hexdec(x8B)),"ы",$s);  $s=str_replace(chr(hexdec(D0)).chr(hexdec(AB)),"Ы",$s);
    $s=str_replace(chr(hexdec(D1)).chr(hexdec(x8C)),"ь",$s);  $s=str_replace(chr(hexdec(D0)).chr(hexdec(AC)),"Ь",$s);
    $s=str_replace(chr(hexdec(D1)).chr(hexdec(x8D)),"э",$s);  $s=str_replace(chr(hexdec(D0)).chr(hexdec(AD)),"Э",$s);
    $s=str_replace(chr(hexdec(D1)).chr(hexdec(x8E)),"ю",$s);  $s=str_replace(chr(hexdec(D0)).chr(hexdec(AE)),"Ю",$s);
    $s=str_replace(chr(hexdec(D1)).chr(hexdec(x8F)),"я",$s);  $s=str_replace(chr(hexdec(D0)).chr(hexdec(AF)),"Я",$s);
return $s;
}

// Строка страниц
//1,2..k1,k2,k3,k4..n-1,n
//  arg1 - тек. страница 
//  arg2 - кол-во страниц
//  arg2 - кол-во k
function doarray1($arg1,$arg2,$arg3)
{
$y=array();
    if($arg2<=($arg3+4)) // без точек
      {
      for($j=0;$j<$arg2;$j++)
        $y[$j]=$j+1;
      }
    else
      {
      if($arg1<=$arg3+2)  // 11121....11111
         {
         for($j=0;$j<($arg3+2);$j++)
           $y[$j]=$j+1;
         $y[$j]="...";$j++;  
         $y[$j]=$arg2-1;$j++;  
         $y[$j]=$arg2;  
         }
      elseif($arg1>($arg2-$arg3-2))  //  1111.....11211
         {
         $y[0]=1;  
         $y[1]=2;  
         $y[2]="...";  
         for($j=3;$j<($arg3+5);$j++)
           $y[$j]=$arg2-$arg3+$j-4;
         }
      else  // 11..112111...11
         {
         $y[0]=1;  
         $y[1]=2;  
         $y[2]="...";  
         for($j=3;$j<=($arg3+3);$j++)
           $y[$j]=$arg1+$j-4;
         $y[$j]="...";$j++;  
         $y[$j]=$arg2-1;$j++;  
         $y[$j]=$arg2;  
         }
      }
    return $y;
}
// фильтр на ; \r \n
function filter1($arg1)
  {

  $arg1=str_replace("\r","",$arg1);
  $arg1=str_replace("\n","",$arg1);
  $arg1=str_replace(";","",$arg1);
  $arg1=ltrim(utftowin($arg1));

  return $arg1;
  }

?>
