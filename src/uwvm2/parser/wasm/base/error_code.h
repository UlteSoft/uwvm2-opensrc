﻿/*************************************************************
 * Ultimate WebAssembly Virtual Machine (Version 2)          *
 * Copyright (c) 2025-present UlteSoft. All rights reserved. *
 * Licensed under the APL-2.0 License (see LICENSE file).    *
 *************************************************************/

/**
 * @author      MacroModel
 * @version     2.0.0
 * @date        2025-04-27
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
#else
// std
# include <cstddef>
# include <cstdint>
# include <stdfloat>
# include <limits>
# include <type_traits>
// macro
# include <uwvm2/utils/macro/push_macros.h>
// import
# include <fast_io.h>
# include <fast_io_dsal/string_view.h>
#endif

#ifndef UWVM_MODULE_EXPORT
# define UWVM_MODULE_EXPORT
#endif

UWVM_MODULE_EXPORT namespace uwvm2::parser::wasm::base
{
    /// @brief define possible errors in wasm parser
    enum class wasm_parse_error_code : ::std::uint_least32_t
    {
        ok = 0u,
        illegal_begin_pointer,
        illegal_wasm_file_format,
        no_wasm_section_found,
        invalid_section_length,
        illegal_section_length,
        not_enough_space_unmarked,
        illegal_section_id,
        invalid_custom_name_length,
        illegal_custom_name_length,
        invalid_parameter_length,
        illegal_parameter_length,
        illegal_value_type,
        invalid_result_length,
        illegal_result_length,
        wasm1_not_allow_multi_value,
        illegal_type_prefix,
        duplicate_section,
        invalid_type_count,
        type_section_resolved_exceeded_the_actual_number,
        type_section_resolved_not_match_the_actual_number,
        forward_dependency_missing,
        invalid_import_count,
        import_section_resolved_exceeded_the_actual_number,
        import_section_resolved_not_match_the_actual_number,
        invalid_import_module_name_length,
        import_module_name_length_cannot_be_zero,
        import_module_name_too_length,
        invalid_import_extern_name_length,
        import_extern_name_length_cannot_be_zero,
        import_extern_name_too_length,
        import_missing_import_type,
        illegal_importdesc_prefix,
        invalid_type_index,
        illegal_type_index,
        not_enough_space,
        table_type_cannot_find_element,
        table_type_illegal_element,
        limit_type_cannot_find_flag,
        limit_type_illegal_flag,
        limit_type_invalid_min,
        limit_type_invalid_max,
        global_type_cannot_find_valtype,
        global_type_illegal_valtype,
        global_type_cannot_find_mut,
        global_type_illegal_mut,
        invalid_func_count,
        func_section_resolved_exceeded_the_actual_number,
        func_section_resolved_not_match_the_actual_number,
        size_exceeds_the_maximum_value_of_size_t,
        duplicate_imports_of_the_same_import_type,
        invalid_utf8_sequence,
        invalid_table_count,
        wasm1_not_allow_multi_table,
        table_section_resolved_exceeded_the_actual_number,
        table_section_resolved_not_match_the_actual_number,
        invalid_memory_count,
        wasm1_not_allow_multi_memory,
        memory_section_resolved_exceeded_the_actual_number,
        memory_section_resolved_not_match_the_actual_number,
        imp_def_num_exceed_u32max,
        invalid_global_count,
        global_section_resolved_exceeded_the_actual_number,
        global_section_resolved_not_match_the_actual_number,
        global_init_terminator_not_found,
        invalid_export_count,
        export_section_resolved_exceeded_the_actual_number,
        export_name_length_cannot_be_zero,
        export_name_too_length,
        export_missing_export_type,
        illegal_exportdesc_prefix,
        duplicate_exports_of_the_same_export_type,
        export_missing_export_idx,
        export_section_resolved_not_match_the_actual_number,
        invalid_export_idx,
        exported_index_exceeds_maxvul,
        invalid_export_name_length,
        invalid_start_idx,
        start_index_exceeds_maxvul,
        invalid_elem_count,
        elem_section_resolved_exceeded_the_actual_number,
        invalid_elem_table_idx,
        elem_table_index_exceeds_maxvul,
        elem_init_terminator_not_found,
        invalid_elem_funcidx_count,
        invalid_elem_funcidx,
        elem_func_index_exceeds_maxvul,
        element_section_resolved_not_match_the_actual_number,
        invalid_code_count,
        code_ne_defined_func,
        code_section_resolved_exceeded_the_actual_number,
        invalid_code_body_size,
        illegal_code_body_size,
        code_missing_local_type,
        invalid_clocal_n,
        final_list_of_locals_exceeds_the_maximum_value_of_u32max,
        code_section_resolved_not_match_the_actual_number,
        invalid_local_count,
        func_ref_by_start_has_illegal_sign,
        invalid_data_count,
        data_section_resolved_exceeded_the_actual_number,
        invalid_data_memory_idx,
        data_memory_index_exceeds_maxvul,
        data_init_terminator_not_found,
        invalid_data_byte_size_count,
        illegal_data_byte_size_count,
        data_section_resolved_not_match_the_actual_number,
        invalid_section_canonical_order,
        invalid_elem_kind,
        invalid_data_kind,
        limit_type_max_lt_min
    };

    /// @brief used for duplicate_imports_of_the_same_import_type
    struct duplic_imports_t
    {
        ::fast_io::u8string_view module_name;
        ::fast_io::u8string_view extern_name;
        ::std::uint_least8_t type;
    };

    /// @brief used for duplicate_exports_of_the_same_export_type
    struct duplic_exports_t
    {
        ::fast_io::u8string_view export_name;
        ::std::uint_least8_t type;
    };

    /// @brief Used to set the output of module_name errors
    struct module_name_err_t
    {
        ::fast_io::u8string_view module_name;
        ::std::uint_least32_t type;
    };

    /// @brief Used to set the output of imp_def_num_exceed_u32max errors
    struct imp_def_num_exceed_u32max_err_t
    {
        ::std::uint_least32_t imported;
        ::std::uint_least32_t defined;
        ::std::uint_least8_t type;
    };

    /// @brief Used to set the output of exported_index_exceeds_maxvul errors
    struct exported_index_exceeds_maxvul_t
    {
        ::std::uint_least32_t idx;
        ::std::uint_least32_t maxval;
        ::std::uint_least8_t type;
    };

    /// @brief Used to set the output of start_index_exceeds_maxvul errors
    struct start_index_exceeds_maxvul_t
    {
        ::std::uint_least32_t idx;
        ::std::uint_least32_t maxval;
    };

    /// @brief Used to set the output of elem_table_index_exceeds_maxvul errors
    struct elem_table_index_exceeds_maxvul_t
    {
        ::std::uint_least32_t idx;
        ::std::uint_least32_t maxval;
    };

    /// @brief Used to set the output of elem_func_index_exceeds_maxvul errors
    struct elem_func_index_exceeds_maxvul_t
    {
        ::std::uint_least32_t idx;
        ::std::uint_least32_t maxval;
    };

    /// @brief define IEEE 754 F32 and F64
#ifdef __STDCPP_FLOAT32_T__
    using error_f32 = ::std::float32_t;  // IEEE 754-2008
#else
    using error_f32 = float;  // The C++ Standard doesn't specify it. Gotta check.
#endif
    static_assert(::std::numeric_limits<error_f32>::is_iec559 && ::std::numeric_limits<error_f32>::digits == 24 &&
                      ::std::numeric_limits<error_f32>::max_exponent == 128 && ::std::numeric_limits<error_f32>::min_exponent == -125,
                  "error_f32 ain't of the IEC 559/IEEE 754 floating-point types");

#ifdef __STDCPP_FLOAT64_T__
    using error_f64 = ::std::float64_t;  // IEEE 754-2008
#else
    using error_f64 = double;  // The C++ Standard doesn't specify it. Gotta check.
#endif
    static_assert(::std::numeric_limits<error_f64>::is_iec559 && ::std::numeric_limits<error_f64>::digits == 53 &&
                      ::std::numeric_limits<error_f64>::max_exponent == 1024 && ::std::numeric_limits<error_f64>::min_exponent == -1021,
                  "error_f64 ain't of the IEC 559/IEEE 754 floating-point types");

    /// @brief Additional information provided by wasm error
    union err_selectable_t
    {
        duplic_imports_t duplic_imports;
        static_assert(::std::is_trivially_copyable_v<duplic_imports_t> && ::std::is_trivially_destructible_v<duplic_imports_t>);
        duplic_exports_t duplic_exports;
        static_assert(::std::is_trivially_copyable_v<duplic_exports_t> && ::std::is_trivially_destructible_v<duplic_exports_t>);
        module_name_err_t error_module_name;
        static_assert(::std::is_trivially_copyable_v<module_name_err_t> && ::std::is_trivially_destructible_v<module_name_err_t>);
        imp_def_num_exceed_u32max_err_t imp_def_num_exceed_u32max;
        static_assert(::std::is_trivially_copyable_v<imp_def_num_exceed_u32max_err_t> && ::std::is_trivially_destructible_v<imp_def_num_exceed_u32max_err_t>);
        exported_index_exceeds_maxvul_t exported_index_exceeds_maxvul;
        static_assert(::std::is_trivially_copyable_v<exported_index_exceeds_maxvul_t> && ::std::is_trivially_destructible_v<exported_index_exceeds_maxvul_t>);
        start_index_exceeds_maxvul_t start_index_exceeds_maxvul;
        static_assert(::std::is_trivially_copyable_v<start_index_exceeds_maxvul_t> && ::std::is_trivially_destructible_v<start_index_exceeds_maxvul_t>);
        elem_table_index_exceeds_maxvul_t elem_table_index_exceeds_maxvul;
        static_assert(::std::is_trivially_copyable_v<elem_table_index_exceeds_maxvul_t> &&
                      ::std::is_trivially_destructible_v<elem_table_index_exceeds_maxvul_t>);
        elem_func_index_exceeds_maxvul_t elem_func_index_exceeds_maxvul;
        static_assert(::std::is_trivially_copyable_v<elem_func_index_exceeds_maxvul_t> && ::std::is_trivially_destructible_v<elem_func_index_exceeds_maxvul_t>);

        ::std::byte const* err_end;
        ::std::size_t err_uz;
        ::std::ptrdiff_t err_pdt;

        ::std::uint_least64_t u64;
        ::std::int_least64_t i64;
        ::std::uint_least32_t u32;
        ::std::int_least32_t i32;
        ::std::uint_least16_t u16;
        ::std::int_least16_t i16;
        ::std::uint_least8_t u8;
        ::std::int_least8_t i8;

        error_f64 f64;
        error_f32 f32;
        bool boolean;

        ::std::uint_least64_t u64arr[1];
        ::std::int_least64_t i64arr[1];
        ::std::uint_least32_t u32arr[2];
        ::std::int_least32_t i32arr[2];
        ::std::uint_least16_t u16arr[4];
        ::std::int_least16_t i16arr[4];
        ::std::uint_least8_t u8arr[8];
        ::std::int_least8_t i8arr[8];

        error_f64 f64arr[1];
        error_f32 f32arr[2];
        bool booleanarr[8];
    };

    /// @brief Structured structure, with reference form as formal parameter
    struct error_impl
    {
        err_selectable_t err_selectable{};
        ::std::byte const* err_curr{};
        wasm_parse_error_code err_code{};
    };

}  // namespace uwvm2::parser::wasm::base

#ifndef UWVM_MODULE
// macro
# include <uwvm2/utils/macro/pop_macros.h>
#endif
