/*******************************************************************************************************************************************************
 *  _______                         __       __                                     _______                    __          __ __ __                    *
 *|        \                       |  \     |  \                                   |       \                  |  \        |  \  \  \                   * 
 *| ▓▓▓▓▓▓▓▓______ ____   ______  _| ▓▓_   _| ▓▓_    ______  __    __  ______      | ▓▓▓▓▓▓▓\ ______  __    __| ▓▓ ______  \▓▓ ▓▓ ▓▓ ______   ______   *
 *| ▓▓__   |      \    \ /      \|   ▓▓ \ |   ▓▓ \  /      \|  \  |  \/      \     | ▓▓__/ ▓▓/      \|  \  |  \ ▓▓|      \|  \ ▓▓ ▓▓/      \ /      \  *
 *| ▓▓  \  | ▓▓▓▓▓▓\▓▓▓▓\  ▓▓▓▓▓▓\\▓▓▓▓▓▓  \▓▓▓▓▓▓ |  ▓▓▓▓▓▓\ ▓▓  | ▓▓  ▓▓▓▓▓▓\    | ▓▓    ▓▓  ▓▓▓▓▓▓\ ▓▓  | ▓▓ ▓▓ \▓▓▓▓▓▓\ ▓▓ ▓▓ ▓▓  ▓▓▓▓▓▓\  ▓▓▓▓▓▓\ *
 *| ▓▓▓▓▓  | ▓▓ | ▓▓ | ▓▓ ▓▓    ▓▓ | ▓▓ __  | ▓▓ __| ▓▓    ▓▓ ▓▓  | ▓▓ ▓▓   \▓▓    | ▓▓▓▓▓▓▓| ▓▓  | ▓▓ ▓▓  | ▓▓ ▓▓/      ▓▓ ▓▓ ▓▓ ▓▓ ▓▓    ▓▓ ▓▓   \▓▓ *
 *| ▓▓_____| ▓▓ | ▓▓ | ▓▓ ▓▓▓▓▓▓▓▓ | ▓▓|  \ | ▓▓|  \ ▓▓▓▓▓▓▓▓ ▓▓__/ ▓▓ ▓▓          | ▓▓     | ▓▓__/ ▓▓ ▓▓__/ ▓▓ ▓▓  ▓▓▓▓▓▓▓ ▓▓ ▓▓ ▓▓ ▓▓▓▓▓▓▓▓ ▓▓       *
 *| ▓▓     \ ▓▓ | ▓▓ | ▓▓\▓▓     \  \▓▓  ▓▓  \▓▓  ▓▓\▓▓     \\▓▓    ▓▓ ▓▓          | ▓▓      \▓▓    ▓▓\▓▓    ▓▓ ▓▓\▓▓    ▓▓ ▓▓ ▓▓ ▓▓\▓▓     \ ▓▓       *
 * \▓▓▓▓▓▓▓▓\▓▓  \▓▓  \▓▓ \▓▓▓▓▓▓▓   \▓▓▓▓    \▓▓▓▓  \▓▓▓▓▓▓▓ \▓▓▓▓▓▓ \▓▓           \▓▓       \▓▓▓▓▓▓  \▓▓▓▓▓▓ \▓▓ \▓▓▓▓▓▓▓\▓▓\▓▓\▓▓ \▓▓▓▓▓▓▓\▓▓       *
 *******************************************************************************************************************************************************/

#include "Defines.hpp"
#include "Gestion_Porte.hpp"
#include "Gestion_Camera.hpp"

#include <MySensors.h>

MyMessage listenOK(ID_LISTEN_OK,V_STATUS);
MyMessage porteMessage(ID_PORTE,V_STATUS);
MyMessage pilesMessage(ID_PILES,V_VOLTAGE);
MyMessage cameraMessage(ID_CAMERA,V_STATUS);
MyMessage tremieMessage(ID_TREMIE,V_STATUS);
MyMessage tempsSleepMessage(ID_SLEEP, V_VAR1);
MyMessage batterieMessage(ID_BATTERIE,V_VOLTAGE);
MyMessage emptyTremieMessage(ID_EMPTY_TREMIE,V_TEXT);
MyMessage middleTremieMessage(ID_MIDDLE_TREMIE,V_TEXT);
MyMessage stateGamelleMessage(ID_GAMELLE_STATE,V_TEXT);

MyMessage debugMessage(10, V_TEXT);

ulong timeSinceCamOFF = 0L;
ulong timeSinceWakeUp = 0L;
ulong timeSinceTremieOn = 0L;

bool command = false;
bool setDeepSleep = false;
bool commandTremieOn = false;
bool commandTremieOff = false;
bool commandCameraOff = false;
bool listenOkToSend = false;

