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
$tid = "";
$transit = "";

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

//Find which the transit id this store is linked to
$selectTransit = "SELECT transit_id FROM local_store WHERE name ='$store'";
$returned = $connection -> query($selectTransit);
$result = $connection -> affected_rows;
if ($result === 1){ //Transit_id is unique only one column is returned
    $row = $returned -> fetch_array(MYSQLI_ASSOC);
    $tid = $row['transit_id'];
}else{
    echo "ERROR";
    exit();
}

//Find the name of the transit hub using the above id
$selectTransitName = "SELECT name FROM transit_hub WHERE id ='$tid'";
$returned = $connection -> query($selectTransitName);
$result = $connection -> affected_rows;
if ($result === 1){ //Transit_id is unique only one column is returned
    $row = $returned -> fetch_array(MYSQLI_ASSOC);
    $transit = $row['name'];
}else{
    echo "ERROR";
    exit();
}

$json_str = ""; //Initialize and convert string to UTF-8
iconv(mb_detect_encoding($json_str, mb_detect_order(), true), "UTF-8", $json_str);
$object = (object) array('tracking_num' => '');

$selectdelivered = "SELECT tracking_num,current_point,route FROM web_order WHERE current_point='$transit'";
//var_dump($username,$store,$tid,$transit);
$returned = $connection -> query($selectdelivered);
$result = $connection -> affected_rows;
if ($result >= 1){ //One or more orders with current_point this stores linked transit
    while($row = $returned -> fetch_array(MYSQLI_ASSOC)) {
        $path = mb_split(",",$row['route']);
        //From local store to Transit
        if((count($path) === 2) && ($path[0] === $store) && ($path[1] === $transit)){
            $object ->tracking_num = $row['tracking_num'];
            $json_str .= json_encode($object,JSON_UNESCAPED_UNICODE);
            $json_str .=";";

            $tracking_num = $row['tracking_num'];
            $deletedelivered = "DELETE FROM web_order WHERE tracking_num ='$tracking_num'";
            $connection -> query($deletedelivered);
        }
        //Delivered order = last entry on route -> this store && current point -> the linked transit
        if($path[count($path)-1] === $store){
            $object ->tracking_num = $row['tracking_num'];
            $json_str .= json_encode($object,JSON_UNESCAPED_UNICODE);
            $json_str .=";";

            $tracking_num = $row['tracking_num'];
            $deletedelivered = "DELETE FROM web_order WHERE tracking_num ='$tracking_num'";
            $connection -> query($deletedelivered);
        }
    }//If the order doesn't have this transit as last entry of route - its disregarded -> nothing executed for it
    //Because in that case this transit is an intermediate and not the end point
    if($json_str === ""){
        echo "NO_ORDERS";
        exit();
    }
    else{
        $json_str = mb_substr($json_str,0,-1); //Remove last ';' from string
        echo $json_str;
    }
}else if($result === 0){
    echo "NO_ORDERS";
}else{
    echo "ERROR";
    exit();
}
?>