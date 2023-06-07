

#include "TextBox.h"


TextBox::TextBox(sf::RenderWindow& game_window, std::string text_string) : window(game_window)
{
  text.setString("");
  if (!font.loadFromFile("Data/Fonts/Legend Bold.otf"))
  {
    std::cerr << "Font did not load - String: " << text_string << std::endl;
  }
  text.setFont(font);
  text.setString(text_string);
  text.setCharacterSize(80);
  text.setOrigin(text.getLocalBounds().width/2, text.getLocalBounds().height/2);
  text.setFillColor(sf::Color::White);
}




void TextBox::render()
{
  if (visible)
  {
    window.draw(text);
  }
}

TextBox::~TextBox()
{

}
