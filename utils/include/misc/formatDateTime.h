//  MIT License
//
//  Copyright (c) 2018 Francisco de Lanuza
//
//  Permission is hereby granted, free of charge, to any person obtaining a copy
//  of this software and associated documentation files (the "Software"), to deal
//  in the Software without restriction, including without limitation the rights
//  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//  copies of the Software, and to permit persons to whom the Software is
//  furnished to do so, subject to the following conditions:
//
//  The above copyright notice and this permission notice shall be included in all
//  copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
//  SOFTWARE.

#ifndef FORMAT_DATE_TIME_H
#define FORMAT_DATE_TIME_H

#include <string>
#include <chrono>
#include <time.h>
#include <exception>
#include <array>


using namespace std;

namespace utils
{
  inline tm timePoint2Tm(const chrono::time_point<chrono::system_clock, std::chrono::seconds>& date_time, tm* (*fmt_conv)(const time_t *time) = localtime) {
    time_t a_time_ = chrono::system_clock::to_time_t(date_time);
    tm *tm_ptr = fmt_conv(&a_time_);
    if (!tm_ptr)
      throw std::out_of_range("Date is too big. Use formatLDatetime");

    return *tm_ptr;
  }


  /**
    *  \brief  Formats a 'system_clock::time_point' according to a format string (must be supported by 'strftime') and a conversion function (gmtime or localtime)
    *  @param   date_time [in] The system_clock::time_point
    *  @param   format    [in] The format of the date time to be returned as a string
    *  @param   fmt_conv  [in] pointer to the function used for converting to the specified format. Possible candidates: gmtime, localtime (DEFAULT)
    *  @return  string    Formatted date-time string
    *  @throw   out_of_range  (At least in MSVisual Studio 2017) If date_time is bigger than year 3000 (not supported by localtime() or gmtime())
    */
  inline string formatDateTime(const chrono::time_point<chrono::system_clock, chrono::seconds>& date_time, const string& format, tm* (*fmt_conv)(const time_t *time) = localtime ) {
    char time_str[120];
    strftime(time_str, sizeof time_str, format.data(), &timePoint2Tm(date_time, fmt_conv));

    return string(time_str);
  }

  /**
   *  \brief  Formats a 'system_clock::time_point' according to a format string (must be supported by 'strftime') and a conversion function (gmtime or localtime)
   *  @param   date_time [in] The system_clock::time_point
   *  @param   date_format    [in] The date format of the date time to be returned as a string
   *  @param   time_format    [in] The time format of the date time to be returned as a string
   *  @param   fmt_conv  [in] pointer to the function used for converting to the specified format. Possible candidates: gmtime, localtime (DEFAULT)
   *  @return  pair<string, string>    Formatted date-time strings
   *  @throw   out_of_range  (At least in MSVisual Studio 2017) If date_time is bigger than year 3000 (not supported by localtime() or gmtime())
   */
  inline pair<string, string> formatDateTime(const chrono::time_point<chrono::system_clock, chrono::seconds>& date_time, const string& date_format, const string& time_format, tm* (*fmt_conv)(const time_t *time) = localtime) {
    char date_str[80];
    char time_str[40];
    tm a_tm = timePoint2Tm(date_time, fmt_conv);
    strftime(date_str, sizeof date_str, date_format.data(), &a_tm);
    strftime(time_str, sizeof time_str, time_format.data(), &a_tm);

    return pair<string, string>(date_str, time_str);
  }

