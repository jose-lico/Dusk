number = 69
print("Heeeeeeeeeeeeeeeey")
Dusk.HelloWorld()
x = Dusk.Return5()
x = x * 2 - 1
dt = Dusk.DeltaTime()
--print(x)
Dusk.Log(x)
--print(dt)
Dusk.Log(dt)
Dusk.Log("This is a string")

function OnAwake()
    print("I am alive")
end

function OnUpdate()
    print(number)
end