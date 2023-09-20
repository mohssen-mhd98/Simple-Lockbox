#include <Keypad.h>
# include <LiquidCrystal.h>;
#include <Servo.h>
#define buzz 23
int servoPin = 2;
Servo servo;
const int rs = 13, en = 12, d4 = 11, d5 = 10, d6 = 9, d7 = 8;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

const int ROW_NUM = 4; //four rows
const int COLUMN_NUM = 3; //three columns
const int red_led = 3;
const int green_led = 4;
const int lock = 5;

char keys[ROW_NUM][COLUMN_NUM] = {
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'},
  {'*', '0', '#'}
};

byte pin_rows[ROW_NUM] = {22, 24, 26, 28}; //connect to the row pinouts of the keypad
byte pin_column[COLUMN_NUM] = {36, 37, 38}; //connect to the column pinouts of the keypad

Keypad keypad = Keypad( makeKeymap(keys), pin_rows, pin_column, ROW_NUM, COLUMN_NUM );

String password = "1234"; // default password
String input_password;
int countDown = 10;
void setup() {
  servo.attach(servoPin);
  servo.write(0);
  pinMode(buzz, OUTPUT);
  pinMode(green_led, OUTPUT);
  pinMode(red_led, OUTPUT);
  lcd.begin(16, 2);
  input_password.reserve(32);
  digitalWrite(red_led, HIGH);

}
int btn = 1;
int len;
void loop() {
  char key = keypad.getKey();
  //btn = digitalRead(lock);

  if (key) {
    input_password += key;
    len = input_password.length();
    lcd.print(key);
    /*lcd.setCursor(0, 1);
      lcd.print(input_password.substring(len-1));
      lcd.setCursor(len, 0);*/
    //input_password.endsWith("##")
    if (input_password.substring(len - 2, len) == "#*") {
      //key='\0';
      countDown = input_password.substring(0, len - 2).toInt();
      lcd.setCursor(0, 1);
      lcd.println("New timer set");
      lcd.setCursor(0, 0);
      lcd.println(countDown);
      delay(1000);
      lcd.clear();
      input_password = "";
    }
    else if (input_password.substring(len - 2, len) == "*#") {
      password = input_password.substring(0, len - 2);
      lcd.setCursor(0, 1);
      lcd.println("New password set");
      lcd.setCursor(0, 0);
      delay(1000);
      lcd.clear();
      input_password = "";

    } else if (input_password.substring(len - 2, len) == "**") {
      if (password == input_password.substring(0, len - 2)) {
        lcd.setCursor(0, 1);
        lcd.print("Correct pass");
        lcd.setCursor(0, 0);
        tone(buzz, 1000);
        servo.write(180);
        digitalWrite(red_led, LOW);
        digitalWrite(green_led, HIGH);
        delay(1000);
        noTone(buzz);
        lcd.clear();
        //countdown for lock
        lcd.print("Countdown begins:");
        delay(1000);
        lcd.clear();

        // this for statement used to decrease the total time in seconds
        for (long int j = countDown; j >= 0; j--)
        {
          ///////button
          btn = digitalRead(lock);
          if (btn == LOW) {

            lcd.clear();
            lcd.setCursor(0, 1);
            lcd.print("Gage LOCKED");
            lcd.setCursor(0, 0);
            servo.write(0);
            digitalWrite(green_led, LOW);
            digitalWrite(red_led, HIGH);
            delay(2000);
            lcd.clear();
            break;

          }

          lcd.println(j);
          lcd.setCursor(0, 0);
          delay(1000);
          if (j == 0)
          {
            lcd.setCursor(0, 1);
            lcd.print("Time is expired");
            delay(300);
            lcd.setCursor(0, 0);
            servo.write(0);
            digitalWrite(green_led, LOW);
            digitalWrite(red_led, HIGH);
            delay(300);
            lcd.clear();
          }
        }

      } else {
        lcd.setCursor(0, 1);
        lcd.print("Incorrect pass");
        lcd.setCursor(0, 0);
        tone(buzz, 1000);
        servo.write(0);
        digitalWrite(green_led, LOW);
        digitalWrite(red_led, HIGH);
        delay(1000);
        noTone(buzz);
        lcd.clear();
      }

      // clear input password
      input_password = "";
    } else if (input_password.substring(len - 2) == "##" /*&& input_password.substring(len - 3, len - 2) != "*" && input_password.substring(len - 3, len - 2) != "#"*/ ) {
      lcd.clear();
      input_password = "";
    }
  }

}
