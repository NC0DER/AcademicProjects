# Web Courier

I made this project for the Web Technologies course in university.  
The teaching goal of this project was to build a full stack web app  
with `ut8-greek-locale` support for an online courier using  
back-end languages and libraries (e.g `php`, `MySQL`, `phpqrcode`)  
and front-end languages and frameworks (e.g `Javascript`, `JQuery`, `HTML5`, `Bootstrap`).

# Prerequisite Libraries / Frameworks

* Bootstrap (https://getbootstrap.com/)
* Jquery (https://jquery.com/)
* Jsqrcode -Scan QR code image- (https://github.com/LazarSoft/jsqrcode)
* Phpqrcode -Generate QR code image - (http://phpqrcode.sourceforge.net/)
* Google maps API (https://cloud.google.com/maps-platform/)

*All prerequisites are free and contained locally in the project, there is no need to download any of them.*

# Features

- [x] Admin menu which can create, read, update, and delete stores & users.
- [x] Local Store menu which can create, read, track, and delete orders.
- [x] Transit Hub menu that can scan QR codes and forward orders in other transit hubs.
- [x] Client Website that can be used to track orders by their ID, or to search stores,   
      or to search with autocomplete nearby stores by approximating his location.
- [x] Realtime dynamic site without page refreshing using AJAX calls.
- [x] Realtime transit / store locating of any tracked order with markers and contact info on custom google map.
- [x] Responsive site, capable of viewing through a mobile device.
- [x] Hashed & Salted passwords inside the database.
- [x] Full UTF-8 Support.

# Project Architecture
![Project Architecture](https://github.com/NC0DER/AcademicProjects/blob/master/Web%20Courier/screenshots/model.jpg)

# Project Setup

The simplest, fastest way to setup the project is to install WAMP (http://www.wampserver.com/en/)  
and copy the project directory inside `../www` directory of your wamp install.  
Then you can open phpMyAdmin an database administration tool which is installed alongside wamp,  
and then import the `SQL` Schema contained in the file `/Web Courier/web_courier.sql`  
For Demonstration purposes the project can be hosted locally (`localhost`).  
In this case, an internet connection is required only for the rendering of the google maps api.

# Project Screenshots

You can find some screenshots below.  
If you wish to access all screenshot images you can click [this link](https://github.com/NC0DER/AcademicProjects/tree/master/Web%20Courier/screenshots)


![Screenshot1](https://github.com/NC0DER/AcademicProjects/blob/master/Web%20Courier/screenshots/login.jpg)  
____
![Screenshot2](https://github.com/NC0DER/AcademicProjects/blob/master/Web%20Courier/screenshots/tracked_order.jpg)  
