#ifndef ESP_CBOX_CONF
#define ESP_CBOX_CONF

// Default IDs that device will respond to
#define ID_SINGLE_DEF 3   //"001"
#define ID_GROUP_1_DEF  100 //"100"
#define ID_GROUP_2_DEF 101
#define ID_ALL    0   //"000"

unsigned short ID_SINGLE = ID_SINGLE_DEF;

#define ID_LENGTH 3

// const char* APPLICABLE_IDS[] = {
//     ID_SINGLE, ID_GROUP, ID_ALL
// };


// IDs that device will respond to
unsigned short APPLICABLE_IDS[] = {
    ID_SINGLE_DEF, ID_ALL, ID_GROUP_1_DEF, ID_GROUP_2_DEF
};

#define CONF_BUTTON_MIN_TIME_MS 100

// Calibration range of analogRead voltage dividers
#define CONF_M5V_MAX_DEF 6652
#define CONF_M12V_MAX_DEF 24500

unsigned int CONF_M5V_MAX = CONF_M5V_MAX_DEF;
unsigned int CONF_M12V_MAX = CONF_M12V_MAX_DEF;

// Time between analogRead measurements
#define CONF_MEASURE_DELAY 1000

#define CONF_BAUD_RATE 38400

// MAX serial command length
#define CONF_BUFFER_SIZE 255

// Special symbols - cannot be used in messages
#define CONF_CHAR_START '!'
#define CONF_CHAR_END '#'

// Amount of stored 3byte keyframes in memory
#define CONF_FRAME_SIZE 32

// COMMAND MODIFIERS
#define CONF_CMD_DIRECT '1' // Set PWM directly
#define CONF_CMD_FRAME_HW '2' // Set PWM to selected frame
#define CONF_CMD_FRAME_L '3' // Set selected frame to a value
#define CONF_CMD_FRAME_HWL '4' // Set selected frame and PWM to a value

#define CONF_CMD_RAPORT '8' // Return id, version and voltages

#define CONF_CMD_CALIB_5 'C' // Calibrate analogRead5 with provided values
#define CONF_CMD_CALIB_12 'D' // Calibrate analogRead12 with provided values
#define CONF_CMD_ID 'E' // Set device ID

/// Address of eeprom config structure
#define CONF_EEPROM_ADDRESS 0

/// PINS
#define PIN_LED1 2 // Onboard LED pin
#define PIN_LED2 3 // Onboard LED pin
#define PIN_LED3 13 // Onboard LED pin

#define PIN_BUTTON 4 // Button pin

#define PIN_M5V A7 // 5V line voltage divider
#define PIN_M12V A6 // 12V line voltage divider

#define PIN_O1 9 // T1A
#define PIN_O2 10 // T1B
#define PIN_O3 11 // T2A

#define PIN_RS_ENABLE 8 // RS485 module enable pins (linked)

#ifdef USE_EEPROM
// EEPROM config structure
struct EE_CONF
{
    unsigned short id_single;
    unsigned short id_group_1;
    unsigned short id_group_2;
    unsigned int max_5v;
    unsigned int max_12v;
};
#endif

#endif

#ifndef DEBUG
#define FIRMWARE_VERSION F(FIRMWARE_VERSION_RAW)
#endif

#ifdef DEBUG
#define FIRMWARE_VERSION F(FIRMWARE_VERSION_RAW "D")
#endif