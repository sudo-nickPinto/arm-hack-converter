#ifndef ARMTOHACK_H
#define ARMTOHACK_H

#include <string>
#include <map>
#include <fstream>
#include "token_io.h"

using namespace std;

class ArmToHack {
public:
    ArmToHack() {
        // Constructor: Initialize register map here
    }

    void reset() {
        // Clears the relevant data members in preparation for another translation
    }

    void translate(string in_filename, string out_filename) {
        // For now mostly calls translateFirstPass
    }

private:
    // data members
    ifstream input_stream;
    ofstream output_stream;

    // curr line number in HACK program
    int line_number;

    // lookup table 1: Hash Map for associating registers with their addresses in RAM
    map<string, int> reg_map;

    // methods
    void write_line(string line) {
        // Writes a single complete line of Hack Assembly.
    }
    
    void translateFirstPass(string in_filename, string out_filename) {
        // Opens the two streams with the given filenames and carries out the translation.
    }
    
    void translate(string line) {
        // Simply dispatches to the relevant translator for the given line.
    }

    void translateXXX(string line) {
        // Translates the given line, which is an ARM Assembly instruction
        // Supports: MOV, ADD, SUB, RSB, CMP, END
    }
};

#endif // ARMTOHACK_H
