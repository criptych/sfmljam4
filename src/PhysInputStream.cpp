////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////

#include "PhysInputStream.hpp"

#include <physfs.h>

////////////////////////////////////////////////////////////////////////////////

PhysInputStream::PhysInputStream(PHYSFS_File *file):
    m_file(file)
{
}

PhysInputStream::~PhysInputStream()
{
    close();
}

void PhysInputStream::close()
{
    if (m_file)
    {
        PHYSFS_close(m_file);
        m_file = nullptr;
    }
}

sf::Int64 PhysInputStream::read(void *data, sf::Int64 size)
{
    return PHYSFS_readBytes(m_file, data, size);
}

sf::Int64 PhysInputStream::seek(sf::Int64 position)
{
    return PHYSFS_seek(m_file, position) ? PHYSFS_tell(m_file) : -1;
}

sf::Int64 PhysInputStream::tell()
{
    return PHYSFS_tell(m_file);
}

sf::Int64 PhysInputStream::getSize()
{
    return PHYSFS_fileLength(m_file);
}

////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////

