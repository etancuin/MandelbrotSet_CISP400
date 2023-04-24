#include "ComplexPlane.hpp"

using namespace sf;
using namespace std;

ComplexPlane::ComplexPlane(float aspectRatio)
{
  m_aspectRatio = aspectRatio;
  m_view.setSize(BASE_WIDTH, -(BASE_HEIGHT * m_aspectRatio));
  m_view.setCenter(0.0, 0.0);
  m_zoomCount = 0;
}
void ComplexPlane::zoomIn()
{
  m_zoomCount++;
  size_t width = BASE_WIDTH * pow(BASE_ZOOM, m_zoomCount);
  size_t height = BASE_HEIGHT * m_aspectRatio * pow(BASE_ZOOM, m_zoomCount);
  m_view.setSize(width, height);
}
void ComplexPlane::zoomOut()
{
  m_zoomCount--;
  size_t width = BASE_WIDTH * pow(BASE_ZOOM, m_zoomCount);
  size_t height = BASE_HEIGHT * m_aspectRatio * pow(BASE_ZOOM, m_zoomCount);
  m_view.setSize(width, height);
}
void ComplexPlane::setCenter(Vector2f coord)
{
  m_view.setCenter(coord.x / 2, coord.y / 2);
}
View ComplexPlane::getView()
{
  return m_view;
}
void ComplexPlane::setMouseLocation(Vector2f coord)
{
  m_mouseLocation = coord;
}
void ComplexPlane::loadText(Text& text)
{
  text.setString("Mandelbrot Set\nCenter: (" + to_string(m_view.getCenter().x) + ", " + to_string(m_view.getCenter().y)
      + ")\nCursor: (" + to_string(m_mouseLocation.x) + ", " + to_string(m_mouseLocation.y) + ")\nLeft-click to zoom in"
      + "\nRight-click to zoom out");
}
size_t ComplexPlane::countIterations(Vector2f coord)
{
  double r, i;
  r = coord.x;
  i = coord.y;
  complex<double> c(r, i);
  complex<double> z(0, 0);
  for(size_t i = 1; i < MAX_ITER; i++)
  {
    z = z * z + c;
    if(abs(z) >= 2.0)
      return i;
  }
  return MAX_ITER;
}
void ComplexPlane::iterationsToRGB(size_t count, Uint8& r, Uint8& g, Uint8& b)
{
  if(count < MAX_ITER / 5)
  {
    r = 127;
    g = 0;
    b = 255;
  }
  else if(count < 2 * (MAX_ITER / 5))
  {
    r = 0;
    g = 0;
    b = 255;
  }
  else if(count < 3 * (MAX_ITER / 5))
  {
    r = 0;
    g = 255;
    b = 0;
  }
  else if(count < 4 * (MAX_ITER / 5))
  {
    r = 255;
    g = 255;
    b = 0;
  }
  else if(count < MAX_ITER)
  {
    r = 255;
    g = 0;
    b = 0;
  }
  else if(count == MAX_ITER)
  {
    r = 0;
    g = 0;
    b = 0;
  }
}