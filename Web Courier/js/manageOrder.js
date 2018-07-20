$(document).ready(function(){
    $('body').on('click.myDisable', 'a', function(e) { e.preventDefault(); });
    var btn_mtx = $('button');     //Locally save reusable selector for performance
    btn_mtx.prop('disabled', true); //disables visuals + functionality

    $.ajax({url: "php/listStore.php", success: function(responseText){
        var response = responseText.split('/');
        var object_array = response[0].split(';');
        var object;
        var html_tags = "";
        var i = 0, x = 0;

        object = JSON.parse(response[1]); //Directly from response table since its only one object
        html_tags = "\t<option>" + object['name'] + "</option>\n";
        $("#sel1").html(html_tags);

        html_tags = ""; //Empty html tags for next dropdown list
        for(i in object_array) {
            if (object_array.hasOwnProperty(i)) {
                object = JSON.parse(object_array[i]);
                for (x in object) {
                    //Checking if x is a property of the object and not its prototype
                    if (object.hasOwnProperty(x)) {
                        html_tags += "\t<option>" + object[x] + "</option>\n";
                    }
                }
            }
        }
        $("#sel2").html(html_tags); //Overwriting <tbody> html not appending!
    }});
    btn_mtx.prop('disabled', false); //enables visuals + functionality
    $('body').off('click.myDisable'); //enables click on links
});

$(document).ready(function(){
    $('body').on('click.myDisable', 'a', function(e) { e.preventDefault(); });
    var btn_mtx = $('button');     //Locally save reusable selector for performance
    btn_mtx.prop('disabled', true); //disables visuals + functionality

    $.ajax({url: "php/selectOrder.php", success: function(responseText){
        var response = responseText.substring(0, responseText.length - 1);
        var object_array = response.split(';');
        var html_tags = "";

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
        $("#x").html(html_tags); //Overwriting <tbody> html not appending!
    }});
    btn_mtx.prop('disabled', false); //enables visuals + functionality
    $('body').off('click.myDisable'); //enables click on links
});

$(document).ready(function(){
    $(document).on("click", "#deliver", function() {
        $('body').on('click.myDisable', 'a', function (e) {
            e.preventDefault();
        });
        var btn_mtx = $('button');    //Locally save reusable selector for performance
        btn_mtx.prop('disabled', true); //disables visuals + functionality

        $.ajax({
            url: "php/deliveredOrder.php", type: "post",
            success: function (responseText) {
                if(responseText !== "NO_ORDERS"){
                    var object_array = responseText.split(';');
                    var i = 0;
                    var text = "Οι ακόλουθες παραγγελίες έφτασαν!\n";
                    for( i in object_array) {
                        if (object_array.hasOwnProperty(i)) {
                            var object = JSON.parse(object_array[i]);
                            text += " -"+ object.tracking_num + "\n";
                            //Remove table row that has first cell equal to the track num
                            //of the delivered order
                            $('#x').find("tr").filter(function(){
                                return $(this).children(":nth-child(1)").text() === object.tracking_num;
                            }).remove();
                        }
                    }
                    alert(text);
                }else if(responseText === "NO_ORDERS"){
                    alert("Δεν υπάρχουν παραγγελίες προς παράδοση!");
                }else{
                    alert("Σφάλμα");
                }
            }});

        btn_mtx.prop('disabled', false); //enables visuals + functionality
        $('body').off('click.myDisable'); //enables click on links
    });
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

        var from = $("#sel1").val();
        var to = $("#sel2").val();
        var type = $("#sel3").val();
        if(from === to){
            alert("Η προέλευση του πακέτου δεν μπορεί να είναι ίδια με τον προορισμό!");
            btn_mtx.prop('disabled', false); //enables visuals + functionality
            $('body').off('click.myDisable'); //enables click on links
            $('#modal1').modal('toggle'); //Toggle modal from on to off
            return;
        }
        if( from !== "" && to !== "" && type !== "") {
            $.ajax({
                url: "php/processOrder.php", type: "post",
                data: {
                    'from': from, 'to': to, 'typeof': type
                },
                dataType: "text", success: function (responseText) {
                    var response = responseText;
                    var html_tags = "";

                    var object = JSON.parse(response);
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

                    $("#qr").attr('src', "php/makeQR.php?qr="+object.tracking_num);
                    $("#x").prepend(html_tags); //Prepending html to the start of <tbody>
                }});
            }
            else{
                alert("Please fill out all forms!");
            }

        btn_mtx.prop('disabled', false); //enables visuals + functionality
        $('body').off('click.myDisable'); //enables click on links
        $('#modal1').modal('toggle'); //Toggle modal from on to off
        $('#modalqr').modal('toggle'); //Toggle modal for qr from off to on
    });
});

$(document).ready(function(){
    $("#delete1").click(function(){
        $('body').on('click.myDisable', 'a', function(e) { e.preventDefault(); });
        var btn_mtx = $('button');    //Locally save reusable selector for performance
        btn_mtx.prop('disabled', true); //disables visuals + functionality

        var checked_option = $("#x").find(".radio input:radio:checked").closest("tr");
        var tosend = checked_option.children(":first").html();

        if(tosend !== undefined)
        {
            $.ajax({url: "php/cancelOrder.php", type:"post", data: {'tracking_num':tosend}, dataType: "text", success: function(responseText){
                if(responseText === "OK"){
                    checked_option.remove();
                }else if(responseText === "FAIL"){
                    alert("Η ακύρωση της παραγελλίας απέτυχε! Παρακαλώ ξαναπροσπαθήστε!");
                }else{
                    alert("Σφάλμα");
                }
            }});
        }else {
            alert("Επιλέξτε μια παραγγελία για ακύρωση!");
        }
        btn_mtx.prop('disabled', false); //enables visuals + functionality
        $('body').off('click.myDisable'); //enables click on links
    });
});