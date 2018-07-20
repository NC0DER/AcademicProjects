$(document).ready(function(){
    if(screen.width <= 500) {
        $("#col-xs-pull-1").addClass("col-xs-pull-1");
        $("#col-xs-pull-2").addClass("col-xs-pull-2");
    }
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

var isMapLoaded = false; //global variable for safe async loading of markers after the map has loaded

function myMap() {
    var greece = new google.maps.LatLng(38.2774666,24.6967213);
    var zoom = 7;
    var div1 = $('#myMap');
    var div2 = $('#myMap2');

    if(screen.width <= 500){
        greece = new google.maps.LatLng(38.2774666,23.6967213);
        zoom = 6;
    }
    if(screen.height <= 1000){
        div1.height(0.70*screen.height);
        div2.height(0.70*screen.height);
    }else{
        div1.height(752);
        div2.height(752);
    }

    var properties= {
        center:greece,
        zoom:zoom,
        mapTypeId: google.maps.MapTypeId.ROADMAP,
        panControl: true,
        zoomControl: false,
        mapTypeControl: false,
        scaleControl: true,
        streetViewControl: false,
        overviewMapControl: true,
        rotateControl: false
    };
    var map = new google.maps.Map(document.getElementById("myMap"),properties);
    var map2 = new google.maps.Map(document.getElementById("myMap2"), properties);
    isMapLoaded = true;
}

//Separating the handler from the rest of google maps dynamic content code
//Because the last dynamic entry overwrites each markers handler (CLosure Issue)
function addClickHandler(Map,InfoWindow,Marker,Content) {
    google.maps.event.addListener(Marker, 'click', function() {
        InfoWindow.setContent(Content);
        if(Content !== "")
            InfoWindow.open(Map, Marker);
    }, { //More responsive for touch screens - cancels preventDefault()
        passive: true  //Permits browser to allow scrolling before all JS is loaded.
    });
}

function addClickHandler2(Map,InfoWindow,Marker,Content) {
    google.maps.event.addListener(Marker, 'click', function() {
        if(Marker.info !== "" && Marker.info !== undefined && Marker.info !== " "){
            var content_str = "<p>"+ Marker.info + "</p>";
            InfoWindow.setContent(content_str);
            InfoWindow.open(Map, Marker);
        }
    }, { //More responsive for touch screens - cancels preventDefault()
        passive: true  //Permits browser to allow scrolling before all JS is loaded.
    });
}
var mark_array = [];
var mark_array2 = [];
$(document).ready(function(){
    if (typeof google === 'object' && typeof google.maps === 'object') {
        isMapLoaded = true;
        if(isMapLoaded){
            $('body').on('click.myDisable', 'a', function(e) { e.preventDefault(); });
            var btn_mtx = $('button');     //Locally save reusable selector for performance
            btn_mtx.prop('disabled', true); //disables visuals + functionality

            var greece = new google.maps.LatLng(38.2774666,24.6967213);
            var zoom = 7;
            if(screen.width <= 800){
                greece = new google.maps.LatLng(38.2774666,23.6967213);
                zoom = 6;
            }
            var properties = {
                center:greece,
                zoom:zoom,
                mapTypeId: google.maps.MapTypeId.ROADMAP,
                panControl: true,
                zoomControl: false,
                mapTypeControl: false,
                scaleControl: true,
                streetViewControl: false,
                overviewMapControl: true,
                rotateControl: false
            };
            var icon = {
                url: "fonts/red-dot.png"
            };
            var map = new google.maps.Map(document.getElementById("myMap"), properties);
            var map2 = new google.maps.Map(document.getElementById("myMap2"), properties);
            var line;

            $.ajax({url: "php/generateMap.php", success: function(responseText){
                var response = responseText.split('/'); //Array with every response
                var object_array = response[0].split(';'); //first response contains transit hubs
                var marker,marker2;

                var center;
                var tr_pos;
                var infowindow = new google.maps.InfoWindow({
                    content: ""
                });
                for(var i in object_array) {
                    if (object_array.hasOwnProperty(i)) {
                        var object = JSON.parse(object_array[i]);
                        var latlng = object.coords.split(','); //(39.0151,28.7536) separates into two strings
                        var latitude = parseFloat(latlng[0]); //getting to each string its float value
                        var longitude = parseFloat(latlng[1]);

                        var content_str =
                            "<p>"+ object.id + ". "
                            + object.name + "<br>"
                            + object.address + " "
                            + object.addr_num + "<br>"
                            + "TK: " + object.zip_code + "<br>"
                            + "Τηλ. " + object.phone_num
                            + "</p>";
                        center = new google.maps.LatLng(latitude,longitude);
                        marker = new google.maps.Marker({
                            position: center,
                            map:map,
                            name: object.name,
                            title:object.id
                        });
                        mark_array.push(marker);
                        addClickHandler(map,infowindow,marker,content_str);

                        //Add Marker for transit hubs for the 2nd map
                        marker2 = new google.maps.Marker({
                            position: center,
                            map:map2,
                            name: object.name,
                            title:object.id,
                            info: ""
                        });
                        mark_array2.push(marker2);
                        addClickHandler2(map,infowindow,marker2,"");
                    }
                }
                object_array = response[1].split(';'); //second response contains local stores
                i = 0;
                for(i in object_array) {
                    if (object_array.hasOwnProperty(i)) {
                        object = JSON.parse(object_array[i]);
                        latlng = object.coords.split(','); //(39.0151,28.7536) separates into two strings
                        latitude = parseFloat(latlng[0]); //getting to each string its float value
                        longitude = parseFloat(latlng[1]);

                        content_str =
                            "<p>"+ object.id + ". "
                            + object.name + "<br>"
                            + object.address + " "
                            + object.addr_num + "<br>"
                            + "TK: " + object.zip_code + "<br>"
                            + "Τηλ. " + object.phone_num
                            + "</p>";
                        center = new google.maps.LatLng(latitude,longitude);
                        marker = new google.maps.Marker({
                            position: center,
                            map:map,
                            name: object.name,
                            icon:icon,
                            title:"_"+ object.id //ids starting with _ are reserved for local stores
                        });
                        //Add Marker for transit hubs for the 2nd map
                        marker2 = new google.maps.Marker({
                            position: center,
                            map:map2,
                            name: object.name,
                            icon:icon,
                            title:"_"+ object.id, //ids starting with _ are reserved for local stores
                            info: ""
                        });
                        mark_array2.push(marker2);
                        addClickHandler2(map,infowindow,marker2,"");
                        for(j in mark_array){
                            if(mark_array[j].getTitle() === object.tr_id){  //Find marker with the transit id for connection
                                tr_pos = mark_array[j].getPosition();
                                break; //Found the specific LatLng object
                            }
                        }
                        line = new google.maps.Polyline({
                            map: map,
                            path: [center, tr_pos],
                            strokeColor: "#0000CC",
                            strokeOpacity: 0.5,
                            strokeWeight: 2
                        });
                        mark_array.push(marker);
                        addClickHandler(map,infowindow,marker,content_str);
                    }
                }
                object_array = response[2].split(';'); //third response contains transit hub paths
                i = 0;
                var tr_a;
                var tr_b;
                for(i in object_array){
                    if (object_array.hasOwnProperty(i)){
                        object = JSON.parse(object_array[i]);
                        for(j in mark_array){
                            if(object.hub_a === mark_array[j].getTitle()){
                                tr_a = mark_array[j].getPosition();
                            }
                            if(object.hub_b === mark_array[j].getTitle()){
                                tr_b = mark_array[j].getPosition();
                            }
                        }
                        line = new google.maps.Polyline({
                            map: map,
                            path: [tr_a, tr_b],
                            strokeColor: "#000000",
                            strokeOpacity: 0.7,
                            strokeWeight: 3
                        });
                    }
                }
                //out of loop
            }});
            btn_mtx.prop('disabled', false); //enables visuals + functionality
            $('body').off('click.myDisable'); //enables click on links
        }
    }
});

$(document).ready(function(){
    $("#search2").click(function(){
        $('body').on('click.myDisable', 'a', function(e) { e.preventDefault(); });
        var btn_mtx = $('button');    //Locally save reusable selector for performance
        btn_mtx.prop('disabled', true); //disables visuals + functionality

        var form_data = $("#search-form2").val();
        if(form_data === "") {
            alert("Παρακαλώ εισάγετε τον κωδικό παραγγελίας!");
            btn_mtx.prop('disabled', false); //enables visuals + functionality
            $('body').off('click.myDisable'); //enables click on links
            return;
        }
        $.ajax({url: "php/orderInfo.php", type: "post",
            data: {
                'track_num': form_data
            }, dataType: "text",
            success: function(responseText){
                if(responseText === "ERROR"){
                    alert("Σφάλμα");
                }else if(responseText === "FAIL"){
                    alert("Η παραγγελία δεν βρέθηκε!");
                }else{
                    var object = JSON.parse(responseText);
                    if (typeof google === 'object' && typeof google.maps === 'object') {
                        content_str =
                            "<p>"+ object.current + "<br>"
                            + object.visited + "<br>"
                            + "</p>";

                        for(var i in mark_array2){
                            if((mark_array2.hasOwnProperty(i)) && (mark_array2[i].info !== "")){
                                mark_array2[i].info = ""; //Erase former current info window
                            }
                            if((mark_array2.hasOwnProperty(i)) && (mark_array2[i].name === object.current)){
                                mark_array2[i].info =
                                    "<b>Τρέχουσα Τοποθεσία Δέματος:</b><br>"+ object.current +
                                    "<br><b>Το δέμα έχει περάσει από:</b><br>" + object.visited;
                                google.maps.event.trigger(mark_array2[i], 'click');
                            }
                        }
                    }
                }
            }});
        btn_mtx.prop('disabled', false); //enables visuals + functionality
        $('body').off('click.myDisable'); //enables click on links
    });
});

$(document).ready(function(){
    $("#search").click(function(){
        $('body').on('click.myDisable', 'a', function(e) { e.preventDefault(); });
        var btn_mtx = $('button');    //Locally save reusable selector for performance
        btn_mtx.prop('disabled', true); //disables visuals + functionality

        var form_data = $("#search-form").val();
        if(form_data === "") {
            alert("Παρακαλώ εισάγετε τον ΤΚ ή την πόλη!");
            btn_mtx.prop('disabled', false); //enables visuals + functionality
            $('body').off('click.myDisable'); //enables click on links
            return;
        }
        if(!isNaN(form_data)){ //form_data is number
            form_data = Math.abs(form_data);
            $.ajax({url: "php/storeInfo.php", success: function(responseText){
                var object_array = responseText.split(';'); //Array of json objects
                var object = [];
                var arr = [];
                var distance = 999999;
                var TK = parseInt(form_data,10);
                var new_TK = 0;
                var name = "";
                //Make object from server's json response
                for(var i in object_array) {
                    if (object_array.hasOwnProperty(i)) {
                        object.push(JSON.parse(object_array[i]));
                        arr.push(object[i].zip_code);
                    }
                }
                //Scan object to find store with closest zip code
                for(var j in object){
                    if(object.hasOwnProperty(j)){
                        console.log(Math.abs(TK - object[j].zip_code));
                        if(Math.abs(TK - object[j].zip_code) <= distance){
                            distance = TK - object[j].zip_code;
                            new_TK = object[j].zip_code;
                            name = object[j].name;
                        }else{
                            break;
                        }
                    }
                }
                //Find which marker is that store
                if (typeof google === 'object' && typeof google.maps === 'object') {
                    //global array of markers
                    i = 0;
                    for(i in mark_array){
                        if((mark_array.hasOwnProperty(i)) && (mark_array[i].name === name)){
                            google.maps.event.trigger(mark_array[i], 'click');
                        }
                    }
                }
            }});
        }

        btn_mtx.prop('disabled', false); //enables visuals + functionality
        $('body').off('click.myDisable'); //enables click on links

    });
});

$(document).ready(function(){ //Dynamic search function
    $("#search-form").on('input',function(){
        $('body').on('click.myDisable', 'a', function(e) { e.preventDefault(); });
        var btn_mtx = $('button');    //Locally save reusable selector for performance
        btn_mtx.prop('disabled', true); //disables visuals + functionality

        var form_data = $(this).val();
        form_data = form_data.toLowerCase(); //works with utf-8 natively
        var diacritic = {
            "ά":"α", "έ":"ε", "ή":"η",
            "ί":"ι", "ύ":"υ", "ό":"ο",
            "ώ":"ω","ΰ":"υ","ΐ":"ι"
        };
        var str = form_data.replace(/[άέήίύόώΰΐ]/gi, function non(x){return diacritic[x];});
        var res = str.charAt(0).toUpperCase() + str.slice(1);

        $.ajax({url: "php/dynamicSearch.php", type: "post",
            data: {
                'search': res
            }, dataType: "text",
            success: function(responseText){
                var suggestion = $('#suggestion');
                var search_form = $('#search-form');
                if(responseText !== "") {
                    var object_array = responseText.split(';');
                    var html_tags = "\n";
                    for(var i in object_array) {
                        if (object_array.hasOwnProperty(i)) {
                            var object = JSON.parse(object_array[i]);
                            html_tags += "<a href=\"javascript:void(0)\" class =\"search-link\">";
                            html_tags += object.name +"</a><br>\n";
                        }
                    }

                    //Get form position
                    var position = search_form.offset(); //position = {left: <int>, top: <int>}
                    position.top += search_form.height() + 5; //+5 adjust padding / borders
                    suggestion.offset(position);
                    suggestion.html(html_tags).css('visibility','visible'); //Show suggestion box
                }else{
                    suggestion.css('visibility','hidden'); //Hide suggestion box
                }
            }});

        btn_mtx.prop('disabled', false); //enables visuals + functionality
        $('body').off('click.myDisable'); //enables click on links
    });

    $(document).on('click','.search-link',function(){ //If search link is pressed, show it on the map
        var link = $(this).text();
        if (typeof google === 'object' && typeof google.maps === 'object') {
            //global array of markers
            for(var i in mark_array){
                if((mark_array.hasOwnProperty(i)) && (mark_array[i].name === link)){
                    google.maps.event.trigger(mark_array[i], 'click');
                }
            }
        }

    });
});