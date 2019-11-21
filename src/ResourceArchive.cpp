#include "ResourceArchive.h"
#include "Helpers.h"

#include <iostream>
#include <fstream>
#include "Logging.h"

ResourceArchive::ResourceArchive(const std::string &path) : m_source_path(path)
{
	Logging::log("Added resource archive " + path);
}

ResourceFile *ResourceArchive::get_item(const std::string &name)
{
	Logging::log("Getting item " + name + " from archive " + m_source_path);
	std::string path = m_source_path + "\\" + name;
	if (!m_files.count(name))
	{
		void *contents = nullptr;
		size_t filesize = 0;
#ifdef RESOURCE_USING_ZIP
		m_zip_archive.get_entry_from_zip(path, &contents, &filesize);
#else  // RESOURCE_USING_ZIP
		read_file(path, &contents, &filesize);
#endif // RESOURCE_USING_ZIP
		ResourceFile *file = new ResourceFile(contents, (uint)filesize);
		m_files[name] = file;
	}
	return m_files[name];
}

void ResourceArchive::free_item(const std::string &name)
{
	Logging::log("Freeing item " + name);
	ASSERT(m_files.count(name), "Trying to free null item");
	delete m_files[name];
	m_files.erase(name);
}

void ResourceArchive::read_file(const std::string &path, void **buffer, size_t *size)
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