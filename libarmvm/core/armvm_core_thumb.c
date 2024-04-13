#include "armvm_core_glue.h"

/* **** */

#include "armvm_core_exception.h"
#include "armvm_core_thumb.h"

/* **** */

#include "armvm.h"

/* **** */

#include "libbse/include/log.h"
#include "libbse/include/unused.h"

/* **** */

#include <stdlib.h>

/* **** */

void armvm_core_thumb_step(armvm_core_p const core)
{ LOG_ACTION(exit(-1)); UNUSED(core); }
