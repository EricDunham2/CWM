#ifndef EEPROMHELPER_H
#define EEPROMHELPER_H

#include <EEPROM.h>
#include <Arduino.h>

class EEPROMHelper {
    public :
        static void begin(const int eepromSize) {
            EEPROM.begin(eepromSize);
        }

        static void end() {
            EEPROM.end();
        }

        template<typename T>
        static void saveObject(const int address, const T& t) {
            EEPROM.put(address, t);
            EEPROM.commit();
        }

        template<typename T>
        static void getObject(const int address, const T& t) {
            EEPROM.get(address, t);
        }

        static void format(unsigned long size) {
            for (unsigned long i = 0; i<size; i++) EEPROM.write(i, 0x00);
            EEPROM.commit();
        }
};

#endif