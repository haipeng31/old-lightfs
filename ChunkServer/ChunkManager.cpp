#include "ChunkManager.h"
#include <dirent.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../netlib/base/Logging.h"
#include <assert.h>
using namespace myfs;
using mynet::Logger;

ChunkManager::ChunkManager(string dir, int size)
 :  dir_(dir),
	used_(0),
	size_(size)
{
	struct dirent *dirp;
	DIR *dp;

	if ((dp = opendir(dir_.c_str())) == NULL) {
		LOG_TRACE << "open dir " << dir_ << "error";
	}

	while ((dirp = readdir(dp)) != NULL) {
		int len = strlen(dirp->d_name);
		if ((len >= 4) && (strcmp(dirp->d_name +  len - 4, ".old") == 0)) {
			delChunks_.push_back(string(dirp->d_name));
		} else if ((strcmp(dirp->d_name, ".") != 0) && (strcmp(dirp->d_name, "..") != 0)) {
			string name = string(dirp->d_name) + ".old";
			delChunks_.push_back(name);

			string oldpath = dir + "/" + dirp->d_name;
			string newpath = dir + "/" + name;
			if (rename(oldpath.c_str(), newpath.c_str()) < 0) {
				LOG_TRACE << "rename file " << oldpath << "to " << newpath << "error";
			}

		}
	}

}

int ChunkManager::addChunk(string name)
{
	if (chunks_.find(name) != chunks_.end()) {
		LOG_TRACE << "The chunk already exist";
		return -1;
	}
	
	assert(chunks_.size() == used_);
	assert(used_ < size_);
	
	if (delChunks_.size() > 0) {
		string oldName = delChunks_.front();
		if (rename(oldName, name) < 0) {
			return -2;
		}
		delChunks_.pop_front();
	} else {
		createFile(name);	
	}

	chunks_[name] = 0;
	used_++;
	return 0;
}

int ChunkManager::delChunk(string name)
{
	if (chunks_.find(name) == chunks_.end()) {
		LOG_TRACE << "chunk " << name << "doesn't exist";
		return -1;
	}
	
	string newname = name + ".old";
	if (rename(name, newname) < 0) {
		return -2;
	}
	chunks_.erase(name);
	delChunks_.push_back(newname);
	used_--;
	return 0;
}

string ChunkManager::readChunk(string name, int index, int length)
{
	assert(chunks_.find(name) != chunks_.end());
	assert(length > 0);
	string path = dir_ + "/" + name;
	FILE *fp = fopen(path.c_str(), "r");
	if (fp == NULL) {
		LOG_TRACE << "open file " << name << " error";
		return "";
	}
	int size = chunks_[name];
	int nread;
	if (index >= size) {
		fclose(fp);
		return "";
	}
	
	nread = ((size - index) <= length) ? (size - index) : length;
	char *str = (char *)malloc(sizeof(char)*(nread + 1));
	memset(str, 0, nread + 1);
	
	fseek(fp, index, SEEK_SET);
	fread(str, nread, 1, fp);
		
	string result = str;
	free(str);
	fclose(fp);
	return result;
}

int ChunkManager::writeChunk(string name, int index, string data)
{
	assert(chunks_.find(name) != chunks_.end());
	string path = dir_ + "/" + name;
	FILE *fp = fopen(path.c_str(), "rw+");
	if (fp == NULL) {
		LOG_TRACE << "open file " << name << " error";
		return -1;
	}

	fseek(fp, index, SEEK_SET);
	fwrite(data.c_str(), data.size(), 1, fp);
	fclose(fp);
	if (index + data.size() > chunks_[name]) {
		chunks_[name] = index + data.size();
	}

	return 0;
}

int ChunkManager::appendChunk(string name, string data)
{
	assert(chunks_.find(name) != chunks_.end());
	return writeChunk(name, chunks_[name], data);
}

/* create a file with size = kChunkSize */
void ChunkManager::createFile(string name)
{
	string path = dir_ + "/" + name;
	FILE *fp;

	if ((fp = fopen(path.c_str(), "w")) == NULL) {
		LOG_TRACE << "Open file " << path << " error";
		return;
	}
	
	int nKb = kChunkSize / 1024;
	char buf[1024];
	memset(buf, 'a', sizeof(buf));

	for (int i = 0; i < nKb; i++) {
		fwrite(buf, 1, 1024, fp);
	}
	
	fclose(fp);
}

int ChunkManager::rename(string oldName, string newName)
{
	string oldpath = dir_ + "/" + oldName;
	string newpath = dir_ + "/" + newName;
	if (::rename(oldpath.c_str(), newpath.c_str()) < 0) {
		LOG_TRACE << "rename file " << oldpath << "to " << newpath << "error";
		return -1;
	}
	return 0;
}
