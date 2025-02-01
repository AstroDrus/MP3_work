#include <fstream>
#include <iostream>
#include <string>
#include <stdexcept>
#include <cstdint>
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
    source_file.read(signature, sizeof(std::uint8_t));

    for (int i = 0; i <= sizeID3; ++i) 
    {
        if (i < 3)
        {
            if( i == 0 ) 
            {
                id3.version_ = std::stoi(make_buffer_array(i, source_file));
                break;
            }
            else if( i == 1 ) 
            {
                id3.sub_version_ = std::stoi(make_buffer_array(i, source_file));
                break;
            }
            else if( i == 2 ) 
            {
                id3.flag_ = std::stoi(make_buffer_array(i, source_file));
                break;
            }
            break;
        }
        else
        {
            id3.size_ = std::stoi(make_buffer_array(i, source_file));
            break;
        }
        break;
    }
    return id3;

}

// make universial array for data from header of MP3
char* make_buffer_array(int index, std::ifstream& source_file)
{
    int size_index = 3;
    
    if (index != size_index)
    {
        // array-buffer for the result Class (besides size_) (C-style) 
        char signature[sizeof(std::uint8_t)+1] = {'\0'};
        source_file.read(signature, sizeof(std::uint8_t));
        return signature;
    }
    else
    {
        // array-buffer for the size_ of result Class (C-style) 
        char signature[sizeof(std::uint32_t)+1] = {'\0'};
        source_file.read(signature, sizeof(std::uint32_t));
        return signature;
    }
}
}// namespace MP3
