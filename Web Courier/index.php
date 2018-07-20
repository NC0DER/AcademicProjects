<?php
session_start();
header("Cache-Control: no-store, no-cache, must-revalidate, max-age=0");
header("Expires: Sat, 26 Jul 1997 05:00:00 GMT");
//If already login-ed go back to your menu.php
if(isset($_SESSION['session_role'])){
    header('Location: http://localhost/webcourier/menu.php', true); // Redirect browser
    exit();
}
?>
<!DOCTYPE html>
<html lang="en">

<head>
    <meta charset="utf-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1.0, maximum-scale=1.0, user-scalable=no"/>
    <meta name="description" content="">
    <meta name="author" content="">

    <title>Web Courier</title>

    <!-- Bootstrap Core CSS -->
    <link href="css/bootstrap.min.css" rel="stylesheet">
    <!-- Custom CSS -->
    <link href="css/mycustom.css" rel="stylesheet">
</head>

<body>
    <!-- Navigation -->
    <nav class="navbar navbar-inverse navbar-fixed-top" role="navigation">
        <div class="container">
            <div class="navbar-header"></div>
        </div>
        <!-- /.container -->
    </nav>

   <div class="jumbotron">
        <h1>Web Courier</h1>
   </div>

   <!-- Page Content -->
   <div class="container">

        <!-- Marketing Icons Section -->
        <div class="row">
            <div class="col-md-4">
                <div></div>
            </div>
            <div class="col-md-4">
                <div class="alert alert-info alert-dismissable">
                    <a href="#" class="close" data-dismiss="alert" aria-label="close">×</a>
                    Είστε πελάτης?&nbsp;
                    <a href="client.php" class="alert-link">Κάντε Κλίκ εδώ</a>
                </div>
            </div>
            <div class="col-md-4">
                <div></div>
            </div>
        </div>
        <div class="row">
            <div class="col-md-4">
                <div></div>
            </div>
            <div class="col-md-4">
                <div class="panel panel-default">
                <div class="panel-body">
                    Εξακρίβωση Χρήστη
                </div>
                <form method="POST" action="menu.php">
                <div class="input-group">
                    <span class="input-group-addon"><i class="glyphicon glyphicon-user"></i></span>
                    <input id="email" type="text" class="form-control" name="user" placeholder="Όνομα Χρήστη">
                </div>
                <div class="input-group">
                    <span class="input-group-addon"><i class="glyphicon glyphicon-lock"></i></span>
                    <input id="password" type="password" class="form-control" name="pwd" placeholder="Συνθηματικό">
                </div>
                <button type="submit" class="btn btn-default" id="login">Login</button>
                </form>
                </div>
            </div>
            <div class="col-md-4"></div>
        </div>
        <div class="col-lg-12">
            <h1 class="page-header text-left">
            </h1>
        </div>
   <footer class="container-fluid footer navbar-fixed-bottom">
        <p id='par'>Created with Bootstrap</p>
   </footer>
   <!-- jQuery -->
   <script src="js/jquery.js"></script>

   <!-- Bootstrap Core JavaScript -->
   <script src="js/bootstrap.min.js"></script>
</body>
</html>
