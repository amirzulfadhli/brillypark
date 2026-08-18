#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_Sensor.h>
#include <SPI.h>
#include <Wire.h>
#include <MFRC522.h>
#include <Servo.h>
#include <LiquidCrystal_I2C.h>
#include <Adafruit_NeoPixel.h>

//---------------------------
#define OLED1_ADDR 0x3C // Replace with the I2C address of the first OLED display
#define OLED2_ADDR 0x3C // Replace with the I2C address of the second OLED display
#define OLED3_ADDR 0x3C // Replace with the I2C address of the third OLED display
#define OLED4_ADDR 0x3C // Replace with the I2C address of the fourth OLED display
#define OLED5_ADDR 0x3C // Replace with the I2C address of the fifth OLED display
#define OLED6_ADDR 0x3C // Replace with the I2C address of the sixth OLED display

#define RST_PIN         49          // Configurable, see typical pin layout above
#define SS_PIN          53         // Configurable, see typical pin layout above
MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance
String uid1 = "73 3D B3 0B";
String uid2 = "53 23 1D 13";

Adafruit_SSD1306 display(128, 64, &Wire, 4);
float temp, hum, pressure;

//---------------------------
#define PIN 13
#define NUM_LEDS 30
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, PIN, NEO_GRB + NEO_KHZ800);

//---------------------------
LiquidCrystal_I2C lcd(0x27, 20, 4);  // Initialize LCD with I2C address 0x27
Servo myservo;

// Define pin numbers for IR sensors, enter, and back buttons
#define ir_enter 11
#define ir_back 12
#define ir_car1 5
#define ir_car4 6
#define ir_car2 7
#define ir_car5 8
#define ir_car3 9
#define ir_car6 10

// Array to store sensor status (0 for empty, 1 for filled)
int sensorStatus[] = {0, 0, 0, 0, 0, 0};
int slot = 6; // Total available parking slots
int flag1 = 0, flag2 = 0; // Flags to track button states

//---------------------------
int iterationCount = 1;
int iteration = 0;
int maxIterations = 1; // Number of iterations before starting the effect

//---------------------------
void TCA9548A(uint8_t bus)
{
  Wire.beginTransmission(0x70);
  Wire.write(1 << bus);
  Wire.endTransmission();
}

// Setup function runs once at the beginning
void setup() {
  Serial.begin(9600);

  // Configure IR sensor pins, enter, and back buttons as inputs
  for (int i = 0; i < 6; i++) {
    pinMode(ir_car1 + i, INPUT);
  }
  pinMode(ir_enter, INPUT);
  pinMode(ir_back, INPUT);

  strip.begin();
  strip.show();

  myservo.attach(3); // Attach the servo motor to pin 3
  myservo.write(100); // Set initial position of the servo to 100 degrees

  lcd.init(); // Initialize the LCD
  lcd.backlight(); // Turn on the LCD backlight
  SPI.begin();      // Initiate  SPI bus
  mfrc522.PCD_Init();   // Initiate MFRC522

  TCA9548A(2);
  display.begin(SSD1306_SWITCHCAPVCC, OLED1_ADDR);
  display.clearDisplay();
  display.display();

  TCA9548A(3);
  display.begin(SSD1306_SWITCHCAPVCC, OLED2_ADDR);
  display.clearDisplay();
  display.display();
  
  TCA9548A(4);
  display.begin(SSD1306_SWITCHCAPVCC, OLED3_ADDR);
  display.clearDisplay();
  display.display();

  TCA9548A(5);
  display.begin(SSD1306_SWITCHCAPVCC, OLED4_ADDR);
  display.clearDisplay();
  display.display();

  TCA9548A(6);
  display.begin(SSD1306_SWITCHCAPVCC, OLED5_ADDR);
  display.clearDisplay();
  display.display();
  
  TCA9548A(7);
  display.begin(SSD1306_SWITCHCAPVCC, OLED6_ADDR);
  display.clearDisplay();
  display.display();

  // Display welcome message on the LCD
  showWelcomeMessage();
  delay(3000);
  lcd.clear();

  // Display system message on the LCD
  showSystemMessage();
  delay(3000);
  lcd.clear();

  iterationCount = 0;

  // Read initial sensor states and calculate available parking slots
  Read_Sensor();
  int total = 0;
  for (int i = 0; i < 6; i++) {
    total += sensorStatus[i];
  }
  slot -= total;
}

// Loop function runs repeatedly as long as the Arduino is powered on
void loop() {
  // Read sensor states and update LCD display
  Read_Sensor();
  lcd.setCursor(0, 0);
  lcd.print(" Available Slot: ");
  lcd.print(slot);
  lcd.print("    ");

  // Display sensor status on OLED and handle car entry/exit
  displaySensorStatus();
  sensor();
  handleCarEntry();
  handleCarExit();
  RFID();
  delay(1);
}

// Function to read sensor states and update sensorStatus array
void Read_Sensor() {
  for (int i = 0; i < 6; i++) {
    sensorStatus[i] = digitalRead(ir_car1 + i) == 0 ? 1 : 0;
  }
}

// Function to display sensor statuses on the LCD
void displaySensorStatus() {
  for (int i = 0; i < 6; i++) {
    lcd.setCursor(i % 2 == 0 ? 0 : 11, 1 + i / 2);
    lcd.print("S" + String(i + 1) + ":" + (sensorStatus[i] == 1 ? " Fill " : " Empty "));
  }
}

