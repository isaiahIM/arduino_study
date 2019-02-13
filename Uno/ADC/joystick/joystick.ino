int g_joyStick_X=A4, g_joyStick_Y=A5;
int g_Read_x, g_Read_y;
void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
g_Read_x=analogRead(g_joyStick_X);
g_Read_y=analogRead(g_joyStick_Y);

Serial.println(g_Read_x);
Serial.println(g_Read_y);
Serial.println();
delay(1000);
}
