﻿/*************************************************************
 * Ultimate WebAssembly Virtual Machine (Version 2)          *
 * Copyright (c) 2025-present UlteSoft. All rights reserved. *
 * Licensed under the APL-2.0 License (see LICENSE file).    *
 *************************************************************/

/**
 * @author      MacroModel
 * @version     2.0.0
 * @date        2025-05-24
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

#pragma once

#ifdef UWVM_MODULE
#else
// std
# include <cstdint>
# include <cstddef>
#endif

#ifndef UWVM_MODULE_EXPORT
# define UWVM_MODULE_EXPORT
#endif

UWVM_MODULE_EXPORT namespace uwvm2::uwvm::run
{
    /// @brief      main return value
    /// @details    In posix [0, 128) are available return values and [128, 256) are signal values. In wasi_proc_exit the available values are [0, 126), so
    ///             [126, 128) can be used as error codes for non-executable parts of the VM, such as parameter errors, wasm parsing errors, etc. On the
    ///             wasm-wasi version of uwvm, the error code is uniformly represented by 1.
    enum retval
    {
        ok = 0,

#ifdef __wasi__
        parameter_error = 1,  // Invalid parameter or handling failure
        load_error = 1,       // The specified file is not available or cannot be opened
        wasm_parser_error = 1,
        duplicate_module_name = 1

#else
        parameter_error = 126,  // Invalid parameter or handling failure
        load_error = 127,       // The specified file is not available or cannot be opened
        wasm_parser_error = 127,
        duplicate_module_name = 127
#endif

    };
}  // namespace uwvm2::uwvm::custom
