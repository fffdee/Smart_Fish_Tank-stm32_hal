
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>


//巴法云服务器地址默认即可
#define TCP_SERVER_ADDR "bemfa.com"
//服务器端口，tcp创客云端口8344
#define TCP_SERVER_PORT "8344"

//********************需要修改的部分*******************//

#define DEFAULT_STASSID  "UFI_CE8D0E"     //WIFI名称，区分大小写，不要写错
#define DEFAULT_STAPSW   "12345678"  //WIFI密码
String UID = "c03b0385d6468c31245bd9f86236fc4d";  //用户私钥，可在控制台获取,修改为自己的UID
String TOPIC =   "fishtank";         //主题名字，可在控制台新建
const int LED_Pin = 2;              //单片机LED引脚值，GPIO0引脚

//**************************************************//
StaticJsonDocument<200> jsonBuffer2;


//最大字节数
#define MAX_PACKETSIZE 512
//设置心跳值30s
#define KEEPALIVEATIME 60*1000

#define upDataTime 2*1000

//tcp客户端相关初始化，默认即可
WiFiClient TCPclient;
String TcpClient_Buff = "";
unsigned int TcpClient_BuffIndex = 0;
unsigned long TcpClient_preTick = 0;
unsigned long preHeartTick = 0;//心跳
unsigned long preTCPStartTick = 0;//连接
bool preTCPConnected = false;



//相关函数初始化
//连接WIFI
void doWiFiTick();
void startSTA();

//TCP初始化连接
void doTCPClientTick();
void startTCPClient();
void sendtoTCPServer(String p);

//led 控制函数
void turnOnLed();
void turnOffLed();

typedef struct{
  
   // unsigned char *wifi_ip;
    uint8_t motorFlag1;
    uint8_t motorFlag2;
    uint8_t waterFlag;
    uint8_t hotFlag;
    uint8_t temp;
    uint8_t ptemp;
    uint32_t zhuod;
    uint32_t pzhuod;
     
    
}Fishtank;

Fishtank fishtank_data={0};

unsigned char MotorFlag1;
unsigned char MotorFlag2;
unsigned char MotorFlag3;
unsigned char hotFlag;
    
unsigned char temp;
unsigned char ptemp;
uint32_t zhuod;
uint32_t pzhuod;
/*
  *发送数据到TCP服务器
 */
void sendtoTCPServer(String p){
  
  if (!TCPclient.connected()) 
  {
    //Serial.println("Client is not readly");
    return;
  }
  TCPclient.print(p);
  //Serial.println("[Send to TCPServer]:String");
  //Serial.println(p);
  preHeartTick = millis();//心跳计时开始，需要每隔60秒发送一次数据
}


/*
  *初始化和服务器建立连接
*/
void startTCPClient(){
  if(TCPclient.connect(TCP_SERVER_ADDR, atoi(TCP_SERVER_PORT))){
   // Serial.print("\nConnected to server:");
   // Serial.printf("%s:%d\r\n",TCP_SERVER_ADDR,atoi(TCP_SERVER_PORT));
    
    String tcpTemp="";  //初始化字符串
    tcpTemp = "cmd=1&uid="+UID+"&topic="+TOPIC+"\r\n"; //构建订阅指令
    sendtoTCPServer(tcpTemp); //发送订阅指令
    tcpTemp="";//清空
    
    preTCPConnected = true;
    TCPclient.setNoDelay(true);
  }
  else{
    //Serial.print("Failed connected to server:");
   // Serial.println(TCP_SERVER_ADDR);
    TCPclient.stop();
    preTCPConnected = false;
  }
  preTCPStartTick = millis();
}


