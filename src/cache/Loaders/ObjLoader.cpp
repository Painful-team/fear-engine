#include "ObjLoader.hpp"

#include <glm/glm.hpp>
#include <glm/gtx/norm.hpp>

#include <vector>

#include <tinyobj_loader_opt.h>

#include <cache/ObjResource.hpp>
#include <utils/PointerCasts.hpp>

#include <core/Engine.hpp>

FearEngine::Cache::errorCode FearEngine::Cache::Loaders::ObjLoader::init()
{
	Engine::logs()->log("Cache", "{0} loader initialization has begun.", getPattern());

	Engine::logs()->log("Cache", "{0} loader initialization has ended successfully.", getPattern());
	return errorCodes::OK;
}

std::string FearEngine::Cache::Loaders::ObjLoader::getPattern() const { return "*.obj"; }

static float angle(glm::vec3& v1, glm::vec3& v2)
{
	float dt = dot(v1, v2);
	float v1Len = length(v1);
	float v2Len = length(v2);
	float lenMul = v1Len * v2Len;
	float div = dt / lenMul;

	return cos(div);
}

void CalcNormal(glm::vec3 normal[3], glm::vec3 v[3])
{
	auto v10 = v[1] - v[0];
	auto v20 = v[2] - v[0];
	auto cross = glm::cross(v10, v20);

	auto v21 = v[2] - v[1];
	auto v01 = v[0] - v[1];
	auto v02 = v[0] - v[2];
	auto v12 = v[1] - v[2];

	auto a0 = angle(v10, v20);
	auto a1 = angle(v21, v02);
	auto a2 = angle(v02, v12);

	auto c0 = cross * a0;
	auto c1 = cross * a1;
	auto c2 = cross * a2;
	auto hitWSum = c0 + c1 + c2;

	auto hitWnorm = glm::normalize(hitWSum);

	normal[0] = hitWnorm;
	normal[1] = normal[0];
	normal[2] = normal[0];
}

// Todo add destruction ability to be able to close file
FearEngine::Cache::errorCode get_file_data(size_t& len, const char* filename, const char*& data)
{
	size_t data_len = 0;

	len = 0;

	HANDLE file = CreateFileA(
		 filename, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN, NULL);
	if (file == INVALID_HANDLE_VALUE)
	{
#ifdef _DEBUG
		LPSTR messageBuffer = nullptr;

		size_t size = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL,
			 GetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&messageBuffer, 0, NULL);

		// To remove \n\r from the end of the message
		messageBuffer[size - 2] = '\0';

		FearEngine::Engine::logs()->error("Cache", "Loading of {0} has failed with error \"{1}\".", filename, messageBuffer);

		LocalFree(messageBuffer);
#endif	// _DEBUG

		return FearEngine::Cache::errorCodes::FILE_OPEN_FAILED;
	}

	HANDLE fileMapping = CreateFileMapping(file, NULL, PAGE_READONLY, 0, 0, NULL);
	if (fileMapping == INVALID_HANDLE_VALUE)
	{
		CloseHandle(file);

#ifdef _DEBUG
		LPSTR messageBuffer = nullptr;

		size_t size = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL,
			 GetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&messageBuffer, 0, NULL);

		// To remove \n\r from the end of the message
		messageBuffer[size - 2] = '\0';

		FearEngine::Engine::logs()->error("Cache", "Loading of {0} has failed with error \"{1}\".", filename, messageBuffer);

		LocalFree(messageBuffer);
#endif	// _DEBUG

		return FearEngine::Cache::errorCodes::FILEMAPPING_FAILED;
	}

	LPVOID fileMapView = MapViewOfFile(fileMapping, FILE_MAP_READ, 0, 0, 0);
	if (fileMapView == nullptr)
	{
		CloseHandle(file);
		CloseHandle(fileMapping);

#ifdef _DEBUG
		LPSTR messageBuffer = nullptr;

		size_t size = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL,
			 GetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&messageBuffer, 0, NULL);

		// To remove \n\r from the end of the message
		messageBuffer[size - 2] = '\0';

		FearEngine::Engine::logs()->error("Cache", "Loading of {0} has failed with error \"{1}\".", filename, messageBuffer);

		LocalFree(messageBuffer);
#endif	// _DEBUG

		return FearEngine::Cache::errorCodes::MAPPEDFILEVIEW_FAILED;
	}

	data = reinterpret_cast<const char*>(fileMapView);

	LARGE_INTEGER fileSize;
	fileSize.QuadPart = 0;
	GetFileSizeEx(file, &fileSize);

	len = static_cast<size_t>(fileSize.QuadPart);

	return FearEngine::Cache::errorCodes::OK;
}

