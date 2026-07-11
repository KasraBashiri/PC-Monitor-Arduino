#include <LiquidCrystal_I2C.h>
#include <Wire.h>

LiquidCrystal_I2C lcd(0x27, 20, 4);

int currentPage = 1;
unsigned long lastChange = 0;
unsigned long target = 4500;
String com;

String cpu;
String ram;
String disk;
String now;

int Percent;
int filled;
int war;

byte fullBlock[8] = {
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111
};

byte emptyBlock[8] = {
  B11111,
  B10001,
  B10001,
  B10001,
  B10001,
  B10001,
  B10001,
  B11111
};

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  lcd.init();
  lcd.backlight();
  lcd.createChar(0, fullBlock);
  lcd.createChar(1, emptyBlock);
  lcd.setCursor(0, 0);
  lcd.print("HELLO SIR!");
}

void checkWarning(String warning, String keyWar) {
  war = warning.toInt();

  if (keyWar == "CPU" && war >= 90) {
    lcd.setCursor(0, 2);
    lcd.print(" !! WARNING !! ");
  }
  else if (keyWar == "RAM" && war >= 85) {
    lcd.setCursor(0, 2);
    lcd.print(" !! WARNING !! ");
  }
  else if (keyWar == "DISK" && war >= 90) {
    lcd.setCursor(0, 2);
    lcd.print(" !! WARNING !! ");
  }
}

void percent(String prc) {
  Percent = prc.toInt();
  filled = Percent / 5;

  lcd.setCursor(0, 1);

  for (int i = 0; i < filled; i++) {
    lcd.write(byte(0));
  }

  for (int i = 0; i < 20 - filled; i++) {
    lcd.write(byte(1));
  }
}

void parseData() {
  while (com.length() > 0) {
    String key;
    String value;

    int pos = com.indexOf("|");

    if (pos != -1) {
      String part = com.substring(0, pos);
      int equal = part.indexOf('=');

      key = part.substring(0, equal);
      value = part.substring(equal + 1);
      com = com.substring(pos + 1);
    }

    else {
      int equal = com.indexOf('=');

      key = com.substring(0, equal);
      value = com.substring(equal + 1);
      com = "";
    }

    if (key == "CPU") {
      cpu = value;
    }

    else if (key == "RAM") {
      ram = value;
    }

    else if (key == "DISK") {
      disk = value;
    }

    else if (key == "NOW") {
      now = value;
    }
  }
}

void showData() {
  Serial.println(currentPage);

  if (millis() - lastChange >= target) {
    lastChange = millis();
    currentPage++;

    if (currentPage > 4) {
      currentPage = 1;
    }
  }

  switch (currentPage) {

    case 1:
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("CPU USAGE : ");
      lcd.print(cpu + " %");
      percent(cpu);
      checkWarning(cpu,"CPU");
      break;

    case 2:
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("RAM USAGE : ");
      lcd.print(ram + " %");
      percent(ram);
      checkWarning(ram,"RAM");
      break;

    case 3:
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("DISK USAGE : ");
      lcd.print(disk + " %");
      percent(disk);
      checkWarning(disk,"DISK");
      break;

    case 4:
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Time : ");
      lcd.print(now);
      break;
  }
}

void loop() {
  if (Serial.available()) {
    com = Serial.readStringUntil('\n');

    parseData();
    showData();
  }
}
