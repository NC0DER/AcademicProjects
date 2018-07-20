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
$uname = isset($_POST['user_name']) ? $_POST['user_name'] : null;
$upass = isset($_POST['user_pass']) ? $_POST['user_pass'] : null;
$urole = isset($_POST['user_role']) ? $_POST['user_role'] : null;
$ustore = isset($_POST['user_store']) ? $_POST['user_store'] : null;

//var_dump($uname,$upass,$urole,$ustore);

if($uname === null || $upass === null || $urole === null || $ustore === null){
    die("The ajax response from manageUsers.js is missing:");
}

// Hash the password.  $hashedPassword will be a 60-character string including random salt,
// At future PHP version it might reach up to a maxixum of 255 characters
// Default algorithm bcrypt resistant to SHA-1/MD5 weaknesses;
$hashedpass = password_hash($upass, PASSWORD_DEFAULT);

//Ensuring that the encoding of the string is set to UTF-8, and eliminating escape characters
$utf8_str = $uname;
iconv(mb_detect_encoding($utf8_str, mb_detect_order(), true), "UTF-8",$utf8_str );
$username = $connection -> real_escape_string($utf8_str);

$utf8_str = $hashedpass;
iconv(mb_detect_encoding($utf8_str, mb_detect_order(), true), "UTF-8",$utf8_str );
$password = $connection -> real_escape_string($utf8_str);

$utf8_str = $urole;
iconv(mb_detect_encoding($utf8_str, mb_detect_order(), true), "UTF-8",$utf8_str );
$role = $connection -> real_escape_string($utf8_str);

$utf8_str = $ustore;
iconv(mb_detect_encoding($utf8_str, mb_detect_order(), true), "UTF-8",$utf8_str );
$store = $connection -> real_escape_string($utf8_str);

//var_dump($uname,$upass,$urole,$ustore);

//Check to see if user already exists
$exists_query = "SELECT uname FROM users WHERE uname ='$username'";
$update_query = "UPDATE users SET uname = '$username', passw = '$password', role = '$role', store = '$store' WHERE uname = '$username'";

//var_dump($exists_query,$update_query);

$connection -> query($exists_query);

//Fetching the result of the select query
$result = $connection -> affected_rows;
if ($result === 0){
    echo "NOT_EXISTS";
    $connection->close();
    exit();
}else{
    $connection -> query($update_query);
    //Fetching the result of the insert query
    $result = 0;
    $result = $connection -> affected_rows;

    if ($result === 1)
        echo "OK";
    else if ($result === 0)
        echo "FAIL";
    else echo "ERROR";
}
$connection->close();
?>