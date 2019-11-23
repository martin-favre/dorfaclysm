#include "ResourceArchive.h"
#include "Helpers.h"

#include <iostream>
#include <fstream>
#include "Logging.h"

ResourceArchive::ResourceArchive(const std::string &path) : m_source_path(path)
{
	Logging::log("Added resource archive " + path);
}

ResourceFile *ResourceArchive::getItem(const std::string &name)
{
	Logging::log("Getting item " + name + " from archive " + m_source_path);
	std::string path = m_source_path + "\\" + name;
	if (!m_files.count(name))
	{
		void *contents = nullptr;
		size_t filesize = 0;
		readFile(path, &contents, &filesize);
		auto file = std::make_unique<ResourceFile>(contents, (uint)filesize)
		m_files[name] = std::move(file);
	}
	return m_files[name].get();
}

void ResourceArchive::freeItem(const std::string &name)
{
	Logging::log("Freeing item " + name);
	ASSERT(m_files.count(name), "Trying to free null item");
	m_files[name].reset();
	m_files.erase(name);
}

void ResourceArchive::readFile(const std::string &path, void **buffer, size_t *size)
{
	std::ifstream file(path, std::ios::in | std::ios::binary | std::ios::ate);
	ASSERT(file.good(), "Could not open file " + path);
	ASSERT(file.is_open(), "Could not open file " + path);
	*size = (size_t)file.tellg();
	char *tmpbuf = new char[*size];
	file.seekg(0, std::ios::beg);
	file.read(tmpbuf, *size);
	*buffer = tmpbuf;
	file.close();
}