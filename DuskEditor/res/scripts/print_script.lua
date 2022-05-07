number = 69
print("Heeeeeeeeeeeeeeeey")

Dusk.HelloWorld()
x = Dusk.Return5()
x = x * 2 - 1
dt = Dusk.DeltaTime()
Dusk.Log(x)
Dusk.Log(dt)
Dusk.Log("This is a string")

local teste = MyTestType.new()
--local meta = Meta.new()

function OnAwake()
    print("I am alive")
end

function OnUpdate()
    print(number)
end