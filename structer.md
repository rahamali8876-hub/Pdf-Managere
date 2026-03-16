### Before I show the structure, remember our layer rule

    OS
    ↓
    platform
    ↓
    system
    ↓
    core
    ↓
    parser / writer / render
    ↓
    apps / tools / UI

    Lower layers must never depend upward.

## Step 1 — Final Engine Folder Structure

    A clean professional layout could look like this:

pdfengine/
│
├── CMakeLists.txt
│
├── cmake/
        CompilerWarnings.cmake
        PerformDetect.cmake
        Sanitizers.cmake
│
├── include/
│  
│       ├── api/
│       │   └── pdf_engine_api.h
│       │
│       ├── core/
│       │   ├── pdf_catalog.h
│       │   ├── pdf_content_stream.h
│       │   ├── pdf_document.h
│       │   ├── pdf_image_loader.h
│       │   ├── pdf_image_xobject.h
│       │   ├── pdf_object.h
│       │   ├── pdf_page.h
│       │   ├── pdf_pages_tree.h
│       │   ├── pdf_stream.h
│       │   ├── pdf_writer_document.h
│       │   └── pdf_xref.h
                 
│       │
│       ├── image_loader/
│       │   ├── bmp_loader.h
│       │   ├── jpeg_loader.h
│       │   └── png_loader.h
│       │
│       ├── platform/
│       │   └── platform_api.h
│       │
│       └── system/
│           ├── memory_pool.h
│           └── vector.h
│
├── src/
│
│   ├── api/
│   │   └── pdf_engine_api.c
│   │
│   ├── core/
│   │   ├── pdf_catalog.c
│   │   ├── pdf_content_stream.c
│   │   ├── pdf_document.c
│   │   ├── pdf_image_loader.c
│   │   ├── pdf_image_xobject.c
│   │   ├── pdf_object.c
│   │   ├── pdf_page.c
│   │   ├── pdf_pages_tree.c
│   │   ├── pdf_stream.c
│   │   ├── pdf_writer_document.c
│   │   └── pdf_xref.c
│   │
│   ├── image_loader/
│   │   ├── bmp_loader.c
│   │   ├── jpeg_loader.c
│   │   └── png_loader.c
│   │
│   ├── platform/
│   │   └── platform_api.c
│   │
│   └── system/
│       ├── memory_pool.c
│       └── vector.c
│
└── tests/
    └── test_image_to_pdf.c

### Professional Engine Folder Structure

    pdfengine/
    │
    ├── build/
    │
    ├── cmake/
    │
    ├── include/
    │   │
    │   ├── platform/
    │   │      platform_api.h
    │   │
    │   ├── system/
    │   │      memory_pool.h
    │   │      vector.h
    │   │      string.h
    │   │      hash_table.h
    │   │      logger.h
    │   │
    │   ├── parser/
    │   │      pdf_parser.h
    │   │      xref_parser.h
    │   │
    │   ├── writer/
    │   │      pdf_writer.h
    │   │
    │   ├── render/
    │   │      renderer.h
    │   │
    │   ├── core/
    │   │      pdf_engine_api.h
    │   │      pdf_document.h
    │   │      pdf_page.h
    │   │      pdf_object.h
    │   │
    │   └── plugin/
    |        ├── plugin_manager
    |        ├── plugin_loader
    |        ├── plugin_registry
    |        ├── dependency_resolver
    |        └── plugin_runtime
    │
    ├── src/
    │   │
    │   ├── platform/
    │   │      platform_windows.c
    │   │      platform_linux.c
    │   │
    │   ├── system/
    │   │      memory_pool.c
    │   │      vector.c
    │   │      string.c
    │   │
    │   ├── core/
    │   │      pdf_engine.c
    │   │      pdf_document.c
    │   │
    │   └── plugin/
    │          plugin_manager.c
    │          plugin_loader.c
    │          dependency_resolver.c
    │
    ├── plugins/
    │   │
    │   ├── renderer_cpu/
    │   │      renderer_cpu.c
    │   │      plugin.c
    │   │
    │   ├── compressor_zlib/
    │   │      compressor_zlib.c
    │   │      plugin.c
    │   │
    │   └── editor_tools/
    │          editor_plugin.c
    │
    ├── bindings/
    │   │
    │   ├── csharp/
    │   └── python/
    │
    └── tools/

### What Real PDF Engines Add Next

    Professional engines next implement:

    JPEG dimension parser
    PNG decompression
    Color spaces
    Image compression
    Multi-page support

### Step This structure is similar in philosophy to projects like MuPDF

    Step 2 — First Code We Actually Write

    We should not jump to parser yet.

    The correct starting modules are:

    system/
    vector
    memory_pool

    Because everything else depends on them.

### Step 3 — First Header to Implement

    Create:

    include/pdf/system/vector.h

    Conceptually it will look like:

    vector
    ├ data
    ├ size
    └ capacity

    Functions will include things like:

    vector_init
    vector_push
    vector_get
    vector_destroy

### Step 4 — Why Vector Comes First

    Your PDF parser will store:

    objects
    pages
    streams
    fonts

    All of those will likely be vectors.

### Step 5 — First Real Tool

    After system layer works, we create a simple program:

    tools/pdfinfo

    Goal:

    open pdf
    count pages
    print metadata

    This is the first milestone before editing or rendering.

    Architect Question (important)

    Look at this vector structure idea:

    vector
    data
    size
    capacity

    Right now data is usually declared as:

    void *data

    But if we store multiple element types, what extra field might we add to the struct to know the size of each element?

    Example hint:

    element_size
