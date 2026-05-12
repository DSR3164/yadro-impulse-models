#pragma once

#include <cstdio>
#include <random>
#include <memory>
#include <iostream>

struct Packet
{
    size_t id = 0;
    size_t size = 84;
    uint64_t processing_time_ns;
};

class TrafficModel
{
public:
    virtual ~TrafficModel() = default;

    virtual void simulate(std::string result_path) = 0;
};

class UniformTrafficModel : public TrafficModel
{
public:
    UniformTrafficModel(size_t packet_size = 700, uint64_t min_ns = 20'000, uint64_t max_ns = 20'000, uint64_t duration_ns = 10'000'000'000)
        : overall_time_ns(duration_ns)
    {
        stats.clear();
        auto packets_expected = duration_ns / ((min_ns + max_ns) / 2);
        std::cout << "Packets expected: " << packets_expected << std::endl;
        stats.reserve(packets_expected);
        time.param(std::uniform_int_distribution<uint64_t>::param_type(min_ns, max_ns));
        current_packet.size = packet_size;
    }

    void simulate(std::string result_path);

private:
    void write_stats(const std::string path);
    void generate_packet();
    void process_packet();
    uint64_t inter_packet_delay();

    std::random_device rd;
    std::mt19937 gen{rd()};

    std::uniform_int_distribution<uint64_t> time;
    Packet current_packet;
    std::vector<Packet> stats;

    uint64_t current_sim_time_ns = 0;
    uint64_t overall_time_ns = 0;
};

class PoissonTrafficModel : public TrafficModel
{
public:
    PoissonTrafficModel(uint64_t mean_time_ns = 20'000, float packet_size_lambda = 1.0f / 700.0f, uint64_t duration_ns = 10'000'000'000)
        : overall_time_ns(duration_ns)
    {
        stats.clear();
        auto packets_expected = duration_ns / mean_time_ns;
        std::cout << "Packets expected: " << packets_expected << std::endl;
        stats.reserve(packets_expected);
        time.param(std::poisson_distribution<uint64_t>::param_type(mean_time_ns));
        size.param(std::exponential_distribution<float>::param_type(packet_size_lambda));
    }

    void simulate(std::string result_path);

private:
    void generate_packet();
    void process_packet();
    uint64_t inter_packet_delay();
    void write_stats(const std::string path);

    std::random_device rd;
    std::mt19937 gen{rd()};

    std::string config_path;

    std::exponential_distribution<float> size;
    std::poisson_distribution<uint64_t> time;
    Packet current_packet;
    std::vector<Packet> stats;

    uint64_t current_sim_time_ns = 0;
    uint64_t overall_time_ns = 0;
};

void parse_args(int argc, const char **argv, std::unique_ptr<TrafficModel> &model);
