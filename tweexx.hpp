#pragma once

#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_container.hpp>
#include <boost/spirit/include/phoenix_statement.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/regex/pending/unicode_iterator.hpp>


namespace runpac {


namespace detail {

    struct push_hash
    {
        template <typename S, typename C>
        struct result { typedef void type; };

        void operator()(std::string& buf, std::string str) const
        {
            buf += "<a href=\"http://twitter.com/search?q=%23" + str + "\">#" + str + "</a>";
        }
    };
    struct push_at
    {
        template <typename S, typename C>
        struct result { typedef void type; };

        void operator()(std::string& buf, std::string str) const
        {
            buf += "<a href=\"http://twitter.com/" + str + "\">@" + str + "</a>";
        }
    };
    struct push_url
    {
        template <typename S, typename C>
        struct result { typedef void type; };

        void operator()(std::string& buf, std::string str) const
        {
            buf += "<a href=\"http://" + str + "\">http://" + str + "</a>";
        }
    };
}



template< typename Iterator
        , typename PushAt   = detail::push_at
        , typename PushHash = detail::push_hash
        , typename PushUrl  = detail::push_url
        >
struct tweet_parser : boost::spirit::qi::grammar<Iterator, std::string()>
{
    tweet_parser() : tweet_parser::base_type(start)
    {
        using boost::spirit::qi::alnum;
        using boost::spirit::qi::lit;
        using boost::spirit::qi::space;
        using boost::spirit::qi::eoi;
        using boost::spirit::qi::_val;
        using boost::spirit::qi::_r1;
        using boost::spirit::qi::_1;
        using boost::spirit::standard::char_;

        boost::phoenix::function<PushAt>   push_at;
        boost::phoenix::function<PushHash> push_hash;
        boost::phoenix::function<PushUrl>  push_url;

        token =
                +( alnum
                   | '_'
                   )
                ;

        url_token =
                +( alnum
                   | '_'
                   | '-'
                   | '/'
                   | '&'
                   | '='
                   | '#'
                   | (   ( char_('.')
                         | '?'
                         )
                     >> !( space
                         | eoi
                         )
                     )
                   )
                ;

        at =
                (  lit('@')
                >> token
                )            [push_at(_r1, _1)]
                ;

        hash =
                (  lit('#')
                >> token
                )            [push_hash(_r1, _1)]
                ;

        url =
                (  lit("http://")
                >> url_token
                )            [push_url(_r1, _1)]
                ;

        start =
               +( hash(_val)
                ^ at(_val)
                ^ url(_val)
                ^ char_     [_val += _1]
                )
                ;
    }
    boost::spirit::qi::rule<Iterator, std::string()>      token;
    boost::spirit::qi::rule<Iterator, std::string()>      url_token;
    boost::spirit::qi::rule<Iterator, void(std::string&)> hash;
    boost::spirit::qi::rule<Iterator, void(std::string&)> at;
    boost::spirit::qi::rule<Iterator, void(std::string&)> url;
    boost::spirit::qi::rule<Iterator, std::string()>      start;
};


// simple API
bool tweexx(const std::string& input, std::string& output)
{
    typedef std::string::const_iterator iter_t;

    iter_t first = input.begin();
    iter_t last  = input.end();

    runpac::tweet_parser<iter_t> p;

    std::string result;
    if (boost::spirit::qi::parse(first, last, p, output) && first == last)
        return true;
    else
        return false;
}


} // ns
