#include "WiFiEsp.h"
#include <Servo.h>

char ssid[] = "ssid";            // your network SSID (name)
char pass[] = "password";        // your network password
int status = WL_IDLE_STATUS;     // the Wifi radio's status
int reqCount = 0;                // number of requests received
long passwd;

WiFiEspServer server(80);
Servo locker;

void printWifiStatus();
long makeRandPasswd();
void startWebBrowser(int passwd);
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
  
  passwd = makeRandPasswd();
  
  server.begin();
  servoLock();
}


void loop()
{
  startWebBrowser(passwd);
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

long makeRandPasswd(){
  long randPasswd;
  randomSeed(analogRead(0));
  randPasswd = random(10000);
  return randPasswd;
}

void startWebBrowser(long passwd){
  WiFiEspClient client = server.available();
  if (client) {
    Serial.println("New client");
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);
        if (c == '\n' && currentLineIsBlank) {
          Serial.println("Sending response");
          client.print(
            "HTTP/1.1 200 OK\r\n"
            "Content-Type: text/html\r\n"
            "Connection: close\r\n"  // the connection will be closed after completion of the response
            "Refresh: 20\r\n"        // refresh the page automatically every 20 sec
            "\r\n");
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
          client.print("<div align=\"center\">\r\n");
          client.print("<h4>Password</h4>\r\n");
          client.print("<h2>");
          client.print(passwd);
          client.print("</h2>\r\n");
          client.print("<p>기기의 버튼을 눌러 비밀번호를 입력해주세요!</p>\r\n");
          client.print("</div>\r\n");
          client.print("</body>\r\n");
          client.print("</html>\r\n");
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
      }
    }
    delay(10);
    client.stop();
    Serial.println("Client disconnected");
  }
}

void servoLock(){
  locker.write(180);
}

void servoUnlock(){
  locker.write(0);
}
