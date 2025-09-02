#include <SDL.h>
#include <string>
#include <iostream>
#include <fstream>
#include <filesystem>
#include <format>
#include <unordered_map>


class WadReader {
    std::ifstream wad;

public:
    explicit WadReader(const std::filesystem::path& wad_file)
        : wad{wad_file, std::ios::binary} {
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
        // Remove any trailing zeroes.
        size_t i = 0;
        for (; i < size; i++) {
            if (buffer[i] == '\0') {
                break;
            }
        }
        return {buffer.data(), i};
    }
};


struct WadHeader {
    // The ASCII characters "IWAD" or "PWAD".
    std::string id;

    // An integer specifying the number of lumps in the WAD.
    Sint32 num_lumps;

    // An integer holding a pointer to the location of the directory.
    Sint32 directory_ofs;

    explicit WadHeader(WadReader& reader)
        : id{reader.readString(4)}, num_lumps{reader.readInt()},
          directory_ofs{reader.readInt()} {
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
    }
};

struct WadLump {
    // An integer holding a pointer to the start of the lump's data in the file.
    Sint32 position;

    // An integer representing the size of the lump in bytes.
    Sint32 size;

    // An ASCII string defining the lump's name. The name has a limit
    // of 8 characters, the same as the main portion of an MS-DOS filename.
    std::string name;

    explicit WadLump(WadReader& reader)
        : position{reader.readInt()}, size{reader.readInt()},
          name{reader.readString(8)} {
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
    }
};

class WadDirectory {
    std::vector<WadLump> lumps;
    std::unordered_map<std::string_view, Sint32> lump_map;

public:
    explicit WadDirectory(WadReader& reader, const WadHeader& header) {
        const auto num_lumps{header.num_lumps};
        reader.seek(header.directory_ofs);
        lumps.reserve(num_lumps);
        lump_map.reserve(num_lumps);
        for (Sint32 i = 0; i < num_lumps; i++) {
            lumps.emplace_back(reader);
            lump_map.try_emplace(lumps[i].name, i);

            std::cout << "WadLump {" << std::endl;
            std::cout << "  position: " << lumps[i].position << std::endl;
            std::cout << "  size: " << lumps[i].size << std::endl;
            std::cout << "  name: " << lumps[i].name << std::endl;
            std::cout << "}" << std::endl;
        }
    }

    [[nodiscard]]
    std::optional<Sint32> searchLump(std::string_view lump_name) const {
        const auto lump_index = lump_map.find(lump_name);
        if (lump_index == lump_map.end()) {
            return std::nullopt;
        }
        return lump_index->second;
    }
};

class WadFile {
    WadReader reader;
    WadHeader header;
    WadDirectory directory;

public:
    explicit WadFile(const std::filesystem::path& wad_file)
        : reader{wad_file}, header{reader}, directory{reader, header} {
    }

    [[nodiscard]]
    std::optional<Sint32> searchLump(std::string_view lump_name) const {
        return directory.searchLump(lump_name);
    }
};

int main(int argc, char* argv[]) {
    const auto filename{"doom.wad"};
    WadFile wad{filename};
    return EXIT_SUCCESS;
}