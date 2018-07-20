<?php
session_start();
header("Location: http://localhost/webcourier/");
session_unset();
exit();
?>