cmake_minimum_required(VERSION 3.17)
project(kaioop)

set(CMAKE_CXX_STANDARD 17)

add_executable(kaioop main.cpp)

if (NOT DEFINED MONO_HOME)
    if (WIN32)
        set(MONO_HOME "C:/Program Files/Mono")
    elseif (APPLE)
        set(MONO_HOME "/Library/Frameworks/Mono.framework/Versions/Current")
    else()
        find_program(MONO_CONFIG NAMES mono-config)
        if (MONO_CONFIG)
            execute_process(COMMAND ${MONO_CONFIG} --prefix OUTPUT_VARIABLE MONO_HOME OUTPUT_STRIP_TRAILING_WHITESPACE)
        else()
            set(MONO_HOME "/usr")
        endif()
    endif()
endif ()

target_compile_definitions(kaioop PRIVATE MONO_HOME="${MONO_HOME}")

target_include_directories(kaioop PRIVATE "${MONO_HOME}/include/mono-2.0")

if (WIN32)
    target_link_libraries(kaioop PRIVATE
            "${MONO_HOME}/bin/mono-2.0-sgen.dll"
    )
    configure_file("${MONO_HOME}/bin/mono-2.0-sgen.dll" "${PROJECT_BINARY_DIR}/mono-2.0-sgen.dll" COPYONLY)
else()
    target_link_libraries(kaioop PRIVATE "${MONO_HOME}/lib/libmono-2.0.a")
endif()

list(APPEND cs "Lines.cs")
foreach (csFile ${cs})
    configure_file("${PROJECT_SOURCE_DIR}/cs/${csFile}" "${PROJECT_BINARY_DIR}/${csFile}")
endforeach()