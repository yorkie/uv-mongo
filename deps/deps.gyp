{
  'targets': [
    {
      'target_name': 'buffer',
      'type': '<(library)',
      'sources': ['buffer/buffer.c','buffer/buffer.h'],
      'include_dirs': [
        '.',
        'include',
        'deps',
        '/usr/local/include/node'
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
    },

    {
      'target_name': 'hash',
      'type': '<(library)',
      'sources': [
        'hash/hash.c',
        'hash/hash.h',
        'hash/khash.h',
      ],
      'include_dirs': [
        '.',
        'include',
        'deps',
        '/usr/local/include/node'
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
    },

    {
      'target_name': 'net',
      'type': '<(library)',
      'sources': [
        'net/net.c',
        'net/net.h',
        'net/tls.h',
        'net/tls.c',
      ],
      'include_dirs': [
        '.',
        'include',
        'deps',
        '/usr/local/include/node'
      ],
      'dependencies': [
        'deps/deps.gyp:buffer',
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
        'libraries': [
          '-lcrypto',
          '-lssl',
          '-luv',
        ]
      },
    },
    
  ]
}
