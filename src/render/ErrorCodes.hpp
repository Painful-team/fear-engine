#ifndef FEARENGINE_RENDER_ERRORCODES_H__
#define FEARENGINE_RENDER_ERRORCODES_H__

namespace FearEngine::Render
{
using errorCode = short;
namespace errorCodes
{
enum : errorCode
{
	OK = 0,
	UNIFORM_LOCATION_NOT_FOUND = 129,
	WINDOW_INITIALIZATION_FAILED = 130,
	SHADER_FILE_EMPTY = 131,
	SHADER_FILE_NOT_EXIST = 132,
	SHADER_COMPILATION_FAILED = 133,
	SHADER_LINKAGE_FAILED = 134
};
}
}  // namespace FearEngine::Render

#endif
