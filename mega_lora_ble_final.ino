#include <LoRaFi.h>
#include <SoftwareSerial.h>

/*definition command to send*/
#define open_door       '1'
#define close_door      '0'
#define stop_door       '2'

/*definition received command*/
#define mov_detecte     '3'
#define mov_no_detecte  '4'
char mov_state;

/*definition LoRa variables */
LoRaFi LoRaFi;
const int messageLength = 1;  //message size
char message[messageLength];     //message buffer
char pre_message;
char pre_send;
char pre_recv;

/*definition ble variables */
SoftwareSerial ble(10,11);
char ble_cmd;
 
void setup() {
  /*Serial port init*/
  Serial.begin(9600);
  delay(100);
  /*BLE init*/
  ble.begin(9600);
  delay(100);
  ble_cmd = ' ';
  /*LoRa init*/
  LoRaFi.begin(53,9,8,7,18);
  delay(100);

  pre_send = 'F';
  pre_recv = 'F';
  Serial.println("Init done");
  
}

void loop() {
  /*LoRa communication*/
    LoRaFi.Send(ble_cmd); 
    LoRaFi.ReceivePackage(message,messageLength);

    if(pre_send != ble_cmd && ( ble_cmd == '1' || ble_cmd == '2' || ble_cmd == '0') ){
      Serial.print("Send message: ");
      Serial.println(ble_cmd);
      pre_send = ble_cmd;
    }
    
  /*ble transmission*/
  if(pre_recv != message[0]){
      Serial.print("Message received: ");
      Serial.println(message[0]);
      ble.print(message[0]);
      pre_recv = message[0];
  }
  /*ble reception*/
  if(ble.available()){
    ble_cmd = ble.read();
    if(ble_cmd == '1' || ble_cmd == '2' || ble_cmd == '0'){
      Serial.print("ble cmd: ");
      Serial.println(ble_cmd);
    }
  }  
}
