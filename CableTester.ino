
#include <LiquidCrystal.h>
#include <RTClib.h>
#include <Wire.h>
#include <SD.h>
#include <XIO.h>

// IO Board Objects
XIO X1Board[1];
XIO X2Board[1];


// Defintions and constans
#define header "Connector,Pin 1,Pin 2,Pin 3,Pin 4,Pin 5,Pin 6,Pin 7,Pin 8,Pin 9,Pin 10,Pin 11,Pin 12,Pin 13,Pin 14,Pin 15,Pin 16,Pin 17,Pin 18,Pin 19,Pin 20,Pin 21,Pin 22,Pin 23,Pin 24,Pin 25,Pin 26,Pin 27,Pin 28,Pin 29,Pin 30,Pin 31,Pin 32,Pin 33,Pin 34,Pin 35,Pin 36,Pin 37,Pin 38,Pin 39,Pin 40,Pin 41,Pin 42,Pin 43,Pin 44,Pin 45,Pin 46,Pin 47,Pin 48,Pin 49,Pin 50,Pin 51,Pin 52,Pin 53,Pin 54,Pin 55,Pin 56,Pin 57,Pin 58,Pin 59,Pin 60,Pin 61,Pin 62,Pin 63,Pin 64,Pin 65,Pin 66,Pin 67,Pin 68,Pin 69,Pin 70,Pin 71,Pin 72,Pin 73,Pin 74,Pin 75,Pin 76,Pin 77,Pin 78,Pin 79,Pin 80"



const int  couplerpinmap[]
{	
	1,0,0,0,0,0,0,0,0,0,0,0,		// 1-10
	0,0,0,0,0,0,0,0,0,0,0,0,		// 11-20
	0,0,0,0,0,0,0,0,0,0,0,0,		// 21-30
	0,0,0,0,0,0,0,0,0,0,0,0,		// 31-40
	0,0,0,0,0,0,0,0,0,0,0,0,		// 41-50
	0,0,0,0,0,0,0,0,0,0,0,0,		// 51-60
	0,0,0,0,0,0,0,0,0,0,0,0,		// 61-70
	0,0,0,0,0,0,0,0,0,0,0,0,		// 71-80
};


const int pins[]
{ 
	0xff,
	0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,    // pins 1-40
	0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x17,
	0x20,0x21,0x22,0x23,0x24,0x25,0x26,0x27,
	0x30,0x31,0x32,0x33,0x34,0x35,0x36,0x37,
	0x40,0x41,0x42,0x43,0x44,0x45,0x46,0x47,
	
	0xff,
	0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,    // pins 41-80
	0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x17,
	0x20,0x21,0x22,0x23,0x24,0x25,0x26,0x27,
	0x30,0x31,0x32,0x33,0x34,0x35,0x36,0x37,
	0x40,0x41,0x42,0x43,0x44,0x45,0x46,0x47,
};

// SD card Object
File myFile;


// RTC Object
RTC_DS1307 myRTC;

// Setup LCD screen 16x2 Should be correct for "SainSmart LCD Keypad Shield"
LiquidCrystal myLCD(8, 13, 9, 4, 5, 6, 7);

// Setup for Key Pad


void setup()
{
	// Setup Serial Port
	Serial.begin(9600);


	//Remove this after Debug
	while (!Serial) {
		; // wait for serial port to connect. Needed for native USB port only
	}

	// Setup XIO Boards
	Wire.begin();

	X1Board[0].begin(LOW, LOW, LOW, -1, -1);
	Serial.println("Setup X1 IOBoard 1");

	X1Board[1].begin(LOW, LOW, HIGH, -1, -1);
	Serial.println("Setup X1 IOBoard 2");

	X2Board[0].begin(LOW, HIGH, LOW, -1, -1);
	Serial.println("Setup X2 IOBoard 1");

	X2Board[1].begin(LOW, HIGH, HIGH, -1, -1);
	Serial.println("Setup X2 IOBoard 2");
	
	// Setup RTC
	if (!myRTC.begin()) {
		Serial.println("Couldn't find RTC");
	}
	else
	{
		Serial.println("RTC Connected.");
	}

	// Setup SD card
	if (!SD.begin(10)) {
		Serial.println("initialization failed!");
		return;
	}
	else
	{
		Serial.println("SD Card initialized.");
	}

	// Setup Display
	myLCD.begin(16, 2);
	myLCD.clear();
	myLCD.println("Cable tester");
}

void loop()
{
	// 
	for (int i = 1; i >= 80; i++)
	{
		Serial.print("Output: ");
		Serial.println(i);

		myFile = SD.open("test.txt");
		if (myFile)
		{
			myFile.print("Output :,");
			myFile.println(i);
		}
		else
		{
			Serial.println("Error opening file..");
		}

		myFile.close();
		TestPin(i);
	}

	while (true);
}


