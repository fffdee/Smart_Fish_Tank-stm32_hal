 #include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>   // 使用WiFiMulti库 
#include <ESP8266WebServer.h>   // 使用WebServer库
#include <Servo.h> 
#include <EEPROM.h>
#include "StringSplitter.h"
#include <WiFiUdp.h>







ESP8266WiFiMulti wifiMulti;     // 建立ESP8266WiFiMulti对象,对象名称是 'wifiMulti'
 
ESP8266WebServer server(80);    // 建立网络服务器对象，该对象用于响应HTTP请求。监听端口（80）
 
IPAddress local_IP(192, 168, 190, 125); // 设置开发板联网后的IP（这个就是填到app里面的IP地址）
IPAddress gateway(192, 168, 190, 1);    // 设置网关IP（通常网关IP是手机的IP）（开热点给开发板连的手机）
IPAddress subnet(255, 255, 255, 0);   // 设置子网掩码
IPAddress dns(192,168,190,1);           // 设置局域网DNS的IP（通常局域网DNS的IP是手机的IP）（开热点给开发板连的手机）

int servo1Value=90;
int servo2Value=90;

int speedValue=200;
int anglexValue = 90;
int angleyValue = 90;
void setup(void){
  
  Serial.begin(9600);          // 启动串口通讯
  EEPROM.begin(25);


  if (!WiFi.config(local_IP, gateway, subnet)) {

  } 
  
  wifiMulti.addAP("BanGO", "88888888"); // 将需要连接的一系列WiFi ID和密码输入这里
  wifiMulti.addAP("ssid_from_AP_2", "your_password_for_AP_2"); // ESP8266-NodeMCU再启动后会扫描当前网络
  wifiMulti.addAP("ssid_from_AP_3", "your_password_for_AP_3"); // 环境查找是否有这里列出的WiFi ID。如果有
                            // 则尝试使用此处存储的密码进行连接。
  
  // 尝试进行wifi连接。
  while (wifiMulti.run() != WL_CONNECTED) { 
    delay(250);

  }
  Serial.println("C");

 
  server.on("/control", handleUpdate);        // 处理服务器更新函数
  server.on("/arm", arm);        // 处理服务器更新函数
  server.on("/hand", hand);        // 处理服务器更新函数
  server.on("/speed", Setspeed);
  server.begin();                            // 启动网站服务

  delay(1000);
}
 
void loop(void){
  server.handleClient(); // 检查http服务器访问


  delay(100);
}



//重点改这里就好了 
void arm(){
   anglexValue = server.arg("anglex").toInt(); 
  
   Serial.print("anglexValue = ");  Serial.println(anglexValue);
}

void hand(){
   angleyValue = server.arg("angley").toInt(); 

   Serial.print("angleyValue = ");  Serial.println(angleyValue);
}

void Setspeed(){
   speedValue = server.arg("speed").toInt(); 
  
   Serial.print("speedValue = ");  Serial.println(speedValue);
}

void handleUpdate(){
        // 获取客户端发送HTTP信息中的整数数值
  int leftupValue = server.arg("leftup").toInt();  // 获取客户端发送HTTP信息中的按键控制量
  int leftdownValue = server.arg("leftdown").toInt(); 
  int rightupValue = server.arg("right").toInt();
  int rightdownValue = server.arg("rightdown").toInt();
  int stopValue = server.arg("stop").toInt();              

 

  if(leftupValue == 1){
       Serial.println("b");
   }

  if(leftdownValue == 1){

      Serial.println("l");
  }
  if(stopValue == 1){
      
      Serial.println("s");
  
  }

  if(rightupValue == 1){
      Serial.println("U");
  }

  if(rightdownValue == 1){
      Serial.println("K");
  }
  
   server.send(200, "text/plain", "Received"); // 发送http响应
  // 通过串口监视器输出获取到的变量数值
  Serial.print("stopValue = ");  Serial.println(stopValue);  
  Serial.print("rightdownValue = ");  Serial.println(rightdownValue); 
  Serial.print("leftupValue = ");  Serial.println(leftupValue); 
  Serial.print("leftdownValue = ");  Serial.println(leftdownValue);
  Serial.print("rightupValue = ");  Serial.println(rightupValue);       
 Serial.print("speedValue = ");  Serial.println(speedValue);
  Serial.println("=================");    
}
