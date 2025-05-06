#pragma once
#include <fstream>
#include <string>
#include <cstdint>
#include <vector>
#include <algorithm>
#include <cstdint>

#include "GetHeader.h"
#include "GetFrame.h"

namespace MP3
{

class Mp3Worker
{
friend std::ostream& operator<<( std::ostream& stream, Mp3Worker& mp3Worker );

public:
    Mp3Worker(std::ifstream&& path2file ) 
    : source_file_(std::move(path2file))
    {
        ParseId3V2Header();
        ReadFrame();
    }

private:
    void ParseId3V2Header();
    void ReadFrame();
 
 
private:
    std::ifstream source_file_;
    ID3v2Header id3v2Header_;
    ID3v2Frame frame_;
}; // class Mp3Worker
 
 
std::ostream& operator<<( std::ostream& stream, Mp3Worker& mp3Worker );

} // namespace MP3

