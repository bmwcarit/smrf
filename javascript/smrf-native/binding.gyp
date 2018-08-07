{
  'variables': {
    'SMRF_LIB_DIR': '/usr/local/lib',
    'SMRF_INCLUDE_DIR': '/usr/local/include'
  },
  'targets': [
  {
    'target_name': 'smrf-native-cpp',
    'sources': [ 'src/smrf.cpp' ],
    'cflags_cc': [ '-std=c++14' ],
    'cflags!': [ '-fno-exceptions'],
    'cflags_cc!': [ '-fno-exceptions'],
    'include_dirs': [
        "<!(node -e \"require('nan')\")",
        '<@(SMRF_INCLUDE_DIR)'
    ],
    'libraries': ['-Wl,-rpath,<@(SMRF_LIB_DIR) -L<@(SMRF_LIB_DIR) -lsmrf'],
    'conditions': [
        ['OS=="mac"',
            {
                'xcode_settings': {
                    'GCC_ENABLE_CPP_EXCEPTIONS': 'YES',
                    'OTHER_CPLUSPLUSFLAGS': ['-std=c++14']
                },
            }
        ]
      ],
    }
  ]
}
