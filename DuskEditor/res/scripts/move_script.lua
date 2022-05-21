print("hey print_script_2")
HelloWorld()
local x = Return5()
print(x)

local object = MyTestType.new(1)
local transform
local speed = 5

function OnAwake()
    print("Awake")
    print(object.age) 
    object:Func()
    print(object.age)
    
    transform = LuaComponent:GetEntity():GetTransform();
end

function OnUpdate()
    local input = Vector2.new(0,0)
    
    if (Input.IsKeyPressed(KeyCode.D)) then input.x = 1
    elseif (Input.IsKeyPressed(KeyCode.A)) then input.x = -1 end

    if (Input.IsKeyPressed(KeyCode.W)) then input.y = 1
    elseif (Input.IsKeyPressed(KeyCode.S)) then input.y = -1 end

    local movement = Vector3.new(input.x, 0, input.y)
    
    transform.position = transform.position + movement * speed * Time.GetDeltaTime()
end

function OnShutdown()
    print("Dead")
end