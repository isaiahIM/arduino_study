int g_PotenPin=A5;
int g_val;

void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);

}

void loop() {
  // put your main code here, to run repeatedly:
g_val=analogRead(g_PotenPin);
Serial.println(g_val);
}
