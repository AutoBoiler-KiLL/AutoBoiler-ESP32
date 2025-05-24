#ifndef UTILS_H
#define UTILS_H

#include <ArduinoJson.h> // For JsonDocument

// #include <KiLL.h> // Will be moved to Utils.cpp
// #include <Memory.h> // Will be moved to Utils.cpp

namespace Utils {
    // Declaration only
    bool verifyRequest(JsonDocument document);
}

#endif // UTILS_H