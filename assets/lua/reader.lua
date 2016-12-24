local lfs = require 'lfs'

local reader = {}

function reader.list_files(path, base_path, t)
	if t == nil then t = {} end
	if base_path == nil then base_path = path end
    for file in lfs.dir(path) do
        if file ~= "." and file ~= ".." then
            local f = path..'/'..file
            local attr = lfs.attributes (f)
            assert (type(attr) == "table")
            if attr.mode == "directory" then
                reader.list_files (f, base_path, t)
			else
				table.insert(t, f:sub(base_path:len()+2))
            end
        end
    end
	return t
end

return reader
