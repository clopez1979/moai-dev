#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <set>
#include <sstream>
#include <iostream>
#include <algorithm>

#include <moai-core/host.h>
#include <moai-core/headers.h>

#include <moai-sim/host.h>
#include <moai-sim/headers.h>

#include <moai-util/headers.h>

#include <zl-util/headers.h>

extern "C"
{
    #include "lua.h"
    #include "lualib.h"
    #include "lauxlib.h"
}

#define UNUSED(p) (( void )p)
