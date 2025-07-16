﻿/*************************************************************
 * Ultimate WebAssembly Virtual Machine (Version 2)          *
 * Copyright (c) 2025-present UlteSoft. All rights reserved. *
 * Licensed under the APL-2.0 License (see LICENSE file).    *
 *************************************************************/

/**
 * @author      MacroModel
 * @version     2.0.0
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
import fast_io;
import uwvm2.parser.wasm.concepts;
import uwvm2.parser.wasm.standard.wasm1.type;
import uwvm2.parser.wasm_custom.customs;
import uwvm2.uwvm.wasm.base;
import uwvm2.uwvm.wasm.feature;
import uwvm2.uwvm.wasm.custom;
#else
// import
# include <fast_io.h>
# include <fast_io_dsal/string_view.h>
# include <uwvm2/parser/wasm/concepts/impl.h>
# include <uwvm2/parser/wasm/standard/wasm1/type/impl.h>
# include <uwvm2/parser/wasm_custom/customs/impl.h>
# include <uwvm2/uwvm/wasm/base/impl.h>
# include <uwvm2/uwvm/wasm/feature/impl.h>
# include <uwvm2/uwvm/wasm/custom/impl.h>
#endif

#ifndef UWVM_MODULE_EXPORT
# define UWVM_MODULE_EXPORT
#endif

UWVM_MODULE_EXPORT namespace uwvm2::uwvm::wasm::type
{
#if (defined(_WIN32) || defined(__CYGWIN__)) && (!defined(__CYGWIN__) && !defined(__WINE__)) ||                                                                \
    ((!defined(_WIN32) || defined(__WINE__)) && (__has_include(<dlfcn.h>) && (defined(__CYGWIN__) || (!defined(__NEWLIB__) && !defined(__wasi__)))))

    extern "C"
    {
        struct capi_module_name_t
        {
            char const* name;
            ::std::size_t name_length;
        };

        /// @brief uwvm_get_module_name
        using capi_get_module_name_t = capi_module_name_t (*)();

        struct capi_custom_handler_t
        {
            char const* custom_name_ptr;
            ::std::size_t custom_name_length;
            ::uwvm2::uwvm::wasm::custom::imported_c_handlefunc_ptr_t custom_handle_func;
        };

        struct capi_custom_handler_vec_t
        {
            capi_custom_handler_t const* custom_handler_begin;
            ::std::size_t custom_handler_size;
        };

        /// @brief uwvm_get_custom_handler_vec
        using capi_get_custom_handler_vec_t = capi_custom_handler_vec_t (*)();

        using capi_wasm_function = void (*)(::std::byte* res, ::std::byte* para);

        /*
            capi_wasm_function example: (must use packed or aligned(1))

            ```c
            typedef struct __attribute__((packed)) para_d
            {
                uint_least32_t p0;
                uint_least64_t p1;
                _Float32 p2;
                _Float32 p3;
            } para;

            typedef struct __attribute__((packed)) res_d
            {
                uint_least32_t p0;
                uint_least32_t p1;
            } res;

            void test(res* r, para* p)
            {

            }
            ```
        */

        struct capi_function_t
        {
            char const* func_name_ptr;
            ::std::size_t func_name_length;

            ::std::uint_least8_t const* para_type_vec_begin;
            ::std::size_t para_type_vec_size;

            ::std::uint_least8_t const* res_type_vec_begin;
            ::std::size_t res_type_vec_size;

            capi_wasm_function func_ptr;
        };

        struct capi_function_vec_t
        {
            capi_function_t const* function_begin;
            ::std::size_t function_size;
        };

        /// @brief uwvm_get_function_vec_t
        using capi_get_function_vec_t = capi_function_vec_t (*)();
    }

    struct wasm_dl_storage_t
    {
        capi_get_module_name_t get_module_name{};
        capi_get_custom_handler_vec_t get_custom_handler_vec{};
        capi_get_function_vec_t get_function_vec{};

        capi_module_name_t capi_module_name{};
        capi_custom_handler_vec_t capi_custom_handler_vec{};
        capi_function_vec_t capi_function_vec{};
    };

    struct wasm_dl_t
    {
        // wasm file name
        ::fast_io::u8cstring_view file_name{};
        // Accurate module names that must work
        ::fast_io::u8string_view module_name{};
        // DL File
        ::fast_io::native_dll_file import_dll_file{};
        // DL handler
        wasm_dl_storage_t wasm_dl_storage{};
        // wasm_parameter_u
        ::uwvm2::uwvm::wasm::type::wasm_parameter_u wasm_parameter{};
    };

#endif
}  // namespace uwvm2::uwvm::wasm::storage
