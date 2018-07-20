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
$select_query = "SELECT id,name,address,addr_num,city,zip_code,phone_num,coords FROM local_store";

$returned = $connection -> query($select_query);

//Create object with cast from array - one liner no class definition
$object = (object) array('id' => '', 'name' => '', 'address' => '', 'addr_num' => ''
, 'city' => '', 'zip_code' => '', 'phone_num' => '', 'coords' => '');

$json_str = ""; //Initialize and convert string to UTF-8
iconv(mb_detect_encoding($json_str, mb_detect_order(), true), "UTF-8", $json_str);

//Fetching the required columns row-by-row from the query
while($row = $returned -> fetch_array(MYSQLI_ASSOC)) {
    //var_dump($row); //Debug Line 4
    $object -> id = $row['id'];
    $object -> name = $row['name'];
    $object -> address = $row['address'];
    $object -> addr_num = $row['addr_num'];
    $object -> city = $row['city'];
    $object -> zip_code = $row['zip_code'];
    $object -> phone_num = $row['phone_num'];
    $object -> coords = $row['coords'];
    $json_str .= json_encode($object,JSON_UNESCAPED_UNICODE);
    $json_str .=";";
}
echo $json_str;
$connection->close();
?>