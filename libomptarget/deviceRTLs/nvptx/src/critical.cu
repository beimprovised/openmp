//===------ critical.cu - NVPTX OpenMP critical ------------------ CUDA -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.txt for details.
//
//===----------------------------------------------------------------------===//
//
// This file contains the implementation of critical with KMPC interface
//
//===----------------------------------------------------------------------===//

#include <stdio.h>
#include <complex.h>

#include "../../../deviceRTLs/nvptx/src/omptarget-nvptx.h"

EXTERN
void __kmpc_critical(kmp_Indent *loc, int32_t global_tid,
                     kmp_CriticalName *lck) {
  PRINT0(LD_IO, "call to kmpc_critical()\n");
  omptarget_nvptx_TeamDescr &teamDescr = getMyTeamDescriptor();
  omp_set_lock(teamDescr.CriticalLock());
}

EXTERN
void __kmpc_end_critical(kmp_Indent *loc, int32_t global_tid,
                         kmp_CriticalName *lck) {
  PRINT0(LD_IO, "call to kmpc_end_critical()\n");
  omptarget_nvptx_TeamDescr &teamDescr = getMyTeamDescriptor();
  omp_unset_lock(teamDescr.CriticalLock());
}
