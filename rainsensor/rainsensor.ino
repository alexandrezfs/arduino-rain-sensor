#include <SPI.h>
#include <WiFi101.h>

char ssid[] = "SFR_0938";
char pass[] = "anxtr9iankethydayber";
int inPin = 12;
int value = 0;
int ledPin = 6;

bool isRaining = false;
long supposedStoppedRainingTimestamp = 0;
char postDataStoppedRaining[] = "stoppedRaining=true";

// Initialize the Wifi client library
WiFiClient client;

// server address:
char server[] = "erp.librairielabourse.fr";

int status = WL_IDLE_STATUS;

void setup() {
  
    pinMode(inPin, INPUT);

    // check for the presence of the shield:
    if (WiFi.status() == WL_NO_SHIELD) {
      Serial.println("WiFi shield not present");
      // don't continue:
      while (true);
    }

}

void loop() {

  value = digitalRead(inPin);
  
  //Not raining
  if(value == 1) {

      Serial.println("Water not detected...");

      if (isRaining && supposedStoppedRainingTimestamp == 0) {
          Serial.println("It maybe stopped raining... let's wait some time.");
          supposedStoppedRainingTimestamp = millis();
      }

      if (supposedStoppedRainingTimestamp > 0 && millis() > (supposedStoppedRainingTimestamp + 10*1000)) {
          Serial.println("It surely stopped raining. Calling server...");
          httpRequestRetry(true);
          supposedStoppedRainingTimestamp = 0;
          isRaining = false;
      }
     
  }
  //Raining
  else if(value == 0) {
    
      Serial.println("Water detected !");

      if(supposedStoppedRainingTimestamp > 0) {
          isRaining = true;
          supposedStoppedRainingTimestamp = 0;
      }
      else if(!isRaining && supposedStoppedRainingTimestamp == 0) {
          isRaining = true;
          Serial.println("Calling server...");
          httpRequestRetry(false);
      }
  }

   delay(1000);
}

bool httpRequestRetry(bool stoppedRaining) {
    
    while(!httpRequest(false)) {
       Serial.println("Call to server failed. Trying to reconnect and send http request...");
    }
}

// this method makes a HTTP connection to the server:
bool httpRequest(bool stoppedRaining) {
    
    wifiDisconnect();
    connectToWifi();
    
    // close any connection before send a new request.
    // This will free the socket on the WiFi shield
    client.stop();
  
    // if there's a successful connection:
    if (client.connect(server, 80)) {
      Serial.println("Connecting...");
   
      client.println("POST http://erp.librairielabourse.fr/postcall/rainsensor.postcall.php HTTP/1.1");
  
      client.println("Host: librairielabourse.fr");
      client.println("User-Agent: Arduino/1.0");
      client.println("Connection: close");
      client.println("Content-Type: application/x-www-form-urlencoded;");
  
      if(stoppedRaining) {
        client.print("Content-Length: ");
        client.print(strlen(postDataStoppedRaining));
        client.println();
        client.println(postDataStoppedRaining);  
      }
      else {
        client.print("Content-Length: ");
        client.println(0);
        client.println();
      }
      
      Serial.println("Connection has been sent !");
      return true;
    }
    else {
      Serial.println("connection failed");
      return false;
    }
  
    wifiDisconnect();  
}

bool connectToWifi() {

  // attempt to connect to Wifi network:
  while ( status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    status = WiFi.begin(ssid, pass);

    // wait 10 seconds for connection:
    delay(10000);

    if(status == WL_CONNECTED) {
      return true;
    }
  }
  
}

void wifiDisconnect() {

  WiFi.disconnect(); 
  status = WiFi.status();
}
