#include "commands.hpp"
#include <istream>
#include <vector>
#include <string>
#include <numeric>
#include <functional>
#include <algorithm>
#include "figures_struct.hpp"
#include <scope_guard.hpp>
#include <iomanip>

bool isOdd(const nikitov::Polygon& figure)
{
  return figure.points.size() % 2;
}

bool isEven(const nikitov::Polygon& figure)
{
  return !(figure.points.size() % 2);
}

bool isSize(const nikitov::Polygon& figure, size_t pointsNum)
{
  return figure.points.size() == pointsNum;
}

double accumulatePolygon(double result, const nikitov::Polygon& figure)
{
  result += figure.getArea();
  return result;
}

double accumulatePolygonIf(double result, const nikitov::Polygon& figure, std::function< bool(const nikitov::Polygon&) > predicate)
{
  if (predicate(figure))
  {
    result += figure.getArea();
  }
  return result;
}

bool vertexesComparator(const nikitov::Polygon& rhs, const nikitov::Polygon& lhs)
{
  return rhs.points.size() < lhs.points.size();
}

bool areaComparator(const nikitov::Polygon& rhs, const nikitov::Polygon& lhs)
{
  return rhs.getArea() < lhs.getArea();
}

void nikitov::areaCmd(const std::vector< Polygon >& data, std::istream& input, std::ostream& output)
{
  ScopeGuard scopeGuard(output);
  output << std::setprecision(1) << std::fixed;
  std::string parameter = {};
  input >> parameter;

  using namespace std::placeholders;
  if (parameter == "ODD")
  {
    std::function< double(double, const Polygon&) > accum = std::bind(accumulatePolygonIf, _1, _2, isOdd);
    output << std::accumulate(data.cbegin(), data.cend(), 0.0, accum);
  }
  else if (parameter == "EVEN")
  {
    std::function< double(double, const Polygon&) > accum = std::bind(accumulatePolygonIf, _1, _2, isEven);
    output << std::accumulate(data.cbegin(), data.cend(), 0.0, accum);
  }
  else if (parameter == "MEAN")
  {
    output << std::accumulate(data.cbegin(), data.cend(), 0.0, accumulatePolygon) / data.size();
  }
  else if (std::all_of(parameter.cbegin(), parameter.cend(), ::isdigit))
  {
    size_t vertexesNum = stoull(parameter);
    if (vertexesNum < 3)
    {
      output << "<INVALID COMMAND>";
      return;
    }
    std::function< bool(const Polygon&) > pred = std::bind(isSize, _1, vertexesNum);
    std::function< double(double, const Polygon&) > accum = std::bind(accumulatePolygonIf, _1, _2, pred);
    output << std::accumulate(data.cbegin(), data.cend(), 0.0, accum);
  }
  else
  {
    output << "<INVALID COMMAND>";
  }
}

void nikitov::maxCmd(const std::vector< Polygon >& data, std::istream& input, std::ostream& output)
{
  std::string parameter = {};
  input >> parameter;

  if (data.empty())
  {
    output << "<INVALID COMMAND>";
    return;
  }

  if (parameter == "AREA")
  {
    ScopeGuard scopeGuard(output);
    output << std::setprecision(1) << std::fixed;
    output << (*std::max_element(data.cbegin(), data.cend(), areaComparator)).getArea();
  }
  else if (parameter == "VERTEXES")
  {
    output << (*std::max_element(data.cbegin(), data.cend(), vertexesComparator)).points.size();
  }
  else
  {
    output << "<INVALID COMMAND>";
  }
}

void nikitov::minCmd(const std::vector< Polygon >& data, std::istream& input, std::ostream& output)
{
  std::string parameter = {};
  input >> parameter;

  if (data.empty())
  {
    output << "<INVALID COMMAND>";
    return;
  }

  if (parameter == "AREA")
  {
    ScopeGuard scopeGuard(output);
    output << std::setprecision(1) << std::fixed;
    output << (*std::min_element(data.cbegin(), data.cend(), areaComparator)).getArea();
  }
  else if (parameter == "VERTEXES")
  {
    output << (*std::min_element(data.cbegin(), data.cend(), vertexesComparator)).points.size();
  }
  else
  {
    output << "<INVALID COMMAND>";
  }
}

void nikitov::countCmd(const std::vector< Polygon >& data, std::istream& input, std::ostream& output)
{
  std::string parameter = {};
  input >> parameter;

  using namespace std::placeholders;
  if (parameter == "ODD")
  {
    output << std::count_if(data.cbegin(), data.cend(), isOdd);
  }
  else if (parameter == "EVEN")
  {
    output << std::count_if(data.cbegin(), data.cend(), isEven);
  }
  else if (std::all_of(parameter.cbegin(), parameter.cend(), ::isdigit))
  {
    size_t vertexesNum = stoull(parameter);
    if (vertexesNum < 3)
    {
      output << "<INVALID COMMAND>";
      return;
    }
    std::function< bool(const Polygon&) > pred = std::bind(isSize, _1, vertexesNum);
    output << std::count_if(data.cbegin(), data.cend(), pred);
  }
  else
  {
    output << "<INVALID COMMAND>";
  }
}