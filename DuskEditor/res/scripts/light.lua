local light

function OnAwake()
    light = entity:GetLight();
    print(light.color.x)
end

function OnUpdate()
    if (Input.IsKeyPressed(KeyCode.I)) then
        light.color = Vector3.new(1,0,0)
    end

    if (Input.IsKeyPressed(KeyCode.O)) then
        light.color = Vector3.new(0,1,0)
    end

    if (Input.IsKeyPressed(KeyCode.F)) then
        light.type = LightType.Point
    end

    if (Input.IsKeyPressed(KeyCode.G)) then
        light.type = LightType.Directional
    end
end