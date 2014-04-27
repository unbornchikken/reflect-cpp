/* reflect.h                                 -*- C++ -*-
   Rémi Attab (remi.attab@gmail.com), 25 Mar 2014
   FreeBSD-style copyright and disclaimer apply

   Main include header for reflect.

   This exists because the order in which the headers are included is hard to
   get right and since all the core headers are always required there's not much
   point trying to deal with includes on a file per file basis.

   Also, I'm lazy.

*/

#pragma once

#include <string>
#include <memory>
#include <functional>
#include <type_traits>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <stdexcept>
#include <cstddef>

#include <iostream> // debug only

#include "utils.h"
#include "clean_type.h"
#include "ref_type.h"
#include "type_vector.h"
#include "function_type.h"
#include "type_name.h"

namespace reflect {

struct Scope;
struct Type;
struct Overloads;
struct Function;
struct Value;

} // namespace reflect

#include "registry.h"
#include "argument.h"
#include "value.h"
#include "cast.h"
#include "value_function.h"
#include "function.h"
#include "overloads.h"
#include "type.h"
#include "scope.h"

#include "argument.tcc"
#include "value.tcc"
#include "function.tcc"
#include "overloads.tcc"
#include "type.tcc"
#include "scope.tcc"

#include "reflect/class.h"

#include "types/primitives.h"
#include "types/reflect/value.h"
