#include <iostream>
#include "tweexx.hpp"



int main()
{
    std::cout << "//////////////////////////////////////\n\n";
    std::cout << "\ttweexx demo\n\n";
    std::cout << "//////////////////////////////////////\n\n";

    std::cout << "Give me tweet.\n";
    std::cout << "Type [q or Q] to quit\n\n";

    std::string str;
    typedef std::string::const_iterator iter_t;

    runpac::tweet_parser<iter_t> p;

    while (getline(std::cin, str))
    {
        if (str.empty() || str[0] == 'q' || str[0] == 'Q')
            break;

        std::string res;
        iter_t first = str.begin();
        iter_t last  = str.end();

        if (boost::spirit::qi::parse(first, last, p, res))
        {
            std::cout << "-------------------------\n";
            std::cout << "Parsing succeeded:\n";
            std::cout << res << std::endl;
            std::cout << "-------------------------\n";
        }
        else
        {
            std::cout << "-------------------------\n";
            std::cout << "Parsing failed\n";
            std::cout << "-------------------------\n";
        }
    }

    std::cout << "Bye... \n\n";
    return 0;
}
