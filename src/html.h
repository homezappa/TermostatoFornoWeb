const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <link rel="stylesheet" href="https://use.fontawesome.com/releases/v5.7.2/css/all.css" integrity="sha384-fnmOCqbTlWIlj8LyTjo7mOUStjsKC4pOpQbqyi7RrhN7udi9RwhKkMHpvLbHG9Sr" crossorigin="anonymous">
  <style>
    html {
     font-family: Arial;
     display: inline-block;
     margin: 0px auto;
     text-align: center;
    }
    h2 { font-size: 2.0rem; }
    p { font-size: 2.0rem; }
    .units { font-size: 1rem; }
    .dht-labels{
      font-size: 1.2rem;
      vertical-align:middle;
      padding-bottom: 5px;
    }
    button { font-size: 2.0rem; }
  </style>
  <script>
    function TempUp(){
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function() {
      if (this.readyState == 4 && this.status == 200) {
        document.getElementById("tempDesired").innerHTML = this.responseText;
        ShowButton();
      }
    };
    xhttp.open("GET", "/tempup", true);
    xhttp.send();
    return false;
  }
  function TempDn(){
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function() {
      if (this.readyState == 4 && this.status == 200) {
        document.getElementById("tempDesired").innerHTML = this.responseText;
        ShowButton();
      }
    };
    xhttp.open("GET", "/tempdn", true);
    xhttp.send();
    return false;
  }
  function SaveTemp(){
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function() {
      if (this.readyState == 4 && this.status == 200) {
        document.getElementById("saveobj").innerHTML = this.responseText;
        ShowResult();
      }
    };
    xhttp.open("GET", "/save", true);
    xhttp.send();
    return false;
  }
  function ShowButton() {
        document.getElementById("saveobj").style.visibility = "hidden";
        document.getElementById("savebtn").style.visibility = "visible";
  }
  function ShowResult() {
        document.getElementById("saveobj").style.visibility = "visible";
        document.getElementById("savebtn").style.visibility = "hidden";
  }
  </script>
</head>
<body>
  <h2>Termostato Forno</h2>
  <p>
    <i class="fas fa-thermometer-half" style="color:#059e8a;"></i> 
    <span class="dht-labels">T&deg; misurata:</span> 
    <span id="temperature">%TEMPERATURE%</span>
    <sup class="units">&deg;C</sup>
  </p>
  <p>
    <span class="dht-labels">Stato:</span> 
    <span id="stRelais">%STATUS%</span>
  </p>
  <p>
    <i class="fas fa-thermometer-half" style="color:#059e8a;"></i> 
    <span class="dht-labels">T&deg; desiderata:</span> 
    <span id="tempDesired">%TEMPDESIRED%</span>
    <sup class="units">&deg;C</sup>
  </p>
  <p>
  <button onclick="return TempDn();" value="-">&nbsp;-&nbsp;</button>
  <button onclick="return TempUp();" value="+">&nbsp;+&nbsp;</button>
  <br />
  <button id="savebtn" style="visibility:hidden" onclick="return SaveTemp();" value="Salva">&nbsp;Salva&nbsp;</button>
  <span id="saveobj" style="visibility:hidden">---</span>
  </p>
  <p>
    <i class="fas fa-tint" style="color:#00add6;"></i> 
    <span class="dht-labels">Umidit&agrave;</span>
    <span id="humidity">%HUMIDITY%</span>
    <sup class="units">%</sup>
  </p>

  
</body>
<script>
setInterval(function ( ) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      var arTemp = this.responseText.split(";");
      document.getElementById("temperature").innerHTML = arTemp[0];
      if(arTemp.length >1)
      document.getElementById("stRelais").innerHTML = arTemp[1];
    }
  };
  xhttp.open("GET", "/temperature", true);
  xhttp.send();
}, 10000 ) ;

setInterval(function ( ) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("humidity").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "/humidity", true);
  xhttp.send();
}, 10000 ) ;

</script>
</html>)rawliteral";