void receive(const MyMessage &message) // Lors de réception d'un message.
{
	if (message.sensor == ID_PORTE) // Si c'est en rapport avec la porte :
	{
		if (message.getBool())
			porteMessage.set(ouverture_Porte());
		else
			porteMessage.set(fermeture_Porte());

		send(porteMessage);
		delay(10);
	}

	else if (message.sensor == ID_CAMERA) // Si rapport avec caméra :
	{
		command = true;

		if (message.getBool())
		{
			camera_On();
			cameraMessage.set(1);
			debugMessage.set("La caméra est maintenant allumée");
		}
		else
		{
			timeSinceCamOFF = millis();

			commandCameraOff = true;
			cameraMessage.set(0);
			debugMessage.set("La caméra est maintenant éteinte");
		}

		send(cameraMessage);
		delay(10);
		send(debugMessage);
		delay(10);
	}
	else if (message.sensor == ID_TREMIE) // Si rapport avec trémie :
	{
		command = true;

		if (message.getBool())
		{
			digitalWrite(PIN_RELAY_TREMIE, HIGH);
			timeSinceTremieOn = millis();
			commandTremieOn = true;
			tremieMessage.set(1);
			send(tremieMessage);

			debugMessage.set("La trémie est maintenant allumée ");			
		}
		else
		{
			commandTremieOn = false;
			commandTremieOff = true;

			debugMessage.set("La trémie est maintenant éteinte.");
		}
		delay(10);
		send(debugMessage);
		delay(10);
	}
	else if (message.sensor == ID_SLEEP) // Si c'est une info du temps de deepsleep :
	{
		uint64_t tempsRecu(0);
		uint16_t tempsSend(0);

		tempsSleepMessage = message;
		tempsRecu = tempsSleepMessage.getUInt();
		tempsRecu *= MICROSECOND_CONVERT;
		tempsSend = tempsSleepMessage.getUInt();
		esp_sleep_enable_timer_wakeup(tempsRecu);

		tempsSleepMessage.set(tempsSend);

		send(tempsSleepMessage);	

		setDeepSleep = true;
	}

	send(pilesMessage);
	delay(10);
	send(batterieMessage);
	delay(1);
}

/*
void wakeup_reason()
{
  switch(esp_sleep_get_wakeup_cause())
  {
    case ESP_SLEEP_WAKEUP_EXT0 : 
	   //Serial.println("Reveil causé par module radio (réception éventuelle).");
    break;
    
    case ESP_SLEEP_WAKEUP_EXT1: 
      //Serial.println("Reveil causé par bouton reset.");
    break;
    
    case ESP_SLEEP_WAKEUP_TIMER: 
      //Serial.println("Reveil causé par timer");
    break;
    
    default: 
      //Serial.println("Wakeup was not caused by deep sleep: other."); 
    break;
  }
}*/

void before() // Initialise tous les messages à envoyer...
{
	debugMessage.set("");
	porteMessage.set(0);
	pilesMessage.set(0);
	cameraMessage.set(0);
	tremieMessage.set(0);
	batterieMessage.set(0);
	emptyTremieMessage.set(0);
	middleTremieMessage.set(0);
	stateGamelleMessage.set(0);
	listenOK.set(listenOkToSend);
	tempsSleepMessage.set(30000000u);
}

void setup()
{
	ledcSetup(0, 5000, 8); // Initialisation du canal pwm 0 à 5kHz en 8 bits (0-254)

	pinMode(PIN_DRIVER_IN1, OUTPUT); //Pilote sens moteur porte
  	pinMode(PIN_DRIVER_IN2, OUTPUT); //Pilote sens moteur porte

	pinMode(PIN_CAMERA_OFF, OUTPUT); // Pilote arret du RPI0W.
	pinMode(PIN_RELAY_PORTE, OUTPUT); // Pilote relais alim puissance moteur porte.
 	pinMode(PIN_RELAY_CAMERA,OUTPUT); // Relais alim Camera (12V pour régul 5V + puissance pour rpi.)
 	pinMode(PIN_RELAY_TREMIE, OUTPUT); // Relais alim. moteur Trémie
  	pinMode(PIN_RELAY_CHARGEUR,OUTPUT); // Relais alim Chargeur de piles alim ESP.
	
	pinMode(PIN_FDC_OPEN, INPUT); // Entrée fdc ouverture
	pinMode(PIN_FDC_CLOSE, INPUT); // Entrée fdc fermeture,
	pinMode(PIN_CONTROL_BATTERY, INPUT); // Entrée ana. info batterie
	pinMode(PIN_CONTOL_PILES, INPUT); // Entrée ana. info piles
	pinMode(PIN_READ_TREMIE_EMPTY, INPUT); // Entrée détecteur grain vide
	pinMode(PIN_READ_TREMIE_MIDDLE,INPUT); // Entrée détecteur grain moitié
	pinMode(PIN_READ_GAMELLE,INPUT); // Entrée detecteur gamelle pleine.

	float tensionPiles = (analogRead(PIN_CONTOL_PILES)*3.3)/4095; // Conversion valeur analogique vers tension. 4095 = 3.3V.

	pilesMessage.set(tensionPiles, 1); // Envoie du message tension des piles avec 1 décimale.

	float tensionBatterie = ((analogRead(PIN_CONTROL_BATTERY)*3.3) / 4095) * 5; // Conversion de valeur analogique en tension (2.88V au max de la batterie, X 5 = 14.4V = 100%).

	batterieMessage.set(tensionBatterie, 1); // Envoie du message tension de la batterie avec 1 décimale.

	//wakeup_reason();

	esp_sleep_enable_timer_wakeup(tempsSleepMessage.getULong()); // Initialise une tempo de reveil.
	esp_sleep_enable_ext0_wakeup(GPIO_NUM_0,LOW); // Definit le GPIO 0 comme pin de réveil de l'esp.

	timeSinceWakeUp = millis();

	listenOkToSend = true; 
	listenOK.set(listenOkToSend);
}

