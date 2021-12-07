#include "Gestion_Porte.hpp"

int ouverture_Porte()
{
    ulong timeCount = millis();

	ledcWrite(0, 254); // Initialise le canal pwm 0 (voir setup()) à un signal pwm de 254 (= 1 logique).
	ledcAttachPin(PIN_DRIVER_IN1, 0); // Attache la pin PIN_DRIVER_IN1 au canal pwm N°0 configuré précédement.
	digitalWrite(PIN_DRIVER_IN2, LOW);

    while (digitalRead(PIN_FDC_OPEN))
    {
        if (millis() - timeCount > DELAY_OUVERTURE)
            return -1;
        
        delay(2);
    } 
     
	ledcDetachPin(PIN_DRIVER_IN1);

    return 1;
}

int fermeture_Porte()
{
    ulong timeCount = millis();

	ledcWrite(0, 254);
	ledcAttachPin(PIN_DRIVER_IN2, 0);
	digitalWrite(PIN_DRIVER_IN1, LOW);

    while (digitalRead(PIN_FDC_CLOSE))
    {
        if (millis() - timeCount > DELAY_FERMETURE)
            return -1;
        
        delay(2);
    }
	
	ledcDetachPin(PIN_DRIVER_IN2);

    return 0;
}