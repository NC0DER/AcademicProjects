<?php
//If not correct type of user redirect back to login handler -> menu.php
if (!(isset($_SESSION['session_role']) && ($_SESSION['session_role'] === 'store_employee'))) {
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
<body>
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
            <a class="navbar-brand" href="#home">Store Menu</a>
        </div>
        <!-- Collect the nav links, forms, and other content for toggling -->
        <div class="collapse navbar-collapse" id="bs-example-navbar-collapse-1">
            <ul class="nav navbar-nav navbar-center">
                <li>
                    <a> </a>
                </li>
                <li>
                    <a href="#table">Διαχείριση</a>
                </li>
                <li>
                    <a href="#myMap">Παρακολούθηση</a>
                </li>
            </ul>
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
<br><br><br id = "table">
<?php
echo "<br><br><br>";
?>
<div class ="container">
    <div class="modal fade" id="modal1" role="dialog">
        <div class="modal-dialog modal-sm">
            <!-- Modal content-->
            <div class="modal-content">
                <div class="modal-header">
                    <button type="button" class="close" data-dismiss="modal">&times;</button>
                    <h4 class="modal-title">Δημιουργία Νέας Αποστολής</h4>
                </div>
                <div class="modal-body">
                    <form class="form-horizontal">
                        <div class="form-group">
                            <label class="col-sm-4 control-label">Από:  </label>
                            <div class ="col-sm-8">
                                <div class="form-group">
                                    <select class="form-text" id="sel1">
                                    </select>
                                </div>
                            </div>
                        </div>
                        <div class="form-group">
                            <label class="col-sm-4 control-label">Πρός:  </label>
                            <div class ="col-sm-8">
                                <div class="form-group">
                                    <select class="form-text" id="sel2">
                                    </select>
                                </div>
                            </div>
                        </div>
                        <div class="form-group">
                            <label class="col-sm-4 control-label">Τύπος:  </label>
                            <div class ="col-sm-8">
                                <div class="form-group">
                                    <select class="form-text" id="sel3">
                                        <option>standard</option>
                                        <option>express</option>
                                    </select>
                                </div>
                            </div>
                        </div>
                    </form>
                </div>
                <div class="modal-footer">
                    <button type="button" class="btn btn-default modal-button" id="submit1">Υποβολή</button>
                </div>
            </div>
        </div>
    </div>
    <div class="modal fade" id="modalqr" role="dialog">
        <div class="modal-dialog modal-sm">
            <!-- Modal content-->
            <div class="modal-content">
                <div class="modal-header">
                    <button type="button" class="close" data-dismiss="modal">&times;</button>
                    <h4 class="modal-title">QRCode Αποστολής</h4>
                </div>
                <div class="modal-body" id ="qr-container">
                    <img id="qr" src="" class="img-responsive"/>
                </div>
            </div>
        </div>
    </div>
    <div class="row" id = "b">
        <div class="col-md-1">
            <div></div>
        </div>
        <div class="col-md-10">
            <h3>
                <span>Διαχείριση Αποστολής</span>
                <button type="button" class="btn-danger btn-lg pull-right btn-primary-spacing" id="delete1"><em class="glyphicon glyphicon-remove"></em></button>
                <button type="button" class="btn-primary btn-lg pull-right btn-primary-spacing" data-toggle="modal" data-target="#modal1" id="add1"><em class="glyphicon glyphicon-plus"></em></button>
                <button type="button" class="btn-success btn-lg pull-right btn-primary-spacing" id="deliver"><em class="glyphicon glyphicon-ok"></em></button>
            </h3><br>
            <div class = "table-responsive ">
                <table class="table table-striped table-hover table-bordered table-condensed">
                    <thead>
                    <tr>
                        <th class ="text-center">Tracking Number</th>
                        <th class ="text-center">Τύπος</th>
                        <th class ="text-center">Τρέχουσα Τοποθεσία</th>
                        <th class ="text-center">Διαδρομή</th>
                        <th class ="text-center">Κόστος</th>
                        <th class ="text-center">Χρόνος παράδοσης</th>
                        <th class ="text-center"><em class="glyphicon glyphicon-record"></em></th>
                    </tr>
                    </thead>
                    <tbody class = "text-center" id = "x"></tbody>
                </table>
            </div>
        </div>
        <div class="col-md-1">
            <div></div>
        </div>
    </div>
    <div class ="row">
        <div class="col-md-1">
            <div></div>
        </div>
        <div class = "col-md-10" id ="space">
            <h3>
                <span>Παρακολούθηση Αποστολής</span>
                <button type="button" class="btn-primary btn-lg pull-right btn-primary-spacing" id="track"><em class="	glyphicon glyphicon-pushpin"></em></button>
            </h3><br>
            <div id="myMap" style="width:100%;height:720px;"></div>
        </div>
        <div class="col-md-1">
            <div></div>
        </div>
    </div>
    <div class = "row">
        <p id ="block"> </p>
    </div>
</div>
<footer class="container-fluid footer navbar-fixed-bottom">
    <p id='par'>Created with Bootstrap</p>
</footer>

<!-- jQuery -->

<script src="js/jquery.js"></script>
<!-- Bootstrap Core JavaScript -->
<script src="js/bootstrap.min.js"></script>

<script src="js/manageOrder.js"></script>
<script src="js/GoogleMap.js"></script>
<!-- Google Maps API with licensed key -->
<script async defer src="https://maps.googleapis.com/maps/api/js?key=AIzaSyAToH3-BnPjDxUDM5ZlwPRaLw0ccc0Eko0&callback=myMap"></script>
</body>
</html>
