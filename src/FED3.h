#define VER "1.0"

#ifndef FED3_H
#define FED3_H

//include these libraries
#include <Arduino.h>
#include <Adafruit_BusIO_Register.h>
#include <Adafruit_I2CDevice.h>
#include <Adafruit_I2CRegister.h>
#include <Adafruit_SPIDevice.h>
#include <Wire.h>
#include <SPI.h>
#include <Stepper.h>
#include "RTClib.h"
#include <SdFat.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SharpMem.h>
#include <Fonts/FreeSans9pt7b.h>
#include <Fonts/Org_01.h>
#include <Adafruit_NeoPixel.h>
#include <Adafruit_AHTX0.h>
#include "Adafruit_MPR121.h"

// Pin definitions
#define NEOPIXEL                A1
#define MOTOR_ENABLE            13
#define GREEN_LED               88
#define PELLET_WELL             1
#define PELLET_WELL_OUTPUT      52
#define LEFT_POKE               6
#define LEFT_POKE_OUTPUT        50
#define RIGHT_POKE              5
#define RIGHT_POKE_OUTPUT       48
#define BUZZER                  0
#define BNC_OUT                 A0
#define SHARP_SCK               12
#define SHARP_MOSI              11
#define SHARP_SS                10
#define LEFT_DRINK              14
#define LEFT_DRINK_OUTPUT       46
#define RIGHT_DRINK             15
#define RIGHT_DRINK_OUTPUT      44
#define touchSensorinterrupt    16
#define houselight_1            22
#define houselight_2            24

//SD card inits
#define SD_FAT_TYPE 3
#define SD_CS_PIN SDCARD_SS_PIN

//Motor Driver inits
#define BLACK 0
#define WHITE 1
#define STEPS 2038

//Touch Sensitive Lickometer inits
#ifndef _BV
#define _BV(bit) (1 << (bit))
#endif

extern bool Left;

class FED3 {
    // Members
public:
    FED3(String sketch);
    String sketch = "undef";
    String sessiontype = "undef";
    
    void classInterruptHandler(void);
    void begin();
    void run();
    
    // SD logging
    SdFat SD;
    File logfile;       // Create file object
    File ratiofile;     // Create another file object
    File configfile;    // Create another file object
    File startfile;     // Create another file object
    File stopfile;      // Create another file object
    char filename[21];  // Array for file name data logged to named in setup
    void logdata();
    void CreateFile();
    void CreateDataFile ();
    void writeHeader();
    void writeConfigFile();
    void writeFEDmode();
    void error(uint8_t errno);
    void getFilename(char *filename);
    bool suppressSDerrors = false;  //set to true to suppress SD card errors at startup
    
    // Neopixel
    void pixelsOn(int R, int G, int B, int W);
    void pixelsOff();
    void Blink(byte PIN, byte DELAY_MS, byte loops);
    void colorWipe(uint32_t c, uint8_t wait);
    void leftPixel(int R, int G, int B, int W);
    void rightPixel(int R, int G, int B, int W);
    void leftPokePixel(int R, int G, int B, int W);
    void rightPokePixel(int R, int G, int B, int W);
    
    // Display functions
    void UpdateDisplay();
    void DisplaySDError();
    void DisplayJamClear();
    void DisplayRetrievalInt();
    void DisplayLeftInt();
    void DisplayRightInt();
    void DisplayDateTime();
    void DisplayIndicators();
    void DisplayTimedFeeding();
    void DisplayNoProgram();
    void DisplayMinPoke();
    void DisplayMouse();
    void DisplayLeftDrinkInt();
    void DisplayRightDrinkInt();
    
    // Startup menu function
    void ClassicMenu();
    void StartScreen();
    void FED3MenuScreen();
    void SetClock();
    
    //BNC input/output
    void ReadBNC(bool blinkGreen);
    bool BNCinput = false;
    
    // Motor
    void ReleaseMotor();
    int numMotorTurns = 0;
    
    // Set FED
    void SelectMode();
    void SetDeviceNumber();
    
    // Stimuli
    void ConditionedStimulus(int duration = 200);
    void Click();
    void Noise(int duration = 200);
    void BNC(int DELAY_MS, int loops);
    void pulseGenerator(int pulse_width, int frequency, int repetitions);
    
    void Tone(int freq, int duration);
    void stopTone();
    
