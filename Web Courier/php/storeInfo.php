<?php
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

//Getting all stores/transit hubs
//$select = "SELECT name,zip_code FROM transit_hub UNION ALL SELECT name,zip_code FROM local_store ORDER BY zip_code";

//Getting all stores
$select = "SELECT name,zip_code FROM local_store ORDER BY zip_code";
$returned = $connection -> query($select);

//Create object with cast from array - one liner no class definition
$object = (object) array('name' => '','zip_code' => 0);

$json_str = ""; //Initialize and convert string to UTF-8
iconv(mb_detect_encoding($json_str, mb_detect_order(), true), "UTF-8", $json_str);

//Fetching the required columns row-by-row from the query
while($row = $returned -> fetch_array(MYSQLI_ASSOC)) {
    //var_dump($row); //Debug Line 4
    $object -> name = $row['name'];
    $object -> zip_code = (int) $row['zip_code']; //cast to int
    $json_str .= json_encode($object,JSON_UNESCAPED_UNICODE);
    $json_str .=";";
}
$json_str = mb_substr($json_str,0,-1); //Remove last ';' from string

echo $json_str;
$connection->close();
?>