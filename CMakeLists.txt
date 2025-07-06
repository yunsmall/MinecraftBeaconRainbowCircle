cmake_minimum_required(VERSION 3.31)
project(minecraft_beacon_circle)

set(CMAKE_CXX_STANDARD 23)

find_package(TBB CONFIG REQUIRED)
find_package(cxxopts CONFIG REQUIRED)
find_package(Gettext REQUIRED)
find_package(Intl REQUIRED)

if (MSVC)
    add_compile_options("/source-charset:utf-8")
endif ()

set(languages en zh_CN)

foreach (language IN LISTS languages)
    configure_file(locale/${language}.po ${language}.po COPYONLY)
endforeach ()

gettext_process_pot_file(
        minecraft_beacon_circle.pot
        ALL
        INSTALL_DESTINATION ${CMAKE_INSTALL_PREFIX}/locale
        LANGUAGES ${languages}
)

include_directories(include)

file(GLOB_RECURSE SRC_FILES src/*.cpp)

add_executable(minecraft_beacon_circle ${SRC_FILES})
target_link_libraries(minecraft_beacon_circle PRIVATE TBB::tbb)
target_link_libraries(minecraft_beacon_circle PRIVATE cxxopts::cxxopts)
target_link_libraries(minecraft_beacon_circle PUBLIC ${Intl_LIBRARY})
target_include_directories(minecraft_beacon_circle PUBLIC ${Intl_INCLUDE_DIRS})

install(TARGETS minecraft_beacon_circle
        RUNTIME_DEPENDENCY_SET deps_set
        RUNTIME DESTINATION ./
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
)

if (WIN32)
    install(CODE [[
    file(GET_RUNTIME_DEPENDENCIES
        RESOLVED_DEPENDENCIES_VAR RESOLVED_DEPS
        UNRESOLVED_DEPENDENCIES_VAR UNRESOLVED_DEPS
        EXECUTABLES $<TARGET_FILE:minecraft_beacon_circle>
        PRE_EXCLUDE_REGEXES
            "api-ms-win-.*"
            "ext-ms-win-.*"
            "ucrtbase.*"
            "vcruntime.*"
            "msvcp.*"
        POST_EXCLUDE_REGEXES
            ".*system32.*"
            ".*WindowsApps.*"
    )
    file(INSTALL ${RESOLVED_DEPS} DESTINATION ${CMAKE_INSTALL_PREFIX})
    ]])
endif ()