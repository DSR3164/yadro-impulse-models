#include "models.hpp"

#include <cstdio>
#include <iostream>
#include <fstream>
#include <cstring>
#include <memory>
#include <sstream>

constexpr uint64_t NS_IN_SEC = 1'000'000'000ULL;

void UniformTrafficModel::simulate(std::string result_path)
{
    while (current_sim_time_ns < overall_time_ns)
    {
        auto delay = inter_packet_delay();

        if (current_sim_time_ns + delay > overall_time_ns)
            break;

        generate_packet();
        current_sim_time_ns += delay;
        process_packet();
    }
    write_stats(result_path);
}

void UniformTrafficModel::generate_packet()
{
    current_packet.id++;
}

void UniformTrafficModel::process_packet()
{
    current_packet.processing_time_ns = current_sim_time_ns;
    stats.push_back(current_packet);
}

uint64_t UniformTrafficModel::inter_packet_delay()
{
    return time(gen);
}

void UniformTrafficModel::write_stats(const std::string path)
{
    std::ofstream csv(path);

    if (csv.is_open())
    {
        csv << "time_ns,size_bytes\n";
        for (auto &x : stats)
            csv << x.processing_time_ns << "," << x.size << "\n";
        csv.close();
    }
    else
    {
        std::cout << "CSV file \"" << path.c_str() << "\" not found !" << "\n";
        return;
    }
}

void PoissonTrafficModel::simulate(std::string result_path)
{
    while (current_sim_time_ns < overall_time_ns)
    {
        auto delay = inter_packet_delay();

        if (current_sim_time_ns + delay > overall_time_ns)
            break;

        generate_packet();
        current_sim_time_ns += delay;
        process_packet();
    }
    write_stats(result_path);
}

void PoissonTrafficModel::generate_packet()
{
    current_packet.id++;
    current_packet.size = static_cast<size_t>(size(gen));
}

void PoissonTrafficModel::process_packet()
{
    current_packet.processing_time_ns = current_sim_time_ns;
    stats.push_back(current_packet);
}

uint64_t PoissonTrafficModel::inter_packet_delay()
{
    return time(gen);
}

void PoissonTrafficModel::write_stats(const std::string path)
{
    std::ofstream csv(path);

    if (csv.is_open())
    {
        csv << "time_ns,size_bytes\n";
        for (auto &x : stats)
            csv << x.processing_time_ns << "," << x.size << "\n";
        csv.close();
    }
    else
    {
        std::cout << "CSV file \"" << path.c_str() << "\" not found !" << "\n";
        return;
    }
}

void parse_args(int argc, const char **argv, std::unique_ptr<TrafficModel> &model)
{
    if (argc < 2)
        return;

    double duration = 1.0;
    std::ifstream config(argv[1]);
    if (!config.is_open())
        return;

    std::string line;

    if (std::getline(config, line))
    {
        duration = std::stod(line);
        if (duration <= 0.0 || duration > 3600.0)
        {
            std::cout << "Too small or too big simulation time!\n"
                      << "Valid range 0 < x < 3600 seconds\n"
                      << "Value set to 10 second\n";
            duration = 10.0;
        }
        std::cout << "Simulation time: " << duration << " seconds\n";
    }

    if (std::getline(config, line))
    {
        std::istringstream iss(line);

        std::string type;
        iss >> type;

        if (type == "Poisson")
        {
            float lambda;
            uint64_t time;

            iss >> time >> lambda;
            if (lambda != 0.0f)
                lambda = 1.0f / lambda;
            else
                lambda = 1.0f / 700.0f;
            std::cout << "Setting Poisson model\n";
            model = std::make_unique<PoissonTrafficModel>(time, lambda, static_cast<uint64_t>(duration * NS_IN_SEC));
        }
        else if (type == "Uniform")
        {
            uint64_t mean_delay;
            size_t size;
            iss >> mean_delay >> size;
            std::cout << "Setting Uniform model\n";
            model = std::make_unique<UniformTrafficModel>(size, mean_delay, mean_delay, static_cast<uint64_t>(duration * NS_IN_SEC));
        }
    }
}
