#include <iostream>
#include <string>
#include "ArmToHack.h"

using namespace std;

int main() {
    ArmToHack translator;

    cout << "Starting Translator..." << endl;


    // hw 11 test files
    translator.translate("test/input/program1.arm", "test/output/program1.asm");
    translator.translate("test/input/program2.arm", "test/output/program2.asm");
    translator.translate("test/input/program3.arm", "test/output/program3.asm");
    translator.translate("test/input/program4.arm", "test/output/program4.asm");
    translator.translate("test/input/program5.arm", "test/output/program5.asm");
    translator.translate("test/input/program6.arm", "test/output/program6.asm");
    translator.translate("test/input/program7.arm", "test/output/program7.asm");
    translator.translate("test/input/program8.arm", "test/output/program8.asm");
    translator.translate("test/input/program9.arm", "test/output/program9.asm");

    // final test (hw11)
    translator.translate("test/input/program10.arm", "test/output/program10.asm");

    // hw 12 test
    translator.translate("test/input/program11.arm", "test/output/program11.asm");
    translator.translate("test/input/program12.arm", "test/output/program12.asm");
    translator.translate("test/input/program13.arm", "test/output/program13.asm");
    translator.translate("test/input/program14.arm", "test/output/program14.asm");
    translator.translate("test/input/program15.arm", "test/output/program15.asm");
    translator.translate("test/input/program16.arm", "test/output/program16.asm");
    translator.translate("test/input/program17.arm", "test/output/program17.asm");
    translator.translate("test/input/program18.arm", "test/output/program18.asm");
    translator.translate("test/input/program19.arm", "test/output/program19.asm");
    translator.translate("test/input/program20.arm", "test/output/program20.asm");
    translator.translate("test/input/program21.arm", "test/output/program21.asm");
    translator.translate("test/input/program22.arm", "test/output/program22.asm");

    // ASR tests
    translator.translate("test/input/programASR1.arm", "test/output/programASR1.asm");
    translator.translate("test/input/programASR2.arm", "test/output/programASR2.asm");
    translator.translate("test/input/programASR3.arm", "test/output/programASR3.asm");

    // final test (hw12)
    translator.translate("test/input/program23.arm", "test/output/program23.asm");

    cout << "Completed." << endl;

    return 0;
}
