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

uint16_t dataCounter = 0;
uint16_t timeDiv = 0;  
uint8_t trigger = 0;
uint8_t findZero = 0;
uint8_t upLimit = 0;
uint8_t lowLimit = 255;
uint8_t prevADCvalue = 0;
uint8_t voltageType = AC;
uint8_t complete = TRUE;
uint16_t voltage;
uint8_t frequency;
uint16_t ADCvalue;
uint8_t hex2asciiBuffer[4];

int8_t Ypos   = 0; 
int8_t Ypos2   = 20;
int8_t position  = 0;

static uint8_t dataLcdBuffer[100];
//static uint8_t dataLcdBackupBuffer[200];
//uint16_t  backupLcdAddress = 0;

void fillDataLcdBuffer (uint8_t address, uint8_t data);
void create_raster();
void frequence_display();
void signal_display();
void create_wave();
void adc_init ();




