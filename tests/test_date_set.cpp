#include "date.h"
#include "date_set.h"

#include <gtest/gtest.h>

#include <cstddef>
#include <sstream>
#include <utility>

static DateSet MakeJanuarySet(const int* days, std::size_t count)
{
    DateSet result;
    for (std::size_t i = 0; i < count; ++i)
    {
        result.add(Date(days[i], 1, 2026));
    }
    return result;
}

static Date StressDate(int index)
{
    return Date(index % 28 + 1, (index / 28) % 12 + 1, 2000 + index / 336);
}

TEST(DateSetTest, EmptySetHasNoElements)
{
    const DateSet set;
    EXPECT_TRUE(set.empty());
    EXPECT_EQ(set.size(), 0u);
    EXPECT_FALSE(set.contains(Date(1, 1, 2026)));
    EXPECT_FALSE(set(Date(1, 1, 2026)));
}

TEST(DateSetTest, AddReportsChangesAndRejectsDuplicates)
{
    DateSet set;
    EXPECT_TRUE(set.add(Date(1, 1, 2026)));
    EXPECT_EQ(set.size(), 1u);
    EXPECT_FALSE(set.empty());
    EXPECT_FALSE(set.add(Date(1, 1, 2026)));
    EXPECT_EQ(set.size(), 1u);
    EXPECT_TRUE(set.contains(Date(1, 1, 2026)));
}

TEST(DateSetTest, RejectsInvalidDatesWithoutChangingContents)
{
    DateSet set;
    set.add(Date(1, 1, 2026));
    const DateSet before(set);
    const Date invalid[] = {
        Date(31, 4, 2026), Date(29, 2, 1900), Date(1, 0, 2026), Date(1, 1, 0)
    };

    for (const Date& date : invalid)
    {
        EXPECT_FALSE(set.add(date)) << date;
        EXPECT_EQ(set, before) << date;
    }
}

TEST(DateSetTest, InsertionsPreserveChronologicalOrder)
{
    DateSet set;
    ASSERT_TRUE(set.add(Date(1, 2, 2026)));
    ASSERT_TRUE(set.add(Date(31, 12, 2025)));
    ASSERT_TRUE(set.add(Date(31, 1, 2026)));
    ASSERT_TRUE(set.add(Date(1, 1, 2026)));
    std::ostringstream output;
    output << set;

    EXPECT_EQ(set.size(), 4u);
    EXPECT_EQ(output.str(), "{ 31.12.2025, 1.1.2026, 31.1.2026, 1.2.2026 }");
}

TEST(DateSetTest, ContainsAndCallOperatorAgreeAtSearchBoundaries)
{
    const int days[] = {5, 1, 3};
    const DateSet set = MakeJanuarySet(days, 3);

    for (int day = 1; day <= 6; ++day)
    {
        SCOPED_TRACE(day);
        const Date date(day, 1, 2026);
        const bool expected = day == 1 || day == 3 || day == 5;
        EXPECT_EQ(set.contains(date), expected);
        EXPECT_EQ(set(date), expected);
    }
    EXPECT_FALSE(set.contains(Date(31, 12, 2025)));
    EXPECT_FALSE(set(Date(31, 12, 2025)));
    EXPECT_FALSE(set.contains(Date(1, 1, 2027)));
    EXPECT_FALSE(set(Date(1, 1, 2027)));
}

TEST(DateSetTest, RemoveFromEmptyOrMissingElementReturnsZero)
{
    DateSet set;
    EXPECT_FALSE(set.remove(Date(1, 1, 2026)));
    set.add(Date(3, 1, 2026));
    EXPECT_FALSE(set.remove(Date(1, 1, 2026)));
    EXPECT_FALSE(set.remove(Date(5, 1, 2026)));
    EXPECT_EQ(set.size(), 1u);
    EXPECT_TRUE(set.contains(Date(3, 1, 2026)));
}

TEST(DateSetTest, RemovesFirstMiddleAndLastWhilePreservingOrder)
{
    const int days[] = {9, 5, 1, 7, 3};
    DateSet set = MakeJanuarySet(days, 5);
    EXPECT_TRUE(set.remove(Date(5, 1, 2026)));
    EXPECT_TRUE(set.remove(Date(1, 1, 2026)));
    EXPECT_TRUE(set.remove(Date(9, 1, 2026)));
    EXPECT_FALSE(set.remove(Date(5, 1, 2026)));
    EXPECT_EQ(set.size(), 2u);
    EXPECT_TRUE(set.contains(Date(3, 1, 2026)));
    EXPECT_TRUE(set.contains(Date(7, 1, 2026)));
    std::ostringstream output;
    output << set;
    EXPECT_EQ(output.str(), "{ 3.1.2026, 7.1.2026 }");
}

