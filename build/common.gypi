{
  'variables': {
    'variables': {
      'target_arch%': "x86",
      'arm_version%': 5,
      'arm_neon%': 1,
      'conditions': [
        ['OS=="android"', {
          'arm_neon%': 0,
        }],
      ],
    },

    'debug_optimization_level%': '0',
    'release_optimization_level%': '3',
    'use_clang%': 0,
    'target_arch%': '<(target_arch)',

    'conditions': [
      ['target_arch=="arm" and arm_version==5', {
        'arm_arch%': "armv5te",
        'arm_tune%': "xscale",
        'arm_fpu%': "",
        'arm_float_abi%': "soft",
        'arm_thumb%': 0,
      }],
      ['target_arch=="arm" and arm_version==6', {
        'arm_arch%': "armv6",
        'arm_tune%': "",
        'arm_fpu%': "",
        'arm_float_abi%': "soft",
        'arm_thumb%': 0,
      }],
      ['target_arch=="arm" and arm_version==7', {
        'arm_arch%': "armv7-a",
        'arm_tune%': "cortex-a8",
        'arm_fpu%': "vfpv3-d16",
        'conditions': [
          ['arm_neon==1', {
            'arm_fpu%': "neon",
          }],
        ],
        'arm_float_abi%': "softfp",
        'arm_thumb%': 1,
      }],
      ['OS=="android"', {
        'variables': {
          'conditions': [
            ['target_arch=="arm"', {
              'variables': {
                'conditions': [
                  ['arm_version<7', {
                      'android_app_abi%': 'armeabi',
                  }, {
                      'android_app_abi%': 'armeabi-v7a',
                  }],
                ],
              },
              'android_app_abi%': '<(android_app_abi)',
              'android_ndk_sysroot%': '<(android_ndk_root)/platforms/android-14/arch-arm',
              'android_toolchain%': '<(android_ndk_root)/toolchains/arm-linux-androideabi-4.7/prebuilt/linux-x86_64/bin',
            }],
            ['target_arch=="x86"', {
              'android_app_abi%': 'x86',
              'android_ndk_sysroot%': '<(android_ndk_root)/platforms/android-14/arch-x86',
              'android_toolchain%': '<(android_ndk_root)/toolchains/x86-4.7/prebuilt/linux-x86_64/bin',
            }],
          ],
          'android_libstd_root%': '<(android_ndk_root)/sources/cxx-stl/gnu-libstdc++/4.7',
        },
        'android_ndk_sysroot%': '<(android_ndk_sysroot)',
        'android_ndk_include%': '<(android_ndk_sysroot)/usr/include',
        'android_libstd_include%': '<(android_libstd_root)/include',
        'android_libstd_include2%': '<(android_libstd_root)/libs/<(android_app_abi)/include',
        'android_libstd_libs%': '<(android_libstd_root)/libs/<(android_app_abi)',
        'android_toolchain%': '<(android_toolchain)',
      }],
    ],
  },

  'target_defaults': {
    'configurations': {
      'Debug': {
        'cflags': [
          '-O<(debug_optimization_level)',
          '-g',
        ],
      },
      'Release': {
        'defines': [
          'NDEBUG',
        ],
        'cflags': [
          '-O<(release_optimization_level)',
        ],
        'ldflags': [
          '-Wl,-s',
        ],
      },
    },
    'conditions': [
      ['OS=="linux" or OS=="android"', {
        'cflags': [
          '-Wall',
          '-Wextra',
          '-pedantic',
          '-Wno-unused-parameter',
        ],
        'cflags_cc': [
          '-std=c++11',
        ],
        'conditions': [
          ['use_clang==1', {
            'cflags': [
              '-fcolor-diagnostics',
            ],
          }],
        ],
      }],
      ['OS=="android"', {
        'target_conditions': [
          ['_type=="static_library"', {
            'standalone_static_library': 1,
          }],
        ],
        'include_dirs': [
          '<(android_ndk_include)',
          '<(android_libstd_include)',
          '<(android_libstd_include2)',
        ],
        'cflags': [
          '--sysroot=<(android_ndk_sysroot)',
        ],
        'ldflags': [
          '-L<(android_libstd_libs)',
          '--sysroot=<(android_ndk_sysroot)',
        ],
        'defines': [
          'ANDROID',
        ],
        'conditions': [
          ['use_clang==1', {
            'cflags_cc': [
              '-target armv7-none-linux-androideabi'
            ],
          }],
        ],
      }],
      ['target_arch=="arm"', {
        'conditions': [
          ['arm_arch!=""', {
            'cflags': [
              '-march=<(arm_arch)',
            ],
          }],
          ['arm_tune!=""', {
            'cflags': [
              '-mtune=<(arm_tune)',
            ],
          }],
          ['arm_fpu!=""', {
            'cflags': [
              '-mfpu=<(arm_fpu)',
            ],
          }],
          ['arm_float_abi!=""', {
            'cflags': [
              '-mfloat-abi=<(arm_float_abi)',
            ],
          }],
          ['arm_thumb==1', {
            'cflags': [
              '-mthumb',
            ],
          }],
        ],
      }],
    ],
  },

  'conditions': [
    ['OS=="linux" and use_clang==1', {
      'make_global_settings': [
        ['CC', '/usr/bin/clang'],
        ['CXX', '/usr/bin/clang++'],
        ['LINK', '/usr/bin/clang++'],
      ],
    }],
    ['OS=="android"', {
      'conditions': [
        ['use_clang==1', {
          'make_global_settings': [
            ['CC', '<(android_toolchain)/clang'],
            ['CXX', '<(android_toolchain)/clang++'],
            ['LINK', '<(android_toolchain)/clang'],
          ],
        }, {
          'make_global_settings': [
            ['CC', '<(android_toolchain)/*-gcc'],
            ['CXX', '<(android_toolchain)/*-g++'],
            ['LINK', '<(android_toolchain)/*-gcc'],
          ],
        }],
      ],
    }],
  ],
}
