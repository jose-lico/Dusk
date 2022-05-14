#include "pch.h"
#include "ScriptingEngine.h"

#include "LuaScript.h"

#include "Core/Macros/LOG.h"
#include "Utils/Profiling/Timer.h"

#include "rttr/registration.h"

void HelloWorld();
void Log(const rttr::variant& var);
int Return5();

struct MyTestType
{
	int age;
	bool enabled;
	//std::string string;

	int id;

	MyTestType(int i)
		:enabled(true), age(5), id(i)
	{
		printf("MyTestType Constructed\n");
	};

	~MyTestType()
	{
		printf("MyTestType is Dead:%i\n", id);
	};
};

namespace DuskEngine
{
	ScriptingEngine::ScriptingEngine()
	{
		//m_State = sol::state();
		//m_State = new sol::state();
		m_State.open_libraries(sol::lib::base);
		RegisterFunctions();
	}

	ScriptingEngine::~ScriptingEngine()
	{
		//delete(m_State);
	}


	int CallGlobalFromLua(lua_State* l);
	int CreateUserDatum(lua_State* l);
	int DestroyUserDatum(lua_State* l); 
	std::string MetaTableName(const rttr::type& t);

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
		Timer timer("Register Funcs");

		m_State.set_function("Log", &Log);
		m_State.set_function("HelloWorld", &HelloWorld);
		m_State.set_function("Return5", &Return5);
		m_State.new_usertype<MyTestType>("MyTestType",
			sol::constructors<MyTestType(int)>(),
			"age", &MyTestType::age, 
			"enabled", &MyTestType::enabled);

		//for(auto& method : rttr::type::get_global_methods())
		//{
		//	/*lua_pushstring(m_LuaState, method.get_name().to_string().c_str());
		//	lua_pushlightuserdata(m_LuaState, (void*)&method);
		//	lua_pushcclosure(m_LuaState, DuskEngine::CallGlobalFromLua, 1);
		//	lua_settable(m_LuaState, -3);*/
		//	m_State.set_function(method.get_name().to_string().c_str(), &method);
		//	std::string msg = "Registered method " + method.get_name().to_string();
		//	LOG(msg.c_str())
		//}

		//for(auto& classToRegister : rttr::type::get_types())
		//{
		//	if(classToRegister.get_name().to_string() == "MyTestType")
		//	//if(classToRegister.is_class())
		//	{
		//		LOG(classToRegister.get_name().to_string())

		//		lua_newtable(m_LuaState);
		//		lua_pushvalue(m_LuaState, -1);
		//		lua_setglobal(m_LuaState, classToRegister.get_name().to_string().c_str());
		//		lua_pushvalue(m_LuaState, -1);

		//		lua_pushstring(m_LuaState, classToRegister.get_name().to_string().c_str());
		//		lua_pushcclosure(m_LuaState, DuskEngine::CreateUserDatum, 1);
		//		lua_setfield(m_LuaState, -2, "new");

		//		// destructor
		//		std::string metaTableName = "_mt_" + classToRegister.get_name().to_string();
		//		//LOG(metaTableName.c_str())
		//		luaL_newmetatable(m_LuaState, metaTableName.c_str());
		//		lua_pushstring(m_LuaState, "__gc");
		//		lua_pushcclosure(m_LuaState, DuskEngine::DestroyUserDatum, 0);
		//		lua_settable(m_LuaState, -3);
		//	}
		//}
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

		std::string metaTableName = "_mt_";
		metaTableName.append(typeName);
		LOG(metaTableName.c_str())
		luaL_getmetatable(l, metaTableName.c_str());
		lua_setmetatable(l, 1);

		lua_newtable(l);
		lua_setuservalue(l, 1);

		return 1;	//return the userdatum
	}

	int DestroyUserDatum(lua_State* l)
	{
		LOG("Destroying native type")
		rttr::variant* ud = (rttr::variant*)lua_touserdata(l, -1);
		ud->~variant();
		return 0;
	}

	std::string MetaTableName(const rttr::type& t)
	{
		std::string metaTableName;
		if (t.is_pointer())
		{
			metaTableName = t.get_raw_type().get_name().to_string();
		}
		else
		{
			metaTableName = t.get_name().to_string();
		}
		metaTableName.append("_MT_");
		return metaTableName;
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

//RTTR_REGISTRATION
//{
//	rttr::registration::method("HelloWorld", &HelloWorld);
//	rttr::registration::method("Log", &Log);
//	rttr::registration::method("Return5", &Return5);
//
//	rttr::registration::class_<MyTestType>("MyTestType").constructor().
//		property("enabled", &MyTestType::enabled).
//		property("age", &MyTestType::age);
//}