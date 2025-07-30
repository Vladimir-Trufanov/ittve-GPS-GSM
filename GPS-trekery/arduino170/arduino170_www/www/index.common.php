<?php

session_start();

require_once ("xajax_core/xajax.inc.php");

  require_once ("clear.php");
  require_once ("dopfun.php");
  // *******
  require_once ("scripts/show_route.php");
  require_once ("scripts/show_point.php");

  
$xajax = new xajax("index.server.php");

  //***** function
  // **********
  $xajax->register(XAJAX_FUNCTION,"ShowRoute");
  $xajax->register(XAJAX_FUNCTION,"ShowPoint");


?>
