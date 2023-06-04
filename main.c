#include "cli/cli_utils.h"
#include "ui/ui_utils.h"

int main(int argc, char *argv[]) {
    if (argc > 1) {
        cli_main(argc, argv);
    } else {
        ui_main(argc, argv);
    }

    return 0;
}