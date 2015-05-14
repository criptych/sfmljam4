////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////

#include "PhysOutputStream.hpp"

#include <physfs.h>

////////////////////////////////////////////////////////////////////////////////

PhysOutputStream::PhysOutputStream(PHYSFS_File *file):
    m_file(file)
{
}

PhysOutputStream::~PhysOutputStream()
{
    close();
}

void PhysOutputStream::close()
{
    if (m_file)
    {
        PHYSFS_close(m_file);
        m_file = nullptr;
    }
}

sf::Int64 PhysOutputStream::write(const void *data, sf::Int64 size)
{
    return PHYSFS_writeBytes(m_file, data, size);
}

sf::Int64 PhysOutputStream::seek(sf::Int64 position)
{
    return PHYSFS_seek(m_file, position) ? PHYSFS_tell(m_file) : -1;
}

sf::Int64 PhysOutputStream::tell()
{
    return PHYSFS_tell(m_file);
}

sf::Int64 PhysOutputStream::getSize()
{
    return PHYSFS_fileLength(m_file);
}

////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////

