{
  'sources': [
    '../audio/AudioDevice.cpp',
    '../audio/AudioDevice.h',
    '../audio/AudioEngine.cpp',
    '../audio/AudioEngine.h',
    '../audio/AudioSource.h',
    '../audio/AudioStream.cpp',
    '../audio/AudioStream.h',
    '../audio/AudioStreamHandle.cpp',
    '../audio/AudioStreamHandle.h',
  ],
  'conditions': [
    ['OS=="android"', {
      'includes': [
        'OpenSLES/OpenSLES.gypi',
      ],
    }],
    ['OS=="linux" or OS=="web" or OS=="osx"', {
      'includes': [
        'OpenAL/OpenAL.gypi',
      ],
    }],
  ],
}
