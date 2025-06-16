#include "subtitle_buffer.h"
#include <string>
#include <deque>
#include <sstream>
#include <vector>

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

void SubtitlesBuffer::changeSize(const size_t new_max_lines, const size_t new_max_chars_per_line) {

    std::vector<std::string> tokens;
    for (const auto& line : lines) {
        size_t i = 0;
        while (i < line.size()) {
            if (std::isspace(line[i])) {
                size_t start = i;
                while (i < line.size() && std::isspace(line[i])) ++i;
                tokens.push_back(line.substr(start, i - start)); // пробелы
            } else {
                size_t start = i;
                while (i < line.size() && !std::isspace(line[i])) ++i;
                tokens.push_back(line.substr(start, i - start)); // слово
            }
        }
    }

    max_lines = new_max_lines;
    max_chars_per_line = new_max_chars_per_line;

    lines.clear();
    for (const auto& word : tokens) {
        addWord(word);
    }
}