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
            <a class="navbar-brand" href="#home">Client Menu</a>
        </div>
        <!-- Collect the nav links, forms, and other content for toggling -->
        <div class="collapse navbar-collapse" id="bs-example-navbar-collapse-1">
            <ul class="nav navbar-nav navbar-center">
                <li>
                    <a> </a>
                </li>
                <li>
                    <a href="#a">Παρακολούθηση Αποστολής</a>
                </li>
                <li>
                    <a href="#b">Δίκτυο Καταστημάτων</a>
                </li>
            </ul>
        </div>
        <!-- /.navbar-collapse -->
    </div>
    <!-- /.container -->
</nav>

<div class="jumbotron" id = "home">
    <h1>Web Courier</h1>
</div>
<?php
echo "";
?>
<div class ="container">
    <div class="row" id ="a">
        <div class="col-md-1">
            <div></div>
        </div>
        <div class="col-md-10">
            <div class="row">
                <div class="col-xs-6">
                    <h4>Παρακολούθηση Αποστολής</h4>
                </div>
                <div class="col-xs-6" id="col-xs-pull-1">
                    <div class="input-group">
                        <input type="text" class="form-control" id="search-form2" placeholder="Κώδ.Παραγγελίας" name="search2">
                        <div class="input-group-btn">
                            <button class="btn-primary btn-lg pull-right btn-primary-spacing" id="search2" ><em class="glyphicon glyphicon-search"></em></button>
                        </div>
                    </div>
                </div>
            </div>
            <br>
            <div class ="col-md-12">
                <div id="myMap2" style="width:100%;height:350px;"></div> <!-- Good Size for mobile -->
            </div>
        </div>
        <div class="col-md-1">
            <div></div>
        </div>
    </div>
    <div class ="row" id="b">
        <div class="col-md-1">
            <div></div>
        </div>
        <div class="col-md-10" id ="space">
            <div class="row">
                <div class="col-xs-6">
                    <h4>Δικτύο Καταστημάτων</h4>
                </div>
                <div class="col-xs-6" id="col-xs-pull-2">
                    <div class="input-group">
                        <input type="text" class="form-control" id="search-form" placeholder="ΤΚ ή Πόλη" name="search">
                        <div class="input-group-btn">
                            <button class="btn-primary btn-lg pull-right btn-primary-spacing" id="search" ><em class="glyphicon glyphicon-search"></em></button>
                        </div>
                    </div>
                    <div id ="suggestion" style="visibility: hidden"></div> <!--By default suggestion box is hidden-->
                </div>
            </div>
            <br>
            <div class ="col-md-12">
                <div id="myMap" style="width:100%;height:350px;"></div> <!-- Good Size for mobile -->
            </div>
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

<script src="js/manageClient.js"></script>
<!-- Google Maps API with licensed key -->
<script async defer src="https://maps.googleapis.com/maps/api/js?key=AIzaSyAToH3-BnPjDxUDM5ZlwPRaLw0ccc0Eko0&callback=myMap"></script>
</body>
</html>