TEST(DateSetTest, RemovingOnlyElementAllowsReuse)
{
    DateSet set;
    set.add(Date(1, 1, 2026));
    EXPECT_TRUE(set.remove(Date(1, 1, 2026)));
    EXPECT_TRUE(set.empty());
    EXPECT_EQ(set.size(), 0u);
    EXPECT_FALSE(set.contains(Date(1, 1, 2026)));
    EXPECT_TRUE(set.add(Date(2, 1, 2026)));
    EXPECT_TRUE(set.contains(Date(2, 1, 2026)));
    EXPECT_EQ(set.size(), 1u);
}

TEST(DateSetTest, CopyConstructorDeepCopiesNonemptySet)
{
    const int days[] = {1, 3, 5};
    const DateSet source = MakeJanuarySet(days, 3);
    DateSet copy(source);
    ASSERT_EQ(copy, source);
    copy.remove(Date(1, 1, 2026));
    copy.add(Date(7, 1, 2026));

    EXPECT_TRUE(source.contains(Date(1, 1, 2026)));
    EXPECT_FALSE(source.contains(Date(7, 1, 2026)));
    EXPECT_NE(copy, source);
}

TEST(DateSetTest, CopyAssignmentReplacesContentsWithIndependentCopy)
{
    const int days[] = {1, 3, 5};
    const DateSet source = MakeJanuarySet(days, 3);
    DateSet target;
    target.add(Date(20, 1, 2026));
    EXPECT_EQ(&(target = source), &target);
    ASSERT_EQ(target, source);
    EXPECT_FALSE(target.contains(Date(20, 1, 2026)));
    target.remove(Date(1, 1, 2026));
    target.add(Date(7, 1, 2026));

    EXPECT_TRUE(source.contains(Date(1, 1, 2026)));
    EXPECT_FALSE(source.contains(Date(7, 1, 2026)));
    EXPECT_NE(target, source);
}

TEST(DateSetTest, CopyAssignmentHandlesEmptySetsInBothDirections)
{
    const DateSet empty;
    DateSet target;
    target.add(Date(1, 1, 2026));
    target = empty;
    EXPECT_TRUE(target.empty());
    EXPECT_EQ(target.size(), 0u);
    EXPECT_TRUE(target.add(Date(2, 1, 2026)));

    DateSet copy(empty);
    EXPECT_TRUE(copy.empty());
    copy = target;
    EXPECT_EQ(copy, target);
    copy.remove(Date(2, 1, 2026));
    EXPECT_TRUE(target.contains(Date(2, 1, 2026)));
}

TEST(DateSetTest, SelfCopyAssignmentPreservesContents)
{
    const int days[] = {1, 3, 5};
    DateSet set = MakeJanuarySet(days, 3);
    const DateSet before(set);
    const DateSet& alias = set;
    EXPECT_EQ(&(set = alias), &set);
    EXPECT_EQ(set, before);
}

TEST(DateSetTest, MoveConstructorEmptiesSourceAndAllowsReuse)
{
    const int days[] = {1, 3, 5};
    DateSet source = MakeJanuarySet(days, 3);
    const DateSet expected(source);
    DateSet moved(std::move(source));

    EXPECT_EQ(moved, expected);
    EXPECT_TRUE(source.empty());
    EXPECT_EQ(source.size(), 0u);
    EXPECT_FALSE(source.contains(Date(1, 1, 2026)));
    DateSet empty_copy(source);
    EXPECT_TRUE(empty_copy.empty());
    EXPECT_TRUE(source.add(Date(7, 1, 2026)));
    EXPECT_FALSE(moved.contains(Date(7, 1, 2026)));
}

TEST(DateSetTest, MoveAssignmentReplacesDestinationAndEmptiesSource)
{
    const int days[] = {1, 3, 5};
    DateSet source = MakeJanuarySet(days, 3);
    const DateSet expected(source);
    DateSet target;
    target.add(Date(20, 1, 2026));

    EXPECT_EQ(&(target = std::move(source)), &target);
    EXPECT_EQ(target, expected);
    EXPECT_FALSE(target.contains(Date(20, 1, 2026)));
    EXPECT_TRUE(source.empty());
    EXPECT_EQ(source.size(), 0u);
    EXPECT_FALSE(source(Date(1, 1, 2026)));
    EXPECT_TRUE(source.add(Date(7, 1, 2026)));
    EXPECT_FALSE(target.contains(Date(7, 1, 2026)));
}

TEST(DateSetTest, SelfMoveAssignmentPreservesContents)
{
    const int days[] = {1, 3, 5};
    DateSet set = MakeJanuarySet(days, 3);
    const DateSet before(set);
    DateSet& alias = set;
    EXPECT_EQ(&(set = std::move(alias)), &set);
    EXPECT_EQ(set, before);
}

