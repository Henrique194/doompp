#include <SDL.h>
#include <string>
#include <iostream>
#include <fstream>
#include <filesystem>
#include <format>


class WadReader {
    std::ifstream wad;

public:
    explicit WadReader(const std::filesystem::path& wad_file)
        : wad(wad_file, std::ios::binary) {
        wad.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    }

    void seek(std::streamoff pos) {
        wad.seekg(pos);
    }

    Sint32 readInt() {
        Sint32 i{};
        wad.read(reinterpret_cast<char*>(&i), sizeof(i));
        return SDL_SwapLE32(i);
    }

    Sint16 readShort() {
        Sint16 i{};
        wad.read(reinterpret_cast<char*>(&i), sizeof(i));
        return SDL_SwapLE16(i);
    }

    std::string readString(std::streamsize size) {
        std::vector<char> buffer(size);
        wad.read(buffer.data(), size);
        size_t i = 0;
        for (; i < size; i++) {
            if (buffer[i] == '\0') {
                break;
            }
        }
        buffer.erase(buffer.begin() + i, buffer.end());
        return {buffer.data(), i};
    }
};


struct WadHeader {
    // The ASCII characters "IWAD" or "PWAD".
    std::string id{};

    // An integer specifying the number of lumps in the WAD.
    Sint32 num_lumps{};

    // An integer holding a pointer to the location of the directory.
    Sint32 directory_ofs{};
};

struct WadLump {
    // An integer holding a pointer to the start of the lump's data in the file.
    Sint32 position{};

    // An integer representing the size of the lump in bytes.
    Sint32 size{};

    // An ASCII string defining the lump's name. The name has a limit
    // of 8 characters, the same as the main portion of an MS-DOS filename.
    // The name must be nul-terminated if less than 8 characters; for maximum
    // tool compatibility, it is best to pad any remainder with nul characters
    // as well.
    std::string name{};
};

using WadDirectory = std::vector<WadLump>;

static WadLump readLump(WadReader& reader) {
    const auto position{reader.readInt()};
    const auto size = reader.readInt();
    auto name{reader.readString(8)};
    if (position < 0) {
        const auto error = std::format(
            "Lump \"{}\" contains invalid data offset!",
            name
        );
        throw std::domain_error(error);
    }
    if (size < 0) {
        const auto error = std::format(
            "Lump \"{}\" contains invalid size!",
            name
        );
        throw std::domain_error(error);
    }
    return {
        .position{position},
        .size{size},
        .name{std::move(name)}
    };
}

static WadDirectory readDirectory(WadReader& reader, const WadHeader& header) {
    // We can't use a map, because multiple lumps may have the same name.
    reader.seek(header.directory_ofs);
    WadDirectory directory(header.num_lumps);
    for (int i = 0; i < header.num_lumps; i++) {
        directory[i] = readLump(reader);
    }
    return directory;
}

static WadHeader readHeader(WadReader& reader) {
    auto id{reader.readString(4)};
    const auto num_lumps{reader.readInt()};
    const auto directory_ofs{reader.readInt()};
    if (id != "IWAD" && id != "PWAD") {
        const auto error = "WAD contains invalid id";
        throw std::domain_error(error);
    }
    if (num_lumps <= 0) {
        const auto error = "WAD contains invalid number of lumps";
        throw std::domain_error(error);
    }
    if (directory_ofs <= 0) {
        const auto error = "WAD contains invalid directory offset";
        throw std::domain_error(error);
    }
    return {
        .id{std::move(id)},
        .num_lumps{num_lumps},
        .directory_ofs{directory_ofs},
    };
}


int main(int argc, char* argv[]) {
    const auto filename = "doom.wad";
    WadReader reader(filename);

    const WadHeader header{readHeader(reader)};
    const WadDirectory directory{readDirectory(reader, header)};

    for (const auto& [position, size, name] : directory) {
        std::cout << "WadLump {" << std::endl;
        std::cout << "  position: " << position << std::endl;
        std::cout << "  size: " << size << std::endl;
        std::cout << "  name: " << name << std::endl;
        std::cout << "}" << std::endl;
    }

    return EXIT_SUCCESS;
}
