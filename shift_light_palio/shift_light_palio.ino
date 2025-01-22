#include "ELMduino.h"
#include <BluetoothSerial.h>

#define RPMlight 33
#define ConnectLight 25
#define debug 26

BluetoothSerial SerialBT;
#define ELM_PORT SerialBT

const bool DEBUG = true;
const int TIMEOUT = 2000;
const bool HALT_ON_FAIL = true;

//#define USE_NAME           // Comment this to use MAC address instead of a slaveName
const char *pin = "1234";  //Change this to reflect the pin expected by the real slave BT device

#if !defined(CONFIG_BT_SPP_ENABLED)
#error Serial Bluetooth not available or not enabled. It is only available for the ESP32 chip.
#endif

ELM327 myELM327;


typedef enum { ENG_RPM } obd_pid_states;

obd_pid_states obd_state = ENG_RPM;

//nome do dispostivo escravo
String slaveName = "OBDII";  // Change this to reflect the real name of your slave BT device

//meu nome
String myName = "ESP32-BT-Master";
//MAC da ELM327
uint8_t address[6] = { 0x00, 0x10, 0xCC, 0x4F, 0x36, 0x03 };  // Change this to reflect real MAC address of your slave BT device

float RPM = 0;

void setup() {

  ELM_PORT.begin(115200);

  // SerialBT.setPin("1234");
  Serial.println("tetando se conectar a OBDII");
  ELM_PORT.begin(myName, true);

  bool connected;

  // if (!myELM327.begin(ELM_PORT, DEBUG, TIMEOUT)) {
  //     Serial.println("Couldn't connect to OBD scanner");

  //     if (HALT_ON_FAIL)
  //       while(1);
  //   }

  // SerialBT.setPin(pin);

  connected = SerialBT.connect(address);

  Serial.print("Connecting to slave BT device with MAC ");

  pinMode(RPMlight, OUTPUT);
  pinMode(ConnectLight, OUTPUT);
  pinMode(debug, OUTPUT);

  digitalWrite(debug, HIGH);


  if (!ELM_PORT.connect("OBDII")) {
    Serial.println("Não foi possível conectar ao scanner OBD I");
    while (1)
      ;
  }
  if (!myELM327.begin(ELM_PORT, DEBUG, TIMEOUT)) {
    Serial.println("Não foi possível conectar ao scanner OBD II");
    while (1)
      ;
  }

  Serial.println("Connected to ELM327");
  digitalWrite(ConnectLight, HIGH);
  digitalWrite(debug, LOW);
}

void loop() {

  switch (obd_state) {
    case ENG_RPM:
      {
        RPM = myELM327.rpm();

        if (myELM327.nb_rx_state == ELM_SUCCESS) {
          Serial.print("RPM: ");
          Serial.println(RPM);
          while (RPM >= 1500) {
            digitalWrite(RPMlight, HIGH);
          }
        } else if (myELM327.nb_rx_state != ELM_GETTING_MSG) {
          myELM327.printError();
        }
      }
  }
}
