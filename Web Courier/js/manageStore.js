$(document).ready(function(){
    $('body').on('click.myDisable', 'a', function(e) { e.preventDefault(); });
    var btn_mtx = $('button');     //Locally save reusable selector for performance
    btn_mtx.prop('disabled', true); //disables visuals + functionality

    $.ajax({url: "php/selectStore.php", success: function(responseText){
        var response = responseText.substring(0, responseText.length - 1);
        var object_array = response.split(';');
        var html_tags = "";
        var i = 0;

        for( i in object_array) {
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
        $("#y").html(html_tags); //Overwriting <tbody> html not appending!
    }});
    $.ajax({url: "php/listTransit.php", success: function(responseText){
        //Options for dropdown
        var transit = responseText.split(';');

        var i = 0, x = 0;
        var html_tags = "";
        var object;

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
        //Parent Transit list in add/edit store modal
        $("#select2").html(html_tags);
    }});

    btn_mtx.prop('disabled', false); //enables visuals + functionality
    $('body').off('click.myDisable'); //enables click on links
});

$(document).ready(function(){
    $(document).on("click", "#submit2", function(){
        $('body').on('click.myDisable', 'a', function(e) { e.preventDefault(); });
        var btn_mtx = $('button');    //Locally save reusable selector for performance
        btn_mtx.prop('disabled', true); //disables visuals + functionality

        var name = $("#formName").val();
        var address = $("#formAddress").val();
        var addr_num = $("#formAddr_num").val();
        var city = $("#formCity").val();
        var zip = $("#formZip").val();
        var phone_num = $("#formPhone_num").val();
        var coords = $("#coords2").val();
        var transit = $("#select2").val();

        if ((isNaN(addr_num)) || (isNaN(zip))|| (isNaN(phone_num))){
            alert("Παρακαλώ είσαγετε ακέραιο στα πεδία Αριθμός, ΤΚ, Τηλέφωνο!");
        }
        else if( name !== "" && address !== "" &&
            addr_num !== "" && city !== "" && zip !== ""
            && phone_num !== "" && coords !== "" && transit !== "") {

            $.ajax({
                url: "php/insertStore.php", type: "post",
                data: {
                    'ls_name': name, 'ls_address': address,
                    'ls_addr_num': addr_num, 'ls_city': city, 'ls_zip': zip,
                    'ls_phone_num': phone_num, 'ls_coords': coords, 'ls_transit': transit
                },
                dataType: "text", success: function (responseText) {
                    if (!isNaN(responseText)) {
                        var id = responseText;
                        var html_tags = "<tr>\n";
                        html_tags += "\t<td>" + id + "</td>\n";
                        html_tags += "\t<td>" + name + "</td>\n";
                        html_tags += "\t<td>" + address + "</td>\n";
                        html_tags += "\t<td>" + addr_num + "</td>\n";
                        html_tags += "\t<td>" + city + "</td>\n";
                        html_tags += "\t<td>" + zip + "</td>\n";
                        html_tags += "\t<td>" + phone_num + "</td>\n";
                        html_tags += "\t<td>" + coords + "</td>\n";
                        html_tags += "\t<td align=\"center\">\n";
                        html_tags += "\t\t<div class=\"radio\">\n  <label><input type=\"radio\" name=\"optradio\"></label>\n";
                        html_tags += "\t\t</div>\n\t</td>\n</tr>\n";
                        $("#y").prepend(html_tags);
                    } else if(responseText === "NO_PARENT"){
                        alert("Λάθος Transit Id, Το μητρικό Transit hub δεν υπάρχει!");
                    } else if (responseText === "EXISTS") {
                        alert("Το κατάστημα προς εισαγωγή υπάρχει ήδη!");
                    } else if (responseText === "FAIL") {
                        alert("Η Εισαγωγή απέτυχε! Παρακαλώ ξαναπροσπαθήστε!");
                    } else {
                        alert("Σφάλμα");
                    }
                }
            });
        }
        else{
            alert("Παρακαλώ συμπληρώστε όλα τα πεδία!");
        }

        btn_mtx.prop('disabled', false); //enables visuals + functionality
        $('body').off('click.myDisable'); //enables click on links
        $('#modal2').modal('toggle'); //Toggle modal from on to off
    });
});

$(document).ready(function(){
    $(document).on("click", "#edit2", function(){
        $('body').on('click.myDisable', 'a', function(e) { e.preventDefault(); });
        var btn_mtx = $('button');    //Locally save reusable selector for performance
        btn_mtx.prop('disabled', true); //disables visuals + functionality

        var btn_html = "\t<button type=\"button\""
            + " class=\"btn btn-default modal-button\""
            + " id=\"submit4\">Υποβολή</button>";
        var modal = $("#modal2");

        var checked_option = $("#y").find(".radio input:radio:checked").closest("tr");

        var name = checked_option.children(":nth-child(2)").html();
        var address = checked_option.children(":nth-child(3)").html();
        var addr_num = checked_option.children(":nth-child(4)").html();
        var city = checked_option.children(":nth-child(5)").html();
        var zip = checked_option.children(":nth-child(6)").html();
        var phone_num = checked_option.children(":nth-child(7)").html();
        var coords = checked_option.children(":nth-child(8)").html();

        if(checked_option !== undefined){
            if(name !== undefined && address !== undefined && addr_num !== undefined
                && city !== undefined && zip !== undefined && phone_num !== undefined && coords !== undefined ){
                modal.find('.modal-title').text("Επεξεργασία Καταστήματος");
                modal.find('.modal-footer').html(btn_html);
                modal.find('#formName').val(name);
                modal.find('#formAddress').val(address);
                modal.find('#formAddr_num').val(addr_num);
                modal.find('#formCity').val(city);
                modal.find('#formZip').val(zip);
                modal.find('#formPhone_num').val(phone_num);
                modal.find('#coords2').val(coords);
                modal.find('#select2').val('');
                modal.modal('toggle');
            }else{
                alert("Παρακαλώ προσδιορίστε τοπικό κατάστημα προς επεξεργασία!");
            }
        }else{
            alert("Παρακαλώ προσδιορίστε τοπικό κατάστημα προς επεξεργασία!");
        }


        btn_mtx.prop('disabled', false); //enables visuals + functionality
        $('body').off('click.myDisable'); //enables click on links
    });
});

$(document).ready(function(){
    $(document).on("click", "#add2", function(){
        $('body').on('click.myDisable', 'a', function(e) { e.preventDefault(); });
        var btn_mtx = $('button');    //Locally save reusable selector for performance
        btn_mtx.prop('disabled', true); //disables visuals + functionality

        var btn_html = "\t<button type=\"button\""
            + " class=\"btn btn-default modal-button\""
            + " id=\"submit2\">Υποβολή</button>";
        var modal = $("#modal2");

        modal.find('.modal-title').text("Προσθήκη Καταστήματος");
        modal.find('.modal-footer').html(btn_html);
        modal.find('#formName').val('');
        modal.find('#formAddress').val('');
        modal.find('#formAddr_num').val('');
        modal.find('#formCity').val('');
        modal.find('#formZip').val('');
        modal.find('#formPhone_num').val('');
        modal.find('#coords2').val('');
        modal.find('#select2').val('');
        modal.modal('toggle');

        btn_mtx.prop('disabled', false); //enables visuals + functionality
        $('body').off('click.myDisable'); //enables click on links
    });
});

$(document).ready(function(){
    $(document).on("click", "#submit4", function(){
        $('body').on('click.myDisable', 'a', function(e) { e.preventDefault(); });
        var btn_mtx = $('button');    //Locally save reusable selector for performance
        btn_mtx.prop('disabled', true); //disables visuals + functionality

        var checked_option = $("#y").find(".radio input:radio:checked").closest("tr");

        var id = checked_option.children(":nth-child(1)").html();
        var name = $("#formName").val();
        var address = $("#formAddress").val();
        var addr_num = $("#formAddr_num").val();
        var city = $("#formCity").val();
        var zip = $("#formZip").val();
        var phone_num = $("#formPhone_num").val();
        var coords = $("#coords2").val();
        var transit = $("#select2").val();

        if ((isNaN(addr_num)) || (isNaN(zip))|| (isNaN(phone_num))){
            alert("Παρακαλώ είσαγετε ακέραιο στα πεδία Αριθμός, ΤΚ, Τηλέφωνο!");
        }
        else if( id !== "" && name !== "" && address !== "" &&
            addr_num !== "" && city !== "" && zip !== "" &&
            phone_num !== "" && coords !== "" && transit !== "") {

            $.ajax({
                url: "php/updateStore.php", type: "post",
                data: {
                    'ls_id': id, 'ls_name': name, 'ls_address': address,
                    'ls_addr_num': addr_num, 'ls_city': city, 'ls_zip': zip,
                    'ls_phone_num': phone_num, 'ls_coords': coords, 'ls_transit': transit
                },
                dataType: "text", success: function (responseText) {
                    if (responseText === "OK") {
                        var html_tags = "";
                        html_tags += "\t<td>" + id + "</td>\n";
                        html_tags += "\t<td>" + name + "</td>\n";
                        html_tags += "\t<td>" + address + "</td>\n";
                        html_tags += "\t<td>" + addr_num + "</td>\n";
                        html_tags += "\t<td>" + city + "</td>\n";
                        html_tags += "\t<td>" + zip + "</td>\n";
                        html_tags += "\t<td>" + phone_num + "</td>\n";
                        html_tags += "\t<td>" + coords + "</td>\n";
                        html_tags += "\t<td align=\"center\">\n";
                        html_tags += "\t\t<div class=\"radio\">\n  <label><input type=\"radio\" name=\"optradio\"></label>\n";
                        html_tags += "\t\t</div>\n\t</td>\n";
                        checked_option.html(html_tags);
                    } else if(responseText === "NO_PARENT"){
                        alert("Λάθος Transit Id, Το μητρικό Transit hub δεν υπάρχει!");
                    }
                    else if (responseText === "NOT_EXISTS") {
                        alert("Το τοπικό κατάστημα δεν υπάρχει!")
                    } else if (responseText === "FAIL") {
                        alert("Τα δεδομένα ενημέρωσης της εγγραφής είναι ιδία με τα αρχικά!");
                    } else {
                        alert("Σφάλμα");
                    }
                }
            });
        }
        else{
            alert("Παρακαλώ συμπληρώστε όλα τα πεδία!");
        }

        btn_mtx.prop('disabled', false); //enables visuals + functionality
        $('body').off('click.myDisable'); //enables click on links
        $('#modal2').modal('toggle');
    });
});

$(document).ready(function(){
    $("#delete2").click(function(){
        $('body').on('click.myDisable', 'a', function(e) { e.preventDefault(); });
        var btn_mtx = $('button');    //Locally save reusable selector for performance
        btn_mtx.prop('disabled', true); //disables visuals + functionality

        var checked_option = $("#y").find(".radio input:radio:checked").closest("tr");
        var tosend = checked_option.children(":first").html();

        if(tosend !== undefined)
        {
            $.ajax({url: "php/deleteStore.php", type:"post", data: {'id':tosend}, dataType: "text", success: function(responseText){
                if(responseText === "OK"){
                    checked_option.remove();
                    //Remove table row that has first cell equal to the track num
                    //of the delivered order
                    var store = checked_option.children(":nth-child(2)").text(); //Find store name
                    $('#x').find("tr").filter(function(){ //#x -> users table
                        return $(this).children(":nth-child(3)").text() === store; //3rd column is the store that the user works in
                    }).remove();
                }else if(responseText === "FAIL"){
                    alert("Η διαγραφή απέτυχε! Παρακαλώ ξαναπροσπαθήστε!");
                }else{
                    alert("Σφάλμα");
                }
            }});
        }else {
            alert("Παρακαλώ προσδιορίστε τοπικό κατάστημα προς διαγραφή!");
        }
        btn_mtx.prop('disabled', false); //enables visuals + functionality
        $('body').off('click.myDisable'); //enables click on links
    });
});
