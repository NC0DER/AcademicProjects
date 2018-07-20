//This function below is used to generate dynamic dropdown lists
$(document).ready(function () {
    $('body').on('click.myDisable', 'a', function(e) { e.preventDefault(); });
    var btn_mtx = $('button');     //Locally save reusable selector for performance
    btn_mtx.prop('disabled', true); //disables visuals + functionality

    //Options for dropdown
    var transit;
    var store;

    $.ajax({url: "php/listAll.php", success: function(responseText){
        var response = responseText.split('/');
        transit = response[0].split(';');
        store = response[1].split(';');
    }});
    btn_mtx.prop('disabled', false); //enables visuals + functionality
    $('body').off('click.myDisable'); //enables click on links

    $("#select1").change(function () {
        $('body').on('click.myDisable', 'a', function(e) { e.preventDefault(); });
        var btn_mtx = $('button');     //Locally save reusable selector for performance
        btn_mtx.prop('disabled', true); //disables visuals + functionality

        var value = $(this).val();
        var change = $("#change");
        var i = 0, x = 0;
        var html_tags = "";
        var object;
        switch(value){
            case "admin":
                change.html("<option>NONE</option>");
                break;
            case "transit_employee":
                html_tags = "";
                for(i in transit) {
                    if (transit.hasOwnProperty(i)) {
                        object = JSON.parse(transit[i]);
                        for (x in object) {
                            //Checking if x is a property of the object and not its prototype
                            if (object.hasOwnProperty(x)) {
                                html_tags += "\t<option>" + object[x] + "</option>\n";
                            }
                        }
                    }
                }
                change.html(html_tags);
                break;
            case "store_employee":
                i = 0;
                x = 0;
                html_tags = "";
                for(i in store) {
                    if (store.hasOwnProperty(i)) {
                        object = JSON.parse(store[i]);
                        for (x in object) {
                            //Checking if x is a property of the object and not its prototype
                            if (object.hasOwnProperty(x)) {
                                html_tags += "\t<option>" + object[x] + "</option>\n";
                            }
                        }
                    }
                }
                change.html(html_tags);
                break;
            default:
                alert("ERROR");
                break;
        }
        btn_mtx.prop('disabled', false); //enables visuals + functionality
        $('body').off('click.myDisable'); //enables click on links
    });
});

