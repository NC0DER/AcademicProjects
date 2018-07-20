<?php
//If not correct type of user redirect back to login handler -> menu.php
if (!(isset($_SESSION['session_role']) && ($_SESSION['session_role'] === 'admin'))){
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
            <a class="navbar-brand" href="#home">Admin Menu</a>
        </div>
        <!-- Collect the nav links, forms, and other content for toggling -->
        <div class="collapse navbar-collapse" id="bs-example-navbar-collapse-1">
            <ul class="nav navbar-nav navbar-center">
                <li>
                    <a> </a>
                </li>
                <li>
                    <a href="#a">Υπάλληλοι</a>
                </li>
                <li>
                    <a href="#b">Τοπικά Καταστήματα</a>
                </li>
            </ul>
            <ul class="nav navbar-nav navbar-right">
                <li>
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
<br><br><br id = "a">
<?php
echo "<br><br><br>";
?>
<div class="container" >
    <!-- Modal -->
    <div class="modal fade" id="modal1" role="dialog">
        <div class="modal-dialog modal-sm">

            <!-- Modal content-->
            <div class="modal-content">
                <div class="modal-header">
                    <button type="button" class="close" data-dismiss="modal">&times;</button>
                    <h4 class="modal-title">Προσθήκη Υπαλλήλου</h4>
                </div>
                <div class="modal-body">
                        <form class="form-horizontal">
                            <div class="form-group">
                                <label class="col-sm-4 control-label">Username:  </label>
                                    <div class ="col-sm-8">
                                        <input class="form-text" id="formUser" type="text" value="">
                                    </div>
                            </div>
                            <div class="form-group">
                                <label class="col-sm-4 control-label">Password:  </label>
                                    <div class ="col-sm-8">
                                        <input class="form-text" id="formPass" type="password" value="">
                                    </div>
                            </div>
                            <div class="form-group">
                                <label class="col-sm-4 control-label">Role:  </label>
                                    <div class ="col-sm-8">
                                        <div class="form-group">
                                            <select class="form-text" id="select1">
                                                <option>admin</option>
                                                <option>transit_employee</option>
                                                <option>store_employee</option>
                                            </select>
                                        </div>
                                    </div>
                            </div>
                            <div class="form-group">
                                <label class="col-sm-4 control-label">Workspace:  </label>
                                <div class ="col-sm-8">
                                    <div class="form-group">
                                        <select class="form-text" id="change">
                                            <option>
                                                &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
                                                &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
                                                &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
                                                &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
                                            </option>
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
    <div class="container">
        <!-- Modal -->
        <div class="modal fade" id="modal2" role="dialog">
            <div class="modal-dialog modal-sm">

                <!-- Modal content-->
                <div class="modal-content">
                    <div class="modal-header">
                        <button type="button" class="close" data-dismiss="modal">&times;</button>
                        <h4 class="modal-title">Προσθήκη Καταστήματος</h4>
                    </div>
                    <div class="modal-body">

                        <form class="form-horizontal">
                            <div class="form-group">
                                <label class="col-sm-4 control-label">Όνομα:  </label>
                                <div class ="col-sm-8">
                                    <input class="form-text" id="formName" type="text" value="">
                                </div>
                            </div>
                            <div class="form-group">
                                <label class="col-sm-4 control-label">Διεύθυνση:  </label>
                                <div class ="col-sm-8">
                                    <input class="form-text" id="formAddress" type="text" value="">
                                </div>
                            </div>
                            <div class="form-group">
                                <label class="col-sm-4 control-label">Αριθμός:  </label>
                                <div class ="col-sm-8">
                                    <input class="form-text" id="formAddr_num" type="text" value="">
                                </div>
                            </div>
                            <div class="form-group">
                                <label class="col-sm-4 control-label">Πόλη:  </label>
                                <div class ="col-sm-8">
                                    <input class="form-text" id="formCity" type="text" value="">
                                </div>
                            </div>
                            <div class="form-group">
                                <label class="col-sm-4 control-label">ΤΚ:  </label>
                                <div class ="col-sm-8">
                                    <input class="form-text" id="formZip" type="text" value="">
                                </div>
                            </div>
                            <div class="form-group">
                                <label class="col-sm-4 control-label">Τηλέφωνο:  </label>
                                <div class ="col-sm-8">
                                    <input class="form-text" id="formPhone_num" type="text" value="">
                                </div>
                            </div>
                            <div class="form-group">
                                <label class="col-sm-4 control-label">Συντεταγμένες:  </label>
                                <div class ="col-sm-8">
                                    <textarea class="form-text" rows="3" id="coords2"></textarea>
                                </div>
                            </div>
                            <div class="form-group">
                                <label class="col-sm-4 control-label">Γονικό Transit:  </label>
                                <div class ="col-sm-8">
                                    <div class="form-group">
                                        <select class="form-text" id="select2">
                                            <option>
                                                &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
                                                &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
                                                &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
                                                &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
                                            </option>
                                        </select>
                                    </div>
                                </div>
                            </div>
                        </form>
                    </div>
                    <div class="modal-footer">
                        <button type="button" class="btn btn-default modal-button" id="submit2">Υποβολή</button>
                    </div>
                </div>
            </div>
        </div>
    </div>
    <div class="row">
    <div class="col-md-2">
            <div></div>
    </div>
    <div class="col-md-8">
        <h3>
            <span>Διαχείριση Yπαλλήλων</span>
            <button type="button" class="btn-danger btn-lg pull-right btn-primary-spacing" id="delete1"><em class="glyphicon glyphicon-remove"></em></button>
            <button type="button" class="btn-primary btn-lg pull-right btn-primary-spacing" id="edit1"><em class="glyphicon glyphicon-pencil"></em></button>
            <button type="button" class="btn-primary btn-lg pull-right btn-primary-spacing" id="add1"><em class="glyphicon glyphicon-plus"></em></button>
        </h3><br>
    <div class = "table-responsive ">

        <table class="table table-striped table-hover table-bordered table-condensed">
            <thead>
              <tr>
                <th class ="text-center">Όνομα Χρήστη</th>
                <th class ="text-center">Ρόλος</th>
                <th class ="text-center">Κατάστημα</th>
                <th class ="text-center">  <em class="glyphicon glyphicon-record"></em></th>
              </tr>
            </thead>
            <tbody class = "text-center" id = "x"></tbody>
        </table>
    </div>
    </div>
    <div class="col-md-2">
            <div></div>
    </div>
    </div>
    <div class="col-md-2">
        <div></div>
    </div>
</div>
<div class ="container">
    <div class="row" id = "b">
        <div class="col-md-2">
            <div></div>
        </div>
        <div class="col-md-10">
            <h3 id ="store">
                <span>Διαχείριση Τοπικού Καταστήματος</span>
                <button type="button" class="btn-danger btn-lg pull-right btn-primary-spacing" id="delete2"><em class="glyphicon glyphicon-remove"></em></button>
                <button type="button" class="btn-primary btn-lg pull-right btn-primary-spacing" id="edit2"><em class="glyphicon glyphicon-pencil"></em></button>
                <button type="button" class="btn-primary btn-lg pull-right btn-primary-spacing" id="add2"><em class="glyphicon glyphicon-plus"></em></button>
            </h3><br>
            <div class = "table-responsive ">
                <table class="table table-striped table-hover table-bordered table-condensed">
                    <thead>
                    <tr>
                        <th class ="text-center">id</th>
                        <th class ="text-center">Όνομα</th>
                        <th class ="text-center">Διεύθυνση</th>
                        <th class ="text-center">#</th>
                        <th class ="text-center">Πόλη</th>
                        <th class ="text-center">ΤΚ</th>
                        <th class ="text-center">Τηλέφωνο</th>
                        <th class ="text-center">Συντεταγμένες</th>
                        <th class ="text-center"><em class="glyphicon glyphicon-record"></em></th>
                    </tr>
                    </thead>
                    <tbody class = "text-center" id = "y"></tbody>
                </table>
            </div>
        </div>
    </div>
</div>
<div class ="container">
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

<script src="js/manageUsers.js"></script>
<script src="js/manageStore.js"></script>
</body>
</html>

