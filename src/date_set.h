#pragma once

#include "date.h"
#include <iosfwd>
#include <cstddef>

class DateSet
{
    Date* data_;
    std:: size_t size_;
    std:: size_t capacity_;
public:
    DateSet();
    DateSet(const DateSet& other);
    DateSet(DateSet&& other) noexcept;
    DateSet& operator=(const DateSet& other);
    DateSet& operator=(DateSet&& other) noexcept;
    ~DateSet();

    bool add(const Date& date);
    bool remove(const Date& date);
    bool contains(const Date& date) const;
    bool operator() (const Date& date) const;

    std::size_t size() const;
    bool empty() const;

    DateSet& operator+=(const Date& date);
    DateSet& operator-=(const Date& date);

    DateSet operator| (const DateSet& dateset) const;
    DateSet operator& (const DateSet& dateset) const;
    DateSet operator- (const DateSet& dateset) const;

    bool operator== (const DateSet& dateset) const;
    bool operator!= (const DateSet& dateset) const;

    friend std::ostream& operator<<(std::ostream& ofs, const DateSet& dateset);

};
