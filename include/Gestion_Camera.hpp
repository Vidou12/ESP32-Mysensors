#ifndef GESTIONCAMERA_HPP
    #define GESTIONCAMERA_HPP

    #include "Defines.hpp"

    #include <Arduino.h>

    void camera_On();
    void camera_Off(ulong &timeSinceCamOFF, bool &commandCameraOff);

#endif