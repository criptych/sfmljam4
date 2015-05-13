////////////////////////////////////////////////////////////////////////////////
// PhysicsFS.hpp
//
// Provides a wrapper around the PhysicsFS C API with RAII semantics.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef __PHYSICSFS_HPP__
#define __PHYSICSFS_HPP__ 1

////////////////////////////////////////////////////////////////////////////////

#include "PhysInputStream.hpp"
#include "PhysOutputStream.hpp"

#include <SFML/Config.hpp>

#include <exception>
#include <string>

////////////////////////////////////////////////////////////////////////////////

class PhysicsFS
{
private:
    static unsigned int _counter;

    static bool init(const char *arg0);
    static bool deinit();

public:
    struct Version
    {
        sf::Uint8 major;
        sf::Uint8 minor;
        sf::Uint8 patch;
    };

    enum ErrorCode : int { OK };

public:
    static void getCompiledVersion(Version &version);
    static void getLinkedVersion(Version &version);

    static const char *getLastError();
    static ErrorCode getLastErrorCode();
    static const char *getErrorByCode(ErrorCode code);

public:
    class Error : public std::exception
    {
    public:
        Error(ErrorCode code): m_code(code), m_msg(getErrorByCode(code)) {}

        ErrorCode code() const { return m_code; }
        const char *what() const throw() { return m_msg.c_str(); }

    private:
        ErrorCode m_code;
        std::string m_msg;
    };

private:
    void doThrow() const throw(PhysicsFS::Error);

public:
    PhysicsFS(const char *arg0);
    ~PhysicsFS();

    ////////////////////////////////////////////////////////////
    // Symbolic link settings
    ////////////////////////////////////////////////////////////

    bool symbolicLinksPermitted() const;
    bool permitSymbolicLinks(bool permit);

    ////////////////////////////////////////////////////////////
    // Directory settings
    ////////////////////////////////////////////////////////////

    const char *getBaseDir() const;
    const char *getWriteDir() const;
    void setWriteDir(const char *path);
    const char *getPrefDir(const char *orgName, const char *appName) const;

    ////////////////////////////////////////////////////////////
    // Mount point operations
    ////////////////////////////////////////////////////////////

    void mount(const char *path, const char *vpath = "/", bool priority = false);
    void unmount(const char *path);

    ////////////////////////////////////////////////////////////
    // File/directory operations
    ////////////////////////////////////////////////////////////

    void makeDir(const char *path);
    void removeDir(const char *path);
    void removeFile(const char *path);

    ////////////////////////////////////////////////////////////
    // File status queries
    ////////////////////////////////////////////////////////////

    bool isDir(const char *path) const;
    bool isFile(const char *path) const;
    bool isSymLink(const char *path) const;
    bool isReadOnly(const char *path) const;

    sf::Int64 getFileSize(const char *path) const;
    sf::Int64 getModTime(const char *path) const;
    sf::Int64 getCreateTime(const char *path) const;
    sf::Int64 getAccessTime(const char *path) const;

    ////////////////////////////////////////////////////////////
    // File stream operations
    ////////////////////////////////////////////////////////////

    PhysInputStream openRead(const char *path);
    PhysOutputStream openWrite(const char *path);
    PhysOutputStream openAppend(const char *path);
};

////////////////////////////////////////////////////////////////////////////////

#endif // __PHYSICSFS_HPP__

////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////

