#include "Core/ECS/Entity.h"
#include "Core/Macros/LOG.h"
#include "Core/Application/Time.h"

namespace DuskEngine
{
	struct ScriptBase;

	struct Script
	{
		ScriptBase* script = nullptr;

		Script(ScriptBase* s): script(s){}
	};

	struct ScriptBase
	{
		template<typename T>
		T& GetComponent()
		{
			return entity->GetComponent<T>();
		}

		ScriptBase(Entity* ent)
			: entity(ent)
		{
			if (!ent->HasComponent<Script>())
				ent->AddComponent<Script>(this);
		};

		virtual void OnCreate() {}
		virtual void OnUpdate() {}
		virtual void OnDestroy() {}

		Entity* entity;
	};


	struct TestScript : ScriptBase
	{
		using ScriptBase::ScriptBase;

		virtual void OnUpdate() override
		{
			std::cout << "This is On Update" << std::endl;
			auto& t = GetComponent<Transform>();
			t.position.y += 1.0f * Time::GetDeltaTime();
		}
	};
}