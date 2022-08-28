#pragma once

#include <optional>
#include <string>

namespace ImageDimensions {
    struct SImageDimensions {
        uint32_t Width;
        uint32_t Height;
    };

    std::optional<SImageDimensions> GetIMGDimensions(const std::string& file);
}
