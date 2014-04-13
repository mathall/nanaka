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
      'includes': [
        '../../../build/external_common.gypi',
      ],
    },
  ],
}
