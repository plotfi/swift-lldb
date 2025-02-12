//===-- SBReproducer.cpp ----------------------------------------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "SBReproducerPrivate.h"

#include "SBReproducerPrivate.h"
#include "lldb/API/LLDB.h"
#include "lldb/API/SBAddress.h"
#include "lldb/API/SBAttachInfo.h"
#include "lldb/API/SBBlock.h"
#include "lldb/API/SBBreakpoint.h"
#include "lldb/API/SBCommandInterpreter.h"
#include "lldb/API/SBData.h"
#include "lldb/API/SBDebugger.h"
#include "lldb/API/SBDeclaration.h"
#include "lldb/API/SBError.h"
#include "lldb/API/SBFileSpec.h"
#include "lldb/API/SBHostOS.h"
#include "lldb/API/SBReproducer.h"

#include "lldb/Host/FileSystem.h"

using namespace lldb;
using namespace lldb_private;
using namespace lldb_private::repro;

SBRegistry::SBRegistry() {
  Registry& R = *this;

  RegisterMethods<SBAddress>(R);
  RegisterMethods<SBAttachInfo>(R);
  RegisterMethods<SBBlock>(R);
  RegisterMethods<SBBreakpoint>(R);
  RegisterMethods<SBBreakpointList>(R);
  RegisterMethods<SBBreakpointLocation>(R);
  RegisterMethods<SBBreakpointName>(R);
  RegisterMethods<SBBroadcaster>(R);
  RegisterMethods<SBCommandInterpreterRunOptions>(R);
  RegisterMethods<SBCommandReturnObject>(R);
  RegisterMethods<SBCommunication>(R);
  RegisterMethods<SBCompileUnit>(R);
  RegisterMethods<SBData>(R);
  RegisterMethods<SBInputReader>(R);
  RegisterMethods<SBDebugger>(R);
  RegisterMethods<SBDeclaration>(R);
  RegisterMethods<SBError>(R);
  RegisterMethods<SBEvent>(R);
  RegisterMethods<SBExecutionContext>(R);
  RegisterMethods<SBExpressionOptions>(R);
  RegisterMethods<SBFileSpec>(R);
  RegisterMethods<SBFileSpecList>(R);
  RegisterMethods<SBFrame>(R);
  RegisterMethods<SBFunction>(R);
  RegisterMethods<SBHostOS>(R);
  RegisterMethods<SBInstruction>(R);
  RegisterMethods<SBInstructionList>(R);
  RegisterMethods<SBLanguageRuntime>(R);
  RegisterMethods<SBLaunchInfo>(R);
  RegisterMethods<SBLineEntry>(R);
  RegisterMethods<SBListener>(R);
  RegisterMethods<SBMemoryRegionInfo>(R);
  RegisterMethods<SBMemoryRegionInfoList>(R);
  RegisterMethods<SBModule>(R);
  RegisterMethods<SBModuleSpec>(R);
  RegisterMethods<SBPlatformConnectOptions>(R);
  RegisterMethods<SBPlatformShellCommand>(R);
  RegisterMethods<SBPlatform>(R);
  RegisterMethods<SBProcess>(R);
  RegisterMethods<SBProcessInfo>(R);
  RegisterMethods<SBQueue>(R);
  RegisterMethods<SBQueueItem>(R);
  RegisterMethods<SBSection>(R);
  RegisterMethods<SBSourceManager>(R);
  RegisterMethods<SBStream>(R);
  RegisterMethods<SBStringList>(R);
  RegisterMethods<SBStructuredData>(R);
  RegisterMethods<SBSymbol>(R);
  RegisterMethods<SBSymbolContext>(R);
  RegisterMethods<SBSymbolContextList>(R);
  RegisterMethods<SBTarget>(R);
  RegisterMethods<SBThread>(R);
  RegisterMethods<SBThreadCollection>(R);
  RegisterMethods<SBThreadPlan>(R);
  RegisterMethods<SBTrace>(R);
  RegisterMethods<SBTraceOptions>(R);
  RegisterMethods<SBType>(R);
  RegisterMethods<SBTypeCategory>(R);
  RegisterMethods<SBTypeEnumMember>(R);
  RegisterMethods<SBTypeFilter>(R);
  RegisterMethods<SBTypeFormat>(R);
  RegisterMethods<SBTypeNameSpecifier>(R);
  RegisterMethods<SBTypeSummaryOptions>(R);
  RegisterMethods<SBTypeSummary>(R);
  RegisterMethods<SBTypeSynthetic>(R);
  RegisterMethods<SBUnixSignals>(R);
  RegisterMethods<SBValue>(R);
  RegisterMethods<SBValueList>(R);
  RegisterMethods<SBVariablesOptions>(R);
  RegisterMethods<SBWatchpoint>(R);
}

const char *SBReproducer::Capture() {
  static std::string error;
  if (auto e = Reproducer::Initialize(ReproducerMode::Capture, llvm::None)) {
    error = llvm::toString(std::move(e));
    return error.c_str();
  }
  return nullptr;
}

const char *SBReproducer::Capture(const char *path) {
  static std::string error;
  if (auto e =
          Reproducer::Initialize(ReproducerMode::Capture, FileSpec(path))) {
    error = llvm::toString(std::move(e));
    return error.c_str();
  }
  return nullptr;
}

const char *SBReproducer::Replay(const char *path) {
  static std::string error;
  if (auto e = Reproducer::Initialize(ReproducerMode::Replay, FileSpec(path))) {
    error = llvm::toString(std::move(e));
    return error.c_str();
  }

  repro::Loader *loader = repro::Reproducer::Instance().GetLoader();
  if (!loader) {
    error = "unable to get replay loader.";
    return error.c_str();
  }

  FileSpec file = loader->GetFile<SBInfo>();
  if (!file) {
    error = "unable to get replay data from reproducer.";
    return error.c_str();
  }

  SBRegistry registry;
  registry.Replay(file);

  return nullptr;
}

char lldb_private::repro::SBProvider::ID = 0;
const char *SBInfo::name = "sbapi";
const char *SBInfo::file = "sbapi.bin";
