#ifndef PROJECT_CONF_H_
#define PROJECT_CONF_H_
#endif
// Enable energy stats collection
#define ENERGEST_CONF_ON 1
#define SERIAL_LINE_CONF_ENABLED 1
#define WITH_SERIAL_LINE 1
#define ASCON_ENABLED 1
#define PLATFORM_SUPPORTS_ADC 1
#define VOLTAGE_SENSOR_PIN 1  // Assume pin 1 is used for internal voltage sensor
#define CURRENT_SENSOR_PIN 2  // Assume pin 2 is used for current sensor (via a shunt resistor)
#define SHUNT_RESISTOR_VALUE 0.1 // Example: 0.1 ohms shunt resistor for current sensing
#define V_TX 3.3  // Assume typical TX voltage (e.g., 3.3V)
#define I_TX 0.03 // Assume typical TX current (e.g., 30mA)
#undef ENCRPTION_ON
#define V_RX 3.3  // Assume typical RX voltage (e.g., 3.3V)
#define I_RX 0.015 // Assume typical RX current (e.g., 15mA)

#define V_CPU 3.3  // Assume typical CPU voltage (e.g., 3.3V)
#define I_CPU 0.005 // Assume typical CPU current (e.g., 5mA)
#ifndef ASCON_CONF

#define ASCON_CONF

#endif /* PROJECT_CONF_H_ */