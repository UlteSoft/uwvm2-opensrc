
if is_plat("djgpp") then
    set_allowedarchs("i586")
end

set_defaultarchs("djgpp|i586")

function djgpp_target()

    local use_llvm_toolchain = get_config("use-llvm")
    if use_llvm_toolchain then	
        set_toolchains("clang")
        add_ldflags("-fuse-ld=lld", {force = true})
    end

    local sysroot_para = get_config("sysroot")
    if sysroot_para ~= "detect" and sysroot_para then
        local sysroot_cvt = "--sysroot=" .. sysroot_para
        add_cxflags(sysroot_cvt, {force = true})
        add_ldflags(sysroot_cvt, {force = true})
    end

    local target_para = get_config("target")
    if target_para ~= "detect" and target_para then
        local target_cvt = "--target=" .. target_para
        add_cxflags(target_cvt, {force = true})
        add_ldflags(target_cvt, {force = true})
    end

    local debug_strip = get_config("debug-strip")
    if debug_strip == "all" or is_mode("release", "releasedbg") then
        add_cxflags("-fno-ident") -- also strip ident data
    end

    add_cxflags("-fno-rtti") -- disable rtti
    add_cxflags("-fno-unwind-tables") -- disable unwind tables
    --add_cxflags("-fno-asynchronous-unwind-tables") -- disable asynchronous unwind tables

    if is_kind("binary") then
        set_extension(".exe")
    end

    local static_link = get_config("static")
    if static_link then	
        add_ldflags("-static", {force = true})
    end

    local march = get_config("march")
    if not (not march or march == "none" or march == "default") then
        local march_target = "-march=" .. march
        add_cxflags(march_target)
    end
end