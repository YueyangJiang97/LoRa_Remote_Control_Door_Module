#include <LoRaFi.h>

/*definition command to send*/
#define open_door       '1'
#define close_door      '0'
#define stop_door       '2'
/*infrarouge state definition*/
#define mov_detecte     '3'
#define mov_no_detecte  '4'

/*inrarouge pin definition*/
#define IR_output        2

/*definition motor pins*/
const int motorforward = 4;
const int motorbackward = 5;

/*definition LoRa variables */
LoRaFi LoRaFi;
//volatile int lora_received_flag;
//volatile int lora_send_flag;
const int messageLength = 1;
char message[messageLength];

/*definition variable infrarouge*/
volatile char ir_state;
char ir_pre_state;

void setup() {
  /*Serial port init*/
  Serial.begin(9600);
  delay(100);
  
  /*init motos pins*/
  pinMode(motorforward,OUTPUT);
  pinMode(motorbackward,OUTPUT);
  /*infrarouge init*/
  pinMode(IR_output,INPUT);
  attachInterrupt(digitalPinToInterrupt(IR_output),detection_IR,CHANGE);
  ir_state = mov_no_detecte;
  ir_pre_state = mov_detecte;
  /*LoRa init*/
  LoRaFi.begin();
  delay(100);
 /*message initiation*/
  Serial.println("intiation done");
}

void loop() { 
  /*LoRa communication*/
  LoRaFi.ReceivePackage(message,messageLength);
  LoRaFi.Send(ir_state); 
  if(ir_pre_state != ir_state){
    Serial.print("Send message: ");
    Serial.println(ir_state);
    ir_pre_state = ir_state;
  }
  
  /*command handle*/
  if(message[0] == open_door){
    Serial.println("Message received: open the door");
    digitalWrite(motorforward,LOW);
    digitalWrite(motorbackward,HIGH);
  }
  else if(message[0] == close_door){
    Serial.println("Message received: close the door");
    digitalWrite(motorforward,HIGH);
    digitalWrite(motorbackward,LOW);
  }
  else if(message[0] == stop_door){
    Serial.println("Message received: stop");
    digitalWrite(motorforward,LOW);
    digitalWrite(motorbackward,LOW);
  }

}

void detection_IR(){
  
  if(digitalRead(IR_output)==HIGH){
    Serial.println("mouvement detecte");
    ir_state = mov_detecte;
  }
  
  if(digitalRead(IR_output)==LOW){ 
    Serial.println("pas de mouvement detecte");
    ir_state = mov_no_detecte;
  }
}
