#include "ImageDimensions.hpp"
#include "Util/Logger.hpp"
#include <filesystem>
#include <fstream>

namespace fs = std::filesystem;

namespace ImageDimensions {
    std::optional<SImageDimensions> getPNGDimensions(const std::string& file);
    std::optional<SImageDimensions> getJPGDimensions(const std::string& file);
}

namespace {
    std::string to_lower(std::string data) {
        std::transform(data.begin(), data.end(), data.begin(), ::tolower);
        return data;
    }
}

std::optional<ImageDimensions::SImageDimensions> ImageDimensions::GetIMGDimensions(const std::string& file) {
    auto ext = to_lower(fs::path(file).extension().string());
    if (ext == ".png")
        return getPNGDimensions(file);
    if (ext == ".jpg" || ext == ".jpeg")
        return getJPGDimensions(file);
    return {};
}

std::optional<ImageDimensions::SImageDimensions> ImageDimensions::getPNGDimensions(const std::string& file) {
    const uint64_t pngSig = 0x89504E470D0A1A0A;

    std::ifstream img(file, std::ios::binary);

    if (!img.good()) {
        LOG(ERROR, "[IMG]: {} failed to open", file);
        return {};
    }

    uint64_t imgSig = 0x0;

    img.seekg(0);
    img.read((char*)&imgSig, sizeof(uint64_t));

    imgSig = _byteswap_uint64(imgSig);

    if (imgSig == pngSig) {
        uint32_t w, h;

        img.seekg(16);
        img.read((char*)&w, 4);
        img.read((char*)&h, 4);

        return ImageDimensions::SImageDimensions{
            .Width = _byteswap_ulong(w),
            .Height = _byteswap_ulong(h)
        };
    }

    LOG(ERROR, "[IMG]: {} not a PNG file, sig: 0x{:16X}", file, imgSig);
    return {};
}

//https://stackoverflow.com/questions/17847171/c-library-for-getting-the-jpeg-image-size
std::optional<ImageDimensions::SImageDimensions> ImageDimensions::getJPGDimensions(const std::string& file) {
    ImageDimensions::SImageDimensions dims{};
    FILE* image = nullptr;

    errno_t err = fopen_s(&image, file.c_str(), "rb");  // open JPEG image file
    if (!image || err) {
        return {};
    }
    fseek(image, 0, SEEK_END);
    int size = ftell(image);
    fseek(image, 0, SEEK_SET);
    unsigned char* data = (unsigned char*)malloc(size);
    fread(data, 1, size, image);
    /* verify valid JPEG header */
    int i = 0;
    if (data[i] == 0xFF && data[i + 1] == 0xD8 && data[i + 2] == 0xFF && data[i + 3] == 0xE0) {
        i += 4;
        /* Check for null terminated JFIF */
        if (data[i + 2] == 'J' && data[i + 3] == 'F' && data[i + 4] == 'I' && data[i + 5] == 'F' && data[i + 6] == 0x00) {
            while (i < size) {
                i++;
                if (data[i] == 0xFF) {
                    if (data[i + 1] == 0xC0) {
                        dims.Width = data[i + 7] * 256 + data[i + 8];
                        dims.Height = data[i + 5] * 256 + data[i + 6];
                        break;
                    }
                }
            }
        }
        else {
            fclose(image);
            return {};
        }
    }
    else {
        fclose(image);
        return {};
    }
    fclose(image);
    return dims;
}
