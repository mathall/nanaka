Nanaka
======

Nanaka is a cross platform game engine with no real goal or purpose beyond practice/learning, it's just something I enjoy working on and experimenting with.

Design
------

Below is a graph of how the modules in Nanaka may depend on each other.

                                     Game
                                      ║
                  ╔═══════════════════╬══════════╗
                  ║                   ║          ║
    ──────────────║───────────────────║──────────║─────────────────
    Nanaka        ║                   ║          ║
                  v                   ║          v
       ┌────────────────────────┐     v      ┌───────┐
       │               ┌────────│────────────│───────│─────┐
       │ game  physics │ scene ══> graphics <══ gui  │     │
       │   ║      ║    └───║────│────────────│───║───│─────┘
       │   ╚══════╬════════╝    │     ║      │   v   │   ║
       │          ║             │     ║      │ input │   ║
       │          v             │     ║      └───────┘   ║
       │        entity          │     ║          ║       ║
       └────────────────────────┘     ║          ║       ║
                  ║                   ║          ║       ║
    --------------║-------------------║----------║-------║---------
                  ║                   ║          ║       ║
                  ╚═══════════════════╬══════════╝       ║
                                      ║                  ║
                                      v                  v
                                   resource           renderer

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
* glew (non-android) and GLES2

And optionally:

* glfw3
* libzip (included, android)

Other prerequisites are:

* GYP
* ninja (optional)
* gcc >=4.7 or clang >=3.2

Projects using Nanaka
---------------------

* [Hobo](https://github.com/mathall/hobo/ "Hobo")

Legal
-----

External dependencies have their licences in their respective subfolder. For everything else, see LICENSE.
