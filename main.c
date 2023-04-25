#include "cli/cli_utils.h"
#include "ui/ui_utils.h"

int main() {
#ifdef USE_CLI
    cli_main();
#else
    ui_main();
#endif

    return 0;
}