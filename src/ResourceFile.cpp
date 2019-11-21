#include "ResourceFile.h"
#include "Logging.h"
ResourceFile::ResourceFile(void* data, uint size):
	m_file_p(SDL_RWFromConstMem(data, size)),
	m_size(size)
{
	Logging::log("Created ResourceFile of size " + std::to_string(size));
	ASSERT(m_file_p, "Resource file failed to load, SDL_Error: " + std::string(SDL_GetError()));
}

ResourceFile::~ResourceFile() {
	Logging::log("Destroying Resourcefile ");
	if (!m_is_font) {
		ASSERT(m_file_p, "Resource file should not be null");
		SDL_RWclose(m_file_p);
	}
}

SDL_RWops * ResourceFile::get_sdl_rwops() {
	return m_file_p;
}
uint ResourceFile::get_size() {
	return m_size;
}

TTF_Font * ResourceFile::get_font(int size) {
	ASSERT(m_file_p, "file is null");
	m_is_font = true;

	/*
	Bug in SDL - Must free memory here
	calling SDL_RWclose will result in
	segmentation fault when reading the font
	*/
	const int free_memory_please = 1;
	TTF_Font * out = TTF_OpenFontRW(m_file_p, free_memory_please, size);
	ASSERT(out, "Could not parse file as font " + std::string(TTF_GetError()));
	return out;
}

SDL_Surface * ResourceFile::get_surface() {
	ASSERT(m_file_p, "file is null");
	const int do_not_clear_source = 0;
	SDL_Surface * out = IMG_Load_RW(m_file_p, do_not_clear_source);
	ASSERT(out, "Could not parse file as surface " + std::string(SDL_GetError()));
	return out;
}

std::string ResourceFile::get_text() {
	ASSERT(m_file_p, "file is null");
	char * buf = new char[m_size];
	int objects = 1; //It's a single object
	size_t result = SDL_RWread(m_file_p, buf, m_size, objects);
	ASSERT(result != 0, "Could parse file as text " + std::string(SDL_GetError()));
	std::string text(buf);
	delete buf;
	return text;
}
