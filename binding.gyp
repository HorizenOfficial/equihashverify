{
    "targets": [
        {
            "target_name": "equihashverify",
            "dependencies": [
                "libequi",
            ],
            "sources": [
                "equihashverify.cc",
            ],
            "include_dirs": [
                "<!(node -e \"require('nan')\")"
            ],
            "defines": [
            ],
            "cflags_cc": [
                "-std=c++11",
                "-Wl,--whole-archive",
                "-fPIC",
            ],
            "link_settings": {
                "libraries": [
                    "-Wl,-rpath,./build/Release/",
                    "-equi.so"
                ]
            },
        },
        {
            "target_name": "libequi",
            "type": "<(library)",
            "dependencies": [
            ],
            "sources": [
                "src/equi/equi.c",
                "src/equi/endian.c"
            ],
            "include_dirs": [
                "<(module_root_dir)src/equi/",
            ],
            "defines": [
                "_GNU_SOURCE"
            ],
            "cflags_c": [
                "-std=c11",
                "-Wl,--whole-archive",
                "-fPIC",
                "-Wno-pointer-sign",
            ],
            "link_settings": {
                "libraries": [
                    "-lsodium"
                ],
            },
        }
    ]
}

