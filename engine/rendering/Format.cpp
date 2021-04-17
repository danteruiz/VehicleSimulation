#include "Format.h"

constexpr int TYPE_SIZE[Type::TypeNum] = {
    4,
    2,
    4,
    2,
    1,
    4,
    2,
    1
};

constexpr int DIMENSION_SIZE[Dimension::DimensionNum] = {
    1,
    2,
    3,
    4
};

uint32_t Format::getSize() const
{
    return TYPE_SIZE[m_type];
}

uint32_t Format::getStride() const
{
    return TYPE_SIZE[m_type] * DIMENSION_SIZE[m_dimension];
}

uint32_t Format::getDimensionSize() const
{
    return DIMENSION_SIZE[m_dimension];
}
