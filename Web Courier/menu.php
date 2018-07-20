<?php
session_start();
header("Cache-Control: no-store, no-cache, must-revalidate, max-age=0");
header("Expires: Sat, 26 Jul 1997 05:00:00 GMT");

if (isset($_POST['user'], $_POST['pwd'])) {
    //Prevent XSS on POST
    $_POST  = filter_input_array(INPUT_POST, FILTER_SANITIZE_STRING);

    //Trimming extra spaces,tab,nul chars at start and end of string (NOT IN BETWEEN!)
    //Caused by incompatible copy paste or a keyboard accident
    $_POST = array_map('trim', $_POST);

    $user = $_POST['user'];
    $pwd = $_POST['pwd'];

    //Prevent empty login
    if($user === "" || $pwd === ""){
        header('Location: http://localhost/webcourier/', true); // Redirect browser
        exit();
    }

    $my_server = "localhost";
    $my_admin = "root";
    $my_pass = "";
    $my_db = "web_courier";

    //Connecting to sql server, in case this fails an error is being displayed and then exit().
    $connection = new mysqli($my_server, $my_admin, $my_pass, $my_db);

    if ($connection->connect_error) {
        die("MySQL Connection at the server failed: " . $connection->connect_error);
    }
    //Setting utf8 php-side for this connection, (it is already set in the db)
    $connection->query('SET CHARACTER SET utf8');
    $connection->query('SET COLLATION_CONNECTION=utf8_general_ci');

    //Hash the password.  $hashedPassword will be a 60-character string including random salt,
    //At future PHP version it might reach up to a maximum of 255 characters
    //Default algorithm bcrypt resistant to SHA-1/MD5 weaknesses;
    //$hashedpass = password_hash($pwd, PASSWORD_DEFAULT);

     //Formatting form strings for the sql query below
    $user = $connection->real_escape_string($_POST['user']);
    $pwd = $connection->real_escape_string($_POST['pwd']);

     //Searching users table
    $login_query = "SELECT uname,passw,role FROM users WHERE uname = '" . $user . "'";

    $returned = $connection->query($login_query);
    $row = $returned->fetch_array(MYSQLI_ASSOC);

    //Check if a user has provided the correct password
    //by comparing what they typed with our db hashed password
    $verified = password_verify($pwd,$row['passw']); // false = verification failed
    if ((strcmp($row['uname'], $user) === 0) && $verified) {
        //Succesful login, Set session
        $_SESSION['session_username'] = $user;
        $_SESSION['session_role'] = $row['role'];
        //var_dump( $_SESSION['session_username']);

        //User already log-in'ed
        if (isset($_SESSION['session_role']) && ($_SESSION['session_role'] === 'admin')) {
            include('admin_menu.php');
        }else if(isset($_SESSION['session_role']) && ($_SESSION['session_role'] === 'store_employee')){
            include('store_menu.php');
        }else if(isset($_SESSION['session_role']) && ($_SESSION['session_role'] === 'transit_employee')){
            include('transit_menu.php');
        }else{
            header('Location: http://localhost/webcourier/', true); // Redirect browser
        }
    } else {
        echo "<!DOCTYPE html>
                   <html lang=\"en\">
                    <head>
                        <meta charset=\"utf-8\">
                        <meta http-equiv=\"X-UA-Compatible\" content=\"IE=edge\">
                        <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, maximum-scale=1.0, user-scalable=no\"/>
                        <meta name=\"description\" content=\"\">
                        <meta name=\"author\" content=\"\">

                        <title>Web Courier</title>

                        <!-- Bootstrap Core CSS -->
                        <link href=\"css/bootstrap.min.css\" rel=\"stylesheet\">
                        <!-- Custom CSS -->
                        <link href=\"css/mycustom.css\" rel=\"stylesheet\">
                    </head>

                    <body>";
        echo "<!-- Navigation -->
                <nav class=\"navbar navbar-inverse navbar-fixed-top\" role=\"navigation\">
                    <div class=\"container\">
                        <!-- Brand and toggle get grouped for better mobile display -->
                        <div class=\"navbar-header\">
                        <button type=\"button\" class=\"navbar-toggle\" data-toggle=\"collapse\" data-target=\"#bs-example-navbar-collapse-1\">
                        <span class=\"sr-only\">Toggle navigation</span>
                        <span class=\"icon-bar\"></span>
                        <span class=\"icon-bar\"></span>
                        <span class=\"icon-bar\"></span>
                        </button>
                    </div>
                <!-- Collect the nav links, forms, and other content for toggling -->
                <div class=\"collapse navbar-collapse\" id=\"bs-example-navbar-collapse-1\"></div>
                <!-- /.navbar-collapse -->
                </div>
                <!-- /.container -->
                </nav>";
        echo "<br><br><br><br><br>";
        echo "<h2 style ='text-align: center; color: #DC143C'>Το όνομα χρήστη/συνθηματικό που δώσατε ήταν εσφαλμένα!</h2>";
        echo "<a href=\"http://localhost/webcourier/\"><h3 style ='text-align: center'>Κλικάρετε εδώ για να επιστρέψετε στην αρχική σελίδα</h3></a>";
        echo "<footer class=\"container-fluid footer navbar-fixed-bottom\">
                    <p id='par'>Created with Bootstrap</p>
                  </footer>
                  </body>
                  </html>";
    }
//Handle if the user is already login-ed and redirect him to the menu
}else if(isset($_SESSION['session_role'])) {
    //User already log-in'ed
    if (isset($_SESSION['session_role']) && ($_SESSION['session_role'] === 'admin')) {
        include('admin_menu.php');
    } else if (isset($_SESSION['session_role']) && ($_SESSION['session_role'] === 'store_employee')) {
        include('store_menu.php');
    } else if (isset($_SESSION['session_role']) && ($_SESSION['session_role'] === 'transit_employee')) {
        include('transit_menu.php');
    }
}
else {
    header('Location: http://localhost/webcourier/', true); // Redirect browser
}
?>