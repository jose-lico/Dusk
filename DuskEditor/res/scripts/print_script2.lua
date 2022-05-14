print("hey print_script_2")
local x = Return5()
local teste = MyTestType.new(2)


function OnAwake()
    print("2 is alive")
end

function OnUpdate()
    --teste.age = teste.age + 2
    x = x + 2
end

function OnShutdown()
    print("x in print_script_2 " .. x)
    --print("teste.age in print_script_2 " .. teste.age)
end