﻿/*************************************************************
 * Ultimate WebAssembly Virtual Machine (Version 2)          *
 * Copyright (c) 2025-present UlteSoft. All rights reserved. *
 * Licensed under the APL-2.0 License (see LICENSE file).    *
 *************************************************************/

/**
 * @author      MacroModel
 * @version     2.0.0
 * @date        2025-05-02
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

#if !(__cpp_pack_indexing >= 202311L)
# error "UWVM requires at least C++26 standard compiler."
#endif

#include <cstddef>
#include <cstdint>
#include <type_traits>
#include <concepts>
#include <memory>

#include <uwvm2/utils/macro/push_macros.h>

#ifdef UWVM_MODULE
import fast_io;
import uwvm2.parser.wasm.concepts;
import uwvm2.parser.wasm.standard.wasm1.type;
import uwvm2.uwvm.io;
#else
# include <fast_io.h>
# include <fast_io_dsal/string_view.h>
# include <fast_io_dsal/tuple.h>
# include <uwvm2/parser/wasm/concepts/impl.h>
# include <uwvm2/parser/wasm/standard/wasm1/type/impl.h>
# include <uwvm2/uwvm/io/impl.h>
#endif

struct B1F1_feapara
{
    ::std::uint_least32_t test{};
};

struct B1F1
{
    inline static constexpr ::fast_io::u8string_view feature_name{u8"B1F1"};
    inline static constexpr ::uwvm2::parser::wasm::standard::wasm1::type::wasm_u32 binfmt_version{1u};

    using parameter = B1F1_feapara;
};

static_assert(::uwvm2::parser::wasm::concepts::has_feature_parameter<B1F1>);

struct B1F2_feapara
{
    float test{};
};

struct B1F2
{
    inline static constexpr ::fast_io::u8string_view feature_name{u8"B1F2"};
    inline static constexpr ::uwvm2::parser::wasm::standard::wasm1::type::wasm_u32 binfmt_version{1u};

    using parameter = B1F2_feapara;
};

static_assert(::uwvm2::parser::wasm::concepts::has_feature_parameter<B1F2>);

struct B1F3
{
    inline static constexpr ::fast_io::u8string_view feature_name{u8"B1F3"};
    inline static constexpr ::uwvm2::parser::wasm::standard::wasm1::type::wasm_u32 binfmt_version{1u};
};

static_assert(!::uwvm2::parser::wasm::concepts::has_feature_parameter<B1F3>);

// For example, the common structure
struct binfmt_ver1_module_storage_t
{
    // This one is used to automatically synthesize storage structures, and supports extensions
};

template <::uwvm2::parser::wasm::concepts::wasm_feature... Fs>
inline constexpr binfmt_ver1_module_storage_t binfmt_ver1_handle_func(::std::byte const*,
                                                                      ::std::byte const*,
                                                                      ::uwvm2::parser::wasm::base::error_impl&,
                                                                      ::uwvm2::parser::wasm::concepts::feature_parameter_t<Fs...> const& feapara) UWVM_THROWS
{
    // This defines the function that handles binary format 1.
    // Supported by <::uwvm2::parser::wasm::concepts::wasm_feature... Fs> Continued Expansion
    []<::std::size_t... I>(::std::index_sequence<I...>) constexpr noexcept
    { ((::fast_io::io::perrln(::uwvm2::uwvm::u8log_output, u8"binfmt1: ", Fs...[I] ::feature_name)), ...); }(::std::make_index_sequence<sizeof...(Fs)>{});

    if constexpr((::std::same_as<B1F1, Fs> || ...))
    {
        /// @brief Before using it, you must determine if there is a desired type in the Fs.
        auto const& B1F1_feapara_r{::uwvm2::parser::wasm::concepts::get_curr_feature_parameter<B1F1>(feapara)};
        ::fast_io::io::perrln(::uwvm2::uwvm::u8log_output, u8"B1F1_feapara_r: ", B1F1_feapara_r.test);
    }

    if constexpr((::std::same_as<B1F2, Fs> || ...))
    {
        /// @brief Before using it, you must determine if there is a desired type in the Fs.
        auto const& B1F2_feapara_r{::uwvm2::parser::wasm::concepts::get_curr_feature_parameter<B1F2>(feapara)};
        ::fast_io::io::perrln(::uwvm2::uwvm::u8log_output, u8"B1F2_feapara_r: ", B1F2_feapara_r.test);
    }
    return {};
}

template <::uwvm2::parser::wasm::concepts::wasm_feature... Fs>
inline constexpr auto define_wasm_binfmt_parsering_strategy(::uwvm2::parser::wasm::concepts::feature_reserve_type_t<B1F1>, ::fast_io::tuple<Fs...>) noexcept
{
    // Since B1F1 defines binfmt as 1 and also defines the handler function, this function is the parsing function for binfmt1
    return ::std::addressof(binfmt_ver1_handle_func<Fs...>);
}

struct B2F4
{
    inline static constexpr ::fast_io::u8string_view feature_name{u8"B2F4"};
    inline static constexpr ::uwvm2::parser::wasm::standard::wasm1::type::wasm_u32 binfmt_version{2u};
};

static_assert(!::uwvm2::parser::wasm::concepts::has_feature_parameter<B2F4>);

// For example, the variable parameter
template <::uwvm2::parser::wasm::concepts::wasm_feature... Fs>
struct binfmt_ver2_module_storage_t
{
    // This one is used to automatically synthesize storage structures, and supports extensions
};

template <::uwvm2::parser::wasm::concepts::wasm_feature... Fs>
inline constexpr binfmt_ver2_module_storage_t<Fs...> binfmt_ver2_handle_func(::std::byte const*,
                                                                             ::std::byte const*,
                                                                             ::uwvm2::parser::wasm::base::error_impl&,
                                                                             ::uwvm2::parser::wasm::concepts::feature_parameter_t<Fs...> const&) UWVM_THROWS
{
    // This defines the function that handles binary format 2.
    // Supported by <::uwvm2::parser::wasm::concepts::wasm_feature... Fs> Continued Expansion
    []<::std::size_t... I>(::std::index_sequence<I...>) constexpr noexcept
    { ((::fast_io::io::perrln(::uwvm2::uwvm::u8log_output, u8"binfmt2: ", Fs...[I] ::feature_name)), ...); }(::std::make_index_sequence<sizeof...(Fs)>{});
    return {};
}

template <::uwvm2::parser::wasm::concepts::wasm_feature... Fs>
inline constexpr auto define_wasm_binfmt_parsering_strategy(::uwvm2::parser::wasm::concepts::feature_reserve_type_t<B2F4>, ::fast_io::tuple<Fs...>) noexcept
{
    // Since B2F4 defines binfmt as 2 and also defines the handler function, this function is the parsing function for binfmt1
    return ::std::addressof(binfmt_ver2_handle_func<Fs...>);
}

int main()
{
    constexpr ::fast_io::tuple<B1F1, B1F2, B1F3, B2F4> features{};

    ::uwvm2::parser::wasm::base::error_impl e{};

    // binfmt 1
    constexpr auto binfmt1_funcp{::uwvm2::parser::wasm::concepts::operation::get_binfmt_handler_func_p_from_tuple<1>(features)};
    using wasm_binfmt1_features_t =
        decltype(::uwvm2::parser::wasm::concepts::operation::get_specified_binfmt_feature_tuple_from_all_features_tuple<1>(features));
    static_assert(::std::same_as<wasm_binfmt1_features_t, ::fast_io::tuple<B1F1, B1F2, B1F3>>,
                  "wasm_binfmt1_features_t includes only features with a binfmt of 1");
    using wasm_binfmt1_storage_t = decltype(::uwvm2::parser::wasm::concepts::operation::get_module_storage_type_from_tuple(wasm_binfmt1_features_t{}));
    static_assert(::std::same_as<wasm_binfmt1_storage_t, binfmt_ver1_module_storage_t>,
                  "wasm_binfmt1_storage_t is the type returned by binfmt_ver1_handle_func<B1F1, B1F2, B1F3>");
    using wasm_binfmt1_feature_parameter_t = decltype(::uwvm2::parser::wasm::concepts::get_feature_parameter_type_from_tuple(wasm_binfmt1_features_t{}));
    wasm_binfmt1_feature_parameter_t wasm_binfmt1_feature_parameter{};
    static_assert(::std::same_as<decltype(wasm_binfmt1_feature_parameter.parameters), ::fast_io::tuple<B1F1_feapara, B1F2_feapara>>);
    // get feature parameters
    auto& B1F1_feapara_r{::uwvm2::parser::wasm::concepts::get_curr_feature_parameter<B1F1>(wasm_binfmt1_feature_parameter)};
    static_assert(::std::same_as<::std::remove_cvref_t<decltype(B1F1_feapara_r)>, B1F1_feapara>);
    B1F1_feapara_r.test = 1;
    auto& B1F2_feapara_r{::uwvm2::parser::wasm::concepts::get_curr_feature_parameter<B1F2>(wasm_binfmt1_feature_parameter)};
    static_assert(::std::same_as<::std::remove_cvref_t<decltype(B1F2_feapara_r)>, B1F2_feapara>);
    B1F2_feapara_r.test = 2.2;
    [[maybe_unused]] wasm_binfmt1_storage_t storage1 = binfmt1_funcp(nullptr, nullptr, e, wasm_binfmt1_feature_parameter);

    // binfmt 2
    constexpr auto binfmt2_funcp{::uwvm2::parser::wasm::concepts::operation::get_binfmt_handler_func_p_from_tuple<2>(features)};
    using wasm_binfmt2_features_t =
        decltype(::uwvm2::parser::wasm::concepts::operation::get_specified_binfmt_feature_tuple_from_all_features_tuple<2>(features));
    static_assert(::std::same_as<wasm_binfmt2_features_t, ::fast_io::tuple<B2F4>>, "wasm_binfmt2_features_t includes only features with a binfmt of 2");
    using wasm_binfmt2_storage_t = decltype(::uwvm2::parser::wasm::concepts::operation::get_module_storage_type_from_tuple(wasm_binfmt2_features_t{}));
    static_assert(::std::same_as<wasm_binfmt2_storage_t, binfmt_ver2_module_storage_t<B2F4>>,
                  "wasm_binfmt2_storage_t is the type of binfmt_ver2_handle_func<B2F4> return");
    using wasm_binfmt2_feature_parameter_t = decltype(::uwvm2::parser::wasm::concepts::get_feature_parameter_type_from_tuple(wasm_binfmt2_features_t{}));
    wasm_binfmt2_feature_parameter_t wasm_binfmt2_feature_parameter{};
    [[maybe_unused]] wasm_binfmt2_storage_t storage2 = binfmt2_funcp(nullptr, nullptr, e, wasm_binfmt2_feature_parameter);
}

/*
output: (stderr)
binfmt1: B1F1
binfmt1: B1F2
binfmt1: B1F3
B1F1_feapara_r: 1
B1F2_feapara_r: 2.2
binfmt2: B2F4
*/

// macro
#include <uwvm2/utils/macro/pop_macros.h>
