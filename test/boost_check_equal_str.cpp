#include <boost/test/test_tools.hpp>

int test_main( int, char *[] )
{
    char const* buf_ptr_cch     = "abc";
    char const  buf_array_cch[] = "abc";
    char*       buf_ptr_ch      = "abc";
    char        buf_array_ch[]  = "abc";
    std::string buf_str         = "abc";

    BOOST_CHECK_EQUAL(buf_ptr_cch, buf_ptr_cch);
    BOOST_CHECK_EQUAL(buf_ptr_cch, buf_array_cch);
    BOOST_CHECK_EQUAL(buf_ptr_cch, buf_ptr_ch);
    BOOST_CHECK_EQUAL(buf_ptr_cch, buf_array_ch);
    BOOST_CHECK_EQUAL(buf_ptr_cch, buf_str);

    BOOST_CHECK_EQUAL(buf_array_cch, buf_ptr_cch);
    BOOST_CHECK_EQUAL(buf_array_cch, buf_array_cch);
    BOOST_CHECK_EQUAL(buf_array_cch, buf_ptr_ch);
    BOOST_CHECK_EQUAL(buf_array_cch, buf_array_ch);
    BOOST_CHECK_EQUAL(buf_array_cch, buf_str);

    BOOST_CHECK_EQUAL(buf_ptr_ch, buf_ptr_cch);
    BOOST_CHECK_EQUAL(buf_ptr_ch, buf_array_cch);
    BOOST_CHECK_EQUAL(buf_ptr_ch, buf_ptr_ch);
    BOOST_CHECK_EQUAL(buf_ptr_ch, buf_array_ch);
    BOOST_CHECK_EQUAL(buf_ptr_ch, buf_str);

    BOOST_CHECK_EQUAL(buf_array_ch, buf_ptr_cch);
    BOOST_CHECK_EQUAL(buf_array_ch, buf_array_cch);
    BOOST_CHECK_EQUAL(buf_array_ch, buf_ptr_ch);
    BOOST_CHECK_EQUAL(buf_array_ch, buf_array_ch);
    BOOST_CHECK_EQUAL(buf_array_ch, buf_str);

    BOOST_CHECK_EQUAL(buf_str, buf_ptr_cch);
    BOOST_CHECK_EQUAL(buf_str, buf_array_cch);
    BOOST_CHECK_EQUAL(buf_str, buf_ptr_ch);
    BOOST_CHECK_EQUAL(buf_str, buf_array_ch);
    BOOST_CHECK_EQUAL(buf_str, buf_str);

    return 0;
}

// EOF