void presentation()
{
	sendSketchInfo("Poulailler 2.4gHz", "1.0");

	present(ID_SLEEP, S_CUSTOM);
	present(ID_PORTE, S_BINARY);
	present(ID_CAMERA, S_BINARY);
	present(ID_TREMIE, S_BINARY);
	present(ID_PILES, S_MULTIMETER,"Piles");
	present(ID_LISTEN_OK, S_INFO);
	present(ID_BATTERIE, S_MULTIMETER,"Batterie");
	present(ID_MIDDLE_TREMIE,S_INFO,"Trémie moitié-vide");
	present(ID_EMPTY_TREMIE,S_INFO,"Trémie vide");
	present(ID_GAMELLE_STATE,S_INFO,"Gamelle");
	present(10,S_INFO,"Debug");
}

void loop()
{
	if (listenOkToSend)
	{
		send(listenOK);

		listenOkToSend = false;
		listenOK.set(listenOkToSend);
		debugMessage.set("Je suis opérationnel");
		
		send(debugMessage);
		send(pilesMessage);
		send(batterieMessage);
	}

	if (setDeepSleep)
	{
		debugMessage.set("Je passe en deepsleep");
		send(debugMessage);

		/*esp_sleep_pd_config(ESP_PD_DOMAIN_RTC_PERIPH,   ESP_PD_OPTION_OFF);
		esp_sleep_pd_config(ESP_PD_DOMAIN_RTC_SLOW_MEM, ESP_PD_OPTION_OFF);
		esp_sleep_pd_config(ESP_PD_DOMAIN_RTC_FAST_MEM, ESP_PD_OPTION_OFF);
		esp_sleep_pd_config(ESP_PD_DOMAIN_XTAL,         ESP_PD_OPTION_OFF);*/

		send(listenOK);
		send(tempsSleepMessage);	

		RF24_sleep();
		esp_deep_sleep_start();
	}
	else if (commandCameraOff)
		camera_Off(timeSinceCamOFF, commandCameraOff);	
	else if (commandTremieOn)
	{
		if (digitalRead(PIN_READ_GAMELLE) || millis() - timeSinceTremieOn > DELAY_TREMIE_FILLING)
    	{
        	commandTremieOn = false;
			commandTremieOff = true;
    	}
	}	
	else if (commandTremieOff)
	{
		commandTremieOff = false;
		bool niveauTremieBas = false;
		bool niveauTremieMedium = false;

		digitalWrite(PIN_RELAY_TREMIE, LOW);

    	tremieMessage.set(0);

		niveauTremieBas = digitalRead(PIN_READ_TREMIE_EMPTY);
    	niveauTremieMedium = digitalRead(PIN_READ_TREMIE_MIDDLE);

		emptyTremieMessage.set(niveauTremieBas);
		middleTremieMessage.set(niveauTremieMedium);

		send(tremieMessage);
		send(emptyTremieMessage);
		send(middleTremieMessage);
	}

	if (pilesMessage.getInt() <= 1.8)
		digitalWrite(PIN_RELAY_CHARGEUR, HIGH);

	if (millis() - timeSinceWakeUp > TEMPO_BEFORE_SLEEP && !digitalRead(PIN_RELAY_PORTE) && !digitalRead(PIN_RELAY_TREMIE) && !digitalRead(PIN_RELAY_CHARGEUR))
	{
		debugMessage.set("Je passe en deepsleep mais pas la radio");
		
		send(debugMessage);
		send(listenOK);

		tempsSleepMessage.set(3600u);
		send(tempsSleepMessage);	
		esp_sleep_enable_timer_wakeup(SLEEP_DAY_DURATION); //1h
		esp_deep_sleep_start();
	}
}