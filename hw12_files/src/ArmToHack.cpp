#include "ArmToHack.h"

ArmToHack::ArmToHack() {
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
    jump_map["BAL"] = "JMP";  // unconditional branch
    jump_map["BL"] = "JMP";   // branch and link (handled specially in translateJumps)
}

void ArmToHack::reset() {
    // clears data members in preparation for another translation
    line_number = 0;
    label_map.clear();  // clear labels from previous translation
    fix_map.clear();  // clear forward jump fix-ups from previous translation
}

void ArmToHack::translate(string in_filename, string out_filename) {
    // pass 1: translate ARM -> Hack, writing @-1 for unknown forward jumps
    //      2: fix @-1 placeholders now that all labels are known
    string tmp_filename = in_filename + ".tmp";
    translateFirstPass(in_filename, tmp_filename);
    translateSecondPass(tmp_filename, out_filename);
}

void ArmToHack::write_line(string line) {
    output_stream << line << endl;
    // increment line number so jumps land on correct instructions after each write
    line_number++;
}

void ArmToHack::write_oper2(string token) {
    // handles operand2: either a register (Rz) or literal (#N, #+N, #-N)
    // loads the value into D at the end for additional functions in translateXXX method

    if (token[0] == '#') {
        // literal value - strip the # from it
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

void ArmToHack::write_pcjump(string rd_str) {
    // If destination register is PC (R15 or PC), we need to actually jump
    // to the value we just stored, since writing to memory doesn't
    // automatically change execution flow in Hack
    if (rd_str == "PC" || rd_str == "R15") {
        write_line("@15");    // Select PC's memory location
        write_line("A=M");    // Load PC's value into A register
        write_line("0;JMP");  // Jump to that address
    }
    // If destination is not PC, do nothing
}

void ArmToHack::translateFirstPass(string in_filename, string out_filename) {
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

void ArmToHack::translateSecondPass(string in_filename, string out_filename) {
    // second pass: fix forward jump placeholders (@-1)
    // reads the temp Hack file from pass 1, and fixes any @-1 lines

    input_stream.open(in_filename);
    output_stream.open(out_filename);

    int current_line = 0;

    while (true) {
        string line;
        getline(input_stream, line);

        // stop if file is completely read
        if (!input_stream && line.empty()) break;

        // check if this line needs fixing (is in fix_map)
        if (fix_map.contains(current_line)) {
            // this line has @-1, replace with correct address
            string label = fix_map[current_line];
            int target_line = label_map[label];
            output_stream << "@" << target_line << endl;
        }
        else {
            // copy line unchanged (no need for fix; doesn't contain @-1)
            output_stream << line << endl;
        }

        current_line++;
    }

    // close streams
    input_stream.close();
    output_stream.close();
}

// dispatches to the relevant translator for the given line
void ArmToHack::translate(string line) {
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

void ArmToHack::translateJumps(string line) {
    // handles BXX commands
    // *D register holds comparison result from previous CMP*

    string opcode = extract_token(line);
    string label = extract_token(line);

    // BL case-specific
    // BL: save return address to LR first
    if (opcode == "BL") {
        // return addr is where execution resumes after BL function call
        // wrote 6 lines total, so return addr = line_number + 6
        int return_addr = line_number + 6;

        // save return address to LR // R14
        write_line("@" + to_string(return_addr)); // get return addr
        write_line("D=A"); // copy to D
        write_line("@14"); // get RAM[14]
        write_line("M=D"); // store RAM[14] in LR

        // do the jump
        if (label_map.contains(label)) { // label known
            int target_line = label_map[label];
            write_line("@" + to_string(target_line));
        }
        else { // label unknown
            fix_map[line_number] = label;
            write_line("@-1");
        }
        write_line("0;JMP");
        return;
    }

    // regular BXX instructions (BEQ, BNE, BGT, etc.)
    // check if this is a back jump (label already known) or forward jump (label unknown)
    if (label_map.contains(label)) {
        // BACK JUMP: label was defined earlier, we know its address
        int target_line = label_map[label];
        write_line("@" + to_string(target_line));
    }
    else {
        // FORWARD JUMP: label hasn't been seen yet
        // write placeholder @-1 and record this line needs fixing
        fix_map[line_number] = label;  // hack line X needs label Y
        write_line("@-1");
    }

    // conditional jump based on D register
    string jump_type = jump_map[opcode];
    write_line("D;" + jump_type);
}

void ArmToHack::translateXXX(string line) {

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

         // if destination is PC, jump to the stored value
         write_pcjump(rd_str);
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

        // if destination is PC, jump to the stored value
        write_pcjump(rd_str);
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

        // if destination is PC, jump to the stored value
        write_pcjump(rd_str);
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

        // if destination is PC, jump to the stored value
        write_pcjump(rd_str);
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
