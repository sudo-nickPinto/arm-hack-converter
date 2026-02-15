#include <iostream>
#include <string>
#include "ArmToHack.h"

using namespace std;

int main() {
    ArmToHack translator;

    cout << "Starting Translator..." << endl;

    translator.translate("test/input/program1.arm", "test/output/program1.asm");
    translator.translate("test/input/program2.arm", "test/output/program2.asm");
    translator.translate("test/input/program3.arm", "test/output/program3.asm");
    translator.translate("test/input/program4.arm", "test/output/program4.asm");
    translator.translate("test/input/program5.arm", "test/output/program5.asm");
    translator.translate("test/input/program6.arm", "test/output/program6.asm");
    translator.translate("test/input/program7.arm", "test/output/program7.asm");
    translator.translate("test/input/program8.arm", "test/output/program8.asm");
    translator.translate("test/input/program9.arm", "test/output/program9.asm");

    // final test
    translator.translate("test/input/program10.arm", "test/output/program10.asm");

    cout << "Completed." << endl;

    return 0;
}
