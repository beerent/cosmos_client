local targetPropertyName = "x"
local ownerPropertyName = "x"

function setup()
	print("setup")
	targetPropertyName = "LOOK_AT_TARGET"
	ownerPropertyName = "OWNER_NAME"
	print("string value " .. ownerPropertyName)
end

function onTick()
	print("onTick " .. targetPropertyName)
	local targetName = LuaCallBack("GetNPCProperty", targetPropertyName);
	local ownerName = LuaCallBack("GetNPCProperty", ownerPropertyName);

	print("targetName " .. targetName .. " ownerName " .. ownerName)
end
