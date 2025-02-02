#include <fstream>
#include <iostream>
#include <string>
#include <stdexcept>
#include <cstdint>

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
          &id3.version_
        , &id3.sub_version_
        , &id3.flag_
    };
    
    for (int i = 0; i < size_of_elements; ++i) 
    {
        source_file.read( reinterpret_cast< char* >( array[ i ] ), sizeof( *( array[ i ] ) ) );
    }
    source_file.read( reinterpret_cast< char* >( &id3.size_ ), sizeof( id3.size_ ) );

    id3.size_ = ntohl(id3.size_);
     
    return id3;
}
} //namespace MP3