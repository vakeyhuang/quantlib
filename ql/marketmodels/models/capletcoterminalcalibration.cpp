/* -*- mode: c++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */

/*
 Copyright (C) 2007 Ferdinando Ametrano
 Copyright (C) 2007 Mark Joshi

 This file is part of QuantLib, a free-software/open-source library
 for financial quantitative analysts and developers - http://quantlib.org/

 QuantLib is free software: you can redistribute it and/or modify it
 under the terms of the QuantLib license.  You should have received a
 copy of the license along with this program; if not, please email
 <quantlib-dev@lists.sf.net>. The license is also available online at
 <http://quantlib.org/reference/license.html>.

 This program is distributed in the hope that it will be useful, but WITHOUT
 ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 FOR A PARTICULAR PURPOSE.  See the license for more details.
*/

#include <ql/marketmodels/models/capletcoterminalcalibration.hpp>
#include <ql/marketmodels/models/piecewiseconstantvariance.hpp>
#include <ql/marketmodels/models/timedependantcorrelationstructure.hpp>
#include <ql/marketmodels/curvestate.hpp>
#include <ql/marketmodels/evolutiondescription.hpp>
#include <ql/marketmodels/marketmodel.hpp>
#include <ql/math/matrix.hpp>

namespace QuantLib {

