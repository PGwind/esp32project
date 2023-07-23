#include <WiFi.h>
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
 
//此处设置您的wifi接入信息
#ifndef STASSID
#define STASSID "ding_2.4"
#define STAPSK  "18360595755"
#endif
 
const char* ssid = STASSID;
const char* password = STAPSK;
 
//创建TCP server对象,并将端口指定为80
WiFiServer server(80);

// 创建PWM对象
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(); 

void setup() {
  //打开串口
  Serial.begin(115200);
 
  //初始化板载LED
  pinMode(LED_BUILTIN, OUTPUT);

  // 舵机
  pwm.begin();
  pwm.setPWMFreq(60); //伺服电机以60hz刷新率工作
 
  //连接WiFi
  Serial.println();
  Serial.println();
  Serial.print(F("Connecting to "));
  Serial.println(ssid);
 
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(F("."));
  }
  Serial.println();
  Serial.println(F("WiFi connected"));
 
  //启动TCP server服务器
  server.begin();
  Serial.println(F("Server started"));
 
  //打印IP地址
  Serial.println(WiFi.localIP());
}
 
void loop() {
  //检查客户端是否已连接
  WiFiClient client = server.available();
  if (!client) {
    return;
  }
 //等到客户端发送一些数据
  Serial.println("new client");
  while (!client.available()) {
    delay(1);
  }
 
  //读取客户端发起的TCP请求
  String request = client.readStringUntil('\r');
  Serial.println(F("request: "));
  Serial.println(request);
 
 // 匹配请求
  if (request.indexOf("/S") != -1) {
    LED();
    sleep();
  }
  if (request.indexOf("/W") != -1) {
    LED();
    normal();
  }
  if (request.indexOf("/F") != -1) {
    LED();
    normal();
    forward();
    forward();
    forward();
    forward();
  }
  if (request.indexOf("/B") != -1) {
    LED();
    normal();
    backward();
    backward();
    backward();
    backward();
  }
  if (request.indexOf("/L") != -1) {
    LED();
    normal();
    left();
    left();
  }
  if (request.indexOf("/R") != -1) {
    LED();
    normal();
    right();
    right();
  }
  if (request.indexOf("/H") != -1) {
    LED();
    normal();
    hello();
  }
 if (request.indexOf("/C") != -1) {
    LED();
    normal();
    come();
  }
 
  // 读取剩余的内容,用于清除缓存
  while (client.available()) {
   client.read();
  }
 
  // 网页端代码
  //将响应体发送到客户端 注:若要使用中文则必须在其中添加<meta charset=\"utf-8\">声明编码
  client.print(F("HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE HTML>\r\n<html>\r\n<meta charset=\"utf-8\">"));
  client.println("<head>");
  client.println("<style>");
  client.println(".button{");
  client.println("background-color: #4CAF50; /* GREEN */");
  client.println("width: 48%;");
  client.println("border: none;");
  client.println("color: white;");
  client.println("padding: 50px 40px;");
  client.println("text-align: center;");
  client.println("text-decoration: none;");
  client.println("display: inline-block;");
  client.println("font-size: 50px;");
  client.println("margin: 4px 2px;");
  client.println("cursor: pointer;");
  client.println("border-radius: 12px;");
  client.println("}");
  client.println("</style>");
  client.println("</head>");
  client.println("<body bgcolor=black>");
  client.println("<button class=\"button\" onclick=\"window.location.href='/F';\">前进</button>");
  client.println("<button class=\"button\" onclick=\"window.location.href='/B';\">后退</button>");
  client.println("<br></br>");
  client.println("<button class=\"button\" onclick=\"window.location.href='/L';\">左转</button>");
  client.println("<button class=\"button\" onclick=\"window.location.href='/R';\">右转</button>");
  client.println("<br></br>");
  client.println("<button class=\"button\" onclick=\"window.location.href='/S';\">睡觉</button>");
  client.println("<button class=\"button\" onclick=\"window.location.href='/W';\">正常</button>");
  client.println("<br></br>");
  client.println("<button class=\"button\" onclick=\"window.location.href='/H';\">你好</button>");
  client.println("<button class=\"button\" onclick=\"window.location.href='/C';\">来啊</button>");
  client.println("</body>");
  client.println("</html>");
 
  //通过浏览器点击按钮之后,TCP连接将会被重置,并且浏览器会发起新的TCP请求到ESP32服务器,从而去控制舵机
  Serial.println(F("Disconnecting from client"));
}


/*----------动作函数----------*/
 
// |   7  |             |   6  |
//  ----- -----   ----- -----
//       |  3   | |  2   |
//        -----   -----
//       |   1  | |  0   |
//  ----- -----   ----- -----
// |  5   |             |  4   |
 
void LED()
{
  digitalWrite(LED_BUILTIN, HIGH);
  delay(100);
  digitalWrite(LED_BUILTIN, LOW);
}

void sleep() 
{
  pwm.setPWM(4, 0, 280);
  delay(100);
  pwm.setPWM(5, 0, 280);
  delay(100);
  pwm.setPWM(6, 0, 280);
  delay(100);
  pwm.setPWM(7, 0, 280);
  delay(100);
  pwm.setPWM(0, 0, 510);
  delay(100);
  pwm.setPWM(1, 0, 100);
  delay(100);
  pwm.setPWM(2, 0, 100);
  delay(100);
  pwm.setPWM(3, 0, 510);
}
 
