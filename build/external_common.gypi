{
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
}
