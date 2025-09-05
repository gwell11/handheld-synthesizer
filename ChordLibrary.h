// ChordLibrary.h
#pragma once
#include <array>
#include <string>

struct ChordDefinition {
    std::string name;
    std::array<float, 4> frequencies;
};

namespace ChordLibrary {
    extern const ChordDefinition CMajor;
    extern const ChordDefinition CMaj7;
    extern const ChordDefinition C6;
    // Declare all other chords here
    
    // Utility function to get chord by name
    const ChordDefinition& GetChordByName(const std::string& name);
}