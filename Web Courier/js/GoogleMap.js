var isMapLoaded = false; //global variable for safe async loading of markers after the map has loaded

function myMap() {
    var greece = new google.maps.LatLng(38.2774666,24.6967213);
    var properties= {
        center:greece,
        zoom:7,
        mapTypeId: google.maps.MapTypeId.ROADMAP,
        panControl: true,
        zoomControl: false,
        mapTypeControl: false,
        scaleControl: true,
        streetViewControl: false,
        overviewMapControl: true,
        rotateControl: false
    };
    var map=new google.maps.Map(document.getElementById("myMap"),properties);
    isMapLoaded = true;
}

//Separating the handler from the rest of google maps dynamic content code
//Because the last dynamic entry overwrites each markers handler (CLosure Issue)
function addClickHandler(Map,InfoWindow,Marker,Content) {
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
$(document).ready(function(){
    if (typeof google === 'object' && typeof google.maps === 'object') {
        isMapLoaded = true;
        if(isMapLoaded){
            $('body').on('click.myDisable', 'a', function(e) { e.preventDefault(); });
            var btn_mtx = $('button');     //Locally save reusable selector for performance
            btn_mtx.prop('disabled', true); //disables visuals + functionality

            var greece = new google.maps.LatLng(38.2774666,24.6967213);
            var properties= {
                center:greece,
                zoom:7,
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
            var line;

            $.ajax({url: "php/generateMap.php", success: function(responseText){
                var response = responseText.split('/'); //Array with every response
                var object_array = response[0].split(';'); //first response contains transit hubs
                var marker;

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

                        center = new google.maps.LatLng(latitude,longitude);
                        marker = new google.maps.Marker({
                            position: center,
                            map:map,
                            name: object.name,
                            title:object.id
                        });
                        mark_array.push(marker);
                        addClickHandler(map,infowindow,marker,"");
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

                        center = new google.maps.LatLng(latitude,longitude);
                        marker = new google.maps.Marker({
                            position: center,
                            map:map,
                            name: object.name,
                            icon:icon,
                            title:"_"+ object.id //ids starting with _ are reserved for local stores
                        });
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
                        addClickHandler(map,infowindow,marker,"");
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
    $("#track").click(function(){
        $('body').on('click.myDisable', 'a', function(e) { e.preventDefault(); });
        var btn_mtx = $('button');    //Locally save reusable selector for performance
        btn_mtx.prop('disabled', true); //disables visuals + functionality

        var checked_option = $("#x").find(".radio input:radio:checked").closest("tr");
        var tosend = checked_option.children(":nth-child(1)").html();
        if(tosend === undefined) {
            alert("Επιλέξτε παραγγελία για να εντοπίσετε στον χάρτη!");
            btn_mtx.prop('disabled', false); //enables visuals + functionality
            $('body').off('click.myDisable'); //enables click on links
            return;
        }
        $.ajax({url: "php/orderInfo.php", type: "post",
            data: {
                'track_num': tosend
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

                        for(var i in mark_array){
                            if((mark_array.hasOwnProperty(i)) && (mark_array[i].info !== "")){
                                mark_array[i].info = ""; //Erase former current info window
                            }
                            if((mark_array.hasOwnProperty(i)) && (mark_array[i].name === object.current)){
                                mark_array[i].info =
                                    "<b>Τρέχουσα Τοποθεσία Δέματος:</b><br>"+ object.current +
                                    "<br><b>Το δέμα έχει περάσει από:</b><br>" + object.visited;
                                google.maps.event.trigger(mark_array[i], 'click');
                            }
                        }
                    }
                }
            }});
        btn_mtx.prop('disabled', false); //enables visuals + functionality
        $('body').off('click.myDisable'); //enables click on links
    });
});