$(document).ready(function(){
    $('body').on('click.myDisable', 'a', function(e) { e.preventDefault(); });
    var btn_mtx = $('button');     //Locally save reusable selector for performance
    btn_mtx.prop('disabled', true); //disables visuals + functionality

    $.ajax({url: "php/selectUsers.php", success: function(responseText){
    var response = responseText.substring(0, responseText.length - 1);
    var object_array = response.split(';');
    var html_tags = "";

    for(var i in object_array) {
        if (object_array.hasOwnProperty(i)) {
            var object = JSON.parse(object_array[i]);
            html_tags += "<tr>\n";
            for (var x in object) {
                //Checking if x is a property of the object and not its prototype
                 if (object.hasOwnProperty(x)) {
                    html_tags += "\t<td>" + object[x] + "</td>\n";
                    }
            }
            html_tags += "\t<td align=\"center\">\n";
            html_tags += "\t\t<div class=\"radio\">\n  <label><input type=\"radio\" name=\"optradio\"></label>\n";
            html_tags +="\t\t</div>\n\t</td>\n</tr>\n";
        }
    }
    $("#x").html(html_tags); //Overwriting <tbody> html not appending!
    }});
    btn_mtx.prop('disabled', false); //enables visuals + functionality
    $('body').off('click.myDisable'); //enables click on links
});
$(document).ready(function(){
    $(document).on('click', 'a[href^="#"]', function(e) {
        // target element id
        var id = $(this).attr('href');

        // target element
        var $id = $(id);
        if ($id.length === 0) {
            return;
        }

        // prevent standard hash navigation (avoid blinking in IE)
        e.preventDefault();

        // top position relative to the document
        var pos = $id.offset().top;

        // animated top scrolling
        $('body, html').animate({scrollTop: pos});
    });
});
$(document).ready(function(){
    $(document).on("click", "#submit1", function(){
        $('body').on('click.myDisable', 'a', function(e) { e.preventDefault(); });
        var btn_mtx = $('button');    //Locally save reusable selector for performance
        btn_mtx.prop('disabled', true); //disables visuals + functionality

        var user = $("#formUser").val();
        var pass = $("#formPass").val();
        var role = $('#select1').find("option:selected").text();
        var work = $("#change").find("option:selected").text();

        if(user !== "" && pass !== "" && role !== "" && work !== "")
        {

            $.ajax({url: "php/insertUsers.php", type:"post",
                data: {'user_name':user, 'user_pass':pass, 'user_role':role, 'user_store':work},
                dataType: "text", success: function(responseText){
                //document.write(responseText);

                if(responseText === "OK"){
                    var html_tags = "<tr>\n"; //We are not attaching password which is saved only for the database insertion
                    html_tags += "\t<td>" + user + "</td>\n";
                    html_tags += "\t<td>" + role + "</td>\n";
                    html_tags += "\t<td>" + work + "</td>\n";
                    html_tags += "\t<td align=\"center\">\n";
                    html_tags += "\t\t<div class=\"radio\">\n  <label><input type=\"radio\" name=\"optradio\"></label>\n";
                    html_tags +="\t\t</div>\n\t</td>\n</tr>\n";
                    $("#x").prepend(html_tags);
                }else if(responseText === "EXISTS"){
                    alert("Ο χρήστης προς εισαγωγή υπάρχει ήδη!");
                }else if(responseText === "FAIL"){
                    alert("Η Εισαγωγή απέτυχε! Παρακαλώ ξαναπροσπαθήστε!");
                }else{
                    alert("Σφάλμα");
                }
            }});
        }
        else{
            alert("Παρακαλώ συμπληρώστε όλα τα πεδία!");
        }

        btn_mtx.prop('disabled', false); //enables visuals + functionality
        $('body').off('click.myDisable'); //enables click on links
        $('#modal1').modal('toggle');
    });
});
$(document).ready(function(){
    $(document).on("click", "#edit1", function(){
        $('body').on('click.myDisable', 'a', function(e) { e.preventDefault(); });
        var btn_mtx = $('button');    //Locally save reusable selector for performance
        btn_mtx.prop('disabled', true); //disables visuals + functionality

        var btn_html = "\t<button type=\"button\""
        + " class=\"btn btn-default modal-button\""
        + " id=\"submit3\">Υποβολή</button>";
        var modal = $("#modal1");

        var checked_option = $("#x").find(".radio input:radio:checked").closest("tr");
        var tosend = checked_option.children(":first").html();

        var res = $("td:nth-child(2)").text();
        var count = (res.match(/admin/g) || []).length;

        var user = checked_option.children(":nth-child(1)").html();
        var role = checked_option.children(":nth-child(2)").html();
        var work = checked_option.children(":nth-child(3)").html();

        if((count <= 1) && (role === 'admin')){
            alert("Σφάλμα Επεξεργασίας! Μόνο ένας διαχειριστής έμεινε!");
            btn_mtx.prop('disabled', false); //enables visuals + functionality
            $('body').off('click.myDisable'); //enables click on links
            return;
        }
        if(checked_option !== undefined){
            if(user !== undefined && role !== undefined && work !== undefined){
                modal.find('.modal-title').text("Επεξεργασία Υπαλλήλου");
                modal.find('.modal-footer').html(btn_html);
                modal.find('#formUser').val(user);
                modal.find('#formPass').val("nopass");
                modal.find('#select1').val(role);
                modal.find('#select1').trigger("change");
                modal.find('#change').val(work);
                modal.modal('toggle');
            }else{
                alert("Παρακαλώ προσδιορίστε χρήστη προς επεξεργασία!");
            }
        }else{
            alert("Παρακαλώ προσδιορίστε χρήστη προς επεξεργασία!");
        }

        btn_mtx.prop('disabled', false); //enables visuals + functionality
        $('body').off('click.myDisable'); //enables click on links
    });
});

