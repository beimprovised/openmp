//===------------- objects.cu - NVPTX OpenMP GPU objects --------- CUDA -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.txt for details.
//
//===----------------------------------------------------------------------===//
//
// This file contains the data objects used on the GPU device.
//
//===----------------------------------------------------------------------===//

#include "../../../deviceRTLs/nvptx/src/omptarget-nvptx.h"
#include "../../../deviceRTLs/nvptx/src/state-queue.h"

////////////////////////////////////////////////////////////////////////////////
// global data holding OpenMP state information
////////////////////////////////////////////////////////////////////////////////

__device__
omptarget_nvptx_Queue<omptarget_nvptx_ThreadPrivateContext, OMP_STATE_COUNT>
    omptarget_nvptx_device_State[MAX_SM];

// Pointer to this team's OpenMP state object
__device__ __shared__ omptarget_nvptx_ThreadPrivateContext
    *omptarget_nvptx_threadPrivateContext;

////////////////////////////////////////////////////////////////////////////////
// The team master sets the outlined parallel function in this variable to
// communicate with the workers.  Since it is in shared memory, there is one
// copy of these variables for each kernel, instance, and team.
////////////////////////////////////////////////////////////////////////////////
__device__ __shared__ omptarget_nvptx_WorkFn   omptarget_nvptx_workFn;

////////////////////////////////////////////////////////////////////////////////
// OpenMP kernel execution mode
////////////////////////////////////////////////////////////////////////////////
__device__ __shared__ int8_t execution_mode;

////////////////////////////////////////////////////////////////////////////////
// Data sharing state
////////////////////////////////////////////////////////////////////////////////
__device__ __shared__ DataSharingStateTy DataSharingState;

