{
  'targets': [
    {
      'target_name': 'uvmongo',
      'type': '<(library)',
      'sources': ['<!@(find src -name *.c)'],
      'include_dirs': [
        '.',
        'src',
        'deps',
        '/usr/local/include/node'
      ],
      # 'dependencies': [
      #   'deps.gyp:buffer',
      #   'deps.gyp:hash',
      #   'deps.gyp:net',
      # ],
      'cflags': [
        '-std=c99',
        '-Wall',
      ],
      'conditions': [
        ['OS=="mac"', {
          'xcode_settings': {
            'GCC_ENABLE_CPP_EXCEPTIONS': 'YES',
            'GCC_C_LANGUAGE_STANDARD': 'c99'
          }
        }]
      ],
      'link_settings': {
        'libraries': [
          '-lcrypto',
          '-luv',
        ]
      },
    },

    {
      'target_name': 'tests',
      'type': 'executable',
      'sources': [
        'tests/simple.c',
      ],
      'include_dirs': [
        '.',
        'src',
        'deps',
        '/usr/local/include/node'
      ],
      'dependencies': [
        'uvmongo',
      ],
      'cflags': [
        '-std=c99',
        '-Wall',
      ],
      'conditions': [
        ['OS=="mac"', {
          'xcode_settings': {
            'GCC_ENABLE_CPP_EXCEPTIONS': 'YES',
            'GCC_C_LANGUAGE_STANDARD': 'c99'
          }
        }]
      ],
      'link_settings': {
        'libraries': ['-luv']
      },
    },

  ]
}
