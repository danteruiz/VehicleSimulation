#pragma once

#include <cstdint>

#include "Format.h"

enum class Slot : uint8_t
{
    Position = 0,
    Normal,
    TexCoord,
    Color
};

struct Attribute
{
    Attribute(Slot slot, Format format, size_t offset)
        : m_format(format), m_slot(slot), m_offset(offset) {}

    size_t getTotalOffset() const;
    Format m_format;
    Slot m_slot { 0 };
    size_t m_offset { 0 };
};