  /**
  *  \brief  Converts a 'system_clock::time_point' to a tm structure. Updates the provided format string so it can be used to format any year (must be supported by 'strftime'. For year formatting, only %y and %Y are supported).
  *          Supports any date by not using the year in the tm type (limited to int) nor the year formatting in the returned format string (which is limited to 4-digit years)
  *  @param   date_time     [in] The system_clock::time_point
  *  @param   format        [in / out] The format of the date time to be returned as a string
  *  @param   is_localtime  [in] If the time must be converted to localtime. Defualt = false (time is converted as UTC)
  *  @return  tm            tm structure for the given time_point
  */
  inline tm anyDateTime2Tm(const chrono::time_point<chrono::system_clock, chrono::seconds>& date_time, string& format, bool is_localtime = false) {
    static const time_t SECONDS_DAY = 86400;
    static const time_t SECONDS_YEAR = 31536000;
    static const time_t SECONDS_2_YEARS = 2 * SECONDS_YEAR;
    static const time_t SECONDS_LEAP_YEAR = 31622400;
    static const time_t SECONDS_AVG_YEAR = 31557600;
    static const time_t SECONDS_4_YEARS = 4 * SECONDS_AVG_YEAR;
    static const array<int, 13> DAYS_MONTH{ 0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365 };
    static const array<int, 13> DAYS_MONTH_LEAP{ 0, 31, 60, 91, 121, 152, 182, 213, 244, 274, 305, 335, 366 };

    time_t a_time_ = date_time.time_since_epoch().count();
    if (is_localtime) {
      _tzset();
      a_time_ -= _timezone;
      a_time_ += 3600 * _daylight;
    }

    // Calculate tm "manually". time_t(0) = Thursday 1, Jan 1970 00:00:00 ; tm(0) = 1 Jan 1900 00:00:00
    // 1 day = 86400, 1 year = 365.25 days. Leap years ==> year % 4 = 0. Leap seconds not accounted
    tm date_time_tm;
    time_t year;
    const array<int, 13>* DAYS_MONTH_PTR;

    // Number of elapsed leap years (adding year 1969, then it is just the number of seconds divided by the total number of seconds in 4 years)
    time_t a_time_plus_1969 = a_time_ + SECONDS_YEAR;
    time_t leap_years = a_time_plus_1969 / SECONDS_4_YEARS;
    time_t total_seconds_leap_cycles = leap_years * SECONDS_4_YEARS;

    // Check whether the date belongs to a leap year (adding year 1969 and substructing 4 years for each elapsed leap year, then dividing by 365 days. If the result is 3 or bigger, then it is a leap year)
    bool is_leap_year = ((a_time_plus_1969 - total_seconds_leap_cycles) / SECONDS_YEAR) >= 3;
    if (is_leap_year) {
      year = 2 + 4 * leap_years;
      date_time_tm.tm_yday = ((a_time_ - SECONDS_2_YEARS - total_seconds_leap_cycles) % SECONDS_LEAP_YEAR) / SECONDS_DAY;
      DAYS_MONTH_PTR = &DAYS_MONTH_LEAP;
    }
    else {
      year = 4 * leap_years - 1 + (a_time_plus_1969 - total_seconds_leap_cycles) / SECONDS_YEAR;
      date_time_tm.tm_yday = ((a_time_plus_1969 - total_seconds_leap_cycles) % SECONDS_YEAR) / SECONDS_DAY;
      DAYS_MONTH_PTR = &DAYS_MONTH;
    }
    year += 1970;

    auto tent_mon = date_time_tm.tm_yday / 29;
    if (tent_mon == 0)
      date_time_tm.tm_mon = tent_mon;
    else if (date_time_tm.tm_yday < (*DAYS_MONTH_PTR)[tent_mon])
      date_time_tm.tm_mon = tent_mon - 1;
    else
      date_time_tm.tm_mon = tent_mon;

    date_time_tm.tm_mday = 1 + date_time_tm.tm_yday - (*DAYS_MONTH_PTR)[date_time_tm.tm_mon];

    date_time_tm.tm_wday = 1 + (a_time_ / SECONDS_DAY + 3) % 7; // Mon = 1, Tue = 2, etc
    date_time_tm.tm_hour = (a_time_ / 3600) % 24;
    date_time_tm.tm_min = (a_time_ / 60) % 60;
    date_time_tm.tm_sec = a_time_ % 60;

    size_t pos = format.find("%Y");
    if (pos != string::npos)
      format.replace(pos, 2, to_string(year));
    else {
      pos = format.find("%y");
      if (pos != string::npos)
        format.replace(pos, 2, to_string((year) % 100));
    }

    return date_time_tm;
  }


  /**
  *  \brief  Formats a 'system_clock::time_point' according to a format string (must be supported by 'strftime'. For year formatting, only %y and %Y are supported).
  *          Supports any date by not using the year in the tm type (limited to int) nor the year formatting in strftime() (limited to 4 digits )
  *  @param   date_time     [in] The system_clock::time_point
  *  @param   format        [in] The format of the date time to be returned as a string
  *  @param   is_localtime  [in] If the time must be converted to localtime. Defualt = false (time is converted as UTC)
  *  @return  string        Formatted date-time string
  */
  inline string formatAnyDateTime(const chrono::time_point<chrono::system_clock, chrono::seconds>& date_time, const string& format, bool is_localtime = false) {
    char date_time_str[120];
    
    string new_format = format;
    tm date_time_tm = anyDateTime2Tm(date_time, new_format, is_localtime);

    strftime(date_time_str, sizeof date_time_str, new_format.data(), &date_time_tm);

    return string(date_time_str);
  }


  /**
  *  \brief  Formats a 'system_clock::time_point' according to a couple of format strings (must be supported by 'strftime'. For year formatting, only %y and %Y are supported).
  *          Supports any date by not using the year in the tm type nor the year formatting in strftime()
  *  @param   date_time     [in] A time_point
  *  @param   format_date   [in] The date format (not validated) of the date time to be returned as a first string 
  *  @param   format_time   [in] The time format (not validated) of the date time to be returned as a second string
  *  @param   is_localtime  [in] If the time must be converted to localtime. Defualt = false (time is converted as UTC)
  *  @return  pair<string, string>  Formatted date-time strings according to the given formats (e.g. a date and a time format string)
  */
  inline pair<string, string> formatAnyDateTime(const chrono::time_point<chrono::system_clock, chrono::seconds>& date_time, const string& date_format, const string& time_format, bool is_localtime = false) {
    char date_str[80];
    char time_str[40];

    string new_date_format = date_format;
    tm date_time_tm = anyDateTime2Tm(date_time, new_date_format, is_localtime);

    strftime(date_str, sizeof date_str, new_date_format.data(), &date_time_tm);
    strftime(time_str, sizeof time_str, time_format.data(), &date_time_tm);

    return pair<string, string>(date_str, time_str);
  }
  

}

#endif // FORMAT_DATE_TIME_H
