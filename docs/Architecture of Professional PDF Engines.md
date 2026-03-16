### High-Level Architecture of Professional PDF Engines

                APPLICATION LAYER
        (Viewer / CLI / C# UI / API bindings)
                       │
                       ▼
                PUBLIC ENGINE API
               (stable ABI boundary)
                       │
                       ▼
                 PDF ENGINE CORE
        ┌─────────────────────────────────┐
        │ document management             │
        │ page model                      │
        │ object model                    │
        │ resource management             │
        └─────────────────────────────────┘
                       │
                       ▼
                 PDF INTERPRETER
        ┌─────────────────────────────────┐
        │ stream parsing                  │
        │ operator interpreter            │
        │ graphics state stack            │
        │ text rendering logic            │
        └─────────────────────────────────┘
                       │
                       ▼
                 RENDERING ENGINE
        ┌─────────────────────────────────┐
        │ rasterizer                      │
        │ path filling / stroking         │
        │ transparency compositor         │
        │ image decoders                  │
        └─────────────────────────────────┘
                       │
                       ▼
                 DEVICE BACKENDS
        ┌─────────────────────────────────┐
        │ bitmap renderer                 │
        │ printer backend                 │
        │ SVG backend                     │
        │ OpenGL / GPU renderer           │
        └─────────────────────────────────┘
                       │
                       ▼
              PLATFORM / SYSTEM LAYER
        (filesystem, threads, memory, etc.)

### Folder Architecture Used by Real PDF Engines

    A professional engine typically looks like this:

    pdfengine/
    │
    ├── include/
    │   ├── pdf/
    │   │   ├── api/
    │   │   │   pdf_engine.h
    │   │   │   pdf_document.h
    │   │   │   pdf_page.h
    │   │   │
    │   │   ├── core/
    │   │   │   object.h
    │   │   │   stream.h
    │   │   │   xref.h
    │   │   │
    │   │   ├── render/
    │   │   │   renderer.h
    │   │   │   rasterizer.h
    │   │   │
    │   │   └── device/
    │   │       bitmap_device.h
    │   │       printer_device.h
    │   │
    │   ├── system/
    │   │   vector.h
    │   │   memory_pool.h
    │   │
    │   └── platform/
    │       platform_api.h
    │
    ├── src/
    │   ├── core/
    │   │   pdf_document.c
    │   │   pdf_page.c
    │   │   pdf_object.c
    │   │   pdf_xref.c
    │   │
    │   ├── parser/
    │   │   pdf_lexer.c
    │   │   pdf_parser.c
    │   │
    │   ├── interpreter/
    │   │   pdf_operator.c
    │   │   pdf_graphics_state.c
    │   │
    │   ├── render/
    │   │   rasterizer.c
    │   │   compositor.c
    │   │
    │   ├── device/
    │   │   bitmap_device.c
    │   │   printer_device.c
    │   │
    │   └── system/
    │       vector.c
    │       memory_pool.c
    │
    └── platform/
        windows/
        linux/
        mac/

### Internal Object Model (Very Important)

    PDF engines revolve around the PDF object system.

    PDF internally contains:

    objects
    ├── dictionary
    ├── array
    ├── stream
    ├── string
    ├── number
    └── reference

    Engine representation:

    pdf_object
    │
    ├── type
    ├── value union
    │
    ├── dictionary
    ├── array
    ├── number
    ├── string
    └── stream

    Example structure:

    typedef enum
    {
        PDF_OBJ_NULL,
        PDF_OBJ_BOOL,
        PDF_OBJ_INT,
        PDF_OBJ_REAL,
        PDF_OBJ_STRING,
        PDF_OBJ_NAME,
        PDF_OBJ_ARRAY,
        PDF_OBJ_DICT,
        PDF_OBJ_STREAM,
        PDF_OBJ_REF

    } pdf_object_type;

    This is the heart of every PDF engine.

### Rendering Pipeline (How a Page Becomes Pixels)

    When rendering a page:

    PDF page
    │
    ▼
    Content stream
    │
    ▼
    Operator parser
    │
    ▼
    Graphics state
    │
    ▼
    Path / Text / Image commands
    │
    ▼
    Rasterizer
    │
    ▼
    Bitmap output

    Example operators:

    m  → move to
    l  → line to
    c  → curve
    S  → stroke
    f  → fill
    Tj → show text

### Device Abstraction (Important for your engine)

    Professional engines don't render directly.

    They render to devices.

    device interface
    │
    ├── bitmap device
    ├── printer device
    ├── SVG device
    └── GPU device

    Example:

    typedef struct pdf_device
    {
        void (*fill_path)(...);
        void (*stroke_path)(...);
        void (*draw_image)(...);
        void (*draw_text)(...);

    } pdf_device;

    This makes the engine extremely extensible.

### Memory Architecture

    Real engines use multiple allocators:

    system/
    ├── arena allocator
    ├── memory pool
    ├── vector
    ├── hash table
    └── string builder

    You already started correctly with:

    memory_pool
    vector

    That is exactly what professional engines do.

### ABI Boundary (Important for your C# UI)

    Your design idea is correct:

    C Engine
    │
    ├── static library (.a)
    ├── dynamic library (.dll / .so)
    │
    ▼
    C# UI (PInvoke)

    Your exported API should look like:

    PDF_API pdf_document_t *
    pdf_open_document(const char *path);

    C# calls it via:

    DllImport("pdfengine.dll")

### The Correct Development Order

    Professional engines are built in this order:

    1. platform layer
    2. system utilities
    3. memory allocators
    4. containers (vector, hash)
    5. PDF object model
    6. lexer
    7. parser
    8. cross reference table
    9. document/page model
    10. rendering interpreter
    11. rasterizer
    12. device backend
    13. public API

    You are currently at:

    STEP 4

    which is exactly correct.

### Next Modules You Should Implement

    Your next critical modules should be:

    system/
        string
        hash_table

    Then:

    pdf/
        object
        dictionary
        array

    Because everything in PDF uses dictionaries and arrays.