    // Pelet and poke functions
    void CheckRatio();
    void logLeftPoke();
    void logRightPoke();
    void Feed(int pulse = 0, bool pixelsoff = true);
    bool dispenseTimer_ms(int ms);
    void pelletTrigger();
    void leftTrigger();
    void rightTrigger();
    void leftdrinkTrigger();
    void rightdrinkTrigger();
    void goToSleep();
    void Timeout(int timeout);
    int minPokeTime = 0;
    void randomizeActivePoke(int max);
    int consecutive = 0;
    void touchsensitive ();
    void houselights(bool houselight_state = false);
    
    // Lickometer functions
    void logLeftDrink();
    void logRightDrink();
    
    //jam movements
    bool RotateDisk(int steps);
    bool ClearJam();
    bool VibrateJam();
    bool MinorJam();
    
    //timed feeding variables
    int timedStart; //hour to start the timed Feeding session, out of 24 hour clock
    int timedEnd; //hour to start the timed Feeding session, out of 24 hour clock
    
    // mode variables
    int FED;
    int FR = 1;
    bool DisplayPokes = true;
    bool DisplayTimed = false;
    byte FEDmode = 1;
    byte previousFEDmode = FEDmode;
    
    // event counters
    int LeftCount = 0;
    int RightCount = 0;
    int PelletCount = 0;
    int BlockPelletCount = 0;
    int timeout = 0;
    int LeftDrinkCount = 0;
    int LeftDrinkDuration = 0;
    int RightDrinkCount = 0;
    int RightDrinkDuration = 0;
    
    // state variables
    bool activePoke = 1;  // 0 for right, 1 for left, defaults to left poke active
    bool Left = false;
    bool Right = false;
    bool LeftDrink = false;
    bool RightDrink = false;
    bool PelletAvailable = false;
    unsigned long currentHour;
    unsigned long currentMinute;
    unsigned long currentSecond;
    unsigned long displayupdate;
    String Event = "None";   //What kind of event just happened?
    
    // timing variables
    int retInterval = 0;
    int leftInterval = 0;
    int rightInterval = 0;
    int leftPokeTime = 0.0;
    int rightPokeTime = 0.0;
    unsigned long pelletTime = 0;
    unsigned long lastPellet = 0;
    unsigned long unixtime = 0;
    int interPelletInterval = 0;
    int leftDrinkTime = 0;
    int rightDrinkTime = 0;
    int leftDrinkInterval = 0;
    int rightDrinkInterval = 0;
    
    // flags
    bool Ratio_Met = false;
    bool EnableSleep = true;
    void disableSleep();
    void enableSleep();
    bool ClassicFED3 = false;
    bool FED3Menu = false;
    bool tempSensor = false;
    bool touchSensor = false;
    
    int EndTime = 0;
    int ratio = 1;
    int previousFR = FR;
    int previousFED = FED;
    
    bool SetFED = false;
    bool setTimed = false;
    
    //touch sensors
    unsigned long touchStartTime3 = 0;
    unsigned long touchEndTime3 = 0;
    unsigned long touchStartTime11 = 0;
    unsigned long touchEndTime11 = 0;
    bool ispin3CurrentlyTouched = false;
    bool ispin11CurrentlyTouched = false;
    int touch_threshold = 2;    //default = 12
    int release_threshold = 0;  //default = 6
    int debounce = 0;           //range 0-7
    int current = 32;           //Range of 1-63uA, Board default = 16
    void SensorSettings();
    void AutoCalibrate_TouchSensors();
    
    // Neopixel strip
    Adafruit_NeoPixel strip = Adafruit_NeoPixel(10, NEOPIXEL, NEO_GRBW + NEO_KHZ800);
    // Display
    Adafruit_SharpMem display = Adafruit_SharpMem(SHARP_SCK, SHARP_MOSI, SHARP_SS, 144, 168);
    // Stepper
    Stepper stepper = Stepper(STEPS, A2, A3, A4, A5);
    // Temp/Humidity Sensor
    Adafruit_AHTX0 aht;
    // Touch Sensitive I2C Board
    Adafruit_MPR121 cap;
    
private:
    static FED3* staticFED;
    static void updatePelletTriggerISR();
    static void updateLeftTriggerISR();
    static void updateRightTriggerISR();
    static void updateLeftdrinkTriggerISR();
    static void updateRightdrinkTriggerISR();
    
};

#endif
