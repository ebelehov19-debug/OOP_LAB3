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
    if (size_ == 0)
    {
        return 1;
    }
    return 0;
}

bool DateSet:: contains(const Date& date) const
{
    std:: size_t left = 0;
    std:: size_t right = size_;

    while(left<right)
    {
        std:: size_t mid = left + (right - left)/2;
        if(data_[mid] == date) return 1;

        if(data_[mid]<date) 
        {
            left = mid+1;
        }
        else 
        {
            right = mid;
        }
    }
    return 0;
}

bool DateSet:: operator() (const Date& date) const
{
    if (contains(date))
    {
        return 1;
    }
    return 0;
}

bool DateSet:: add(const Date& date)
{
    if(!date.is_valid())
    {
        return 0;
    }

    if(contains(date))
    {
        return 0;
    }

    if(size_ == capacity_)
    {
        std::size_t new_capacity = capacity_ == 0 ? 1 : capacity_ * 2;
        Date* new_data = new Date[new_capacity];

        for (std::size_t i = 0; i < size_; ++i)
        {
            new_data[i] = data_[i];
        }

        delete[] data_;
        data_ = new_data;
        capacity_ = new_capacity;
    }

    std::size_t pos = size_;

    while (pos > 0 && date < data_[pos - 1])
    {
        data_[pos] = data_[pos - 1];
        --pos;
    }

    data_[pos] = date;
    ++size_;

    return 1;
}

 bool DateSet:: remove(const Date& date)
 {
     std::size_t left = 0;
    std::size_t right = size_;

    while (left < right)
    {
        std::size_t mid = left + (right - left) / 2;

        if (data_[mid] == date)
        { for (std::size_t i = mid + 1; i < size_; ++i)
            {
                data_[i - 1] = data_[i];
            }

            --size_;
            return 1;
        }
         if (data_[mid] < date)
        {
            left = mid + 1;
        }
        else
        {
            right = mid;
        }
    }
    return 0;
 }

DateSet& DateSet:: operator+=(const Date& date)
{
    add(date);
    return *this;
}

DateSet& DateSet:: operator-=(const Date& date)
{
    remove(date);
    return *this;
}

DateSet DateSet::operator|(const DateSet& dateset) const
{
    DateSet result;
    const std::size_t new_capacity = size_ + dateset.size_;
    if (new_capacity == 0)
    {
        return result;
    }

    Date* new_data = new Date[new_capacity];
    delete[] result.data_;
    result.data_ = new_data;
    result.capacity_ = new_capacity;

    std::size_t i = 0;
    std::size_t j = 0;

    while (i < size_ && j < dateset.size_)
    {
        if (data_[i] < dateset.data_[j])
        {
            result.data_[result.size_++] = data_[i++];
        }
        else if (dateset.data_[j] < data_[i])
        {
            result.data_[result.size_++] = dateset.data_[j++];
        }
        else
        {
            result.data_[result.size_++] = data_[i++];
            ++j;
        }
    }

    while (i < size_)
    {
        result.data_[result.size_++] = data_[i++];
    }
    while (j < dateset.size_)
    {
        result.data_[result.size_++] = dateset.data_[j++];
    }

    return result;
}

DateSet DateSet::operator&(const DateSet& dateset) const
{
    DateSet result;
    const std::size_t new_capacity = size_ < dateset.size_ ? size_ : dateset.size_;
    if (new_capacity == 0)
    {
        return result;
    }

    Date* new_data = new Date[new_capacity];
    delete[] result.data_;
    result.data_ = new_data;
    result.capacity_ = new_capacity;

    std::size_t i = 0;
    std::size_t j = 0;

    while (i < size_ && j < dateset.size_)
    {
        if (data_[i] < dateset.data_[j])
        {
            ++i;
        }
        else if (dateset.data_[j] < data_[i])
        {
            ++j;
        }
        else
        {
            result.data_[result.size_++] = data_[i++];
            ++j;
        }
    }

    return result;
}

DateSet DateSet::operator-(const DateSet& dateset) const
{
    DateSet result;
    if (size_ == 0)
    {
        return result;
    }

    Date* new_data = new Date[size_];
    delete[] result.data_;
    result.data_ = new_data;
    result.capacity_ = size_;

    std::size_t i = 0;
    std::size_t j = 0;

    while (i < size_ && j < dateset.size_)
    {
        if (data_[i] < dateset.data_[j])
        {
            result.data_[result.size_++] = data_[i++];
        }
        else if (dateset.data_[j] < data_[i])
        {
            ++j;
        }
        else
        {
            ++i;
            ++j;
        }
    }

    while (i < size_)
    {
        result.data_[result.size_++] = data_[i++];
    }

    return result;
}

bool DateSet::operator==(const DateSet& dateset) const
{
    if (size_ != dateset.size_)
    {
        return 0;
    }

    for (std::size_t i = 0; i < size_; ++i)
    {
        if (!(data_[i] == dateset.data_[i]))
        {
            return 0;
        }
    }

    return 1;
}

bool DateSet::operator!=(const DateSet& dateset) const
{
    if (*this == dateset)
    {
        return 0;
    }
    return 1;
}
std::ostream& operator<<(std::ostream& ofs, const DateSet& dateset)
{
    ofs << '{';

    for (std::size_t i = 0; i < dateset.size_; ++i)
    {
        if (i > 0)
        {
            ofs << ',';
        }
        ofs << ' ' << dateset.data_[i];
    }

    ofs << " }";
    return ofs;
}
