### 🔥 How To Build (Windows MinGW)

    mkdir build
    cd build
    cmake -G "MinGW Makefiles" ..
    cmake --build . -j

### project(pdfengine) like google

    # 1. project options
    include(ProjectOptions)

    # 2. compiler settings
    include(CompilerWarnings)
    include(PlatformDetect)
    include(Sanitizers)
    include(ClangTools)

    # 3. create library
    add_library(pdfengine ...)

    # 4. dependencies
    include(Dependencies)

    # 5. tests
    include(Testing)

### If You Want to Go EVEN DEEPER

    Next PhD-level subsystem:

    👉 Layout Engine (auto placement, scaling, grids)
    👉 Font/Text Engine (hard but powerful)
    👉 Incremental PDF writing (advanced)
