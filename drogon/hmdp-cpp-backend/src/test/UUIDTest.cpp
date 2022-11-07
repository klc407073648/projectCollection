#include "Poco/UUID.h"
#include "Poco/UUIDGenerator.h"
#include <iostream>
using Poco::UUID;
using Poco::UUIDGenerator;

int main(int argc, char **argv)
{
    UUIDGenerator &generator = UUIDGenerator::defaultGenerator();
    UUID uuid1(generator.create()); // time based
    UUID uuid2(generator.createRandom());
    UUID uuid3(generator.createFromName(UUID::uri(), "http://appinf.com"));
    std::cout << uuid1.toString() << std::endl;
    std::cout << uuid2.toString() << std::endl;
    std::cout << uuid3.toString() << std::endl;
    return 0;
}