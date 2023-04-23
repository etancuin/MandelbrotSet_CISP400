#include "ComplexPlane.hpp"
using namespace sf;
using namespace std;

/*
  we need to figure out what professor means by pixelWidth, he mentioned it in the description of the
  calculating section but never actually said what it is.
*/

int main() 
{
  size_t height = VideoMode::getDesktopMode().height;
  size_t width = VideoMode::getDesktopMode().width;
  
  RenderWindow window(VideoMode(width, height), "Mandelbrot Set", Style::Fullscreen);
  
  ComplexPlane mandelbrot(height / width);

  Font font;
  font.loadFromFile("arial.ttf");

  Text text;
  text.setFont(font);
  text.setCharacterSize(18);
  text.setPosition(10, 10);

  VertexArray points(Points, height * width);

  enum State = {CALCULATING, DISPLAYING};
  State state = CALCULATING;
  
  while(window.isOpen())
  {
    Event event;
    while(window.pollEvent(event))
    {
      switch(event.type)
      {
        case Event::Closed:
          window.close();
          break;
        case Event::KeyPressed:
          if(event.key.code == Keyboard::Escape)
            window.close();
          break;
        case Event::MouseButtonPresed:
          if(event.mouseButton.button == Mouse::Left)
          {
            mandelbrot.zoomIn();
          }
          else if(event.mouseButton.button == Mouse::Right)
          {
            mandelbrot.zoomOut();
          }
          Vector2f coord = window.mapPixelToCoords(event.mouseButton.x, event.mouseButton.y, mandelbrot.getView);
          mandelbrot.setCenter(coord);
          state = CALCULATING;
          break;
        case Event::MouseMoved;
          Vector2f coord = window.mapPixelToCoords(Vector2i(event.mouseMove.x, event.mouseMove.y), mandelbrot.getView());
          mandelbrot.setMouseLocation(coord);
          break;
      }
    }

    if(state == CALCULATING)
    {
      //may also need to flip the inner and outer loop for statements
      for(int y = 0; y < VideoMode::getDesktopMode().height; y++)
      {
        for(int x = 0; x < VideoMode::getDesktopMode().width; x++)
        {
          //note not actually sure what to use for pixelWidth
          points[x + y * pixelWidth].position = {x.f, y.f};
          Vector2f coord = window.mapPixelToCoords(Vector2i(x, y), mandelbrot.getView());
          size_t iterations = mandelbrot.countIterations(coord);
          Uint8 r, g, b;
          mandelbrot.iterationsToRGB(iterations, r, g, b);
          //note not actually sure what to use for pixelWidth
          points[x + y * pixelWidth].color = {r, g, b};
          mandelbrot.loadText(text);
          
        }
      }
      state = DISPLAYING;
    }

    window.clear();
    window.draw(text);
    for(int y = 0; y < VideoMode::getDesktopMode().height; y++)
    {
      for(int x = 0; x < VideoMode::getDesktopMode().width; x++)
      {
        //note not actually sure what to use for pixelWidth
        window.draw(points[x + y * pixelWidth]);
      }
    }
    window.display();
  }
}