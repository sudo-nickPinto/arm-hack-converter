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
        
        // init reg map here
        for (int i = 0; i <= 15; ++i) {
            reg_map["R" + to_string(i)] = i;  // Examples: R0->0, R1->1
        }
        
        // special registers 
        reg_map["FP"] = 12; // FP
        reg_map["SP"] = 13; // SP
        reg_map["LR"] = 14; // LR
        reg_map["PC"] = 15; // PC

        // init jump map (ARM BXX -> Hack JXX)
        jump_map["BEQ"] = "JEQ";  // branch if equal (zero)
        jump_map["BNE"] = "JNE";  // branch if not equal
        jump_map["BGT"] = "JGT";  // branch if greater than
        jump_map["BGE"] = "JGE";  // branch if greater or equal
        jump_map["BLT"] = "JLT";  // branch if less than
        jump_map["BLE"] = "JLE";  // branch if less or equal
        jump_map["BAL"] = "JMP";    // unconditional branch
    }

    void reset() {
        // clears data members in preparation for another trnaslation
        line_number = 0;
        label_map.clear();  // clear labels from previous translation
    }

    void translate(string in_filename, string out_filename) {
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

    // lookup table 2
    // Hashmap for ARM BXX -> Hack JXX mnemonics
    map<string, string> jump_map;

    // lookup table 3
    // Hashmap for ARM labels -> Hack line numbers
    map<string, int> label_map;



    // methods

    void write_line(string line) {
        output_stream << line << endl;
        // increment line number so jumps land on correct instructions after each write
        line_number++;
    }

    void write_oper2(string token) {
        // handles operand2: either a register (Rz) or literal (#N, #+N, #-N)
        // loads the value into D at the end for additional functions in translateXXX method
        
        if (token[0] == '#') {
            // litersl value - strip the # from it
            strip(token, "#");
            
            // @value
            // D=A
            write_line("@" + token);
            write_line("D=A");
        }
        else {
            // register - look up address and load from memory
            int addr = reg_map[token];
            
            // @reg_addr
            // D=M
            write_line("@" + to_string(addr));
            write_line("D=M");
        }
    }
    
    void translateFirstPass(string in_filename, string out_filename) {
        // opens the two streams with the given filenames
        input_stream.open(in_filename);
        output_stream.open(out_filename);

        reset();

        while (true) {
            // read next line
            string line = read_line(input_stream);
            
            // stop if file is completely read
            if (!input_stream && line.empty()) break;
            
            // skip empty lines
            if (line.empty()) continue;
            
            // modification of this method !
            // check if this line is a label
            // labels have no second token, except END which is also single-token
            // label (not end) = store in map and continue
            // no label or END = translate
            string first = peek_first(line);
            string second = peek_second(line);
            if (second.empty() && first != "END") {
                label_map[first] = line_number;
                continue;
            }
            
            // translate the instruction
            translate(line);
        }

        // close streams
        input_stream.close();
        output_stream.close();
    }

    // dispatches to the relevant translator for the given line
    void translate(string line) {
        string opcode = peek_first(line);
        
        // check if it's a branch instruction
        // if branch instruction (exists in map) -> translate jumps
        // if no, call translateXXX
        if (jump_map.contains(opcode)) {
            translateJumps(line);
        }
        else {
            translateXXX(line);
        }
    }
    
    void translateJumps(string line) {
        // handles BXX commands
        // *D register holds comparison result from previous CMP*
        
        string opcode = extract_token(line);
        string label = extract_token(line);
        
        // get the hack line number for this label
        int target_line = label_map[label];
        
        // load target address into A
        write_line("@" + to_string(target_line));
        
        // conditional jump based on D register
        string jump_type = jump_map[opcode];
        write_line("D;" + jump_type);
    }

    void translateXXX(string line) {
        
        // get opcode for the line to-be translated
        string opcode = extract_token(line);
        
        if (opcode == "MOV") {
             // ARM: MOV Rd, Oper2 (Rd = Oper2)
             // oper2 can be register or #immediate
             
             string rd_str = extract_token(line);
             string oper2 = extract_token(line);
             
             int rd = reg_map[rd_str];

             // load oper2 into D
             write_oper2(oper2);
             
             // store D in Rd
             write_line("@" + to_string(rd));
             write_line("M=D");
        }

        if (opcode == "ADD") {
            // ARM: ADD Rd, Rn, Oper2 --> Rd = Rn + Oper2
            // load oper2 into D first, then add Rn
            
            string rd_str = extract_token(line);
            string rn_str = extract_token(line);
            string oper2 = extract_token(line);
            
            int rd = reg_map[rd_str];
            int rn = reg_map[rn_str];
            
            // load oper2 into D
            write_oper2(oper2);
            
            // add Rn to D (commutative so order doesn't matter)

            // @Rn_addr
            // D = D+M
            write_line("@" + to_string(rn));
            write_line("D=D+M");
            
            // store result in Rd

            // @Rd_addr
            // M = D
            write_line("@" + to_string(rd));
            write_line("M=D");
        }

        if (opcode == "SUB") {
            // ARM: SUB Rd, Rn, Oper2 --> Rd = Rn - Oper2
            // load oper2 into D first, then do M-D to get Rn - Oper2
            
            string rd_str = extract_token(line);
            string rn_str = extract_token(line);
            string oper2 = extract_token(line);
            
            int rd = reg_map[rd_str];
            int rn = reg_map[rn_str];
            
            // load oper2 into D
            write_oper2(oper2);
            
            // compute Rn - D (which is Rn - Oper2)

            // @Rn_addr
            // D = M-D
            write_line("@" + to_string(rn));
            write_line("D=M-D");
            
            // store result in Rd

            // @Rd_addr
            // M = D
            write_line("@" + to_string(rd));
            write_line("M=D");
        }

        if (opcode == "RSB") {
            // ARM: RSB Rd, Rn, Oper2 --> Rd = Oper2 - Rn (reverse subtract)
            // load Rn into D, then compute Oper2 - D
            
            string rd_str = extract_token(line);
            string rn_str = extract_token(line);
            string oper2 = extract_token(line);
            
            int rd = reg_map[rd_str];
            int rn = reg_map[rn_str];
            
            // get Rn into D

            // @Rn_addr
            // D=M
            write_line("@" + to_string(rn));
            write_line("D=M");
            
            // compute Oper2 - D
            if (oper2[0] == '#') {
                // immediate: use A-D
                strip(oper2, "#");
                write_line("@" + oper2);
                write_line("D=A-D");
            }
            else {
                // register: use M-D
                int rm = reg_map[oper2];
                write_line("@" + to_string(rm));
                write_line("D=M-D");
            }
            
            // store result in Rd

            // @Rd_addr
            // M = D
            write_line("@" + to_string(rd));
            write_line("M=D");
        }

        if (opcode == "CMP") {
            // ARM: CMP Rn, Oper2 --> computes Rn - Oper2, result in D for branching
            // same as SUB but no store
            
            string rn_str = extract_token(line);
            string oper2 = extract_token(line);
            
            int rn = reg_map[rn_str];
            
            // load oper2 into D
            write_oper2(oper2);
            
            // compute Rn - D (which is Rn - Oper2), result stays in D

            // @Rn_addr
            // D = M-D
            write_line("@" + to_string(rn));
            write_line("D=M-D");
        }

        if (opcode == "END") {
            // infinite loop to halt the program
            // jump to this line forever

            // @current_line
            // 0;JMP
            write_line("@" + to_string(line_number));
            write_line("0;JMP");
        }
    }
};

#endif // ARMTOHACK_H
