#ifndef GESTIONTREMIE_HPP
    #define GESTIONTREMIE_HPP

    #include "Defines.hpp"

    #include <Arduino.h>

    void tremie_On(ulong &tempo, bool &commandTremieOn);
    void tremie_Off(bool &commandTremieOff);
    void controlNiveauxTremie(bool &niveauTremieBas, bool &niveauTremieMedium);

#endif