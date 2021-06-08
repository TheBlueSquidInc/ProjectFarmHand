#include <Arduino.h>
#include <WiFi.h>
#include <iostream>
#include <Adafruit_Sensor.h>
#include <DHT_U.h>
#include "DHT.h"

#include <OneWire.h>



#define DHTPIN 32
#define DHTTYPE DHT22

#define HUMPIN 33

#define SOILTEMPPIN 25

OneWire soilTempProb(SOILTEMPPIN);


DHT dht(DHTPIN, DHTTYPE);
WiFiServer wfServer(80);
char *w_ssid = ""; // your wifi network
char *pass = ""; //your wifi password
int status = WL_IDLE_STATUS;

String testString = R"(

<h1>This is a test r literal</h1> 

)";


String htmlData = R"(

<!DOCTYPE HTML>

<html>
<head>
<title>Vyne V0.1</title>
<head>

<body style="background-color:#5E8B7E">

<div id="main_div" style="width:50%;  margin:auto; background-color:#2F5D62; padding-top:20px; padding-bottom:20px;border-radius:25px" >


<center><p style="font-family:Lucida Console, Helvetica, Monaco; color:white; font-size:27px"><bold>Vyne - Flora Critical Monitoring</bold></p></center>

<div id="dataTable" ">

<table border="0" align="center">
<thead>
<TR>
<TH style="text-align: center" colspan="8"><p><span style="Font-Family:Monospace; margin: auto;">Current device stats</span></p></TH>
</TR>
</thead>

)";



String htmlData2 = R"(</p></TD>
<TH><p>Soil Temp:</p></TH>
<TD><p>NaN</p></TD>
<TH><p>Soil Hum:</p></TH>
<TD><p>NaN</p></TD>
</TR></tbody></table></div> <!-- end dataTable div --></div> <!-- End of main div container --></body></html>)";


void setup() {
    // put your setup code here, to run once:
  dht.begin();
  Serial.begin(9600);
  WiFi.begin(w_ssid, pass);
  while(WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    delay(1000);
  }
  Serial.println("Wifi is connected.");

  wfServer.begin();
  Serial.println("Web Server started.");


   Serial.println("Local IP: ");
  Serial.println(WiFi.localIP());



 //OneWire soilTempProbe(SOILTEMPPIN);
  


  /*


 
  Serial.print("Current Status: ");

   Serial.println(WiFi.status());

  Serial.print("Current RSSI: ");

  Serial.println(WiFi.RSSI());

  Serial.print("Number of local wifi networks: ");

  Serial.println(WiFi.scanNetworks());

*/





}

void loop() {
  // put your main code here, to run repeatedly:

float t = dht.readTemperature(true);
    WiFiClient incomingClientW = wfServer.available();
    if(incomingClientW) // Check if client has connected to server
    {
      Serial.println("New client connected");
      boolean currentLineIsBlank = true;
      char c {0};
      while(incomingClientW.connected()) // ONLY while client is connected, do stuff
        {
         if(incomingClientW.available())
         {
            c  = incomingClientW.read();
           Serial.write(c);
         }
                 // Send a reply if end of line detected
        if ( c == '\n' && currentLineIsBlank) {
          
          // Send a standard HTTP response header
          incomingClientW.println("HTTP/1.1 200 OK");
          incomingClientW.println("Content-Type: text/html");
          incomingClientW.println("Connection: close");
		      incomingClientW.println("Refresh: 5");  // Refresh the page automatically every 5 sec
          incomingClientW.println(htmlData);
          //incomingClientW.println("<tbody><TR><TH><p>Ambient Temperature(F): </p></TH><TD><p>");
          incomingClientW.println("<tbody><bold><TR style='font-family:Georgia, Didot, serif; color:white; font-size:22px; text-align: center'><TH><p>Ambient Temperature(F): </p></TH><TH><p>Ambient Humidity(%): </p></TH>");
          incomingClientW.println("<TH><p>Soil Temperature(F): </p></TH><TH><p>Soil Humidity: </p></TH></TR></bold>");
          

          incomingClientW.println("<TR style='font-family:Lucida Console, Helvetica, Monaco; color:white; font-size:27px; text-align: center'>");
          incomingClientW.println("<TD><P>");
          incomingClientW.println(dht.readTemperature(true));
          incomingClientW.println("</P></TD>");

          incomingClientW.println("<TD><P>");
          incomingClientW.println(dht.readHumidity(true));
          incomingClientW.println("</P></TD>");
        
         incomingClientW.println("<TD><P>");
          incomingClientW.println("NaN");
          incomingClientW.println("</P></TD>");


          incomingClientW.println("<TD><P>");
          incomingClientW.println(analogRead(HUMPIN));
          incomingClientW.println("</P></TD>");


        incomingClientW.println("</TR>");


          incomingClientW.println("</tbody></table></div> <!-- end dataTable div --></div> <!-- End of main div container --></body></html>");

          break;
        }
        if (c == '\n') {
          // you're starting a new line
          currentLineIsBlank = true;
        } 
        else if (c != '\r') {
          // you've gotten a character on the current line
          currentLineIsBlank = false;
        }
        } // End while loop cause cxn was lost
        Serial.println("Connection lost!");
    }
}
