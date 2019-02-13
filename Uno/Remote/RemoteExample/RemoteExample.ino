#include <IRremote.h> 

/* Command from IR */
int RECV_PIN = 11;        // IRED 연결 핀
IRrecv irrecv(RECV_PIN); 
decode_results results;
int irCommand;
void setup() {
   irrecv.enableIRIn(); // Start the IR receiver
   Serial.begin(9600);
 } 

void loop() {
   /////////////////////////////////////////////////////
   // Read data from IR receiver
   /////////////////////////////////////////////////////
   
   if (irrecv.decode(&results)) {    // 수신 신호를 디코딩
    irCommand = results.value;    // 커맨드 추출
    
     Serial.print(F("IR Command="));    // 시리얼 출력
    Serial.print(irCommand);
     Serial.print(F(", "));
     
     irrecv.resume();  // Receive the next value
   }
   
 } 

