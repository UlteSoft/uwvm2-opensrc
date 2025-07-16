﻿/*************************************************************
 * Ultimate WebAssembly Virtual Machine (Version 2)          *
 * Copyright (c) 2025-present UlteSoft. All rights reserved. *
 * Licensed under the APL-2.0 License (see LICENSE file).    *
 *************************************************************/

/**
 * @file        main.cc
 * @brief       main function for crt startup
 * @details     This function does nothing but call the cpp boot method of the other module
 * @author      MacroModel
 * @version     2.0.0
 * @date        2025-03-20
 * @copyright   APL-2.0 License
 */

/****************************************
 *  _   _ __        ____     __ __  __  *
 * | | | |\ \      / /\ \   / /|  \/  | *
 * | | | | \ \ /\ / /  \ \ / / | |\/| | *
 * | |_| |  \ V  V /    \ V /  | |  | | *
 *  \___/    \_/\_/      \_/   |_|  |_| *
 *                                      *
 ****************************************/

// macro
#include <uwvm2/utils/macro/push_macros.h>
// import
#ifdef UWVM_MODULE
import uwvm2.uwvm.crtmain;
#else
// import
# include "crtmain/impl.h"
#endif

/// @brief      (crt) main func. The main function is a special function, but there are still meaningless [[gnu::used]].
/// @details    winnt avoid using int main(argc argv), use int main() directly to avoid wasting time parsing and transcoding ACP.
/// @param      argc Argument Count
/// @param      argv Argument Vector
/// @return     exit(3)
/// @see        ::uwvm2::uwvm::uwvm_main()
UWVM_GNU_USED int main(
#if !((defined(_WIN32) && !defined(__CYGWIN__)) && !defined(_WIN32_WINDOWS))  // NOT WINNT
    int argc,
    char** argv
#endif
)
{
#if !((defined(_WIN32) && !defined(__CYGWIN__)) && !defined(_WIN32_WINDOWS))  // NOT WINNT
    return ::uwvm2::uwvm::uwvm_main_non_winnt(argc, argv);
#else
    return ::uwvm2::uwvm::uwvm_main_winnt();
#endif
}

#include <uwvm2/utils/macro/pop_macros.h>
