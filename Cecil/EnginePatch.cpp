/* Copyright (c) 2022-2024 Dreamy Cecil
This program is free software; you can redistribute it and/or modify
it under the terms of version 2 of the GNU General Public License as published by
the Free Software Foundation


This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License along
with this program; if not, write to the Free Software Foundation, Inc.,
51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA. */

#include "StdH.h"

// Original function pointers
static void (*pInitEngineFunc)(CTString) = NULL;
static void (*pEndEngineFunc)(void) = NULL;

// Engine patches
static CPatch *_pInitEnginePatch = NULL;
static CPatch *_pEndEnginePatch = NULL;

// Patched SE_InitEngine() method
static void P_InitEngine(CTString strGameID) {
  // Setup Serious Editor or modelling application
  CCoreAPI::Setup(strGameID == "SeriousEditor" ? CCoreAPI::APP_EDITOR : CCoreAPI::APP_MODELER);

#if CLASSICSPATCH_ENGINEPATCHES
  // Optional patches for full integration into vanilla applications
  if (CCoreAPI::Props().bFullAppIntegration) {
    _EnginePatches.FileSystem();
    _EnginePatches.UnpageStreams();
  }
#endif

  // Initialize Serious Engine
  (*pInitEngineFunc)(strGameID);

  // Initialize the tools
  if (strGameID == "") {
    ClassicsPatch_InitTools();
  }

  // Unpatch initialization method
  delete _pInitEnginePatch;
  _pInitEnginePatch = NULL;
};

// Patched SE_EndEngine() method
static void P_EndEngine(void) {
  // Clean up the core
  ClassicsPatch_EndCore();

  // End Serious Engine
  (*pEndEngineFunc)();
};

// Custom initialization for other tools
static const struct LibInit {
  LibInit()
  {
    // Patch engine methods
    pInitEngineFunc = &SE_InitEngine;
    _pInitEnginePatch = NewRawPatch(pInitEngineFunc, &P_InitEngine, "SE_InitEngine(...)");

    pEndEngineFunc = &SE_EndEngine;
    _pEndEnginePatch = NewRawPatch(pEndEngineFunc, &P_EndEngine, "SE_EndEngine()");
  };
} _libInit;
