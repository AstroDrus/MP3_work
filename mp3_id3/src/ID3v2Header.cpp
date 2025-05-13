
#include <cstring>
#include <ostream>

#include "../ID3v2Header.h"

namespace MP3
{

std::ostream& operator<<( std::ostream& stream, const ID3v2Header& out_head )
{
    return stream << "header version - " << + out_head.version << "\n"
        << "header sub version - " << + out_head.sub_version << "\n"
        << "header flag - " << + out_head.flag << "\n"
        << "header size - " << std::hex << out_head.size;

}

} // namespace MP3