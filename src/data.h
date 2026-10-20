#pragma once

#include <iosfwd>

class Date
{
public:
    Date();
    Date(int day, int month, int year);
    Date(const Date& other);
    Date(Date&& other) noexcept;
    Date& operator=(const Date& other);
    Date& operator=(Date&& other) noexcept;
    ~Date();

    bool is_valid() const;
    bool operator==(const Date& other) const;
    bool operator<(const Date& other) const;

    friend std::ostream& operator<<(std::ostream& out, const Date& date);

private:
    int day_;
    int month_;
    int year_;
};
