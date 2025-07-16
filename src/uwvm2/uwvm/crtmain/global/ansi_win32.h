﻿/*************************************************************
 * Ultimate WebAssembly Virtual Machine (Version 2)          *
 * Copyright (c) 2025-present UlteSoft. All rights reserved. *
 * Licensed under the APL-2.0 License (see LICENSE file).    *
 *************************************************************/

/**
 * @author      MacroModel
 * @version     2.0.0
 * @date        2025-03-21
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

/// @brief      only support on winnt (with win32 api)
#if (defined(_WIN32) && !defined(__CYGWIN__)) && !defined(_WIN32_WINDOWS)

# ifdef UWVM_MODULE
import fast_io;
import uwvm2.uwvm.utils.ansies;
import uwvm2.uwvm.io;
# else
// std
#  include <cstdint>
#  include <memory>
// macro
#  include <uwvm2/utils/macro/push_macros.h>
#  include <uwvm2/uwvm/utils/ansies/uwvm_color_push_macro.h>
// import
#  include <fast_io.h>
#  include <uwvm2/uwvm/utils/ansies/impl.h>
#  include <uwvm2/uwvm/io/impl.h>
# endif

# ifndef UWVM_MODULE_EXPORT
#  define UWVM_MODULE_EXPORT
# endif

UWVM_MODULE_EXPORT namespace uwvm2::uwvm::global
{
    /// @brief      cmd on windows nt does not enable ansi escaping by default.
    /// @details    Declare this via a global variable to set up ansi escaping at program runtime.
    ///             No need to restore, the console is automatically restored at the end of the program
    struct enable_win32_ansi
    {
        inline static constexpr ::std::uint_least32_t enable_virtual_terminal_processing{0x0004u /*ENABLE_VIRTUAL_TERMINAL_PROCESSING*/};

        UWVM_GNU_COLD inline enable_win32_ansi() noexcept
        {
            set_out();
            set_err();
        }

        inline void set_out() noexcept
        {
            ::std::uint_least32_t out_omode;  // No initialization required

            void* out_handle{::fast_io::win32::GetStdHandle(::fast_io::win32_stdout_number)};
            if(out_handle == nullptr) [[unlikely]]
            {
                if(::uwvm2::uwvm::show_vm_warning)
                {
                    ::fast_io::io::perr(::uwvm2::uwvm::u8log_output,
                                        ::fast_io::mnp::cond(::uwvm2::uwvm::utils::ansies::put_color, UWVM_COLOR_U8_RST_ALL_AND_SET_WHITE),
                                        u8"uwvm: ",
                                        ::fast_io::mnp::cond(::uwvm2::uwvm::utils::ansies::put_color, UWVM_COLOR_U8_YELLOW),
                                        u8"[warn]  ",
                                        ::fast_io::mnp::cond(::uwvm2::uwvm::utils::ansies::put_color, UWVM_COLOR_U8_WHITE),
                                        u8"enable_virtual_terminal_processing: GetStdHandle stdout failed. ",
                                        ::fast_io::mnp::cond(::uwvm2::uwvm::utils::ansies::put_color, UWVM_COLOR_U8_ORANGE),
                                        u8"(vm)\n",
                                        ::fast_io::mnp::cond(::uwvm2::uwvm::utils::ansies::put_color, UWVM_COLOR_U8_RST_ALL));
                }
                return;
            }

            if(!::fast_io::win32::GetConsoleMode(out_handle, ::std::addressof(out_omode))) [[unlikely]]
            {
                if(::uwvm2::uwvm::show_vm_warning)
                {
                    ::fast_io::io::perr(::uwvm2::uwvm::u8log_output,
                                        ::fast_io::mnp::cond(::uwvm2::uwvm::utils::ansies::put_color, UWVM_COLOR_U8_RST_ALL_AND_SET_WHITE),
                                        u8"uwvm: ",
                                        ::fast_io::mnp::cond(::uwvm2::uwvm::utils::ansies::put_color, UWVM_COLOR_U8_YELLOW),
                                        u8"[warn]  ",
                                        ::fast_io::mnp::cond(::uwvm2::uwvm::utils::ansies::put_color, UWVM_COLOR_U8_WHITE),
                                        u8"enable_virtual_terminal_processing: GetConsoleMode stdout failed. ",
                                        ::fast_io::mnp::cond(::uwvm2::uwvm::utils::ansies::put_color, UWVM_COLOR_U8_ORANGE),
                                        u8"(vm)\n",
                                        ::fast_io::mnp::cond(::uwvm2::uwvm::utils::ansies::put_color, UWVM_COLOR_U8_RST_ALL));
                }
                return;
            }

            if(!::fast_io::win32::SetConsoleMode(out_handle, out_omode | enable_virtual_terminal_processing)) [[unlikely]]
            {
                if(::uwvm2::uwvm::show_vm_warning)
                {
                    ::fast_io::io::perr(::uwvm2::uwvm::u8log_output,
                                        ::fast_io::mnp::cond(::uwvm2::uwvm::utils::ansies::put_color, UWVM_COLOR_U8_RST_ALL_AND_SET_WHITE),
                                        u8"uwvm: ",
                                        ::fast_io::mnp::cond(::uwvm2::uwvm::utils::ansies::put_color, UWVM_COLOR_U8_YELLOW),
                                        u8"[warn]  ",
                                        ::fast_io::mnp::cond(::uwvm2::uwvm::utils::ansies::put_color, UWVM_COLOR_U8_WHITE),
                                        u8"enable_virtual_terminal_processing: SetConsoleMode stdout failed. ",
                                        ::fast_io::mnp::cond(::uwvm2::uwvm::utils::ansies::put_color, UWVM_COLOR_U8_ORANGE),
                                        u8"(vm)\n",
                                        ::fast_io::mnp::cond(::uwvm2::uwvm::utils::ansies::put_color, UWVM_COLOR_U8_RST_ALL));
                }
                return;
            }
        }

        inline void set_err() noexcept
        {
            ::std::uint_least32_t err_omode;  // No initialization required

            void* err_handle{::fast_io::win32::GetStdHandle(::fast_io::win32_stderr_number)};
            if(err_handle == nullptr) [[unlikely]]
            {
                if(::uwvm2::uwvm::show_vm_warning)
                {
                    ::fast_io::io::perr(::uwvm2::uwvm::u8log_output,
                                        ::fast_io::mnp::cond(::uwvm2::uwvm::utils::ansies::put_color, UWVM_COLOR_U8_RST_ALL_AND_SET_WHITE),
                                        u8"uwvm: ",
                                        ::fast_io::mnp::cond(::uwvm2::uwvm::utils::ansies::put_color, UWVM_COLOR_U8_YELLOW),
                                        u8"[warn]  ",
                                        ::fast_io::mnp::cond(::uwvm2::uwvm::utils::ansies::put_color, UWVM_COLOR_U8_WHITE),
                                        u8"enable_virtual_terminal_processing: GetStdHandle stderr failed. ",
                                        ::fast_io::mnp::cond(::uwvm2::uwvm::utils::ansies::put_color, UWVM_COLOR_U8_ORANGE),
                                        u8"(vm)\n",
                                        ::fast_io::mnp::cond(::uwvm2::uwvm::utils::ansies::put_color, UWVM_COLOR_U8_RST_ALL));
                }
                return;
            }

            if(!::fast_io::win32::GetConsoleMode(err_handle, ::std::addressof(err_omode))) [[unlikely]]
            {
                if(::uwvm2::uwvm::show_vm_warning)
                {
                    ::fast_io::io::perr(::uwvm2::uwvm::u8log_output,
                                        ::fast_io::mnp::cond(::uwvm2::uwvm::utils::ansies::put_color, UWVM_COLOR_U8_RST_ALL_AND_SET_WHITE),
                                        u8"uwvm: ",
                                        ::fast_io::mnp::cond(::uwvm2::uwvm::utils::ansies::put_color, UWVM_COLOR_U8_YELLOW),
                                        u8"[warn]  ",
                                        ::fast_io::mnp::cond(::uwvm2::uwvm::utils::ansies::put_color, UWVM_COLOR_U8_WHITE),
                                        u8"enable_virtual_terminal_processing: GetConsoleMode stderr failed. ",
                                        ::fast_io::mnp::cond(::uwvm2::uwvm::utils::ansies::put_color, UWVM_COLOR_U8_ORANGE),
                                        u8"(vm)\n",
                                        ::fast_io::mnp::cond(::uwvm2::uwvm::utils::ansies::put_color, UWVM_COLOR_U8_RST_ALL));
                }
                return;
            }

            if(!::fast_io::win32::SetConsoleMode(err_handle, err_omode | enable_virtual_terminal_processing)) [[unlikely]]
            {
                if(::uwvm2::uwvm::show_vm_warning)
                {
                    ::fast_io::io::perr(::uwvm2::uwvm::u8log_output,
                                        ::fast_io::mnp::cond(::uwvm2::uwvm::utils::ansies::put_color, UWVM_COLOR_U8_RST_ALL_AND_SET_WHITE),
                                        u8"uwvm: ",
                                        ::fast_io::mnp::cond(::uwvm2::uwvm::utils::ansies::put_color, UWVM_COLOR_U8_YELLOW),
                                        u8"[warn]  ",
                                        ::fast_io::mnp::cond(::uwvm2::uwvm::utils::ansies::put_color, UWVM_COLOR_U8_WHITE),
                                        u8"enable_virtual_terminal_processing: SetConsoleMode stderr failed. ",
                                        ::fast_io::mnp::cond(::uwvm2::uwvm::utils::ansies::put_color, UWVM_COLOR_U8_ORANGE),
                                        u8"(vm)\n",
                                        ::fast_io::mnp::cond(::uwvm2::uwvm::utils::ansies::put_color, UWVM_COLOR_U8_RST_ALL));
                }
                return;
            }
        }
    };
}  // namespace uwvm2::uwvm::global

# ifndef UWVM_MODULE
// macro
#  include <uwvm2/uwvm/utils/ansies/uwvm_color_pop_macro.h>
#  include <uwvm2/utils/macro/pop_macros.h>
# endif
#endif