// Function to handle car entry
void handleCarEntry() {
  if (digitalRead(ir_enter) == 0 && flag1 == 0) {
    if (slot > 0) {
      flag1 = 1;
      if (flag2 == 0) {
        myservo.write(180); // Move the servo to release the parking barrier
        slot--;
        iterationCount--;
        displaylcd();
      }
    } else {
      lcd.setCursor(0, 0);
      lcd.print(" Sorry Parking Full ");
      delay(1500);
    }
  }
}

// Function to handle car exit
void handleCarExit() {
  if (digitalRead(ir_back) == 0 && flag2 == 0) {
    flag2 = 1;
    if (flag1 == 0) {
      myservo.write(180); // Move the servo to release the parking barrier
      if (slot < 6) {
        slot++;
      }
    }
  }
  if (flag1 == 1 && flag2 == 1) {
    delay(1000);
    myservo.write(100); // Reset the servo position to the initial state
    flag1 = 0, flag2 = 0; // Reset the flags
  }
}

// Function to display a welcome message on the LCD
void showWelcomeMessage() {
  lcd.setCursor(0, 1);
  lcd.print(" Hi Welcome To ");
  lcd.setCursor(0, 2);
  lcd.print(" Brilly Park ");
}

// Function to display a system message on the LCD
void showSystemMessage() {
  lcd.setCursor(0, 0);
  lcd.print("     ");
  lcd.setCursor(0, 1);
  lcd.print("    Smart  Parking  ");
  lcd.setCursor(0, 2);
  lcd.print("       System     ");
}

// Function to display a message on LCD 5
void displayOnLCD5(const char* message) {
  TCA9548A(5);  // Assuming LCD 5 is connected to channel 5
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(message);
}

// Function to clear all OLED displays
void clearAllOLED() {
  for (int i = 2; i <= 7; ++i) {
    TCA9548A(i);
    display.clearDisplay();
    display.display();
  }
}

// Function to display the number plate on the OLED display
void displaylcd() {
  // Check each sensor
  for (int i = 0; i < 6; ++i) {
    if (!sensorStatus[i]) {
      // Call the function specific to the inactive sensor
      if (i == 0) {
        Serial.println("Before Neopixel LED function call");
        ledstrip();
        Serial.println("After Neopixel LED function call");
      }

      // Display information for the inactive sensor
      TCA9548A(i + 2);  // Assuming TCA9548A channel starts from 2
      display.setTextColor(WHITE);
      display.clearDisplay();
      display.setTextSize(2);
      display.setCursor(20, 0);
      display.print("No plate");
      display.setTextSize(3);
      display.setCursor(20, 30);

      display.display();
      break;  // Exit the loop after displaying information for the first inactive sensor
    }
  }
  // You can add additional code here to handle other sensors if needed
}

// Function to handle sensor-specific operations
void handleSensor(int sensorIndex) {
  Serial.print("Handling Sensor ");
  Serial.println(sensorIndex + 1); // Adding 1 to make it human-readable (Sensor 1, Sensor 2, etc.)

  TCA9548A(sensorIndex + 2); // Assuming sensorIndex is 0-based
  display.clearDisplay();
  display.display();

  Serial.println("Display cleared for Sensor ");
}

// Function to check sensors and perform specific actions
void sensor() {
  for (int i = 0; i < 6; ++i) {
    if (sensorStatus[i] == 1) {
      handleSensor(i);
    }
  }
}

// Function to perform a color wipe effect on the NeoPixel LED strip
void colorWipe(uint32_t color, int wait) {
  for(int i=strip.numPixels()-1; i>=0; i--) {
    strip.setPixelColor(i, color);
    strip.show();
    delay(wait);
  }

  delay(1000);
}

// Function to turn on the LED strip with a color wipe effect
void ledstrip() {
  while (iterationCount < maxIterations) {
    if (iterationCount < maxIterations) {
      // If it has, start the effect
      colorWipe(strip.Color(10, 0, 0), 50); // Red
      colorWipe(strip.Color(0, 0, 0), 50); // Red
      iterationCount++;
      delay(100);
    }
  }
  return;
}


void RFID() {
  // Look for new RFID cards
  if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
    // Display the UID on the OLED
      TCA9548A(2);
      display.begin(SSD1306_SWITCHCAPVCC, OLED1_ADDR);
      display.clearDisplay();
      display.display();

      TCA9548A(3);
      display.begin(SSD1306_SWITCHCAPVCC, OLED2_ADDR);
      display.clearDisplay();
      display.display();
      
      TCA9548A(4);
      display.begin(SSD1306_SWITCHCAPVCC, OLED3_ADDR);
      display.clearDisplay();
      display.display();

      TCA9548A(5);
      display.begin(SSD1306_SWITCHCAPVCC, OLED4_ADDR);
      display.clearDisplay();
      display.display();

      TCA9548A(6);  // Assuming TCA9548A channel starts from 2
      display.setTextColor(WHITE);
      display.clearDisplay();
      display.setTextSize(2);
      display.setCursor(20, 0);
      display.print("No plate");
      display.setTextSize(3);
      display.setCursor(20, 30);
    
    display.display();
    
    delay(2000); // Delay for visibility
    display.clearDisplay();
    mfrc522.PICC_HaltA();
    delay(1000);
  }
}
