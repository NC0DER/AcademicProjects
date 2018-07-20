<?php
session_start();
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

//Find which user is logged in
$username=$_SESSION['session_username'];
$store = "";

//Find where the store employee works
$selectStore = "SELECT store FROM users WHERE uname ='$username'";
$returned = $connection -> query($selectStore);
$result = $connection -> affected_rows;
if ($result === 1){ //Username is unique only one column is returned
    $row = $returned -> fetch_array(MYSQLI_ASSOC);
    $store = $row['store'];
}else{
    echo "ERROR";
    exit();
}

//Searching web order table
$select_query = "SELECT tracking_num,typeof,current_point,route,cost,eta FROM web_order";
$returned = $connection -> query($select_query);

//Create object with cast from array - one liner no class definition
$object = (object) array(
    'tracking_num' => '', 'typeof' => '',
    'current_point' => '','route' => '',
    'cost' => '','eta' => ''
);

$json_str = ""; //Initialize and convert string to UTF-8
iconv(mb_detect_encoding($json_str, mb_detect_order(), true), "UTF-8", $json_str);

//Fetching the required columns row-by-row from the query
while($row = $returned -> fetch_array(MYSQLI_ASSOC)) {
    //var_dump($row); //Debug Line 4
    $path = mb_split(",",$row['route']);
    //If first and last entry of path are the store itself then the order belongs to it
    if(($path[0] === $store) || ($path[count($path)-1] === $store)){
        $object -> tracking_num = $row['tracking_num'];
        $object -> typeof = $row['typeof'];
        $object -> current_point = $row['current_point'];
        $object -> route = $row['route'];
        $object -> cost = $row['cost'];
        $object -> eta = $row['eta'];
        $json_str .= json_encode($object,JSON_UNESCAPED_UNICODE);
        $json_str .=";";
    }
}
echo $json_str;
$connection->close();
?>