#include "cli/cli_utils.h"
#include "ui/ui_utils.h"

int main(int argc, char *argv[]) {
    #ifdef USE_CLI
    cli_main(argc, argv);
    #else
    ui_main();
    #endif

    return 0;
}