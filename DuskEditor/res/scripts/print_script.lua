number = 69
--print("Heeeeeeeeeeeeeeeey")

HelloWorld()
print("hey")
print(Return5())
local x = Return5()
x = x * 2 - 1
print(x)

--dt = DeltaTime()
--print(dt)
--Log(x)
--Log(dt)
--Log("This is a string")

local teste = MyTestType.new()
print(teste.age)
--print(teste.age)
--local meta = Meta.new()

function OnAwake()
    print("I am alive")
end

function OnUpdate()
    print(number)
end