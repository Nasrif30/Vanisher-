#pragma once
#include "ewr/payload.h"
#include <cstdint>
#include <string>
#include <vector>

#ifdef _WIN32
#include <windows.h>
#endif

namespace ewr {

    constexpr uint16_t EPSON_VID = 0x04B8;

    typedef void* EwrDeviceHandle;

    // Connect only when the printer-reported IEEE 1284 identity contains the
    // expected model.  This is intentionally fail-closed: an unknown Epson
    // device must never receive another model's EEPROM writes.
    EwrDeviceHandle AutoConnectEpsonPrinter(const std::string& expectedModel);
    bool IdentityMatchesModel(const std::string& identity, const std::string& expectedModel);
    bool ExecutePayloadSequence(EwrDeviceHandle hPrinter, const std::vector<std::vector<unsigned char>>& sequence);
    void DisconnectPrinter(EwrDeviceHandle hPrinter);

}
