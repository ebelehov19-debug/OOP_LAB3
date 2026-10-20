#include "data.h"
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
    if (month_ > 12 || month_ < 1) return false;
    if (year_ < 1) return false;
    if (day_ < 1) return false;

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

    return day_ <= days_in_month;
}

bool Date:: operator== (const Date& other) const
{
    return day_ == other.day_ && month_ == other.month_ && year_ == other.year_;
}

bool Date::operator<(const Date& other) const
{
    if (year_ != other.year_) 
    {
        return year_ < other.year_;
    }
    if (month_ != other.month_) 
    {
        return month_ < other.month_;
    }
    return day_ < other.day_;
}

std::ostream& operator<<(std::ostream& out, const Date& date)
{
    return out << date.day_ << '.' << date.month_ << '.' << date.year_;
}