$(document).ready(function(){
    $(document).on("click", "#add1", function(){
        $('body').on('click.myDisable', 'a', function(e) { e.preventDefault(); });
        var btn_mtx = $('button');    //Locally save reusable selector for performance
        btn_mtx.prop('disabled', true); //disables visuals + functionality

        var btn_html = "\t<button type=\"button\""
            + " class=\"btn btn-default modal-button\""
            + " id=\"submit1\">Υποβολή</button>";
        var modal = $("#modal1");
        var spaces = "\n&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;\n" +
            "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;\n" +
            "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;\n" +
            "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;\n";

        modal.find('.modal-title').text("Προσθήκη Υπαλλήλου");
        modal.find('.modal-footer').html(btn_html);
        modal.find('#formUser').val('');
        modal.find('#formPass').val('');
        modal.find('#select1').val('');
        modal.find('#change').html("<option>"+spaces+"</option>");
        modal.modal('toggle');

        btn_mtx.prop('disabled', false); //enables visuals + functionality
        $('body').off('click.myDisable'); //enables click on links
    });
});

$(document).ready(function(){
    $(document).on("click", "#submit3", function(){
        $('body').on('click.myDisable', 'a', function(e) { e.preventDefault(); });
        var btn_mtx = $('button');    //Locally save reusable selector for performance
        btn_mtx.prop('disabled', true); //disables visuals + functionality

        var checked_option = $("#x").find(".radio input:radio:checked").closest("tr");
        var user = $("#formUser").val();
        var pass = $("#formPass").val();
        var role = $('#select1').find("option:selected").text();
        var work = $("#change").find("option:selected").text();

        if(user !== "" && pass !== "" && role !== "" && work !== "")
        {

            $.ajax({url: "php/updateUsers.php", type:"post",
                data: {'user_name':user, 'user_pass':pass, 'user_role':role, 'user_store':work},
                dataType: "text", success: function(responseText){
                    if(responseText === "OK"){
                        var html_tags = "";
                        html_tags += "\t<td>" + user + "</td>\n";
                        html_tags += "\t<td>" + role + "</td>\n";
                        html_tags += "\t<td>" + work + "</td>\n";
                        html_tags += "\t<td align=\"center\">\n";
                        html_tags += "\t\t<div class=\"radio\">\n  <label><input type=\"radio\" name=\"optradio\"></label>\n";
                        html_tags +="\t\t</div>\n\t</td>\n";
                        checked_option.html(html_tags);
                    }else if(responseText === "NOT_EXISTS"){
                        alert("Ο χρήστης δεν υπάρχει!");
                    }else if(responseText === "FAIL"){
                        alert("Τα δεδομένα ενημέρωσης της εγγραφής είναι ιδία με τα αρχικά!");
                    }else{
                        alert("Σφάλμα");
                    }
                }});
        }
        else{
            alert("Παρακαλώ συμπληρώστε όλα τα πεδία!");
        }

        btn_mtx.prop('disabled', false); //enables visuals + functionality
        $('body').off('click.myDisable'); //enables click on links
        $('#modal1').modal('toggle');
    });
});

$(document).ready(function(){
    $("#delete1").click(function(){
        $('body').on('click.myDisable', 'a', function(e) { e.preventDefault(); });
        var btn_mtx = $('button');    //Locally save reusable selector for performance
        btn_mtx.prop('disabled', true); //disables visuals + functionality

        var checked_option = $("#x").find(".radio input:radio:checked").closest("tr");
        var tosend = checked_option.children(":first").html();

        var res = $("td:nth-child(2)").text();
        var count = (res.match(/admin/g) || []).length;

        var role = checked_option.children(":nth-child(2)").html();

        if((count <= 1) && (role === 'admin')){
            alert("Σφάλμα Διαγραφής! Μόνο ένας διαχειριστής έμεινε!");
            btn_mtx.prop('disabled', false); //enables visuals + functionality
            $('body').off('click.myDisable'); //enables click on links
            return;
        }

        if(tosend !== undefined)
        {
            $.ajax({url: "php/deleteUsers.php", type:"post", data: {'user_name':tosend}, dataType: "text", success: function(responseText){
                if(responseText === "OK"){
                    checked_option.remove();
                }else if(responseText === "FAIL"){
                    alert("Η διαγραφή απέτυχε! Παρακαλώ ξαναπροσπαθήστε!");
                }else{
                    alert("Σφάλμα");
                }
            }});
        }else{
            alert("Παρακαλώ προσδιορίστε χρήστη για διαγραφή!");
        }
        btn_mtx.prop('disabled', false); //enables visuals + functionality
        $('body').off('click.myDisable'); //enables click on links
    });
});