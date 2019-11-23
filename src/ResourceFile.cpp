#include "ResourceFile.h"
#include "Logging.h"
#include <SDL_ttf.h>

ResourceFile::ResourceFile(const void* data, size_t size):
	mFile(SDL_RWFromConstMem(data, size)),
	mSize(size)
{
	Logging::log("Created ResourceFile of size " + std::to_string(size));
	ASSERT(mFile, "Resource file failed to load, SDL_Error: " + std::string(SDL_GetError()));
}

ResourceFile::~ResourceFile() {
	Logging::log("Destroying Resourcefile ");
	// if (!mIsFont) {
		ASSERT(mFile, "Resource file should not be null");
		SDL_RWclose(mFile);
	// }
}

size_t ResourceFile::getSize() const {
	return mSize;
}

TTF_Font* ResourceFile::getAsFont(int fontSize) const {
	ASSERT(mFile, "file is null");
	mIsFont = true;

	/*
	Bug in SDL - Must free memory here
	calling SDL_RWclose will result in
	segmentation fault when reading the font
	*/
	// const int free_memory_please = 1;
	const int do_not_clear_source = 0;
	TTF_Font * out = TTF_OpenFontRW(mFile, do_not_clear_source, fontSize);
	ASSERT(out, "Could not parse file as font " + std::string(TTF_GetError()));
	return out;
}

SDL_Surface* ResourceFile::getAsSurface() const{
	ASSERT(mFile, "file is null");
	const int do_not_clear_source = 0;
	SDL_Surface * out = IMG_Load_RW(mFile, do_not_clear_source);
	ASSERT(out, "Could not parse file as surface " + std::string(SDL_GetError()));
	return out;
}

std::string ResourceFile::getAsText() const {
	ASSERT(mFile, "file is null");
	char * buf = new char[mSize];
	int objects = 1; //It's a single object
	size_t result = SDL_RWread(mFile, buf, mSize, objects);
	ASSERT(result != 0, "Could parse file as text " + std::string(SDL_GetError()));
	std::string text(buf);
	delete[] buf;
	return text;
}
