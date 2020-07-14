#include "rev1.h"

void matrix_init_kb(void) {

    // Turn status LED on
    setPinOutput(E6);
    writePinHigh(E6);

  matrix_init_user();
};
