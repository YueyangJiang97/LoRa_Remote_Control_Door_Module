#include <LoRaFi.h>
#include <SoftwareSerial.h>

/*definition command to send*/
#define close_door      '0'
#define open_door       '1'
#define stop_door       '2'
/*door state definition*/
#define door_closed     '0'
#define door_opened     '1'
#define door_error      '2'
/*definition received command*/
#define mov_detecte     '3'
#define mov_no_detecte  '4'
char mov_state;

/*definition LoRa variables */
LoRaFi LoRaFi;
const int messageLength = 2;     //message size
char message[messageLength];     //message buffer

char pre_send;
char pre_ir_recv;
char pre_door_recv;

/*definition ble variables */
SoftwareSerial ble(5,4);//RX=5,TX=4
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
  LoRaFi.begin();
  delay(100);

  pre_send = 'F';
  pre_ir_recv = 'F';
  Serial.println("Initiation done");
  
}

void loop() {
  /*LoRa communication*/
  LoRaFi.Send(ble_cmd); 
  LoRaFi.ReceivePackage(message,messageLength);
  
  if(ble_cmd == '1' || ble_cmd == '2' || ble_cmd == '0'){
    Serial.print("LoRa send message: ");
    Serial.println(ble_cmd);
    ble_cmd = ' ';
  }
  
  /*ble transmission*/
  if(pre_ir_recv != message[0] || pre_door_recv != message[1] || message[1] == door_error){
      Serial.print("LORa message received: ");
      Serial.print(message[0]);
      Serial.println(message[1]);
      Serial.print("Bluetooth send message: ");
      ble.print(message[0]);
      Serial.println(message[0]);
      delay(500);
      Serial.print("Bluetooth send message: ");
      ble.print(message[1]);
      Serial.println(message[1]);
      
      pre_ir_recv = message[0];
      pre_door_recv = message[1];
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
