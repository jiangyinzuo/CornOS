#include "int_vectors.h"

.text
.globl __alltraps

BUILD_256VECTORS

# vector table
.data
.globl __vectors
__vectors:
    WRITE_256VECTORS