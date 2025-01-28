#include <fstream>
#include <iostream>
#include <string>
#include <stdexcept>
#include <cstdint>



namespace MP3
{
struct ID3
{
    std::uint32_t size_ = 0;
    std::uint8_t version_ = 0;
    std::uint8_t sub_version_ = 0;
    std::uint8_t flag_ = 0;
};

//give to the main.cpp the head of the MP3_file
ID3 GetHeading( std::ifstream& source_file)
{  
    // amount of bayts in head of MP3_file
    static constexpr const int sizeID3 = 3;
    static const std::string ID3_head = "ID3";
    // array-buffer for the result Class of this func (C-style) 
    //+2 mean 1 is '\0' and another 1 for full size of ID3
    char signature[sizeID3+2] = {'\0'};

    //verification if file can be opened  
    if (!source_file.is_open())
    {
    throw std::invalid_argument("Incorrect source file name!"); 
    }

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
                source_file.read(signature, sizeof(std::uint8_t));
                id3.version_ = std::stoi(signature);
                break;
            }
            else if( i == 1 ) 
            {
                source_file.read(signature, sizeof(std::uint8_t));
                id3.sub_version_ = std::stoi(signature);
                break;
            }
            else if( i == 2 ) 
            {
                source_file.read(signature, sizeof(std::uint8_t));
                id3.flag_ = std::stoi(signature);
                break;
            }
            break;
        }
        else
        {
            source_file.read(signature, sizeof(std::uint32_t));
            id3.size_ = std::stoi(signature);
            break;
        }
        break;
    }
    return id3;

}
}// namespace MP3
