// ChordLibrary.cpp
#include "ChordLibrary.h"
#include <unordered_map>

namespace ChordLibrary {
    const ChordDefinition CMajor = {"C", {261.63f, 329.63f, 392.00f, 0.0f}};
    const ChordDefinition CMaj7 = {"Cmaj7", {261.63f, 329.63f, 392.00f, 493.88f}};
    const ChordDefinition C6 = {"C6", {261.63f, 329.63f, 392.00f, 440.00f}};
    // Define all other chords here

    static const std::unordered_map<std::string, const ChordDefinition&> chordMap = {
        {"C", CMajor},
        {"Cmaj7", CMaj7},
        {"C6", C6},
        // Add all other chords to this map
    };

    const ChordDefinition& GetChordByName(const std::string& name) {
        auto it = chordMap.find(name);
        if (it != chordMap.end()) {
            return it->second;
        }
        // Return a default chord or throw an exception if not found
        return CMajor;
    }
}