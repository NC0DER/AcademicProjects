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

//Sanitizing the entire POST array from html, input
$_POST  = filter_input_array(INPUT_POST, FILTER_SANITIZE_STRING);

$from = isset($_POST['from']) ? $_POST['from'] : null;
$to = isset($_POST['to']) ? $_POST['to'] : null;
$type = isset($_POST['typeof']) ? $_POST['typeof'] : null;

if($from === null || $to === null || $type === null){
    die("The ajax response from manageOrder.js is missing:");
}
//$from = 'Thessaloniki';
//$to = 'Heraklion';
//$type = 'express'; //standard -> least cost , express -> least time

$cost = 0; //in euros
$eta = 0; //in hours
$from_tid = 0;
$to_tid = 0;
$from_transit = "";
$to_transit = "";

//Extract 2 first letters, uppercase them, concatenate them with the number of seconds passed from unix epoch
$tracking_num = mb_substr($from,'0','2',mb_detect_encoding($from, mb_detect_order(), true));
$tracking_num = mb_convert_case($tracking_num, MB_CASE_UPPER, mb_detect_encoding($tracking_num, mb_detect_order(), true));
$tracking_num .= time();
$str = mb_substr($to,'0','2',mb_detect_encoding($to, mb_detect_order(), true));
$tracking_num .= mb_convert_case($str, MB_CASE_UPPER, mb_detect_encoding($str, mb_detect_order(), true));
//echo $tracking_num;

$utf8_str = $from;
iconv(mb_detect_encoding($utf8_str, mb_detect_order(), true), "UTF-8",$utf8_str );
$from = $connection -> real_escape_string($utf8_str);

$utf8_str = $to;
iconv(mb_detect_encoding($utf8_str, mb_detect_order(), true), "UTF-8",$utf8_str );
$to = $connection -> real_escape_string($utf8_str);


$is_from_local_store = "SELECT transit_id FROM local_store WHERE name ='$from'";
$is_to_local_store = "SELECT transit_id FROM local_store WHERE name ='$to'";

//var_dump($is_from_local_store,$is_to_local_store);
//Checking if from is transit or local store
$returned = $connection -> query($is_from_local_store);
$result = $connection -> affected_rows;
if ($result === 1){
    //echo "from -> local\n";
    $row1 = $returned ->fetch_array(MYSQL_ASSOC);
    $from_tid = (int)$row1['transit_id'];
    //var_dump($from_tid);
    $from_nearest_transit = "SELECT name FROM transit_hub WHERE id ='$from_tid'";
    $returned = $connection -> query($from_nearest_transit);
    $row2 = $returned ->fetch_array(MYSQL_ASSOC);
    $from_transit = $row2['name'];
    //var_dump($from_transit);
    ++$cost;
}else if ($result === 0){
    //echo "from -> transit\n";
    $from_transit = $from;
    $from_nearest_transit = "SELECT id FROM transit_hub WHERE name ='$from_transit'";
    $returned = $connection -> query($from_nearest_transit);
    $row = $returned ->fetch_array(MYSQL_ASSOC);
    $from_tid = (int)$row['id'];
    //var_dump($from_transit,$from_tid);
}
else if ($result === -1){
    //echo "Error\n";
    $connection->close();
    exit();
}

//Checking if to is transit or local store
$returned = $connection -> query($is_to_local_store);
$result = $connection -> affected_rows;
if ($result === 1){
    //echo "to -> local\n";
    $row1 = $returned ->fetch_array(MYSQL_ASSOC);
    $to_tid = (int)$row1['transit_id'];
    $to_nearest_transit="SELECT name FROM transit_hub WHERE id ='$to_tid'";
    //var_dump($to_tid);
    $returned = $connection -> query($to_nearest_transit);
    $row2 = $returned ->fetch_array(MYSQL_ASSOC);
    $to_transit = $row2['name'];
    //var_dump($to_transit);
    ++$cost;
}else if ($result === 0){
    //echo "to -> transit\n";
    $to_transit = $to;
    $to_nearest_transit = "SELECT id FROM transit_hub WHERE name ='$to_transit'";
    $returned = $connection -> query($to_nearest_transit);
    $row = $returned ->fetch_array(MYSQL_ASSOC);
    $to_tid = (int)$row['id'];
    //var_dump($to_transit,$to_tid);
}
else if ($result === -1){
    echo "Error";
    $connection->close();
    exit();
}
//echo "Starter cost is: ".$cost;

$map = array(array()); //Declaring primary cost 2D array
$sec_map = array(array()); //Declaring secondary cost 2D array
//var_dump($map);