/*
  *检查数据，发送心跳
*/
void doTCPClientTick(){
 //检查是否断开，断开后重连
   if(WiFi.status() != WL_CONNECTED) return;

  if (!TCPclient.connected()) {//断开重连

  if(preTCPConnected == true){

    preTCPConnected = false;
    preTCPStartTick = millis();
    //Serial.println();
   // Serial.println("TCP Client disconnected.");
    TCPclient.stop();
  }
  else if(millis() - preTCPStartTick > 1*1000)//重新连接
    startTCPClient();
  }
  else
  {
    if (TCPclient.available()) {//收数据
      char c =TCPclient.read();
      TcpClient_Buff +=c;
      TcpClient_BuffIndex++;
      TcpClient_preTick = millis();
      
      if(TcpClient_BuffIndex>=MAX_PACKETSIZE - 1){
        TcpClient_BuffIndex = MAX_PACKETSIZE-2;
        TcpClient_preTick = TcpClient_preTick - 200;
      }
    }
    if(millis() - preHeartTick >= KEEPALIVEATIME){//保持心跳
      preHeartTick = millis();
     // Serial.println("--Keep alive:");
      sendtoTCPServer("cmd=0&msg=keep\r\n");
    }
  }

//      if(millis() - preHeartTick >= upDataTime){//上传数据
//      preHeartTick = millis();
//      
//      /*********************数据上传*******************/
//      String upstr = "";
//      upstr = "cmd=2&uid="+UID+"&topic="+TOPIC+"&msg=#0#1\r\n";
//      sendtoTCPServer(upstr);
//      upstr = "";
//    }
    
  if((TcpClient_Buff.length() >= 1) && (millis() - TcpClient_preTick>=200))
  {//data ready
    TCPclient.flush();
    //Serial.print("Rev string: ");
    TcpClient_Buff.trim(); //去掉首位空格
    //Serial.println(TcpClient_Buff); //打印接收到的消息
    String getTopic = "";
    String getMsg = "";
    if(TcpClient_Buff.length() > 15){//注意TcpClient_Buff只是个字符串，在上面开头做了初始化 String TcpClient_Buff = "";
          //此时会收到推送的指令，指令大概为 cmd=2&uid=xxx&topic=light002&msg=off
          int topicIndex = TcpClient_Buff.indexOf("&topic=")+7; //c语言字符串查找，查找&topic=位置，并移动7位，不懂的可百度c语言字符串查找
          int msgIndex = TcpClient_Buff.indexOf("&msg=");//c语言字符串查找，查找&msg=位置
          getTopic = TcpClient_Buff.substring(topicIndex,msgIndex);//c语言字符串截取，截取到topic,不懂的可百度c语言字符串截取
          getMsg = TcpClient_Buff.substring(msgIndex+5);//c语言字符串截取，截取到消息
         /* Serial.print("topic:------");
          Serial.println(getTopic); //打印截取到的主题值
          Serial.print("msg:--------");
          Serial.println(getMsg);   //打印截取到的消息值*/
          DeserializationError error = deserializeJson(jsonBuffer2, getMsg);
         if (error) 
        {
            Serial.print(F("deserializeJson() failed: "));
            Serial.println(error.f_str());
            return;
        }
     
   /*     Serial.println(String(jsonBuffer2["temp"]));
        Serial.println(String(jsonBuffer2["Ptemp"]));
        Serial.println(String(jsonBuffer2["zhuod"]));
        Serial.println(String(jsonBuffer2["Pzhuod"]));
        Serial.println(String(jsonBuffer2["motor1"]));
        Serial.println(String(jsonBuffer2["motor2"]));
        Serial.println(String(jsonBuffer2["hot"]));
        Serial.println(String(jsonBuffer2["waterFlag"]));*/
        fishtank_data.temp = jsonBuffer2["temp"];
        fishtank_data.ptemp = jsonBuffer2["Ptemp"];
        fishtank_data.zhuod = jsonBuffer2["zhuod"];
        fishtank_data.pzhuod = jsonBuffer2["Pzhuod"];
        fishtank_data.motorFlag1 = jsonBuffer2["motor1"];
        fishtank_data.motorFlag2 = jsonBuffer2["motor2"];
        fishtank_data.hotFlag = jsonBuffer2["hot"];
        fishtank_data.waterFlag = jsonBuffer2["waterFlag"];
        digitalWrite(LED_Pin,LOW);
        byte b[sizeof(fishtank_data)];
        if(fishtank_data.motorFlag1 == 1){
             Serial.write("*");
             digitalWrite(LED_Pin,HIGH);
        }else{
            Serial.write("+");
            digitalWrite(LED_Pin,LOW);
        } 
       // memcpy(b,&fishtank_data,sizeof(Fishtank));
       // Serial.write(b, sizeof(Fishtank));
   }


   TcpClient_Buff="";
   TcpClient_BuffIndex = 0;
  }
}

void startSTA(){
  WiFi.disconnect();
  WiFi.mode(WIFI_STA);
  WiFi.begin(DEFAULT_STASSID, DEFAULT_STAPSW);
}



/**************************************************************************
                                 WIFI
***************************************************************************/
/*
  WiFiTick
  检查是否需要初始化WiFi
  检查WiFi是否连接上，若连接成功启动TCP Client
  控制指示灯
*/
void doWiFiTick(){
  static bool startSTAFlag = false;
  static bool taskStarted = false;
  static uint32_t lastWiFiCheckTick = 0;

  if (!startSTAFlag) {
    startSTAFlag = true;
    startSTA();
  //  Serial.printf("Heap size:%d\r\n", ESP.getFreeHeap());
  }

  //未连接1s重连
  if ( WiFi.status() != WL_CONNECTED ) {
    if (millis() - lastWiFiCheckTick > 1000) {
      lastWiFiCheckTick = millis();
    }
  }
  //连接成功建立
  else {
    if (taskStarted == false) {
      taskStarted = true;
    //  Serial.print("\r\nGet IP Address: ");
    //  Serial.println(WiFi.localIP());
      startTCPClient();
    }
  }
}


// 初始化，相当于main 函数
void setup() {
  Serial.begin(115200);
  pinMode(LED_Pin,OUTPUT);
  digitalWrite(LED_Pin,HIGH);
 // Serial.println("Beginning...");
}

//循环
void loop() {
  doWiFiTick();
  doTCPClientTick();
}
