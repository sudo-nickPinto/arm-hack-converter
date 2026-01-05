#ifndef ARMTOHACK_H
#define ARMTOHACK_H

#include <string>
#include <map>
#include <fstream>
#include "token_io.h"

using namespace std;

class ArmToHack {
public:
    ArmToHack();
    void reset();
    void translate(string in_filename, string out_filename);

private:
    // File streams
    ifstream input_stream;
    ofstream output_stream;

    // Current line number in Hack program
    int hack_line_number;

    // Lookup tables
    map<string, int> register_map;
    map<string, string> jump_map;
    map<string, int> label_map; // Maps ARM labels to Hack line numbers
    map<int, string> hack_to_arm_label_map; // Maps Hack lines to ARM labels (for full jumps)

    // Helper methods
    void translateFirstPass(string in_filename, string out_filename);
    void translateLine(string line);
    void translateInstruction(string line); // translateXXX in description
    void translateJumps(string line);
    void write_line(string line);
    void write_oper2(string token);
};

#endif // ARMTOHACK_H
