/*
 * HTMLPage.h
 *
 *  Created on: 23 Dec 2019
 *      Author: jp112sdl
 */

/*
 * HTMLPage.h
 *
 *  Created on: 23 Dec 2019
 *      Author: jp112sdl
 */

#ifndef HTML_H_
#define HTML_H_

const char index_html[] PROGMEM = R"=====(
<!DOCTYPE html>
<html lang="en">

  <head>
    <meta name="viewport" content="width=device-width, initial-scale=1, user-scalable=no" />
    <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
    <style>
      body { text-align: center; }
      .vert { margin-bottom: 10%;}
      .hori { margin-bottom: 0%; }
      .slider { width: 300px; }
    </style>
  </head>

  <body>
    <div>
      <img src="/pic" id="picimg" width="70%">
    </div>
    <div>
      <p>
        <button id="reloadBtn" onclick="location.reload();">Aktualisieren</button>
        <button id="srcBtn" onclick="location.href='?show=video';">Video-Ansicht</button>
        <button id="flashlightBtn" onclick="toggleFlashlight()">Blitzlicht anschalten</button>
        <input type="range" min="0" max="180" value="90" class="slider" id="servoSlider" onchange="SetServoPos(this.value)" />
      </p>
    </div>

    <script src="//ajax.googleapis.com/ajax/libs/jquery/3.3.1/jquery.min.js"></script>
    <script>

        var slider    = document.getElementById("servoSlider");
        var picimg    = document.getElementById("picimg");
        var reloadBtn = document.getElementById("reloadBtn");
        var srcBtn    = document.getElementById("srcBtn");
        var flashlightBtn = document.getElementById("flashlightBtn");
        var flashlightState = '0';

        $.ajaxSetup({
          timeout: 1000
        });
        
        function ajaxErrorHandler(err) {
          // TODO: implement some error handling
        }
        
        function GetServoPos(){
          $.get('/get?item=servopos')
            .done(function(data) {
              console.log("GetServoPos = "+data);

              slider.value = data;
            })
            .fail(ajaxErrorHandler);
        } 

        function GetFlashlightState(){
          $.get('/get?item=flashlight')
            .done(function(data) {
              flashlightState = data;
              console.log("flashlightState = "+data);
              flashlightBtn.innerText = "Blitzlicht " + (data === '1' ? 'ausschalten' : 'anschalten');
            })
            .fail(ajaxErrorHandler);
        } 

        function toggleFlashlight() {
          $.post('/set', 'flashlight=' + (flashlightState === '1' ? '0' : '1'))
            .done(function() {
               flashlightState = (flashlightState === '1' ? '0' : '1');
              console.log("flashlightState = "+flashlightState);
              flashlightBtn.innerText = "Blitzlicht " + (flashlightState === '1' ? 'ausschalten' : 'anschalten');
             })
           .fail(ajaxErrorHandler);
        }
        
        slider.oninput = function() {
          slider.value = this.value;
        }

        function SetServoPos(pos) {
          $.post("/set","servopos=" + pos)
            .fail(ajaxErrorHandler);

          var i = false;
          if(i) clearTimeout(i);
          i = setTimeout(function() {document.getElementById("picimg").src = "/pic?" + Date.now(); i=false}, 1500);
        }

        GetServoPos();
        GetFlashlightState();

        var queryStr = window.location.search;
        console.log(queryStr);
        if (queryStr === "?show=video") {
          reloadBtn.style.display = "none";
          // Warum mit FQDN und nicht einfach "/vid" ?
          picimg.src              = "http://"+window.location.hostname+":81/vid";
          srcBtn.innerHTML        = "Bild-Ansicht";
          srcBtn.setAttribute("onClick", "location.href='?show=picture';");
        }

        if (queryStr === "?show=picture") {
          reloadBtn.style.display = "inline";
          picimg.src              = "/pic";
          srcBtn.innerHTML        = "Video-Ansicht";
          srcBtn.setAttribute("onClick", "location.href='?show=video';");
        }
      
    </script>
  </body>

</html>

)=====";


#endif

/*
const char index_html[] PROGMEM = R"=====(
<!DOCTYPE html>
<html lang="en">

  <head>
    <meta name="viewport" content="width=device-width, initial-scale=1, user-scalable=no" />
    <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
    <style>
      body { text-align: center; }
      .vert { margin-bottom: 10%;}
      .hori { margin-bottom: 0%; }
      .slider { width: 300px; }
    </style>
  </head>

  <body onload="parseParams()">
    <div>
      <img src="/pic" id="picimg" width="70%">
    </div>
    <div>
      <p>
        <button id="reloadBtn" onclick="location.reload();">Aktualisieren</button>
        <button id="srcBtn" onclick="location.href='?show=video';">Video-Ansicht</button>
        <input type="range" min="0" max="180" value="90" class="slider" id="servoSlider" onchange="SetServoPos(this.value)" />
      </p>
    </div>

    <script src="//ajax.googleapis.com/ajax/libs/jquery/3.3.1/jquery.min.js"></script>
    <script>
      var slider    = document.getElementById("servoSlider");
      var picimg    = document.getElementById("picimg");
      var reloadBtn = document.getElementById("reloadBtn");
      var srcBtn    = document.getElementById("srcBtn");

      function GetServoPos(){ 
        var xhr = new XMLHttpRequest(); 
        xhr.open('GET','/servo?pos=get',true); 

        xhr.onload = function (e) {
          if (xhr.readyState === 4) {
            if (xhr.status === 200) {
              console.log(xhr.responseText);
              slider.value = xhr.responseText;
            } else {
              console.error(xhr.statusText);
            }
          }
        };

        xhr.onerror = function (e) {
          console.error(xhr.statusText);
        };
        xhr.send(null); 
      } 

      function parseParams() {
        GetServoPos();

        var queryStr = window.location.search;
        console.log(queryStr);
        if (queryStr === "?show=video") {
          reloadBtn.style.display = "none";
          picimg.src              = "http://"+window.location.hostname+":81/vid";
          srcBtn.innerHTML        = "Bild-Ansicht";
          srcBtn.setAttribute("onClick", "location.href='?show=picture';");
        }

        if (queryStr === "?show=picture") {
          reloadBtn.style.display = "inline";
          picimg.src              = "/pic";
          srcBtn.innerHTML        = "Video-Ansicht";
          srcBtn.setAttribute("onClick", "location.href='?show=video';");
        }
      }

      slider.oninput = function() {
        slider.value = this.value;
      }
      $.ajaxSetup({
        timeout: 1000
      });

      function SetServoPos(pos) {
        $.post("/set","servopos=" + pos).fail(function(err) { console.log("set servo pos failed: "+err); } );
        var i = false;
        if(i) clearTimeout(i);
        i = setTimeout(function() {document.getElementById("picimg").src = "/pic?" + Date.now(); i=false}, 1500);
      }
    </script>
  </body>

</html>

)=====";


#endif
*/
