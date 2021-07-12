#include <LoRaFi.h>

/*definition command to send*/
#define close_door      '0'
#define open_door       '1'
#define stop_door       '2'
/*door state definition*/
#define door_closed     '0'
#define door_opened     '1'
#define door_error      '2'
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
const int sendLength = 2;
char message[messageLength];
char send_msg[sendLength];
char pre_send_msg[sendLength];

/*definition variable infrarouge*/
volatile char ir_state;

/*definition door state pins*/
const int end_open_pin = 18;
const int end_close_pin = 19;
volatile char door_state;


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

  /*LoRa init*/
  LoRaFi.begin(53,9,8,7,3);//CS=53;RST=9;RX=8;TX=7;DIO0=3;
  delay(100);  
  
  /*door state init*/
  pinMode(end_open_pin,INPUT);
  pinMode(end_close_pin,INPUT);
  attachInterrupt(digitalPinToInterrupt(end_open_pin),end_open,RISING);
  attachInterrupt(digitalPinToInterrupt(end_close_pin),end_close,RISING);
  
  /*states initiation*/
  ir_state = mov_no_detecte;
  door_state = door_closed;
  
  Serial.println("intiation done");
}

void loop() { 
  
  send_msg[0] = ir_state;
  send_msg[1] = door_state;
  
  /*LoRa communication*/
  LoRaFi.ReceivePackage(message,messageLength);
  LoRaFi.Send(send_msg);
  
  if(send_msg[1] != pre_send_msg[1]){
    if(send_msg[1] == door_closed) {
      digitalWrite(motorforward,LOW);
      digitalWrite(motorbackward,LOW);
      Serial.println("Porte est fermée");
    }
    else if(send_msg[1] == door_opened) {
      digitalWrite(motorforward,LOW);
      digitalWrite(motorbackward,LOW);
      Serial.println("Porte est ouverte");
    }
    else if(send_msg[1] == door_error) Serial.println("En cours"); 
    pre_send_msg[1] = send_msg[1];     
  }
    
  /*send message handle*/
  if(send_msg[0] != pre_send_msg[0]){
    Serial.print("LoRa send message: ");
    Serial.print(send_msg[0]);
    Serial.println(send_msg[1]);    
    pre_send_msg[0] = send_msg[0];
  }

  /*received command handle*/
  if(message[0] == open_door && door_state != open_door){
    Serial.println("Message received: open the door");
    digitalWrite(motorforward,LOW);
    digitalWrite(motorbackward,HIGH);
    door_state = door_error;
  }
  else if(message[0] == close_door && door_state != door_closed){
    Serial.println("Message received: close the door");
    digitalWrite(motorforward,HIGH);
    digitalWrite(motorbackward,LOW);
    door_state = door_error;
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

void end_open(){
  door_state = door_opened;
}
void end_close(){
  door_state = door_closed;
}
