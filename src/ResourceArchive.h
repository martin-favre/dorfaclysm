#pragma once
#include <string>
#include <map>
#include "ResourceFile.h"

/*-------------------------------------------------------
Class to abstract access to the file system.
This represents one folder/zip file alike.
Only supports files smaller than 4GB.
---------------------------------------------------------*/
class ResourceArchive {
public:
	
	/*-------------------------------------------------------
	@param path, path to archive
	---------------------------------------------------------*/
	ResourceArchive(const std::string & path);
	
	/*-------------------------------------------------------
	Get item from file system
	---------------------------------------------------------
	@param name, relative path to item
	@param stored_in_archive, will keep a copy of the file in for faster loading the next times.
						 Set to false for large files.
	@return Pointer to ResourceFile
	---------------------------------------------------------*/
	ResourceFile * getItem(const std::string & name);
	
	/*-------------------------------------------------------
	Free resource item based on path.
	All pointers to that item will no longer be valid. 
	---------------------------------------------------------
	@param name, relative path to item
	---------------------------------------------------------*/
	void freeItem(const std::string & name);

private:
	void readFile(const std::string & path, void ** buffer, size_t* size);
	std::map<std::string, std::unique_ptr<ResourceFile>> m_files;
	const std::string m_source_path;
};