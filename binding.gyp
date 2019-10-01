{
  "targets": [
    {
      "target_name": "discord_game",
      "sources": [
        "src/discord_game.c",
        "src/application.c"
      ],
      "include_dirs": [
        "include/",
        "sdk/"
      ],
      "conditions": [
        ["OS == 'win'", {
          "conditions": [
            ["target_arch == 'ia32'", {
              "link_settings": {
                "libraries": [
                  "<(module_root_dir)/sdk/x86/discord_game_sdk.dll.lib"
                ]
              }
            }],
            ["target_arch == 'x64'", {
              "link_settings": {
                "libraries": [
                  "<(module_root_dir)/sdk/x86_64/discord_game_sdk.dll.lib"
                ]
              }
            }]
          ]
        }],
        ["OS == 'mac'", {
          "link_settings": {
            "libraries": [
              "-Wl,-rpath,<(module_root_dir)/sdk/x86_64",
              "<(module_root_dir)/sdk/x86_64/discord_game_sdk.bundle"
            ]
          }
        }]
      ]
    }
  ]
}
