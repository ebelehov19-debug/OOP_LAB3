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

    bool add(const Date&);
    bool remove(const Date&);
    bool contains(const Date&) const;
    bool operator() (const Date&) const;

    std::size_t size() const;
    bool empty() const;

    DateSet& operator+=(const Date&);
    DateSet& operator-=(const Date&);

    DateSet operator| (const DateSet&) const;
    DateSet operator& (const DateSet&) const;
    DateSet operator- (const DateSet&) const;

    bool operator== (const DateSet&) const;
    bool operator!= (const DateSet&) const;

    friend std::ostream& operator<<(std::ostream&, const DateSet&);

};
