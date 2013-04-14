#define FUSE_USE_VERSION 26

#include <fuse.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <errno.h>
#include <sys/time.h>
#include "FsClient.h"
#include "Dirent.h"
#include "../netlib/base/Logging.h"
#include <ctime>

using mynet::Logger;
using myfs::Dirent;
using myfs::FsClient;

FsClient fsClient("FsClient", "127.0.0.1", 5555);
static int myfs_getattr(const char *path, struct stat *stbuf)
{
	LOG_INFO << "getattr";
	return fsClient.getattr(path, stbuf);
#if 0
	LOG_INFO << "uid = " << stbuf->st_uid;
	LOG_INFO << "gid = " << stbuf->st_gid;
	LOG_INFO << "size = " << stbuf->st_size;
	LOG_INFO << "atime = " << ctime(&stbuf->st_atime);
	LOG_INFO << "ctime = " << ctime(&stbuf->st_ctime);
	LOG_INFO << "mtime = " << ctime(&stbuf->st_mtime);
	LOG_INFO << "mode = " << std::oct << stbuf->st_mode << std::dec;
	return 0;
#endif
}

static int myfs_access(const char *path, int mask)
{
	LOG_INFO << "access";
	return 0;
}

static int myfs_readlink(const char *path, char *buf, size_t size)
{
	LOG_INFO << "readlink";
	return 0;
}

static int myfs_readdir(const char *path, void *buf, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fi)
{
	LOG_INFO << "readdir";
	Dirent *dirents = NULL;
	int rt = fsClient.readdir(path, &dirents);

	if (rt < 0) {
		/* do different things wiht different errors */
		return rt;
	} else {
		for (int i = 0; i < rt; i++) {
			struct stat st;
			memset(&st, 0, sizeof(st));
			st.st_mode = dirents[i].type;
			if (filler(buf, dirents[i].name.c_str(), &st, 0)) {
				break;
			}
		}
		return 0;
	}
}

static int myfs_mknod(const char *path, mode_t mode, dev_t rdev)
{
	LOG_INFO << "mknod";
	return fsClient.mknod(path, mode);
}



static int myfs_mkdir(const char *path, mode_t mode)
{
	LOG_INFO << "mkdir";
	return fsClient.mkdir(path, mode);
}

static int myfs_unlink(const char *path)
{
	LOG_INFO << "unlink";
	return fsClient.unlink(path);
}

static int myfs_rmdir(const char *path)
{
	LOG_INFO << "rmdir";
	return fsClient.rmdir(path);
}

static int myfs_symlink(const char *from, const char *to)
{
	LOG_INFO << "symlink";
	return 0;
}

static int myfs_rename(const char *from, const char *to)
{
	LOG_INFO << "rename";
	return fsClient.rename(from ,to);
}

static int myfs_link(const char *from, const char *to)
{
	LOG_INFO << "link";
	return 0;
}

static int myfs_chmod(const char *path, mode_t mode)
{
	LOG_INFO << "chmod";
	return 0;
}

static int myfs_chown(const char *path, uid_t uid, gid_t gid)
{
	LOG_INFO << "chown";
	return 0;
}

static int myfs_truncate(const char *path, off_t size)
{
	LOG_INFO << "truncate";
	return 0;
}

static int myfs_utimens(const char *path, const struct timespec ts[2])
{
	LOG_INFO<< "utimens";
	return 0;
}

static int myfs_open(const char *path, struct fuse_file_info *fi)
{
	LOG_INFO << "open";
	/* first chenk mod, not implement now */

	int fd = fsClient.open(path);

	if (fd < 0) {
		return fd;
	}
	
	/* set the file handle, used by read and write */
	fi->fh = fd;
	return 0;
}

static int myfs_create(const char *path, mode_t mode, struct fuse_file_info *fi)
{
	LOG_INFO << "create";
	int rt = fsClient.mknod(path, mode);
	if (rt < 0) {
		return rt;
	}

	return myfs_open(path, fi);
}

