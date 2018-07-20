<?php
//If not correct type of user redirect back to login handler -> menu.php
if (!(isset($_SESSION['session_role']) && ($_SESSION['session_role'] === 'transit_employee'))) {
    header('Location: http://localhost/webcourier/menu.php', true); // Redirect browser
    exit();
}
?>
<!DOCTYPE html>
<html>
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
<body onload="load()">
<!-- Navigation -->
<nav class="navbar navbar-inverse navbar-fixed-top" role="navigation">
    <div class="container">
        <!-- Brand and toggle get grouped for better mobile display -->
        <div class="navbar-header">
            <button type="button" class="navbar-toggle" data-toggle="collapse" data-target="#bs-example-navbar-collapse-1">
                <span class="sr-only">Toggle navigation</span>
                <span class="icon-bar"></span>
                <span class="icon-bar"></span>
                <span class="icon-bar"></span>
            </button>
            <a class="navbar-brand" href="#">Transit Hub Menu</a>
        </div>
        <!-- Collect the nav links, forms, and other content for toggling -->
        <div class="collapse navbar-collapse" id="bs-example-navbar-collapse-1">
            <ul class="nav navbar-nav navbar-right">
                <li style>
                    <?php
                    echo "<a id='user'>".$_SESSION['session_username']." "."<em class=\"glyphicon glyphicon-user\"></em></a>";
                    ?>
                </li>
                <li>
                    <a href="logout.php">Αποσύνδεση <em class="glyphicon glyphicon-log-out"></em></a>
                <li>
            </ul>
        </div>
        <!-- /.navbar-collapse -->
    </div>
    <!-- /.container -->
</nav>

<div class="jumbotron" id = "home">
    <h1>Web Courier</h1>
</div>
<br>
<?php
echo " ";
?>
<div class ="container">
    <div class="row">

        <div class="col-md-4 col-md-offset-4">
            <div class = "row">
                <div class = "col-md-12">
                    <h3>
                        <span>Σάρωση Αποστολής</span>
                        <button type="button" class="btn-default btn-lg" id="video"><em class="glyphicon glyphicon-facetime-video"></em></button>
                    </h3>
                </div>
            </div>
            <br>
            <div class = "row">
                <div class = "col-md-12">
                    <canvas id="qr-canvas" width="640" height="480" style="display:none"></canvas>
                    <div id="outdiv"></div>
                    <div id="result">- scanning -</div>
                </div>
            </div>
        </div>

    </div>
</div>
<footer class="container-fluid footer navbar-fixed-bottom">
    <p id='par'>Created with Bootstrap</p>
</footer>

<!-- jQuery -->

<script src="js/jquery.js"></script>
<!-- Bootstrap Core JavaScript -->
<script src="js/bootstrap.min.js"></script>
<script src="js/llqrcode.js"></script>
<script src="js/webqr.js"></script>
<script src="js/readQR.js"></script>

</body>
</html>
