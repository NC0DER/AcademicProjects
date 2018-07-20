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
$tid = isset($_POST['id']) ? $_POST['id'] : null;

if($tid  === null){
    die("The ajax response from manageUsers.js is missing:");
}

//Ensuring that the encoding of the string is set to UTF-8, and eliminating escape characters
$utf8_str = $tid;
iconv(mb_detect_encoding($utf8_str, mb_detect_order(), true), "UTF-8",$utf8_str );
$id = $connection -> real_escape_string($utf8_str);

//-----Delete user & orders from deleting store-----
//Find where the store employee works
$selectStore = "SELECT name FROM local_store WHERE id ='$id'";
$returned = $connection -> query($selectStore);
$result = $connection -> affected_rows;
if ($result === 1){ //id is unique only one column is returned
    $row = $returned -> fetch_array(MYSQLI_ASSOC);
    $store = $row['name'];
}else{
    echo "ERROR";
    exit();

}

//Delete user from table based on the deleting store
$delete_user = "DELETE FROM users WHERE store ='$store'";
$connection -> query($delete_user);
$result = $connection -> affected_rows;
if($result < 0){
    echo "ERROR";
    exit();
}
//Delete all orders linked to the deleting store
//Pattern match is first or last 2 letters of store name capitalized
$char = mb_substr($store,0,2);
$ch = mb_strtoupper($char);

$delete_orders = "DELETE FROM web_order WHERE tracking_num LIKE '%{$ch}%'";
$connection -> query($delete_orders);
$result = $connection -> affected_rows;
if($result < 0){
    echo "ERROR";
    exit();
}
//-----Delete user & orders from deleting store-----

$delete_query = "DELETE FROM local_store WHERE id ='$id'";
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