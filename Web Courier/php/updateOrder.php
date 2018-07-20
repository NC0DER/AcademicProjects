<?php
session_start();

//Sanitizing the entire POST array from html, input
$_POST  = filter_input_array(INPUT_POST, FILTER_SANITIZE_STRING);

$text = isset($_POST['track_num']) ? $_POST['track_num'] : null;

if($text === null){
    die("The ajax response from readQR.js is missing:");
}

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

//Ensuring that the encoding of the string is set to UTF-8, and eliminating escape characters
$utf8_str = $text;
iconv(mb_detect_encoding($utf8_str, mb_detect_order(), true), "UTF-8",$utf8_str );
$text = $connection -> real_escape_string($utf8_str);

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
    $connection->close();
    exit();
}

//Find the path of the web order scanned by the QR
$find_order = "SELECT current_point,route FROM web_order WHERE tracking_num ='$text'";
$returned = $connection -> query($find_order);
$result = $connection -> affected_rows;
if ($result === 1){ //Username is unique only one column is returned
    $row = $returned -> fetch_array(MYSQLI_ASSOC);
    $current = $row['current_point'];
    $path = mb_split(",",$row['route']);
    $new_current = "";

    //If current doesn't match this store
    //then the package belongs to a different store
    if($current !== $store){
        echo "WRONG_STORE";
        $connection->close();
        exit();
    }
    $length = count($path);

    if($current === $path[$length-2]){
        //echo "ELSEYYYIFFFYYY";
        //Check if the before last entry is store or transit
        $check = $path[$length-1];
        $find_store = "SELECT name FROM local_store WHERE name ='$check'";
        $returned = $connection -> query($find_store);
        $result = $connection -> affected_rows;
        if ($result === 0){ //it's transit
            $new_current = $path[$length-1]; //equal to last
        }else if($result < 0){ //it's error
            echo "ERROR";
            $connection->close();
            exit();
        }
    }
    else{
        //echo "ELSEYYY";
        for($i = 0; $i < $length; ++$i){
            if(($path[$i] === $current) && ($path[$i] === $store)){
                $new_current = $path[$i+1];
                break;
            }
        }
    }
    //If new current is empty or new current is old current
    //then it can't be forwarded
    if(($new_current !== '') && ($new_current !== $current)){
        //Updating web_order with the value of  $new_current
        $update_current = "UPDATE web_order SET current_point ='$new_current' WHERE tracking_num ='$text'";
        $returned = $connection -> query($update_current);
        $result = $connection -> affected_rows;
        if($result === 1){
            echo "FORWARD_TO_TRANSIT";
        }
        else{
            echo "UPDATE_ERROR";
        }
    }else{ //new_current = old_current;
        echo "FORWARD_TO_LOCAL_STORE";
        //var_dump($path,$current);
        $connection->close();
        exit();
    }
//var_dump($path,$new_current,$current);
}else if($result === 0){
    echo "NO_ORDER";
    $connection->close();
    exit();
}else{
    echo "ERROR";
    $connection->close();
    exit();
}
$connection->close();
?>