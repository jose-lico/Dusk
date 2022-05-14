print("hey print_script_2")
local x = Return5()
local teste = MyTestType.new(2) -- will get garbage collected


function OnAwake()
    print("2 is alive")
end

function OnUpdate()
    x = x + 2

    if(Input.IsKeyPressed(KeyCode.W)) then
        print("w pressed")
    end
end

function OnShutdown()
    print("x in print_script_2 " .. x)
end