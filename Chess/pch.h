#pragma once
#pragma warning( disable : 4244 ) 

//#define NDEBUG

//std
#include <memory>
#include <string>
#include <queue>
#include <vector>
#include <map>
#include <algorithm>
#include <iostream>
#include <stdexcept>
#include <array>
#include <functional>
#include <stack>
#include <cassert>
#include <cmath>
#include <unordered_map>

//SFML
#include "SFML/Audio.hpp"
#include "SFML/Graphics.hpp"
#include "SFML/Window.hpp"

//TGUI
#include "TGUI/TGUI.hpp"

//Self
#include "Utils.h"
#include "Enums.h"
#include "Bitmask.hpp"
#include "NonCopyable.hpp"
#include "Signal.hpp"



#ifdef NDEBUG
#define ASSERT(Expr, Msg)
#else
#define ASSERT(Expr, Msg) \
do { \
    if(!(Expr)) \
	{ \
    std::cerr << "Assert failed:\t" << Msg << "\n" \
    << "Expected:\t" << #Expr << "\n" \
    << "Source:\t\t" << __FILE__ << ", line " << __LINE__ << "\n";    \
    abort();    \
    } \
} while(0)
#endif