$connects_query ="SELECT hub_a,hub_b,cost_ab,eta_ab FROM connects";
$returned = $connection -> query($connects_query);

if($type === 'standard'){
    while($row = $returned -> fetch_array(MYSQLI_ASSOC)) {
        //var_dump($row); //Debug Line 4
        $map[$row['hub_a']][$row['hub_b']] = (int)$row['cost_ab'];
        $sec_map[$row['hub_a']][$row['hub_b']] = (int)$row['eta_ab'];
    }
}else{ //express
    while($row = $returned -> fetch_array(MYSQLI_ASSOC)) {
        //var_dump($row); //Debug Line 4
        $map[$row['hub_a']][$row['hub_b']] = (int)$row['eta_ab'];
        $sec_map[$row['hub_a']][$row['hub_b']] = (int)$row['cost_ab'];
    }
}

//var_dump($map);

/**
 *  Dijkstra's algorithm in PHP by zairwolf
 *
 *  Demo: http://www.you4be.com/dijkstra_algorithm.php
 *
 *  Source: https://github.com/zairwolf/Algorithms/blob/master/dijkstra_algorithm.php
 *
 *  Author: Hai Zheng @ https://www.linkedin.com/in/zairwolf/
 *
 */

//Initialiting Start, End points
$start = $from_tid;
$end = $to_tid;

//Array Initialization
$Shortest = array(); //the nearest path from target to source
$Queue = array(); //the remaining nodes without including the nearest path
foreach(array_keys($map) as $val) $Queue[$val] = 99999;
$Queue[$start] = 0;

//Begin Search
while(!empty($Queue)){
    $min = array_search(min($Queue), $Queue);
    if($min == $end) break;
    foreach($map[$min] as $key=>$val)
        if(!empty($Queue[$key]) && $Queue[$min] + $val < $Queue[$key]) {
            $Queue[$key] = $Queue[$min] + $val;
            $Shortest[$key] = array($min, $Queue[$key]);
    }
    unset($Queue[$min]);
}

//Finalizing the shortest path in an array
$path = array();
$pos = $end;
while($pos != $start){
    $path[] = $pos;
    $pos = $Shortest[$pos][0];
}
$path[] = $start;
$path = array_reverse($path);

if($type === 'standard'){
    $length = count($path) - 1;
    for($i = 0; $i < $length; ++$i){ //[1,2,3] path converts to {(1,2),(2,3)} here
        $cost += $map[$path[$i]][$path[$i+1]]; //+= to include the starting cost/eta from local store(s) to transit
        $eta += $sec_map[$path[$i]][$path[$i+1]]; //Secondary map holds eta here
    }
}else{ //express
    $length = count($path) - 1 ;
    for($i = 0; $i < $length; ++$i){
        $eta += $map[$path[$i]][$path[$i+1]]; //+= to include the starting eta/cost from local store(s) to transit
        $cost += $sec_map[$path[$i]][$path[$i+1]]; //Secondary map holds cost here
    }
}

//Constructing the path string (comma-separated) with transit/store names instead of ids
$path_str = ""; //Initialize and convert string to UTF-8
iconv(mb_detect_encoding($path_str, mb_detect_order(), true), "UTF-8", $path_str);


if($from_transit !== $from){
    $path_str .= $from . ",";
}

foreach($path as $i){
    $id_to_name = "SELECT name FROM transit_hub WHERE id='$i'";
    $returned = $connection -> query($id_to_name);
    $row = $returned -> fetch_array(MYSQLI_ASSOC);
    $path_str .= $row['name'] .",";
}

if($to_transit !== $to){
    $path_str  .= $to . ",";
}
$path_str = mb_substr($path_str,0,-1); //Remove last ',' from string
//var_dump($path_str);

$insert_query = "INSERT INTO web_order (tracking_num, typeof, current_point, route, cost, eta) VALUES ('$tracking_num', '$type', '$from_transit', '$path_str', '$cost', '$eta')";
$connection -> query($insert_query);
//Fetching the result of the insert query
$result = 0;
$result = $connection -> affected_rows;
if ($result !== 1){
    echo "ERROR";
    $connection->close();
    exit();
}
//If the insertion was successful, we create and send the json object
$object = (object) array(
    'tracking_num' => $tracking_num, 'typeof' => $type,
    'current_point' => $from_transit, 'route' => $path_str,
    'cost' => $cost, 'eta' => $eta
);

$json_str = json_encode($object,JSON_UNESCAPED_UNICODE);
echo $json_str;
?>