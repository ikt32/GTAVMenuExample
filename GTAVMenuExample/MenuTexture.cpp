#include "MenuTexture.hpp"
#include "ImageDimensions.hpp"
#include "Util/Logger.hpp"
#include "Util/Paths.hpp"

#include <inc/main.h>
#include <filesystem>

namespace fs = std::filesystem;

namespace {
    std::map<std::string, SMenuTexture> textures;
}

const std::map<std::string, SMenuTexture>& MenuTexture::GetTextures() {
    return textures;
}

void MenuTexture::UpdateTextures() {
    auto imgPath = Paths::GetModPath() / "img";
    LOG(INFO, "Loading images in {}", imgPath.string());

    for (auto& file : fs::directory_iterator(imgPath)) {
        std::string fileName = file.path().string();
        auto stem = file.path().stem().string();
        LOG(INFO, "Loading {}", fileName);
        LOG(INFO, "  Stem: {}", stem);

        if (textures.find(stem) != textures.end()) {
            LOG(INFO, "  Skip: Image already registered");
            continue;
        }

        auto dims = ImageDimensions::GetIMGDimensions(fileName);
        if (!dims) {
            LOG(ERROR, "  Skip: Failed to get image size");
            continue;
        }

        int handle = createTexture(fileName.c_str());

        auto result = textures.emplace(
            stem,
            SMenuTexture(fileName, handle, dims->Width, dims->Height));

        if (result.second) {
            LOG(INFO, "  Emplaced {} with id [{}]", stem, result.first->second.Handle);
        }
        else {
            LOG(ERROR, "  Failed to emplace {}", stem);
        }
    }
    std::vector<std::string> filesToErase;
    for (const auto& [stem, texture] : textures) {
        if (!fs::exists(texture.Filename)) {
            filesToErase.push_back(stem);
            LOG(INFO, "Removing stale file handle {} / {}", stem, texture.Filename);
        }
    }
    for (const auto& file : filesToErase) {
        textures.erase(file);
    }
}
