#include "subtitle_buffer.h"
#include <string>
#include <deque>
#include <sstream>

SubtitlesBuffer::SubtitlesBuffer(size_t max_lines, size_t max_chars_per_line)
    : max_lines(max_lines), max_chars_per_line(max_chars_per_line) {}

void SubtitlesBuffer::addWord(const std::string& word) {
    // Проверяем, можем ли мы добавить слово в текущую строку
    if (!lines.empty() && lines.back().size() + word.size() + 1 <= max_chars_per_line) {
        // Если слово помещается в последнюю строку, добавляем его туда
        lines.back() += word;
    } else {
        // Если слово не влезает в последнюю строку, создаем новую строку
        if (lines.size() == max_lines) {
            // Удаляем самую старую строку, если буфер переполнен
            lines.pop_front();
        }
        lines.push_back(word);
    }
}

std::string SubtitlesBuffer::getBufferContent() const {
    std::string content;
    for (const auto& line : lines) {
        content += line + "\n";
    }
    return content;
}