#include <WiFi.h>
#include "ota.h"
#include <esp_ping.h>
#include <HTTPClient.h>
#include <ESPping.h>
const char* baidu_host = "baidu.com";
const char* serverUrl = "http://www.baidu.com/";
String redirectLocation;
void bupt_login(String BUPTACC, String BUPTPASS) {
  display.clear();
  display.setFont(ArialMT_Plain_16);
  display.drawString(64, 10, "BUPT");
  display.drawString(64, 30, "AUTO LOGIN");
  display.drawString(64, 45, BUPTACC);
  display.display();
  delay(1000);
  String postData = "user=" + BUPTACC + "&pass=" + BUPTPASS;
  while (1) {
    String payload;
    String redirectUrl;
    HTTPClient http;
    http.begin(serverUrl);
    http.setTimeout(100000);
    int httpCode = http.GET();
    Serial.println(httpCode);
    Serial.println(http.getString());
    display.clear();
  display.drawString(64, 10, "REDIRECTION");
  display.drawString(64, 30, "ONE");
  display.drawString(64, 45, "CODE:"+String(httpCode));
  display.display();
    delay(100);
    //String newURL = Apple_http.header("Location");
    http.end();
    
    http.begin("http://www.baidu.com/?cmd=redirect&arubalp=12345");
    const char *headerKeys[] = {"Server", "Content-Type", "Date", "Location","Set-Cookie"};
    http.collectHeaders(headerKeys, sizeof(headerKeys) / sizeof(headerKeys[0]));
    httpCode = http.GET();
    Serial.println(httpCode);   
    for (int i = 0; i < http.headers(); i++) {
      Serial.printf("%s: %s\n", http.headerName(i).c_str(), http.header(i).c_str());
        if (strcmp(http.headerName(i).c_str(), "Location") == 0) {
        redirectLocation = http.header(i);
        if (redirectLocation.length() == 0) {
          Serial.println("You already login so there is no redirection.");
          display.clear();
          display.drawString(64, 10, "NO");
          display.drawString(64, 30, "REDIRECTION");
          display.drawString(64, 45, "Already LOGIN");
          display.display();
          delay(1000);
          http.end();
          return;
        }
        Serial.printf("Redirect Location: %s\n", redirectLocation.c_str());
      }
    }
        display.clear();
  display.drawString(64, 10, "REDIRECTION");
  display.drawString(64, 30, "TWO");
  display.drawString(64, 45, "CODE:"+String(httpCode));
  display.display();
    delay(1000);
    //String newURL = Apple_http.header("Location");
    http.end();
    
    http.begin(redirectLocation);
    http.collectHeaders(headerKeys, sizeof(headerKeys) / sizeof(headerKeys[0]));
    httpCode = http.GET();
    String receivedCookies = http.header("Set-Cookie");
    String sessionId;
    int semicolonIndex = receivedCookies.indexOf(';');
    if (semicolonIndex != -1) {
      sessionId = receivedCookies.substring(0, semicolonIndex);
    }
    Serial.printf("Session id: %s\n", sessionId.c_str());
            display.clear();
  display.drawString(64, 10, "REDIRECTION");
  display.drawString(64, 30, "THREE  SID:");
  display.drawString(64, 45, sessionId);
  display.display();
    http.end();
    
    display.clear();
  display.drawString(64, 10, "GOT COOKIE");
  display.drawString(64, 30, "LAUNCH POST..");
  display.display();
    http.begin("http://10.3.8.216/login");
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");
    http.addHeader("Cookie", sessionId);
    http.addHeader("User-Agent", "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/120.0.0.0 Safari/537.36 Edg/120.0.0.0");
    int httpResponseCode = http.POST(postData);
    if (httpResponseCode > 0) {
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);

      // Print the response payload
      String response = http.getString();
     // Serial.println("Response: " + response);
    } else {
      Serial.print("HTTP Request failed. Error code: ");
      Serial.println(httpResponseCode);
      Serial.println(http.errorToString(httpResponseCode).c_str());
    }
    http.end();
        display.clear();
  display.drawString(64, 10, "FINISHED");
  display.drawString(64, 30, "WELCOME");
  display.drawString(64, 45, BUPTACC);
  display.display();
    delay(1000);
    break;
  }
}
