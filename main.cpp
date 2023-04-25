#include "ComplexPlane.hpp"

using namespace sf;
using namespace std;

int main() 
{
  size_t height = VideoMode::getDesktopMode().height;
  size_t width = VideoMode::getDesktopMode().width;
  
  RenderWindow window(VideoMode(width, height), "Mandelbrot Set", Style::Default);
  
  ComplexPlane mandelbrot((float)height /(float)width);

  Font font;
  font.loadFromFile("helvetica.ttf");

  Text text;
  text.setFont(font);
  text.setCharacterSize(25);
  text.setPosition(10, 10);

  VertexArray points(Points, height * width);

  enum State{CALCULATING, DISPLAYING};
  State state = CALCULATING;
  
  while(window.isOpen())
  {
    Event event;
    while(window.pollEvent(event))
    {
      switch(event.type)
      {
        case Event::Closed:
        {
          window.close();
          break;
        }
        case Event::KeyPressed:
        {
          if(event.key.code == Keyboard::Escape)
            window.close();
          break;
        }  
        case Event::MouseButtonPressed:
        {
          Vector2f coord = window.mapPixelToCoords(Vector2i(event.mouseButton.x, event.mouseButton.y), mandelbrot.getView());
          mandelbrot.setCenter(coord);
          if(event.mouseButton.button == Mouse::Left)
            mandelbrot.zoomIn();
          else if(event.mouseButton.button == Mouse::Right)
            mandelbrot.zoomOut();
          state = CALCULATING;
          break;
        }
        case Event::MouseMoved:
        {
          Vector2f coord = window.mapPixelToCoords(Vector2i(event.mouseMove.x, event.mouseMove.y), mandelbrot.getView());
          mandelbrot.setMouseLocation(coord);
          break;
        }
      }
    }

    if(state == CALCULATING)
    {
      for(size_t j = 0; j < width; j++)
      {
        for(size_t i = 0; i < height; i++)
        {
          points[j + i * width].position = {(float)j, (float)i};
          Vector2f coord = window.mapPixelToCoords(Vector2i(j, i), mandelbrot.getView());
          size_t iterations = mandelbrot.countIterations(coord);
          Uint8 r, g, b;
          mandelbrot.iterationsToRGB(iterations, r, g, b);
          points[j + i * width].color = {r, g, b};
        }
      }
      state = DISPLAYING;
    }
    
    mandelbrot.loadText(text);
    window.clear();
    window.draw(points);
    window.draw(text);
    window.display();
  }
}
