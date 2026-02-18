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
    // data members

    ifstream input_stream;
    ofstream output_stream;

    // curr line number in HACK program (tracks ROM address)
    int line_number;

    // default stack base
    int default_sp_address = 256;

    // next free RAM cell for variables (DCD starts at idx 16)
    int var_address = 16;


    // lookup table 1
    // Hashmap for associating registers with their addresses in RAM
    map<string, int> reg_map;

    // lookup table 2
    // Hashmap for ARM BXX -> Hack JXX mnemonics
    map<string, string> jump_map;

    // lookup table 3
    // Hashmap for ARM labels -> Hack line numbers
    map<string, int> label_map;

    // lookup table 4
    // Hashmap for Hack line numbers -> ARM labels (for forward jumps)
    // when we encounter a forward jump, we don't know the target yet
    // we write @-1 as placeholder and record: fix_map[hack_line] = label and then
    // the second pass will fix these by looking up label_map[label]
    map<int, string> fix_map;
    
    // lookup table 5
    // hashmap for variable/array name into RAM address
    map<string, int> var_map;


    // methods

    void write_line(string line);
    void write_oper2(string token);
    void write_pcjump(string rd_str);

    void translateFirstPass(string in_filename, string out_filename);
    void translateSecondPass(string in_filename, string out_filename);

    void translate(string line);
    void translateJumps(string line);
    void translateXXX(string line);
    void translateDCD(string line);
};

#endif // ARMTOHACK_H
