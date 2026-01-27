#include <iostream>
#include <string>
#include "ArmToHack.h"

using namespace std;

int main() {
    ArmToHack translator;

    cout << "Starting Translator..." << endl;

    translator.translate("test/program1.arm", "test/program1.asm");
    translator.translate("test/program2.arm", "test/program2.asm");
    translator.translate("test/program3.arm", "test/program3.asm");
    translator.translate("test/program4.arm", "test/program4.asm");
    translator.translate("test/program5.arm", "test/program5.asm");
    translator.translate("test/program6.arm", "test/program6.asm");
    translator.translate("test/program7.arm", "test/program7.asm");
    translator.translate("test/program8.arm", "test/program8.asm");
    translator.translate("test/program9.arm", "test/program9.asm");

    // final test
    translator.translate("test/program10.arm", "test/program10.asm");

    cout << "Completed." << endl;

    return 0;
}
