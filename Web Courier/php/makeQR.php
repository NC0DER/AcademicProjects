<?php
if (isset($_GET['qr']))
{
    include('phpqrcode/qrlib.php');
    //image is output'ed directly to DON inside a modal

    //Sanitizing the entire GET array from html, input
    $_GET   = filter_input_array(INPUT_GET, FILTER_SANITIZE_STRING);

    $message = $_GET['qr'];

    //Αλλάζοντας το QR error correction level
    //Level L – up to 7% damage
    //Level M – up to 15% damage
    //Level Q – up to 25% damage
    //Level H – up to 30% damage

     QRcode::png($message,false,QR_ECLEVEL_H,3);
     //Save the QRcode as image in project folder webcourier/
     QRcode::png($message,"../qr.png",3,3);
}
?>