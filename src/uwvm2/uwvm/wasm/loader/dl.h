/*************************************************************
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
import uwvm2.uwvm.io;
import uwvm2.utils.ansies;
# if defined(UWVM_TIMER) || ((defined(_DEBUG) || defined(DEBUG)) && defined(UWVM_ENABLE_DETAILED_DEBUG_CHECK))
import uwvm2.utils.debug;
# endif
import uwvm2.utils.madvise;
import uwvm2.parser.wasm.base;
import uwvm2.parser.wasm.concepts;
import uwvm2.parser.wasm.standard;
import uwvm2.parser.wasm.binfmt.base;
import uwvm2.uwvm.utils.ansies;
import uwvm2.uwvm.utils.memory;
import uwvm2.uwvm.cmdline;
import uwvm2.uwvm.wasm.base;
import uwvm2.uwvm.wasm.type;
import uwvm2.uwvm.wasm.storage;
import uwvm2.uwvm.wasm.feature;
import uwvm2.uwvm.wasm.custom;
#else
// std
# include <cstddef>
# include <cstdint>
# include <type_traits>
# include <atomic>
// macro
# include <uwvm2/utils/macro/push_macros.h>
# include <uwvm2/uwvm/utils/ansies/uwvm_color_push_macro.h>
// import
# include <fast_io.h>
# include <uwvm2/uwvm/io/impl.h>
# include <uwvm2/utils/ansies/impl.h>
# if defined(UWVM_TIMER) || ((defined(_DEBUG) || defined(DEBUG)) && defined(UWVM_ENABLE_DETAILED_DEBUG_CHECK))
#  include <uwvm2/utils/debug/impl.h>
# endif
# include <uwvm2/utils/madvise/impl.h>
# include <uwvm2/parser/wasm/base/impl.h>
# include <uwvm2/parser/wasm/concepts/impl.h>
# include <uwvm2/parser/wasm/standard/impl.h>
# include <uwvm2/parser/wasm/binfmt/base/impl.h>
# include <uwvm2/uwvm/utils/ansies/impl.h>
# include <uwvm2/uwvm/utils/memory/impl.h>
# include <uwvm2/uwvm/cmdline/impl.h>
# include <uwvm2/uwvm/wasm/base/impl.h>
# include <uwvm2/uwvm/wasm/type/impl.h>
# include <uwvm2/uwvm/wasm/storage/impl.h>
# include <uwvm2/uwvm/wasm/feature/impl.h>
# include <uwvm2/uwvm/wasm/custom/impl.h>
#endif

namespace uwvm2::uwvm::wasm::loader
{
#if (defined(_WIN32) || defined(__CYGWIN__)) && (!defined(__CYGWIN__) && !defined(__WINE__)) ||                                                                \
    ((!defined(_WIN32) || defined(__WINE__)) && (__has_include(<dlfcn.h>) && (defined(__CYGWIN__) || (!defined(__NEWLIB__) && !defined(__wasi__)))))

    enum class load_dl_rtl
    {
        ok,
        load_error,
        parse_error
    };

    /// @todo load dl not finished
    inline constexpr load_dl_rtl load_dl(::uwvm2::uwvm::wasm::type::wasm_dl_t& wd,
                                         ::fast_io::u8cstring_view load_file_name,
                                         ::fast_io::u8string_view rename_module_name,
                                         ::uwvm2::uwvm::wasm::type::wasm_parameter_u para) noexcept
    {
        if(::uwvm2::uwvm::show_untrusted_dl_warning)
        {
            // show warning
            static ::std::atomic<bool> warned{};  // [global]

            bool const already_warned{warned.exchange(true, ::std::memory_order_relaxed)};
            if(!already_warned) [[unlikely]]
            {
                ::fast_io::io::perr(::uwvm2::uwvm::u8log_output,
                                    // 1
                                    ::fast_io::mnp::cond(::uwvm2::uwvm::utils::ansies::put_color, UWVM_COLOR_U8_RST_ALL_AND_SET_WHITE),
                                    u8"uwvm: ",
                                    ::fast_io::mnp::cond(::uwvm2::uwvm::utils::ansies::put_color, UWVM_COLOR_U8_YELLOW),
                                    u8"[warn]  ",
                                    ::fast_io::mnp::cond(::uwvm2::uwvm::utils::ansies::put_color, UWVM_COLOR_U8_WHITE),
                                    u8"Importing an untrusted dl may cause security issues, please make sure the dl is trusted. ",
                                    ::fast_io::mnp::cond(::uwvm2::uwvm::utils::ansies::put_color, UWVM_COLOR_U8_ORANGE),
                                    u8"(untrusted-dl)\n",
                                    ::fast_io::mnp::cond(::uwvm2::uwvm::utils::ansies::put_color, UWVM_COLOR_U8_RST_ALL));
            }
        }

        wd.file_name = load_file_name;

        wd.wasm_parameter = para;

        // verbose
        if(::uwvm2::uwvm::show_verbose) [[unlikely]]
        {
            ::fast_io::io::perr(::uwvm2::uwvm::u8log_output,
                                ::fast_io::mnp::cond(::uwvm2::uwvm::utils::ansies::put_color, UWVM_COLOR_U8_RST_ALL_AND_SET_WHITE),
                                u8"uwvm: ",
                                ::fast_io::mnp::cond(::uwvm2::uwvm::utils::ansies::put_color, UWVM_COLOR_U8_LT_GREEN),
                                u8"[info]  ",
                                ::fast_io::mnp::cond(::uwvm2::uwvm::utils::ansies::put_color, UWVM_COLOR_U8_WHITE),
                                u8"Loading DL File \"",
                                ::fast_io::mnp::cond(::uwvm2::uwvm::utils::ansies::put_color, UWVM_COLOR_U8_YELLOW),
                                load_file_name,
                                ::fast_io::mnp::cond(::uwvm2::uwvm::utils::ansies::put_color, UWVM_COLOR_U8_WHITE),
                                u8"\". ",
                                ::fast_io::mnp::cond(::uwvm2::uwvm::utils::ansies::put_color, UWVM_COLOR_U8_ORANGE),
                                u8"(verbose)\n",
                                ::fast_io::mnp::cond(::uwvm2::uwvm::utils::ansies::put_color, UWVM_COLOR_U8_RST_ALL));
        }

# ifdef __cpp_exceptions
        try
# endif
        {
# ifdef UWVM_TIMER
            ::uwvm2::utils::debug::timer parsing_timer{u8"dl loader"};
# endif

            wd.import_dll_file = ::fast_io::native_dll_file{load_file_name, ::fast_io::dll_mode::posix_rtld_lazy};
        }
# ifdef __cpp_exceptions
        catch(::fast_io::error e)
        {
            ::fast_io::io::perr(::uwvm2::uwvm::u8log_output,
                                ::fast_io::mnp::cond(::uwvm2::uwvm::utils::ansies::put_color, UWVM_COLOR_U8_RST_ALL_AND_SET_WHITE),
                                u8"uwvm: ",
                                ::fast_io::mnp::cond(::uwvm2::uwvm::utils::ansies::put_color, UWVM_COLOR_U8_RED),
                                u8"[error] ",
                                ::fast_io::mnp::cond(::uwvm2::uwvm::utils::ansies::put_color, UWVM_COLOR_U8_WHITE),
                                u8"Unable to open DL file \"",
                                ::fast_io::mnp::cond(::uwvm2::uwvm::utils::ansies::put_color, UWVM_COLOR_U8_CYAN),
                                load_file_name,
                                ::fast_io::mnp::cond(::uwvm2::uwvm::utils::ansies::put_color, UWVM_COLOR_U8_WHITE),
                                u8"\": ",
                                e,
                                ::fast_io::mnp::cond(::uwvm2::uwvm::utils::ansies::put_color, UWVM_COLOR_U8_RST_ALL),
                                u8"\n"
#  ifndef _WIN32  // Win32 automatically adds a newline (winnt and win9x)
                                u8"\n"
#  endif
            );

            return load_dl_rtl::load_error;
        }
# endif

        using char8_t_const_may_alias_ptr UWVM_GNU_MAY_ALIAS = char8_t const*;

        {
            bool can_get_module_name{};

# ifdef __cpp_exceptions
            try
# endif
            {
                wd.wasm_dl_storage.get_module_name = reinterpret_cast<::uwvm2::uwvm::wasm::type::capi_get_module_name_t>(
                    ::fast_io::dll_load_symbol(wd.import_dll_file, u8"uwvm_get_module_name"));
                can_get_module_name = true;
            }
# ifdef __cpp_exceptions
            catch(::fast_io::error)
            {
            }
# endif

            if(can_get_module_name)
            {
                if(::uwvm2::uwvm::show_verbose) [[unlikely]]
                {
                    ::fast_io::io::perr(::uwvm2::uwvm::u8log_output,
                                        ::fast_io::mnp::cond(::uwvm2::uwvm::utils::ansies::put_color, UWVM_COLOR_U8_RST_ALL_AND_SET_WHITE),
                                        u8"uwvm: ",
                                        ::fast_io::mnp::cond(::uwvm2::uwvm::utils::ansies::put_color, UWVM_COLOR_U8_LT_GREEN),
                                        u8"[info]  ",
                                        ::fast_io::mnp::cond(::uwvm2::uwvm::utils::ansies::put_color, UWVM_COLOR_U8_WHITE),
                                        u8"Successful acquisition of symbols \"",
                                        ::fast_io::mnp::cond(::uwvm2::uwvm::utils::ansies::put_color, UWVM_COLOR_U8_YELLOW),
                                        u8"uwvm_get_module_name",
                                        ::fast_io::mnp::cond(::uwvm2::uwvm::utils::ansies::put_color, UWVM_COLOR_U8_WHITE),
                                        u8"\". ",
                                        ::fast_io::mnp::cond(::uwvm2::uwvm::utils::ansies::put_color, UWVM_COLOR_U8_ORANGE),
                                        u8"(verbose)\n",
                                        ::fast_io::mnp::cond(::uwvm2::uwvm::utils::ansies::put_color, UWVM_COLOR_U8_RST_ALL));
                }

                wd.wasm_dl_storage.capi_module_name = (wd.wasm_dl_storage.get_module_name)();
            }

            // 1st: para --wasm-set-main-module-name
            // 2st: custom section "name": module name
            // 3st: file path

            if(rename_module_name.empty())
            {
                if(wd.wasm_dl_storage.capi_module_name.name_length)
                {
                    wd.module_name = ::fast_io::u8string_view{reinterpret_cast<char8_t_const_may_alias_ptr>(wd.wasm_dl_storage.capi_module_name.name),
                                                              wd.wasm_dl_storage.capi_module_name.name_length};
                }
                else
                {
                    wd.module_name = ::fast_io::u8string_view{load_file_name};
                }
            }
            else
            {
                wd.module_name = rename_module_name;
            }

            if(wd.module_name.empty()) [[unlikely]]
            {
# ifndef UWVM_DISABLE_OUTPUT_WHEN_PARSE
                // Output the main information and memory indication
                ::fast_io::io::perr(::uwvm2::uwvm::u8log_output,
                                    // 1
                                    ::fast_io::mnp::cond(::uwvm2::uwvm::utils::ansies::put_color, UWVM_COLOR_U8_RST_ALL_AND_SET_WHITE),
                                    u8"uwvm: ",
                                    ::fast_io::mnp::cond(::uwvm2::uwvm::utils::ansies::put_color, UWVM_COLOR_U8_RED),
                                    u8"[error] ",
                                    ::fast_io::mnp::cond(::uwvm2::uwvm::utils::ansies::put_color, UWVM_COLOR_U8_WHITE),
                                    u8"Parsing error in DL \"",
                                    ::fast_io::mnp::cond(::uwvm2::uwvm::utils::ansies::put_color, UWVM_COLOR_U8_YELLOW),
                                    load_file_name,
                                    ::fast_io::mnp::cond(::uwvm2::uwvm::utils::ansies::put_color, UWVM_COLOR_U8_WHITE),
                                    u8"\".\n"
                                    // 2
                                    u8"uwvm: ",
                                    ::fast_io::mnp::cond(::uwvm2::uwvm::utils::ansies::put_color, UWVM_COLOR_U8_RED),
                                    u8"[error] ",
                                    ::fast_io::mnp::cond(::uwvm2::uwvm::utils::ansies::put_color, UWVM_COLOR_U8_WHITE),
                                    u8"The Overall WebAssembly Module Name Length Cannot Be 0.\n\n",
                                    ::fast_io::mnp::cond(::uwvm2::uwvm::utils::ansies::put_color, UWVM_COLOR_U8_RST_ALL));
# endif
            }

            // check module_name is utf8
            auto const [utf8pos, utf8err]{::uwvm2::uwvm::wasm::feature::handle_text_format(::uwvm2::uwvm::wasm::feature::wasm_binfmt_ver1_text_format_wapper,
                                                                                           wd.module_name.cbegin(),
                                                                                           wd.module_name.cend())};

            if(utf8err != ::uwvm2::utils::utf::utf_error_code::success) [[unlikely]]
            {
# ifndef UWVM_DISABLE_OUTPUT_WHEN_PARSE

                // default print_memory
                ::uwvm2::uwvm::utils::memory::print_memory const memory_printer{reinterpret_cast<::std::byte const*>(wd.module_name.cbegin()),
                                                                                reinterpret_cast<::std::byte const*>(utf8pos),
                                                                                reinterpret_cast<::std::byte const*>(wd.module_name.cend())};

                // Output the main information and memory indication
                ::fast_io::io::perr(::uwvm2::uwvm::u8log_output,
                                    // 1
                                    ::fast_io::mnp::cond(::uwvm2::uwvm::utils::ansies::put_color, UWVM_COLOR_U8_RST_ALL_AND_SET_WHITE),
                                    u8"uwvm: ",
                                    ::fast_io::mnp::cond(::uwvm2::uwvm::utils::ansies::put_color, UWVM_COLOR_U8_RED),
                                    u8"[error] ",
                                    ::fast_io::mnp::cond(::uwvm2::uwvm::utils::ansies::put_color, UWVM_COLOR_U8_WHITE),
                                    u8"Parsing error in DL \"",
                                    ::fast_io::mnp::cond(::uwvm2::uwvm::utils::ansies::put_color, UWVM_COLOR_U8_YELLOW),
                                    load_file_name,
                                    ::fast_io::mnp::cond(::uwvm2::uwvm::utils::ansies::put_color, UWVM_COLOR_U8_WHITE),
                                    u8"\".\n"
                                    // 2
                                    u8"uwvm: ",
                                    ::fast_io::mnp::cond(::uwvm2::uwvm::utils::ansies::put_color, UWVM_COLOR_U8_RED),
                                    u8"[error] ",
                                    ::fast_io::mnp::cond(::uwvm2::uwvm::utils::ansies::put_color, UWVM_COLOR_U8_WHITE),
                                    u8"(offset=",
                                    ::fast_io::mnp::addrvw(utf8pos - wd.module_name.cbegin()),
                                    u8") Module Name Is Invalid UTF-8 Sequence. Reason: \"",
                                    ::fast_io::mnp::cond(::uwvm2::uwvm::utils::ansies::put_color, UWVM_COLOR_U8_YELLOW),
                                    ::uwvm2::utils::utf::get_utf_error_description<char8_t>(utf8err),
                                    ::fast_io::mnp::cond(::uwvm2::uwvm::utils::ansies::put_color, UWVM_COLOR_U8_WHITE),
                                    u8"\".\n"
                                    // 3
                                    u8"uwvm: ",
                                    ::fast_io::mnp::cond(::uwvm2::uwvm::utils::ansies::put_color, UWVM_COLOR_U8_LT_GREEN),
                                    u8"[info]  ",
                                    ::fast_io::mnp::cond(::uwvm2::uwvm::utils::ansies::put_color, UWVM_COLOR_U8_WHITE),
                                    u8"Parser Memory Indication: ",
                                    memory_printer,
                                    ::fast_io::mnp::cond(::uwvm2::uwvm::utils::ansies::put_color, UWVM_COLOR_U8_RST_ALL),
                                    u8"\n\n");
# endif

                return load_dl_rtl::parse_error;
            }

            // verbose
            if(::uwvm2::uwvm::show_verbose) [[unlikely]]
            {
                ::fast_io::io::perr(::uwvm2::uwvm::u8log_output,
                                    ::fast_io::mnp::cond(::uwvm2::uwvm::utils::ansies::put_color, UWVM_COLOR_U8_RST_ALL_AND_SET_WHITE),
                                    u8"uwvm: ",
                                    ::fast_io::mnp::cond(::uwvm2::uwvm::utils::ansies::put_color, UWVM_COLOR_U8_LT_GREEN),
                                    u8"[info]  ",
                                    ::fast_io::mnp::cond(::uwvm2::uwvm::utils::ansies::put_color, UWVM_COLOR_U8_WHITE),
                                    u8"Overall DL's WebAssembly Module Name \"",
                                    ::fast_io::mnp::cond(::uwvm2::uwvm::utils::ansies::put_color, UWVM_COLOR_U8_YELLOW),
                                    wd.module_name,
                                    ::fast_io::mnp::cond(::uwvm2::uwvm::utils::ansies::put_color, UWVM_COLOR_U8_WHITE),
                                    u8"\". ",
                                    ::fast_io::mnp::cond(::uwvm2::uwvm::utils::ansies::put_color, UWVM_COLOR_U8_ORANGE),
                                    u8"(verbose)\n",
                                    ::fast_io::mnp::cond(::uwvm2::uwvm::utils::ansies::put_color, UWVM_COLOR_U8_RST_ALL));
            }
        }

        {
            bool can_get_custom_handler_vec{};

# ifdef __cpp_exceptions
            try
# endif
            {
                wd.wasm_dl_storage.get_custom_handler_vec = reinterpret_cast<::uwvm2::uwvm::wasm::type::capi_get_custom_handler_vec_t>(
                    ::fast_io::dll_load_symbol(wd.import_dll_file, u8"uwvm_get_custom_handler"));
                can_get_custom_handler_vec = true;
            }
# ifdef __cpp_exceptions
            catch(::fast_io::error)
            {
            }
# endif

            if(can_get_custom_handler_vec)
            {
                if(::uwvm2::uwvm::show_verbose) [[unlikely]]
                {
                    ::fast_io::io::perr(::uwvm2::uwvm::u8log_output,
                                        ::fast_io::mnp::cond(::uwvm2::uwvm::utils::ansies::put_color, UWVM_COLOR_U8_RST_ALL_AND_SET_WHITE),
                                        u8"uwvm: ",
                                        ::fast_io::mnp::cond(::uwvm2::uwvm::utils::ansies::put_color, UWVM_COLOR_U8_LT_GREEN),
                                        u8"[info]  ",
                                        ::fast_io::mnp::cond(::uwvm2::uwvm::utils::ansies::put_color, UWVM_COLOR_U8_WHITE),
                                        u8"Successful acquisition of symbols \"",
                                        ::fast_io::mnp::cond(::uwvm2::uwvm::utils::ansies::put_color, UWVM_COLOR_U8_YELLOW),
                                        u8"uwvm_get_custom_handler",
                                        ::fast_io::mnp::cond(::uwvm2::uwvm::utils::ansies::put_color, UWVM_COLOR_U8_WHITE),
                                        u8"\". ",
                                        ::fast_io::mnp::cond(::uwvm2::uwvm::utils::ansies::put_color, UWVM_COLOR_U8_ORANGE),
                                        u8"(verbose)\n",
                                        ::fast_io::mnp::cond(::uwvm2::uwvm::utils::ansies::put_color, UWVM_COLOR_U8_RST_ALL));
                }

                wd.wasm_dl_storage.capi_custom_handler_vec = (wd.wasm_dl_storage.get_custom_handler_vec)();

                auto const handler_begin{wd.wasm_dl_storage.capi_custom_handler_vec.custom_handler_begin};
                auto const handler_end{handler_begin + wd.wasm_dl_storage.capi_custom_handler_vec.custom_handler_size};

                for(auto handler_curr{handler_begin}; handler_curr != handler_end; ++handler_curr)
                {
                    // check utf-8
                    ::fast_io::u8string_view const custom_name{reinterpret_cast<char8_t_const_may_alias_ptr>(handler_curr->custom_name_ptr),
                                                               handler_curr->custom_name_length};

                    if(custom_name.empty()) [[unlikely]]
                    {
# ifndef UWVM_DISABLE_OUTPUT_WHEN_PARSE
                        if(::uwvm2::uwvm::show_dl_warning)
                        {
                            // Output the main information and memory indication
                            ::fast_io::io::perr(::uwvm2::uwvm::u8log_output,
                                                // 1
                                                ::fast_io::mnp::cond(::uwvm2::uwvm::utils::ansies::put_color, UWVM_COLOR_U8_RST_ALL_AND_SET_WHITE),
                                                u8"uwvm: ",
                                                ::fast_io::mnp::cond(::uwvm2::uwvm::utils::ansies::put_color, UWVM_COLOR_U8_RED),
                                                u8"[warn] ",
                                                ::fast_io::mnp::cond(::uwvm2::uwvm::utils::ansies::put_color, UWVM_COLOR_U8_WHITE),
                                                u8"Parsing error in DL \"",
                                                ::fast_io::mnp::cond(::uwvm2::uwvm::utils::ansies::put_color, UWVM_COLOR_U8_YELLOW),
                                                load_file_name,
                                                ::fast_io::mnp::cond(::uwvm2::uwvm::utils::ansies::put_color, UWVM_COLOR_U8_WHITE),
                                                u8"\".",
                                                ::fast_io::mnp::cond(::uwvm2::uwvm::utils::ansies::put_color, UWVM_COLOR_U8_ORANGE),
                                                u8" (dl)\n",
                                                ::fast_io::mnp::cond(::uwvm2::uwvm::utils::ansies::put_color, UWVM_COLOR_U8_WHITE),
                                                // 2
                                                u8"uwvm: ",
                                                ::fast_io::mnp::cond(::uwvm2::uwvm::utils::ansies::put_color, UWVM_COLOR_U8_RED),
                                                u8"[warn] ",
                                                ::fast_io::mnp::cond(::uwvm2::uwvm::utils::ansies::put_color, UWVM_COLOR_U8_WHITE),
                                                u8"The Custom Name Length Cannot Be 0.\n",
                                                ::fast_io::mnp::cond(::uwvm2::uwvm::utils::ansies::put_color, UWVM_COLOR_U8_RST_ALL));
                        }
# endif
                    }

                    // check custom_name is utf8
                    auto const [utf8pos,
                                utf8err]{::uwvm2::uwvm::wasm::feature::handle_text_format(::uwvm2::uwvm::wasm::feature::wasm_binfmt_ver1_text_format_wapper,
                                                                                          custom_name.cbegin(),
                                                                                          custom_name.cend())};

                    if(utf8err != ::uwvm2::utils::utf::utf_error_code::success) [[unlikely]]
                    {
# ifndef UWVM_DISABLE_OUTPUT_WHEN_PARSE
                        if(::uwvm2::uwvm::show_dl_warning)
                        {
                            // default print_memory
                            ::uwvm2::uwvm::utils::memory::print_memory const memory_printer{reinterpret_cast<::std::byte const*>(custom_name.cbegin()),
                                                                                            reinterpret_cast<::std::byte const*>(utf8pos),
                                                                                            reinterpret_cast<::std::byte const*>(custom_name.cend())};

                            // Output the main information and memory indication
                            ::fast_io::io::perr(::uwvm2::uwvm::u8log_output,
                                                // 1
                                                ::fast_io::mnp::cond(::uwvm2::uwvm::utils::ansies::put_color, UWVM_COLOR_U8_RST_ALL_AND_SET_WHITE),
                                                u8"uwvm: ",
                                                ::fast_io::mnp::cond(::uwvm2::uwvm::utils::ansies::put_color, UWVM_COLOR_U8_YELLOW),
                                                u8"[warn]  ",
                                                ::fast_io::mnp::cond(::uwvm2::uwvm::utils::ansies::put_color, UWVM_COLOR_U8_WHITE),
                                                u8"Parsing error in DL \"",
                                                ::fast_io::mnp::cond(::uwvm2::uwvm::utils::ansies::put_color, UWVM_COLOR_U8_YELLOW),
                                                load_file_name,
                                                ::fast_io::mnp::cond(::uwvm2::uwvm::utils::ansies::put_color, UWVM_COLOR_U8_WHITE),
                                                u8"\".",
                                                ::fast_io::mnp::cond(::uwvm2::uwvm::utils::ansies::put_color, UWVM_COLOR_U8_ORANGE),
                                                u8" (dl)\n",
                                                ::fast_io::mnp::cond(::uwvm2::uwvm::utils::ansies::put_color, UWVM_COLOR_U8_WHITE),
                                                // 2
                                                u8"uwvm: ",
                                                ::fast_io::mnp::cond(::uwvm2::uwvm::utils::ansies::put_color, UWVM_COLOR_U8_YELLOW),
                                                u8"[warn]  ",
                                                ::fast_io::mnp::cond(::uwvm2::uwvm::utils::ansies::put_color, UWVM_COLOR_U8_WHITE),
                                                u8"(offset=",
                                                ::fast_io::mnp::addrvw(utf8pos - custom_name.cbegin()),
                                                u8") Custom Name Is Invalid UTF-8 Sequence. Reason: \"",
                                                ::fast_io::mnp::cond(::uwvm2::uwvm::utils::ansies::put_color, UWVM_COLOR_U8_YELLOW),
                                                ::uwvm2::utils::utf::get_utf_error_description<char8_t>(utf8err),
                                                ::fast_io::mnp::cond(::uwvm2::uwvm::utils::ansies::put_color, UWVM_COLOR_U8_WHITE),
                                                u8"\".\n"
                                                // 3
                                                u8"uwvm: ",
                                                ::fast_io::mnp::cond(::uwvm2::uwvm::utils::ansies::put_color, UWVM_COLOR_U8_LT_GREEN),
                                                u8"[info]  ",
                                                ::fast_io::mnp::cond(::uwvm2::uwvm::utils::ansies::put_color, UWVM_COLOR_U8_WHITE),
                                                u8"Parser Memory Indication: ",
                                                memory_printer,
                                                ::fast_io::mnp::cond(::uwvm2::uwvm::utils::ansies::put_color, UWVM_COLOR_U8_RST_ALL),
                                                u8"\n");
                        }
# endif

                        // Just skip here, no need to disable
                        continue;
                    }

                    if(::uwvm2::uwvm::wasm::custom::custom_handle_funcs.contains(custom_name)) [[unlikely]]
                    {
# ifndef UWVM_DISABLE_OUTPUT_WHEN_PARSE
                        if(::uwvm2::uwvm::show_dl_warning)
                        {
                            // Output the main information and memory indication
                            ::fast_io::io::perr(::uwvm2::uwvm::u8log_output,
                                                // 1
                                                ::fast_io::mnp::cond(::uwvm2::uwvm::utils::ansies::put_color, UWVM_COLOR_U8_RST_ALL_AND_SET_WHITE),
                                                u8"uwvm: ",
                                                ::fast_io::mnp::cond(::uwvm2::uwvm::utils::ansies::put_color, UWVM_COLOR_U8_YELLOW),
                                                u8"[warn]  ",
                                                ::fast_io::mnp::cond(::uwvm2::uwvm::utils::ansies::put_color, UWVM_COLOR_U8_WHITE),
                                                u8"Parsing error in DL \"",
                                                ::fast_io::mnp::cond(::uwvm2::uwvm::utils::ansies::put_color, UWVM_COLOR_U8_YELLOW),
                                                load_file_name,
                                                ::fast_io::mnp::cond(::uwvm2::uwvm::utils::ansies::put_color, UWVM_COLOR_U8_WHITE),
                                                u8"\".",
                                                ::fast_io::mnp::cond(::uwvm2::uwvm::utils::ansies::put_color, UWVM_COLOR_U8_ORANGE),
                                                u8" (dl)\n",
                                                ::fast_io::mnp::cond(::uwvm2::uwvm::utils::ansies::put_color, UWVM_COLOR_U8_WHITE),
                                                // 2
                                                u8"uwvm: ",
                                                ::fast_io::mnp::cond(::uwvm2::uwvm::utils::ansies::put_color, UWVM_COLOR_U8_YELLOW),
                                                u8"[warn]  ",
                                                ::fast_io::mnp::cond(::uwvm2::uwvm::utils::ansies::put_color, UWVM_COLOR_U8_WHITE),
                                                u8"Duplicate custom section handler: \"",
                                                ::fast_io::mnp::cond(::uwvm2::uwvm::utils::ansies::put_color, UWVM_COLOR_U8_YELLOW),
                                                custom_name,
                                                ::fast_io::mnp::cond(::uwvm2::uwvm::utils::ansies::put_color, UWVM_COLOR_U8_WHITE),
                                                u8"\".\n",
                                                ::fast_io::mnp::cond(::uwvm2::uwvm::utils::ansies::put_color, UWVM_COLOR_U8_RST_ALL));
                        }
# endif

                        // Just skip here, no need to disable
                        continue;
                    }

                    ::uwvm2::uwvm::wasm::custom::custom_handle_funcs.emplace(
                        custom_name,
                        ::uwvm2::uwvm::wasm::custom::handlefunc_t{reinterpret_cast<void*>(handler_curr->custom_handle_func), true});
                }
            }
        }

        {
            bool can_get_function_vec{};

# ifdef __cpp_exceptions
            try
# endif
            {
                wd.wasm_dl_storage.get_function_vec =
                    reinterpret_cast<::uwvm2::uwvm::wasm::type::capi_get_function_vec_t>(::fast_io::dll_load_symbol(wd.import_dll_file, u8"uwvm_function"));
                can_get_function_vec = true;
            }
# ifdef __cpp_exceptions
            catch(::fast_io::error)
            {
            }
# endif

            if(can_get_function_vec)
            {
                if(::uwvm2::uwvm::show_verbose) [[unlikely]]
                {
                    ::fast_io::io::perr(::uwvm2::uwvm::u8log_output,
                                        ::fast_io::mnp::cond(::uwvm2::uwvm::utils::ansies::put_color, UWVM_COLOR_U8_RST_ALL_AND_SET_WHITE),
                                        u8"uwvm: ",
                                        ::fast_io::mnp::cond(::uwvm2::uwvm::utils::ansies::put_color, UWVM_COLOR_U8_LT_GREEN),
                                        u8"[info]  ",
                                        ::fast_io::mnp::cond(::uwvm2::uwvm::utils::ansies::put_color, UWVM_COLOR_U8_WHITE),
                                        u8"Successful acquisition of symbols \"",
                                        ::fast_io::mnp::cond(::uwvm2::uwvm::utils::ansies::put_color, UWVM_COLOR_U8_YELLOW),
                                        u8"uwvm_function",
                                        ::fast_io::mnp::cond(::uwvm2::uwvm::utils::ansies::put_color, UWVM_COLOR_U8_WHITE),
                                        u8"\". ",
                                        ::fast_io::mnp::cond(::uwvm2::uwvm::utils::ansies::put_color, UWVM_COLOR_U8_ORANGE),
                                        u8"(verbose)\n",
                                        ::fast_io::mnp::cond(::uwvm2::uwvm::utils::ansies::put_color, UWVM_COLOR_U8_RST_ALL));
                }

                wd.wasm_dl_storage.capi_function_vec = (wd.wasm_dl_storage.get_function_vec)();

                auto const func_begin{wd.wasm_dl_storage.capi_function_vec.function_begin};
                auto const func_end{func_begin + wd.wasm_dl_storage.capi_function_vec.function_size};

                for(auto func_curr{func_begin}; func_curr != func_end; ++func_curr)
                {
                    ::fast_io::u8string_view const func_name{reinterpret_cast<char8_t_const_may_alias_ptr>(func_curr->func_name_ptr),
                                                             func_curr->func_name_length};

                    if(func_name.empty()) [[unlikely]]
                    {
# ifndef UWVM_DISABLE_OUTPUT_WHEN_PARSE
                        if(::uwvm2::uwvm::show_dl_warning)
                        {
                            // Output the main information and memory indication
                            ::fast_io::io::perr(::uwvm2::uwvm::u8log_output,
                                                // 1
                                                ::fast_io::mnp::cond(::uwvm2::uwvm::utils::ansies::put_color, UWVM_COLOR_U8_RST_ALL_AND_SET_WHITE),
                                                u8"uwvm: ",
                                                ::fast_io::mnp::cond(::uwvm2::uwvm::utils::ansies::put_color, UWVM_COLOR_U8_RED),
                                                u8"[error] ",
                                                ::fast_io::mnp::cond(::uwvm2::uwvm::utils::ansies::put_color, UWVM_COLOR_U8_WHITE),
                                                u8"Parsing error in DL \"",
                                                ::fast_io::mnp::cond(::uwvm2::uwvm::utils::ansies::put_color, UWVM_COLOR_U8_YELLOW),
                                                load_file_name,
                                                ::fast_io::mnp::cond(::uwvm2::uwvm::utils::ansies::put_color, UWVM_COLOR_U8_WHITE),
                                                u8"\".",
                                                ::fast_io::mnp::cond(::uwvm2::uwvm::utils::ansies::put_color, UWVM_COLOR_U8_ORANGE),
                                                u8" (dl)\n",
                                                ::fast_io::mnp::cond(::uwvm2::uwvm::utils::ansies::put_color, UWVM_COLOR_U8_WHITE),
                                                // 2
                                                u8"uwvm: ",
                                                ::fast_io::mnp::cond(::uwvm2::uwvm::utils::ansies::put_color, UWVM_COLOR_U8_RED),
                                                u8"[error] ",
                                                ::fast_io::mnp::cond(::uwvm2::uwvm::utils::ansies::put_color, UWVM_COLOR_U8_WHITE),
                                                u8"The Function Name Length Cannot Be 0.\n\n",
                                                ::fast_io::mnp::cond(::uwvm2::uwvm::utils::ansies::put_color, UWVM_COLOR_U8_RST_ALL));
                        }
# endif
                    }

                    // check custom_name is utf8
                    auto const [utf8pos,
                                utf8err]{::uwvm2::uwvm::wasm::feature::handle_text_format(::uwvm2::uwvm::wasm::feature::wasm_binfmt_ver1_text_format_wapper,
                                                                                          func_name.cbegin(),
                                                                                          func_name.cend())};

                    if(utf8err != ::uwvm2::utils::utf::utf_error_code::success) [[unlikely]]
                    {
# ifndef UWVM_DISABLE_OUTPUT_WHEN_PARSE
                        if(::uwvm2::uwvm::show_dl_warning)
                        {
                            // default print_memory
                            ::uwvm2::uwvm::utils::memory::print_memory const memory_printer{reinterpret_cast<::std::byte const*>(func_name.cbegin()),
                                                                                            reinterpret_cast<::std::byte const*>(utf8pos),
                                                                                            reinterpret_cast<::std::byte const*>(func_name.cend())};

                            // Output the main information and memory indication
                            ::fast_io::io::perr(::uwvm2::uwvm::u8log_output,
                                                // 1
                                                ::fast_io::mnp::cond(::uwvm2::uwvm::utils::ansies::put_color, UWVM_COLOR_U8_RST_ALL_AND_SET_WHITE),
                                                u8"uwvm: ",
                                                ::fast_io::mnp::cond(::uwvm2::uwvm::utils::ansies::put_color, UWVM_COLOR_U8_YELLOW),
                                                u8"[warn]  ",
                                                ::fast_io::mnp::cond(::uwvm2::uwvm::utils::ansies::put_color, UWVM_COLOR_U8_WHITE),
                                                u8"Parsing error in DL \"",
                                                ::fast_io::mnp::cond(::uwvm2::uwvm::utils::ansies::put_color, UWVM_COLOR_U8_YELLOW),
                                                load_file_name,
                                                ::fast_io::mnp::cond(::uwvm2::uwvm::utils::ansies::put_color, UWVM_COLOR_U8_WHITE),
                                                u8"\".",
                                                ::fast_io::mnp::cond(::uwvm2::uwvm::utils::ansies::put_color, UWVM_COLOR_U8_ORANGE),
                                                u8" (dl)\n",
                                                ::fast_io::mnp::cond(::uwvm2::uwvm::utils::ansies::put_color, UWVM_COLOR_U8_WHITE),
                                                // 2
                                                u8"uwvm: ",
                                                ::fast_io::mnp::cond(::uwvm2::uwvm::utils::ansies::put_color, UWVM_COLOR_U8_YELLOW),
                                                u8"[warn]  ",
                                                ::fast_io::mnp::cond(::uwvm2::uwvm::utils::ansies::put_color, UWVM_COLOR_U8_WHITE),
                                                u8"(offset=",
                                                ::fast_io::mnp::addrvw(utf8pos - func_name.cbegin()),
                                                u8") Function Name Is Invalid UTF-8 Sequence. Reason: \"",
                                                ::fast_io::mnp::cond(::uwvm2::uwvm::utils::ansies::put_color, UWVM_COLOR_U8_YELLOW),
                                                ::uwvm2::utils::utf::get_utf_error_description<char8_t>(utf8err),
                                                ::fast_io::mnp::cond(::uwvm2::uwvm::utils::ansies::put_color, UWVM_COLOR_U8_WHITE),
                                                u8"\".\n"
                                                // 3
                                                u8"uwvm: ",
                                                ::fast_io::mnp::cond(::uwvm2::uwvm::utils::ansies::put_color, UWVM_COLOR_U8_LT_GREEN),
                                                u8"[info]  ",
                                                ::fast_io::mnp::cond(::uwvm2::uwvm::utils::ansies::put_color, UWVM_COLOR_U8_WHITE),
                                                u8"Parser Memory Indication: ",
                                                memory_printer,
                                                ::fast_io::mnp::cond(::uwvm2::uwvm::utils::ansies::put_color, UWVM_COLOR_U8_RST_ALL),
                                                u8"\n");
                        }
# endif

                        // Just skip here, no need to disable
                        continue;
                    }

                    // Parameter checking needs to be done at compile time

                    if(func_curr->func_ptr == nullptr) [[unlikely]]
                    {
# ifndef UWVM_DISABLE_OUTPUT_WHEN_PARSE
                        if(::uwvm2::uwvm::show_dl_warning)
                        {
                            // Output the main information and memory indication
                            ::fast_io::io::perr(::uwvm2::uwvm::u8log_output,
                                                // 1
                                                ::fast_io::mnp::cond(::uwvm2::uwvm::utils::ansies::put_color, UWVM_COLOR_U8_RST_ALL_AND_SET_WHITE),
                                                u8"uwvm: ",
                                                ::fast_io::mnp::cond(::uwvm2::uwvm::utils::ansies::put_color, UWVM_COLOR_U8_YELLOW),
                                                u8"[warn]  ",
                                                ::fast_io::mnp::cond(::uwvm2::uwvm::utils::ansies::put_color, UWVM_COLOR_U8_WHITE),
                                                u8"Parsing error in DL \"",
                                                ::fast_io::mnp::cond(::uwvm2::uwvm::utils::ansies::put_color, UWVM_COLOR_U8_YELLOW),
                                                load_file_name,
                                                ::fast_io::mnp::cond(::uwvm2::uwvm::utils::ansies::put_color, UWVM_COLOR_U8_WHITE),
                                                u8"\".",
                                                ::fast_io::mnp::cond(::uwvm2::uwvm::utils::ansies::put_color, UWVM_COLOR_U8_ORANGE),
                                                u8" (dl)\n",
                                                ::fast_io::mnp::cond(::uwvm2::uwvm::utils::ansies::put_color, UWVM_COLOR_U8_WHITE),
                                                // 2
                                                u8"uwvm: ",
                                                ::fast_io::mnp::cond(::uwvm2::uwvm::utils::ansies::put_color, UWVM_COLOR_U8_YELLOW),
                                                u8"[warn]  ",
                                                ::fast_io::mnp::cond(::uwvm2::uwvm::utils::ansies::put_color, UWVM_COLOR_U8_WHITE),
                                                u8"Functions cannot be nullptr.\n",
                                                ::fast_io::mnp::cond(::uwvm2::uwvm::utils::ansies::put_color, UWVM_COLOR_U8_RST_ALL));
                        }
# endif
                        // Just skip here, no need to disable
                        continue;
                    }
                }
            }
        }

        return load_dl_rtl::ok;
    }
#endif

}  // namespace uwvm2::uwvm::wasm::loader

#ifndef UWVM_MODULE
# include <uwvm2/uwvm/utils/ansies/uwvm_color_pop_macro.h>
# include <uwvm2/utils/macro/pop_macros.h>
#endif