TEST(DateSetTest, CompoundOperatorsReturnReferenceAndSupportChaining)
{
    DateSet set;
    EXPECT_EQ(&((set += Date(1, 1, 2026)) += Date(3, 1, 2026)), &set);
    EXPECT_EQ(set.size(), 2u);
    set += Date(1, 1, 2026);
    set += Date(31, 4, 2026);
    EXPECT_EQ(set.size(), 2u);
    EXPECT_EQ(&((set -= Date(1, 1, 2026)) -= Date(3, 1, 2026)), &set);
    EXPECT_TRUE(set.empty());
    EXPECT_EQ(&(set -= Date(1, 1, 2026)), &set);
    EXPECT_TRUE(set.empty());
}

TEST(DateSetTest, EqualityIgnoresInsertionOrderAndUnusedCapacity)
{
    const int first_days[] = {1, 3};
    const int second_days[] = {5, 4, 3, 2, 1};
    const DateSet first = MakeJanuarySet(first_days, 2);
    DateSet second = MakeJanuarySet(second_days, 5);
    second.remove(Date(2, 1, 2026));
    second.remove(Date(4, 1, 2026));
    second.remove(Date(5, 1, 2026));

    EXPECT_EQ(first, second);
    EXPECT_EQ(second, first);
    EXPECT_FALSE(first != second);
    EXPECT_EQ(first, first);
    EXPECT_FALSE(first != first);
    second.remove(Date(1, 1, 2026));
    second.remove(Date(3, 1, 2026));
    EXPECT_EQ(second, DateSet());
}

TEST(DateSetTest, EqualityDetectsDifferentSizesAndDates)
{
    const int first_days[] = {1, 3};
    const int other_days[] = {1, 4};
    const DateSet first = MakeJanuarySet(first_days, 2);
    const DateSet other = MakeJanuarySet(other_days, 2);

    EXPECT_NE(first, DateSet());
    EXPECT_NE(DateSet(), first);
    EXPECT_NE(first, other);
    EXPECT_FALSE(first == other);
    EXPECT_NE(other, first);
}

TEST(DateSetTest, StreamOutputHandlesEmptyAndSortedContents)
{
    const DateSet empty;
    const int days[] = {5, 1, 3};
    const DateSet set = MakeJanuarySet(days, 3);
    const DateSet before(set);
    std::ostringstream output;

    EXPECT_EQ(&(output << empty), &output);
    output << " | " << set;
    EXPECT_EQ(output.str(), "{ } | { 1.1.2026, 3.1.2026, 5.1.2026 }");
    EXPECT_EQ(set, before);
}

TEST(DateSetTest, UnionCombinesSortedUniqueElements)
{
    const int first_days[] = {1, 3, 5};
    const int second_days[] = {3, 4, 6};
    const int expected_days[] = {1, 3, 4, 5, 6};
    const DateSet first = MakeJanuarySet(first_days, 3);
    const DateSet second = MakeJanuarySet(second_days, 3);
    const DateSet expected = MakeJanuarySet(expected_days, 5);

    EXPECT_EQ(first | second, expected);
    EXPECT_EQ(second | first, expected);
}

TEST(DateSetTest, IntersectionKeepsOnlyCommonElements)
{
    const int first_days[] = {1, 3, 5};
    const int second_days[] = {3, 4, 6};
    const int expected_days[] = {3};
    const DateSet first = MakeJanuarySet(first_days, 3);
    const DateSet second = MakeJanuarySet(second_days, 3);
    const DateSet expected = MakeJanuarySet(expected_days, 1);

    EXPECT_EQ(first & second, expected);
    EXPECT_EQ(second & first, expected);
}

TEST(DateSetTest, DifferenceKeepsOnlyLeftElements)
{
    const int first_days[] = {1, 3, 5};
    const int second_days[] = {3, 4, 6};
    const int expected_days[] = {1, 5};
    const int reverse_days[] = {4, 6};
    const DateSet first = MakeJanuarySet(first_days, 3);
    const DateSet second = MakeJanuarySet(second_days, 3);

    EXPECT_EQ(first - second, MakeJanuarySet(expected_days, 2));
    EXPECT_EQ(second - first, MakeJanuarySet(reverse_days, 2));
}

TEST(DateSetTest, SetOperationsHandleEmptyOperands)
{
    const int days[] = {1, 3, 5};
    const DateSet set = MakeJanuarySet(days, 3);
    const DateSet empty;

    EXPECT_EQ(set | empty, set);
    EXPECT_EQ(empty | set, set);
    EXPECT_TRUE((set & empty).empty());
    EXPECT_TRUE((empty & set).empty());
    EXPECT_EQ(set - empty, set);
    EXPECT_TRUE((empty - set).empty());
    EXPECT_TRUE((empty | empty).empty());
    EXPECT_TRUE((empty & empty).empty());
    EXPECT_TRUE((empty - empty).empty());
}

