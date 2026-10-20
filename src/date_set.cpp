#include "date_set.h"
#include <cctype>
#include <limits>
#include <ostream>
#include <stdexcept>
#include <utility>

DateSet::DateSet()
{
    data_ = new Date[0];
    size_ = 0;
    capacity_ = 0;

}
DateSet::~DateSet()
{
    delete[] data_;
}
DateSet::  DateSet(const DateSet& other)
{
    size_ = other.size_;
    capacity_ = other.capacity_;
    data_ = new Date[capacity_];

    for(std::size_t i =0;i<size_;++i)
    {
        data_[i] = other.data_[i];
    }
}

 DateSet:: DateSet(DateSet&& other) noexcept
 {
    data_ = other.data_;
    size_ = other.size_;
    capacity_ = other.capacity_;

    other.data_ = nullptr;
    other.size_ = 0;
    other.capacity_ = 0;
 }

DateSet& DateSet::operator=(const DateSet& other)
{
    if (this != &other)
    {
        DateSet copy(other);
        std::swap(data_, copy.data_);
        std::swap(size_, copy.size_);
        std::swap(capacity_, copy.capacity_);
    }
    return *this;
}

DateSet& DateSet::operator=(DateSet&& other) noexcept
{
    if (this != &other)
    {
        delete[] data_;
        data_ = other.data_;
        size_ = other.size_;
        capacity_ = other.capacity_;

        other.data_ = nullptr;
        other.size_ = 0;
        other.capacity_ = 0;
    }
    return *this;
}

size_t DateSet:: size() const
{
    return size_;
}

bool DateSet:: empty() const
{
    return size_ == 0;
}