static int myfs_read(const char *path, char *buf, size_t size, off_t offset,
			struct fuse_file_info *fi)
{
	LOG_INFO << "read";
	return fsClient.read(fi->fh, path, buf, size, offset);
}

static int myfs_write(const char *path, const char *buf, size_t size,
			off_t offset, struct fuse_file_info *fi)
{
	LOG_INFO << "write";
	return fsClient.write(fi->fh, path, buf, size, offset);
}

static int myfs_statfs(const char *path, struct statvfs *stbuf)
{
	LOG_INFO << "statfs";
	return 0;
}

static int myfs_flush(const char *path, struct fuse_file_info *fi)
{
	LOG_INFO << "flush";
	return 0;
}
static int myfs_release(const char *path, fuse_file_info *fi)
{
	LOG_INFO << "release";
	return fsClient.release(fi->fh, path);
}

static int myfs_fgetattr(const char *path, struct stat *st, struct fuse_file_info *fi)
{
	return myfs_getattr(path, st);
}
static int myfs_fsync(const char *path, int isdatasync,
		     struct fuse_file_info *fi)
{
	/* Just a stub.	 This method is optional and can safely be left
	   unimplemented */
	LOG_INFO << "fsync";
	(void) path;
	(void) isdatasync;
	(void) fi;
	return 0;
}

#ifdef HAVE_SETXATTR
/* xattr operations are optional and can safely be left unimplemented */
static int myfs_setxattr(const char *path, const char *name, const char *value,
			size_t size, int flags)
{
	LOG_INFO << "setxattr";
	return 0;
}

static int myfs_getxattr(const char *path, const char *name, char *value,
			size_t size)
{
	LOG_INFO << "getxattr";
	return res;
}

static int myfs_listxattr(const char *path, char *list, size_t size)
{
	LOG_INFO << "listxattr";
	return res;
}

static int myfs_removexattr(const char *path, const char *name)
{	
	LOG_INFO << "removexattr";
	return 0;
}
#endif /* HAVE_SETXATTR */

static int myfs_opendir(const char *path, struct fuse_file_info *fi)
{
	LOG_INFO << "opendir";
	return fsClient.opendir(path);
}

static int myfs_releasedir(const char *path, struct fuse_file_info *fi)
{
	LOG_INFO << "release dir";
	return 0;
}

static int myfs_fsyncdir(const char *path, int sync, struct fuse_file_info *fi)
{
	LOG_INFO << "fsyncdir";
	return 0;
}
int main(int argc, char *argv[])
{
	struct fuse_operations oper;
	oper.getattr = myfs_getattr;
	oper.access = myfs_access;           // not implemented
	oper.readlink = myfs_readlink;       // not implemented
	oper.readdir = myfs_readdir;
	oper.mknod = myfs_mknod;
	oper.create = myfs_create;
	oper.mkdir = myfs_mkdir;
	oper.symlink = myfs_symlink;		 // not implemented
	oper.unlink = myfs_unlink;	
	oper.rmdir = myfs_rmdir;
	oper.rename = myfs_rename;
	oper.link = myfs_link;				 // not implemented
	oper.chmod = myfs_chmod;
	oper.chown = myfs_chown;
	oper.truncate = myfs_truncate;		 // not implemented
	oper.utimens = myfs_utimens;		 // not implemented
	oper.open = myfs_open;
	oper.release = myfs_release;
	oper.flush = myfs_flush;
	oper.read = myfs_read;
	oper.write = myfs_write;
	oper.statfs = myfs_statfs;
	oper.fgetattr = myfs_fgetattr;
	oper.fsync = myfs_fsync;
#ifdef HAVE_SETXATTR
	oper.setxattr = myfs_setxattr;
	oper.getxattr = myfs_getxattr;
	oper.listxattr = myfs_listxattr;
	oper.removexattr = myfs_removexattr;
#endif
	oper.opendir = myfs_opendir;
	oper.releasedir = myfs_releasedir;
	oper.fsyncdir = myfs_fsyncdir;
	return fuse_main(argc, argv, &oper, NULL);
}

