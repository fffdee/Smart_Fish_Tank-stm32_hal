 #include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>   // 使用WiFiMulti库 
#include <ESP8266WebServer.h>   // 使用WebServer库
#include <Servo.h> 
#include<Wire.h>
const int MPU6050_addr=0x68;
int16_t AccX,AccY,AccZ,Temp,GyroX,GyroY,GyroZ;
Servo myServo;
Servo myServo1;
ESP8266WiFiMulti wifiMulti;     // 建立ESP8266WiFiMulti对象,对象名称是 'wifiMulti'
 
ESP8266WebServer server(80);    // 建立网络服务器对象，该对象用于响应HTTP请求。监听端口（80）
 
IPAddress local_IP(192, 168, 43, 125); // 设置开发板联网后的IP（这个就是填到app里面的IP地址）
IPAddress gateway(192, 168, 43, 1);    // 设置网关IP（通常网关IP是手机的IP）（开热点给开发板连的手机）
IPAddress subnet(255, 255, 255, 0);   // 设置子网掩码
IPAddress dns(192,168,43,1);           // 设置局域网DNS的IP（通常局域网DNS的IP是手机的IP）（开热点给开发板连的手机）
 
void setup(void){
  Serial.begin(115200);          // 启动串口通讯
  Serial.println("");
  Wire.begin(0,2);
  Wire.beginTransmission(MPU6050_addr);
  Wire.write(0x6B);
  Wire.write(0);
  Wire.endTransmission(true);
 
myServo.attach(2); //设置引脚2为舵机引脚
myServo1.attach(0);  
myServo1.write(90); 
  // 设置开发板网络环境
  if (!WiFi.config(local_IP, gateway, subnet)) {
    Serial.println("Failed to Config ESP8266 IP"); 
  } 
  
  wifiMulti.addAP("HONOR 20i", "88888888"); // 将需要连接的一系列WiFi ID和密码输入这里
  wifiMulti.addAP("ssid_from_AP_2", "your_password_for_AP_2"); // ESP8266-NodeMCU再启动后会扫描当前网络
  wifiMulti.addAP("ssid_from_AP_3", "your_password_for_AP_3"); // 环境查找是否有这里列出的WiFi ID。如果有
  Serial.println("Connecting ...");                            // 则尝试使用此处存储的密码进行连接。
  
  // 尝试进行wifi连接。
  while (wifiMulti.run() != WL_CONNECTED) { 
    delay(250);
    Serial.print('.');
  }
 
  // WiFi连接成功后将通过串口监视器输出连接成功信息 
  Serial.println('\n');
  Serial.print("Connected to ");
  Serial.println(WiFi.SSID());              // 通过串口监视器输出连接的WiFi名称
  Serial.print("IP address:\t");
  Serial.println(WiFi.localIP());           // 通过串口监视器输出ESP8266-NodeMCU的IP
 
  server.on("/control", handleUpdate);        // 处理服务器更新函数
  server.on("/MPU", MPU6050); 
  server.begin();                            // 启动网站服务
  Serial.println("HTTP server started");
}
 
void loop(void){
  server.handleClient(); // 检查http服务器访问
    Wire.beginTransmission(MPU6050_addr);
  Wire.write(0x3B);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU6050_addr,14,true);
  AccX=Wire.read()<<8|Wire.read();
  AccY=Wire.read()<<8|Wire.read();
  AccZ=Wire.read()<<8|Wire.read();
  Temp=Wire.read()<<8|Wire.read();
  GyroX=Wire.read()<<8|Wire.read();
  GyroY=Wire.read()<<8|Wire.read();
  GyroZ=Wire.read()<<8|Wire.read();
  Serial.println(AccX);
  delay(100);
}
void MPU6050(){
  server.send(200, "text/plain", "X:"+AccX);
}
//重点改这里就好了 

void handleUpdate(){
        // 获取客户端发送HTTP信息中的整数数值
  int fanValue = server.arg("fan").toInt();  // 获取客户端发送HTTP信息中的按键控制量
  int leftValue = server.arg("left").toInt(); 
  int rightValue = server.arg("right").toInt();
  int goValue = server.arg("go").toInt();
  int backValue = server.arg("back").toInt();              
 
   //风扇控制程序
   if(fanValue==1){
      Serial.print("开风扇");
      myServo.write(135);
      myServo1.write(135);//我这里用舵机测试
      //此处放车风扇的控制程序
   }else if(fanValue == 0){
      Serial.print("停");
      myServo.write(45);
      myServo1.write(45);
   }


  //电机控制程序

 
   if(goValue == 1){
      Serial.print("前进");
      //此处放车前进的控制程序
   }else if(goValue == 0){
      Serial.print("停");
      //放停止前进的程序
   }


   if(backValue == 1){
      Serial.print("后退");
      //此处放车后退的控制程序
   }else if(backValue == 0){
      Serial.print("停");
      //放停止前进的程序
   }

   if(leftValue== 1){
      Serial.print("左转");
      //此处放车左转的控制程序
   }else if(leftValue == 0){
      Serial.print("停");
      //放停止前进的程序
   }


   if(rightValue == 1){
      Serial.print("右转");
      //此处放车前进的控制程序
   }else if(rightValue == 0){
      Serial.print("停");
      //放停止前进的程序
   }
   server.send(200, "text/plain", "Received"); // 发送http响应
  // 通过串口监视器输出获取到的变量数值
  Serial.print("backValue = ");  Serial.println(backValue);  
  Serial.print("goValue = ");  Serial.println(goValue); 
  Serial.print("fanValue = ");  Serial.println(fanValue); 
  Serial.print("leftValue = ");  Serial.println(leftValue);
  Serial.print("rightValue = ");  Serial.println(rightValue);       
  Serial.println("=================");    
}
