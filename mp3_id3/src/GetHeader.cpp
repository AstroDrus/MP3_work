#include <fstream>
#include <iostream>
#include <string>
#include <stdexcept>
#include <cstdint>
#include <vector>
#include <algorithm>
#include <cstring>

#include <arpa/inet.h>

#include "../ParseMP3.h"
#include "../GetHeader.h"
#include "../GetFrame.h"

namespace MP3
{

std::uint32_t ID3v2Header::make_right_sizeadr( std::uint32_t size )
{
 size = ntohl(size);
 
		constexpr static std::uint32_t mask = 0x7F;
		
		std::uint32_t prevMask{0};
		for( std::size_t byte = 1; byte < sizeof( std::uint32_t ); ++byte )
		{
       const std::uint32_t crntByteMask = (( prevMask << 7 ) | mask);
       const std::uint32_t oldButGold = ( size & crntByteMask );
       const std::uint32_t shifted = size >> 1;
       const std::uint32_t newValue = ( shifted & ~( crntByteMask ) );
       
       size = ( (oldButGold) | newValue );
       
       prevMask = crntByteMask;
		}

return size;
}


// header outstream
std::ostream& operator<<( std::ostream& stream, ID3v2Header& out_head )
{
    return stream << "header version " << int(out_head.get_version_()) << "\n"
    << "header sub version - " << int(out_head.get_sub_version_()) << "\n"
    << "header flag - " << int(out_head.get_flag_()) << "\n"
    << "header size - " << std::hex << out_head.get_size_();

}

//getters for main header (id3v2Header_)
std::uint8_t ID3v2Header::get_version_()
{
	return version_;
} 

std::uint8_t ID3v2Header::get_sub_version_()
{
	return sub_version_;
} 

std::uint8_t ID3v2Header::get_flag_()
{
	return flag_;
} 

std::uint32_t ID3v2Header::get_size_()
{
	return size_;
} 

//setters for main header (id3v2Header_)
void ID3v2Header::set_version_(const std::uint8_t val)
{
	version_ = val;
}

void ID3v2Header::set_sub_version_(const std::uint8_t val)
{
	sub_version_ = val;
}

void ID3v2Header::set_flag_(const std::uint8_t val)
{
	flag_ = val;
}

void ID3v2Header::set_size_(const std::uint32_t val)
{
	size_ = val;
}

}
