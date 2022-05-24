#include <HardwareSerial.h>    //导入ESP32串口操作库
#include <WiFi.h> //导入wifi库
#include <HTTPClient.h>
//测试需要用到的串口
HardwareSerial MySerial_stm32(1);  
unsigned short i;
char temp;

boolean send=1;

// WiFi数据初始化
void WiFi_Connect(){
  WiFi.begin("8T","12345678xly");
  //循环等待链接成功
  while(WiFi.status()!=WL_CONNECTED){
    delay(300);
    Serial.print(".");
  }
}

// 发发送邮件
void SendEmail(){

  // 发起http请求
  HTTPClient http;
  http.begin("https://xmxhxl.top/iot_kcsj/send_email");

  http.GET();


  http.end();

  Serial.print("successfully email!");
}

//清理串口读取缓存区的缓存
void clear_usart_buffer(){
  i = MySerial_stm32.available();
  if(i != 0){
    Serial.print("Clear cache...");
    Serial.println(MySerial_stm32.available());
    while(i--)
      MySerial_stm32.read();    //读取串口接收回来的数据但是不做处理只给与打印
    }
  else
    Serial.println("buffer area!!!");
  }
   
/*    关于串口读取
*/
void read_usart(){
  i = MySerial_stm32.available();  //返回目前串口接收区内的已经接受的数据量
  if(i != 0){
    Serial.print("Amount of data:");
    Serial.println(MySerial_stm32.available());
    while(i--){
      temp = MySerial_stm32.read();   //读取一个数据并且将它从缓存区删除
      Serial.print(temp);    //读取串口接收回来的数据但是不做处理只给与打印
      }
    Serial.println("");
    // 发送邮件
      if(send){
        SendEmail();
        send=0;
      }
    }else{
      send=1;
      Serial.println("No data!!!");
    }
  }

void setup() {
  Serial.begin(115200);
  //串口的开启，波特率，默认SERIAL_8N1为8位数据位、无校验、1位停止位，后面两个分别为 RXD,TXD 引脚
  MySerial_stm32.begin(115200, SERIAL_8N1, 14, 15);
  clear_usart_buffer();  //清空接收区缓存函数
    // wifi初始化
  WiFi_Connect();
  
}

void loop() {
  MySerial_stm32.println("usart test!!!");
  read_usart();
  delay(500);
  read_usart();
  delay(500);
  read_usart();
  delay(500);
}
