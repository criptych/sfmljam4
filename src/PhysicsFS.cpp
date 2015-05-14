////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////

#include "PhysicsFS.hpp"

#include <physfs.h>

////////////////////////////////////////////////////////////////////////////////

bool PhysicsFS::init(const char *arg0)
{
    return PHYSFS_init(arg0);
}

bool PhysicsFS::deinit()
{
    return PHYSFS_deinit();
}

void PhysicsFS::getCompiledVersion(Version &version)
{
    PHYSFS_VERSION(&version);
}

void PhysicsFS::getLinkedVersion(Version &version)
{
    PHYSFS_getLinkedVersion(reinterpret_cast<PHYSFS_Version*>(&version));
}

const char *PhysicsFS::getLastError()
{
    return getErrorByCode(getLastErrorCode());
}

PhysicsFS::ErrorCode PhysicsFS::getLastErrorCode()
{
    return static_cast<PhysicsFS::ErrorCode>(PHYSFS_getLastErrorCode());
}

const char *PhysicsFS::getErrorByCode(PhysicsFS::ErrorCode code)
{
    return PHYSFS_getErrorByCode(static_cast<PHYSFS_ErrorCode>(code));
}

void PhysicsFS::doThrow() const throw( PhysicsFS::Error )
{
    throw PhysicsFS::Error(getLastErrorCode());
}

unsigned int PhysicsFS::_counter = 0;

PhysicsFS::PhysicsFS(const char *arg0)
{
    if (_counter == 0)
    {
        init(arg0);
    }

    _counter += 1;
}

PhysicsFS::~PhysicsFS()
{
    if (_counter > 0)
    {
        _counter -= 1;
    }

    if (_counter == 0)
    {
        deinit();
    }
}

////////////////////////////////////////////////////////////////////////////////

bool PhysicsFS::symbolicLinksPermitted() const
{
    return PHYSFS_symbolicLinksPermitted();
}

bool PhysicsFS::permitSymbolicLinks(bool permit)
{
    bool old = symbolicLinksPermitted();
    PHYSFS_permitSymbolicLinks(permit);
    return old;
}

////////////////////////////////////////////////////////////////////////////////

const char *PhysicsFS::getBaseDir() const
{
    return PHYSFS_getBaseDir();
}

const char *PhysicsFS::getWriteDir() const
{
    return PHYSFS_getWriteDir();
}

void PhysicsFS::setWriteDir(const char *path)
{
    if (!PHYSFS_setWriteDir(path))
    {
        doThrow();
    }
}

const char *PhysicsFS::getPrefDir(const char *orgName, const char *appName) const
{
    const char *prefDir = PHYSFS_getPrefDir(orgName, appName);

    if (!prefDir)
    {
        doThrow();
    }

    return prefDir;
}

////////////////////////////////////////////////////////////////////////////////

void PhysicsFS::mount(const char *path, const char *vpath, bool priority)
{
    if (!PHYSFS_mount(path, vpath, !priority))
    {
        doThrow();
    }
}

void PhysicsFS::unmount(const char *path)
{
    if (!PHYSFS_unmount(path))
    {
        doThrow();
    }
}

////////////////////////////////////////////////////////////////////////////////

void PhysicsFS::makeDir(const char *path)
{
    if (!PHYSFS_mkdir(path))
    {
        doThrow();
    }
}

void PhysicsFS::remove(const char *path)
{
    if (!PHYSFS_delete(path))
    {
        doThrow();
    }
}

////////////////////////////////////////////////////////////////////////////////

bool PhysicsFS::isDir(const char *path) const
{
    PHYSFS_Stat st;
    if (!PHYSFS_stat(path, &st))
    {
        doThrow();
    }
    return (st.filetype == PHYSFS_FILETYPE_DIRECTORY);
}

bool PhysicsFS::isFile(const char *path) const
{
    PHYSFS_Stat st;
    if (!PHYSFS_stat(path, &st))
    {
        doThrow();
    }
    return (st.filetype == PHYSFS_FILETYPE_REGULAR);
}

bool PhysicsFS::isSymLink(const char *path) const
{
    PHYSFS_Stat st;
    if (!PHYSFS_stat(path, &st))
    {
        doThrow();
    }
    return (st.filetype == PHYSFS_FILETYPE_SYMLINK);
}

bool PhysicsFS::isReadOnly(const char *path) const
{
    PHYSFS_Stat st;
    if (!PHYSFS_stat(path, &st))
    {
        doThrow();
    }
    return st.readonly;
}

sf::Int64 PhysicsFS::getFileSize(const char *path) const
{
    PHYSFS_Stat st;
    if (!PHYSFS_stat(path, &st))
    {
        doThrow();
    }
    return st.filesize;
}

sf::Int64 PhysicsFS::getModTime(const char *path) const
{
    PHYSFS_Stat st;
    if (!PHYSFS_stat(path, &st))
    {
        doThrow();
    }
    return st.modtime;
}

sf::Int64 PhysicsFS::getCreateTime(const char *path) const
{
    PHYSFS_Stat st;
    if (!PHYSFS_stat(path, &st))
    {
        doThrow();
    }
    return st.createtime;
}

sf::Int64 PhysicsFS::getAccessTime(const char *path) const
{
    PHYSFS_Stat st;
    if (!PHYSFS_stat(path, &st))
    {
        doThrow();
    }
    return st.accesstime;
}

////////////////////////////////////////////////////////////////////////////////

PhysInputStream PhysicsFS::openRead(const char *path)
{
    PHYSFS_File *file = PHYSFS_openRead(path);
    if (!file)
    {
        doThrow();
    }
    return PhysInputStream(file);
}

PhysOutputStream PhysicsFS::openWrite(const char *path)
{
    PHYSFS_File *file = PHYSFS_openWrite(path);
    if (!file)
    {
        doThrow();
    }
    return PhysOutputStream(file);
}

PhysOutputStream PhysicsFS::openAppend(const char *path)
{
    PHYSFS_File *file = PHYSFS_openAppend(path);
    if (!file)
    {
        doThrow();
    }
    return PhysOutputStream(file);
}

////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////

