#include "date.h"

#include <gtest/gtest.h>

#include <cstddef>
#include <sstream>
#include <utility>

TEST(DateTest, DefaultConstructorCreatesValidDate)
{
    const Date date;
    EXPECT_TRUE(date.is_valid());
}

TEST(DateTest, ParameterizedConstructorPreservesFields)
{
    const Date date(29, 2, 2024);
    std::ostringstream output;
    output << date;

    EXPECT_TRUE(date.is_valid());
    EXPECT_EQ(output.str(), "29.2.2024");
}

TEST(DateTest, AcceptsValidDatesAndLeapYears)
{
    const Date dates[] = {
        Date(1, 1, 1), Date(31, 1, 2026), Date(28, 2, 2026),
        Date(29, 2, 2000), Date(29, 2, 2024), Date(29, 2, 2400),
        Date(31, 3, 2026), Date(30, 4, 2026), Date(31, 5, 2026),
        Date(30, 6, 2026), Date(31, 7, 2026), Date(31, 8, 2026),
        Date(30, 9, 2026), Date(31, 10, 2026), Date(30, 11, 2026),
        Date(31, 12, 9999)
    };

    for (const Date& date : dates)
    {
        EXPECT_TRUE(date.is_valid()) << date;
    }
}

TEST(DateTest, RejectsInvalidDatesAndNonLeapCenturies)
{
    const Date dates[] = {
        Date(0, 1, 2026), Date(-1, 1, 2026), Date(32, 1, 2026),
        Date(1, 0, 2026), Date(1, -1, 2026), Date(1, 13, 2026),
        Date(1, 1, 0), Date(1, 1, -1),
        Date(29, 2, 1900), Date(29, 2, 2026), Date(29, 2, 2100),
        Date(30, 2, 2000), Date(31, 4, 2026), Date(31, 6, 2026),
        Date(31, 9, 2026), Date(31, 11, 2026)
    };

    for (const Date& date : dates)
    {
        EXPECT_FALSE(date.is_valid()) << date;
    }
}

TEST(DateTest, EqualityComparesAllThreeFields)
{
    const Date date(15, 3, 2026);

    EXPECT_TRUE(date == Date(15, 3, 2026));
    EXPECT_TRUE(date == date);
    EXPECT_FALSE(date == Date(16, 3, 2026));
    EXPECT_FALSE(date == Date(15, 4, 2026));
    EXPECT_FALSE(date == Date(15, 3, 2027));
}

TEST(DateTest, OrderingIsChronologicalAndStrict)
{
    const Date dates[] = {
        Date(31, 12, 1999), Date(1, 1, 2000), Date(28, 2, 2000),
        Date(29, 2, 2000), Date(1, 3, 2000), Date(2, 3, 2000),
        Date(1, 1, 2026)
    };
    const std::size_t count = sizeof(dates) / sizeof(dates[0]);

    for (std::size_t i = 0; i < count; ++i)
    {
        for (std::size_t j = 0; j < count; ++j)
        {
            EXPECT_EQ(dates[i] < dates[j], i < j) << dates[i] << " / " << dates[j];
        }
    }
}

TEST(DateTest, CopyConstructorCreatesIndependentValue)
{
    const Date original(15, 3, 2026);
    Date copy(original);

    EXPECT_EQ(copy, original);
    copy = Date(1, 1, 2000);
    EXPECT_EQ(original, Date(15, 3, 2026));
    EXPECT_EQ(copy, Date(1, 1, 2000));
}

TEST(DateTest, CopyAssignmentCopiesValueAndReturnsReference)
{
    const Date original(15, 3, 2026);
    Date target(1, 1, 2000);

    EXPECT_EQ(&(target = original), &target);
    EXPECT_EQ(target, original);
    target = Date(2, 2, 2002);
    EXPECT_EQ(original, Date(15, 3, 2026));
}

TEST(DateTest, MoveConstructorTransfersValueAndAllowsSourceReuse)
{
    Date source(15, 3, 2026);
    Date moved(std::move(source));

    EXPECT_EQ(moved, Date(15, 3, 2026));
    source = Date(1, 1, 2000);
    EXPECT_EQ(source, Date(1, 1, 2000));
    EXPECT_EQ(moved, Date(15, 3, 2026));
}

TEST(DateTest, MoveAssignmentTransfersValueAndReturnsReference)
{
    Date source(15, 3, 2026);
    Date target(1, 1, 2000);

    EXPECT_EQ(&(target = std::move(source)), &target);
    EXPECT_EQ(target, Date(15, 3, 2026));
    source = Date(2, 2, 2002);
    EXPECT_EQ(source, Date(2, 2, 2002));
    EXPECT_EQ(target, Date(15, 3, 2026));
}

TEST(DateTest, SelfAssignmentsPreserveValue)
{
    Date date(15, 3, 2026);
    const Date& copy_alias = date;
    EXPECT_EQ(&(date = copy_alias), &date);
    EXPECT_EQ(date, Date(15, 3, 2026));

    Date& move_alias = date;
    EXPECT_EQ(&(date = std::move(move_alias)), &date);
    EXPECT_EQ(date, Date(15, 3, 2026));
}

TEST(DateTest, StreamOutputSupportsChaining)
{
    const Date first(1, 2, 2026);
    const Date second(31, 12, 2025);
    std::ostringstream output;

    EXPECT_EQ(&(output << first), &output);
    output << " | " << second;
    EXPECT_EQ(output.str(), "1.2.2026 | 31.12.2025");
}

TEST(DateTest, SupportsDefaultConstructedArrays)
{
    Date* dates = new Date[4];
    for (std::size_t i = 0; i < 4; ++i)
    {
        EXPECT_TRUE(dates[i].is_valid());
    }
    dates[0] = Date(29, 2, 2024);
    EXPECT_EQ(dates[0], Date(29, 2, 2024));
    delete[] dates;
}
