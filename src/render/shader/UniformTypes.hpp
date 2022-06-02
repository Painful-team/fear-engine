#ifndef FEARENGINE_RENDER_SHADER_UNIFORMTYPES_H__
#define FEARENGINE_RENDER_SHADER_UNIFORMTYPES_H__

namespace FearEngine::Render::Shaders
{
using uniformType = uint16_t;
namespace uniformTypes
{
enum : uniformType
{
	INT = 0x1404,
	FLOAT = 0x1406,
	VEC2 = 0x8B50,
	VEC3 = 0x8B51,
	VEC4 = 0x8B52,
	MAT2 = 0x8B5A,
	MAT3 = 0x8B5B,
	MAT4 = 0x8B5C
};
}
}  // namespace FearEngine::Render::Shaders

#endif
