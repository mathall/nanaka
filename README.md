Nanaka
======

Nanaka is a cross platform game engine with no real goal or purpose beyond practice/learning, it's just something I enjoy working on and experimenting with. Currently supported platforms include android, linux, and the web.

Design
------

Below is a graph of how the modules in Nanaka may depend on each other.

                                        Game
                                         ║
             ╔═════════════════╦═════════╩═════════╦══════════╗
             ║                 ║                   ║          ║
    ─────────║─────────────────║───────────────────║──────────║─────────────────
    Nanaka   ║                 ║                   ║          ║
             ║                 v                   ║          v
             v      ┌────────────────────────┐     v      ┌───────┐
         ┌───────┐  │               ┌────────│────────────│───────│─────┐
         │ sound │  │ game  physics │ scene ══> graphics <══ gui  │     │
         └───────┘  │   ║      ║    └───║────│────────────│───║───│─────┘
           ║   ║    │   ╚══════╬════════╝    │     ║      │   v   │   ║
           ║   ║    │          ║             │     ║      │ input │   ║
           ║   ║    │          v             │     ║      └───────┘   ║
           ║   ║    │        entity          │     ║          ║       ║
           ║   ║    └────────────────────────┘     ║          ║       ║
           ║   ║               ║                   ║          ║       ║
    -------║---║---------------║-------------------║----------║-------║---------
           ║   ║               ║                   ║          ║       ║
           ║   ╚═══════════════╩═════════╦═════════╩══════════╝       ║
           ║                             ║                            ║
           v                             v                            v
         audio                        resource                     renderer

For details concerning each module, have a look at the modules' DOC files.

Using Nanaka
------------

Three principal targets are required for building a game using Nanaka; The Nanaka library itself, the game library which uses Nanaka, and a platform executable which will depend on the two.

GYP is used to generate the project files for these targets. In the build folder there are make files and gyp includes for setting up various flags and target defaults. Simply construct the gyp files for the game library and platform executable, then build with the targets defined in the make files included with Nanaka.

Prerequisites
-------------

To build a project using Nanaka, the following dependencies are required:

* libpng (included)
* json-parser (included)
* libogg (included)
* tremor (included)
* zlib (included)
* glew (non-android)
* OpenAL (non-android)
* OpenGL ES2

Optional dependencies for platform integration:

* glfw3
* libzip (included, android)

Platform specific prerequisites:

* Linux: gcc >=4.7 or clang >=3.2
* Android: Android SDK/NDK - some version
* Web: Emscripten - some version

Other prerequisites:

* GYP
* ninja (optional)

Projects using Nanaka
---------------------

* [Hobo](https://github.com/mathall/hobo/ "Hobo")

Legal
-----

External dependencies have their licences in their respective subfolder. For everything else, see LICENSE.
