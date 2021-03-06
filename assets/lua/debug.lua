local debug = {}

-- Just for debugging
function debug.tprint (tbl, indent)
  if not indent then indent = 0 end
  for k, v in pairs(tbl) do
	formatting = string.rep("  ", indent) .. k .. ": "
	if type(v) == "table" then
	  print(formatting)
	  debug.tprint(v, indent+1)
	elseif type(v) == 'boolean' then
	  print(formatting .. tostring(v))      
	else
	  print(formatting .. v)
	end
  end
end

return debug
