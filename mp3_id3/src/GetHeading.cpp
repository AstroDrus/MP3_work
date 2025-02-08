#include <fstream>
#include <iostream>
#include <string>
#include <stdexcept>
#include <cstdint>
#include <vector>
#include <algorithm>
#include <bitset>
#include <climits>

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
     ID3 id3{};
    
    // besides size_
    constexpr int size_of_elements = 3;
    std::uint8_t* array[size_of_elements] = 
    { 
          id3.get_adr_version_()
        , id3.get_adr_sub_version_()
        , id3.get_adr_flag_()
    };
    
    for (int i = 0; i < size_of_elements; ++i) 
    {
        source_file.read( reinterpret_cast< char* >( array[ i ] ), sizeof( *( array[ i ] ) ) );
    }

    source_file.read( reinterpret_cast< char* >( id3.get_adr_size_() ), sizeof( *id3.get_adr_size_() ) );

    id3.set_size_(id3.make_right_sizeadr(*id3.get_adr_size_()));

return id3;

}

std::uint32_t ID3::make_right_sizeadr(uint32_t size)
{
    size = ntohl(size);
    //int buffer_size = 0xFFFFF;
    int buffer_size = size;
   // std::cout << "bits of size before magic " << std::bitset<sizeof(size) * CHAR_BIT>(size) << std::endl;

//todo make cycle
    unsigned int first_byte = buffer_size % 128; // 1000 0000 - take last 2 symbols
  // std::cout << "first byte " << std::hex << first_byte << " " << "bits of first byte " << std::bitset<sizeof(first_byte) * CHAR_BIT>(first_byte) << std::endl;
    buffer_size = buffer_size >> 8;
   // std::cout << "buffer_size after making first_byte " << std::hex << buffer_size << std::endl;

    unsigned int second_byte = buffer_size % 128;
   // std::cout << "second byte " << std::hex << second_byte << " " << "bits of second byte " <<std::bitset<sizeof(second_byte) * CHAR_BIT>(second_byte) << std::endl;
    buffer_size = buffer_size >> 8;
   // std::cout << "buffer_size after making second_byte " << std::hex << buffer_size << std::endl;

    unsigned int third_byte = buffer_size % 128;
    // std::cout << "third byte " << std::hex << third_byte << " " << "bits of third byte " << std::bitset<sizeof(third_byte) * CHAR_BIT>(third_byte) << std::endl;



    if ((second_byte & 0x1) == 0x1)
    {
       first_byte = first_byte | (1u << 7);
    }
    else
    {
        first_byte = first_byte | (0u << 7);
    }
    second_byte = second_byte >> 1;

    if ((third_byte & 0x3) == 0x1)
    {
       second_byte = second_byte | (1u << 6);
       second_byte = second_byte | (0u << 7);
    }

    else if ((third_byte & 0x3) == 0x2)
    {
       second_byte = second_byte | (0u << 6);
       second_byte = second_byte | (1u << 7);
    }

    else if ((third_byte & 0x3) == 0x3)
    {
       second_byte = second_byte | (1u << 6);
       second_byte = second_byte | (1u << 7);
    }

    third_byte = third_byte >> 2;

    int result = 0;
    result = third_byte;
    result = result << 8;
    result += second_byte;
    result = result << 8;
    result += first_byte;

    //std::cout << "result in 16th " <<std::hex << result << " result in bits " << std::bitset<sizeof(result) * CHAR_BIT>(result) << std::endl;

    return result;
}

//getters
std::uint8_t* ID3::get_adr_version_()
{
    return &version_;
} 

std::uint8_t* ID3::get_adr_sub_version_()
{
    return &sub_version_;
} 

std::uint8_t* ID3::get_adr_flag_()
{
    return &flag_;
} 

std::uint32_t* ID3::get_adr_size_()
{
    return &size_;
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