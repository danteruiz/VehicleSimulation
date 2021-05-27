#pragma once

#include <string>
#include <chrono>
#include <iostream>

#include "spdlog/spdlog.h"

template <typename T> class StopWatch : T
{
public:
    explicit StopWatch(std::string const &category) : m_category(category)
    {
        T::start();
    }

    ~StopWatch()
    {
        int64_t duration = T::getMs();
        spdlog::info("duration: {}", duration);
    }

private:
    std::string m_category;
};

class ChronoTimerBase
{
public:
    ChronoTimerBase() :  m_start(std::chrono::high_resolution_clock::now()) { }
    void start()
    {
        m_start = std::chrono::high_resolution_clock::now();
    }

    int64_t getMs()
    {
        auto diff = std::chrono::high_resolution_clock::now() - m_start;
        return (int64_t) (std::chrono::duration_cast<std::chrono::milliseconds>(diff).count());
    }

private:
    std::chrono::high_resolution_clock::time_point m_start;
};

class Clock
{
public:
    Clock() : m_start(std::chrono::high_resolution_clock::now()) {}
    ~Clock() = default;

    float getDeltaTime()
    {
        auto current = std::chrono::high_resolution_clock::now();
        auto diff = current - m_start;
        m_start = current;
        return (float) (std::chrono::duration_cast<std::chrono::milliseconds>(diff).count() / 1000.0f);
    }

private:
    std::chrono::high_resolution_clock::time_point m_start;
};


using ChronoStopWatch = StopWatch<ChronoTimerBase>;


