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
$name = isset($_POST['ls_name']) ? $_POST['ls_name'] : null;
$address = isset($_POST['ls_address']) ? $_POST['ls_address'] : null;
$addr_num = isset($_POST['ls_addr_num']) ? $_POST['ls_addr_num'] : null;
$city = isset($_POST['ls_city']) ? $_POST['ls_city'] : null;
$zip = isset($_POST['ls_zip']) ? $_POST['ls_zip'] : null;
$phone_num = isset($_POST['ls_phone_num']) ? $_POST['ls_phone_num'] : null;
$coords = isset($_POST['ls_coords']) ? $_POST['ls_coords'] : null;
$transit = isset($_POST['ls_transit']) ? $_POST['ls_transit'] : null;

//var_dump($uname,$upass,$urole,$ustore);

if($name === null || $address === null || $addr_num === null || $city === null
    || $zip === null || $phone_num === null || $coords === null){
    die("The ajax response from manageStore.js is missing:");
}

//Ensuring that the encoding of the string is set to UTF-8, and eliminating escape characters

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
$exists_query = "SELECT name FROM local_store WHERE name ='$name'";
$insert_query = "INSERT INTO local_store (name, address, addr_num, city, zip_code, phone_num, coords, transit_id) VALUES ('$name', '$address', '$addr_num','$city', '$zip', '$phone_num', '$coords', '$transit_id')";
$retrieve_id_query = "SELECT id FROM local_store WHERE name ='$name'";


$connection -> query($exists_query);
//Fetching the result of the select query
$result = $connection -> affected_rows;
if ($result === 1){
    echo "EXISTS";
    $connection->close();
    exit();
}else{
    $connection -> query($insert_query);
    //Fetching the result of the insert query
    $result = 0;
    $result = $connection -> affected_rows;

    if ($result === 1){
        $returned = $connection -> query($retrieve_id_query);
        $row = $returned -> fetch_array(MYSQLI_ASSOC);
        $number = (int)$row['id'];
        echo $number;
    }
    else if ($result === 0)
        echo "FAIL";
    else echo "ERROR";
}
$connection->close();
?>