local transform
local speed = 3
local slowMode = false

function OnAwake()
    print("Awake")

    transform = entity:GetTransform();
end

function OnUpdate()
    -- need to add glfw bind for KeyPress (not just IsKeyPressed)
    if(Input.IsKeyPressed(KeyCode.P)) then
        if(slowMode) then
            Time.SetTimescale(1)
            slowMode = false
        else
            Time.SetTimescale(0.5)
            slowMode = true
        end
    end

    local input = Vector2.new(0,0)
    local front = transform.front
    
    if (Input.IsKeyPressed(KeyCode.D)) then input.x = -1
    elseif (Input.IsKeyPressed(KeyCode.A)) then input.x = 1 end

    if (Input.IsKeyPressed(KeyCode.W)) then input.y = 1
    elseif (Input.IsKeyPressed(KeyCode.S)) then input.y = -1 end

    local movement = Vector3.new(input.x, 0, input.y)
    
    transform.position = transform.position + movement * speed * Time.GetDeltaTime()
end

function OnShutdown()
    print("Dead")
end