static uint8_t g_joystick_pin_X=A8, g_joystick_pin_Y=A9, g_joystick_Pin_Btn=A10;
static uint16_t g_joystick_val_X, g_joystick_val_Y;
void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
g_joystick_val_X=analogRead(g_joystick_pin_X);
g_joystick_val_Y=analogRead(g_joystick_pin_Y);

Serial.println(g_joystick_val_X);
Serial.println(g_joystick_val_Y);
Serial.println();

delay(1000);
}
