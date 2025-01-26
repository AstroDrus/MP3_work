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
    static constexpr const int size_of_tag = 9;
    static constexpr const int sizeID3 = 3;
    static const std::string ID3_head = "ID3";
    // massiv for the result of this func (C-style -> std::string)
    char tag[size_of_tag+1] = {'\0'};

    //verification if file can be opened  
    if (!source_file.is_open())
	{
		throw std::invalid_argument("Incorrect source file name!"); 
    }

    source_file.read(tag, sizeID3);
    if (ID3_head != tag)
    {
        throw std::invalid_argument("source file is not an MP3 file");
    }
    ID3 id3{};
    source_file.read(tag, sizeof(std::uint8_t));

    for (int i = 0; i <= size_of_tag - sizeID3; ++i) 
    {
        if (i < 3)
        {
            if( i == 0 ) 
            {
                source_file.read(tag, sizeof(std::uint8_t));
                id3.version_ = std::stoi(tag);
                break;
            }
            else if( i == 1 ) 
            {
                source_file.read(tag, sizeof(std::uint8_t));
                id3.sub_version_ = std::stoi(tag);
                break;
            }
            else if( i == 2 ) 
            {
                source_file.read(tag, sizeof(std::uint8_t));
                id3.flag_ = std::stoi(tag);
                break;
            }
            break;
        }
        else
        {
            source_file.read(tag, sizeof(std::uint32_t));
            id3.size_ = std::stoi(tag);
            break;
        }
        break;
    }
    return id3;

}
}// namespace MP3
