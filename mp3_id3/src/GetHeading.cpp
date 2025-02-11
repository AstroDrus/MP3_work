#include <fstream>
#include <iostream>
#include <string>
#include <stdexcept>
#include <cstdint>
#include <vector>
#include <algorithm>

#include <arpa/inet.h>

#include "../libMP3.h"

namespace MP3
{

//give to the main.cpp the head of the MP3_file
ID3 GetHeading( std::ifstream& source_file)
{  

    static const std::string ID3_head = "ID3";
    // array-buffer for the result Class (besides size_) of this func (C-style) 
    //verification if file can be opened  
    if (!source_file.is_open())
    {

        throw std::invalid_argument("Incorrect source file name!"); 

    }
    // amount of bayts in head of MP3_file
    static constexpr const int sizeID3 = 3;
    // array-buffer for the result Class (besides size_) (C-style) 
    char signature[sizeID3+1] = {'\0'};
    source_file.read(signature, sizeID3);
    if (ID3_head != signature)
    {
        throw std::invalid_argument("source file is not an MP3 file with IDv3 tag");
    }
       
    // besides size_
    constexpr int size_of_elements = 3;

    std::uint8_t array[ size_of_elements ] = { 0 };

    for (int i = 0; i < size_of_elements; ++i) 
    {
        source_file.read( reinterpret_cast< char* >( &array[ i ] ), sizeof( ( array[ i ] ) ) );
    }

    ID3 id3{};
    id3.set_version_( array[0] );
    id3.set_sub_version_( array[1] ); 
    id3.set_flag_( array[2] );

    std::uint32_t buffer_size;
    source_file.read( reinterpret_cast< char* >( &buffer_size ), sizeof( buffer_size) );
    id3.set_size_( buffer_size );
    id3.set_size_(id3.make_right_sizeadr(id3.get_size_()));

return id3;
}

std::uint32_t ID3::make_right_sizeadr( std::uint32_t size )
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

//getters
std::uint8_t ID3::get_version_()
{
		return version_;
} 

std::uint8_t ID3::get_sub_version_()
{
		return sub_version_;
} 

std::uint8_t ID3::get_flag_()
{
		return flag_;
} 

std::uint32_t ID3::get_size_()
{
		return size_;

} 

//setters
void ID3::set_version_(const std::uint8_t val)
{
		version_ = val;
}

void ID3::set_sub_version_(const std::uint8_t val)
{
		sub_version_ = val;
}

void ID3::set_flag_(const std::uint8_t val)
{
	 flag_ = val;
}

void ID3::set_size_(const std::uint32_t val)
{
   size_ = val;
}

} //namespace MP3