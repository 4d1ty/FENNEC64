#include "vixen/vconsole.hpp"
#include <iostream>

VirtualConsole::VirtualConsole(int width, int height, int charSize, sf::Font &font)
    : m_width(width), m_height(height), m_charSize(charSize), font(font)
{
    m_buffer.resize(m_height, std::string(m_width, ' '));
}

void VirtualConsole::clear()
{
    m_buffer.clear();
    // m_buffer.resize(m_height, std::string(m_width, ' '));
    m_cursorX = 0;
    m_cursorY = 0;
}

void VirtualConsole::drawPixel(int x, int y, sf::Color color)
{
    if (x < 0 || x >= m_width || y < 0 || y >= m_height)
        return; // Out of bounds
    // Set pixel color in framebuffer
    m_frameBuffer[y][x] = color;
}

void VirtualConsole::putChar(char c)
{
    // if (c == '\n')
    // {
    //     m_cursorX = 0;
    //     m_cursorY++;
    // }
    if (c == '\r')
    {
        m_cursorX = 0;
    }
    if (c == '\t')
    {
        int spacesToAdd = 4 - (m_cursorX % 4);
        for (int i = 0; i < spacesToAdd; ++i)
        {
            putChar(' ');
        }
    }
    else
    {
        m_buffer[m_cursorY][m_cursorX] = c;
        m_cursorX++;
    }

    if (m_cursorX >= m_width)
    {
        m_cursorX = 0;
        m_cursorY++;
    }

    if (m_cursorY >= m_height)
    {
        scrollUp();
    }
}

void VirtualConsole::print(const std::string &text, int x, int y)
{
    m_cursorX = x;
    m_cursorY = y;
    for (char c : text)
    {
        putChar(c);
    }
}
void VirtualConsole::print(const std::string &text)
{
    for (char c : text)
    {
        putChar(c);
    }
}

void VirtualConsole::scrollUp()
{
    // Shift all lines up by 1
    for (int i = 0; i < m_height - 1; i++)
        m_buffer[i] = m_buffer[i + 1];

    // Clear the last line
    m_buffer[m_height - 1] = std::string(m_width, ' ');

    if (m_cursorY > 0)
        m_cursorY--;
}

void VirtualConsole::render(sf::RenderWindow &window)
{

     // Draw the framebuffer pixels
    for (int y = 0; y < m_height; ++y)
    {
        for (int x = 0; x < m_width; ++x)
        {
            sf::Color color = m_frameBuffer[y][x];
            if (color.a != 0) // Only draw if pixel is set
            {
                sf::RectangleShape pixel(sf::Vector2f(1.f, 1.f));
                pixel.setPosition(sf::Vector2f({static_cast<float>(x), static_cast<float>(y)}));
                pixel.setSize(sf::Vector2f(static_cast<float>(pixelSize), static_cast<float>(pixelSize)));
                pixel.setFillColor(color);
                window.draw(pixel);
            }
        }
    }

    sf::Time elapsed = m_clock.getElapsedTime();
    if (elapsed.asMilliseconds() >= m_cursorBlinkInterval)
    {
        m_clock.restart();
    }

    for (int y = 0; y < m_height; y++)
    {
        sf::Text lineText(font);
        lineText.setFont(font);
        lineText.setString(m_buffer[y]);
        lineText.setCharacterSize(m_charSize);
        lineText.setFillColor(sf::Color::White);
        lineText.setPosition({0.f, static_cast<float>(y * m_charSize)});
        window.draw(lineText);
    }

    sf::RectangleShape cursor(
        sf::Vector2f(static_cast<float>(m_charSize / 2), static_cast<float>(m_charSize)));
    cursor.setFillColor(sf::Color(255, 255, 255, 80));
    cursor.setPosition(sf::Vector2f({static_cast<float>(m_cursorX * m_charSize), static_cast<float>(m_cursorY * m_charSize)}));

    if (elapsed.asMilliseconds() < m_cursorBlinkInterval / 2)
    {
        return; // Don't draw cursor in the off phase
    }

   
    // window.draw(cursor);
}