    bool capletCoterminalCalibration(
        const TimeDependantCorrelationStructure& corr,
        const std::vector<boost::shared_ptr<PiecewiseConstantVariance> >& displacedSwapVariances,
        const std::vector<Volatility>& capletVols,
        const CurveState& cs,
        const Spread displacement,
        const std::vector<Real>& alpha,
        std::vector<Matrix>& pseudoRoots) {

        const EvolutionDescription& evolution = corr.evolution();
        //for (Size i=0; i<displacedSwapVariances.size(); ++i)
        //    QL_REQUIRE(evolution==displacedSwapVariances[i]->evolution(),
        //               "mismatched evolutions");

        Size numberOfRates_ = evolution.numberOfRates();

        QL_REQUIRE(numberOfRates_==displacedSwapVariances.size(),
                   "mismatch between number of rates (" << numberOfRates_ <<
                   ") and displacedSwapVariances");

        QL_REQUIRE(numberOfRates_==capletVols.size(),
                   "mismatch between number of rates (" << numberOfRates_ <<
                   ") and capletVols");

        const std::vector<Time>& rateTimes = evolution.rateTimes();
        QL_REQUIRE(rateTimes==cs.rateTimes(),
                   "mismatch between EvolutionDescriptionand CurveState rate times ");
        QL_REQUIRE(numberOfRates_==cs.numberOfRates(),
                   "mismatch between number of rates (" << numberOfRates_ <<
                   ") and CurveState");

        QL_REQUIRE(numberOfRates_==alpha.size(),
                   "mismatch between number of rates (" << numberOfRates_ <<
                   ") and alphas (" << alpha.size() << ")");

        const std::vector<Time>& evolutionTimes = evolution.evolutionTimes();
        QL_REQUIRE(std::vector<Time>(rateTimes.begin(), rateTimes.end()-1)==evolutionTimes,
                   "mismatch between evolutionTimes and rateTimes");

        Size numberOfSteps = evolution.numberOfSteps();
        Size numberOfFactors = corr.numberOfFactors();

       Matrix swapTimeInhomogeneousVariances(numberOfSteps, numberOfRates_, 0.0);
        for (Size i=0; i<numberOfSteps; ++i) {
            Real s = (i==0 ? 0.0 : evolutionTimes[i-1]);
            for (Size j=i; j<numberOfRates_; ++j) {
                const std::vector<Real>& var = displacedSwapVariances[j]->variances();
                swapTimeInhomogeneousVariances[i][j] = var[j]/
                    ((1.0+alpha[j]*s)*(1.0+alpha[j]*s));
            }
        }

        const std::vector<Time>& taus = evolution.rateTaus();

        std::vector<Real> totVariance(numberOfRates_, 0.0);
        std::vector<Real> almostTotVariance(numberOfRates_, 0.0);
        
        std::vector<Real> almostTotCovariance(numberOfRates_, 0.0);
        std::vector<Real> leftCovariance(numberOfRates_, 0.0);


        for (Size i=0; i<numberOfRates_; ++i) {
            for (Size j=0; j<=i; ++j)
                totVariance[i] += displacedSwapVariances[i]->variances()[j];
            for (Integer j=0; j<=static_cast<Integer>(i)-1; ++j)
                almostTotVariance[i] += swapTimeInhomogeneousVariances[j][i];
            Integer j=0;
            for (; j<=static_cast<Integer>(i)-2; ++j) {
                 const Matrix& thisPseudo = corr.pseudoRoot(j);
                 Real correlation = 0.0;

                 for (Size k=0; k<numberOfFactors; ++k)
                     correlation += thisPseudo[i-1][k]*thisPseudo[i][k];
                 
                 almostTotCovariance[i] += correlation*
                                            sqrt(swapTimeInhomogeneousVariances[j][i]
                                                *swapTimeInhomogeneousVariances[j][i-1]);

            }

            if (i>0) {
                const Matrix& thisPseudo = corr.pseudoRoot(j);
                Real correlation = 0.0;
                for (Size k=0; k<numberOfFactors; ++k)
                    correlation += thisPseudo[i-1][k]*thisPseudo[i][k];
                leftCovariance[j] = correlation*
                                    sqrt(swapTimeInhomogeneousVariances[j][i]
                                         *swapTimeInhomogeneousVariances[j][i-1]);

            }
        }

        // multiplier up to rate reset previous time
        // the first element is not used
        std::vector<Real> a(numberOfSteps, 1.0);

        // multiplier afterward
        std::vector<Real> b(numberOfSteps);
        b[0]=displacedSwapVariances[0]->variances()[0]/swapTimeInhomogeneousVariances[0][0];
        for (Size i=1; i<numberOfSteps-1; ++i) 
        {
            int j=0;
            for (; j < static_cast<int>(i) -2; j++)
                swapTimeInhomogeneousVariances[j][i-1]*= a[i-1]*a[i-1];
            swapTimeInhomogeneousVariances[j][i-1]*= b[i-1]*b[i-1];

            Real sr0w0 = (cs.coterminalSwapRates()[i-1]+displacement)*
                cs.coterminalSwapAnnuity(i, i-1)/taus[i-1];
            Real sr1w1 = (cs.coterminalSwapRates()[i]+displacement)*
                         cs.coterminalSwapAnnuity(i, i)/taus[i-1];
            Real f0v1t1 = (cs.forwardRates()[i-1]+displacement)*
                          (cs.forwardRates()[i-1]+displacement)*
                capletVols[i]*capletVols[i]*rateTimes[i];

            Real constantPart = sr0w0*sr0w0*totVariance[i-1]-f0v1t1;
            Real linearPart = -2*sr0w0*sr1w1*(a[i-1]*almostTotCovariance[i]
                                              +b[i-1]*leftCovariance[i]);
            Real quadraticPart = sr1w1*sr1w1*almostTotCovariance[i];

            Real disc = linearPart*linearPart-4*constantPart*quadraticPart;

            if (disc <0)
                return false;

            Real root = (-linearPart -sqrt(disc))/(2*quadraticPart);
            if (root<0.0) {
                QL_FAIL("negative root -- it should have not happened");
                root = (-linearPart +sqrt(disc))/(2*quadraticPart);
            }
                          
            a[i]=root;

            Real varianceFound = root*root*almostTotVariance[i];
            Real varianceToFind = totVariance[i]-varianceFound;
            Real mult = varianceToFind/swapTimeInhomogeneousVariances[i][i];
            b[i]=sqrt(mult);
        }


        pseudoRoots.resize(numberOfSteps);
        for (Size k=0; k<numberOfSteps; ++k) {
            pseudoRoots[k] = corr.pseudoRoot(k);
            for (Size j=0; j<numberOfRates_; ++j) {
                Real coeff = std::sqrt(swapTimeInhomogeneousVariances[k][j]);
                coeff *= (j<k ? a[j] : b[j]);
                for (Size i=0; i<numberOfFactors; ++i) {
                    pseudoRoots[k][j][i]*=coeff;
                }
            }
            QL_ENSURE(pseudoRoots[k].rows()==numberOfRates_,
                      "step " << k
                      << " abcd vol wrong number of rows: "
                      << pseudoRoots[k].rows()
                      << " instead of " << numberOfRates_);
            QL_ENSURE(pseudoRoots[k].columns()==numberOfFactors,
                      "step " << k
                      << " abcd vol wrong number of columns: "
                      << pseudoRoots[k].columns()
                      << " instead of " << numberOfFactors);
        }

        return true;

    }

}