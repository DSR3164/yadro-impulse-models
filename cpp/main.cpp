#include "models.hpp"

#include <cstring>
#include <memory>

int main(int argc, const char **argv)
{
    std::unique_ptr<TrafficModel> model;

    parse_args(argc, argv, model);

    model->simulate("../../files/stats.csv");

    return 0;
}
