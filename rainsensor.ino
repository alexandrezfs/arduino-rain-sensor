#include <SPI.h>
#include <WiFi101.h>

char ssid[] = "SFR_0938";
char pass[] = "anxtr9iankethydayber";
int inPin = 12;
int value = 0;
int ledPin = 6;

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
  //Serial.print("digital read ");
  //Serial.println(value);

  //Not raining
  if(value == 1) {
    
  }
  //Raining
  else if(value == 0) {
    httpRequest();
  }
}


// this method makes a HTTP connection to the server:
void httpRequest() {

  connectToWifi();
  
  // close any connection before send a new request.
  // This will free the socket on the WiFi shield
  client.stop();

  // if there's a successful connection:
  if (client.connect(server, 80)) {
    Serial.println("connecting...");
 
    // EDIT: The POST 'URL' to the location of your insert_mysql.php on your web-host
    client.println("POST /postcall/rainsensor.postcall.php HTTP/1.1");

    // EDIT: 'Host' to match your domain
    client.println("Host: librairielabourse.fr");
    client.println("User-Agent: Arduino/1.0");
    client.println("Connection: close");
    client.println("Content-Type: application/x-www-form-urlencoded;");
    client.print("Content-Length: ");
    client.println(0);
    client.println();
  }
  else {
    // if you couldn't make a connection:
    Serial.println("connection failed");
  }

  WiFi.disconnect(); 
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

