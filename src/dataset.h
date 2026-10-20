#pragma once

#include "data.h"

class DateSet
{
public:
    DateSet();
    DateSet(const DateSet&);
    DateSet(DateSet&&) noexcept;
    DateSet& operator=(const DateSet&);
    DateSet& operator=(DateSet&&) noexcept;
    ~DateSet();
};