FearEngine::Cache::errorCode FearEngine::Cache::Loaders::ObjLoader::load(const std::string_view& filename,
	 std::shared_ptr<Resource>& resource,
	 ResourceFlags flags)
{
	Engine::logs()->log("Cache", "Loading of {0} with flags {1} has begun.", filename, flags);

	tinyobj_opt::attrib_t attrib;

	auto load_t_begin = std::chrono::high_resolution_clock::now();
	size_t data_len = 0;
	const char* data = nullptr;
	auto result = get_file_data(data_len, filename.data(), data);
	if (result != errorCodes::OK)
	{
		Engine::logs()->error("Cache", "Loading of {0} with flags {1} has failed.", filename, flags);
		return result;
	}

	tinyobj_opt::LoadOption option;
	option.req_num_threads = ObjLoader::Threads;

	auto extra = std::make_shared<ObjData>();
	std::vector<tinyobj_opt::shape_t> shapes;
	std::vector<tinyobj_opt::material_t> materials;

	option.verbose = false;
	bool ret = parseObj(&attrib, &shapes, &materials, data, data_len, option);
	if (!ret)
	{
		Engine::logs()->error(
			 "Cache", "Loading of {0} with flags {1} has failed with error \"Failed to parse obj file.\".", filename, flags);
		return errorCodes::OBJECT_PARSE_FAILED;
	}

	constexpr const uint8_t posVertices = 3;
	constexpr const uint8_t normals = 3;
	constexpr const uint8_t colors = 3;
	constexpr const uint8_t texCoords = 2;
	constexpr const uint8_t vertices = 3;

	auto& vb = extra->vertices;	 // pos(3float), normal(3float), color(3float), texCoord(2float)
	vb.reserve(attrib.face_num_verts.size() * vertices * ObjData::VertexSize);
	for (size_t v = 0, face_offset = 0; v < attrib.face_num_verts.size(); ++v)
	{
		assert(attrib.face_num_verts[v] % vertices == 0);  // assume all triangle face(multiple of 3).
		for (size_t f = 0; f < attrib.face_num_verts[v] / vertices; ++f)
		{
			tinyobj_opt::index_t* idx0 = attrib.indices.data() + face_offset + vertices * f;

			glm::vec3 v[vertices];
			for (uint8_t k = 0; k < vertices; ++k)
			{
				v[k] = *reinterpret_cast<glm::vec3*>(&attrib.vertices[posVertices * (idx0 + k)->vertex_index]);
			}

			glm::vec2 vt[vertices];
			for (uint8_t k = 0; k < vertices; ++k)
			{
				vt[k] = *reinterpret_cast<glm::vec2*>(&attrib.texcoords[texCoords * (idx0 + k)->texcoord_index]);
			}

			glm::vec3 n[vertices];
			if (attrib.normals.size() > 0)
			{
				if (idx0->normal_index >= 0 && (idx0 + 1)->normal_index >= 0 && (idx0 + 2)->normal_index >= 0)
				{
					if (flags & ResourceFlag::RecalcNormals)
					{
						CalcNormal(n, v);
					}
					else
					{
						for (uint8_t k = 0; k < vertices; ++k)
						{
							n[k] = *reinterpret_cast<glm::vec3*>(&attrib.normals[normals * (idx0 + k)->normal_index]);
						}
					}
				}
				else
				{
					CalcNormal(n, v);
				}
			}
			else
			{
				CalcNormal(n, v);
			}

			for (uint8_t k = 0; k < vertices; ++k)
			{
				vb.emplace_back(v[k].x);
				vb.emplace_back(v[k].y);
				vb.emplace_back(v[k].z);

				vb.emplace_back(n[k].x);
				vb.emplace_back(n[k].y);
				vb.emplace_back(n[k].z);
				// Use normal as color.
				glm::vec3 c = n[0];
				float len2 = glm::length2(c);
				if (len2 > 1.0e-6f)
				{
					float len = sqrtf(len2);

					c /= len;
				}

				c *= 0.5;
				c += 0.5;

				vb.emplace_back(c.x);
				vb.emplace_back(c.y);
				vb.emplace_back(c.z);

				vb.emplace_back(vt[k].x);
				vb.emplace_back(vt[k].y);
			}
		}

		face_offset += attrib.face_num_verts[v];
	}

	resource = std::make_shared<Cache::Resource>();
	resource->filename = filename.data();
	resource->flags = flags;

	errorCode materialLoadResult = errorCodes::OK;
	std::vector<std::shared_ptr<Material>> materialRefs;
	materialRefs.resize(materials.size());
	for (uint8_t i = 0; i < materials.size(); ++i)
	{
		errorCode result = Material::create(materials[i], materialRefs[i]);
		if (result != errorCodes::OK)
		{
			materialLoadResult = result;
		}
	}

	if (materialLoadResult == errorCodes::OK)
	{
		Engine::logs()->log("Cache", "Loading of {0} with flags {1} materials has ended successfully.", filename, flags);
	}
	else
	{
		Engine::logs()->error(
			 "Cache", "Loading of {0} with flags {1} materials has ended with error {2}.", filename, flags, materialLoadResult);
	}

	extra->materials = std::move(materialRefs);
	resource->extra = utils::reinterpret_pointer_cast<ResourceExtra>(extra);
	resource->data = reinterpret_cast<int8_t*>(vb.data());
	resource->size = vb.size() * sizeof(vb.data());
	extra->count = resource->size / ObjData::VertexSize;

	Engine::logs()->log("Cache",
		 "Loading of {0} with flags {1} has ended successfully.\nFilename:{0}\nFlags:{1}\nSize:{2}\nVertices:{3}\nMaterials:{4}", filename,
		 flags, resource->size, vb.size(), extra->materials.size());


	return materialLoadResult;
}

FearEngine::Cache::Loaders::ObjLoader::~ObjLoader() {}
