$(document).ready(function(){
    $(document).on("click", "#video", function(){ //Manual Click restart
       setwebcam(); //from webqrjs, it (re)starts the camera
    });
});

//Triggered manually by read() function on webqr.js
$(document).ready(function(){
    $(document).on("change", "#result", function(){
        var decodedData = $(this).text();
        if(decodedData.length !== 14){ //4 chars for start/end & 10 digits for unix epoch in seconds
            alert(decodedData); //Basically we output the error message
        }else{
            $.ajax({
                url: "php/updateOrder.php", type: "post",
                data: {
                    'track_num': decodedData
                },
                dataType: "text", success: function (responseText) {
                    //document.write(responseText);
                    switch(responseText){
                        case "ERROR":
                            alert("Σφάλμα");
                            break;
                        case "NO_ORDER":
                            alert("Το QR που δώσατε δεν αντιστοιχεί σε υπάρχουσα παραγγελία!");
                            break;
                        case "WRONG_STORE":
                            alert("Αυτή η παραγγελία ανήκει σε άλλο κατάστημα!");
                            break;
                        case "FORWARD_TO_TRANSIT":
                            alert("Η παραγγελία προωθήθηκε στο επόμενο Transit hub!");
                            break;
                        case "FORWARD_TO_LOCAL_STORE":
                            alert("Η παραγγελία προωθήθηκε στο τοπικό κατάστημα!");
                            break;
                        case "UPDATE_ERROR":
                            alert("Σφάλμα ενημέρωσης της τρέχουσας τοποθεσίας του πακέτου!");
                            break;
                        case "DELETE_ERROR":
                            alert("Σφάλμα διαγραφής της παραγγελίας του παρεδοθέντος πακέτου!");
                            break;
                        default:
                            alert("Default Case");
                            break;
                    }
                }
            });
        }
    });
});