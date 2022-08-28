#pragma once
#include <string>
#include <vector>
#include <inc/types.h>

namespace UI {
    void Notify(const std::string& message);
    void Notify(const std::string& message, bool removePrevious);
}

