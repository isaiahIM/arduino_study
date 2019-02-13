int g_CDS_pin=A6;
int g_CDS_val;

void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);
}

void loop() {
  g_CDS_val=analogRead(g_CDS_pin);
Serial.println(g_CDS_val);
  delay(1000);
}
  // put your main code here, to run repeatedly:

