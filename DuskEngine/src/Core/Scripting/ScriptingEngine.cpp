#include "pch.h"
#include "ScriptingEngine.h"

#include "LuaScript.h"

#include "Core/Macros/LOG.h"

#include "lua/lua.hpp"
#include "rttr/registration.h"

namespace DuskEngine
{
	lua_State* ScriptingEngine::m_LuaState = nullptr;

	int CallGlobalFromLua(lua_State* l);
	int CreateUserDatum(lua_State* l);

	void ScriptingEngine::Init()
	{
		m_LuaState = luaL_newstate();
		
		// Use Dusk to do pretty much everything, from maths to logging
		luaL_openlibs(m_LuaState); 

		RegisterFunctions();
	}

	void ScriptingEngine::Shutdown()
	{
		lua_close(m_LuaState);
	}

	int ScriptingEngine::LoadScript(const std::filesystem::path& path)
	{
		// probably useless, maybe should instead load text into memory as to not open file constantly

		// load script into text when scene is loaded, have a reload button in the editor for runtime editing
		//return luaL_loadfile(m_LuaState, path.string().c_str());
		return 0;
	}

	void ScriptingEngine::OnAwake(Ref<LuaScript>& script)
	{
		int r = luaL_dofile(m_LuaState, script->GetPath().string().c_str());

		if (r == LUA_OK)
		{
			//lua_getglobal(L, "sprite");
		}
		else
		{
			std::string error = lua_tostring(m_LuaState, -1);
			std::cout << error << std::endl;
		}
	}

	void ScriptingEngine::RegisterFunctions()
	{
		// Set globals
		lua_newtable(m_LuaState);
		lua_pushvalue(m_LuaState, -1);
		lua_setglobal(m_LuaState, "Dusk");

		lua_pushvalue(m_LuaState, -1);
		for(auto& method : rttr::type::get_global_methods())
		{
			lua_pushstring(m_LuaState, method.get_name().to_string().c_str());
			lua_pushlightuserdata(m_LuaState, (void*)&method);
			lua_pushcclosure(m_LuaState, DuskEngine::CallGlobalFromLua, 1);
			lua_settable(m_LuaState, -3);
			std::string msg = "Registered method " + method.get_name().to_string();
			LOG(msg.c_str())
		}

		for(auto& classToRegister : rttr::type::get_types())
		{
			//if(classToRegister.get_name().to_string() == "MyTestType")
			if(classToRegister.is_class())
			{
				LOG(classToRegister.get_name().to_string())

				lua_newtable(m_LuaState);
				lua_pushvalue(m_LuaState, -1);
				lua_setglobal(m_LuaState, classToRegister.get_name().to_string().c_str());
				
				lua_pushstring(m_LuaState, classToRegister.get_name().to_string().c_str());
				lua_pushcclosure(m_LuaState, DuskEngine::CreateUserDatum, 1);
				lua_setfield(m_LuaState, -2, "new");
			}
		}

	}

	int CallGlobalFromLua(lua_State* l)
	{
		rttr::method& method = *(rttr::method*)(lua_touserdata(l, lua_upvalueindex(1)));
		
		int numLuaArgs = lua_gettop(l);
		size_t numNativeArgs = method.get_parameter_infos().size();

		union PassByValue
		{
			int intValue;
			float floatValue;
			std::string stringValue;
			rttr::variant variantValue;
			
			PassByValue() { new(&variantValue) rttr::variant(); }
			~PassByValue() {}
		};

		if (numLuaArgs != numNativeArgs)
		{
			std::string message = "Wrong number of arguments. \
				Function Dusk." + method.get_name().to_string() + " takes " + std::to_string(numNativeArgs) + ", not " + std::to_string(numLuaArgs);
			ERROR(message.c_str())
		}

		std::vector<PassByValue> pbv(numNativeArgs);
		std::vector<rttr::argument> args(numNativeArgs);
		auto nativeAgrsIt = method.get_parameter_infos().begin();

		for (int i = 0; i < numNativeArgs; i++, nativeAgrsIt++)
		{
			const rttr::type nativeType = nativeAgrsIt->get_type();

			int luaArg = i + 1;
			int luaType = lua_type(l, luaArg);
			
			switch (luaType)
			{
			case LUA_TNUMBER:
				if (nativeType == rttr::type::get<int>())
				{
					pbv[i].intValue = (int)lua_tonumber(l, luaArg);
					args[i] = pbv[i].intValue;
				}
				if (nativeType == rttr::type::get<float>())
				{
					pbv[i].floatValue = (float)lua_tonumber(l, luaArg);
					args[i] = pbv[i].floatValue;
				}
				if (nativeType == rttr::type::get<rttr::variant>())
				{
					pbv[i].variantValue = (rttr::variant)lua_tonumber(l, luaArg);
					args[i] = pbv[i].variantValue;
				}
				break;
			case LUA_TSTRING:
				if (nativeType == rttr::type::get<std::string>())
				{
					pbv[i].stringValue = (std::string)lua_tostring(l, luaArg);
					args[i] = pbv[i].stringValue;
				}
				if (nativeType == rttr::type::get<rttr::variant>())
				{
					pbv[i].variantValue = (rttr::variant)lua_tostring(l, luaArg);
					args[i] = pbv[i].variantValue;
				}
				break;
			default:
				LOG("Receiving unknown type from lua")
					break;
			}			
		}

		rttr::variant nativeResult = method.invoke_variadic({}, args);
		int numberOfReturnValues = 0;
		if (!nativeResult.is_valid())
		{
			std::string s = "Unable to invoke " + method.get_name().to_string();
			LOG(s.c_str())
		}
		else if (!nativeResult.is_type<void>())
		{
			if (nativeResult.is_type<int>())
			{
				lua_pushnumber(l, nativeResult.get_value<int>());
				numberOfReturnValues++;
			}
			if (nativeResult.is_type<float>())
			{
				lua_pushnumber(l, nativeResult.get_value<float>());
				numberOfReturnValues++;
			}
		}
		return numberOfReturnValues;
	}

	int CreateUserDatum(lua_State* l)
	{
		LOG("Creating native type");

		const char* typeName = (const char*)lua_tostring(l, lua_upvalueindex(1));
		rttr::type typeToCreate = rttr::type::get_by_name(typeName);

		void* ud = lua_newuserdata(l, sizeof(rttr::variant));
		new (ud) rttr::variant(typeToCreate.create());

		lua_newtable(l);
		lua_setuservalue(l, 1);

		return 1;	//return the userdatum
	}
}


void HelloWorld()
{
	LOG("Hello from cpp");
}

void Log(const rttr::variant& var)
{
	if (var.can_convert<float>())
		LOG(var.convert<float>())
	else if(var.can_convert<char*>())
		LOG(var.convert<char*>())
	else
		LOG("Cant convert type");
}

int Return5()
{
	return 5;
}

struct MyTestType
{
	bool enabled;
	int age;
	//std::string string;

	MyTestType()
		:enabled(true), age(5)
	{
		printf("MyTestType Constructed\n"); 
	};

	~MyTestType()
	{
		printf("MyTestType is Dead\n");
	};
};

RTTR_REGISTRATION
{
	rttr::registration::method("HelloWorld", &HelloWorld);
	rttr::registration::method("Log", &Log);
	rttr::registration::method("Return5", &Return5);

	rttr::registration::class_<MyTestType>("MyTestType").constructor().
		property("enabled", &MyTestType::enabled).
		property("age", &MyTestType::age);
}