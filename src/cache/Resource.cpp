#include "Resource.hpp"

void FearEngine::Cache::deleterFunc(void*) {}

FearEngine::Cache::Resource::~Resource()
{
	if (deleter != nullptr)
	{
		deleterFunc(data);
	}
}
