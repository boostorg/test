#include <iostream>

namespace ieee_754 {

template<typename T>
struct decoded {
    typedef long long mantissa_holder_type;
    typedef short     exponent_holder_type;

    bool                  p_sign;
    mantissa_holder_type  p_mantissa;
    exponent_holder_type  p_exponent;
};

//___________________________________________________________________________//

void decode( double v, decoded<double>& d )
{
    union {
        double v;
        long long m;
    } tmp;

    tmp.v = v;

    d.p_sign     = !(tmp.m & 0x8000000000000000LL);
    d.p_mantissa = tmp.m & 0x000FFFFFFFFFFFFFLL;
    d.p_exponent = (short)((tmp.m & 0x7FF0000000000000LL) >> 52) - 1075;

    if( d.p_exponent != 0 )
        d.p_mantissa |= 0x0010000000000000LL;
}

//___________________________________________________________________________//

} // namespace ieee_754

using namespace ieee_754;

int
main()
{
    double d = 0.2;
    decoded<double> dec;

    decode( d, dec );

    std::cout << "orig = "           << std::hex << *(long long*)&d << std::endl;
    std::cout << "dec.p_sign = "     << (dec.p_sign ? '+' : '-') << std::endl;
    std::cout << "dec.p_mantissa = " << std::hex << dec.p_mantissa << std::endl;
    std::cout << "dec.p_exponent = " << std::dec << dec.p_exponent << std::endl;
}
