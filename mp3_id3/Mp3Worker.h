#pragma once
#include <fstream>
#include <cstdint>

#include "ID3v2Frame.h"
#include "ID3v2Header.h"

namespace MP3
{
// besides size in main header of the whole MP3
constexpr static int size_of_version_flag = {3};

class Mp3Worker
{
friend std::ostream& operator<<( std::ostream& stream, Mp3Worker& mp3Worker );

public:
    Mp3Worker(std::ifstream&& path2file );

    std::uint32_t make_right_sizeadr(uint32_t size);

private:
    void ParseId3V2Header();
    void ReadFrame();
 
 
private:
    std::ifstream source_file_;
    ID3v2Header id3v2Header_;
    ID3v2Frame frame_;
}; // class Mp3Worker


// redefinition for my testing (will be deleted before release)
std::ostream& operator<<( std::ostream& stream, Mp3Worker& mp3Worker );

} // namespace MP3

