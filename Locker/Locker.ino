#include "WiFiEsp.h"
#include <Servo.h>

char ssid[] = "Galaxy S21 Ultra 5G";            // your network SSID (name)
char pass[] = "password";        // your network password
int status = WL_IDLE_STATUS;     // the Wifi radio's status
int reqCount = 0;                // number of requests received
bool servoStatus = false;

WiFiEspServer server(80);
RingBuffer buf(8);
Servo locker;

void printWifiStatus();
void sendHttpResponse(WiFiEspClient client);
void servoLock();
void servoUnlock();

void setup()
{
  Serial.begin(115200);
  Serial1.begin(9600);
  WiFi.init(&Serial1);
  locker.attach(7);

  // check for the presence of the shield
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    // don't continue
    while (true);
  }

  // attempt to connect to WiFi network
  while ( status != WL_CONNECTED) {
    Serial.print("Attempting to connect to WPA SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network
    status = WiFi.begin(ssid, pass);
  }

  Serial.println("You're connected to the network");
  printWifiStatus();
  server.begin();
  servoUnlock();
}


void loop()
{
  WiFiEspClient client = server.available();
  if (client) {                               // if you get a client,
    Serial.println("New client");             // print a message out the serial port
    buf.init();                               // initialize the circular buffer
    while (client.connected()) {              // loop while the client's connected
      if (client.available()) {               // if there's bytes to read from the client,
        char c = client.read();               // read a byte, then
        buf.push(c);                          // push it to the ring buffer

        // printing the stream to the serial monitor will slow down
        // the receiving of data from the ESP filling the serial buffer
        //Serial.write(c);
        
        // you got two newline characters in a row
        // that's the end of the HTTP request, so send a response
        if (buf.endsWith("\r\n\r\n")) {
          sendHttpResponse(client);
          break;
        }

        if (buf.endsWith("GET /PW")) {
          if (servoStatus){
            servoUnlock();
          }
          else{
            servoLock();
          }
        }
      }
    }
    
    // close the connection
    client.stop();
    Serial.println("Client disconnected");
  }
}

void printWifiStatus()
{
  // print the SSID of the network you're attached to
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);
  
  // print where to go in the browser
  Serial.println();
  Serial.print("To see this page in action, open a browser to http://");
  Serial.println(ip);
  Serial.println();
}

