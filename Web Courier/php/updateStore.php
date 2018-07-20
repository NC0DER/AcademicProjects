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
$id = isset($_POST['ls_id']) ? $_POST['ls_id'] : null;
$name = isset($_POST['ls_name']) ? $_POST['ls_name'] : null;
$address = isset($_POST['ls_address']) ? $_POST['ls_address'] : null;
$addr_num = isset($_POST['ls_addr_num']) ? $_POST['ls_addr_num'] : null;
$city = isset($_POST['ls_city']) ? $_POST['ls_city'] : null;
$zip = isset($_POST['ls_zip']) ? $_POST['ls_zip'] : null;
$phone_num = isset($_POST['ls_phone_num']) ? $_POST['ls_phone_num'] : null;
$coords = isset($_POST['ls_coords']) ? $_POST['ls_coords'] : null;
$transit = isset($_POST['ls_transit']) ? $_POST['ls_transit'] : null;


if($id === null || $name === null || $address === null || $addr_num === null
    || $city === null || $zip === null || $phone_num === null || $coords === null || $transit === null ){
    die("The ajax response from manageStore.js is missing:");
}

//Ensuring that the encoding of the string is set to UTF-8, and eliminating escape characters
$utf8_str = $id;
iconv(mb_detect_encoding($utf8_str, mb_detect_order(), true), "UTF-8",$utf8_str );
$id = $connection -> real_escape_string($utf8_str);

$utf8_str = $name;
iconv(mb_detect_encoding($utf8_str, mb_detect_order(), true), "UTF-8",$utf8_str );
$name = $connection -> real_escape_string($utf8_str);

$utf8_str = $address;
iconv(mb_detect_encoding($utf8_str, mb_detect_order(), true), "UTF-8",$utf8_str );
$address = $connection -> real_escape_string($utf8_str);

$utf8_str = $addr_num;
iconv(mb_detect_encoding($utf8_str, mb_detect_order(), true), "UTF-8",$utf8_str );
$addr_num = $connection -> real_escape_string($utf8_str);
//var_dump($uname,$upass,$urole,$ustore);

$utf8_str = $city;
iconv(mb_detect_encoding($utf8_str, mb_detect_order(), true), "UTF-8",$utf8_str );
$city = $connection -> real_escape_string($utf8_str);

$utf8_str = $zip;
iconv(mb_detect_encoding($utf8_str, mb_detect_order(), true), "UTF-8",$utf8_str );
$zip = $connection -> real_escape_string($utf8_str);

$utf8_str = $phone_num;
iconv(mb_detect_encoding($utf8_str, mb_detect_order(), true), "UTF-8",$utf8_str );
$phone_num = $connection -> real_escape_string($utf8_str);

$utf8_str = $coords;
iconv(mb_detect_encoding($utf8_str, mb_detect_order(), true), "UTF-8",$utf8_str );
$coords = $connection -> real_escape_string($utf8_str);

$utf8_str = $transit;
iconv(mb_detect_encoding($utf8_str, mb_detect_order(), true), "UTF-8",$utf8_str );
$transit = $connection -> real_escape_string($utf8_str);

//Find transit id from transit name
$exists_parent_store = "SELECT id FROM transit_hub WHERE name ='$transit'";

$returned = $connection -> query($exists_parent_store);
$result = $connection -> affected_rows;

$transit_id = "";
if($result === 1){
    $row = $returned -> fetch_array(MYSQLI_ASSOC);
    $transit_id = $row['id'];
}
else if ($result === 0){
    echo "NO_PARENT";
    $connection->close();
    exit();
}else{
    echo "Error";
    $connection->close();
    exit();
}

//Check to see if it already exists
$exists_query = "SELECT name FROM local_store WHERE id ='$id'";
$update_query = "UPDATE local_store SET name = '$name', address = '$address', addr_num = '$addr_num', city = '$city', zip_code = '$zip', phone_num = '$phone_num', coords = '$coords', transit_id = '$transit_id' WHERE id = '$id'";

$connection -> query($exists_query);
//Fetching the result of the select query
$result = $connection -> affected_rows;
if ($result === 0){
    echo "NOT_EXISTS";
    $connection->close();
    exit();
}else if($result === 1){
    $connection -> query($update_query);
    //Fetching the result of the insert query
    $result = 0;
    $result = $connection -> affected_rows;

    if ($result === 1){
        echo "OK";
    }
    else if ($result === 0)
        echo "FAIL";
    else echo "ERROR";
}else{
    echo "ERROR";
    $connection->close();
    exit();
}
$connection->close();
?>