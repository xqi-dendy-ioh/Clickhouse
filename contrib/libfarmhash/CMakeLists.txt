add_library(_farmhash
    farmhash.cc
    farmhash.h)

if (MSVC)
    target_compile_definitions (_farmhash PRIVATE FARMHASH_NO_BUILTIN_EXPECT=1)
endif ()

if (ARCH_S390X)
    add_compile_definitions(WORDS_BIGENDIAN)
endif ()

target_include_directories (_farmhash BEFORE PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_library(ch_contrib::farmhash ALIAS _farmhash)
