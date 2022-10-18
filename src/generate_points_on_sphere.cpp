#include <vector>
#include <algorithm>
#include <iostream>
#include <cmath>
#include <random>
#include <fstream>
#include <iterator>

constexpr static auto PI{3.14159265358979323846};

namespace type
{
    struct Point
    {
        double theta;
        double phi;
        double x;
        double y;
        double z;
        Point(double const theta, double const phi) noexcept
            : theta{theta}
            , phi{phi}
            , x{std::sin(phi) * std::cos(theta)}
            , y{std::sin(phi) * std::sin(theta)}
            , z{std::cos(phi)}
        {
        }
    };
} // namespace type

namespace utility
{
    constexpr static auto construct_generator{[](auto const min, auto const max) noexcept
    {
        return [gen{std::mt19937{std::random_device{}()}}, dist{std::uniform_real_distribution<>{min, max}}]() mutable noexcept { return dist(gen); };
    }};

    constexpr static auto write_to_file{[](auto const &points, auto const &filename) noexcept
    {
        std::ofstream file{filename};
        file << "theta,phi,x,y,z\n";
        std::for_each(std::cbegin(points), std::cend(points), [&file](auto const &p) noexcept
        {
            file << p.theta << ',' << p.phi << ',' << p.x << ',' << p.y << ',' << p.z << '\n';
        });
        file.close();
    }};
} // namespace utility

namespace etkin
{
    constexpr static auto generate_points{[](auto &rng, auto const num) noexcept
    {
        std::vector<type::Point> out{};
        std::generate_n(std::back_inserter(out), num, [&rng]() noexcept
        {
            return type::Point{2 * PI * rng(), std::acos(1 - 2 * rng())};
        });
        return out;
    }};
} // namespace etkin

int main(int argc, char const *argv[])
{
    auto rng{utility::construct_generator(0.0, 1.0)};
    auto const points{etkin::generate_points(rng, 1000)};
    utility::write_to_file(points, "data.csv");
    return 0;
}