TEST(DateSetTest, SetOperationsHandleSameObject)
{
    const int days[] = {1, 3, 5};
    const DateSet set = MakeJanuarySet(days, 3);
    EXPECT_EQ(set | set, set);
    EXPECT_EQ(set & set, set);
    EXPECT_TRUE((set - set).empty());
}

TEST(DateSetTest, SetOperationsHandleDisjointInputs)
{
    const int first_days[] = {1, 3};
    const int second_days[] = {2, 4};
    const int all_days[] = {1, 2, 3, 4};
    const DateSet first = MakeJanuarySet(first_days, 2);
    const DateSet second = MakeJanuarySet(second_days, 2);

    EXPECT_EQ(first | second, MakeJanuarySet(all_days, 4));
    EXPECT_TRUE((first & second).empty());
    EXPECT_EQ(first - second, first);
    EXPECT_EQ(second - first, second);
}

TEST(DateSetTest, SetOperationsHandleSubsetInputs)
{
    const int all_days[] = {1, 2, 3, 4};
    const int subset_days[] = {2, 3};
    const int remaining_days[] = {1, 4};
    const DateSet all = MakeJanuarySet(all_days, 4);
    const DateSet subset = MakeJanuarySet(subset_days, 2);

    EXPECT_EQ(all | subset, all);
    EXPECT_EQ(subset | all, all);
    EXPECT_EQ(all & subset, subset);
    EXPECT_EQ(subset & all, subset);
    EXPECT_EQ(all - subset, MakeJanuarySet(remaining_days, 2));
    EXPECT_TRUE((subset - all).empty());
}

TEST(DateSetTest, SetOperationResultsAreIndependentOfOperands)
{
    const int first_days[] = {1, 3, 5};
    const int second_days[] = {3, 4, 6};
    const DateSet first = MakeJanuarySet(first_days, 3);
    const DateSet second = MakeJanuarySet(second_days, 3);
    const DateSet first_before(first);
    const DateSet second_before(second);
    DateSet united = first | second;
    DateSet common = first & second;
    DateSet difference = first - second;

    united.remove(Date(1, 1, 2026));
    united.add(Date(7, 1, 2026));
    common.remove(Date(3, 1, 2026));
    common.add(Date(8, 1, 2026));
    difference.remove(Date(1, 1, 2026));
    difference.add(Date(9, 1, 2026));

    EXPECT_EQ(first, first_before);
    EXPECT_EQ(second, second_before);
}

TEST(DateSetTest, SetOperationsAcceptMovedFromOperands)
{
    const int days[] = {1, 3, 5};
    DateSet source = MakeJanuarySet(days, 3);
    const DateSet moved(std::move(source));

    EXPECT_EQ(source | moved, moved);
    EXPECT_EQ(moved | source, moved);
    EXPECT_TRUE((source & moved).empty());
    EXPECT_TRUE((moved & source).empty());
    EXPECT_TRUE((source - moved).empty());
    EXPECT_EQ(moved - source, moved);
}

TEST(DateSetTest, Stress1000InsertionsDuplicatesAndRemovals)
{
    DateSet set;
    for (int i = 999; i >= 0; --i)
    {
        SCOPED_TRACE(i);
        ASSERT_TRUE(set.add(StressDate(i)));
    }
    ASSERT_EQ(set.size(), 1000u);

    for (int i = 0; i < 1000; ++i)
    {
        SCOPED_TRACE(i);
        EXPECT_FALSE(set.add(StressDate(i)));
        EXPECT_TRUE(set.contains(StressDate(i)));
        EXPECT_TRUE(set(StressDate(i)));
    }
    EXPECT_EQ(set.size(), 1000u);

    for (int i = 0; i < 1000; i += 2)
    {
        SCOPED_TRACE(i);
        EXPECT_TRUE(set.remove(StressDate(i)));
        EXPECT_FALSE(set.remove(StressDate(i)));
    }
    ASSERT_EQ(set.size(), 500u);

    DateSet expected;
    for (int i = 0; i < 1000; ++i)
    {
        SCOPED_TRACE(i);
        EXPECT_EQ(set.contains(StressDate(i)), i % 2 != 0);
        if (i % 2 != 0)
        {
            expected.add(StressDate(i));
        }
    }
    EXPECT_EQ(set, expected);
}

TEST(DateSetTest, SupportsArraysWithIndependentOwnedStorage)
{
    DateSet* sets = new DateSet[2];
    sets[0].add(Date(1, 1, 2026));
    sets[1] = sets[0];
    sets[0].remove(Date(1, 1, 2026));

    EXPECT_TRUE(sets[0].empty());
    EXPECT_TRUE(sets[1].contains(Date(1, 1, 2026)));
    EXPECT_EQ(sets[1].size(), 1u);
    delete[] sets;
}
