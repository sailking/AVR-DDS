#define msUp				1
#define msDwn				4      
#define YposUp				0
#define YposDwn				3
#define freeze				2

#define AC 					0
#define DC 					1
#define SQUARE				2
#define TRUE				0
#define FALSE				1

unsigned int  dataCounter = 0;
unsigned int  timeDiv = 0;  
unsigned char trigger = 0;
unsigned char findZero = 0;
unsigned char upLimit = 0;
unsigned char lowLimit = 255;
unsigned char prevADCvalue = 0;
unsigned char voltageType = AC;
unsigned char complete = TRUE;
unsigned int  voltage;
unsigned char frequency;
unsigned int  ADCvalue;
unsigned char hex2asciiBuffer[4];

signed char Ypos   = 0; 
signed char Ypos2   = 0;
signed char position  = 0;

static unsigned char dataLcdBuffer[100];
static unsigned char dataLcdBackupBuffer[200];
unsigned int  backupLcdAddress = 0;

void fillDataLcdBuffer (unsigned char address, unsigned char data);