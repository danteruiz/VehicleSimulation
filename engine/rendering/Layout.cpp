#include "Layout.h"


size_t Attribute::getTotalOffset() const
{
    return m_offset * m_format.getSize();
}
