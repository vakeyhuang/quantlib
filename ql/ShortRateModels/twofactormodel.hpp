/*
 Copyright (C) 2001, 2002 Sadruddin Rejeb

 This file is part of QuantLib, a free-software/open-source library
 for financial quantitative analysts and developers - http://quantlib.org/

 QuantLib is free software: you can redistribute it and/or modify it under the
 terms of the QuantLib license.  You should have received a copy of the
 license along with this program; if not, please email ferdinando@ametrano.net
 The license is also available online at http://quantlib.org/html/license.html

 This program is distributed in the hope that it will be useful, but WITHOUT
 ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 FOR A PARTICULAR PURPOSE.  See the license for more details.
*/
/*! \file twofactormodel.hpp
    \brief Abstract two-factor interest rate model class

    \fullpath
    ql/ShortRateModels/%twofactormodel.hpp
*/

// $Id$

#ifndef quantlib_interest_rate_modelling_two_factor_model_h
#define quantlib_interest_rate_modelling_two_factor_model_h

#include <ql/diffusionprocess.hpp>
#include <ql/ShortRateModels/model.hpp>
#include <ql/Lattices/twodimensionaltree.hpp>

namespace QuantLib {

    namespace ShortRateModels {

        //! Abstract base-class for two-factor models
        class TwoFactorModel : public Model {
          public:
            TwoFactorModel(Size nParams);

            class ShortRateDynamics;

            //! Returns the short-rate dynamics
            virtual Handle<ShortRateDynamics> dynamics() const = 0;

            //! Returns a two-dimensional trinomial tree
            virtual Handle<Lattices::Tree> tree(const TimeGrid& grid) const;

          protected:
            class ShortRateDiscounting;
        };

        //! Class describing the dynamics of the two state variables
        /*! We assume here that the short-rate is a function of two state
            variables x and y.
            \f[
                r_t = f(t, x_t, y_t)
            \f]
            of two state variables \f$ x_t \f$ and \f$ y_t \f$. These stochastic
            processes satisfy
            \f[
                x_t = \mu_x(t, x_t)dt + \sigma_x(t, x_t) dW_t^x
            \f]
            and
            \f[
                y_t = \mu_y(t,y_t)dt + \sigma_y(t, y_t) dW_t^y
            \f]
            where \f$ W^x \f$ and \f$ W^y \f$ are two brownian motions 
            satisfying
            \f[
                dW^x_t dW^y_t = \rho dt
            \f].
        */
                
        class TwoFactorModel::ShortRateDynamics {
          public:
            ShortRateDynamics(const Handle<DiffusionProcess>& xProcess,
                              const Handle<DiffusionProcess>& yProcess,
                              double correlation)
            : xProcess_(xProcess), yProcess_(yProcess), 
              correlation_(correlation) {}
            virtual ~ShortRateDynamics() {}

            virtual Rate shortRate(Time t, double x, double y) const = 0;

            //! Risk-neutral dynamics of the first state variable x
            const Handle<DiffusionProcess>& xProcess() const {
                return xProcess_;
            }

            //! Risk-neutral dynamics of the second state variable y
            const Handle<DiffusionProcess>& yProcess() const {
                return yProcess_;
            }

            //! Correlation \f$ \rho \f$ between the two brownian motions.
            double correlation() const {
                return correlation_;
            }
          private:
            Handle<DiffusionProcess> xProcess_, yProcess_;
            double correlation_;
        };

        class TwoFactorModel::ShortRateDiscounting 
            : public Lattices::Discounting {
          public:
            ShortRateDiscounting(
                const Handle<ShortRateDynamics>& process,
                const Handle<Lattices::TwoDimensionalBranching>& branching,
                Time t, Time dt, double dx, double dy);

            double discount(Size index) const;
          private:
            Size modulo_;
            Time t_, dt_;
            double xMin_, dx_, yMin_, dy_;
            Handle<ShortRateDynamics> dynamics_;
        };

    }

}
#endif
