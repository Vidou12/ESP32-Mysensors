#ifndef DEFINE_HPP
    #define DEFINE_HPP

    #define MY_DEBUG

    #define MY_RADIO_RF24
    #define MY_RF24_CE_PIN 22
    #define MY_RF24_CS_PIN 21
    #define MY_RF24_IRQ_PIN 0
    #define MY_RF24_PA_LEVEL (RF24_PA_MIN) // RF24_PA_LOW, RF24_PA_MIN, RF24_PA_HIGH, RF24_PA_MAX
    #define MY_RF24_DATARATE (RF24_250KBPS)
    //#define MY_RF24_POWER_PIN 22 // Pour couper l'alim du rf24. si mode veille utiliser RF24_sleep.

    #define ID_PORTE 0
    #define ID_CAMERA 1
    #define ID_TREMIE 2
    #define ID_BATTERIE 3
    #define ID_PILES 4
    #define ID_SLEEP 5
    #define ID_MIDDLE_TREMIE 6
    #define ID_EMPTY_TREMIE 7
    #define ID_GAMELLE_STATE 8
    #define ID_LISTEN_OK 9

    #define DELAY_CAMERA_OFF 60000

    #define MICROSECOND_CONVERT 1000000L

    #define SLEEP_DAY_DURATION 3600000000L

    #define PIN_RELAY_CAMERA GPIO_NUM_13
    #define PIN_RELAY_TREMIE GPIO_NUM_15
    #define PIN_CAMERA_OFF GPIO_NUM_32
    #define PIN_RELAY_CHARGEUR GPIO_NUM_33
    #define PIN_RELAY_PORTE GPIO_NUM_16
    #define PIN_DRIVER_IN1 GPIO_NUM_2
    #define PIN_DRIVER_IN2 GPIO_NUM_4
    #define PIN_FDC_OPEN GPIO_NUM_34
    #define PIN_FDC_CLOSE GPIO_NUM_35
    #define PIN_CONTOL_PILES GPIO_NUM_36
    #define PIN_CONTROL_BATTERY GPIO_NUM_39
    #define PIN_READ_GAMELLE GPIO_NUM_26
    #define PIN_READ_TREMIE_MIDDLE GPIO_NUM_25
    #define PIN_READ_TREMIE_EMPTY GPIO_NUM_27

    #define TEMPO_BEFORE_SLEEP 60000

    #define DELAY_OUVERTURE 6000
    #define DELAY_FERMETURE 6000
    #define DELAY_TREMIE_FILLING 5000

#endif