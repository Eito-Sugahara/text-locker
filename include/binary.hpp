#include <fstream>
#include <vector>
#include <cstdint>
#include <filesystem>

namespace fs = std::filesystem;

void saveKey(
    const std::vector<int>& key,
    const std::vector<int>& value,
    const fs::path filename
)
{
    std::ofstream file(filename, std::ios::binary);

    if (!file) {
        return;
    }

    for (size_t i = 0; i < key.size(); i++) {
        std::uint8_t k = static_cast<std::uint8_t>(key[i]);
        std::uint8_t v = static_cast<std::uint8_t>(value[i]);

        file.write(reinterpret_cast<const char*>(&k), 1);
        file.write(reinterpret_cast<const char*>(&v), 1);
    }
}

void loadKey(
    std::vector<int>& key,
    std::vector<int>& value,
    const fs::path filename
)
{
    std::ifstream file(filename, std::ios::binary);

    if (!file) {
        return;
    }

    std::uint8_t k;
    std::uint8_t v;

    while (
        file.read(reinterpret_cast<char*>(&k), 1) &&
        file.read(reinterpret_cast<char*>(&v), 1)
    ) {
        key.push_back(static_cast<int>(k));
        value.push_back(static_cast<int>(v));
    }
}

void writeEncrypted(
    const std::vector<int>& encrypted,
    const fs::path filename
)
{
    std::ofstream file(filename, std::ios::binary | std::ios::trunc);

    if (!file) {
        return;
    }

    for (int value : encrypted) {
        std::uint8_t byte = static_cast<std::uint8_t>(value);

        file.write(
            reinterpret_cast<const char*>(&byte),
            1
        );
    }
}

std::vector<int> readEncrypted(
    const fs::path filename
)
{
    std::vector<int> encrypted;

    std::ifstream file(filename, std::ios::binary);

    if (!file) {
        return encrypted;
    }

    std::uint8_t byte;

    while (file.read(
        reinterpret_cast<char*>(&byte),
        1
    )) {
        encrypted.push_back(static_cast<int>(byte));
    }

    return encrypted;
}
