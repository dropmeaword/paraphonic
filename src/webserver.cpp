#include "webserver.h"

#include <Arduino.h>
#include <ArduinoJson.h>
#include "motor.h"
#include "ledstrip.h"
#include "log.h"

ESP8266WebServer server(80);

void parse_css_color(String ccode, int &r, int &g, int &b) {
    //string hexstring = "#FF3Fa0";
    int number = strtol( &ccode[1], NULL, 16);

    // Split them up into r, g, b values
    r = number >> 16;
    g = number >> 8 & 0xFF;
    b = number & 0xFF;
}

void post_result(String retval) {
    //server.sendHeader("Access-Control-Allow-Origin", "*");
    server.sendHeader("Access-Control-Max-Age", "10000");
    server.sendHeader("Access-Control-Allow-Methods", "POST,GET,OPTIONS");
    server.sendHeader("Access-Control-Allow-Headers", "Origin, X-Requested-With, Content-Type, Accept");    
    server.send(200, "text/plain", retval);
}

void handle_motor() {
    String posted = server.arg("plain");

    DynamicJsonBuffer jsonBuffer(4096);
    JsonObject& root = jsonBuffer.parseObject(posted);

    if (!root.success()) {
        Serial.println("JSON parsing failed!");
        server.send(500, "text/plain", "failed to parse incoming JSON");
        return;
    }

    Serial.print("Id = ");
    int id = root["motor"]["id"];
    Serial.println(id);

    Serial.print("State = ");
    String st = root["motor"]["state"];
    Serial.println(st);

    Serial.print("Time = ");
    int ms = root["motor"]["time"];
    Serial.println(ms);

    LOG("motor on");
    LOG_NEW_LINE

    motor_on(id, ms);

    post_result("{'result':'ok'}");
}

void handle_light_test() {
    ledstrip_test_pattern();
    post_result("{'result':'ok'}");
}

void handle_light_gradient() {
    post_result("{'result':'ok'}");
}

void handle_light_solid() {
    String posted = server.arg("plain");

    DynamicJsonBuffer jsonBuffer(4096);
    JsonObject& root = jsonBuffer.parseObject(posted);

    if (!root.success()) {
        Serial.println("JSON parsing failed!");
        server.send(500, "text/plain", "failed to parse incoming JSON");
        return;
    }

    Serial.print("solid = ");
    String solid = root["solid"];
    Serial.println(solid);

    int r, g, b;
    parse_css_color(solid, r, g, b);
    ledstrip_solid(r, g, b);


    post_result("{'result':'ok'}");
}

void handle_root() {
}

void handle_404(){
    server.send(404, "text/plain", "404: and then there was silence");
}

void web_init() {
    // action handlers
    server.on("/", handle_root);
    server.on("/light/test", handle_light_test);
    server.on("/light/solid", handle_light_solid);
    server.on("/light/gradient", handle_light_gradient);
    server.on("/spin",  handle_motor);
    // 404 handler    
    server.onNotFound(handle_404); 


    server.begin();
    Serial.println ( "HTTP server is listening" );
}

void web_loop() {
    server.handleClient();
}
