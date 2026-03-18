# main cmake file is this

cmake_minimum_required(VERSION 3.16)

project(pdfengine
    VERSION 0.1
    LANGUAGES C
)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# -------------------------------------------------
# CMake modules
# -------------------------------------------------

list(APPEND CMAKE_MODULE_PATH
    ${PROJECT_SOURCE_DIR}/cmake
)

include(CompilerWarnings)
include(PlatformDetect)
include(Sanitizers)

# -------------------------------------------------
# Include directories
# -------------------------------------------------

include_directories(
    ${PROJECT_SOURCE_DIR}/include
)

# -------------------------------------------------
# Collect sources
# -------------------------------------------------

file(GLOB API_SRC
    src/api/*.c
)

file(GLOB CORE_SRC
    src/core/*.c
)

file(GLOB SYSTEM_SRC
    src/system/*.c
)

file(GLOB PLATFORM_SRC
    src/platform/*.c
)

file(GLOB IMAGE_SRC
    src/image_loader/*.c
)

# -------------------------------------------------
# Engine library
# -------------------------------------------------

add_library(pdfengine STATIC
    ${API_SRC}
    ${CORE_SRC}
    ${SYSTEM_SRC}
    ${PLATFORM_SRC}
    ${IMAGE_SRC}
)


target_include_directories(pdfengine
    PUBLIC
        ${PROJECT_SOURCE_DIR}/include/api

    PRIVATE
        ${PROJECT_SOURCE_DIR}/include
)

# target_include_directories(pdfengine
#     PUBLIC
#     ${PROJECT_SOURCE_DIR}/include
# )

# set_project_warnings(pdfengine)

# -------------------------------------------------
# Test executable
# -------------------------------------------------

add_executable(test_pdf
    tests/test_image_to_pdf.c
)

target_link_libraries(test_pdf
    pdfengine
)







# **********************
# main cmake file is this

# cmake_minimum_required(VERSION 3.16)

# project(pdfengine
#     VERSION 0.1
#     LANGUAGES C
# )

# set(CMAKE_C_STANDARD 11)
# set(CMAKE_C_STANDARD_REQUIRED ON)

# # -------------------------------------------------
# # CMake modules
# # -------------------------------------------------

# list(APPEND CMAKE_MODULE_PATH
#     ${PROJECT_SOURCE_DIR}/cmake
# )

# include(CompilerWarnings)
# include(PlatformDetect)
# include(Sanitizers)

# # -------------------------------------------------
# # Include directories
# # -------------------------------------------------

# include_directories(
#     ${PROJECT_SOURCE_DIR}/include
# )

# # -------------------------------------------------
# # Collect sources
# # -------------------------------------------------

# # file(API_SRC
# #     src/api/*.c
# # )

# file(GLOB API_SRC
#     src/api/*.c
# )

# set(CORE_SRC
#     src/core/pdf_catalog.c
#     src/core/pdf_arrary.c
#     src/core/pdf_dictionary.c
#     src/core/pdf_document.c
#     src/core/pdf_image_xobject.c
#     src/core/pdf_image.c
#     src/core/pdf_object_allocator.c
#     src/core/pdf_object_store.c
#     src/core/pdf_object.c
#     src/core/pdf_page.c
#     src/core/pdf_page_tree.c
#     src/core/pdf_stream.c
#     src/core/pdf_write_document.c

#     src/core/pdf_xref.c
#     src/write/pdf_write.c
#     src/write/pdf_xref_write.c
#     src/write/pdf_serializer.c
# )


# file(GLOB SYSTEM_SRC
#     src/system/*.c
# )

# file(GLOB PLATFORM_SRC
#     src/platform/*.c
# )

# file(GLOB IMAGE_SRC
#     src/image_loader/*.c
# )

# # file(GLOB API_SRC
# #     src/api/*.c
# # )

# # file(GLOB CORE_SRC
# #     src/core/*.c
# # )

# # -------------------------------------------------
# # Engine library
# # -------------------------------------------------

# add_library(pdfengine STATIC
#     ${API_SRC}
#     ${CORE_SRC}
#     ${SYSTEM_SRC}
#     ${PLATFORM_SRC}
#     ${IMAGE_SRC}
# )

# target_include_directories(pdfengine
#     PUBLIC
#     ${PROJECT_SOURCE_DIR}/include
# )

# # set_project_warnings(pdfengine)

# # -------------------------------------------------
# # Test executable
# # -------------------------------------------------

# add_executable(test_pdf
#     tests/test_image_to_pdf.c
# )

# target_link_libraries(test_pdf
#     pdfengine
# )