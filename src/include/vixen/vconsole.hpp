#ifndef VIRTUAL_CONSOLE_HPP
#define VIRTUAL_CONSOLE_HPP

#include <SFML/Graphics.hpp>
#include <string>
#include <memory>

class VirtualConsole
{
    /*
    A simple virtual console that allows printing text at specific
    coordinates and rendering it onto an SFML RenderWindow.
    */
public:
    VirtualConsole(int width,int height,int charSize, sf::Font &font);
    void clear();
    void putChar(char c);
    void print(const std::string &text, int x,int y);
    void print(const std::string &text);
    void render(sf::RenderWindow &window);

private:
    int m_width;
    int m_height;
    int m_charSize;
    int m_padding = 2;
    sf::Font &font;
    std::vector<std::string> m_buffer;
    int m_cursorX = 0;
    int m_cursorY = 0;
    int m_cursorBlinkTimer = 0;
    int m_cursorBlinkInterval = 500; // milliseconds
    sf::Clock m_clock;
    void scrollUp();
};

#endif // VIRTUAL_CONSOLE_HPP