
/*
 Copyright (C) 2003 Ferdinando Ametrano

 This file is part of QuantLib, a free-software/open-source library
 for financial quantitative analysts and developers - http://quantlib.org/

 QuantLib is free software: you can redistribute it and/or modify it under the
 terms of the QuantLib license.  You should have received a copy of the
 license along with this program; if not, please email quantlib-dev@lists.sf.net
 The license is also available online at http://quantlib.org/html/license.html

 This program is distributed in the hope that it will be useful, but WITHOUT
 ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 FOR A PARTICULAR PURPOSE.  See the license for more details.
*/

/*! \file asianoption.cpp
    \brief Asian option (average rate) on a single asset
*/

#include <ql/Instruments/asianoption.hpp>

namespace QuantLib {

    DiscreteAveragingAsianOption::DiscreteAveragingAsianOption(
                         Average::Type averageType,
                         Handle<StrikedTypePayoff> payoff,
                         const RelinkableHandle<Quote>& underlying,
                         double runningProduct,
                         Size pastFixings,
                         std::vector<Date> fixingDates,
                         const RelinkableHandle<TermStructure>& dividendTS,
                         const RelinkableHandle<TermStructure>& riskFreeTS,
                         const Exercise& exercise,
                         const RelinkableHandle<BlackVolTermStructure>& volTS,
                         const Handle<PricingEngine>& engine,
                         const std::string& isinCode,
                         const std::string& description)
    : OneAssetStrikedOption(payoff, underlying, dividendTS, riskFreeTS,
      exercise, volTS, engine, isinCode, description),
      averageType_(averageType), 
      runningProduct_(runningProduct), pastFixings_(pastFixings),
      fixingDates_(fixingDates) {
        std::sort(fixingDates_.begin(), fixingDates_.end());
    }



    void DiscreteAveragingAsianOption::setupArguments(Arguments* args) const {

        DiscreteAveragingAsianOption::arguments* moreArgs =
            dynamic_cast<DiscreteAveragingAsianOption::arguments*>(args);
        QL_REQUIRE(moreArgs != 0,
                   "DiscreteAveragingAsianOption::setupArguments : "
                   "wrong argument type");
        moreArgs->averageType = averageType_;
        moreArgs->runningProduct = runningProduct_;
        moreArgs->pastFixings = pastFixings_;
        moreArgs->fixingDates = fixingDates_;

        OneAssetStrikedOption::arguments* arguments =
            dynamic_cast<OneAssetStrikedOption::arguments*>(args);
        QL_REQUIRE(arguments != 0,
                   "DiscreteAveragingAsianOption::setupArguments : "
                   "wrong argument type");
        OneAssetStrikedOption::setupArguments(arguments);

    }

    void DiscreteAveragingAsianOption::performCalculations() const {
        // enforce in this class any check on engine/payoff coupling
        OneAssetStrikedOption::performCalculations();
    }

    void DiscreteAveragingAsianOption::arguments::validate() const {

        #if defined(QL_PATCH_MICROSOFT)
        OneAssetStrikedOption::arguments copy = *this;
        copy.validate();
        #else
        OneAssetStrikedOption::arguments::validate();
        #endif

        QL_REQUIRE(runningProduct >= 0.0,
                   "DiscreteAveragingAsianOption::arguments::validate() : "
                   "negative running product");

        // check fixingTimes_ here
    }

}

