{
  'targets': [
    {
      'target_name': 'nanaka',
      'type': 'static_library',
      'dependencies': [
        '../external/libpng/build/libpng.gyp:nanaka_png',
        '../external/json-parser/build/json-parser.gyp:nanaka_json_parser',
      ],
      'include_dirs': [
        '..',
      ],
      'includes': [
        '../entity/entity.gypi',
        '../game/game.gypi',
        '../graphics/graphics.gypi',
        '../gui/gui.gypi',
        '../input/input.gypi',
        '../main/main.gypi',
        '../math/math.gypi',
        '../physics/physics.gypi',
        '../pi/pi.gypi',
        '../renderer/renderer.gypi',
        '../resource/resource.gypi',
        '../scene/scene.gypi',
        '../utils/utils.gypi',
      ],
      'conditions': [
        ['OS=="android"', {
          'dependencies': [
            '../external/libzip/build/libzip.gyp:nanaka_zip',
          ],
        }],
        ['OS=="linux"', {
          'all_dependent_settings': {
            'link_settings': {
              'libraries': [
                '-lGL',
                '-lpthread',
              ],
            },
          },
        }],
      ],
    },
  ],
}
