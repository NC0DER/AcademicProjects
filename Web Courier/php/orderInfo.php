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
$tracking_num = isset($_POST['track_num']) ? $_POST['track_num'] : null;

if($tracking_num  === null){
    die("The ajax response from manageOrder.js is missing:");
}

//Ensuring that the encoding of the string is set to UTF-8, and eliminating escape characters
$utf8_str = $tracking_num;
iconv(mb_detect_encoding($utf8_str, mb_detect_order(), true), "UTF-8",$utf8_str );
$id = $connection -> real_escape_string($utf8_str);

//Find the order with the unique tracking number
$select_query = "SELECT current_point,route FROM web_order WHERE tracking_num ='$tracking_num'";

$returned = $connection -> query($select_query);

//Fetching the result of the query
$result = $connection -> affected_rows;

if ($result === 1){
    $row = $returned->fetch_array(MYSQL_ASSOC);
    $current = $row['current_point'];
    $path = $row['route'];

    $pos = mb_strpos($path,$current);
    $visited = mb_substr($path,0, $pos);

    //Remove comma from the end of the string, if it exists
    if(mb_substr($visited,-1) === ',')
        $visited = mb_substr($visited,0, mb_strlen($visited)-1);

    //Create object with cast from array - one liner no class definition
    $object = (object) array(
        'current' => '','visited' => ''
    );
    //Creating json string to send
    $json_str = ""; //Initialize and convert string to UTF-8
    iconv(mb_detect_encoding($json_str, mb_detect_order(), true), "UTF-8", $json_str);

    //Setting json string from object
    $object -> current = $current;
    $object -> visited = $visited;
    $json_str .= json_encode($object,JSON_UNESCAPED_UNICODE);

    echo $json_str;
}
else if ($result === 0)
    echo "FAIL";
else echo "ERROR";

$connection->close();
?>