{
  'targets': [
    {
      'target_name': 'nanaka_tremor',
      'type': '<(library_type)',
      'dependencies': [
        '../../libogg/build/libogg.gyp:nanaka_ogg',
      ],
      'sources': [
        '../src/asm_arm.h',
        '../src/backends.h',
        '../src/block.c',
        '../src/block.h',
        '../src/codebook.c',
        '../src/codebook.h',
        '../src/codec_internal.h',
        '../src/config_types.h',
        '../src/floor0.c',
        '../src/floor1.c',
        '../src/info.c',
        '../src/ivorbiscodec.h',
        '../src/ivorbisfile.h',
        '../src/lsp_lookup.h',
        '../src/mapping0.c',
        '../src/mdct.c',
        '../src/mdct.h',
        '../src/mdct_lookup.h',
        '../src/misc.h',
        '../src/os.h',
        '../src/registry.c',
        '../src/registry.h',
        '../src/res012.c',
        '../src/sharedbook.c',
        '../src/synthesis.c',
        '../src/vorbisfile.c',
        '../src/window.c',
        '../src/window.h',
        '../src/window_lookup.h',
      ],
      'all_dependent_settings': {
        'include_dirs': [
          '../src',
        ],
      },
      'conditions': [
        ['OS=="linux" or OS=="android" or OS=="web"', {
          'cflags': [
            '-Wno-duplicate-decl-specifier',
          ],
          'cflags!': [
            '-Wall',
            '-Wextra',
            '-pedantic',
            '-mthumb',
          ],
        }],
        ['OS=="osx"', {
          'xcode_settings': {
            'OTHER_CFLAGS': [
              '-Wno-duplicate-decl-specifier',
            ],
            'WARNING_CFLAGS!': [
              '-Wall',
              '-Wextra',
              '-pedantic',
            ],
          },
        }],
        ['target_arch=="arm"', {
          'defines': [
            '_ARM_ASSEM_',
          ],
        }],
        ['OS=="android" and target_arch=="x86"', {
          'defines': [
            'LITTLE_ENDIAN',
            'BYTE_ORDER=LITTLE_ENDIAN',
          ],
        }],
      ],
    },
  ],
}
