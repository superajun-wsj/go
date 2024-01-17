// Copyright 2013 The Go Authors. All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

// This file defines flags attached to various functions
// and data objects. The compilers, assemblers, and linker must
// all agree on these values.
//
// Keep in sync with src/cmd/internal/obj/textflag.go.

// Don't profile the marked routine. This flag is deprecated.
#define NOPROF	1
// It is ok for the linker to get multiple of these symbols. It will
// pick one of the duplicates to use.
#define DUPOK	2
// Don't insert stack check preamble.
#define NOSPLIT	4
// Put this data in a read-only section.
#define RODATA	8
// This data contains no pointers.
#define NOPTR	16
// This is a wrapper function and should not count as disabling 'recover'.
#define WRAPPER 32
// This function uses its incoming context register.
#define NEEDCTXT 64
// Allocate a word of thread local storage and store the offset from the
// thread local base to the thread local storage in this variable.
#define TLSBSS	256
// Do not insert instructions to allocate a stack frame for this function.
// Only valid on functions that declare a frame size of 0.
// TODO(mwhudson): only implemented for ppc64x at present.
#define NOFRAME 512
// Function can call reflect.Type.Method or reflect.Type.MethodByName.
#define REFLECTMETHOD 1024
// Function is the outermost frame of the call stack. Call stack unwinders
// should stop at this function.
#define TOPFRAME 2048
// Function is an ABI wrapper.
#define ABIWRAPPER 4096

// SYSCALL_ENHANCE is for SGX TEE's syscall enhancement, it will hook syscall
// and callback to a special glibc or musl syscall stub.
// step one: assign syscall return address to register rcx, occlum know where
//           should return according to rcx.
// step two: jump to syscall interface address provided by occlum when go
//           .bin file loaded.
// <BYTE $0x48; BYTE $0x8d; BYTE $0x0d; BYTE $0x0c; BYTE $0x00; BYTE $0x00; BYTE $0x00>
// actually is the assembler instruction: lea 0xc(%rip),%rcx
#define SYSCALL_ENHANCE  \
    CMPQ runtime·occlumentry(SB), $0x0  \
    JBE  10(PC)  \
    BYTE $0x48; BYTE $0x8d; BYTE $0x0d; BYTE $0x0c; BYTE $0x00; BYTE $0x00; BYTE $0x00  \
    MOVQ runtime·occlumentry(SB), R11  \
    JMP  R11  \
    SYSCALL
