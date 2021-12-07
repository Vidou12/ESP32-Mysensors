#include "Gestion_Camera.hpp"



void camera_On()
{
	digitalWrite(PIN_CAMERA_OFF, HIGH);
	digitalWrite(PIN_RELAY_CAMERA, HIGH);
}

void camera_Off(ulong &timeSinceCamOFF, bool &commandCameraOff)
{
	if (digitalRead(PIN_CAMERA_OFF))
        digitalWrite(PIN_CAMERA_OFF, LOW);
    else if(millis() - timeSinceCamOFF > DELAY_CAMERA_OFF)
	{
		digitalWrite(PIN_RELAY_CAMERA, LOW);
		commandCameraOff = false;
	}
}