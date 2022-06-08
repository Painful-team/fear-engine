#include "ObjLoader.hpp"

#include <vector>

#include <tinyobj_loader_opt.h>

#include <cache/ObjResource.hpp>
#include <utils/PointerCasts.hpp>

int FearEngine::Cache::Loaders::ObjLoader::init() { return errorCodes::OK; }

std::string FearEngine::Cache::Loaders::ObjLoader::getPattern() const { return "*.obj"; }

// TODO rewrite into optimized glm::version.
void CalcNormal(float normal[3], float v0[3], float v1[3], float v2[3])
{
	float v10[3];
	v10[0] = v1[0] - v0[0];
	v10[1] = v1[1] - v0[1];
	v10[2] = v1[2] - v0[2];

	float v20[3];
	v20[0] = v2[0] - v0[0];
	v20[1] = v2[1] - v0[1];
	v20[2] = v2[2] - v0[2];

	normal[0] = v20[1] * v10[2] - v20[2] * v10[1];
	normal[1] = v20[2] * v10[0] - v20[0] * v10[2];
	normal[2] = v20[0] * v10[1] - v20[1] * v10[0];
	
	float len2 = normal[0] * normal[0] + normal[1] * normal[1] + normal[2] * normal[2];
	if (len2 > 0.0f)
	{
		float len = sqrtf(len2);

		normal[0] /= len;
		normal[1] /= len;
	}
}

const char* get_file_data(size_t& len, const char* filename)
{
	const char* ext = strrchr(filename, '.');

	size_t data_len = 0;
	const char* data = nullptr;

	len = 0;

	HANDLE file = CreateFileA(
		 filename, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN, NULL);
	assert(file != INVALID_HANDLE_VALUE);

	HANDLE fileMapping = CreateFileMapping(file, NULL, PAGE_READONLY, 0, 0, NULL);
	assert(fileMapping != INVALID_HANDLE_VALUE);

	LPVOID fileMapView = MapViewOfFile(fileMapping, FILE_MAP_READ, 0, 0, 0);
	assert(fileMapView != nullptr);

	data = (const char*)fileMapView;

	LARGE_INTEGER fileSize;
	fileSize.QuadPart = 0;
	GetFileSizeEx(file, &fileSize);

	len = static_cast<size_t>(fileSize.QuadPart);

	return data;
}

