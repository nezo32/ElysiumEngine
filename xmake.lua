set_project("ElysiumEngine")
set_version("1.0.0")

set_languages("c++20")
set_warnings("all", "error")

add_rules("mode.debug", "mode.release")

if is_mode("debug") then
    set_symbols("debug")
    set_optimize("none")
    set_runtimes("MDd")
    add_defines("NDEBUG")
else
    set_symbols("hidden")
    set_optimize("fastest")
    set_strip("all")
    set_runtimes("MD")
end

add_requires("glfw 3.4", "vulkansdk")
add_packages("glfw", "vulkansdk")

target("elysium")
    set_kind("static")
    add_files("engine/*.cpp")
    set_targetdir("ElysiumEngine/lib")

    after_build(function (target)
        os.rm("ElysiumEngine/includes")
        os.mkdir("ElysiumEngine/includes")
        os.cp("engine/*.h", "ElysiumEngine/includes/elysium/")
        os.cp("engine/external/*.h", "ElysiumEngine/includes/elysium/external/")
    end)

target("app")
    set_kind("binary")
    add_files("app/*.cpp")
    add_deps("elysium")
    add_includedirs("ElysiumEngine/includes")
