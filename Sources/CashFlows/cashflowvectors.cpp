
/*
 * Copyright (C) 2000-2001 QuantLib Group
 *
 * This file is part of QuantLib.
 * QuantLib is a C++ open source library for financial quantitative
 * analysts and developers --- http://quantlib.sourceforge.net/
 *
 * QuantLib is free software and you are allowed to use, copy, modify, merge,
 * publish, distribute, and/or sell copies of it under the conditions stated
 * in the QuantLib License.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE. See the license for more details.
 *
 * You should have received a copy of the license along with this file;
 * if not, contact ferdinando@ametrano.net
 * The license is also available at http://quantlib.sourceforge.net/LICENSE.TXT
 *
 * The members of the QuantLib Group are listed in the Authors.txt file, also
 * available at http://quantlib.sourceforge.net/Authors.txt
*/

/*! \file cashflowvectors.cpp
    \brief Cash flow vector builders

    $Id$
*/

//  $Source$
//  $Log$
//  Revision 1.1  2001/05/31 08:56:40  lballabio
//  Cash flows, scheduler, and generic swap added - the latter should be specialized and tested
//

#include "ql/CashFlows/cashflowvectors.hpp"
#include "ql/scheduler.hpp"

namespace QuantLib {

    namespace CashFlows {
        
        FixedRateCouponVector::FixedRateCouponVector(
          const std::vector<double>& nominals, 
          const std::vector<Rate>& couponRates, 
          const Date& startDate, const Date& endDate, 
          int frequency, const Handle<Calendar>& calendar, 
          RollingConvention rollingConvention, bool isAdjusted, 
          const Handle<DayCounter>& dayCount, const Date& stubDate, 
          const Handle<DayCounter>& firstPeriodDayCount) {
            Scheduler scheduler(calendar, startDate, endDate, frequency, 
                rollingConvention, isAdjusted, stubDate);
            // safety check - don't know how to handle single period
            QL_REQUIRE(scheduler.size() >= 3, "illegal coupon schedule");
            QL_REQUIRE(couponRates.size() != 0, "unspecified coupon rates");
            QL_REQUIRE(nominals.size() != 0, "unspecified nominals");
            // first period might be short or long
            Date start = scheduler.date(0), end = scheduler.date(1);
            Rate rate = couponRates[0];
            double nominal = nominals[0];
            Handle<DayCounter> firstDC;
            if (firstPeriodDayCount.isNull())
                firstDC = dayCount;
            else
                firstDC = firstPeriodDayCount;
            if (scheduler.isRegular(1)) {
                push_back(Handle<CashFlow>(
                    new FixedRateCoupon(nominal, rate, calendar, 
                        rollingConvention, firstDC, 
                        start, end, start, end)));
            } else {
                Date reference = end.plusMonths(-12/frequency);
                if (isAdjusted)
                    reference = 
                        calendar->roll(reference,rollingConvention);
                push_back(Handle<CashFlow>(
                    new FixedRateCoupon(nominal, rate, calendar, 
                        rollingConvention, firstDC, 
                        start, end, reference, end)));
            }
            // regular periods
            for (int i=2; i<scheduler.size()-1; i++) {
                start = end; end = scheduler.date(i);
                if ((i-1) < couponRates.size())
                    rate = couponRates[i-1];
                else
                    rate = couponRates.back();
                if ((i-1) < nominals.size())
                    nominal = nominals[i-1];
                else
                    nominal = nominals.back();
                push_back(Handle<CashFlow>(
                    new FixedRateCoupon(nominal, rate, calendar, 
                        rollingConvention, dayCount, start, end, 
                        start, end)));
            }
            // last period might be short or long
            int N = scheduler.size();
            start = end; end = scheduler.date(N-1);
            if ((N-2) < couponRates.size())
                rate = couponRates[N-2];
            else
                rate = couponRates.back();
            if ((N-2) < nominals.size())
                nominal = nominals[N-2];
            else
                nominal = nominals.back();
            if (scheduler.isRegular(N-1)) {
                push_back(Handle<CashFlow>(
                    new FixedRateCoupon(nominal, rate, calendar, 
                        rollingConvention, dayCount, start, end, 
                        start, end)));
            } else {
                Date reference = start.plusMonths(12/frequency);
                if (isAdjusted)
                    reference = 
                        calendar->roll(reference,rollingConvention);
                push_back(Handle<CashFlow>(
                    new FixedRateCoupon(nominal, rate, calendar, 
                        rollingConvention, dayCount, start, end, 
                        start, reference)));
            }
            
        }

