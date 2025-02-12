//===-- HostInfoMacOSX.h ----------------------------------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef lldb_Host_macosx_HostInfoMacOSX_h_
#define lldb_Host_macosx_HostInfoMacOSX_h_

#include "lldb/Host/posix/HostInfoPosix.h"
#include "lldb/Utility/FileSpec.h"
#include "llvm/Support/VersionTuple.h"

namespace lldb_private {

class ArchSpec;

class HostInfoMacOSX : public HostInfoPosix {
  friend class HostInfoBase;

private:
  // Static class, unconstructable.
  HostInfoMacOSX();
  ~HostInfoMacOSX();

public:
  static llvm::VersionTuple GetOSVersion();
  static bool GetOSBuildString(std::string &s);
  static bool GetOSKernelDescription(std::string &s);
  static FileSpec GetProgramFileSpec();

protected:
  static bool ComputeSupportExeDirectory(FileSpec &file_spec);
  static bool ComputeSupportFileDirectory(FileSpec &file_spec);
  static void ComputeHostArchitectureSupport(ArchSpec &arch_32,
                                             ArchSpec &arch_64);
  static bool ComputeHeaderDirectory(FileSpec &file_spec);
  static bool ComputeSystemPluginsDirectory(FileSpec &file_spec);
  static bool ComputeUserPluginsDirectory(FileSpec &file_spec);
};
}

#endif
