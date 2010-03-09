
#include "tweexx.hpp"


struct push_hash
{
    template <typename S, typename C>
    struct result { typedef void type; };

    void operator()(std::string& buf, std::string str) const
    {
        buf += "<#" + str + "#>";
    }
};
struct push_at
{
    template <typename S, typename C>
    struct result { typedef void type; };

    void operator()(std::string& buf, std::string str) const
    {
        buf += "<@" + str + "@>";
    }
};
struct push_url
{
    template <typename S, typename C>
    struct result { typedef void type; };

    void operator()(std::string& buf, std::string str) const
    {
        buf += "<<" + str + ">>";
    }
};


bool test(const std::string& input, const std::string& expected, bool should_pass = true)
{

    typedef std::string::const_iterator iter_t;

    iter_t first = input.begin();
    iter_t last = input.end();

    runpac::tweet_parser<iter_t, push_at, push_hash, push_url> p;

    std::string result;
    if (boost::spirit::qi::parse(first, last, p, result) && first == last && result == expected)
        std::cout << " PASS " << input << std::endl;
    else
        std::cout << "-FAIL " << input << "\n"
                  << "  exp " << expected << "\n"
                  << "  out " << result << std::endl;
}


int main()
{

    test("a"
        ,"a"
    );
    test("ab cd!"
        ,"ab cd!"
    );

    test("@ab"
        ,"<@ab@>"
    );
    test("@ab_cd"
        ,"<@ab_cd@>"
    );
    test("@ab-cd"
        ,"<@ab@>-cd"
    );

    test("#ab"
        ,"<#ab#>"
    );
    test("#ab_cd"
        ,"<#ab_cd#>"
    );
    test("#ab-cd"
        ,"<#ab#>-cd"
    );

    test("http://ab.cd/ef.gh"
        ,"<<ab.cd/ef.gh>>"
    );
    test("http://ab.cd/ef.gh?ee=33&ww=42."
        ,"<<ab.cd/ef.gh?ee=33&ww=42>>."
    );
    test("http://ab.cd/ef.gh?ee=33&ww=42#ee."
         ,"<<ab.cd/ef.gh?ee=33&ww=42#ee>>."
    );

    test("RT @mongodb: #State! of #MongoDB @March, 2010 http://blog.mongodb.org/post/434865639/state-of-mongodb-march-2010. OK"
        ,"RT <@mongodb@>: <#State#>! of <#MongoDB#> <@March@>, 2010 <<blog.mongodb.org/post/434865639/state-of-mongodb-march-2010>>. OK"
    );

    std::string res;
    bool b = runpac::tweexx(
        "RT @mongodb: #State! of #MongoDB @March, 2010 http://blog.mongodb.org/post/434865639/state-of-mongodb-march-2010. OK"
      , res
    );

    std::cout << b << " " << res << std::endl;

 }
