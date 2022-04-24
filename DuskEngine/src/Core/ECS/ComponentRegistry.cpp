#include "pch.h"

#include "Components.h"

#include "rttr/registration.h"

enum class MetaData_Type
{
	COMPONENT,
	OTHER
};

// #############################################################
// META
// #############################################################

#define REGISTER_META\
	rttr::registration::class_<DuskEngine::Meta>("Meta")\
		(rttr::metadata(MetaData_Type::COMPONENT, true)).\
		property("name", &DuskEngine::Meta::name).\
		property("enabled", &DuskEngine::Meta::enabled);

// #############################################################
// TRANSFORM
// #############################################################

#define REGISTER_TRANSFORM\
	rttr::registration::class_<DuskEngine::Transform>("Transform")\
		(rttr::metadata(MetaData_Type::COMPONENT, true)).\
		property("position", &DuskEngine::Transform::position).\
		property("rotation", &DuskEngine::Transform::rotation).\
		property("scale", &DuskEngine::Transform::scale);

// #############################################################
// MESH RENDERER
// #############################################################

#define REGISTER_MESH_RENDERER\
	rttr::registration::class_<DuskEngine::MeshRenderer>("MeshRenderer")\
		(rttr::metadata(MetaData_Type::COMPONENT, true)).\
		property("mesh", &DuskEngine::MeshRenderer::mesh).\
		property("material", &DuskEngine::MeshRenderer::material);\

// #############################################################
// CAMERA
// #############################################################

#define REGISTER_CAMERA\
	rttr::registration::class_<DuskEngine::Camera>("Camera")\
		(rttr::metadata(MetaData_Type::COMPONENT, true)).\
		property("main", &DuskEngine::Camera::main);

// #############################################################
// LIGHT
// #############################################################

#define REGISTER_LIGHT\
	rttr::registration::class_<DuskEngine::Light>("Light")\
		(rttr::metadata(MetaData_Type::COMPONENT, true)).\
		property("type", &DuskEngine::Light::type).\
		property("color", &DuskEngine::Light::color);

// #############################################################
// SCRIPT
// #############################################################

#define REGISTER_SCRIPT\
	rttr::registration::class_<DuskEngine::Script>("Script")\
		(rttr::metadata(MetaData_Type::COMPONENT, true)).\
		property("scripts", &DuskEngine::Script::scripts);

// #############################################################
// RTTR REGISTRATION
// #############################################################

RTTR_REGISTRATION
{
	REGISTER_META
	REGISTER_TRANSFORM
	REGISTER_MESH_RENDERER
	REGISTER_CAMERA
	REGISTER_LIGHT
	REGISTER_SCRIPT
}