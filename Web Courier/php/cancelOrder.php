<?php
//var_dump($_POST); //Debug Line 1
$my_server = "localhost";
$my_admin = "root";
$my_pass = "";
$my_db = "web_courier";

//Connecting to sql server, in case this fails an error is being displayed and then exit().
$connection = new mysqli($my_server, $my_admin, $my_pass, $my_db);

if($connection -> connect_error){
    die("MySQL Connection at the server failed: " . $connection -> connect_error);
}
//Setting utf8 php-side for this connection, (it is already set in the db)
$connection -> query ('SET CHARACTER SET utf8');
$connection -> query ('SET COLLATION_CONNECTION=utf8_general_ci');

//Sanitizing the entire POST array from html, input
$_POST  = filter_input_array(INPUT_POST, FILTER_SANITIZE_STRING);

//Checking the integrity of the response
$tracking_num = isset($_POST['tracking_num']) ? $_POST['tracking_num'] : null;

if($tracking_num  === null){
    die("The ajax response from manageOrder.js is missing:");
}

//Ensuring that the encoding of the string is set to UTF-8, and eliminating escape characters
$utf8_str = $tracking_num;
iconv(mb_detect_encoding($utf8_str, mb_detect_order(), true), "UTF-8",$utf8_str );
$id = $connection -> real_escape_string($utf8_str);

//Delete user from table based on unique username
$delete_query = "DELETE FROM web_order WHERE tracking_num ='$tracking_num'";

$connection -> query($delete_query);

//Fetching the result of the delete query
$result = $connection -> affected_rows;

if ($result === 1)
    echo "OK";
else if ($result === 0)
    echo "FAIL";
else echo "ERROR";

$connection->close();
?>