#include "pch.h"
#include "ScriptingEngine.h"

#include "LuaScript.h"

#include "Core/Application/Input.h"
#include "Core/Application/Time.h"
#include "Core/ECS/Components.h"
#include "Core/ECS/Entity.h"

#include "Utils/Profiling/Timer.h"

#include "glm/glm.hpp"
#include "entt/entt.hpp"

namespace DuskEngine
{
	ScriptingEngine::ScriptingEngine()
	{
		m_State.open_libraries(sol::lib::base);
        RegisterFunctions();
	}

    void ScriptingEngine::RegisterFunctions()
    {
        Timer timer("Register Functions");

        RegisterInput();
        RegisterTime();
        RegisterMath();
        RegisterECS();
    }

	void ScriptingEngine::LoadScript(LuaScript& script)
	{
		script.LoadScript(m_State);
	}

	void ScriptingEngine::OnAwake(LuaScript& script)
	{
		script.OnAwake();
	}

	void ScriptingEngine::OnUpdate(LuaScript& script)
	{
		script.OnUpdate();
	}

	void ScriptingEngine::OnShutdown(LuaScript& script)
	{
		script.OnShutdown();
	}

	void ScriptingEngine::RegisterInput()
	{
		sol::table input = m_State["Input"].get_or_create<sol::table>();

        input.set_function("IsKeyPressed", &Input::IsKeyPressed);
        input.set_function("IsMouseButtonPressed", &Input::IsMouseButtonPressed);

        std::initializer_list<std::pair<sol::string_view, KeyCode>> keyCodes = {
            { "UNKOWN", Key::UNKNOWN},
            { "Space", Key::SPACE },
            { "Apostrophe", Key::APOSTROPHE},
            { "Comma", Key::COMMA},
            { "Minus", Key::MINUS},
            { "Period", Key::PERIOD},
            { "Slash", Key::SLASH},
            { "D0", Key::D0},
            { "D1", Key::D1},
            { "D2", Key::D2},
            { "D3", Key::D3},
            { "D4", Key::D4},
            { "D5", Key::D5},
            { "D6", Key::D6},
            { "D7", Key::D7},
            { "D8", Key::D8},
            { "D9", Key::D9},
            { "Semicolon", Key::SEMICOLON},
            { "Equal", Key::EQUAL},
            { "A", Key::A },
            { "B", Key::B },
            { "C", Key::C },
            { "D", Key::D },
            { "E", Key::E },
            { "F", Key::F },
            { "H", Key::H },
            { "G", Key::G },
            { "I", Key::I },
            { "J", Key::J },
            { "K", Key::K },
            { "L", Key::L },
            { "M", Key::M },
            { "N", Key::N },
            { "O", Key::O },
            { "P", Key::P },
            { "Q", Key::Q },
            { "R", Key::R },
            { "S", Key::S },
            { "T", Key::T },
            { "U", Key::U },
            { "V", Key::V },
            { "W", Key::W },
            { "X", Key::X },
            { "Y", Key::Y },
            { "Z", Key::Z },
            { "LeftBracket", Key::LEFT_BRACKET },
            { "Backslash", Key::BACKSLASH },
            { "RightBracket", Key::RIGHT_BRACKET },
            { "GraveAccent", Key::GRAVE_ACCENT },
            { "World_1", Key::WORLD_1 },
            { "World_2", Key::WORLD_2 },
            { "Escape", Key::ESCAPE },
            { "Enter", Key::ENTER },
            { "Tab", Key::TAB },
            { "Backspace", Key::BACKSPACE },
            { "Insert", Key::INSERT },
            //{ "Delete", Key::DELETE },
            { "Right", Key::RIGHT },
            { "Left", Key::LEFT },
            { "Down", Key::DOWN },
            { "Up", Key::UP },
            { "PageUp", Key::PAGE_UP },
            { "PageDown", Key::PAGE_DOWN },
            { "Home",Key::HOME },
            { "End", Key::END },
            { "CapsLock", Key::CAPS_LOCK },
            { "ScrollLock", Key::SCROLL_LOCK },
            { "NumLock", Key::NUM_LOCK},
            { "PrintScreen", Key::PRINT_SCREEN },
            { "Pause", Key::PAUSE },
            { "F1", Key::F1 },
            { "F2", Key::F2 },
            { "F3", Key::F3 },
            { "F4", Key::F4 },
            { "F5", Key::F5 },
            { "F6", Key::F6 },
            { "F7", Key::F7 },
            { "F8", Key::F8 },
            { "F9", Key::F9 },
            { "F10", Key::F10 },
            { "F11", Key::F11 },
            { "F12", Key::F12 },
            { "Keypad0", Key::KP_0 },
            { "Keypad1", Key::KP_1 },
            { "Keypad2", Key::KP_2 },
            { "Keypad3", Key::KP_3 },
            { "Keypad4", Key::KP_4 },
            { "Keypad5", Key::KP_5 },
            { "Keypad6", Key::KP_6 },
            { "Keypad7", Key::KP_7 },
            { "Keypad8", Key::KP_8 },
            { "Keypad9", Key::KP_9 },
            { "KeypadDecimal", Key::KP_DECIMAL },
            { "KeypadDivide", Key::KP_DIVIDE },
            { "KeypadMultiply", Key::KP_MULTIPLY },
            { "KeypadSubtract", Key::KP_SUBTRACT },
            { "KeypadAdd", Key::KP_ADD },
            { "KeypadEnter", Key::KP_ENTER },
            { "KeypadEqual", Key::KP_EQUAL },
            { "LeftShift", Key::LEFT_SHIFT },
            { "LeftControl", Key::LEFT_CONTROL },
            { "LeftAlt", Key::LEFT_ALT },
            { "LeftSuper", Key::LEFT_SUPER },
            { "RightShift", Key::RIGHT_SHIFT },
            { "RightControl", Key::RIGHT_CONTROL },
            { "RightAlt", Key::RIGHT_ALT },
            { "RightSuper", Key::RIGHT_SUPER },
            { "Menu", Key::MENU }
        };

		m_State.new_enum<KeyCode>("KeyCode", keyCodes);

        std::initializer_list<std::pair<sol::string_view, MouseCode>> mouseCodes = {
            { "Left", Mouse::MOUSE_BUTTON_1 },
            { "Right", Mouse::MOUSE_BUTTON_2 },
            { "Middle", Mouse::MOUSE_BUTTON_3 },
        };

        m_State.new_enum<MouseCode>("MouseCode", mouseCodes);
	}

