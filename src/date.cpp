#include "date.h"
#include <cctype>
#include <limits>
#include <ostream>
#include <stdexcept>

Date::Date()
: day_(1), month_(1), year_(2000) {}

Date::Date(int day, int month, int year)
    : day_(day), month_(month), year_(year) {}

Date::Date(const Date& other)
    : day_(other.day_), month_(other.month_), year_(other.year_)
{}

Date::Date(Date&& other) noexcept
    : day_(other.day_), month_(other.month_), year_(other.year_)
{}

Date& Date::operator=(const Date& other)
{
    if (this != &other)
    {
        day_ = other.day_;
        month_ = other.month_;
        year_ = other.year_;
    }
    return *this;
}

Date& Date::operator=(Date&& other) noexcept
{
    if (this != &other)
    {
        day_ = other.day_;
        month_ = other.month_;
        year_ = other.year_;
    }
    return *this;
}

Date::~Date()
{}

bool Date::is_valid() const
{
    if (month_ > 12 || month_ < 1) return 0;
    if (year_ < 1) return 0;
    if (day_ < 1) return 0;

    int days_in_month = 31;
    if (month_ == 4 || month_ == 6 || month_ == 9 || month_ == 11) 
    {
        days_in_month = 30;
    } 
    else 
        if (month_ == 2) 
        {
        const bool leap_year = year_ % 400 == 0 ||
                               (year_ % 4 == 0 && year_ % 100 != 0);
        days_in_month = leap_year ? 29 : 28;
        }

    if (day_ <= days_in_month)
    {
        return 1;
    }
    return 0;
}

bool Date:: operator== (const Date& other) const
{
    if (day_ == other.day_ && month_ == other.month_ && year_ == other.year_)
    {
        return 1;
    }
    return 0;
}

bool Date::operator<(const Date& other) const
{
    if (year_ < other.year_)
    {
        return 1;
    }
    if (year_ > other.year_)
    {
        return 0;
    }
    if (month_ < other.month_)
    {
        return 1;
    }
    if (month_ > other.month_)
    {
        return 0;
    }
    if (day_ < other.day_)
    {
        return 1;
    }
    return 0;
}

std::ostream& operator<<(std::ostream& out, const Date& date)
{
    return out << date.day_ << '.' << date.month_ << '.' << date.year_;
}
