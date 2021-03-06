local Color = require("Lib/StdLib/ConsoleColor");
local Route = require("Lib/Toolkit/Route");

local Functions = {};

Functions.use = function(workspaceName)
    local parser = Core.Vili.DataParser.new();
    parser:parseFile("Workspace/Workspaces.vili", true);
    if (parser:root():contains(Core.Vili.AttributeType.ComplexAttribute, workspaceName)) then
        Core.Utils.File.copy("Workspace/" .. workspaceName .. "/Mount.vili", "Mount.vili");
        Color.print({ text = "Workspace '" .. workspaceName .. "' has  been successfully mounted !", color = {0, 255, 0}}, 1);
        Core.Path.MountPaths();
    else
        Color.print({ text = "Workspace '" .. workspaceName .. "' does not exists", color = {255, 0, 0}}, 1);
    end
end

Functions.create = function(workspaceName)
    local parser = Core.Vili.DataParser.new();
    parser:parseFile("Workspace/Workspaces.vili", true);
    os.execute(("mkdir Workspace/" .. workspaceName):gsub("/", Core.Utils.File.separator()));
    os.execute(("mkdir Workspace/" .. workspaceName .. "/Data"):gsub("/", Core.Utils.File.separator()));
    os.execute(("mkdir Workspace/" .. workspaceName .. "/Data/Maps"):gsub("/", Core.Utils.File.separator()));
    os.execute(("mkdir Workspace/" .. workspaceName .. "/Data/GameObjects"):gsub("/", Core.Utils.File.separator()));
    os.execute(("mkdir Workspace/" .. workspaceName .. "/Data/GameScripts"):gsub("/", Core.Utils.File.separator()));
    os.execute(("mkdir Workspace/" .. workspaceName .. "/Sprites"):gsub("/", Core.Utils.File.separator()));
    os.execute(("mkdir Workspace/" .. workspaceName .. "/Sprites/GameObjects"):gsub("/", Core.Utils.File.separator()));
    os.execute(("mkdir Workspace/" .. workspaceName .. "/Sprites/LevelSprites"):gsub("/", Core.Utils.File.separator()));
    parser:root():createComplexAttribute(workspaceName);
    parser:root():getPath(workspaceName):createBaseAttribute("path", "Workspace/" .. workspaceName);
    parser:writeFile("Workspace/Workspaces.vili", true);
    local defaultMount = io.open("Workspace/" .. workspaceName .. "/Mount.vili", "w")

    defaultMount:write("Include(Obe);\n\n");
    defaultMount:write("Mount:\n");
    defaultMount:write("    " .. workspaceName .. ":" .. "Workspace(\"" .. workspaceName .. "\", 1)\n");
    defaultMount:write("    Root:Path(\"\", 0)");
    defaultMount:close()

    Color.print({ text = "Workspace '" .. workspaceName .. "' has been successfully created", color = {0, 255, 0}}, 1);
end


return {
    Functions = Functions,
    Routes = {
        Route.Arg("create", {
            Route.Arg("workspaceName", Route.Types.Any, {
                Route.Call("create");
            });
        }),
        Route.Arg("use", {
            Route.Arg("workspaceName", Route.Types.Any, {
                Route.Call("use");
            });
        })
    }
};