        IndexLinkedCouponVector::IndexLinkedCouponVector(
          const std::vector<double>& nominals, 
          const Handle<Index>& index, int fixingDays,
          const std::vector<Spread>& spreads, 
          const Date& startDate, const Date& endDate, 
          int frequency, const Handle<Calendar>& calendar, 
          RollingConvention rollingConvention, bool isAdjusted, 
          const Handle<DayCounter>& dayCount, const Date& stubDate, 
          const Handle<DayCounter>& firstPeriodDayCount) {
            Scheduler scheduler(calendar, startDate, endDate, frequency, 
                rollingConvention, isAdjusted, stubDate);
            // safety check - don't know how to handle single period
            QL_REQUIRE(scheduler.size() >= 3, "illegal coupon schedule");
            QL_REQUIRE(nominals.size() != 0, "unspecified nominals");
            // first period might be short or long
            Date start = scheduler.date(0), end = scheduler.date(1);
            Spread spread;
            if (spreads.size() > 0)
                spread = spreads[0];
            else
                spread = 0.0;
            double nominal = nominals[0];
            Handle<DayCounter> firstDC;
            if (firstPeriodDayCount.isNull())
                firstDC = dayCount;
            else
                firstDC = firstPeriodDayCount;
            if (scheduler.isRegular(1)) {
                push_back(Handle<CashFlow>(
                    new IndexLinkedCoupon(nominal, index, 12/frequency, Months, 
                        spread, calendar, rollingConvention, 
                        firstDC, start, end, fixingDays, 
                        start, end)));
            } else {
                Date reference = end.plusMonths(-12/frequency);
                if (isAdjusted)
                    reference = 
                        calendar->roll(reference,rollingConvention);
                push_back(Handle<CashFlow>(
                    new IndexLinkedCoupon(nominal, index, 12/frequency, Months, 
                        spread, calendar, rollingConvention, 
                        firstDC, start, end, fixingDays, 
                        reference, end)));
            }
            // regular periods
            for (int i=2; i<scheduler.size()-1; i++) {
                start = end; end = scheduler.date(i);
                if ((i-1) < spreads.size())
                    spread = spreads[i-1];
                else if (spreads.size() > 0)
                    spread = spreads.back();
                else
                    spread = 0.0;
                if ((i-1) < nominals.size())
                    nominal = nominals[i-1];
                else 
                    nominal = nominals.back();
                push_back(Handle<CashFlow>(
                    new IndexLinkedCoupon(nominal, index, 12/frequency, Months, 
                        spread, calendar, rollingConvention, 
                        dayCount, start, end, fixingDays, start, end)));
            }
            // last period might be short or long
            int N = scheduler.size();
            start = end; end = scheduler.date(N-1);
            if ((N-2) < spreads.size())
                spread = spreads[N-2];
            else if (spreads.size() > 0)
                spread = spreads.back();
            else
                spread = 0.0;
            if ((N-2) < nominals.size())
                nominal = nominals[N-2];
            else 
                nominal = nominals.back();
            if (scheduler.isRegular(N-1)) {
                push_back(Handle<CashFlow>(
                    new IndexLinkedCoupon(nominal, index, 12/frequency, Months, 
                        spread, calendar, rollingConvention, 
                        dayCount, start, end, fixingDays, start, end)));
            } else {
                Date reference = start.plusMonths(12/frequency);
                if (isAdjusted)
                    reference = 
                        calendar->roll(reference,rollingConvention);
                push_back(Handle<CashFlow>(
                    new IndexLinkedCoupon(nominal, index, 12/frequency, Months, 
                        spread, calendar, rollingConvention, 
                        dayCount, start, end, fixingDays, 
                        start, reference)));
            }
        }

    }

}

