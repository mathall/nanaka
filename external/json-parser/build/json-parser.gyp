{
  'targets': [
    {
      'target_name': 'nanaka_json_parser',
      'type': '<(library_type)',
      'sources': [
        '../src/json.h',
        '../src/json.c',
      ],
      'all_dependent_settings': {
        'include_dirs': [
          '../src',
        ],
      },
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
