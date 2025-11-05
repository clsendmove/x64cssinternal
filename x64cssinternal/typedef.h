#pragma once
#include <math.h>
#include <Windows.h>
#include <memory>
#include <utility>

typedef void* (*CreateClientClassFn)(int entnum, int serialNum);
typedef void* (*CreateEventFn)();

#define fc __fastcall
#define fc_rg void* rcx // guh, some hooks dont have fastcall registers. (CL_Move)

#define add_interface(name, target_class) \
  namespace i { inline target_class* name; }

#define add_feature(name, target_class) \
  namespace f { inline target_class name; }

#define add_feature_(name, target_class) \
	namespace f {inline target_class* name = new target_class; }