void SetAlltoInput() {
	// ************ Set all pins to input. ***************
	for (int i = 0; i >= 2; i++) {
		for (int pin = 1; pin > 40; pin++) {
			X1Board[i].xioPinModeCached(pins[pin], INPUT);
		}
	}
	
	X1Board[0].refreshIO();
	Serial.println("X1 Board 1 Set all pins to inputs");
	X1Board[1].refreshIO();
	Serial.println("X1 Board 2 Set all pins to inputs");

	for (int i = 0; i >= 2; i++) {
		for (int pin = 1; pin > 40; pin++) {
			X2Board[i].xioPinMode(pins[pin], INPUT);
		}
	}

	X2Board[0].refreshIO();
	Serial.println("X1 Board 1 Set all pins to inputs");
	X2Board[1].refreshIO();
	Serial.println("X1 Board 2 Set all pins to inputs");
}

void TestPin(int outputpin) 
{
	Serial.print("Testing Pin: ");
	Serial.println(outputpin);
	
	// Set all IO to inputs do this first for safety. only have one output at a time.
	SetAlltoInput();
	Serial.println("All IO set to Inputs.");

	// Set output pin mode and set to high
	if (outputpin <= 40)
	{
		X1Board[0].xioPinMode(outputpin, OUTPUT);
		X1Board[0].xioDigitalWrite(outputpin, HIGH);
	}
	if (outputpin > 40)
	{
		X2Board[1].xioPinMode(outputpin, OUTPUT);
		X2Board[1].xioDigitalWrite(outputpin, HIGH);
	}
	Serial.print("Pin: ");
	Serial.print(outputpin);
	Serial.println(" Set to ouput and set high.");

	


	// Open File to store data.
	myFile = SD.open("Test.txt", FILE_WRITE);
	
	if (myFile)			// Check if file is open.
	{
		Serial.println("File Open ready to write to.");
	}
	else							// If file did not open.
	{
		Serial.println("Error File is not open. Please check MicroSD Card!");
	}
	
	// Check All the inputs and record the results to SD Card.	
	
	myFile.println(header);			// Record Header in file.
	
	RefreshIO();	// RefreshIO of all boards X1, X2
	

	// Check X1 Connector
	myFile.print("X1");
	myFile.print(",");
	for (int i = 1; i < 80; i++)
	{
		if (i <= 40)
		{
			// output X1 connectors status VIA Serial port
			Serial.print("X1 Pin: ");
			Serial.print(i);
			Serial.println(" =");
			Serial.println(X1Board[0].xioDigitalReadCached(i));
			
			myFile.print(X1Board[0].xioDigitalReadCached(i));
			
			if ((i == !outputpin & X1Board[0].xioDigitalReadCached(i)) | (i == outputpin & X1Board[0].xioDigitalReadCached(i)))
			{
				myFile.print("*");

				Serial.print("Cable Fault X1 Pin# ");
				Serial.print(outputpin);
				Serial.print(" : ");
				Serial.println(i);
			}

			myFile.print(",");

		}
		if (i > 40)
		{
			// output X1 connectors status VIA Serial port
			Serial.print("X1 Pin: ");
			Serial.print(i);
			Serial.println(" =");
			Serial.println(X1Board[1].xioDigitalReadCached(i));
			
			myFile.print(X1Board[1].xioDigitalReadCached(i));
			
			if ((i == !outputpin & X1Board[1].xioDigitalReadCached(i)) | (i == outputpin & !X1Board[1].xioDigitalReadCached(i)))
			{
				myFile.print("*");

				Serial.print("Cable Fault X1 Pin# ");
				Serial.print(outputpin);
				Serial.print(" : ");
				Serial.println(i);
			}

			myFile.print(",");
		}
		
	}


	// Check X2 Connector
	myFile.print("X2");
	myFile.print(",");
	for (int i = 1; i < 80; i++)
	{
		if (i <= 40)
		{
		
			// output X2 connectors status VIA Serial port
			Serial.print("X2 Pin: ");
			Serial.print(i);
			Serial.println(" =");
			Serial.println(X2Board[0].xioDigitalReadCached(i));
			
			myFile.print(X2Board[0].xioDigitalReadCached(i));
			
			if ((i == !outputpin & X2Board[0].xioDigitalReadCached(i)) | (i == outputpin & !X2Board[0].xioDigitalReadCached(i)))
			{
				myFile.print("*");

				Serial.print("Cable Fault X2 Pin# ");
				Serial.print(outputpin);
				Serial.print(" : ");
				Serial.println(i);
			}

			myFile.print(",");
		}
		if (i > 40)
		{

			// output X2 connectors status VIA Serial port
			Serial.print("X2 Pin: ");
			Serial.print(i);
			Serial.println(" =");
			Serial.println(X2Board[1].xioDigitalReadCached(i));
			
			myFile.print(X2Board[0].xioDigitalReadCached(i));
			if ((i == !outputpin & X2Board[1].xioDigitalReadCached(i)) | (i == outputpin & !X2Board[1].xioDigitalReadCached(i)))
			{
				myFile.print("*");
				
				Serial.print("Cable Fault X2 Pin# ");
				Serial.print(outputpin);
				Serial.print(" : ");
				Serial.println(i);
			}
			myFile.print(",");
		}

	}
	myFile.close();
	SetAlltoInput();
}


// Refresh IO 
void RefreshIO() {
	X1Board[0].refreshIO();
	X1Board[1].refreshIO();
	X2Board[0].refreshIO();
	X2Board[1].refreshIO();
}