FearEngine::Cache::errorCode FearEngine::Cache::Loaders::ObjLoader::load(const std::string_view& filename,
	 std::shared_ptr<Resource>& resource,
	 ResourceFlags flags)
{
	tinyobj_opt::attrib_t attrib;

	auto load_t_begin = std::chrono::high_resolution_clock::now();
	size_t data_len = 0;
	const char* data = get_file_data(data_len, filename.data());
	if (data == nullptr)
	{
		return Cache::errorCodes::RESOURCE_NOT_FOUND;
	}

	bool verbose = false;
#ifdef DEBUG
	verbose = true;
	auto load_t_end = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double, std::milli> load_ms = load_t_end - load_t_begin;
	std::cout << "filesize: " << data_len << std::endl;
	std::cout << "load time: " << load_ms.count() << " [msecs]" << std::endl;
#endif

	tinyobj_opt::LoadOption option;
	option.req_num_threads = ObjLoader::Threads;

	auto extra = std::make_shared<ObjData>();
	std::vector<tinyobj_opt::shape_t> shapes;
	std::vector<tinyobj_opt::material_t> materials;

	option.verbose = verbose;
	bool ret = parseObj(&attrib, &shapes, &materials, data, data_len, option);

	if (!ret)
	{
		return false;
	}

	auto& vb = extra->vertices;	 // pos(3float), normal(3float), color(3float), texCoord(2float)
	size_t face_offset = 0;
	for (size_t v = 0; v < attrib.face_num_verts.size(); v++)
	{
		assert(attrib.face_num_verts[v] % 3 == 0);	// assume all triangle face(multiple of 3).
		for (size_t f = 0; f < attrib.face_num_verts[v] / 3; f++)
		{
			tinyobj_opt::index_t idx0 = attrib.indices[face_offset + 3 * f + 0];
			tinyobj_opt::index_t idx1 = attrib.indices[face_offset + 3 * f + 1];
			tinyobj_opt::index_t idx2 = attrib.indices[face_offset + 3 * f + 2];

			float v[3][3];
			for (int k = 0; k < 3; k++)
			{
				int f0 = idx0.vertex_index;
				int f1 = idx1.vertex_index;
				int f2 = idx2.vertex_index;
				assert(f0 >= 0);
				assert(f1 >= 0);
				assert(f2 >= 0);

				v[0][k] = attrib.vertices[3 * f0 + k];
				v[1][k] = attrib.vertices[3 * f1 + k];
				v[2][k] = attrib.vertices[3 * f2 + k];
			}

			float vt[3][2];
			for (int k = 0; k < 2; k++)
			{
				int f0 = idx0.texcoord_index;
				int f1 = idx1.texcoord_index;
				int f2 = idx2.texcoord_index;
				assert(f0 >= 0);
				assert(f1 >= 0);
				assert(f2 >= 0);

				vt[0][k] = attrib.texcoords[2 * f0 + k];
				vt[1][k] = attrib.texcoords[2 * f1 + k];
				vt[2][k] = attrib.texcoords[2 * f2 + k];
			}

			float n[3][3];
			if (attrib.normals.size() > 0)
			{
				int nf0 = idx0.normal_index;
				int nf1 = idx1.normal_index;
				int nf2 = idx2.normal_index;

				if (nf0 >= 0 && nf1 >= 0 && nf2 >= 0)
				{
					assert(3 * nf0 + 2 < attrib.normals.size());
					assert(3 * nf1 + 2 < attrib.normals.size());
					assert(3 * nf2 + 2 < attrib.normals.size());
					for (int k = 0; k < 3; k++)
					{
						n[0][k] = attrib.normals[3 * nf0 + k];
						n[1][k] = attrib.normals[3 * nf1 + k];
						n[2][k] = attrib.normals[3 * nf2 + k];
					}
				}
				else
				{
					// compute geometric normal
					CalcNormal(n[0], v[0], v[1], v[2]);
					n[1][0] = n[0][0];
					n[1][1] = n[0][1];
					n[1][2] = n[0][2];
					n[2][0] = n[0][0];
					n[2][1] = n[0][1];
					n[2][2] = n[0][2];
				}
			}
			else
			{
				// compute geometric normal
				CalcNormal(n[0], v[0], v[1], v[2]);
				n[1][0] = n[0][0];
				n[1][1] = n[0][1];
				n[1][2] = n[0][2];
				n[2][0] = n[0][0];
				n[2][1] = n[0][1];
				n[2][2] = n[0][2];
			}

			for (int k = 0; k < 3; k++)
			{
				vb.push_back(v[k][0]);
				vb.push_back(v[k][1]);
				vb.push_back(v[k][2]);
				vb.push_back(n[k][0]);
				vb.push_back(n[k][1]);
				vb.push_back(n[k][2]);
				// Use normal as color.
				float c[3] = {n[k][0], n[k][1], n[k][2]};
				float len2 = c[0] * c[0] + c[1] * c[1] + c[2] * c[2];
				if (len2 > 1.0e-6f)
				{
					float len = sqrtf(len2);

					c[0] /= len;
					c[1] /= len;
					c[2] /= len;
				}
				vb.push_back(c[0] * 0.5 + 0.5);
				vb.push_back(c[1] * 0.5 + 0.5);
				vb.push_back(c[2] * 0.5 + 0.5);

				vb.push_back(vt[k][0]);
				vb.push_back(vt[k][1]);
			}
		}
		face_offset += attrib.face_num_verts[v];
	}

	resource = std::make_shared<Cache::Resource>();
	resource->filename = filename.data();
	resource->flags = flags;

	std::vector<std::shared_ptr<Material>> materialRefs;
	materialRefs.resize(materials.size());
	for (uint8_t i = 0; i < materials.size(); ++i)
	{
		materialRefs[i] = Material::create(materials[i]);
	}

	extra->materials = std::move(materialRefs);
	resource->extra = utils::reinterpret_pointer_cast<ResourceExtra>(extra);
	resource->data = reinterpret_cast<int8_t*>(vb.data());
	resource->size = vb.size() * sizeof(vb[0]);
	return errorCodes::OK;
}

FearEngine::Cache::Loaders::ObjLoader::~ObjLoader() {}
