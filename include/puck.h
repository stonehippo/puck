void startup();
void accelerometerSetup();
void wifiSetup();
void MQTT_connect();

boolean isFlipped();

/** FSM state handlers **/

void on_not_flipped_enter();
void on_not_flipped_exit();
void on_flipped_enter();
void on_flipped_exit();
