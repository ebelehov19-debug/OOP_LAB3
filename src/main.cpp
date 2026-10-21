#include "date.h"
#include "date_set.h"

#include <iostream>
#include <utility>

int main()
{
    std::cout << std::noboolalpha;

    const Date default_date;
    const Date date(15, 3, 2026);
    const Date leap_day(29, 2, 2024);
    const Date invalid_date(29, 2, 2025);

    std::cout << "Date examples\n";
    std::cout << "Default date: " << default_date
              << ", valid: " << default_date.is_valid() << '\n';
    std::cout << leap_day << ", valid: " << leap_day.is_valid() << '\n';
    std::cout << invalid_date << ", valid: " << invalid_date.is_valid() << '\n';
    std::cout << date << " == " << Date(15, 3, 2026) << ": "
              << (date == Date(15, 3, 2026)) << '\n';
    std::cout << leap_day << " < " << date << ": " << (leap_day < date) << '\n';

    Date copied_date(date);
    Date assigned_date;
    assigned_date = date;
    Date moved_date(std::move(copied_date));
    Date move_assigned_date;
    move_assigned_date = std::move(assigned_date);
    std::cout << "Copied and moved dates: " << moved_date
              << ", " << move_assigned_date << '\n';

    std::cout << "\nAdding and removing dates\n";
    DateSet first;
    std::cout << "Empty set: " << first << ", size: " << first.size()
              << ", empty: " << first.empty() << '\n';
    std::cout << "Add " << date << ": " << first.add(date) << '\n';
    first.add(Date(1, 1, 2026));
    first.add(leap_day);
    std::cout << "Sorted set: " << first << '\n';
    std::cout << "Add duplicate: " << first.add(date) << '\n';
    std::cout << "Add invalid date: " << first.add(invalid_date) << '\n';
    std::cout << "contains(" << date << "): " << first.contains(date) << '\n';
    std::cout << "operator()(" << date << "): " << first(date) << '\n';
    std::cout << "Contains missing date: " << first.contains(Date(1, 1, 2030)) << '\n';

    first += Date(1, 4, 2026);
    first -= Date(1, 1, 2026);
    std::cout << "After += 1.4.2026 and -= 1.1.2026: " << first << '\n';
    std::cout << "Remove " << leap_day << ": " << first.remove(leap_day) << '\n';
    std::cout << "Remove missing date: " << first.remove(Date(31, 12, 2025)) << '\n';
    std::cout << "A = " << first << ", size: " << first.size() << '\n';

    std::cout << "\nSet operations\n";
    DateSet second;
    second += Date(31, 12, 2026);
    second += Date(1, 4, 2026);
    std::cout << "B = " << second << '\n';
    std::cout << "A | B = " << (first | second) << '\n';
    std::cout << "A & B = " << (first & second) << '\n';
    std::cout << "A - B = " << (first - second) << '\n';
    std::cout << "B - A = " << (second - first) << '\n';

    std::cout << "\nCopying and moving sets\n";
    DateSet copy(first);
    std::cout << "Copy == A: " << (copy == first) << '\n';
    copy -= date;
    std::cout << "Modified copy: " << copy << '\n';
    std::cout << "Original A: " << first << '\n';
    std::cout << "Copy != A: " << (copy != first) << '\n';

    DateSet assigned;
    assigned += Date(1, 1, 2000);
    assigned = first;
    std::cout << "Copy assignment: " << assigned << '\n';

    DateSet moved(std::move(copy));
    std::cout << "Move constructor: " << moved
              << ", source empty: " << copy.empty() << '\n';
    copy += Date(1, 1, 2030);
    std::cout << "Reused source: " << copy << '\n';

    DateSet move_assigned;
    move_assigned += Date(1, 1, 2000);
    move_assigned = std::move(assigned);
    std::cout << "Move assignment: " << move_assigned
              << ", source empty: " << assigned.empty() << '\n';

    return 0;
}