void normal() 
{
  pwm.setPWM(0, 0, 370);
  delay(100);
  pwm.setPWM(1, 0, 190);
  delay(100);
  pwm.setPWM(2, 0, 190);
  delay(100);
  pwm.setPWM(3, 0, 370);
  delay(100);
  pwm.setPWM(4, 0, 510);
  delay(100);
  pwm.setPWM(7, 0, 100);
  delay(100);
  pwm.setPWM(6, 0, 510);
  delay(100);
  pwm.setPWM(5, 0, 100);
}
 
void left()
{
  pwm.setPWM(4, 0, 370);
  delay(100);
  pwm.setPWM(0, 0, 510);
  delay(100);
  pwm.setPWM(4, 0, 510);
  delay(100);
  pwm.setPWM(6, 0, 370);
  delay(100);
  pwm.setPWM(2, 0, 280);
  delay(100);
  pwm.setPWM(6, 0, 510);
  delay(100);
  pwm.setPWM(7, 0, 190);
  delay(100);
  pwm.setPWM(3, 0, 510);
  delay(100);
  pwm.setPWM(7, 0, 100);
  delay(100);
  pwm.setPWM(5, 0, 190);
  delay(100);
  pwm.setPWM(1, 0, 280);
  delay(100);
  pwm.setPWM(5, 0, 100);
  delay(100);
  pwm.setPWM(0, 0, 370);
  pwm.setPWM(2, 0, 190);
  pwm.setPWM(3, 0, 370);
  pwm.setPWM(1, 0, 190);
}
 
void right()
{
  pwm.setPWM(5, 0, 190);
  delay(100);
  pwm.setPWM(1, 0, 100);
  delay(100);
  pwm.setPWM(5, 0, 100);
  delay(100);
  pwm.setPWM(7, 0, 190);
  delay(100);
  pwm.setPWM(3, 0, 280);
  delay(100);
  pwm.setPWM(7, 0, 100);
  delay(100);
  pwm.setPWM(6, 0, 370);
  delay(100);
  pwm.setPWM(2, 0, 100);
  delay(100);
  pwm.setPWM(6, 0, 510);
  delay(100);
  pwm.setPWM(4, 0, 370);
  delay(100);
  pwm.setPWM(0, 0, 190);
  delay(100);
  pwm.setPWM(4, 0, 510);
  delay(100);
  pwm.setPWM(1, 0, 190);
  pwm.setPWM(3, 0, 370);
  pwm.setPWM(2, 0, 190);
  pwm.setPWM(0, 0, 370);
}
 
void forward()
{ 
  pwm.setPWM(5, 0, 190);
  delay(50);
  pwm.setPWM(1, 0, 100);
  delay(50);
  pwm.setPWM(5, 0, 100);
  delay(50);
  pwm.setPWM(4, 0, 370);
  delay(50);
  pwm.setPWM(0, 0, 510);
  delay(50);
  pwm.setPWM(4, 0, 510);
  delay(50);
  pwm.setPWM(6, 0, 370);
  delay(50);
  pwm.setPWM(2, 0, 280);
  delay(50);
  pwm.setPWM(6, 0, 510);
  delay(50);
  pwm.setPWM(7, 0, 190);
  delay(50);
  pwm.setPWM(3, 0, 280);
  delay(50);
  pwm.setPWM(7, 0, 100);
  delay(50);
  pwm.setPWM(0, 0, 370);
  pwm.setPWM(1, 0, 190);
  pwm.setPWM(2, 0, 190);
  pwm.setPWM(3, 0, 370);
}
 
void backward()
{ 
  pwm.setPWM(7, 0, 190);
  delay(50);
  pwm.setPWM(3, 0, 510);
  delay(50);
  pwm.setPWM(7, 0, 100);
  delay(50);
  pwm.setPWM(6, 0, 370);
  delay(50);
  pwm.setPWM(2, 0, 100);
  delay(50);
  pwm.setPWM(6, 0, 510);
  delay(50);
  pwm.setPWM(5, 0, 190);
  delay(50);
  pwm.setPWM(1, 0, 280);
  delay(50);
  pwm.setPWM(5, 0, 100);
  delay(50);
  pwm.setPWM(4, 0, 370);
  delay(50);
  pwm.setPWM(0, 0, 280);
  delay(50);
  pwm.setPWM(4, 0, 510);
  delay(50);
  pwm.setPWM(3, 0, 370);
  pwm.setPWM(2, 0, 190);
  pwm.setPWM(1, 0, 190);
  pwm.setPWM(0, 0, 370);
}
 
void hello()
{
  pwm.setPWM(0, 0, 510);
  delay(100);
  pwm.setPWM(4, 0, 100);
  delay(500);
  pwm.setPWM(4, 0, 190);
  delay(500);
  pwm.setPWM(4, 0, 100);
  delay(500);
  pwm.setPWM(4, 0, 190);
  delay(500);
  pwm.setPWM(4, 0, 100);
  delay(500);
  pwm.setPWM(4, 0, 510);
}
 
void come()
{
  pwm.setPWM(0, 0, 280);
  delay(100);
  pwm.setPWM(4, 0, 100);
  delay(500);
  pwm.setPWM(4, 0, 190);
  delay(500);
  pwm.setPWM(4, 0, 100);
  delay(500);
  pwm.setPWM(4, 0, 190);
  delay(500);
  pwm.setPWM(4, 0, 100);
  delay(500);
  pwm.setPWM(4, 0, 510);
}