{
  'targets': [
    {
      'target_name': 'nanaka_ogg',
      'type': '<(library_type)',
      'sources': [
        '../src/bitwise.c',
        '../src/framing.c',
        '../src/ogg/config_types.h',
        '../src/ogg/ogg.h',
        '../src/ogg/os_types.h',
      ],
      'all_dependent_settings': {
        'include_dirs': [
          '../src',
        ],
      },
      'include_dirs': [
        '../src',
      ],
      'conditions': [
        ['OS=="linux" or OS=="android" or OS=="web"', {
          'cflags!': [
            '-Wall',
            '-Wextra',
            '-pedantic',
          ],
        }],
        ['OS=="osx"', {
          'xcode_settings': {
            'WARNING_CFLAGS!': [
              '-Wall',
              '-Wextra',
              '-pedantic',
            ],
          },
        }],
      ],
    },
  ],
}
