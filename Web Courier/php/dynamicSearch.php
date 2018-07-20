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
//Sanitizing the entire POST array from html, input
$_POST  = filter_input_array(INPUT_POST, FILTER_SANITIZE_STRING);

//Checking the integrity of the response
$search = isset($_POST['search']) ? $_POST['search'] : null;

if($search === ""){ //Empty form
    echo ""; //No suggestions
    exit();
}

//Setting utf8 php-side for this connection, (it is already set in the db)
$connection -> query ('SET CHARACTER SET utf8');
$connection -> query ('SET COLLATION_CONNECTION=utf8_general_ci');

//Getting all stores/transit hubs
//$select = "SELECT name,city FROM transit_hub UNION ALL SELECT name,city FROM local_store ORDER BY name,city";

//Getting all stores
$select = "SELECT name,city FROM local_store ORDER BY name,city";

$returned = $connection -> query($select);

//Create object with cast from array - one liner no class definition
$object = (object) array('name' => '','city' => '');

$json_str = ""; //Initialize and convert string to UTF-8
iconv(mb_detect_encoding($json_str, mb_detect_order(), true), "UTF-8", $json_str);

$diacritic = array(
    'ά'=>'α', 'έ'=>'ε', 'ή'=>'η',
    'ί'=>'ι', 'ύ'=>'υ', 'ό'=>'ο',
    'ώ'=>'ω','ΰ'=>'υ','ΐ'=>'ι'
);

//Fetching the required columns row-by-row from the query
while($row = $returned -> fetch_array(MYSQLI_ASSOC)) {
    //If the search matches as a substring,
    //we append the object to the json response
    //Search with no diacritic
    $pos = strpos(strtr($row['name'], $diacritic), $search);
    if($pos === false) //If name search failed then search city
        $pos = strpos(strtr($row['city'], $diacritic), $search);
    if ($pos !== false) { //Found substr
        //Replace diacritic char with non-diacritic char
        $object -> name = $row['name'];
        $object -> city = $row['city']; //Return strings with diacritic
        $json_str .= json_encode($object,JSON_UNESCAPED_UNICODE);
        $json_str .=";";
    }
}
if($json_str !== "") //If not empty, then we remove the last ';'
    $json_str = mb_substr($json_str,0,-1); //Remove last ';' from string

echo $json_str; //Either empty (no suggestions) or has some json objects (suggestions)
$connection->close();
?>