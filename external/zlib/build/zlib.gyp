{
  'targets': [
    {
      'target_name': 'nanaka_zlib',
      'type': '<(library_type)',
      'sources': [
        '../src/adler32.c',
        '../src/compress.c',
        '../src/crc32.c',
        '../src/crc32.h',
        '../src/deflate.c',
        '../src/deflate.h',
        '../src/gzclose.c',
        '../src/gzguts.h',
        '../src/gzlib.c',
        '../src/gzread.c',
        '../src/gzwrite.c',
        '../src/infback.c',
        '../src/inffast.c',
        '../src/inffast.h',
        '../src/inffixed.h',
        '../src/inflate.c',
        '../src/inflate.h',
        '../src/inftrees.c',
        '../src/inftrees.h',
        '../src/trees.c',
        '../src/trees.h',
        '../src/uncompr.c',
        '../src/zconf.h',
        '../src/zlib.h',
        '../src/zutil.c',
        '../src/zutil.h',
      ],
      'direct_dependent_settings': {
        'include_dirs': [
          '../src',
        ],
      },
      'includes': [
        '../../../build/external_common.gypi',
      ],
      'conditions': [
        ['OS=="linux" or OS=="android" or OS=="web"', {
          'cflags': [
            '-Wno-unused-value',
          ],
        }],
        ['OS=="osx"', {
          'xcode_settings': {
            'OTHER_CFLAGS': [
              '-Wno-unused-value',
            ],
          },
        }],
      ],
    },
  ],
}
