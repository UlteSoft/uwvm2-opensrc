﻿/*************************************************************
 * Ultimate WebAssembly Virtual Machine (Version 2)          *
 * Copyright (c) 2025-present UlteSoft. All rights reserved. *
 * Licensed under the APL-2.0 License (see LICENSE file).    *
 *************************************************************/

/**
 * @author      MacroModel
 * @version     2.0.0
 * @date        2025-03-27
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
import uwvm2.utils.cmdline;
import uwvm2.uwvm.cmdline.params;
#else
// std
# include <memory>
// import
# include <uwvm2/utils/cmdline/impl.h>
# include "params/impl.h"
#endif

#ifndef UWVM_MODULE_EXPORT
# define UWVM_MODULE_EXPORT
#endif

UWVM_MODULE_EXPORT namespace uwvm2::uwvm::cmdline
{
    namespace details
    {
        inline constexpr ::uwvm2::utils::cmdline::parameter const* parameter_unsort[]{
            // global
            ::std::addressof(::uwvm2::uwvm::cmdline::params::version),
            ::std::addressof(::uwvm2::uwvm::cmdline::params::run),
            ::std::addressof(::uwvm2::uwvm::cmdline::params::help),
            ::std::addressof(::uwvm2::uwvm::cmdline::params::mode),

        // debug
#ifdef _DEBUG
            ::std::addressof(::uwvm2::uwvm::cmdline::params::debug_test),
#endif

            // wasm
            ::std::addressof(::uwvm2::uwvm::cmdline::params::wasm_abi),
            ::std::addressof(::uwvm2::uwvm::cmdline::params::wasm_set_main_module_name),
            ::std::addressof(::uwvm2::uwvm::cmdline::params::wasm_preload_library),
#if (defined(_WIN32) || defined(__CYGWIN__)) && (!defined(__CYGWIN__) && !defined(__WINE__)) ||                                                                \
    ((!defined(_WIN32) || defined(__WINE__)) && (__has_include(<dlfcn.h>) && (defined(__CYGWIN__) || (!defined(__NEWLIB__) && !defined(__wasi__)))))
            ::std::addressof(::uwvm2::uwvm::cmdline::params::wasm_register_dl),
#endif

            // log
            ::std::addressof(::uwvm2::uwvm::cmdline::params::log_output),
            ::std::addressof(::uwvm2::uwvm::cmdline::params::log_disable_warning),
            ::std::addressof(::uwvm2::uwvm::cmdline::params::log_verbose),
#if defined(_WIN32) && (_WIN32_WINNT < 0x0A00 || defined(_WIN32_WINDOWS))
            ::std::addressof(::uwvm2::uwvm::cmdline::params::log_win32_use_ansi),
#endif
        };
    }  // namespace details

    inline constexpr auto parameters{::uwvm2::utils::cmdline::parameter_sort(details::parameter_unsort)};

    inline constexpr ::std::size_t parameter_lookup_table_size{::uwvm2::utils::cmdline::calculate_alias_parameters_size(parameters)};
    inline constexpr auto parameter_lookup_table{::uwvm2::utils::cmdline::expand_alias_parameters_and_check<parameter_lookup_table_size>(parameters)};

    inline constexpr ::std::size_t parameter_max_principal_name_size{::uwvm2::utils::cmdline::calculate_max_principal_name_size(parameters)};
    inline constexpr ::std::size_t parameter_max_name_size{::uwvm2::utils::cmdline::calculate_max_para_size(parameter_lookup_table)};

    inline constexpr auto hash_table_size{::uwvm2::utils::cmdline::calculate_hash_table_size(parameter_lookup_table)};
    inline constexpr auto hash_table{
        ::uwvm2::utils::cmdline::generate_hash_table<hash_table_size.hash_table_size, hash_table_size.extra_size, hash_table_size.real_max_conflict_size>(
            parameter_lookup_table)};

    inline constexpr ::std::size_t hash_table_byte_sz{sizeof(hash_table)};
    // inline constexpr auto sizeof_hash_table{sizeof(hash_table)};
}  // namespace uwvm2::uwvm::cmdline
