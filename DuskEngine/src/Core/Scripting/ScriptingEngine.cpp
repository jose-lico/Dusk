#include "pch.h"
#include "ScriptingEngine.h"

#include "LuaScript.h"

#include "Core/Macros/LOG.h"
#include "Core/Application/Input.h"
#include "Core/Application/Time.h"

#include "Utils/Profiling/Timer.h"

void HelloWorld();
int Return5();

struct MyTestType
{
	int age;
	bool enabled;

	int id;

	MyTestType(int i)
		:enabled(true), age(5), id(i)
	{
		printf("MyTestType Constructed\n");
	};

	~MyTestType()
	{
		printf("MyTestType is Dead: %i\n", id);
	};
};

namespace DuskEngine
{
	ScriptingEngine::ScriptingEngine()
	{
		m_State.open_libraries(sol::lib::base);
		RegisterFunctions();
	}

	void ScriptingEngine::LoadScript(Ref<LuaScript>& script)
	{
		script->LoadScript(m_State);
	}

	void ScriptingEngine::OnAwake(Ref<LuaScript>& script)
	{
		script->OnAwake();
	}

	void ScriptingEngine::OnUpdate(Ref<LuaScript>& script)
	{
		script->OnUpdate();
	}

	void ScriptingEngine::OnShutdown(Ref<LuaScript>& script)
	{
		script->OnShutdown();
	}

	void ScriptingEngine::RegisterFunctions()
	{
		Timer timer("Register Functions");

		m_State.set_function("HelloWorld", &HelloWorld);
		m_State.set_function("Return5", &Return5);

		m_State.new_usertype<MyTestType>("MyTestType",
			sol::constructors<MyTestType(int)>(),
			"age", &MyTestType::age, 
			"enabled", &MyTestType::enabled);


        RegisterInput();
        RegisterTime();
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
            { "H", Key::G },
            { "G", Key::H },
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

        time.set_function("GetDeltaTime", &Time::GetDeltaTime);
    }
}

void HelloWorld()
{
	LOG("Hello from cpp");
}

int Return5()
{
	return 5;
}