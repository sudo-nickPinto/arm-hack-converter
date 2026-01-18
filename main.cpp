#include <iostream>
#include <string>
#include "ArmToHack.h"

using namespace std;

int main() {
    ArmToHack translator;

    cout << "Starting ArmToHackASM Translator." << endl;

    translator.translate("test/program1.arm", "test/program1.asm");
    translator.translate("test/program2.arm", "test/program2.asm");
    translator.translate("test/program3.arm", "test/program3.asm");
    translator.translate("test/program4.arm", "test/program4.asm");
    translator.translate("test/program5.arm", "test/program5.asm");

    cout << "Translation complete." << endl;

    return 0;
}
