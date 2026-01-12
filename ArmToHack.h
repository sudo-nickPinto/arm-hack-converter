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
        // R0..R15 describe memory cells 0..15
        for (int i = 0; i <= 15; ++i) {
            reg_map["R" + to_string(i)] = i;  // Examples: R0->0, R1->1
        }
        
        // Special registers Aliases
        reg_map["FP"] = 12; // Frame Pointer -> R12
        reg_map["SP"] = 13; // Stack Pointer -> R13
        reg_map["LR"] = 14; // Link Register -> R14
        reg_map["PC"] = 15; // Program Counter -> R15
    }

    void reset() {
        // Clears the relevant data members in preparation for another translation
        // Must reset instruction counter because Hack ROM always starts at 0
        line_number = 0;
    }

    void translate(string in_filename, string out_filename) {
        // Public Wrapper: For now mostly calls translateFirstPass
        translateFirstPass(in_filename, out_filename);
    }

private:
    // data members
    ifstream input_stream;
    ofstream output_stream;

    // curr line number in HACK program (tracks ROM address)
    int line_number;

    // lookup table 1
    // Hashmap for associating registers with their addresses in RAM
    map<string, int> reg_map;

    // methods
    void write_line(string line) {
        // Writes a single complete line of Hack Assembly.
        output_stream << line << endl;
        // IMPORTANT: Increment line number so jumps land on correct instructions
        line_number++;
    }
    
    void translateFirstPass(string in_filename, string out_filename) {
        // Opens the two streams with the given filenames and carries out the translation.
        input_stream.open(in_filename);
        output_stream.open(out_filename);
        
        // Safety check
        if (!input_stream.is_open()) return;

        reset(); // Reset counters

        while (true) {
            // Read next line, cleaning out comments and whitespace
            string line = read_line(input_stream);
            
            // Stop if file is done
            if (!input_stream && line.empty()) break;
            
            // Skip empty lines
            if (line.empty()) continue;
            
            // Process the valid instruction
            translate(line);
        }

        // Always close files to flush buffers
        input_stream.close();
        output_stream.close();
    }
    
    void translate(string line) {
        // Simply dispatches to the relevant translator for the given line.
        // Currently we only support "XXX" (Arithmetic/Logic) instructions
        translateXXX(line);
    }

    void translateXXX(string line) {
        // Translates the given line, which is an ARM Assembly instruction
        // Supports: MOV
        
        // Extract the Operation Code (e.g. "MOV")
        string opcode = extract_token(line);
        
        if (opcode == "MOV") {
             // ARM: MOV Rd, Rn  (Rd = Rn)
             // Logic: Load Rn -> D, Store D -> Rd
             
             string rd_str = extract_token(line); // Destination Register
             string rn_str = extract_token(line); // Source Register
             
             int rd = reg_map[rd_str]; // Lookup Address
             int rn = reg_map[rn_str]; // Lookup Address

             // 1. Get Source (Rn)
             write_line("@" + to_string(rn));
             write_line("D=M"); // D holds value of Rn
             
             // 2. Put in Dest (Rd)
             write_line("@" + to_string(rd));
             write_line("M=D"); // Rd's memory gets D
        }
    }
};

#endif // ARMTOHACK_H
