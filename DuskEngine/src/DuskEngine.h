#pragma once

#include "Core/Application/Application.h"
#include "Core/Application/Layer.h"
#include "Core/Events/Event.h"

#include "Core/Renderer/Resources/Mesh.h"
#include "Core/Renderer/Resources/Shader.h"
#include "Core/Renderer/Resources/Material.h"
#include "Core/Renderer/Resources/Texture.h"
#include "Core/Renderer/Resources/Framebuffer.h"
#include "Core/Renderer/Resources/Model.h"

#include "Core/Macros/LOG.h"
#include "Core/Application/Input.h"
#include "Utils/Window/WindowManager.h"
#include "Core/Application/Time.h"

#include "Utils/Memory/Memory.h"
#include "Utils/Rendering/PrimitiveMesh.h"

#include "Core/ECS/Scene.h"
#include "Core/ECS/Entity.h"
#include "Core/ECS/Components.h"