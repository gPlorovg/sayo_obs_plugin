#ifndef SUBTITLES_BUFFER_H
#define SUBTITLES_BUFFER_H

#include <string>
#include <deque>
#include <iostream>

class SubtitlesBuffer {
public:
    SubtitlesBuffer(size_t max_lines, size_t max_chars_per_line);

    void addWord(const std::string& word);

    [[nodiscard]] std::string getBufferContent() const;

private:
    size_t max_lines;
    size_t max_chars_per_line;
    std::deque<std::string> lines;
};

#endif

