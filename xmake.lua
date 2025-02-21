set_project("ElysiumEngine")
set_version("1.0.0")

set_languages("c++20")
set_warnings("all", "error")

add_rules("mode.debug", "mode.release")

if is_mode("debug") then
    set_symbols("debug")
    set_optimize("none")
    set_runtimes("MDd")
else
    set_symbols("hidden")
    set_optimize("fastest")
    set_strip("all")
end

add_requires("glfw 3.4", "vulkansdk", "glm")
add_packages("glfw", "vulkansdk", "glm")

target("elysium")
    set_kind("static")
    add_files("engine/*.cpp")
    add_includedirs("engine/external")
    set_targetdir("ElysiumEngine/lib")

    if is_mode("debug") then
        add_defines("_DEBUG")
    end

    after_build(function (target)
        os.rm("ElysiumEngine/includes")
        os.mkdir("ElysiumEngine/includes")
        os.cp("engine/*.hpp", "ElysiumEngine/includes/elysium/")
        os.cp("engine/external/*.hpp", "ElysiumEngine/includes/elysium/external/")

    end)

target("app")
    set_kind("binary")
    add_files("app/*.cpp")
    add_deps("elysium")
    add_includedirs("ElysiumEngine/includes")


    if is_mode("debug") then
        add_defines("_DEBUG")
    end
