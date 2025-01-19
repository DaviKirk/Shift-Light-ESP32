#include "ELMduino.h"
#include "BluetoothSerial.h"



BluetoothSerial SerialBT;
#define ELM_PORT SerialBT

const bool DEBUG = true;
const int TIMEOUT = 2000;
const bool HALT_ON_FAIL = false;


ELM327 myELM327;


typedef enum { ENG_RPM } obd_pid_states;
obd_pid_states
  obd_state = ENG_RPM;


uint32_t rpm = 0;

void setup() {
#if LED_BUILTIN
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
#endif

  ELM_PORT.begin(38400);
  // SerialBT.setPin("1234");
  ELM_PORT.begin("ArduHUD", true);

  if (!ELM_PORT.connect("OBDII")) {
    Serial.println("Não foi possível conectar ao scanner OBD I");
    while (1)
;
  }
  if (!myELM327.begin(ELM_PORT, true, 2000)) {
    Serial.println("Não foi possível conectar ao scanner OBD II");
    while (1)
      ;
  }

  Serial.println("Connected to ELM327");
}

void loop() {

  float tempRPM = myELM327.rpm();

  if (myELM327.nb_rx_state == ELM_SUCCESS) {
    rpm = (uint32_t)tempRPM;
    Serial.print("RPM: ");
    Serial.println(rpm);
  } else if (myELM327.nb_rx_state != ELM_GETTING_MSG) {
    myELM327.printError();
  }
}
