/* -*- mode: c++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */

/*
 Copyright (C) 2006 Giorgio Facchinetti

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

/*! \file swaptionvolcubebysabr.hpp
    \brief Swaption volatility cube, fit-early-interpolate-later approach
*/

#ifndef quantlib_swaption_volcube_fit_early_interpolate_later_h
#define quantlib_swaption_volcube_fit_early_interpolate_later_h

#include <ql/Volatilities/swaptionvolcube.hpp>
#include <ql/Math/bilinearinterpolation.hpp>
#include <ql/Patterns/lazyobject.hpp>

namespace QuantLib {

    class SwaptionVolCube1 : public SwaptionVolatilityCube {
        class Cube {
          public:
            Cube() {};
            Cube(const std::vector<Date>& optionDates,
                 const std::vector<Period>& swapTenors,
                 const std::vector<Time>& optionTimes,
                 const std::vector<Time>& swapLengths,
                 Size nLayers,
                 bool extrapolation = true);
            Cube& operator=(const Cube& o);
            Cube(const Cube&);
            virtual ~Cube() {};
            void setElement(Size IndexOfLayer,
                            Size IndexOfRow,
                            Size IndexOfColumn,
                            Real x);
            void setPoints(const std::vector<Matrix>& x);
            void setPoint(const Date& optionDate,
                          const Period& swapTenor,
                          const Time optionTime,
                          const Time swapLengths,
                          const std::vector<Real>& point);
            void setLayer(Size i,
                          const Matrix& x);
            void expandLayers(Size i,
                              bool expandOptionTimes,
                              Size j,
                              bool expandSwapLengths);
            const std::vector<Date>& optionDates() const {
                return optionDates_;
            }
            const std::vector<Period>& swapTenors() const {
                return swapTenors_;
            }
            const std::vector<Time>& optionTimes() const;
            const std::vector<Time>& swapLengths() const;
            const std::vector<Matrix>& points() const;
            std::vector<Real> operator()(const Time optionTime,
                                         const Time swapLengths) const;
            void updateInterpolators()const;
            Matrix browse() const;
          private:
            std::vector<Time> optionTimes_, swapLengths_;
            std::vector<Date> optionDates_;
            std::vector<Period> swapTenors_;
            Size nLayers_;
            std::vector<Matrix> points_;
            mutable std::vector<Disposable<Matrix> > transposedPoints_;
            bool extrapolation_;
            mutable std::vector< boost::shared_ptr<BilinearInterpolation> > interpolators_;
         };
      public:
        SwaptionVolCube1(
            const Handle<SwaptionVolatilityStructure>& atmVolStructure,
            const std::vector<Period>& optionTenors,
            const std::vector<Period>& swapTenors,
            const std::vector<Spread>& strikeSpreads,
            const std::vector<std::vector<Handle<Quote> > >& volSpreads,
            const boost::shared_ptr<SwapIndex>& swapIndexBase,
            bool vegaWeightedSmileFit,
            const Matrix& parametersGuess,
            const std::vector<bool>& isParameterFixed,
            bool isAtmCalibrated);
        ////! \name LazyObject interface
        ////@{
        void performCalculations() const;
        ////@}
        //! \name SwaptionVolatilityCube interface
        //@{
        boost::shared_ptr<SmileSection> smileSection(
                                              Time optionTime,
                                              Time swapLength) const;
        boost::shared_ptr<SmileSection> smileSection(
                                              const Date& optionDate,
                                              const Period& swapTenor) const;
        //@}
        //! \name Other inspectors
        //@{
        const Matrix& marketVolCube(Size i) const {
            return marketVolCube_.points()[i];
        }
        Matrix sparseSabrParameters() const;
        Matrix denseSabrParameters() const;
        Matrix marketVolCube() const;
        Matrix volCubeAtmCalibrated() const;
        //@}
        void sabrCalibrationSection(const Cube& marketVolCube,
                                    const Period& swapTenor) const;
        void recalibration(Real beta,
                           const Period& swapTenor);
     protected:
        boost::shared_ptr<SmileSection> smileSection(
                                    Time optionTime,
                                    Time swapLength,
                                    const Cube& sabrParametersCube) const;
        Cube sabrCalibration(const Cube& marketVolCube) const;
        void fillVolatilityCube() const;
        void createSparseSmiles() const;
        std::vector<Real> spreadVolInterpolation(const Date& atmOptionDate,
                                                 const Period& atmSwapTenor) const;
      private:
        mutable Cube marketVolCube_;
        mutable Cube volCubeAtmCalibrated_;
        mutable Cube sparseParameters_;
        mutable Cube denseParameters_;
        mutable std::vector< std::vector<boost::shared_ptr<SmileSection> > >
                                                                sparseSmiles_;
        mutable Cube parametersGuess_;
        std::vector<bool> isParameterFixed_;
        bool isAtmCalibrated_;
    };
    
}

#endif