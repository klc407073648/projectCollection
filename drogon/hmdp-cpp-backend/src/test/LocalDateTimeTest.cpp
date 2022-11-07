#include "Poco/LocalDateTime.h"
#include "Poco/DateTime.h"
#include "Poco/DateTimeFormat.h"
#include "Poco/DateTimeFormatter.h"
#include "Poco/DateTimeParser.h"
#include <iostream>


using Poco::LocalDateTime;
using Poco::DateTime;
using Poco::DateTimeFormat;
using Poco::DateTimeFormatter;
using Poco::DateTimeParser;


int main(int argc, char** argv)
{
    LocalDateTime now;
    std::cout<<"年："<<now.year()<<std::endl;
    std::cout<<"月："<<now.month()<<std::endl;
    std::cout<<"日："<<now.day()<<std::endl;
    std::cout<<"时："<<now.hour()<<std::endl;
    std::cout<<"分："<<now.minute()<<std::endl;
    std::cout<<"秒："<<now.second()<<std::endl;

   std::cout<<"本周中的第d%天："<<now.dayOfWeek()<<std::endl;
   std::cout<<"本年中的第d%天："<<now.dayOfYear()<<std::endl;
   std::cout<<"儒略日："<<now.julianDay()<<std::endl;

    std::string str = DateTimeFormatter::format(now, DateTimeFormat::ISO8601_FORMAT);
    std::cout<<"标准格式时间："<<str<<std::endl;

    std::string str_http = DateTimeFormatter::format(now, DateTimeFormat::HTTP_FORMAT);
    std::cout<<"http格式时间："<<str_http<<std::endl;

    std::string str_asctime = DateTimeFormatter::format(now, DateTimeFormat::ASCTIME_FORMAT);
    std::cout<<"ANSI格式时间："<<str_asctime<<std::endl;

    std::string str_simple = DateTimeFormatter::format(now, DateTimeFormat::SORTABLE_FORMAT);
    std::cout<<"简明格式时间："<<str_simple<<std::endl;


    DateTime dt;
    int tzd;
    DateTimeParser::parse(DateTimeFormat::ISO8601_FORMAT, str, dt, tzd);
    dt.makeUTC(tzd);
    LocalDateTime ldt(tzd, dt);
	
	std::string str_simple2 = DateTimeFormatter::format(ldt, DateTimeFormat::SORTABLE_FORMAT);
    std::cout<<"简明格式时间："<<str_simple2<<std::endl;
    return 0;
}
