{
  'targets': [
    {
      'target_name': 'nanaka_json_parser',
      'type': 'static_library',
      'sources': [
        '../src/json.h',
        '../src/json.c',
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
    },
  ],
}
