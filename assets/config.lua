-- local reader = require 'lua.reader'

local files = {
	"hello.txt",
	-- "musketeers.txt",
	-- "textures/grass.png",
	"textures/test.png",
	"shader/test.vert",
	"shader/test.frag",
	-- "file"
}

local config = {
	packages = {
		test_zlib = {
			name = "test",
			path = flexy.base_path .. "test",
			-- files = reader.list_files(flexy.base_path .. "test"),
			files = files,
			compression = {
				type = flexy.compression.zlib,
				level = 9
			}
		}
	},
	plugins = {
		default = {
			extensions = {},
			name = "plugin_default",
			default = true
		},
		png = {
			extensions = {"png"},
			name = "plugin_png"
		},
		shader = {
			extensions = {"frag", "vert"},
			name = "plugin_shader"
		}
	}
}

-- local debug = require 'lua.debug'
-- debug.tprint(config)
-- debug.tprint(flexy)

return config