    void ScriptingEngine::RegisterTime()
    {
        sol::table time = m_State["Time"].get_or_create<sol::table>();

        time.set("GetDeltaTime", sol::property(&Time::GetDeltaTime));
        time.set("GetTime", sol::property(&Time::GetTime));
        time.set("GetUnscaledDeltaTime", sol::property(&Time::GetUnscaledDeltaTime));
        time.set("GetUnscaledTime", sol::property(&Time::GetUnscaledTime));
        time.set("GetTimescale", sol::property(&Time::GetTimescale));
        time.set("SetTimescale", sol::property(&Time::SetTimescale));
    }

    void ScriptingEngine::RegisterMath()
    {
        m_State.new_usertype<glm::vec2>(
            "Vector2",
            sol::constructors<glm::vec2(float, float)>(),
            "x", &glm::vec2::x,
            "y", &glm::vec2::y,
            sol::meta_function::addition, [](const glm::vec2& a, const glm::vec2& b)
            { return a + b; },
            sol::meta_function::multiplication, [](const glm::vec2& a, const glm::vec2& b)
            { return a * b; },
            sol::meta_function::subtraction, [](const glm::vec2& a, const glm::vec2& b)
            { return a - b; },
            sol::meta_function::division, [](const glm::vec2& a, const glm::vec2& b)
            { return a / b; },
            sol::meta_function::equal_to, [](const glm::vec2& a, const glm::vec2& b)
            { return a == b; });

        auto mult_overloads = sol::overload(
            [](const glm::vec3& v1, const glm::vec3& v2) -> glm::vec3
            { return v1 * v2; },
            [](const glm::vec3& v1, float f) -> glm::vec3
            { return v1 * f; },
            [](float f, const glm::vec3& v1) -> glm::vec3
            { return f * v1; });

        m_State.new_usertype<glm::vec3>(
            "Vector3",
            sol::constructors<sol::types<>, sol::types<float, float, float>>(),
            "x", &glm::vec3::x,
            "y", &glm::vec3::y,
            "z", &glm::vec3::z,
            "Length", &glm::vec3::length,
            sol::meta_function::addition, [](const glm::vec3& a, const glm::vec3& b)
            { return a + b; },
            sol::meta_function::multiplication, mult_overloads,
            sol::meta_function::subtraction, [](const glm::vec3& a, const glm::vec3& b)
            { return a - b; },
            sol::meta_function::unary_minus, [](glm::vec3& v) -> glm::vec3
            { return -v; },
            sol::meta_function::division, [](const glm::vec3& a, const glm::vec3& b)
            { return a / b; },
            sol::meta_function::equal_to, [](const glm::vec3& a, const glm::vec3& b)
            { return a == b; },
            "Normalize", [](glm::vec3& v)
            { return glm::normalize(v); });
    }
    
    void ScriptingEngine::RegisterECS()
    {
        sol::usertype<LuaScript> scriptType = m_State.new_usertype<LuaScript>("LuaScript",
            sol::constructors<sol::types<std::filesystem::path&, const uuids::uuid&>>());

        sol::usertype<Entity> entityType = m_State.new_usertype<Entity>("Entity", sol::constructors<sol::types<entt::entity, Scene*>>());
        
        // #############################################################
        // TRANSFORM
        // #############################################################

        m_State.new_usertype<Transform>("Transform",
            sol::constructors<Transform()>(),
            "position", &Transform::position,
            "rotation", &Transform::rotation,
            "scale", &Transform::scale,
            "front", &Transform::front,
            "up", &Transform::up, 
            "right", &Transform::right);

        entityType.set_function("GetTransform", &Entity::GetComponent<Transform>);

        // #############################################################
        // LIGHT
        // #############################################################

        m_State.new_usertype<Light>("Light",
            sol::constructors<Light()>(),
            "color", &Light::color,
            "type", &Light::type);

        std::initializer_list<std::pair<sol::string_view, LightType>> lightTypes = {
            { "Directional", LightType::Directional },
            { "Point", LightType::Point },
            { "Spot", LightType::Spot },
        };

        m_State.new_enum<LightType>("LightType", lightTypes);

        entityType.set_function("GetLight", &Entity::GetComponent<Light>);
    }
}

void HelloWorld()
{
	DuskEngine::LOG("Hello from cpp");
}

int Return5()
{
	return 5;
}