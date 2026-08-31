#include <QTest>
#include <QDate>
#include <QDebug>

/*
 * Test the wallpaper number calculation logic.
 * This mirrors the calculateWallpaperNumber function from natureprovider.cpp
 */

int calculateWallpaperNumber(const QDate &date, int count)
{
    if (count <= 0) {
        return -1;
    }

    const QDate epoch(2026, 1, 1);
    const qint64 days = epoch.daysTo(date);

    // Normalize negative modulo results for dates before epoch
    const int number = static_cast<int>(((days % count) + count) % count) + 1;

    return number;
}

class TestWallpaperCalculation : public QObject
{
    Q_OBJECT

private slots:
    void testFirstDayOfEpoch();
    void testSecondDay();
    void testNormalSequentialDates();
    void testWraparound();
    void testDateBeforeEpoch();
    void testLeapDay();
    void testZeroCount();
    void testNegativeCount();
    void testLargeDateOffset();
    void testYearBoundaries();
};

void TestWallpaperCalculation::testFirstDayOfEpoch()
{
    // 2026-01-01 should be wallpaper 001
    QDate date(2026, 1, 1);
    QCOMPARE(calculateWallpaperNumber(date, 365), 1);
}

void TestWallpaperCalculation::testSecondDay()
{
    // 2026-01-02 should be wallpaper 002
    QDate date(2026, 1, 2);
    QCOMPARE(calculateWallpaperNumber(date, 365), 2);
}

void TestWallpaperCalculation::testNormalSequentialDates()
{
    // Test a few sequential dates
    QDate date1(2026, 1, 10);
    QCOMPARE(calculateWallpaperNumber(date1, 365), 10);

    QDate date2(2026, 2, 1);
    QCOMPARE(calculateWallpaperNumber(date2, 365), 32);

    QDate date3(2026, 12, 31);
    QCOMPARE(calculateWallpaperNumber(date3, 365), 365);
}

void TestWallpaperCalculation::testWraparound()
{
    // With 365 wallpapers, day 366 should wrap to 1
    QDate date(2027, 1, 1);
    QCOMPARE(calculateWallpaperNumber(date, 365), 1);

    // Day 367 should be 2
    QDate date2(2027, 1, 2);
    QCOMPARE(calculateWallpaperNumber(date2, 365), 2);

    // Test with smaller count (e.g., 10 wallpapers)
    QDate date3(2026, 1, 1);
    QCOMPARE(calculateWallpaperNumber(date3, 10), 1);

    QDate date4(2026, 1, 11);
    QCOMPARE(calculateWallpaperNumber(date4, 10), 1);  // wraps around

    QDate date5(2026, 1, 12);
    QCOMPARE(calculateWallpaperNumber(date5, 10), 2);
}

void TestWallpaperCalculation::testDateBeforeEpoch()
{
    // Date before epoch should wrap correctly
    // 2025-12-31 is 1 day before epoch, should be wallpaper 365 (with 365 count)
    QDate date(2025, 12, 31);
    QCOMPARE(calculateWallpaperNumber(date, 365), 365);

    // 2025-12-30 is 2 days before epoch, should be wallpaper 364
    QDate date2(2025, 12, 30);
    QCOMPARE(calculateWallpaperNumber(date2, 365), 364);

    // 2025-01-01 (365 days before epoch)
    QDate date3(2025, 1, 1);
    QCOMPARE(calculateWallpaperNumber(date3, 365), 1);

    // Far before epoch
    QDate date4(2000, 1, 1);
    int result = calculateWallpaperNumber(date4, 365);
    QVERIFY(result >= 1 && result <= 365);
}

void TestWallpaperCalculation::testLeapDay()
{
    // 2028 is a leap year, test Feb 29
    QDate leapDay(2028, 2, 29);
    int result = calculateWallpaperNumber(leapDay, 365);
    QVERIFY(result >= 1 && result <= 365);

    // 2024 is also a leap year (before epoch)
    QDate leapDayBefore(2024, 2, 29);
    int result2 = calculateWallpaperNumber(leapDayBefore, 365);
    QVERIFY(result2 >= 1 && result2 <= 365);
}

void TestWallpaperCalculation::testZeroCount()
{
    QDate date(2026, 1, 1);
    QCOMPARE(calculateWallpaperNumber(date, 0), -1);
}

void TestWallpaperCalculation::testNegativeCount()
{
    QDate date(2026, 1, 1);
    QCOMPARE(calculateWallpaperNumber(date, -1), -1);
    QCOMPARE(calculateWallpaperNumber(date, -100), -1);
}

void TestWallpaperCalculation::testLargeDateOffset()
{
    // Test far future date
    QDate date(2100, 1, 1);
    int result = calculateWallpaperNumber(date, 365);
    QVERIFY(result >= 1 && result <= 365);

    // Test far past date
    QDate date2(1900, 1, 1);
    int result2 = calculateWallpaperNumber(date2, 365);
    QVERIFY(result2 >= 1 && result2 <= 365);
}

void TestWallpaperCalculation::testYearBoundaries()
{
    // Test year boundaries
    QDate date1(2026, 12, 31);
    QCOMPARE(calculateWallpaperNumber(date1, 365), 365);

    QDate date2(2027, 1, 1);
    QCOMPARE(calculateWallpaperNumber(date2, 365), 1);

    QDate date3(2027, 12, 31);
    QCOMPARE(calculateWallpaperNumber(date3, 365), 365);

    QDate date4(2028, 1, 1);
    QCOMPARE(calculateWallpaperNumber(date4, 365), 1);
}

QTEST_MAIN(TestWallpaperCalculation)

#include "test_wallpaper_calculation.moc"