void sendHttpResponse(WiFiEspClient client)
{
  client.println("HTTP/1.1 200 OK");
  client.println("Content-type:text/html");
  client.println();
  client.print("<!DOCTYPE html>\r\n");
  client.print("<html lang=\"en\">\r\n");
  client.print("<head>\r\n");
  client.print("<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\r\n");
  client.print("<meta charset=\"UTF-8\">\r\n");
  client.print("<title>Line Delever Password</title>\r\n");
  client.print("<link href=\"https://cdn.jsdelivr.net/npm/bootstrap@5.1.3/dist/css/bootstrap.min.css\" rel=\"stylesheet\" integrity=\"sha384-1BmE4kWBq78iYhFldvKuhfTAU6auU8tT94WrHftjDbrCEXSU1oBoqyl2QvZ6jIW3\" crossorigin=\"anonymous\">\r\n");
  client.print("</head>\r\n");
  client.print("<body>\r\n");
  client.print("<header class=\"d-flex flex-wrap justify-content-center py-3 mb-4 border-bottom\">\r\n");
  client.print("<a href=\"/\" class=\"d-flex align-items-center mb-3 mb-md-0 me-md-auto text-dark text-decoration-none\">\r\n");
  client.print("<span class=\"fs-4\">Line Delever</span>\r\n");
  client.print("</a>\r\n");
  client.print("</header>\r\n");
  if (servoStatus){
    client.print("<div align=\"center\">\r\n");
    client.println("<p>현재 상태</p>");
    client.println("<div class=\"fs-2 mb-3\">");
    client.println("<svg xmlns=\"http://www.w3.org/2000/svg\" width=\"16\" height=\"16\" fill=\"currentColor\" class=\"bi bi-lock\" viewBox=\"0 0 16 16\">");
    client.println("<path d=\"M8 1a2 2 0 0 1 2 2v4H6V3a2 2 0 0 1 2-2zm3 6V3a3 3 0 0 0-6 0v4a2 2 0 0 0-2 2v5a2 2 0 0 0 2 2h6a2 2 0 0 0 2-2V9a2 2 0 0 0-2-2zM5 8h6a1 1 0 0 1 1 1v5a1 1 0 0 1-1 1H5a1 1 0 0 1-1-1V9a1 1 0 0 1 1-1z\"/>");
    client.println("</svg>");
    client.println("Lock");
    client.println("</div>");
    client.print("<p>아래 버튼을 눌러 잠금을 해제해주세요!</p>\r\n");
    client.println("<form action=\"/PW\" method=\"get\">");
    client.println("<button type=\"submit\" style=\"border:none\" class=\"w-50 btn btn-success\">");
    client.println("<svg xmlns=\"http://www.w3.org/2000/svg\" width=\"16\" height=\"16\" fill=\"currentColor\" class=\"bi bi-lock\" viewBox=\"0 0 16 16\">");
    client.println("<path d=\"M11 1a2 2 0 0 0-2 2v4a2 2 0 0 1 2 2v5a2 2 0 0 1-2 2H3a2 2 0 0 1-2-2V9a2 2 0 0 1 2-2h5V3a3 3 0 0 1 6 0v4a.5.5 0 0 1-1 0V3a2 2 0 0 0-2-2zM3 8a1 1 0 0 0-1 1v5a1 1 0 0 0 1 1h6a1 1 0 0 0 1-1V9a1 1 0 0 0-1-1H3z\"/>");
    client.println("</svg>");
    client.println("잠금 해제하기");
    client.println("</button>");
    client.println("</form>");
    client.println("</div>");
  }
  else{
    client.print("<div align=\"center\">\r\n");
    client.println("<p>현재 상태</p>");
    client.println("<div class=\"fs-2 mb-3\">");
    client.println("<svg xmlns=\"http://www.w3.org/2000/svg\" width=\"16\" height=\"16\" fill=\"currentColor\" class=\"bi bi-lock\" viewBox=\"0 0 16 16\">");
    client.println("<path d=\"M11 1a2 2 0 0 0-2 2v4a2 2 0 0 1 2 2v5a2 2 0 0 1-2 2H3a2 2 0 0 1-2-2V9a2 2 0 0 1 2-2h5V3a3 3 0 0 1 6 0v4a.5.5 0 0 1-1 0V3a2 2 0 0 0-2-2zM3 8a1 1 0 0 0-1 1v5a1 1 0 0 0 1 1h6a1 1 0 0 0 1-1V9a1 1 0 0 0-1-1H3z\"/>");
    client.println("</svg>");
    client.println("Unlock");
    client.println("</div>");
    client.print("<p>아래 버튼을 눌러 잠금장치를 설정해주세요!</p>\r\n");
    client.println("<form action=\"/PW\" method=\"get\">");
    client.println("<button type=\"submit\" style=\"border:none\" class=\"w-50 btn btn-warning\">");
    client.println("<svg xmlns=\"http://www.w3.org/2000/svg\" width=\"16\" height=\"16\" fill=\"currentColor\" class=\"bi bi-lock\" viewBox=\"0 0 16 16\">");
    client.println("<path d=\"M8 1a2 2 0 0 1 2 2v4H6V3a2 2 0 0 1 2-2zm3 6V3a3 3 0 0 0-6 0v4a2 2 0 0 0-2 2v5a2 2 0 0 0 2 2h6a2 2 0 0 0 2-2V9a2 2 0 0 0-2-2zM5 8h6a1 1 0 0 1 1 1v5a1 1 0 0 1-1 1H5a1 1 0 0 1-1-1V9a1 1 0 0 1 1-1z\"/>");
    client.println("</svg>");
    client.println("잠그기");
    client.println("</button>");
    client.println("</form>");
    client.println("</div>");
  }
  client.print("</body>\r\n");
  client.print("</html>\r\n");
}

void servoLock(){
  Serial.println("잠금완료!!!!!!!!!!!");
  servoStatus = true;
  locker.write(180);
}

void servoUnlock(){
  Serial.println("잠금해제!!!!!!!!!!!");
  servoStatus = false;
  locker.write(0);
}
