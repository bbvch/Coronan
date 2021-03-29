option(ENABLE_CPPCHECK "Enable static analysis with cppcheck" OFF)
option(ENABLE_CLANG_TIDY "Enable static analysis with clang-tidy" ON)
option(ENABLE_INCLUDE_WHAT_YOU_USE
       "Enable static analysis with include-what-you-use" OFF
)

function(enable_static_analysis project_name)

  if(ENABLE_CPPCHECK)
    find_program(CPPCHECK cppcheck)
    if(CPPCHECK)
      set_target_properties(
        ${project_name}
        PROPERTIES
          CXX_CPPCHECK
          "${CPPCHECK}
          --suppress=missingInclude
          --enable=all
          --inline-suppr
          --inconclusive"
      )
      message(STATUS "cppcheck static analysis enabled for ${project_name}")
    else()
      message(SEND_ERROR "cppcheck requested but executable not found")
    endif()
  endif()

  if(ENABLE_CLANG_TIDY)
    find_program(
      CLANGTIDY
      NAMES
      NAMES clang-tidy clang-tidy-8 clang-tidy-9 clang-tidy-10 clang-tidy-11
    )
    if(CLANGTIDY)
      set(CLANG_TIDY_PROPERTIES "${CLANGTIDY}"
                                "-extra-arg=-Wno-unknown-warning-option"
      )
      set_target_properties(
        ${project_name} PROPERTIES CXX_CLANG_TIDY "${CLANG_TIDY_PROPERTIES}"
      )
      message(STATUS "clang-tidy static analysis enabled for ${project_name}")
    else()
      message(SEND_ERROR "clang-tidy requested but executable not found")
    endif()
  endif()

  if(ENABLE_INCLUDE_WHAT_YOU_USE)
    find_program(INCLUDE_WHAT_YOU_USE include-what-you-use)
    if(INCLUDE_WHAT_YOU_USE)
      set_target_properties(
        ${project_name} PROPERTIES CXX_INCLUDE_WHAT_YOU_USE
                                   ${INCLUDE_WHAT_YOU_USE}
      )
      message(
        STATUS
          "include-what-you-use static analysis enabled for ${project_name}"
      )
    else()
      message(
        SEND_ERROR "include-what-you-use requested but executable not found"
      )
    endif()
  endif()

endfunction()
