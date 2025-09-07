#include <SDL.h>
#include <filesystem>
#include <format>
#include <fstream>
#include <string>
#include <unordered_map>
#include "window.h"

using std::domain_error;
using std::ifstream;
using std::optional;
using std::string;
using std::string_view;
using std::vector;
using std::filesystem::path;


class WadReader {
    ifstream wad;

  public:
    explicit WadReader(const path& wad_file)
        : wad{wad_file, std::ios::binary} {
        wad.exceptions(ifstream::failbit | ifstream::badbit);
    }

    WadReader(WadReader&& other) noexcept = default;

    void seek(const std::streamoff pos) {
        wad.seekg(pos);
    }

    void read(char* buffer, const std::streamsize count) {
        wad.read(buffer, count);
        if (wad.gcount() != count) {
            const auto error{std::format("Failed to extract {} bytes", count)};
            throw domain_error{error};
        }
    }

    Sint32 readInt() {
        Sint32 i{};
        read((char*) &i, sizeof(i));
        return SDL_SwapLE32(i);
    }

    Sint16 readShort() {
        Sint16 i{};
        read((char*) &i, sizeof(i));
        return SDL_SwapLE16(i);
    }

    string readString(const std::streamsize size) {
        vector<char> buffer(size);
        read(buffer.data(), size);
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
    string id;

    // An integer specifying the number of lumps in the WAD.
    Sint32 num_lumps;

    // An integer holding a pointer to the location of the directory.
    Sint32 directory_ofs;

    explicit WadHeader(WadReader& reader)
        : id{reader.readString(4)}
        , num_lumps{reader.readInt()}
        , directory_ofs{reader.readInt()} {
        if (id != "IWAD" && id != "PWAD") {
            const auto error{"WAD contains invalid id"};
            throw domain_error{error};
        }
        if (num_lumps <= 0) {
            const auto error{"WAD contains invalid number of lumps"};
            throw domain_error{error};
        }
        if (directory_ofs <= 0) {
            const auto error{"WAD contains invalid directory offset"};
            throw domain_error{error};
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
    string name;

    explicit WadLump(WadReader& reader)
        : position{reader.readInt()}
        , size{reader.readInt()}
        , name{reader.readString(8)} {
        if (position < 0) {
            const auto error{
                std::format("Lump \"{}\" contains invalid data offset!", name)
            };
            throw domain_error{error};
        }
        if (size < 0) {
            const auto error{
                std::format("Lump \"{}\" contains invalid size!", name)
            };
            throw domain_error{error};
        }
    }

    [[nodiscard]]
    bool isMarker() const {
        return size == 0;
    }
};

class WadDirectory {
    vector<WadLump> lumps{};
    std::unordered_map<string_view, Sint32> lump_map{};

  public:
    WadDirectory(WadReader& reader, const WadHeader& header) {
        const auto num_lumps{header.num_lumps};
        reader.seek(header.directory_ofs);
        lumps.reserve(num_lumps);
        lump_map.reserve(num_lumps);
        for (Sint32 i = 0; i < num_lumps; i++) {
            lumps.emplace_back(reader);
            lump_map.try_emplace(lumps[i].name, i);
        }
    }

    WadDirectory(WadDirectory&& other) noexcept = default;

    [[nodiscard]]
    optional<Sint32> searchLump(const string_view lump_name) const {
        const auto lump_index{lump_map.find(lump_name)};
        if (lump_index == lump_map.end()) {
            return std::nullopt;
        }
        return lump_index->second;
    }

    [[nodiscard]]
    const WadLump& getLump(const Sint32 lump_index) const {
        if (lump_index < 0 || lump_index >= lumps.size()) {
            throw domain_error{"No valid lump index"};
        }
        return lumps[lump_index];
    }
};

/**
 * A WAD file consists of a header, a directory, and the data lumps
 * that make up the resources stored within the file. A WAD file can
 * be of two types:
 * - IWAD: An "Internal WAD" (or "Initial WAD"), or a core WAD that is
 *   loaded automatically by the engine and generally provides all the
 *   data required to run the game.
 * - PWAD: A "Patch WAD", or an optional file that replaces data from
 *   the IWAD loaded or provides additional data to the engine.
 */
class WadFile {
    WadReader reader;
    WadHeader header;
    WadDirectory directory;

  public:
    explicit WadFile(const path& wad_file)
        : reader{wad_file}
        , header{reader}
        , directory{reader, header} {
    }

    WadFile(WadFile&& other) noexcept = default;

    [[nodiscard]]
    optional<Sint32> searchLump(const string_view lump_name) const {
        return directory.searchLump(lump_name);
    }

    [[nodiscard]]
    const WadLump& getLump(const Sint32 lump_index) const {
        return directory.getLump(lump_index);
    }

    [[nodiscard]]
    vector<Uint8> getLumpData(const Sint32 lump_index) {
        const auto lump{getLump(lump_index)};
        vector<Uint8> lump_data(lump.size);
        reader.seek(lump.position);
        reader.read((char*) lump_data.data(), lump.size);
        return lump_data;
    }
};


struct LumpIndex {
    size_t wad;
    Sint32 lump;

    LumpIndex(const size_t wad, const Sint32 lump)
        : wad{wad}
        , lump{lump} {
    }

    friend LumpIndex operator+(const LumpIndex& index, const Sint32& inc) {
        return {index.wad, index.lump + inc};
    }

    friend LumpIndex operator+(const Sint32& inc, const LumpIndex& index) {
        return index + inc;
    }
};

class WadManager {
    vector<WadFile> files{};

    [[nodiscard]]
    optional<LumpIndex> searchLump(const string_view lump_name) const {
        for (size_t i = 0; i < files.size(); i++) {
            auto lump{files[i].searchLump(lump_name)};
            if (lump.has_value()) {
                return LumpIndex{i, *lump};
            }
        }
        return std::nullopt;
    }

  public:
    void addWad(const path& wad_file) {
        files.emplace_back(wad_file);
    }

    [[nodiscard]]
    bool hasLump(const string_view lump_name) const {
        return searchLump(lump_name) != std::nullopt;
    }

    [[nodiscard]]
    LumpIndex getLumpIndex(const string_view lump_name) const {
        if (const auto lump_index{searchLump(lump_name)}) {
            return *lump_index;
        }
        const auto error{std::format("Could not find lump \"{}\"", lump_name)};
        throw domain_error{error};
    }

    [[nodiscard]]
    vector<Uint8> getLumpData(const LumpIndex& lump_index) {
        WadFile& wad{files[lump_index.wad]};
        const auto lump{lump_index.lump};
        return wad.getLumpData(lump);
    }

    [[nodiscard]]
    vector<Uint8> getLumpData(const string_view lump_name) {
        const auto lump_index{getLumpIndex(lump_name)};
        return getLumpData(lump_index);
    }
};


int main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[]) {
    WadManager wad_manager;
    wad_manager.addWad("doom.wad");

    SDL_InitSubSystem(SDL_INIT_VIDEO);
    Window window{};

    SDL_InitSubSystem(SDL_INIT_EVENTS);
    auto quit{false};
    while (!quit) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            quit = (event.type == SDL_QUIT);
        }
        SDL_Delay(16); // 60 FPS
    }

    return EXIT_SUCCESS;
}
