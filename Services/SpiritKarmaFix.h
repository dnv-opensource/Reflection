#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include <boost/spirit/include/karma.hpp>
#include <boost/spirit/include/karma_int.hpp>
#include <boost/spirit/include/karma_real.hpp>

namespace boost { namespace spirit { namespace karma 
{

	template <typename T, typename Policies, typename CharEncoding, typename Tag>
	struct real_inserter_workaround
{
	template <typename OutputIterator, typename U>
	static bool
		call(OutputIterator& sink, U n, Policies const& p = Policies())
	{
		if (traits::test_nan(n)) {
			return p.template nan<CharEncoding, Tag>(
				sink, n, p.force_sign(n));
		}
		else if (traits::test_infinite(n)) {
			return p.template inf<CharEncoding, Tag>(
				sink, n, p.force_sign(n));
		}
		return p.template call<real_inserter_workaround>(sink, n, p);
	}

#if BOOST_WORKAROUND(BOOST_MSVC, >= 1400)  
# pragma warning(push)
# pragma warning(disable: 4100)   // 'p': unreferenced formal parameter  
# pragma warning(disable: 4127)   // conditional expression is constant
# pragma warning(disable: 4267)   // conversion from 'size_t' to 'unsigned int', possible loss of data
#endif 
	///////////////////////////////////////////////////////////////////////
	//  This is the workhorse behind the real generator
	///////////////////////////////////////////////////////////////////////
	template <typename OutputIterator, typename U>
	static bool
		call_n(OutputIterator& sink, U n, Policies const& p)
	{
		// prepare sign and get output format
		bool force_sign = p.force_sign(n);
		bool sign_val = false;
		int flags = p.floatfield(n);
		if (traits::test_negative(n))
		{
			n = -n;
			sign_val = true;
		}

		// The scientific representation requires the normalization of the 
		// value to convert.

			// get correct precision for generated number
		unsigned precision = p.precision(n);
		if (std::numeric_limits<U>::digits10)
		{
			// limit generated precision to digits10, if defined
			precision = (std::min)(precision,
				(unsigned)std::numeric_limits<U>::digits10 + 1);
		}

		// allow for ADL to find the correct overloads for log10 et.al.
		using namespace std;

		U dim = 0;
		if (0 == (Policies::fmtflags::fixed & flags) && !traits::test_zero(n))
		{
			dim = log10(n);
			if (dim > 0)
				n /= spirit::traits::pow10<U>(traits::truncate_to_long::call(dim));
			else if (n < 1.) {
				long exp = traits::truncate_to_long::call(-dim);
				if (exp != -dim)
					++exp;
				dim = static_cast<U>(-exp);
				n *= spirit::traits::pow10<U>(exp);
			}
		}

		// prepare numbers (sign, integer and fraction part)
		U integer_part;
		U precexp = spirit::traits::pow10<U>(precision);
		U fractional_part = modf(n, &integer_part);

		fractional_part = floor(fractional_part * precexp + U(0.5));
		if (fractional_part >= precexp)
		{
			fractional_part = floor(fractional_part - precexp);
			integer_part += 1;    // handle rounding overflow
		}

		// if trailing zeros are to be omitted, normalize the precision and
		// fractional part
		U long_int_part = floor(integer_part);
		U long_frac_part = fractional_part;
		unsigned prec = precision;
		if (!p.trailing_zeros(n))
		{
			U frac_part_floor = long_frac_part;
			if (0 != long_frac_part) {
				// remove the trailing zeros
				while (0 != prec &&
					0 == traits::remainder<10>::call(long_frac_part))
				{
					long_frac_part = traits::divide<10>::call(long_frac_part);
					--prec;
				}
			}
			else {
				// if the fractional part is zero, we don't need to output 
				// any additional digits
				prec = 0;
			}

			if (precision != prec)
			{
				long_frac_part = frac_part_floor /
					spirit::traits::pow10<U>(precision - prec);
			}
		}

		// call the actual generating functions to output the different parts
		if ((force_sign || sign_val) &&
			traits::test_zero(long_int_part) &&
			traits::test_zero(long_frac_part))
		{
			sign_val = false;     // result is zero, no sign please
			force_sign = false;
		}

		// generate integer part
		bool r = p.integer_part(sink, long_int_part, sign_val, force_sign);

		// generate decimal point
		r = r && p.dot(sink, long_frac_part, precision);

		// generate fractional part with the desired precision
		r = r && p.fraction_part(sink, long_frac_part, prec, precision);

		if (r && 0 == (Policies::fmtflags::fixed & flags)) {
			return p.template exponent<CharEncoding, Tag>(sink,
				traits::truncate_to_long::call(dim));
		}
		return r;
	}

#if BOOST_WORKAROUND(BOOST_MSVC, >= 1400)
# pragma warning(pop)
#endif 

};
}}}
