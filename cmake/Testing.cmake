# ============================================================
# Testing configuration
# ============================================================

if(NOT PDFENGINE_BUILD_TESTS)
    return()
endif()

enable_testing()

# ------------------------------------------------------------
# Collect test files
# ------------------------------------------------------------

file(GLOB_RECURSE TEST_SOURCES
    ${PROJECT_SOURCE_DIR}/tests/*.c
)

# ------------------------------------------------------------
# Create test executables dynamically
# ------------------------------------------------------------

foreach(test_file ${TEST_SOURCES})

    # Extract filename without extension
    get_filename_component(test_name ${test_file} NAME_WE)

    add_executable(${test_name} ${test_file})

    target_link_libraries(${test_name}
        PRIVATE pdfengine
    )

    add_test(NAME ${test_name} COMMAND ${test_name})

endforeach()

# cmake -DPDFENGINE_BUILD_TESTS=ON
# ctest

# or disable:

# cmake -DPDFENGINE_BUILD_TESTS=OFF