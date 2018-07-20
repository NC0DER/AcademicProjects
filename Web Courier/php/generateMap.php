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

//Searching users table
$select1 = "SELECT id,name,address,addr_num,zip_code,phone_num,coords FROM transit_hub";
$select2 = "SELECT id,name,address,addr_num,zip_code,coords,phone_num,transit_id FROM local_store";
$select3 = "SELECT hub_a,hub_b FROM connects";

$returned = $connection -> query($select1);

//Create object with cast from array - one liner no class definition
$object = (object) array('id' => '', 'name' => '','address' => '','addr_num' => ''
,'city' => '','zip_code' => '','phone_num' => '','coords' => '');

$json_str = ""; //Initialize and convert string to UTF-8
iconv(mb_detect_encoding($json_str, mb_detect_order(), true), "UTF-8", $json_str);

//Fetching the required columns row-by-row from the query
while($row = $returned -> fetch_array(MYSQLI_ASSOC)) {
    //var_dump($row); //Debug Line 4
    $object -> id = $row['id'];
    $object -> name = $row['name'];
    $object -> address = $row['address'];
    $object -> addr_num = $row['addr_num'];
    $object -> zip_code = $row['zip_code'];
    $object -> phone_num = $row['phone_num'];
    $object -> coords = $row['coords'];
    $json_str .= json_encode($object,JSON_UNESCAPED_UNICODE);
    $json_str .=";";
}
$json_str = mb_substr($json_str,0,-1); //Remove last ';' from string
$json_str .= "/"; //Separator for next array of objects

$returned = $connection -> query($select2);

//Create new object with cast from array - one liner no class definition
$object = (object) array('id' => '', 'name' => '','address' => '','addr_num' => ''
,'city' => '','zip_code' => '','phone_num' => '','coords' => '', 'tr_id' => '');

//Fetching the required columns row-by-row from the query
while($row = $returned -> fetch_array(MYSQLI_ASSOC)) {
    //var_dump($row); //Debug Line 4
    $object -> id = $row['id'];
    $object -> name = $row['name'];
    $object -> address = $row['address'];
    $object -> addr_num = $row['addr_num'];
    $object -> zip_code = $row['zip_code'];
    $object -> phone_num = $row['phone_num'];
    $object -> coords = $row['coords'];
    $object -> tr_id = $row['transit_id'];
    $json_str .= json_encode($object,JSON_UNESCAPED_UNICODE);
    $json_str .=";";
}
$json_str = mb_substr($json_str,0,-1); //Remove last ';' from string
$json_str .= "/"; //Separator for next array of objects

$returned = $connection -> query($select3); //Here

//Create new object with cast from array - one liner no class definition
$object = (object) array('hub_a' => '', 'hub_b' => '');

//Fetching the required columns row-by-row from the query
while($row = $returned -> fetch_array(MYSQLI_ASSOC)) {
    //var_dump($row); //Debug Line 4
    $object -> hub_a = $row['hub_a'];
    $object -> hub_b = $row['hub_b'];
    $json_str .= json_encode($object,JSON_UNESCAPED_UNICODE);
    $json_str .=";";
}
$json_str = mb_substr($json_str,0,-1); //Remove last ';' from string

echo $json_str;
$connection->close();
?>