{
  'targets': [
    {
      'target_name': 'nanaka_png',
      'type': '<(library_type)',
      'sources': [
        '../src/png.c',
        '../src/png.h',
        '../src/pngconf.h',
        '../src/pngdebug.h',
        '../src/pngerror.c',
        '../src/pngget.c',
        '../src/pnginfo.h',
        '../src/pnglibconf.h',
        '../src/pngmem.c',
        '../src/pngpread.c',
        '../src/pngpriv.h',
        '../src/pngread.c',
        '../src/pngrio.c',
        '../src/pngrtran.c',
        '../src/pngrutil.c',
        '../src/pngset.c',
        '../src/pngstruct.h',
        '../src/pngtrans.c',
        '../src/pngwio.c',
        '../src/pngwrite.c',
        '../src/pngwtran.c',
        '../src/pngwutil.c',
      ],
      'all_dependent_settings': {
        'include_dirs': [
          '../src',
        ],
      },
      'cflags!': [
        '-Wall',
        '-Wextra',
        '-pedantic',
      ],
      'conditions': [
        ['OS=="linux" or OS=="android"', {
          'all_dependent_settings': {
            'link_settings': {
              'libraries': [
                '-lz',
              ],
            },
          },
        }],
        ['OS=="web"', {
          'dependencies': [
            '../../zlib/build/zlib.gyp:nanaka_zlib',
          ],
        }],
      ],
    },
  ],
}
