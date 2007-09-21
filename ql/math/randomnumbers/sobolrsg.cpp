/* -*- mode: c++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */

/*
 Copyright (C) 2003, 2004 Ferdinando Ametrano
 Copyright (C) 2006 Richard Gould

 This file is part of QuantLib, a free-software/open-source library
 for financial quantitative analysts and developers - http://quantlib.org/

 QuantLib is free software: you can redistribute it and/or modify it
 under the terms of the QuantLib license.  You should have received a
 copy of the license along with this program; if not, please email
 <quantlib-dev@lists.sf.net>. The license is also available online at
 <http://quantlib.org/license.shtml>.

 This program is distributed in the hope that it will be useful, but WITHOUT
 ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 FOR A PARTICULAR PURPOSE.  See the license for more details.
*/

#include <ql/math/randomnumbers/sobolrsg.hpp>
#include <ql/math/randomnumbers/primitivepolynomials.h>
#include <ql/math/randomnumbers/mt19937uniformrng.hpp>
#include <ql/errors.hpp>
#include <cmath>

namespace QuantLib {

    namespace {

        /* Sobol' Levitan coefficients of the free direction integers as given
           by Bratley, P., Fox, B.L. (1988)
        */
        const unsigned long dim02SLinitializers[] = {
            1UL, 0UL };
        const unsigned long dim03SLinitializers[] = {
            1UL, 1UL, 0UL };
        const unsigned long dim04SLinitializers[] = {
            1UL, 3UL, 7UL, 0UL };
        const unsigned long dim05SLinitializers[] = {
            1UL, 1UL, 5UL, 0UL };
        const unsigned long dim06SLinitializers[] = {
            1UL, 3UL, 1UL, 1UL, 0UL };
        const unsigned long dim07SLinitializers[] = {
            1UL, 1UL, 3UL, 7UL, 0UL };
        const unsigned long dim08SLinitializers[] = {
            1UL, 3UL, 3UL, 9UL, 9UL, 0UL };
        const unsigned long dim09SLinitializers[] = {
            1UL, 3UL, 7UL, 13UL, 3UL, 0UL };
        const unsigned long dim10SLinitializers[] = {
            1UL, 1UL, 5UL, 11UL, 27UL, 0UL };
        const unsigned long dim11SLinitializers[] = {
            1UL, 3UL, 5UL, 1UL, 15UL, 0UL };
        const unsigned long dim12SLinitializers[] = {
            1UL, 1UL, 7UL, 3UL, 29UL, 0UL };
        const unsigned long dim13SLinitializers[] = {
            1UL, 3UL, 7UL, 7UL, 21UL, 0UL };
        const unsigned long dim14SLinitializers[] = {
            1UL, 1UL, 1UL, 9UL, 23UL, 37UL, 0UL };
        const unsigned long dim15SLinitializers[] = {
            1UL, 3UL, 3UL, 5UL, 19UL, 33UL, 0UL };
        const unsigned long dim16SLinitializers[] = {
            1UL, 1UL, 3UL, 13UL, 11UL, 7UL, 0UL };
        const unsigned long dim17SLinitializers[] = {
            1UL, 1UL, 7UL, 13UL, 25UL, 5UL, 0UL };
        const unsigned long dim18SLinitializers[] = {
            1UL, 3UL, 5UL, 11UL, 7UL, 11UL, 0UL };
        const unsigned long dim19SLinitializers[] = {
            1UL, 1UL, 1UL, 3UL, 13UL, 39UL, 0UL };
        const unsigned long dim20SLinitializers[] = {
            1UL, 3UL, 1UL, 15UL, 17UL, 63UL, 13UL, 0UL };
        const unsigned long dim21SLinitializers[] = {
            1UL, 1UL, 5UL, 5UL, 1UL, 27UL, 33UL, 0UL };
        const unsigned long dim22SLinitializers[] = {
            1UL, 3UL, 3UL, 3UL, 25UL, 17UL, 115UL, 0UL };
        const unsigned long dim23SLinitializers[] = {
            1UL, 1UL, 3UL, 15UL, 29UL, 15UL, 41UL, 0UL };
        const unsigned long dim24SLinitializers[] = {
            1UL, 3UL, 1UL, 7UL, 3UL, 23UL, 79UL, 0UL };
        const unsigned long dim25SLinitializers[] = {
            1UL, 3UL, 7UL, 9UL, 31UL, 29UL, 17UL, 0UL };
        const unsigned long dim26SLinitializers[] = {
            1UL, 1UL, 5UL, 13UL, 11UL, 3UL, 29UL, 0UL };
        const unsigned long dim27SLinitializers[] = {
            1UL, 3UL, 1UL, 9UL, 5UL, 21UL, 119UL, 0UL };
        const unsigned long dim28SLinitializers[] = {
            1UL, 1UL, 3UL, 1UL, 23UL, 13UL, 75UL, 0UL };
        const unsigned long dim29SLinitializers[] = {
            1UL, 3UL, 3UL, 11UL, 27UL, 31UL, 73UL, 0UL };
        const unsigned long dim30SLinitializers[] = {
            1UL, 1UL, 7UL, 7UL, 19UL, 25UL, 105UL, 0UL };
        const unsigned long dim31SLinitializers[] = {
            1UL, 3UL, 5UL, 5UL, 21UL, 9UL, 7UL, 0UL };
        const unsigned long dim32SLinitializers[] = {
            1UL, 1UL, 1UL, 15UL, 5UL, 49UL, 59UL, 0UL };
        const unsigned long dim33SLinitializers[] = {
            1UL, 1UL, 1UL, 1UL, 1UL, 33UL, 65UL, 0UL };
        const unsigned long dim34SLinitializers[] = {
            1UL, 3UL, 5UL, 15UL, 17UL, 19UL, 21UL, 0UL };
        const unsigned long dim35SLinitializers[] = {
            1UL, 1UL, 7UL, 11UL, 13UL, 29UL, 3UL, 0UL };
        const unsigned long dim36SLinitializers[] = {
            1UL, 3UL, 7UL, 5UL, 7UL, 11UL, 113UL, 0UL };
        const unsigned long dim37SLinitializers[] = {
            1UL, 1UL, 5UL, 3UL, 15UL, 19UL, 61UL, 0UL };
        const unsigned long dim38SLinitializers[] = {
            1UL, 3UL, 1UL, 1UL, 9UL, 27UL, 89UL, 7UL, 0UL };
        const unsigned long dim39SLinitializers[] = {
            1UL, 1UL, 3UL, 7UL, 31UL, 15UL, 45UL, 23UL, 0UL };
        const unsigned long dim40SLinitializers[] = {
            1UL, 3UL, 3UL, 9UL, 9UL, 25UL, 107UL, 39UL, 0UL };

        const unsigned long * const SLinitializers[39] = {
            dim02SLinitializers,
            dim03SLinitializers,
            dim04SLinitializers,
            dim05SLinitializers,
            dim06SLinitializers,
            dim07SLinitializers,
            dim08SLinitializers,
            dim09SLinitializers,
            dim10SLinitializers,
            dim11SLinitializers,
            dim12SLinitializers,
            dim13SLinitializers,
            dim14SLinitializers,
            dim15SLinitializers,
            dim16SLinitializers,
            dim17SLinitializers,
            dim18SLinitializers,
            dim19SLinitializers,
            dim20SLinitializers,
            dim21SLinitializers,
            dim22SLinitializers,
            dim23SLinitializers,
            dim24SLinitializers,
            dim25SLinitializers,
            dim26SLinitializers,
            dim27SLinitializers,
            dim28SLinitializers,
            dim29SLinitializers,
            dim30SLinitializers,
            dim31SLinitializers,
            dim32SLinitializers,
            dim33SLinitializers,
            dim34SLinitializers,
            dim35SLinitializers,
            dim36SLinitializers,
            dim37SLinitializers,
            dim38SLinitializers,
            dim39SLinitializers,
            dim40SLinitializers
        };

        /* coefficients of the free direction integers as given in
           "Monte Carlo Methods in Finance", by Peter J�ckel, section 8.3
        */
        const unsigned long dim09initializers[] = {
            1UL, 3UL, 7UL, 7UL, 21UL, 0UL };
        const unsigned long dim10initializers[] = {
            1UL, 1UL, 5UL, 11UL, 27UL, 0UL };
        const unsigned long dim11initializers[] = {
            1UL, 1UL, 7UL, 3UL, 29UL, 0UL };
        const unsigned long dim12initializers[] = {
            1UL, 3UL, 7UL, 13UL, 3UL, 0UL };
        const unsigned long dim13initializers[] = {
            1UL, 3UL, 5UL, 1UL, 15UL, 0UL };
        const unsigned long dim14initializers[] = {
            1UL, 1UL, 1UL, 9UL, 23UL, 37UL, 0UL };
        const unsigned long dim15initializers[] = {
            1UL, 1UL, 3UL, 13UL, 11UL, 7UL, 0UL };
        const unsigned long dim16initializers[] = {
            1UL, 3UL, 3UL, 5UL, 19UL, 33UL, 0UL };
        const unsigned long dim17initializers[] = {
            1UL, 1UL, 7UL, 13UL, 25UL, 5UL, 0UL };
        const unsigned long dim18initializers[] = {
            1UL, 1UL, 1UL, 3UL, 13UL, 39UL, 0UL };
        const unsigned long dim19initializers[] = {
            1UL, 3UL, 5UL, 11UL, 7UL, 11UL, 0UL };
        const unsigned long dim20initializers[] = {
            1UL, 3UL, 1UL, 7UL, 3UL, 23UL, 79UL, 0UL };
        const unsigned long dim21initializers[] = {
            1UL, 3UL, 1UL, 15UL, 17UL, 63UL, 13UL, 0UL };
        const unsigned long dim22initializers[] = {
            1UL, 3UL, 3UL, 3UL, 25UL, 17UL, 115UL, 0UL };
        const unsigned long dim23initializers[] = {
            1UL, 3UL, 7UL, 9UL, 31UL, 29UL, 17UL, 0UL };
        const unsigned long dim24initializers[] = {
            1UL, 1UL, 3UL, 15UL, 29UL, 15UL, 41UL, 0UL };
        const unsigned long dim25initializers[] = {
            1UL, 3UL, 1UL, 9UL, 5UL, 21UL, 119UL, 0UL };
        const unsigned long dim26initializers[] = {
            1UL, 1UL, 5UL, 5UL, 1UL, 27UL, 33UL, 0UL };
        const unsigned long dim27initializers[] = {
            1UL, 1UL, 3UL, 1UL, 23UL, 13UL, 75UL, 0UL };
        const unsigned long dim28initializers[] = {
            1UL, 1UL, 7UL, 7UL, 19UL, 25UL, 105UL, 0UL };
        const unsigned long dim29initializers[] = {
            1UL, 3UL, 5UL, 5UL, 21UL, 9UL, 7UL, 0UL };
        const unsigned long dim30initializers[] = {
            1UL, 1UL, 1UL, 15UL, 5UL, 49UL, 59UL, 0UL };
        const unsigned long dim31initializers[] = {
            1UL, 3UL, 5UL, 15UL, 17UL, 19UL, 21UL, 0UL };
        const unsigned long dim32initializers[] = {
            1UL, 1UL, 7UL, 11UL, 13UL, 29UL, 3UL, 0UL };

        const unsigned long * const initializers[31] = {
            dim02SLinitializers,
            dim03SLinitializers,
            dim04SLinitializers,
            dim05SLinitializers,
            dim06SLinitializers,
            dim07SLinitializers,
            dim08SLinitializers,
            dim09initializers,
            dim10initializers,
            dim11initializers,
            dim12initializers,
            dim13initializers,
            dim14initializers,
            dim15initializers,
            dim16initializers,
            dim17initializers,
            dim18initializers,
            dim19initializers,
            dim20initializers,
            dim21initializers,
            dim22initializers,
            dim23initializers,
            dim24initializers,
            dim25initializers,
            dim26initializers,
            dim27initializers,
            dim28initializers,
            dim29initializers,
            dim30initializers,
            dim31initializers,
            dim32initializers
        };

        /* Lemieux coefficients of the free direction integers as given
           in Christiane Lemieux, private communication, September 2004
        */
        const unsigned long dim041Linitializers[] = {
            1UL,1UL,3UL,13UL,7UL,35UL,61UL,91UL,0UL};
        const unsigned long dim042Linitializers[] = {
            1UL,1UL,7UL,11UL,5UL,35UL,55UL,75UL,0UL};
        const unsigned long dim043Linitializers[] = {
            1UL,3UL,5UL,5UL,11UL,23UL,29UL,139UL,0UL};
        const unsigned long dim044Linitializers[] = {
            1UL,1UL,1UL,7UL,11UL,15UL,17UL,81UL,0UL};
        const unsigned long dim045Linitializers[] = {
            1UL,1UL,7UL,9UL,5UL,57UL,79UL,103UL,0UL};
        const unsigned long dim046Linitializers[] = {
            1UL,1UL,7UL,13UL,19UL,5UL,5UL,185UL,0UL};
        const unsigned long dim047Linitializers[] = {
            1UL,3UL,1UL,3UL,13UL,57UL,97UL,131UL,0UL};
        const unsigned long dim048Linitializers[] = {
            1UL,1UL,5UL,5UL,21UL,25UL,125UL,197UL,0UL};
        const unsigned long dim049Linitializers[] = {
            1UL,3UL,3UL,9UL,31UL,11UL,103UL,201UL,0UL};
        const unsigned long dim050Linitializers[] = {
            1UL,1UL,5UL,3UL,7UL,25UL,51UL,121UL,0UL};
        const unsigned long dim051Linitializers[] = {
            1UL,3UL,7UL,15UL,19UL,53UL,73UL,189UL,0UL};
        const unsigned long dim052Linitializers[] = {
            1UL,1UL,1UL,15UL,19UL,55UL,27UL,183UL,0UL};
        const unsigned long dim053Linitializers[] = {
            1UL,1UL,7UL,13UL,3UL,29UL,109UL,69UL,0UL};
        const unsigned long dim054Linitializers[] = {
            1UL,1UL,5UL,15UL,15UL,23UL,15UL,1UL,57UL,0UL};
        const unsigned long dim055Linitializers[] = {
            1UL,3UL,1UL,3UL,23UL,55UL,43UL,143UL,397UL,0UL};
        const unsigned long dim056Linitializers[] = {
            1UL,1UL,3UL,11UL,29UL,9UL,35UL,131UL,411UL,0UL};
        const unsigned long dim057Linitializers[] = {
            1UL,3UL,1UL,7UL,27UL,39UL,103UL,199UL,277UL,0UL};
        const unsigned long dim058Linitializers[] = {
            1UL,3UL,7UL,3UL,19UL,55UL,127UL,67UL,449UL,0UL};
        const unsigned long dim059Linitializers[] = {
            1UL,3UL,7UL,3UL,5UL,29UL,45UL,85UL,3UL,0UL};
        const unsigned long dim060Linitializers[] = {
            1UL,3UL,5UL,5UL,13UL,23UL,75UL,245UL,453UL,0UL};
        const unsigned long dim061Linitializers[] = {
            1UL,3UL,1UL,15UL,21UL,47UL,3UL,77UL,165UL,0UL};
        const unsigned long dim062Linitializers[] = {
            1UL,1UL,7UL,9UL,15UL,5UL,117UL,73UL,473UL,0UL};
        const unsigned long dim063Linitializers[] = {
            1UL,3UL,1UL,9UL,1UL,21UL,13UL,173UL,313UL,0UL};
        const unsigned long dim064Linitializers[] = {
            1UL,1UL,7UL,3UL,11UL,45UL,63UL,77UL,49UL,0UL};
        const unsigned long dim065Linitializers[] = {
            1UL,1UL,1UL,1UL,1UL,25UL,123UL,39UL,259UL,0UL};
        const unsigned long dim066Linitializers[] = {
            1UL,1UL,1UL,5UL,23UL,11UL,59UL,11UL,203UL,0UL};
        const unsigned long dim067Linitializers[] = {
            1UL,3UL,3UL,15UL,21UL,1UL,73UL,71UL,421UL,0UL};
        const unsigned long dim068Linitializers[] = {
            1UL,1UL,5UL,11UL,15UL,31UL,115UL,95UL,217UL,0UL};
        const unsigned long dim069Linitializers[] = {
            1UL,1UL,3UL,3UL,7UL,53UL,37UL,43UL,439UL,0UL};
        const unsigned long dim070Linitializers[] = {
            1UL,1UL,1UL,1UL,27UL,53UL,69UL,159UL,321UL,0UL};
        const unsigned long dim071Linitializers[] = {
            1UL,1UL,5UL,15UL,29UL,17UL,19UL,43UL,449UL,0UL};
        const unsigned long dim072Linitializers[] = {
            1UL,1UL,3UL,9UL,1UL,55UL,121UL,205UL,255UL,0UL};
        const unsigned long dim073Linitializers[] = {
            1UL,1UL,3UL,11UL,9UL,47UL,107UL,11UL,417UL,0UL};
        const unsigned long dim074Linitializers[] = {
            1UL,1UL,1UL,5UL,17UL,25UL,21UL,83UL,95UL,0UL};
        const unsigned long dim075Linitializers[] = {
            1UL,3UL,5UL,13UL,31UL,25UL,61UL,157UL,407UL,0UL};
        const unsigned long dim076Linitializers[] = {
            1UL,1UL,7UL,9UL,25UL,33UL,41UL,35UL,17UL,0UL};
        const unsigned long dim077Linitializers[] = {
            1UL,3UL,7UL,15UL,13UL,39UL,61UL,187UL,461UL,0UL};
        const unsigned long dim078Linitializers[] = {
            1UL,3UL,7UL,13UL,5UL,57UL,23UL,177UL,435UL,0UL};
        const unsigned long dim079Linitializers[] = {
            1UL,1UL,3UL,15UL,11UL,27UL,115UL,5UL,337UL,0UL};
        const unsigned long dim080Linitializers[] = {
            1UL,3UL,7UL,3UL,15UL,63UL,61UL,171UL,339UL,0UL};
        const unsigned long dim081Linitializers[] = {
            1UL,3UL,3UL,13UL,15UL,61UL,59UL,47UL,1UL,0UL};
        const unsigned long dim082Linitializers[] = {
            1UL,1UL,5UL,15UL,13UL,5UL,39UL,83UL,329UL,0UL};
        const unsigned long dim083Linitializers[] = {
            1UL,1UL,5UL,5UL,5UL,27UL,25UL,39UL,301UL,0UL};
        const unsigned long dim084Linitializers[] = {
            1UL,1UL,5UL,11UL,31UL,41UL,35UL,233UL,27UL,0UL};
        const unsigned long dim085Linitializers[] = {
            1UL,3UL,5UL,15UL,7UL,37UL,119UL,171UL,419UL,0UL};
        const unsigned long dim086Linitializers[] = {
            1UL,3UL,5UL,5UL,3UL,29UL,21UL,189UL,417UL,0UL};
        const unsigned long dim087Linitializers[] = {
            1UL,1UL,1UL,1UL,21UL,41UL,117UL,119UL,351UL,0UL};
        const unsigned long dim088Linitializers[] = {
            1UL,1UL,3UL,1UL,7UL,27UL,87UL,19UL,213UL,0UL};
        const unsigned long dim089Linitializers[] = {
            1UL,1UL,1UL,1UL,17UL,7UL,97UL,217UL,477UL,0UL};
        const unsigned long dim090Linitializers[] = {
            1UL,1UL,7UL,1UL,29UL,61UL,103UL,231UL,269UL,0UL};
        const unsigned long dim091Linitializers[] = {
            1UL,1UL,7UL,13UL,9UL,27UL,107UL,207UL,311UL,0UL};
        const unsigned long dim092Linitializers[] = {
            1UL,1UL,7UL,5UL,25UL,21UL,107UL,179UL,423UL,0UL};
        const unsigned long dim093Linitializers[] = {
            1UL,3UL,5UL,11UL,7UL,1UL,17UL,245UL,281UL,0UL};
        const unsigned long dim094Linitializers[] = {
            1UL,3UL,5UL,9UL,1UL,5UL,53UL,59UL,125UL,0UL};
        const unsigned long dim095Linitializers[] = {
            1UL,1UL,7UL,1UL,31UL,57UL,71UL,245UL,125UL,0UL};
        const unsigned long dim096Linitializers[] = {
            1UL,1UL,7UL,5UL,5UL,57UL,53UL,253UL,441UL,0UL};
        const unsigned long dim097Linitializers[] = {
            1UL,3UL,1UL,13UL,19UL,35UL,119UL,235UL,381UL,0UL};
        const unsigned long dim098Linitializers[] = {
            1UL,3UL,1UL,7UL,19UL,59UL,115UL,33UL,361UL,0UL};
        const unsigned long dim099Linitializers[] = {
            1UL,1UL,3UL,5UL,13UL,1UL,49UL,143UL,501UL,0UL};
        const unsigned long dim100Linitializers[] = {
            1UL,1UL,3UL,5UL,1UL,63UL,101UL,85UL,189UL,0UL};
        const unsigned long dim101Linitializers[] = {
            1UL,1UL,5UL,11UL,27UL,63UL,13UL,131UL,5UL,0UL};
        const unsigned long dim102Linitializers[] = {
            1UL,1UL,5UL,7UL,15UL,45UL,75UL,59UL,455UL,585UL,0UL};
        const unsigned long dim103Linitializers[] = {
            1UL,3UL,1UL,3UL,7UL,7UL,111UL,23UL,119UL,959UL,0UL};
        const unsigned long dim104Linitializers[] = {
            1UL,3UL,3UL,9UL,11UL,41UL,109UL,163UL,161UL,879UL,0UL};
        const unsigned long dim105Linitializers[] = {
            1UL,3UL,5UL,1UL,21UL,41UL,121UL,183UL,315UL,219UL,0UL};
        const unsigned long dim106Linitializers[] = {
            1UL,1UL,3UL,9UL,15UL,3UL,9UL,223UL,441UL,929UL,0UL};
        const unsigned long dim107Linitializers[] = {
            1UL,1UL,7UL,9UL,3UL,5UL,93UL,57UL,253UL,457UL,0UL};
        const unsigned long dim108Linitializers[] = {
            1UL,1UL,7UL,13UL,15UL,29UL,83UL,21UL,35UL,45UL,0UL};
        const unsigned long dim109Linitializers[] = {
            1UL,1UL,3UL,7UL,13UL,61UL,119UL,219UL,85UL,505UL,0UL};
        const unsigned long dim110Linitializers[] = {
            1UL,1UL,3UL,3UL,17UL,13UL,35UL,197UL,291UL,109UL,0UL};
        const unsigned long dim111Linitializers[] = {
            1UL,1UL,3UL,3UL,5UL,1UL,113UL,103UL,217UL,253UL,0UL};
        const unsigned long dim112Linitializers[] = {
            1UL,1UL,7UL,1UL,15UL,39UL,63UL,223UL,17UL,9UL,0UL};
        const unsigned long dim113Linitializers[] = {
            1UL,3UL,7UL,1UL,17UL,29UL,67UL,103UL,495UL,383UL,0UL};
        const unsigned long dim114Linitializers[] = {
            1UL,3UL,3UL,15UL,31UL,59UL,75UL,165UL,51UL,913UL,0UL};
        const unsigned long dim115Linitializers[] = {
            1UL,3UL,7UL,9UL,5UL,27UL,79UL,219UL,233UL,37UL,0UL};
        const unsigned long dim116Linitializers[] = {
            1UL,3UL,5UL,15UL,1UL,11UL,15UL,211UL,417UL,811UL,0UL};
        const unsigned long dim117Linitializers[] = {
            1UL,3UL,5UL,3UL,29UL,27UL,39UL,137UL,407UL,231UL,0UL};
        const unsigned long dim118Linitializers[] = {
            1UL,1UL,3UL,5UL,29UL,43UL,125UL,135UL,109UL,67UL,0UL};
        const unsigned long dim119Linitializers[] = {
            1UL,1UL,1UL,5UL,11UL,39UL,107UL,159UL,323UL,381UL,0UL};
        const unsigned long dim120Linitializers[] = {
            1UL,1UL,1UL,1UL,9UL,11UL,33UL,55UL,169UL,253UL,0UL};
        const unsigned long dim121Linitializers[] = {
            1UL,3UL,5UL,5UL,11UL,53UL,63UL,101UL,251UL,897UL,0UL};
        const unsigned long dim122Linitializers[] = {
            1UL,3UL,7UL,1UL,25UL,15UL,83UL,119UL,53UL,157UL,0UL};
        const unsigned long dim123Linitializers[] = {
            1UL,3UL,5UL,13UL,5UL,5UL,3UL,195UL,111UL,451UL,0UL};
        const unsigned long dim124Linitializers[] = {
            1UL,3UL,1UL,15UL,11UL,1UL,19UL,11UL,307UL,777UL,0UL};
        const unsigned long dim125Linitializers[] = {
            1UL,3UL,7UL,11UL,5UL,5UL,17UL,231UL,345UL,981UL,0UL};
        const unsigned long dim126Linitializers[] = {
            1UL,1UL,3UL,3UL,1UL,33UL,83UL,201UL,57UL,475UL,0UL};
        const unsigned long dim127Linitializers[] = {
            1UL,3UL,7UL,7UL,17UL,13UL,35UL,175UL,499UL,809UL,0UL};
        const unsigned long dim128Linitializers[] = {
            1UL,1UL,5UL,3UL,3UL,17UL,103UL,119UL,499UL,865UL,0UL};
        const unsigned long dim129Linitializers[] = {
            1UL,1UL,1UL,11UL,27UL,25UL,37UL,121UL,401UL,11UL,0UL};
        const unsigned long dim130Linitializers[] = {
            1UL,1UL,1UL,11UL,9UL,25UL,25UL,241UL,403UL,3UL,0UL};
        const unsigned long dim131Linitializers[] = {
            1UL,1UL,1UL,1UL,11UL,1UL,39UL,163UL,231UL,573UL,0UL};
        const unsigned long dim132Linitializers[] = {
            1UL,1UL,1UL,13UL,13UL,21UL,75UL,185UL,99UL,545UL,0UL};
        const unsigned long dim133Linitializers[] = {
            1UL,1UL,1UL,15UL,3UL,63UL,69UL,11UL,173UL,315UL,0UL};
        const unsigned long dim134Linitializers[] = {
            1UL,3UL,5UL,15UL,11UL,3UL,95UL,49UL,123UL,765UL,0UL};
        const unsigned long dim135Linitializers[] = {
            1UL,1UL,1UL,15UL,3UL,63UL,77UL,31UL,425UL,711UL,0UL};
        const unsigned long dim136Linitializers[] = {
            1UL,1UL,7UL,15UL,1UL,37UL,119UL,145UL,489UL,583UL,0UL};
        const unsigned long dim137Linitializers[] = {
            1UL,3UL,5UL,15UL,3UL,49UL,117UL,211UL,165UL,323UL,0UL};
        const unsigned long dim138Linitializers[] = {
            1UL,3UL,7UL,1UL,27UL,63UL,77UL,201UL,225UL,803UL,0UL};
        const unsigned long dim139Linitializers[] = {
            1UL,1UL,1UL,11UL,23UL,35UL,67UL,21UL,469UL,357UL,0UL};
        const unsigned long dim140Linitializers[] = {
            1UL,1UL,7UL,7UL,9UL,7UL,25UL,237UL,237UL,571UL,0UL};
        const unsigned long dim141Linitializers[] = {
            1UL,1UL,3UL,15UL,29UL,5UL,107UL,109UL,241UL,47UL,0UL};
        const unsigned long dim142Linitializers[] = {
            1UL,3UL,5UL,11UL,27UL,63UL,29UL,13UL,203UL,675UL,0UL};
        const unsigned long dim143Linitializers[] = {
            1UL,1UL,3UL,9UL,9UL,11UL,103UL,179UL,449UL,263UL,0UL};
        const unsigned long dim144Linitializers[] = {
            1UL,3UL,5UL,11UL,29UL,63UL,53UL,151UL,259UL,223UL,0UL};
        const unsigned long dim145Linitializers[] = {
            1UL,1UL,3UL,7UL,9UL,25UL,5UL,197UL,237UL,163UL,0UL};
        const unsigned long dim146Linitializers[] = {
            1UL,3UL,7UL,13UL,5UL,57UL,67UL,193UL,147UL,241UL,0UL};
        const unsigned long dim147Linitializers[] = {
            1UL,1UL,5UL,15UL,15UL,33UL,17UL,67UL,161UL,341UL,0UL};
        const unsigned long dim148Linitializers[] = {
            1UL,1UL,3UL,13UL,17UL,43UL,21UL,197UL,441UL,985UL,0UL};
        const unsigned long dim149Linitializers[] = {
            1UL,3UL,1UL,5UL,15UL,33UL,33UL,193UL,305UL,829UL,0UL};
        const unsigned long dim150Linitializers[] = {
            1UL,1UL,1UL,13UL,19UL,27UL,71UL,187UL,477UL,239UL,0UL};
        const unsigned long dim151Linitializers[] = {
            1UL,1UL,1UL,9UL,9UL,17UL,41UL,177UL,229UL,983UL,0UL};
        const unsigned long dim152Linitializers[] = {
            1UL,3UL,5UL,9UL,15UL,45UL,97UL,205UL,43UL,767UL,0UL};
        const unsigned long dim153Linitializers[] = {
            1UL,1UL,1UL,9UL,31UL,31UL,77UL,159UL,395UL,809UL,0UL};
        const unsigned long dim154Linitializers[] = {
            1UL,3UL,3UL,3UL,29UL,19UL,73UL,123UL,165UL,307UL,0UL};
        const unsigned long dim155Linitializers[] = {
            1UL,3UL,1UL,7UL,5UL,11UL,77UL,227UL,355UL,403UL,0UL};
        const unsigned long dim156Linitializers[] = {
            1UL,3UL,5UL,5UL,25UL,31UL,1UL,215UL,451UL,195UL,0UL};
        const unsigned long dim157Linitializers[] = {
            1UL,3UL,7UL,15UL,29UL,37UL,101UL,241UL,17UL,633UL,0UL};
        const unsigned long dim158Linitializers[] = {
            1UL,1UL,5UL,1UL,11UL,3UL,107UL,137UL,489UL,5UL,0UL};
        const unsigned long dim159Linitializers[] = {
            1UL,1UL,1UL,7UL,19UL,19UL,75UL,85UL,471UL,355UL,0UL};
        const unsigned long dim160Linitializers[] = {
            1UL,1UL,3UL,3UL,9UL,13UL,113UL,167UL,13UL,27UL,0UL};
        const unsigned long dim161Linitializers[] = {
            1UL,3UL,5UL,11UL,21UL,3UL,89UL,205UL,377UL,307UL,0UL};
        const unsigned long dim162Linitializers[] = {
            1UL,1UL,1UL,9UL,31UL,61UL,65UL,9UL,391UL,141UL,867UL,0UL};
        const unsigned long dim163Linitializers[] = {
            1UL,1UL,1UL,9UL,19UL,19UL,61UL,227UL,241UL,55UL,161UL,0UL};
        const unsigned long dim164Linitializers[] = {
            1UL,1UL,1UL,11UL,1UL,19UL,7UL,233UL,463UL,171UL,1941UL,0UL};
        const unsigned long dim165Linitializers[] = {
            1UL,1UL,5UL,7UL,25UL,13UL,103UL,75UL,19UL,1021UL,1063UL,0UL};
        const unsigned long dim166Linitializers[] = {
            1UL,1UL,1UL,15UL,17UL,17UL,79UL,63UL,391UL,403UL,1221UL,0UL};
        const unsigned long dim167Linitializers[] = {
            1UL,3UL,3UL,11UL,29UL,25UL,29UL,107UL,335UL,475UL,963UL,0UL};
        const unsigned long dim168Linitializers[] = {
            1UL,3UL,5UL,1UL,31UL,33UL,49UL,43UL,155UL,9UL,1285UL,0UL};
        const unsigned long dim169Linitializers[] = {
            1UL,1UL,5UL,5UL,15UL,47UL,39UL,161UL,357UL,863UL,1039UL,0UL};
        const unsigned long dim170Linitializers[] = {
            1UL,3UL,7UL,15UL,1UL,39UL,47UL,109UL,427UL,393UL,1103UL,0UL};
        const unsigned long dim171Linitializers[] = {
            1UL,1UL,1UL,9UL,9UL,29UL,121UL,233UL,157UL,99UL,701UL,0UL};
        const unsigned long dim172Linitializers[] = {
            1UL,1UL,1UL,7UL,1UL,29UL,75UL,121UL,439UL,109UL,993UL,0UL};
        const unsigned long dim173Linitializers[] = {
            1UL,1UL,1UL,9UL,5UL,1UL,39UL,59UL,89UL,157UL,1865UL,0UL};
        const unsigned long dim174Linitializers[] = {
            1UL,1UL,5UL,1UL,3UL,37UL,89UL,93UL,143UL,533UL,175UL,0UL};
        const unsigned long dim175Linitializers[] = {
            1UL,1UL,3UL,5UL,7UL,33UL,35UL,173UL,159UL,135UL,241UL,0UL};
        const unsigned long dim176Linitializers[] = {
            1UL,1UL,1UL,15UL,17UL,37UL,79UL,131UL,43UL,891UL,229UL,0UL};
        const unsigned long dim177Linitializers[] = {
            1UL,1UL,1UL,1UL,1UL,35UL,121UL,177UL,397UL,1017UL,583UL,0UL};
        const unsigned long dim178Linitializers[] = {
            1UL,1UL,3UL,15UL,31UL,21UL,43UL,67UL,467UL,923UL,1473UL,0UL};
        const unsigned long dim179Linitializers[] = {
            1UL,1UL,1UL,7UL,1UL,33UL,77UL,111UL,125UL,771UL,1975UL,0UL};
        const unsigned long dim180Linitializers[] = {
            1UL,3UL,7UL,13UL,1UL,51UL,113UL,139UL,245UL,573UL,503UL,0UL};
        const unsigned long dim181Linitializers[] = {
            1UL,3UL,1UL,9UL,21UL,49UL,15UL,157UL,49UL,483UL,291UL,0UL};
        const unsigned long dim182Linitializers[] = {
            1UL,1UL,1UL,1UL,29UL,35UL,17UL,65UL,403UL,485UL,1603UL,0UL};
        const unsigned long dim183Linitializers[] = {
            1UL,1UL,1UL,7UL,19UL,1UL,37UL,129UL,203UL,321UL,1809UL,0UL};
        const unsigned long dim184Linitializers[] = {
            1UL,3UL,7UL,15UL,15UL,9UL,5UL,77UL,29UL,485UL,581UL,0UL};
        const unsigned long dim185Linitializers[] = {
            1UL,1UL,3UL,5UL,15UL,49UL,97UL,105UL,309UL,875UL,1581UL,0UL};
        const unsigned long dim186Linitializers[] = {
            1UL,3UL,5UL,1UL,5UL,19UL,63UL,35UL,165UL,399UL,1489UL,0UL};
        const unsigned long dim187Linitializers[] = {
            1UL,3UL,5UL,3UL,23UL,5UL,79UL,137UL,115UL,599UL,1127UL,0UL};
        const unsigned long dim188Linitializers[] = {
            1UL,1UL,7UL,5UL,3UL,61UL,27UL,177UL,257UL,91UL,841UL,0UL};
        const unsigned long dim189Linitializers[] = {
            1UL,1UL,3UL,5UL,9UL,31UL,91UL,209UL,409UL,661UL,159UL,0UL};
        const unsigned long dim190Linitializers[] = {
            1UL,3UL,1UL,15UL,23UL,39UL,23UL,195UL,245UL,203UL,947UL,0UL};
        const unsigned long dim191Linitializers[] = {
            1UL,1UL,3UL,1UL,15UL,59UL,67UL,95UL,155UL,461UL,147UL,0UL};
        const unsigned long dim192Linitializers[] = {
            1UL,3UL,7UL,5UL,23UL,25UL,87UL,11UL,51UL,449UL,1631UL,0UL};
        const unsigned long dim193Linitializers[] = {
            1UL,1UL,1UL,1UL,17UL,57UL,7UL,197UL,409UL,609UL,135UL,0UL};
        const unsigned long dim194Linitializers[] = {
            1UL,1UL,1UL,9UL,1UL,61UL,115UL,113UL,495UL,895UL,1595UL,0UL};
        const unsigned long dim195Linitializers[] = {
            1UL,3UL,7UL,15UL,9UL,47UL,121UL,211UL,379UL,985UL,1755UL,0UL};
        const unsigned long dim196Linitializers[] = {
            1UL,3UL,1UL,3UL,7UL,57UL,27UL,231UL,339UL,325UL,1023UL,0UL};
        const unsigned long dim197Linitializers[] = {
            1UL,1UL,1UL,1UL,19UL,63UL,63UL,239UL,31UL,643UL,373UL,0UL};
        const unsigned long dim198Linitializers[] = {
            1UL,3UL,1UL,11UL,19UL,9UL,7UL,171UL,21UL,691UL,215UL,0UL};
        const unsigned long dim199Linitializers[] = {
            1UL,1UL,5UL,13UL,11UL,57UL,39UL,211UL,241UL,893UL,555UL,0UL};
        const unsigned long dim200Linitializers[] = {
            1UL,1UL,7UL,5UL,29UL,21UL,45UL,59UL,509UL,223UL,491UL,0UL};
        const unsigned long dim201Linitializers[] = {
            1UL,1UL,7UL,9UL,15UL,61UL,97UL,75UL,127UL,779UL,839UL,0UL};
        const unsigned long dim202Linitializers[] = {
            1UL,1UL,7UL,15UL,17UL,33UL,75UL,237UL,191UL,925UL,681UL,0UL};
        const unsigned long dim203Linitializers[] = {
            1UL,3UL,5UL,7UL,27UL,57UL,123UL,111UL,101UL,371UL,1129UL,0UL};
        const unsigned long dim204Linitializers[] = {
            1UL,3UL,5UL,5UL,29UL,45UL,59UL,127UL,229UL,967UL,2027UL,0UL};
        const unsigned long dim205Linitializers[] = {
            1UL,1UL,1UL,1UL,17UL,7UL,23UL,199UL,241UL,455UL,135UL,0UL};
        const unsigned long dim206Linitializers[] = {
            1UL,1UL,7UL,15UL,27UL,29UL,105UL,171UL,337UL,503UL,1817UL,0UL};
        const unsigned long dim207Linitializers[] = {
            1UL,1UL,3UL,7UL,21UL,35UL,61UL,71UL,405UL,647UL,2045UL,0UL};
        const unsigned long dim208Linitializers[] = {
            1UL,1UL,1UL,1UL,1UL,15UL,65UL,167UL,501UL,79UL,737UL,0UL};
        const unsigned long dim209Linitializers[] = {
            1UL,1UL,5UL,1UL,3UL,49UL,27UL,189UL,341UL,615UL,1287UL,0UL};
        const unsigned long dim210Linitializers[] = {
            1UL,1UL,1UL,9UL,1UL,7UL,31UL,159UL,503UL,327UL,1613UL,0UL};
        const unsigned long dim211Linitializers[] = {
            1UL,3UL,3UL,3UL,3UL,23UL,99UL,115UL,323UL,997UL,987UL,0UL};
        const unsigned long dim212Linitializers[] = {
            1UL,1UL,1UL,9UL,19UL,33UL,93UL,247UL,509UL,453UL,891UL,0UL};
        const unsigned long dim213Linitializers[] = {
            1UL,1UL,3UL,1UL,13UL,19UL,35UL,153UL,161UL,633UL,445UL,0UL};
        const unsigned long dim214Linitializers[] = {
            1UL,3UL,5UL,15UL,31UL,5UL,87UL,197UL,183UL,783UL,1823UL,0UL};
        const unsigned long dim215Linitializers[] = {
            1UL,1UL,7UL,5UL,19UL,63UL,69UL,221UL,129UL,231UL,1195UL,0UL};
        const unsigned long dim216Linitializers[] = {
            1UL,1UL,5UL,5UL,13UL,23UL,19UL,231UL,245UL,917UL,379UL,0UL};
        const unsigned long dim217Linitializers[] = {
            1UL,3UL,1UL,15UL,19UL,43UL,27UL,223UL,171UL,413UL,125UL,0UL};
        const unsigned long dim218Linitializers[] = {
            1UL,1UL,1UL,9UL,1UL,59UL,21UL,15UL,509UL,207UL,589UL,0UL};
        const unsigned long dim219Linitializers[] = {
            1UL,3UL,5UL,3UL,19UL,31UL,113UL,19UL,23UL,733UL,499UL,0UL};
        const unsigned long dim220Linitializers[] = {
            1UL,1UL,7UL,1UL,19UL,51UL,101UL,165UL,47UL,925UL,1093UL,0UL};
        const unsigned long dim221Linitializers[] = {
            1UL,3UL,3UL,9UL,15UL,21UL,43UL,243UL,237UL,461UL,1361UL,0UL};
        const unsigned long dim222Linitializers[] = {
            1UL,1UL,1UL,9UL,17UL,15UL,75UL,75UL,113UL,715UL,1419UL,0UL};
        const unsigned long dim223Linitializers[] = {
            1UL,1UL,7UL,13UL,17UL,1UL,99UL,15UL,347UL,721UL,1405UL,0UL};
        const unsigned long dim224Linitializers[] = {
            1UL,1UL,7UL,15UL,7UL,27UL,23UL,183UL,39UL,59UL,571UL,0UL};
        const unsigned long dim225Linitializers[] = {
            1UL,3UL,5UL,9UL,7UL,43UL,35UL,165UL,463UL,567UL,859UL,0UL};
        const unsigned long dim226Linitializers[] = {
            1UL,3UL,3UL,11UL,15UL,19UL,17UL,129UL,311UL,343UL,15UL,0UL};
        const unsigned long dim227Linitializers[] = {
            1UL,1UL,1UL,15UL,31UL,59UL,63UL,39UL,347UL,359UL,105UL,0UL};
        const unsigned long dim228Linitializers[] = {
            1UL,1UL,1UL,15UL,5UL,43UL,87UL,241UL,109UL,61UL,685UL,0UL};
        const unsigned long dim229Linitializers[] = {
            1UL,1UL,7UL,7UL,9UL,39UL,121UL,127UL,369UL,579UL,853UL,0UL};
        const unsigned long dim230Linitializers[] = {
            1UL,1UL,1UL,1UL,17UL,15UL,15UL,95UL,325UL,627UL,299UL,0UL};
        const unsigned long dim231Linitializers[] = {
            1UL,1UL,3UL,13UL,31UL,53UL,85UL,111UL,289UL,811UL,1635UL,0UL};
        const unsigned long dim232Linitializers[] = {
            1UL,3UL,7UL,1UL,19UL,29UL,75UL,185UL,153UL,573UL,653UL,0UL};
        const unsigned long dim233Linitializers[] = {
            1UL,3UL,7UL,1UL,29UL,31UL,55UL,91UL,249UL,247UL,1015UL,0UL};
        const unsigned long dim234Linitializers[] = {
            1UL,3UL,5UL,7UL,1UL,49UL,113UL,139UL,257UL,127UL,307UL,0UL};
        const unsigned long dim235Linitializers[] = {
            1UL,3UL,5UL,9UL,15UL,15UL,123UL,105UL,105UL,225UL,1893UL,0UL};
        const unsigned long dim236Linitializers[] = {
            1UL,3UL,3UL,1UL,15UL,5UL,105UL,249UL,73UL,709UL,1557UL,0UL};
        const unsigned long dim237Linitializers[] = {
            1UL,1UL,1UL,9UL,17UL,31UL,113UL,73UL,65UL,701UL,1439UL,0UL};
        const unsigned long dim238Linitializers[] = {
            1UL,3UL,5UL,15UL,13UL,21UL,117UL,131UL,243UL,859UL,323UL,0UL};
        const unsigned long dim239Linitializers[] = {
            1UL,1UL,1UL,9UL,19UL,15UL,69UL,149UL,89UL,681UL,515UL,0UL};
        const unsigned long dim240Linitializers[] = {
            1UL,1UL,1UL,5UL,29UL,13UL,21UL,97UL,301UL,27UL,967UL,0UL};
        const unsigned long dim241Linitializers[] = {
            1UL,1UL,3UL,3UL,15UL,45UL,107UL,227UL,495UL,769UL,1935UL,0UL};
        const unsigned long dim242Linitializers[] = {
            1UL,1UL,1UL,11UL,5UL,27UL,41UL,173UL,261UL,703UL,1349UL,0UL};
        const unsigned long dim243Linitializers[] = {
            1UL,3UL,3UL,3UL,11UL,35UL,97UL,43UL,501UL,563UL,1331UL,0UL};
        const unsigned long dim244Linitializers[] = {
            1UL,1UL,1UL,7UL,1UL,17UL,87UL,17UL,429UL,245UL,1941UL,0UL};
        const unsigned long dim245Linitializers[] = {
            1UL,1UL,7UL,15UL,29UL,13UL,1UL,175UL,425UL,233UL,797UL,0UL};
        const unsigned long dim246Linitializers[] = {
            1UL,1UL,3UL,11UL,21UL,57UL,49UL,49UL,163UL,685UL,701UL,0UL};
        const unsigned long dim247Linitializers[] = {
            1UL,3UL,3UL,7UL,11UL,45UL,107UL,111UL,379UL,703UL,1403UL,0UL};
        const unsigned long dim248Linitializers[] = {
            1UL,1UL,7UL,3UL,21UL,7UL,117UL,49UL,469UL,37UL,775UL,0UL};
        const unsigned long dim249Linitializers[] = {
            1UL,1UL,5UL,15UL,31UL,63UL,101UL,77UL,507UL,489UL,1955UL,0UL};
        const unsigned long dim250Linitializers[] = {
            1UL,3UL,3UL,11UL,19UL,21UL,101UL,255UL,203UL,673UL,665UL,0UL};
        const unsigned long dim251Linitializers[] = {
            1UL,3UL,3UL,15UL,17UL,47UL,125UL,187UL,271UL,899UL,2003UL,0UL};
        const unsigned long dim252Linitializers[] = {
            1UL,1UL,7UL,7UL,1UL,35UL,13UL,235UL,5UL,337UL,905UL,0UL};
        const unsigned long dim253Linitializers[] = {
            1UL,3UL,1UL,15UL,1UL,43UL,1UL,27UL,37UL,695UL,1429UL,0UL};
        const unsigned long dim254Linitializers[] = {
            1UL,3UL,1UL,11UL,21UL,27UL,93UL,161UL,299UL,665UL,495UL,0UL};
        const unsigned long dim255Linitializers[] = {
            1UL,3UL,3UL,15UL,3UL,1UL,81UL,111UL,105UL,547UL,897UL,0UL};
        const unsigned long dim256Linitializers[] = {
            1UL,3UL,5UL,1UL,3UL,53UL,97UL,253UL,401UL,827UL,1467UL,0UL};
        const unsigned long dim257Linitializers[] = {
            1UL,1UL,1UL,5UL,19UL,59UL,105UL,125UL,271UL,351UL,719UL,0UL};
        const unsigned long dim258Linitializers[] = {
            1UL,3UL,5UL,13UL,7UL,11UL,91UL,41UL,441UL,759UL,1827UL,0UL};
        const unsigned long dim259Linitializers[] = {
            1UL,3UL,7UL,11UL,29UL,61UL,61UL,23UL,307UL,863UL,363UL,0UL};
        const unsigned long dim260Linitializers[] = {
            1UL,1UL,7UL,1UL,15UL,35UL,29UL,133UL,415UL,473UL,1737UL,0UL};
        const unsigned long dim261Linitializers[] = {
            1UL,1UL,1UL,13UL,7UL,33UL,35UL,225UL,117UL,681UL,1545UL,0UL};
        const unsigned long dim262Linitializers[] = {
            1UL,1UL,1UL,3UL,5UL,41UL,83UL,247UL,13UL,373UL,1091UL,0UL};
        const unsigned long dim263Linitializers[] = {
            1UL,3UL,1UL,13UL,25UL,61UL,71UL,217UL,233UL,313UL,547UL,0UL};
        const unsigned long dim264Linitializers[] = {
            1UL,3UL,1UL,7UL,3UL,29UL,3UL,49UL,93UL,465UL,15UL,0UL};
        const unsigned long dim265Linitializers[] = {
            1UL,1UL,1UL,9UL,17UL,61UL,99UL,163UL,129UL,485UL,1087UL,0UL};
        const unsigned long dim266Linitializers[] = {
            1UL,1UL,1UL,9UL,9UL,33UL,31UL,163UL,145UL,649UL,253UL,0UL};
        const unsigned long dim267Linitializers[] = {
            1UL,1UL,1UL,1UL,17UL,63UL,43UL,235UL,287UL,111UL,567UL,0UL};
        const unsigned long dim268Linitializers[] = {
            1UL,3UL,5UL,13UL,29UL,7UL,11UL,69UL,153UL,127UL,449UL,0UL};
        const unsigned long dim269Linitializers[] = {
            1UL,1UL,5UL,9UL,11UL,21UL,15UL,189UL,431UL,493UL,1219UL,0UL};
        const unsigned long dim270Linitializers[] = {
            1UL,1UL,1UL,15UL,19UL,5UL,47UL,91UL,399UL,293UL,1743UL,0UL};
        const unsigned long dim271Linitializers[] = {
            1UL,3UL,3UL,11UL,29UL,53UL,53UL,225UL,409UL,303UL,333UL,0UL};
        const unsigned long dim272Linitializers[] = {
            1UL,1UL,1UL,15UL,31UL,31UL,21UL,81UL,147UL,287UL,1753UL,0UL};
        const unsigned long dim273Linitializers[] = {
            1UL,3UL,5UL,5UL,5UL,63UL,35UL,125UL,41UL,687UL,1793UL,0UL};
        const unsigned long dim274Linitializers[] = {
            1UL,1UL,1UL,9UL,19UL,59UL,107UL,219UL,455UL,971UL,297UL,0UL};
        const unsigned long dim275Linitializers[] = {
            1UL,1UL,3UL,5UL,3UL,51UL,121UL,31UL,245UL,105UL,1311UL,0UL};
        const unsigned long dim276Linitializers[] = {
            1UL,3UL,1UL,5UL,5UL,57UL,75UL,107UL,161UL,431UL,1693UL,0UL};
        const unsigned long dim277Linitializers[] = {
            1UL,3UL,1UL,3UL,19UL,53UL,27UL,31UL,191UL,565UL,1015UL,0UL};
        const unsigned long dim278Linitializers[] = {
            1UL,3UL,5UL,13UL,9UL,41UL,35UL,249UL,287UL,49UL,123UL,0UL};
        const unsigned long dim279Linitializers[] = {
            1UL,1UL,5UL,7UL,27UL,17UL,21UL,3UL,151UL,885UL,1165UL,0UL};
        const unsigned long dim280Linitializers[] = {
            1UL,1UL,7UL,1UL,15UL,17UL,65UL,139UL,427UL,339UL,1171UL,0UL};
        const unsigned long dim281Linitializers[] = {
            1UL,1UL,1UL,5UL,23UL,5UL,9UL,89UL,321UL,907UL,391UL,0UL};
        const unsigned long dim282Linitializers[] = {
            1UL,1UL,7UL,9UL,15UL,1UL,77UL,71UL,87UL,701UL,917UL,0UL};
        const unsigned long dim283Linitializers[] = {
            1UL,1UL,7UL,1UL,17UL,37UL,115UL,127UL,469UL,779UL,1543UL,0UL};
        const unsigned long dim284Linitializers[] = {
            1UL,3UL,7UL,3UL,5UL,61UL,15UL,37UL,301UL,951UL,1437UL,0UL};
        const unsigned long dim285Linitializers[] = {
            1UL,1UL,1UL,13UL,9UL,51UL,127UL,145UL,229UL,55UL,1567UL,0UL};
        const unsigned long dim286Linitializers[] = {
            1UL,3UL,7UL,15UL,19UL,47UL,53UL,153UL,295UL,47UL,1337UL,0UL};
        const unsigned long dim287Linitializers[] = {
            1UL,3UL,3UL,5UL,11UL,31UL,29UL,133UL,327UL,287UL,507UL,0UL};
        const unsigned long dim288Linitializers[] = {
            1UL,1UL,7UL,7UL,25UL,31UL,37UL,199UL,25UL,927UL,1317UL,0UL};
        const unsigned long dim289Linitializers[] = {
            1UL,1UL,7UL,9UL,3UL,39UL,127UL,167UL,345UL,467UL,759UL,0UL};
        const unsigned long dim290Linitializers[] = {
            1UL,1UL,1UL,1UL,31UL,21UL,15UL,101UL,293UL,787UL,1025UL,0UL};
        const unsigned long dim291Linitializers[] = {
            1UL,1UL,5UL,3UL,11UL,41UL,105UL,109UL,149UL,837UL,1813UL,0UL};
        const unsigned long dim292Linitializers[] = {
            1UL,1UL,3UL,5UL,29UL,13UL,19UL,97UL,309UL,901UL,753UL,0UL};
        const unsigned long dim293Linitializers[] = {
            1UL,1UL,7UL,1UL,19UL,17UL,31UL,39UL,173UL,361UL,1177UL,0UL};
        const unsigned long dim294Linitializers[] = {
            1UL,3UL,3UL,3UL,3UL,41UL,81UL,7UL,341UL,491UL,43UL,0UL};
        const unsigned long dim295Linitializers[] = {
            1UL,1UL,7UL,7UL,31UL,35UL,29UL,77UL,11UL,335UL,1275UL,0UL};
        const unsigned long dim296Linitializers[] = {
            1UL,3UL,3UL,15UL,17UL,45UL,19UL,63UL,151UL,849UL,129UL,0UL};
        const unsigned long dim297Linitializers[] = {
            1UL,1UL,7UL,5UL,7UL,13UL,47UL,73UL,79UL,31UL,499UL,0UL};
        const unsigned long dim298Linitializers[] = {
            1UL,3UL,1UL,11UL,1UL,41UL,59UL,151UL,247UL,115UL,1295UL,0UL};
        const unsigned long dim299Linitializers[] = {
            1UL,1UL,1UL,9UL,31UL,37UL,73UL,23UL,295UL,483UL,179UL,0UL};
        const unsigned long dim300Linitializers[] = {
            1UL,3UL,1UL,15UL,13UL,63UL,81UL,27UL,169UL,825UL,2037UL,0UL};
        const unsigned long dim301Linitializers[] = {
            1UL,3UL,5UL,15UL,7UL,11UL,73UL,1UL,451UL,101UL,2039UL,0UL};
        const unsigned long dim302Linitializers[] = {
            1UL,3UL,5UL,3UL,13UL,53UL,31UL,137UL,173UL,319UL,1521UL,0UL};
        const unsigned long dim303Linitializers[] = {
            1UL,3UL,1UL,3UL,29UL,1UL,73UL,227UL,377UL,337UL,1189UL,0UL};
        const unsigned long dim304Linitializers[] = {
            1UL,3UL,3UL,13UL,27UL,9UL,31UL,101UL,229UL,165UL,1983UL,0UL};
        const unsigned long dim305Linitializers[] = {
            1UL,3UL,1UL,13UL,13UL,19UL,19UL,111UL,319UL,421UL,223UL,0UL};
        const unsigned long dim306Linitializers[] = {
            1UL,1UL,7UL,15UL,25UL,37UL,61UL,55UL,359UL,255UL,1955UL,0UL};
        const unsigned long dim307Linitializers[] = {
            1UL,1UL,5UL,13UL,17UL,43UL,49UL,215UL,383UL,915UL,51UL,0UL};
        const unsigned long dim308Linitializers[] = {
            1UL,1UL,3UL,1UL,3UL,7UL,13UL,119UL,155UL,585UL,967UL,0UL};
        const unsigned long dim309Linitializers[] = {
            1UL,3UL,1UL,13UL,1UL,63UL,125UL,21UL,103UL,287UL,457UL,0UL};
        const unsigned long dim310Linitializers[] = {
            1UL,1UL,7UL,1UL,31UL,17UL,125UL,137UL,345UL,379UL,1925UL,0UL};
        const unsigned long dim311Linitializers[] = {
            1UL,1UL,3UL,5UL,5UL,25UL,119UL,153UL,455UL,271UL,2023UL,0UL};
        const unsigned long dim312Linitializers[] = {
            1UL,1UL,7UL,9UL,9UL,37UL,115UL,47UL,5UL,255UL,917UL,0UL};
        const unsigned long dim313Linitializers[] = {
            1UL,3UL,5UL,3UL,31UL,21UL,75UL,203UL,489UL,593UL,1UL,0UL};
        const unsigned long dim314Linitializers[] = {
            1UL,3UL,7UL,15UL,19UL,63UL,123UL,153UL,135UL,977UL,1875UL,0UL};
        const unsigned long dim315Linitializers[] = {
            1UL,1UL,1UL,1UL,5UL,59UL,31UL,25UL,127UL,209UL,745UL,0UL};
        const unsigned long dim316Linitializers[] = {
            1UL,1UL,1UL,1UL,19UL,45UL,67UL,159UL,301UL,199UL,535UL,0UL};
        const unsigned long dim317Linitializers[] = {
            1UL,1UL,7UL,1UL,31UL,17UL,19UL,225UL,369UL,125UL,421UL,0UL};
        const unsigned long dim318Linitializers[] = {
            1UL,3UL,3UL,11UL,7UL,59UL,115UL,197UL,459UL,469UL,1055UL,0UL};
        const unsigned long dim319Linitializers[] = {
            1UL,3UL,1UL,3UL,27UL,45UL,35UL,131UL,349UL,101UL,411UL,0UL};
        const unsigned long dim320Linitializers[] = {
            1UL,3UL,7UL,11UL,9UL,3UL,67UL,145UL,299UL,253UL,1339UL,0UL};
        const unsigned long dim321Linitializers[] = {
            1UL,3UL,3UL,11UL,9UL,37UL,123UL,229UL,273UL,269UL,515UL,0UL};
        const unsigned long dim322Linitializers[] = {
            1UL,3UL,7UL,15UL,11UL,25UL,75UL,5UL,367UL,217UL,951UL,0UL};
        const unsigned long dim323Linitializers[] = {
            1UL,1UL,3UL,7UL,9UL,23UL,63UL,237UL,385UL,159UL,1273UL,0UL};
        const unsigned long dim324Linitializers[] = {
            1UL,1UL,5UL,11UL,23UL,5UL,55UL,193UL,109UL,865UL,663UL,0UL};
        const unsigned long dim325Linitializers[] = {
            1UL,1UL,7UL,15UL,1UL,57UL,17UL,141UL,51UL,217UL,1259UL,0UL};
        const unsigned long dim326Linitializers[] = {
            1UL,1UL,3UL,3UL,15UL,7UL,89UL,233UL,71UL,329UL,203UL,0UL};
        const unsigned long dim327Linitializers[] = {
            1UL,3UL,7UL,11UL,11UL,1UL,19UL,155UL,89UL,437UL,573UL,0UL};
        const unsigned long dim328Linitializers[] = {
            1UL,3UL,1UL,9UL,27UL,61UL,47UL,109UL,161UL,913UL,1681UL,0UL};
        const unsigned long dim329Linitializers[] = {
            1UL,1UL,7UL,15UL,1UL,33UL,19UL,15UL,23UL,913UL,989UL,0UL};
        const unsigned long dim330Linitializers[] = {
            1UL,3UL,1UL,1UL,25UL,39UL,119UL,193UL,13UL,571UL,157UL,0UL};
        const unsigned long dim331Linitializers[] = {
            1UL,1UL,7UL,13UL,9UL,55UL,59UL,147UL,361UL,935UL,515UL,0UL};
        const unsigned long dim332Linitializers[] = {
            1UL,1UL,1UL,9UL,7UL,59UL,67UL,117UL,71UL,855UL,1493UL,0UL};
        const unsigned long dim333Linitializers[] = {
            1UL,3UL,1UL,3UL,13UL,19UL,57UL,141UL,305UL,275UL,1079UL,0UL};
        const unsigned long dim334Linitializers[] = {
            1UL,1UL,1UL,9UL,17UL,61UL,33UL,7UL,43UL,931UL,781UL,0UL};
        const unsigned long dim335Linitializers[] = {
            1UL,1UL,3UL,1UL,11UL,17UL,21UL,97UL,295UL,277UL,1721UL,0UL};
        const unsigned long dim336Linitializers[] = {
            1UL,3UL,1UL,13UL,15UL,43UL,11UL,241UL,147UL,391UL,1641UL,0UL};
        const unsigned long dim337Linitializers[] = {
            1UL,1UL,1UL,1UL,1UL,19UL,37UL,21UL,255UL,263UL,1571UL,0UL};
        const unsigned long dim338Linitializers[] = {
            1UL,1UL,3UL,3UL,23UL,59UL,89UL,17UL,475UL,303UL,757UL,543UL,0UL};
        const unsigned long dim339Linitializers[] = {
           1UL,3UL,3UL,9UL,11UL,55UL,35UL,159UL,139UL,203UL,1531UL,1825UL,0UL};
        const unsigned long dim340Linitializers[] = {
            1UL,1UL,5UL,3UL,17UL,53UL,51UL,241UL,269UL,949UL,1373UL,325UL,0UL};
        const unsigned long dim341Linitializers[] = {
            1UL,3UL,7UL,7UL,5UL,29UL,91UL,149UL,239UL,193UL,1951UL,2675UL,0UL};
        const unsigned long dim342Linitializers[] = {
            1UL,3UL,5UL,1UL,27UL,33UL,69UL,11UL,51UL,371UL,833UL,2685UL,0UL};
        const unsigned long dim343Linitializers[] = {
            1UL,1UL,1UL,15UL,1UL,17UL,35UL,57UL,171UL,1007UL,449UL,367UL,0UL};
        const unsigned long dim344Linitializers[] = {
            1UL,1UL,1UL,7UL,25UL,61UL,73UL,219UL,379UL,53UL,589UL,4065UL,0UL};
        const unsigned long dim345Linitializers[] = {
            1UL,3UL,5UL,13UL,21UL,29UL,45UL,19UL,163UL,169UL,147UL,597UL,0UL};
        const unsigned long dim346Linitializers[] = {
            1UL,1UL,5UL,11UL,21UL,27UL,7UL,17UL,237UL,591UL,255UL,1235UL,0UL};
        const unsigned long dim347Linitializers[] = {
            1UL,1UL,7UL,7UL,17UL,41UL,69UL,237UL,397UL,173UL,1229UL,2341UL,0UL};
        const unsigned long dim348Linitializers[] = {
            1UL,1UL,3UL,1UL,1UL,33UL,125UL,47UL,11UL,783UL,1323UL,2469UL,0UL};
        const unsigned long dim349Linitializers[] = {
            1UL,3UL,1UL,11UL,3UL,39UL,35UL,133UL,153UL,55UL,1171UL,3165UL,0UL};
        const unsigned long dim350Linitializers[] = {
           1UL,1UL,5UL,11UL,27UL,23UL,103UL,245UL,375UL,753UL,477UL,2165UL,0UL};
        const unsigned long dim351Linitializers[] = {
          1UL,3UL,1UL,15UL,15UL,49UL,127UL,223UL,387UL,771UL,1719UL,1465UL,0UL};
        const unsigned long dim352Linitializers[] = {
            1UL,1UL,1UL,9UL,11UL,9UL,17UL,185UL,239UL,899UL,1273UL,3961UL,0UL};
        const unsigned long dim353Linitializers[] = {
            1UL,1UL,3UL,13UL,11UL,51UL,73UL,81UL,389UL,647UL,1767UL,1215UL,0UL};
        const unsigned long dim354Linitializers[] = {
            1UL,3UL,5UL,15UL,19UL,9UL,69UL,35UL,349UL,977UL,1603UL,1435UL,0UL};
        const unsigned long dim355Linitializers[] = {
            1UL,1UL,1UL,1UL,19UL,59UL,123UL,37UL,41UL,961UL,181UL,1275UL,0UL};
        const unsigned long dim356Linitializers[] = {
            1UL,1UL,1UL,1UL,31UL,29UL,37UL,71UL,205UL,947UL,115UL,3017UL,0UL};
        const unsigned long dim357Linitializers[] = {
            1UL,1UL,7UL,15UL,5UL,37UL,101UL,169UL,221UL,245UL,687UL,195UL,0UL};
        const unsigned long dim358Linitializers[] = {
            1UL,1UL,1UL,1UL,19UL,9UL,125UL,157UL,119UL,283UL,1721UL,743UL,0UL};
        const unsigned long dim359Linitializers[] = {
            1UL,1UL,7UL,3UL,1UL,7UL,61UL,71UL,119UL,257UL,1227UL,2893UL,0UL};
        const unsigned long dim360Linitializers[] = {
            1UL,3UL,3UL,3UL,25UL,41UL,25UL,225UL,31UL,57UL,925UL,2139UL,0UL};


        const unsigned long * const Linitializers[359] = {
            dim02SLinitializers,
            dim03SLinitializers,
            dim04SLinitializers,
            dim05SLinitializers,
            dim06SLinitializers,
            dim07SLinitializers,
            dim08SLinitializers,
            dim09SLinitializers,
            dim10SLinitializers,
            dim11SLinitializers,
            dim12SLinitializers,
            dim13SLinitializers,
            dim14SLinitializers,
            dim15SLinitializers,
            dim16SLinitializers,
            dim17SLinitializers,
            dim18SLinitializers,
            dim19SLinitializers,
            dim20SLinitializers,
            dim21SLinitializers,
            dim22SLinitializers,
            dim23SLinitializers,
            dim24SLinitializers,
            dim25SLinitializers,
            dim26SLinitializers,
            dim27SLinitializers,
            dim28SLinitializers,
            dim29SLinitializers,
            dim30SLinitializers,
            dim31SLinitializers,
            dim32SLinitializers,
            dim33SLinitializers,
            dim34SLinitializers,
            dim35SLinitializers,
            dim36SLinitializers,
            dim37SLinitializers,
            dim38SLinitializers,
            dim39SLinitializers,
            dim40SLinitializers,
            dim041Linitializers,
            dim042Linitializers,
            dim043Linitializers,
            dim044Linitializers,
            dim045Linitializers,
            dim046Linitializers,
            dim047Linitializers,
            dim048Linitializers,
            dim049Linitializers,
            dim050Linitializers,
            dim051Linitializers,
            dim052Linitializers,
            dim053Linitializers,
            dim054Linitializers,
            dim055Linitializers,
            dim056Linitializers,
            dim057Linitializers,
            dim058Linitializers,
            dim059Linitializers,
            dim060Linitializers,
            dim061Linitializers,
            dim062Linitializers,
            dim063Linitializers,
            dim064Linitializers,
            dim065Linitializers,
            dim066Linitializers,
            dim067Linitializers,
            dim068Linitializers,
            dim069Linitializers,
            dim070Linitializers,
            dim071Linitializers,
            dim072Linitializers,
            dim073Linitializers,
            dim074Linitializers,
            dim075Linitializers,
            dim076Linitializers,
            dim077Linitializers,
            dim078Linitializers,
            dim079Linitializers,
            dim080Linitializers,
            dim081Linitializers,
            dim082Linitializers,
            dim083Linitializers,
            dim084Linitializers,
            dim085Linitializers,
            dim086Linitializers,
            dim087Linitializers,
            dim088Linitializers,
            dim089Linitializers,
            dim090Linitializers,
            dim091Linitializers,
            dim092Linitializers,
            dim093Linitializers,
            dim094Linitializers,
            dim095Linitializers,
            dim096Linitializers,
            dim097Linitializers,
            dim098Linitializers,
            dim099Linitializers,
            dim100Linitializers,
            dim101Linitializers,
            dim102Linitializers,
            dim103Linitializers,
            dim104Linitializers,
            dim105Linitializers,
            dim106Linitializers,
            dim107Linitializers,
            dim108Linitializers,
            dim109Linitializers,
            dim110Linitializers,
            dim111Linitializers,
            dim112Linitializers,
            dim113Linitializers,
            dim114Linitializers,
            dim115Linitializers,
            dim116Linitializers,
            dim117Linitializers,
            dim118Linitializers,
            dim119Linitializers,
            dim120Linitializers,
            dim121Linitializers,
            dim122Linitializers,
            dim123Linitializers,
            dim124Linitializers,
            dim125Linitializers,
            dim126Linitializers,
            dim127Linitializers,
            dim128Linitializers,
            dim129Linitializers,
            dim130Linitializers,
            dim131Linitializers,
            dim132Linitializers,
            dim133Linitializers,
            dim134Linitializers,
            dim135Linitializers,
            dim136Linitializers,
            dim137Linitializers,
            dim138Linitializers,
            dim139Linitializers,
            dim140Linitializers,
            dim141Linitializers,
            dim142Linitializers,
            dim143Linitializers,
            dim144Linitializers,
            dim145Linitializers,
            dim146Linitializers,
            dim147Linitializers,
            dim148Linitializers,
            dim149Linitializers,
            dim150Linitializers,
            dim151Linitializers,
            dim152Linitializers,
            dim153Linitializers,
            dim154Linitializers,
            dim155Linitializers,
            dim156Linitializers,
            dim157Linitializers,
            dim158Linitializers,
            dim159Linitializers,
            dim160Linitializers,
            dim161Linitializers,
            dim162Linitializers,
            dim163Linitializers,
            dim164Linitializers,
            dim165Linitializers,
            dim166Linitializers,
            dim167Linitializers,
            dim168Linitializers,
            dim169Linitializers,
            dim170Linitializers,
            dim171Linitializers,
            dim172Linitializers,
            dim173Linitializers,
            dim174Linitializers,
            dim175Linitializers,
            dim176Linitializers,
            dim177Linitializers,
            dim178Linitializers,
            dim179Linitializers,
            dim180Linitializers,
            dim181Linitializers,
            dim182Linitializers,
            dim183Linitializers,
            dim184Linitializers,
            dim185Linitializers,
            dim186Linitializers,
            dim187Linitializers,
            dim188Linitializers,
            dim189Linitializers,
            dim190Linitializers,
            dim191Linitializers,
            dim192Linitializers,
            dim193Linitializers,
            dim194Linitializers,
            dim195Linitializers,
            dim196Linitializers,
            dim197Linitializers,
            dim198Linitializers,
            dim199Linitializers,
            dim200Linitializers,
            dim201Linitializers,
            dim202Linitializers,
            dim203Linitializers,
            dim204Linitializers,
            dim205Linitializers,
            dim206Linitializers,
            dim207Linitializers,
            dim208Linitializers,
            dim209Linitializers,
            dim210Linitializers,
            dim211Linitializers,
            dim212Linitializers,
            dim213Linitializers,
            dim214Linitializers,
            dim215Linitializers,
            dim216Linitializers,
            dim217Linitializers,
            dim218Linitializers,
            dim219Linitializers,
            dim220Linitializers,
            dim221Linitializers,
            dim222Linitializers,
            dim223Linitializers,
            dim224Linitializers,
            dim225Linitializers,
            dim226Linitializers,
            dim227Linitializers,
            dim228Linitializers,
            dim229Linitializers,
            dim230Linitializers,
            dim231Linitializers,
            dim232Linitializers,
            dim233Linitializers,
            dim234Linitializers,
            dim235Linitializers,
            dim236Linitializers,
            dim237Linitializers,
            dim238Linitializers,
            dim239Linitializers,
            dim240Linitializers,
            dim241Linitializers,
            dim242Linitializers,
            dim243Linitializers,
            dim244Linitializers,
            dim245Linitializers,
            dim246Linitializers,
            dim247Linitializers,
            dim248Linitializers,
            dim249Linitializers,
            dim250Linitializers,
            dim251Linitializers,
            dim252Linitializers,
            dim253Linitializers,
            dim254Linitializers,
            dim255Linitializers,
            dim256Linitializers,
            dim257Linitializers,
            dim258Linitializers,
            dim259Linitializers,
            dim260Linitializers,
            dim261Linitializers,
            dim262Linitializers,
            dim263Linitializers,
            dim264Linitializers,
            dim265Linitializers,
            dim266Linitializers,
            dim267Linitializers,
            dim268Linitializers,
            dim269Linitializers,
            dim270Linitializers,
            dim271Linitializers,
            dim272Linitializers,
            dim273Linitializers,
            dim274Linitializers,
            dim275Linitializers,
            dim276Linitializers,
            dim277Linitializers,
            dim278Linitializers,
            dim279Linitializers,
            dim280Linitializers,
            dim281Linitializers,
            dim282Linitializers,
            dim283Linitializers,
            dim284Linitializers,
            dim285Linitializers,
            dim286Linitializers,
            dim287Linitializers,
            dim288Linitializers,
            dim289Linitializers,
            dim290Linitializers,
            dim291Linitializers,
            dim292Linitializers,
            dim293Linitializers,
            dim294Linitializers,
            dim295Linitializers,
            dim296Linitializers,
            dim297Linitializers,
            dim298Linitializers,
            dim299Linitializers,
            dim300Linitializers,
            dim301Linitializers,
            dim302Linitializers,
            dim303Linitializers,
            dim304Linitializers,
            dim305Linitializers,
            dim306Linitializers,
            dim307Linitializers,
            dim308Linitializers,
            dim309Linitializers,
            dim310Linitializers,
            dim311Linitializers,
            dim312Linitializers,
            dim313Linitializers,
            dim314Linitializers,
            dim315Linitializers,
            dim316Linitializers,
            dim317Linitializers,
            dim318Linitializers,
            dim319Linitializers,
            dim320Linitializers,
            dim321Linitializers,
            dim322Linitializers,
            dim323Linitializers,
            dim324Linitializers,
            dim325Linitializers,
            dim326Linitializers,
            dim327Linitializers,
            dim328Linitializers,
            dim329Linitializers,
            dim330Linitializers,
            dim331Linitializers,
            dim332Linitializers,
            dim333Linitializers,
            dim334Linitializers,
            dim335Linitializers,
            dim336Linitializers,
            dim337Linitializers,
            dim338Linitializers,
            dim339Linitializers,
            dim340Linitializers,
            dim341Linitializers,
            dim342Linitializers,
            dim343Linitializers,
            dim344Linitializers,
            dim345Linitializers,
            dim346Linitializers,
            dim347Linitializers,
            dim348Linitializers,
            dim349Linitializers,
            dim350Linitializers,
            dim351Linitializers,
            dim352Linitializers,
            dim353Linitializers,
            dim354Linitializers,
            dim355Linitializers,
            dim356Linitializers,
            dim357Linitializers,
            dim358Linitializers,
            dim359Linitializers,
            dim360Linitializers
        };
            const unsigned long	dim1KuoInit[]		=	{	1	};										
            const unsigned long	dim2KuoInit[]		=	{	1	,	1	};								
            const unsigned long	dim3KuoInit[]		=	{	1	,	1	,	1	};						
            const unsigned long	dim4KuoInit[]		=	{	1	,	3	,	1	};						
            const unsigned long	dim5KuoInit[]		=	{	1	,	1	,	7	,	13	};				
            const unsigned long	dim6KuoInit[]		=	{	1	,	1	,	3	,	7	};				
            const unsigned long	dim7KuoInit[]		=	{	1	,	3	,	1	,	7	,	21	};		
            const unsigned long	dim8KuoInit[]		=	{	1	,	1	,	5	,	7	,	11	};		
            const unsigned long	dim9KuoInit[]		=	{	1	,	1	,	5	,	9	,	13	};		
            const unsigned long	dim10KuoInit[]		=	{	1	,	1	,	5	,	3	,	7	};		
            const unsigned long	dim11KuoInit[]		=	{	1	,	3	,	1	,	3	,	9	};		
            const unsigned long	dim12KuoInit[]		=	{	1	,	1	,	3	,	9	,	13	};		
            const unsigned long	dim13KuoInit[]		=	{	1	,	3	,	3	,	13	,	15	,	43	};
            const unsigned long	dim14KuoInit[]		=	{	1	,	1	,	3	,	11	,	3	,	45	};
            const unsigned long	dim15KuoInit[]		=	{	1	,	3	,	5	,	11	,	25	,	45	};
            const unsigned long	dim16KuoInit[]		=	{	1	,	1	,	3	,	13	,	9	,	9	};		
            const unsigned long	dim17KuoInit[]		=	{	1	,	3	,	5	,	1	,	9	,	21	};		
            const unsigned long	dim18KuoInit[]		=	{	1	,	3	,	5	,	7	,	17	,	53	};		
            const unsigned long	dim19KuoInit[]		=	{	1	,	3	,	1	,	7	,	11	,	51	,	115	};
            const unsigned long	dim20KuoInit[]		=	{	1	,	3	,	1	,	1	,	31	,	5	,	1	};
            const unsigned long	dim21KuoInit[]		=	{	1	,	1	,	1	,	15	,	11	,	59	,	21	};
            const unsigned long	dim22KuoInit[]		=	{	1	,	1	,	5	,	9	,	11	,	1	,	121	};
            const unsigned long	dim23KuoInit[]		=	{	1	,	1	,	5	,	7	,	15	,	25	,	13	};
            const unsigned long	dim24KuoInit[]		=	{	1	,	1	,	5	,	7	,	21	,	21	,	37	};
            const unsigned long	dim25KuoInit[]		=	{	1	,	1	,	1	,	3	,	3	,	35	,	123	};
            const unsigned long	dim26KuoInit[]		=	{	1	,	3	,	7	,	7	,	1	,	45	,	7	};
            const unsigned long	dim27KuoInit[]		=	{	1	,	1	,	3	,	13	,	9	,	23	,	33	};
            const unsigned long	dim28KuoInit[]		=	{	1	,	1	,	7	,	9	,	25	,	35	,	11	};
            const unsigned long	dim29KuoInit[]		=	{	1	,	1	,	5	,	3	,	5	,	13	,	91	};
            const unsigned long	dim30KuoInit[]		=	{	1	,	1	,	5	,	5	,	23	,	7	,	101	};
            const unsigned long	dim31KuoInit[]		=	{	1	,	1	,	7	,	5	,	15	,	47	,	117	};
            const unsigned long	dim32KuoInit[]		=	{	1	,	3	,	1	,	13	,	9	,	49	,	23	};		
            const unsigned long	dim33KuoInit[]		=	{	1	,	3	,	1	,	11	,	9	,	61	,	127	};		
            const unsigned long	dim34KuoInit[]		=	{	1	,	3	,	1	,	3	,	17	,	49	,	51	};		
            const unsigned long	dim35KuoInit[]		=	{	1	,	3	,	1	,	9	,	1	,	57	,	93	};		
            const unsigned long	dim36KuoInit[]		=	{	1	,	1	,	1	,	13	,	25	,	23	,	63	};		
            const unsigned long	dim37KuoInit[]		=	{	1	,	1	,	1	,	1	,	31	,	31	,	25	,	231	};
            const unsigned long	dim38KuoInit[]		=	{	1	,	1	,	3	,	15	,	5	,	63	,	117	,	217	};
            const unsigned long	dim39KuoInit[]		=	{	1	,	3	,	1	,	1	,	29	,	5	,	111	,	51	};
            const unsigned long	dim40KuoInit[]		=	{	1	,	1	,	7	,	1	,	17	,	37	,	11	,	53	};
            const unsigned long	dim41KuoInit[]		=	{	1	,	3	,	7	,	15	,	29	,	5	,	3	,	67	};
            const unsigned long	dim42KuoInit[]		=	{	1	,	3	,	7	,	11	,	23	,	27	,	35	,	143	};
            const unsigned long	dim43KuoInit[]		=	{	1	,	1	,	5	,	11	,	11	,	33	,	103	,	179	};
            const unsigned long	dim44KuoInit[]		=	{	1	,	3	,	5	,	7	,	13	,	45	,	87	,	143	};
            const unsigned long	dim45KuoInit[]		=	{	1	,	3	,	7	,	7	,	29	,	17	,	59	,	215	};
            const unsigned long	dim46KuoInit[]		=	{	1	,	3	,	1	,	13	,	17	,	17	,	49	,	249	};
            const unsigned long	dim47KuoInit[]		=	{	1	,	1	,	1	,	3	,	17	,	29	,	75	,	143	};
            const unsigned long	dim48KuoInit[]		=	{	1	,	1	,	1	,	3	,	19	,	55	,	65	,	109	};		
            const unsigned long	dim49KuoInit[]		=	{	1	,	1	,	7	,	3	,	21	,	29	,	13	,	191	};		
            const unsigned long	dim50KuoInit[]		=	{	1	,	1	,	3	,	7	,	7	,	5	,	97	,	179	};		
            const unsigned long	dim51KuoInit[]		=	{	1	,	3	,	5	,	13	,	1	,	17	,	113	,	149	};		
            const unsigned long	dim52KuoInit[]		=	{	1	,	3	,	3	,	11	,	25	,	13	,	105	,	75	};		
            const unsigned long	dim53KuoInit[]		=	{	1	,	3	,	1	,	13	,	29	,	33	,	71	,	117	,	77	};
            const unsigned long	dim54KuoInit[]		=	{	1	,	1	,	1	,	1	,	31	,	41	,	9	,	245	,	205	};
            const unsigned long	dim55KuoInit[]		=	{	1	,	1	,	1	,	15	,	25	,	23	,	111	,	105	,	95	};
            const unsigned long	dim56KuoInit[]		=	{	1	,	1	,	3	,	5	,	3	,	5	,	81	,	251	,	221	};
            const unsigned long	dim57KuoInit[]		=	{	1	,	3	,	5	,	5	,	9	,	53	,	37	,	41	,	509	};
            const unsigned long	dim58KuoInit[]		=	{	1	,	1	,	5	,	15	,	21	,	25	,	77	,	225	,	333	};
            const unsigned long	dim59KuoInit[]		=	{	1	,	3	,	5	,	13	,	3	,	17	,	1	,	101	,	397	};
            const unsigned long	dim60KuoInit[]		=	{	1	,	3	,	5	,	11	,	27	,	53	,	115	,	87	,	47	};
            const unsigned long	dim61KuoInit[]		=	{	1	,	1	,	3	,	11	,	31	,	39	,	21	,	233	,	9	};
            const unsigned long	dim62KuoInit[]		=	{	1	,	1	,	3	,	15	,	17	,	19	,	73	,	147	,	351	};
            const unsigned long	dim63KuoInit[]		=	{	1	,	1	,	3	,	3	,	29	,	17	,	1	,	105	,	293	};
            const unsigned long	dim64KuoInit[]		=	{	1	,	1	,	3	,	15	,	9	,	21	,	103	,	239	,	433	};
            const unsigned long	dim65KuoInit[]		=	{	1	,	1	,	1	,	15	,	17	,	35	,	49	,	7	,	435	};
            const unsigned long	dim66KuoInit[]		=	{	1	,	3	,	7	,	3	,	9	,	51	,	91	,	177	,	255	};
            const unsigned long	dim67KuoInit[]		=	{	1	,	1	,	5	,	5	,	31	,	9	,	39	,	209	,	511	};
            const unsigned long	dim68KuoInit[]		=	{	1	,	1	,	7	,	3	,	11	,	11	,	19	,	227	,	343	};
            const unsigned long	dim69KuoInit[]		=	{	1	,	3	,	7	,	3	,	17	,	13	,	105	,	151	,	225	};
            const unsigned long	dim70KuoInit[]		=	{	1	,	1	,	7	,	11	,	29	,	35	,	105	,	11	,	497	};
            const unsigned long	dim71KuoInit[]		=	{	1	,	3	,	7	,	15	,	5	,	13	,	31	,	201	,	63	};
            const unsigned long	dim72KuoInit[]		=	{	1	,	1	,	5	,	7	,	17	,	31	,	9	,	63	,	167	};
            const unsigned long	dim73KuoInit[]		=	{	1	,	1	,	1	,	13	,	1	,	51	,	9	,	115	,	217	};
            const unsigned long	dim74KuoInit[]		=	{	1	,	1	,	7	,	15	,	23	,	49	,	125	,	163	,	39	};
            const unsigned long	dim75KuoInit[]		=	{	1	,	1	,	3	,	13	,	11	,	45	,	77	,	149	,	173	};
            const unsigned long	dim76KuoInit[]		=	{	1	,	3	,	3	,	3	,	7	,	27	,	43	,	27	,	361	};
            const unsigned long	dim77KuoInit[]		=	{	1	,	3	,	3	,	5	,	5	,	37	,	9	,	51	,	149	};
            const unsigned long	dim78KuoInit[]		=	{	1	,	1	,	1	,	15	,	13	,	13	,	49	,	251	,	385	};
            const unsigned long	dim79KuoInit[]		=	{	1	,	1	,	3	,	7	,	1	,	57	,	27	,	25	,	335	};
            const unsigned long	dim80KuoInit[]		=	{	1	,	3	,	3	,	9	,	31	,	55	,	41	,	143	,	97	};
            const unsigned long	dim81KuoInit[]		=	{	1	,	3	,	3	,	11	,	31	,	33	,	65	,	57	,	113	};
            const unsigned long	dim82KuoInit[]		=	{	1	,	1	,	5	,	13	,	19	,	63	,	5	,	71	,	317	};
            const unsigned long	dim83KuoInit[]		=	{	1	,	1	,	1	,	5	,	29	,	45	,	35	,	107	,	113	};
            const unsigned long	dim84KuoInit[]		=	{	1	,	1	,	3	,	7	,	3	,	31	,	81	,	57	,	439	};
            const unsigned long	dim85KuoInit[]		=	{	1	,	3	,	5	,	5	,	1	,	7	,	3	,	21	,	319	};
            const unsigned long	dim86KuoInit[]		=	{	1	,	3	,	1	,	15	,	27	,	49	,	25	,	247	,	455	};
            const unsigned long	dim87KuoInit[]		=	{	1	,	1	,	3	,	5	,	5	,	43	,	105	,	207	,	271	};
            const unsigned long	dim88KuoInit[]		=	{	1	,	3	,	3	,	7	,	29	,	63	,	25	,	239	,	165	};
            const unsigned long	dim89KuoInit[]		=	{	1	,	3	,	7	,	1	,	19	,	23	,	87	,	23	,	161	};
            const unsigned long	dim90KuoInit[]		=	{	1	,	3	,	7	,	5	,	27	,	1	,	15	,	113	,	85	};
            const unsigned long	dim91KuoInit[]		=	{	1	,	1	,	1	,	13	,	23	,	39	,	103	,	59	,	105	};
            const unsigned long	dim92KuoInit[]		=	{	1	,	3	,	1	,	15	,	9	,	35	,	65	,	225	,	385	};
            const unsigned long	dim93KuoInit[]		=	{	1	,	1	,	5	,	9	,	5	,	63	,	109	,	153	,	25	};
            const unsigned long	dim94KuoInit[]		=	{	1	,	3	,	7	,	11	,	5	,	41	,	103	,	103	,	245	};
            const unsigned long	dim95KuoInit[]		=	{	1	,	1	,	1	,	3	,	25	,	5	,	71	,	203	,	351	};
            const unsigned long	dim96KuoInit[]		=	{	1	,	1	,	5	,	5	,	27	,	49	,	111	,	253	,	265	};		
            const unsigned long	dim97KuoInit[]		=	{	1	,	1	,	1	,	11	,	31	,	63	,	91	,	179	,	359	};		
            const unsigned long	dim98KuoInit[]		=	{	1	,	3	,	7	,	1	,	9	,	41	,	117	,	69	,	141	};		
            const unsigned long	dim99KuoInit[]		=	{	1	,	3	,	1	,	5	,	11	,	25	,	7	,	73	,	477	};		
            const unsigned long	dim100KuoInit[]		=	{	1	,	1	,	3	,	1	,	31	,	7	,	51	,	67	,	97	};		
            const unsigned long	dim101KuoInit[]		=	{	1	,	3	,	3	,	15	,	31	,	61	,	27	,	137	,	319	,	605	};
            const unsigned long	dim102KuoInit[]		=	{	1	,	3	,	5	,	7	,	29	,	45	,	31	,	211	,	287	,	13	};
            const unsigned long	dim103KuoInit[]		=	{	1	,	3	,	1	,	7	,	25	,	61	,	121	,	91	,	215	,	439	};
            const unsigned long	dim104KuoInit[]		=	{	1	,	1	,	3	,	11	,	31	,	51	,	31	,	151	,	421	,	195	};
            const unsigned long	dim105KuoInit[]		=	{	1	,	1	,	5	,	13	,	17	,	23	,	47	,	9	,	33	,	1003	};
            const unsigned long	dim106KuoInit[]		=	{	1	,	1	,	1	,	3	,	23	,	5	,	109	,	23	,	125	,	773	};
            const unsigned long	dim107KuoInit[]		=	{	1	,	3	,	5	,	9	,	7	,	41	,	33	,	81	,	277	,	623	};
            const unsigned long	dim108KuoInit[]		=	{	1	,	1	,	1	,	11	,	9	,	33	,	51	,	31	,	181	,	985	};
            const unsigned long	dim109KuoInit[]		=	{	1	,	1	,	7	,	11	,	3	,	37	,	3	,	107	,	405	,	639	};
            const unsigned long	dim110KuoInit[]		=	{	1	,	1	,	5	,	15	,	17	,	7	,	35	,	221	,	255	,	753	};
            const unsigned long	dim111KuoInit[]		=	{	1	,	1	,	7	,	3	,	31	,	11	,	113	,	105	,	275	,	683	};
            const unsigned long	dim112KuoInit[]		=	{	1	,	1	,	3	,	11	,	3	,	39	,	57	,	237	,	189	,	561	};
            const unsigned long	dim113KuoInit[]		=	{	1	,	3	,	3	,	7	,	17	,	37	,	55	,	97	,	499	,	511	};
            const unsigned long	dim114KuoInit[]		=	{	1	,	1	,	7	,	11	,	31	,	27	,	21	,	241	,	127	,	279	};
            const unsigned long	dim115KuoInit[]		=	{	1	,	3	,	1	,	7	,	1	,	19	,	95	,	3	,	511	,	143	};
            const unsigned long	dim116KuoInit[]		=	{	1	,	3	,	5	,	3	,	21	,	39	,	115	,	155	,	67	,	607	};
            const unsigned long	dim117KuoInit[]		=	{	1	,	1	,	7	,	7	,	25	,	37	,	33	,	149	,	89	,	997	};
            const unsigned long	dim118KuoInit[]		=	{	1	,	3	,	1	,	7	,	3	,	7	,	113	,	165	,	235	,	943	};
            const unsigned long	dim119KuoInit[]		=	{	1	,	3	,	7	,	9	,	29	,	5	,	93	,	255	,	323	,	555	};
            const unsigned long	dim120KuoInit[]		=	{	1	,	1	,	1	,	3	,	13	,	45	,	89	,	87	,	29	,	23	};
            const unsigned long	dim121KuoInit[]		=	{	1	,	3	,	3	,	11	,	11	,	29	,	75	,	91	,	211	,	459	};
            const unsigned long	dim122KuoInit[]		=	{	1	,	3	,	5	,	15	,	11	,	43	,	105	,	1	,	227	,	711	};
            const unsigned long	dim123KuoInit[]		=	{	1	,	1	,	5	,	5	,	13	,	33	,	29	,	217	,	511	,	241	};
            const unsigned long	dim124KuoInit[]		=	{	1	,	3	,	7	,	9	,	1	,	47	,	125	,	151	,	375	,	815	};
            const unsigned long	dim125KuoInit[]		=	{	1	,	1	,	1	,	3	,	25	,	29	,	5	,	39	,	133	,	585	};
            const unsigned long	dim126KuoInit[]		=	{	1	,	1	,	5	,	9	,	9	,	31	,	79	,	227	,	395	,	517	};
            const unsigned long	dim127KuoInit[]		=	{	1	,	3	,	3	,	3	,	15	,	3	,	47	,	159	,	233	,	339	};
            const unsigned long	dim128KuoInit[]		=	{	1	,	3	,	5	,	3	,	15	,	13	,	79	,	109	,	187	,	249	};
            const unsigned long	dim129KuoInit[]		=	{	1	,	3	,	1	,	15	,	23	,	39	,	85	,	157	,	323	,	535	};
            const unsigned long	dim130KuoInit[]		=	{	1	,	1	,	1	,	5	,	11	,	25	,	5	,	153	,	19	,	779	};
            const unsigned long	dim131KuoInit[]		=	{	1	,	3	,	5	,	1	,	9	,	43	,	27	,	179	,	407	,	167	};
            const unsigned long	dim132KuoInit[]		=	{	1	,	3	,	5	,	5	,	25	,	47	,	21	,	23	,	57	,	415	};
            const unsigned long	dim133KuoInit[]		=	{	1	,	1	,	1	,	7	,	3	,	29	,	45	,	195	,	349	,	267	};
            const unsigned long	dim134KuoInit[]		=	{	1	,	3	,	5	,	3	,	19	,	13	,	61	,	243	,	125	,	961	};
            const unsigned long	dim135KuoInit[]		=	{	1	,	3	,	7	,	3	,	29	,	49	,	7	,	197	,	387	,	747	};
            const unsigned long	dim136KuoInit[]		=	{	1	,	3	,	7	,	11	,	31	,	31	,	109	,	87	,	345	,	807	};
            const unsigned long	dim137KuoInit[]		=	{	1	,	3	,	1	,	3	,	21	,	57	,	43	,	207	,	221	,	85	};
            const unsigned long	dim138KuoInit[]		=	{	1	,	1	,	3	,	13	,	19	,	19	,	41	,	19	,	509	,	419	};
            const unsigned long	dim139KuoInit[]		=	{	1	,	1	,	3	,	3	,	23	,	17	,	85	,	99	,	421	,	875	};
            const unsigned long	dim140KuoInit[]		=	{	1	,	3	,	1	,	15	,	27	,	43	,	89	,	151	,	39	,	643	};
            const unsigned long	dim141KuoInit[]		=	{	1	,	1	,	1	,	5	,	3	,	59	,	25	,	79	,	319	,	959	};
            const unsigned long	dim142KuoInit[]		=	{	1	,	3	,	5	,	9	,	3	,	27	,	71	,	209	,	195	,	569	};
            const unsigned long	dim143KuoInit[]		=	{	1	,	3	,	3	,	3	,	11	,	47	,	67	,	1	,	221	,	119	};
            const unsigned long	dim144KuoInit[]		=	{	1	,	1	,	5	,	11	,	11	,	39	,	95	,	29	,	7	,	957	};
            const unsigned long	dim145KuoInit[]		=	{	1	,	1	,	7	,	13	,	13	,	57	,	47	,	71	,	165	,	393	};
            const unsigned long	dim146KuoInit[]		=	{	1	,	3	,	5	,	3	,	5	,	47	,	127	,	209	,	377	,	53	};
            const unsigned long	dim147KuoInit[]		=	{	1	,	3	,	1	,	5	,	13	,	51	,	69	,	165	,	477	,	961	};
            const unsigned long	dim148KuoInit[]		=	{	1	,	3	,	5	,	9	,	25	,	37	,	5	,	133	,	117	,	29	};
            const unsigned long	dim149KuoInit[]		=	{	1	,	1	,	1	,	9	,	7	,	21	,	95	,	53	,	205	,	805	};
            const unsigned long	dim150KuoInit[]		=	{	1	,	1	,	5	,	11	,	25	,	39	,	25	,	15	,	497	,	525	};
            const unsigned long	dim151KuoInit[]		=	{	1	,	3	,	5	,	5	,	15	,	37	,	33	,	255	,	97	,	341	};
            const unsigned long	dim152KuoInit[]		=	{	1	,	3	,	1	,	13	,	31	,	63	,	35	,	203	,	415	,	903	};
            const unsigned long	dim153KuoInit[]		=	{	1	,	1	,	5	,	7	,	13	,	59	,	55	,	17	,	103	,	221	};
            const unsigned long	dim154KuoInit[]		=	{	1	,	1	,	5	,	3	,	7	,	37	,	113	,	139	,	269	,	987	};
            const unsigned long	dim155KuoInit[]		=	{	1	,	3	,	3	,	7	,	21	,	3	,	47	,	175	,	327	,	633	};
            const unsigned long	dim156KuoInit[]		=	{	1	,	1	,	1	,	3	,	13	,	61	,	19	,	209	,	39	,	695	};
            const unsigned long	dim157KuoInit[]		=	{	1	,	1	,	1	,	9	,	31	,	61	,	21	,	75	,	445	,	647	};
            const unsigned long	dim158KuoInit[]		=	{	1	,	1	,	5	,	3	,	21	,	1	,	65	,	211	,	405	,	629	};
            const unsigned long	dim159KuoInit[]		=	{	1	,	3	,	7	,	15	,	9	,	25	,	119	,	13	,	465	,	865	};
            const unsigned long	dim160KuoInit[]		=	{	1	,	3	,	1	,	15	,	29	,	19	,	103	,	39	,	37	,	289	};		
            const unsigned long	dim161KuoInit[]		=	{	1	,	1	,	1	,	5	,	25	,	23	,	21	,	111	,	455	,	665	,	1609	};
            const unsigned long	dim162KuoInit[]		=	{	1	,	3	,	3	,	13	,	27	,	29	,	45	,	191	,	385	,	413	,	1875	};
            const unsigned long	dim163KuoInit[]		=	{	1	,	3	,	7	,	7	,	19	,	17	,	47	,	231	,	337	,	781	,	1235	};
            const unsigned long	dim164KuoInit[]		=	{	1	,	1	,	5	,	7	,	7	,	25	,	99	,	239	,	501	,	29	,	159	};
            const unsigned long	dim165KuoInit[]		=	{	1	,	1	,	1	,	3	,	5	,	19	,	101	,	147	,	225	,	353	,	1117	};
            const unsigned long	dim166KuoInit[]		=	{	1	,	3	,	7	,	11	,	17	,	57	,	45	,	73	,	219	,	853	,	1379	};
            const unsigned long	dim167KuoInit[]		=	{	1	,	3	,	3	,	7	,	11	,	47	,	23	,	67	,	157	,	283	,	867	};
            const unsigned long	dim168KuoInit[]		=	{	1	,	1	,	7	,	7	,	23	,	55	,	71	,	155	,	237	,	927	,	929	};
            const unsigned long	dim169KuoInit[]		=	{	1	,	3	,	3	,	15	,	11	,	19	,	59	,	147	,	373	,	619	,	53	};
            const unsigned long	dim170KuoInit[]		=	{	1	,	1	,	3	,	15	,	13	,	23	,	11	,	33	,	45	,	121	,	241	};
            const unsigned long	dim171KuoInit[]		=	{	1	,	1	,	1	,	9	,	13	,	29	,	3	,	131	,	373	,	91	,	1207	};
            const unsigned long	dim172KuoInit[]		=	{	1	,	1	,	1	,	15	,	21	,	29	,	59	,	197	,	375	,	637	,	1275	};
            const unsigned long	dim173KuoInit[]		=	{	1	,	1	,	3	,	11	,	27	,	31	,	87	,	95	,	119	,	249	,	1913	};
            const unsigned long	dim174KuoInit[]		=	{	1	,	3	,	1	,	5	,	25	,	51	,	1	,	241	,	53	,	883	,	61	};
            const unsigned long	dim175KuoInit[]		=	{	1	,	3	,	1	,	5	,	23	,	37	,	83	,	45	,	287	,	911	,	717	};
            const unsigned long	dim176KuoInit[]		=	{	1	,	3	,	1	,	3	,	9	,	39	,	105	,	129	,	475	,	399	,	1783	};
            const unsigned long	dim177KuoInit[]		=	{	1	,	3	,	5	,	5	,	23	,	11	,	119	,	205	,	317	,	797	,	605	};
            const unsigned long	dim178KuoInit[]		=	{	1	,	1	,	7	,	5	,	13	,	57	,	19	,	225	,	53	,	987	,	1463	};
            const unsigned long	dim179KuoInit[]		=	{	1	,	3	,	5	,	11	,	3	,	59	,	1	,	201	,	97	,	1011	,	1059	};
            const unsigned long	dim180KuoInit[]		=	{	1	,	3	,	3	,	9	,	29	,	51	,	39	,	199	,	243	,	877	,	883	};
            const unsigned long	dim181KuoInit[]		=	{	1	,	1	,	3	,	5	,	29	,	13	,	97	,	15	,	387	,	435	,	701	};
            const unsigned long	dim182KuoInit[]		=	{	1	,	3	,	3	,	11	,	21	,	41	,	3	,	119	,	385	,	503	,	2015	};
            const unsigned long	dim183KuoInit[]		=	{	1	,	3	,	3	,	1	,	17	,	9	,	49	,	13	,	309	,	29	,	1501	};
            const unsigned long	dim184KuoInit[]		=	{	1	,	3	,	1	,	3	,	13	,	29	,	91	,	51	,	225	,	989	,	1701	};
            const unsigned long	dim185KuoInit[]		=	{	1	,	1	,	7	,	13	,	1	,	55	,	59	,	243	,	399	,	65	,	1725	};
            const unsigned long	dim186KuoInit[]		=	{	1	,	3	,	7	,	7	,	19	,	29	,	97	,	161	,	195	,	257	,	1595	};
            const unsigned long	dim187KuoInit[]		=	{	1	,	3	,	7	,	15	,	29	,	49	,	49	,	23	,	157	,	363	,	1355	};
            const unsigned long	dim188KuoInit[]		=	{	1	,	3	,	1	,	13	,	23	,	39	,	45	,	73	,	147	,	323	,	1965	};
            const unsigned long	dim189KuoInit[]		=	{	1	,	3	,	1	,	15	,	17	,	27	,	17	,	19	,	371	,	387	,	395	};
            const unsigned long	dim190KuoInit[]		=	{	1	,	1	,	1	,	9	,	19	,	39	,	31	,	3	,	291	,	379	,	605	};
            const unsigned long	dim191KuoInit[]		=	{	1	,	1	,	7	,	1	,	15	,	17	,	47	,	133	,	485	,	145	,	629	};
            const unsigned long	dim192KuoInit[]		=	{	1	,	1	,	1	,	13	,	11	,	11	,	117	,	233	,	85	,	1	,	1357	};
            const unsigned long	dim193KuoInit[]		=	{	1	,	1	,	7	,	15	,	19	,	51	,	115	,	19	,	333	,	683	,	917	};
            const unsigned long	dim194KuoInit[]		=	{	1	,	3	,	7	,	9	,	27	,	47	,	17	,	209	,	461	,	261	,	761	};
            const unsigned long	dim195KuoInit[]		=	{	1	,	1	,	3	,	1	,	27	,	15	,	43	,	141	,	291	,	363	,	605	};
            const unsigned long	dim196KuoInit[]		=	{	1	,	3	,	7	,	5	,	1	,	3	,	43	,	75	,	115	,	653	,	847	};
            const unsigned long	dim197KuoInit[]		=	{	1	,	1	,	3	,	1	,	11	,	51	,	69	,	119	,	195	,	979	,	1543	};
            const unsigned long	dim198KuoInit[]		=	{	1	,	1	,	7	,	3	,	11	,	29	,	29	,	59	,	339	,	273	,	855	};
            const unsigned long	dim199KuoInit[]		=	{	1	,	1	,	1	,	1	,	11	,	15	,	59	,	97	,	267	,	647	,	2037	};
            const unsigned long	dim200KuoInit[]		=	{	1	,	1	,	5	,	3	,	25	,	43	,	41	,	195	,	457	,	155	,	1707	};
            const unsigned long	dim201KuoInit[]		=	{	1	,	3	,	3	,	15	,	11	,	47	,	43	,	15	,	505	,	761	,	1259	};
            const unsigned long	dim202KuoInit[]		=	{	1	,	3	,	5	,	15	,	27	,	29	,	55	,	109	,	187	,	545	,	1177	};
            const unsigned long	dim203KuoInit[]		=	{	1	,	3	,	1	,	5	,	29	,	25	,	89	,	11	,	75	,	771	,	1437	};
            const unsigned long	dim204KuoInit[]		=	{	1	,	3	,	5	,	11	,	19	,	17	,	27	,	59	,	231	,	35	,	391	};
            const unsigned long	dim205KuoInit[]		=	{	1	,	1	,	7	,	9	,	3	,	17	,	31	,	81	,	317	,	973	,	1913	};
            const unsigned long	dim206KuoInit[]		=	{	1	,	1	,	1	,	7	,	3	,	21	,	113	,	41	,	335	,	789	,	1195	};
            const unsigned long	dim207KuoInit[]		=	{	1	,	1	,	7	,	1	,	13	,	61	,	41	,	117	,	301	,	707	,	693	};
            const unsigned long	dim208KuoInit[]		=	{	1	,	1	,	1	,	7	,	3	,	1	,	79	,	145	,	137	,	307	,	205	};
            const unsigned long	dim209KuoInit[]		=	{	1	,	3	,	7	,	1	,	27	,	49	,	83	,	127	,	265	,	669	,	87	};
            const unsigned long	dim210KuoInit[]		=	{	1	,	1	,	3	,	15	,	21	,	31	,	115	,	115	,	343	,	55	,	1049	};
            const unsigned long	dim211KuoInit[]		=	{	1	,	1	,	5	,	9	,	7	,	25	,	43	,	255	,	363	,	123	,	299	};
            const unsigned long	dim212KuoInit[]		=	{	1	,	3	,	5	,	13	,	1	,	9	,	79	,	245	,	91	,	233	,	1703	};
            const unsigned long	dim213KuoInit[]		=	{	1	,	3	,	5	,	1	,	17	,	39	,	81	,	147	,	147	,	335	,	819	};
            const unsigned long	dim214KuoInit[]		=	{	1	,	3	,	7	,	3	,	31	,	63	,	55	,	53	,	389	,	307	,	451	};
            const unsigned long	dim215KuoInit[]		=	{	1	,	1	,	7	,	7	,	23	,	41	,	125	,	147	,	323	,	7	,	1211	};
            const unsigned long	dim216KuoInit[]		=	{	1	,	1	,	1	,	13	,	23	,	9	,	35	,	177	,	165	,	315	,	1885	};
            const unsigned long	dim217KuoInit[]		=	{	1	,	3	,	3	,	7	,	11	,	39	,	99	,	197	,	405	,	377	,	555	};
            const unsigned long	dim218KuoInit[]		=	{	1	,	3	,	3	,	11	,	15	,	39	,	125	,	59	,	323	,	377	,	325	};
            const unsigned long	dim219KuoInit[]		=	{	1	,	3	,	5	,	9	,	7	,	45	,	75	,	143	,	265	,	161	,	1701	};
            const unsigned long	dim220KuoInit[]		=	{	1	,	1	,	7	,	5	,	31	,	21	,	35	,	1	,	511	,	585	,	923	};
            const unsigned long	dim221KuoInit[]		=	{	1	,	3	,	1	,	5	,	3	,	31	,	65	,	33	,	199	,	205	,	1673	};
            const unsigned long	dim222KuoInit[]		=	{	1	,	3	,	7	,	9	,	17	,	61	,	123	,	33	,	405	,	235	,	645	};
            const unsigned long	dim223KuoInit[]		=	{	1	,	3	,	3	,	15	,	27	,	1	,	35	,	165	,	509	,	681	,	621	};
            const unsigned long	dim224KuoInit[]		=	{	1	,	3	,	5	,	15	,	19	,	13	,	49	,	85	,	5	,	661	,	805	};
            const unsigned long	dim225KuoInit[]		=	{	1	,	3	,	7	,	3	,	3	,	51	,	77	,	167	,	461	,	731	,	1681	};
            const unsigned long	dim226KuoInit[]		=	{	1	,	1	,	3	,	5	,	21	,	51	,	125	,	65	,	41	,	613	,	1009	};
            const unsigned long	dim227KuoInit[]		=	{	1	,	3	,	7	,	11	,	17	,	39	,	127	,	31	,	423	,	87	,	1413	};
            const unsigned long	dim228KuoInit[]		=	{	1	,	3	,	1	,	5	,	11	,	41	,	25	,	49	,	169	,	637	,	1599	};
            const unsigned long	dim229KuoInit[]		=	{	1	,	3	,	5	,	13	,	31	,	27	,	75	,	65	,	391	,	857	,	1491	};
            const unsigned long	dim230KuoInit[]		=	{	1	,	1	,	1	,	13	,	5	,	61	,	65	,	183	,	373	,	161	,	235	};
            const unsigned long	dim231KuoInit[]		=	{	1	,	3	,	1	,	1	,	27	,	49	,	47	,	55	,	503	,	683	,	301	};
            const unsigned long	dim232KuoInit[]		=	{	1	,	1	,	7	,	11	,	17	,	31	,	113	,	255	,	281	,	341	,	2033	};
            const unsigned long	dim233KuoInit[]		=	{	1	,	3	,	3	,	13	,	27	,	27	,	27	,	225	,	161	,	187	,	485	};
            const unsigned long	dim234KuoInit[]		=	{	1	,	1	,	1	,	3	,	21	,	27	,	105	,	215	,	241	,	211	,	899	};
            const unsigned long	dim235KuoInit[]		=	{	1	,	3	,	5	,	5	,	11	,	37	,	1	,	7	,	335	,	221	,	757	};
            const unsigned long	dim236KuoInit[]		=	{	1	,	1	,	5	,	7	,	21	,	55	,	61	,	99	,	23	,	957	,	35	};
            const unsigned long	dim237KuoInit[]		=	{	1	,	3	,	5	,	5	,	13	,	53	,	31	,	121	,	111	,	47	,	1491	};
            const unsigned long	dim238KuoInit[]		=	{	1	,	1	,	5	,	13	,	17	,	5	,	11	,	179	,	357	,	1003	,	1179	};
            const unsigned long	dim239KuoInit[]		=	{	1	,	1	,	5	,	13	,	29	,	9	,	3	,	91	,	471	,	369	,	323	};
            const unsigned long	dim240KuoInit[]		=	{	1	,	3	,	7	,	5	,	7	,	25	,	109	,	43	,	133	,	261	,	1623	};
            const unsigned long	dim241KuoInit[]		=	{	1	,	3	,	7	,	15	,	13	,	33	,	125	,	81	,	491	,	331	,	1243	};
            const unsigned long	dim242KuoInit[]		=	{	1	,	3	,	3	,	11	,	15	,	21	,	57	,	63	,	55	,	341	,	1717	};
            const unsigned long	dim243KuoInit[]		=	{	1	,	3	,	1	,	1	,	31	,	13	,	67	,	89	,	459	,	879	,	863	};
            const unsigned long	dim244KuoInit[]		=	{	1	,	3	,	7	,	15	,	7	,	21	,	117	,	229	,	467	,	227	,	1463	};
            const unsigned long	dim245KuoInit[]		=	{	1	,	1	,	1	,	7	,	23	,	43	,	53	,	79	,	3	,	905	,	1049	};
            const unsigned long	dim246KuoInit[]		=	{	1	,	3	,	7	,	1	,	19	,	1	,	61	,	15	,	377	,	579	,	145	};
            const unsigned long	dim247KuoInit[]		=	{	1	,	3	,	1	,	15	,	27	,	25	,	61	,	171	,	425	,	631	,	1343	};
            const unsigned long	dim248KuoInit[]		=	{	1	,	3	,	7	,	5	,	15	,	31	,	69	,	5	,	217	,	809	,	647	};
            const unsigned long	dim249KuoInit[]		=	{	1	,	1	,	5	,	3	,	5	,	35	,	101	,	21	,	33	,	757	,	559	};
            const unsigned long	dim250KuoInit[]		=	{	1	,	3	,	5	,	13	,	3	,	7	,	101	,	93	,	31	,	315	,	1897	};
            const unsigned long	dim251KuoInit[]		=	{	1	,	1	,	3	,	1	,	21	,	31	,	37	,	127	,	211	,	429	,	1019	};
            const unsigned long	dim252KuoInit[]		=	{	1	,	1	,	3	,	9	,	11	,	27	,	113	,	203	,	115	,	765	,	759	};
            const unsigned long	dim253KuoInit[]		=	{	1	,	1	,	3	,	13	,	7	,	21	,	89	,	99	,	469	,	729	,	683	};
            const unsigned long	dim254KuoInit[]		=	{	1	,	1	,	1	,	5	,	27	,	17	,	43	,	177	,	225	,	405	,	1929	};
            const unsigned long	dim255KuoInit[]		=	{	1	,	3	,	1	,	7	,	21	,	55	,	69	,	41	,	309	,	487	,	681	};
            const unsigned long	dim256KuoInit[]		=	{	1	,	3	,	5	,	11	,	19	,	21	,	61	,	163	,	337	,	61	,	301	};
            const unsigned long	dim257KuoInit[]		=	{	1	,	3	,	1	,	7	,	21	,	61	,	67	,	219	,	281	,	21	,	1271	};
            const unsigned long	dim258KuoInit[]		=	{	1	,	1	,	5	,	11	,	5	,	25	,	51	,	207	,	235	,	289	,	395	};
            const unsigned long	dim259KuoInit[]		=	{	1	,	1	,	3	,	3	,	17	,	61	,	5	,	179	,	457	,	941	,	1577	};
            const unsigned long	dim260KuoInit[]		=	{	1	,	1	,	7	,	7	,	17	,	23	,	127	,	35	,	407	,	545	,	169	};
            const unsigned long	dim261KuoInit[]		=	{	1	,	3	,	7	,	15	,	21	,	19	,	123	,	145	,	97	,	263	,	1881	};
            const unsigned long	dim262KuoInit[]		=	{	1	,	3	,	7	,	11	,	25	,	49	,	117	,	165	,	79	,	137	,	1311	};
            const unsigned long	dim263KuoInit[]		=	{	1	,	3	,	1	,	11	,	3	,	17	,	9	,	177	,	367	,	863	,	1255	};
            const unsigned long	dim264KuoInit[]		=	{	1	,	1	,	5	,	13	,	9	,	1	,	31	,	3	,	69	,	1009	,	1091	};
            const unsigned long	dim265KuoInit[]		=	{	1	,	3	,	7	,	15	,	25	,	41	,	13	,	3	,	319	,	131	,	493	};
            const unsigned long	dim266KuoInit[]		=	{	1	,	1	,	1	,	15	,	9	,	45	,	25	,	21	,	405	,	61	,	1167	};
            const unsigned long	dim267KuoInit[]		=	{	1	,	1	,	1	,	1	,	31	,	3	,	95	,	113	,	475	,	209	,	1473	};
            const unsigned long	dim268KuoInit[]		=	{	1	,	1	,	5	,	5	,	27	,	9	,	95	,	121	,	193	,	481	,	1785	};
            const unsigned long	dim269KuoInit[]		=	{	1	,	1	,	3	,	13	,	1	,	13	,	57	,	167	,	273	,	491	,	867	};
            const unsigned long	dim270KuoInit[]		=	{	1	,	1	,	7	,	1	,	9	,	27	,	123	,	65	,	407	,	609	,	1979	};
            const unsigned long	dim271KuoInit[]		=	{	1	,	3	,	1	,	15	,	5	,	5	,	89	,	17	,	207	,	249	,	1725	};
            const unsigned long	dim272KuoInit[]		=	{	1	,	3	,	5	,	11	,	11	,	15	,	91	,	33	,	113	,	531	,	831	};
            const unsigned long	dim273KuoInit[]		=	{	1	,	1	,	5	,	9	,	27	,	51	,	17	,	141	,	215	,	171	,	455	};
            const unsigned long	dim274KuoInit[]		=	{	1	,	1	,	5	,	9	,	11	,	25	,	115	,	5	,	173	,	209	,	1455	};
            const unsigned long	dim275KuoInit[]		=	{	1	,	1	,	7	,	7	,	13	,	51	,	21	,	79	,	357	,	423	,	1855	};
            const unsigned long	dim276KuoInit[]		=	{	1	,	1	,	7	,	1	,	13	,	37	,	123	,	27	,	477	,	1023	,	1235	};
            const unsigned long	dim277KuoInit[]		=	{	1	,	1	,	7	,	13	,	25	,	43	,	123	,	41	,	199	,	779	,	1691	};
            const unsigned long	dim278KuoInit[]		=	{	1	,	3	,	3	,	9	,	7	,	27	,	81	,	147	,	465	,	135	,	829	};
            const unsigned long	dim279KuoInit[]		=	{	1	,	1	,	7	,	11	,	17	,	5	,	25	,	177	,	377	,	629	,	915	};
            const unsigned long	dim280KuoInit[]		=	{	1	,	1	,	3	,	15	,	11	,	19	,	45	,	229	,	409	,	187	,	507	};
            const unsigned long	dim281KuoInit[]		=	{	1	,	1	,	7	,	5	,	7	,	49	,	21	,	49	,	395	,	185	,	391	};
            const unsigned long	dim282KuoInit[]		=	{	1	,	3	,	1	,	1	,	1	,	37	,	123	,	255	,	223	,	473	,	1011	};
            const unsigned long	dim283KuoInit[]		=	{	1	,	3	,	3	,	13	,	3	,	7	,	113	,	183	,	457	,	561	,	1427	};
            const unsigned long	dim284KuoInit[]		=	{	1	,	1	,	1	,	11	,	27	,	39	,	125	,	241	,	279	,	905	,	1907	};
            const unsigned long	dim285KuoInit[]		=	{	1	,	3	,	1	,	15	,	19	,	19	,	9	,	233	,	39	,	701	,	939	};
            const unsigned long	dim286KuoInit[]		=	{	1	,	3	,	5	,	11	,	15	,	51	,	1	,	25	,	461	,	671	,	611	};
            const unsigned long	dim287KuoInit[]		=	{	1	,	3	,	1	,	3	,	23	,	39	,	23	,	191	,	219	,	911	,	1247	};
            const unsigned long	dim288KuoInit[]		=	{	1	,	3	,	1	,	3	,	1	,	27	,	91	,	97	,	55	,	863	,	805	};
            const unsigned long	dim289KuoInit[]		=	{	1	,	3	,	7	,	7	,	5	,	63	,	81	,	33	,	383	,	645	,	841	};
            const unsigned long	dim290KuoInit[]		=	{	1	,	3	,	7	,	5	,	27	,	39	,	33	,	25	,	185	,	799	,	1109	};
            const unsigned long	dim291KuoInit[]		=	{	1	,	3	,	1	,	11	,	11	,	61	,	109	,	209	,	345	,	465	,	935	};
            const unsigned long	dim292KuoInit[]		=	{	1	,	3	,	5	,	13	,	19	,	55	,	117	,	237	,	455	,	953	,	905	};
            const unsigned long	dim293KuoInit[]		=	{	1	,	3	,	7	,	9	,	9	,	25	,	77	,	37	,	111	,	357	,	131	};
            const unsigned long	dim294KuoInit[]		=	{	1	,	1	,	3	,	13	,	1	,	51	,	27	,	235	,	349	,	517	,	653	};
            const unsigned long	dim295KuoInit[]		=	{	1	,	3	,	7	,	3	,	27	,	43	,	23	,	129	,	455	,	431	,	1999	};
            const unsigned long	dim296KuoInit[]		=	{	1	,	3	,	7	,	13	,	11	,	3	,	19	,	55	,	261	,	217	,	535	};
            const unsigned long	dim297KuoInit[]		=	{	1	,	3	,	7	,	7	,	23	,	53	,	33	,	237	,	73	,	421	,	1439	};
            const unsigned long	dim298KuoInit[]		=	{	1	,	1	,	5	,	5	,	19	,	7	,	9	,	201	,	339	,	33	,	2019	};
            const unsigned long	dim299KuoInit[]		=	{	1	,	3	,	5	,	7	,	1	,	3	,	9	,	79	,	265	,	125	,	1063	};
            const unsigned long	dim300KuoInit[]		=	{	1	,	1	,	7	,	11	,	31	,	61	,	83	,	71	,	93	,	393	,	851	};
            const unsigned long	dim301KuoInit[]		=	{	1	,	3	,	1	,	13	,	13	,	45	,	65	,	89	,	87	,	657	,	1635	};
            const unsigned long	dim302KuoInit[]		=	{	1	,	3	,	5	,	7	,	21	,	3	,	27	,	251	,	287	,	305	,	1143	};
            const unsigned long	dim303KuoInit[]		=	{	1	,	1	,	3	,	13	,	5	,	45	,	35	,	89	,	61	,	347	,	1349	};
            const unsigned long	dim304KuoInit[]		=	{	1	,	1	,	7	,	1	,	7	,	61	,	19	,	33	,	11	,	845	,	839	};
            const unsigned long	dim305KuoInit[]		=	{	1	,	3	,	5	,	15	,	3	,	1	,	127	,	245	,	423	,	57	,	865	};
            const unsigned long	dim306KuoInit[]		=	{	1	,	3	,	5	,	7	,	31	,	33	,	53	,	109	,	397	,	705	,	307	};
            const unsigned long	dim307KuoInit[]		=	{	1	,	3	,	7	,	7	,	11	,	51	,	53	,	75	,	325	,	61	,	2035	};
            const unsigned long	dim308KuoInit[]		=	{	1	,	3	,	3	,	9	,	23	,	37	,	63	,	115	,	377	,	741	,	1181	};
            const unsigned long	dim309KuoInit[]		=	{	1	,	1	,	1	,	13	,	13	,	7	,	77	,	171	,	129	,	615	,	157	};
            const unsigned long	dim310KuoInit[]		=	{	1	,	1	,	5	,	5	,	9	,	41	,	59	,	215	,	275	,	969	,	31	};
            const unsigned long	dim311KuoInit[]		=	{	1	,	1	,	3	,	1	,	27	,	1	,	33	,	111	,	459	,	503	,	1703	};
            const unsigned long	dim312KuoInit[]		=	{	1	,	3	,	3	,	9	,	11	,	41	,	27	,	213	,	67	,	203	,	497	};
            const unsigned long	dim313KuoInit[]		=	{	1	,	3	,	3	,	3	,	1	,	35	,	39	,	69	,	457	,	765	,	903	};
            const unsigned long	dim314KuoInit[]		=	{	1	,	3	,	5	,	11	,	11	,	31	,	31	,	195	,	481	,	831	,	1727	};
            const unsigned long	dim315KuoInit[]		=	{	1	,	3	,	1	,	1	,	9	,	39	,	11	,	179	,	241	,	945	,	439	};
            const unsigned long	dim316KuoInit[]		=	{	1	,	1	,	5	,	13	,	1	,	29	,	37	,	113	,	193	,	229	,	1639	};
            const unsigned long	dim317KuoInit[]		=	{	1	,	3	,	5	,	7	,	9	,	27	,	75	,	105	,	449	,	631	,	2025	};
            const unsigned long	dim318KuoInit[]		=	{	1	,	3	,	7	,	11	,	7	,	9	,	73	,	201	,	273	,	915	,	729	};
            const unsigned long	dim319KuoInit[]		=	{	1	,	3	,	3	,	15	,	3	,	41	,	61	,	5	,	475	,	193	,	699	};
            const unsigned long	dim320KuoInit[]		=	{	1	,	1	,	3	,	3	,	13	,	57	,	51	,	201	,	313	,	877	,	507	};
            const unsigned long	dim321KuoInit[]		=	{	1	,	1	,	7	,	9	,	7	,	3	,	73	,	103	,	113	,	719	,	1747	};
            const unsigned long	dim322KuoInit[]		=	{	1	,	1	,	7	,	1	,	1	,	25	,	77	,	249	,	185	,	583	,	123	};
            const unsigned long	dim323KuoInit[]		=	{	1	,	1	,	5	,	11	,	25	,	49	,	17	,	45	,	161	,	647	,	1685	};
            const unsigned long	dim324KuoInit[]		=	{	1	,	3	,	1	,	15	,	15	,	49	,	35	,	59	,	47	,	523	,	1961	};
            const unsigned long	dim325KuoInit[]		=	{	1	,	1	,	5	,	5	,	19	,	11	,	105	,	33	,	333	,	451	,	45	};
            const unsigned long	dim326KuoInit[]		=	{	1	,	3	,	1	,	7	,	21	,	1	,	1	,	15	,	465	,	731	,	447	};
            const unsigned long	dim327KuoInit[]		=	{	1	,	3	,	5	,	7	,	3	,	9	,	31	,	205	,	479	,	921	,	1705	};
            const unsigned long	dim328KuoInit[]		=	{	1	,	1	,	7	,	13	,	7	,	29	,	55	,	1	,	293	,	135	,	1459	};
            const unsigned long	dim329KuoInit[]		=	{	1	,	1	,	7	,	5	,	19	,	17	,	7	,	217	,	133	,	959	,	811	};
            const unsigned long	dim330KuoInit[]		=	{	1	,	1	,	1	,	15	,	3	,	37	,	55	,	223	,	253	,	535	,	1039	};
            const unsigned long	dim331KuoInit[]		=	{	1	,	3	,	7	,	3	,	19	,	47	,	125	,	117	,	473	,	257	,	1215	};
            const unsigned long	dim332KuoInit[]		=	{	1	,	1	,	3	,	5	,	5	,	1	,	87	,	95	,	189	,	431	,	1355	};
            const unsigned long	dim333KuoInit[]		=	{	1	,	1	,	5	,	11	,	17	,	5	,	63	,	15	,	153	,	325	,	259	};
            const unsigned long	dim334KuoInit[]		=	{	1	,	1	,	3	,	11	,	9	,	41	,	101	,	21	,	161	,	653	,	1091	};
            const unsigned long	dim335KuoInit[]		=	{	1	,	3	,	3	,	13	,	5	,	19	,	37	,	223	,	469	,	953	,	15	};
            const unsigned long	dim336KuoInit[]		=	{	1	,	1	,	1	,	5	,	3	,	33	,	7	,	63	,	111	,	685	,	381	};		
            const unsigned long	dim337KuoInit[]		=	{	1	,	1	,	3	,	11	,	31	,	27	,	49	,	119	,	277	,	303	,	1719	,	909	};
            const unsigned long	dim338KuoInit[]		=	{	1	,	3	,	1	,	11	,	27	,	13	,	9	,	107	,	251	,	759	,	1641	,	2177	};
            const unsigned long	dim339KuoInit[]		=	{	1	,	3	,	3	,	3	,	3	,	57	,	33	,	169	,	239	,	357	,	1519	,	997	};
            const unsigned long	dim340KuoInit[]		=	{	1	,	3	,	3	,	3	,	11	,	29	,	53	,	109	,	411	,	775	,	1647	,	819	};
            const unsigned long	dim341KuoInit[]		=	{	1	,	1	,	5	,	13	,	9	,	3	,	15	,	219	,	483	,	513	,	1319	,	965	};
            const unsigned long	dim342KuoInit[]		=	{	1	,	1	,	1	,	5	,	3	,	41	,	9	,	53	,	303	,	349	,	1071	,	681	};
            const unsigned long	dim343KuoInit[]		=	{	1	,	3	,	5	,	15	,	27	,	15	,	107	,	211	,	317	,	283	,	1317	,	2661	};
            const unsigned long	dim344KuoInit[]		=	{	1	,	3	,	3	,	1	,	13	,	41	,	29	,	17	,	91	,	985	,	327	,	1637	};
            const unsigned long	dim345KuoInit[]		=	{	1	,	1	,	3	,	15	,	27	,	3	,	127	,	233	,	177	,	877	,	1467	,	3951	};
            const unsigned long	dim346KuoInit[]		=	{	1	,	3	,	1	,	5	,	1	,	51	,	5	,	195	,	501	,	859	,	905	,	3163	};
            const unsigned long	dim347KuoInit[]		=	{	1	,	1	,	1	,	5	,	1	,	25	,	5	,	205	,	65	,	435	,	1645	,	4013	};
            const unsigned long	dim348KuoInit[]		=	{	1	,	3	,	5	,	13	,	29	,	57	,	101	,	59	,	289	,	701	,	1363	,	961	};
            const unsigned long	dim349KuoInit[]		=	{	1	,	3	,	7	,	5	,	1	,	61	,	33	,	117	,	311	,	395	,	1531	,	2491	};
            const unsigned long	dim350KuoInit[]		=	{	1	,	1	,	1	,	1	,	5	,	35	,	15	,	59	,	201	,	195	,	1423	,	2195	};
            const unsigned long	dim351KuoInit[]		=	{	1	,	1	,	7	,	7	,	13	,	29	,	37	,	109	,	481	,	573	,	1347	,	3335	};
            const unsigned long	dim352KuoInit[]		=	{	1	,	1	,	5	,	5	,	25	,	1	,	63	,	53	,	419	,	475	,	1527	,	3327	};
            const unsigned long	dim353KuoInit[]		=	{	1	,	1	,	3	,	1	,	27	,	55	,	75	,	225	,	379	,	471	,	211	,	1655	};
            const unsigned long	dim354KuoInit[]		=	{	1	,	1	,	7	,	15	,	13	,	61	,	111	,	95	,	457	,	801	,	1865	,	2529	};
            const unsigned long	dim355KuoInit[]		=	{	1	,	3	,	5	,	5	,	21	,	3	,	31	,	97	,	3	,	495	,	1953	,	145	};
            const unsigned long	dim356KuoInit[]		=	{	1	,	3	,	3	,	5	,	13	,	27	,	31	,	143	,	133	,	357	,	1975	,	1077	};
            const unsigned long	dim357KuoInit[]		=	{	1	,	3	,	3	,	15	,	19	,	19	,	23	,	221	,	9	,	783	,	1403	,	4003	};
            const unsigned long	dim358KuoInit[]		=	{	1	,	3	,	7	,	3	,	5	,	53	,	63	,	35	,	209	,	221	,	1517	,	1915	};
            const unsigned long	dim359KuoInit[]		=	{	1	,	3	,	3	,	11	,	27	,	21	,	53	,	95	,	359	,	925	,	1035	,	3093	};
            const unsigned long	dim360KuoInit[]		=	{	1	,	1	,	3	,	9	,	9	,	13	,	25	,	125	,	101	,	119	,	1361	,	3503	};
            const unsigned long	dim361KuoInit[]		=	{	1	,	3	,	7	,	11	,	19	,	43	,	125	,	95	,	113	,	373	,	655	,	1001	};
            const unsigned long	dim362KuoInit[]		=	{	1	,	1	,	3	,	15	,	3	,	29	,	125	,	5	,	287	,	859	,	567	,	3451	};
            const unsigned long	dim363KuoInit[]		=	{	1	,	3	,	1	,	1	,	7	,	21	,	67	,	249	,	487	,	663	,	2041	,	799	};
            const unsigned long	dim364KuoInit[]		=	{	1	,	1	,	3	,	9	,	9	,	29	,	11	,	229	,	303	,	821	,	1187	,	287	};
            const unsigned long	dim365KuoInit[]		=	{	1	,	1	,	5	,	11	,	25	,	19	,	77	,	93	,	209	,	155	,	1717	,	4085	};
            const unsigned long	dim366KuoInit[]		=	{	1	,	1	,	3	,	3	,	23	,	39	,	5	,	29	,	295	,	757	,	1185	,	817	};
            const unsigned long	dim367KuoInit[]		=	{	1	,	3	,	7	,	7	,	27	,	25	,	33	,	73	,	489	,	543	,	2011	,	447	};
            const unsigned long	dim368KuoInit[]		=	{	1	,	3	,	1	,	5	,	21	,	59	,	107	,	109	,	415	,	599	,	325	,	1457	};
            const unsigned long	dim369KuoInit[]		=	{	1	,	3	,	1	,	5	,	27	,	61	,	45	,	129	,	37	,	923	,	285	,	2077	};
            const unsigned long	dim370KuoInit[]		=	{	1	,	1	,	1	,	3	,	11	,	11	,	17	,	157	,	289	,	575	,	749	,	2463	};
            const unsigned long	dim371KuoInit[]		=	{	1	,	1	,	3	,	7	,	25	,	1	,	19	,	177	,	361	,	831	,	911	,	1833	};
            const unsigned long	dim372KuoInit[]		=	{	1	,	3	,	7	,	7	,	25	,	27	,	11	,	105	,	489	,	399	,	483	,	1287	};
            const unsigned long	dim373KuoInit[]		=	{	1	,	3	,	7	,	11	,	3	,	49	,	27	,	63	,	33	,	991	,	341	,	2239	};
            const unsigned long	dim374KuoInit[]		=	{	1	,	3	,	1	,	5	,	3	,	49	,	23	,	23	,	157	,	145	,	1709	,	1459	};
            const unsigned long	dim375KuoInit[]		=	{	1	,	1	,	5	,	9	,	29	,	29	,	5	,	239	,	201	,	503	,	229	,	1943	};
            const unsigned long	dim376KuoInit[]		=	{	1	,	3	,	7	,	3	,	5	,	39	,	43	,	127	,	239	,	679	,	1225	,	2123	};
            const unsigned long	dim377KuoInit[]		=	{	1	,	1	,	7	,	11	,	13	,	17	,	125	,	253	,	223	,	67	,	1319	,	4019	};
            const unsigned long	dim378KuoInit[]		=	{	1	,	3	,	3	,	3	,	7	,	57	,	71	,	131	,	205	,	953	,	915	,	1349	};
            const unsigned long	dim379KuoInit[]		=	{	1	,	1	,	1	,	7	,	1	,	59	,	77	,	221	,	227	,	67	,	1853	,	2577	};
            const unsigned long	dim380KuoInit[]		=	{	1	,	1	,	7	,	11	,	31	,	15	,	103	,	51	,	505	,	403	,	1947	,	1915	};
            const unsigned long	dim381KuoInit[]		=	{	1	,	3	,	3	,	3	,	15	,	7	,	41	,	179	,	265	,	787	,	1249	,	3663	};
            const unsigned long	dim382KuoInit[]		=	{	1	,	3	,	1	,	11	,	27	,	53	,	123	,	95	,	427	,	877	,	471	,	2167	};
            const unsigned long	dim383KuoInit[]		=	{	1	,	1	,	1	,	9	,	25	,	7	,	31	,	47	,	21	,	299	,	1397	,	1119	};
            const unsigned long	dim384KuoInit[]		=	{	1	,	3	,	3	,	9	,	15	,	49	,	9	,	255	,	19	,	995	,	837	,	2323	};
            const unsigned long	dim385KuoInit[]		=	{	1	,	1	,	7	,	3	,	31	,	45	,	35	,	93	,	179	,	561	,	1737	,	3969	};
            const unsigned long	dim386KuoInit[]		=	{	1	,	1	,	1	,	5	,	7	,	59	,	27	,	71	,	9	,	411	,	59	,	1783	};
            const unsigned long	dim387KuoInit[]		=	{	1	,	3	,	3	,	7	,	27	,	17	,	43	,	37	,	505	,	893	,	709	,	3163	};
            const unsigned long	dim388KuoInit[]		=	{	1	,	1	,	5	,	7	,	7	,	59	,	47	,	119	,	39	,	153	,	259	,	1409	};
            const unsigned long	dim389KuoInit[]		=	{	1	,	3	,	5	,	1	,	21	,	29	,	23	,	153	,	309	,	137	,	1015	,	1527	};
            const unsigned long	dim390KuoInit[]		=	{	1	,	1	,	5	,	15	,	15	,	27	,	41	,	15	,	135	,	707	,	481	,	2429	};
            const unsigned long	dim391KuoInit[]		=	{	1	,	1	,	7	,	13	,	1	,	17	,	95	,	131	,	497	,	873	,	193	,	1683	};
            const unsigned long	dim392KuoInit[]		=	{	1	,	1	,	1	,	13	,	29	,	39	,	55	,	51	,	163	,	581	,	1867	,	2219	};
            const unsigned long	dim393KuoInit[]		=	{	1	,	1	,	7	,	3	,	21	,	29	,	9	,	111	,	503	,	7	,	89	,	3431	};
            const unsigned long	dim394KuoInit[]		=	{	1	,	3	,	7	,	9	,	13	,	45	,	61	,	239	,	61	,	969	,	77	,	4053	};
            const unsigned long	dim395KuoInit[]		=	{	1	,	1	,	7	,	11	,	13	,	29	,	79	,	39	,	361	,	635	,	1867	,	3051	};
            const unsigned long	dim396KuoInit[]		=	{	1	,	3	,	7	,	3	,	29	,	11	,	81	,	193	,	469	,	365	,	17	,	3657	};
            const unsigned long	dim397KuoInit[]		=	{	1	,	3	,	3	,	15	,	13	,	45	,	69	,	181	,	95	,	315	,	1025	,	1935	};
            const unsigned long	dim398KuoInit[]		=	{	1	,	1	,	5	,	15	,	31	,	61	,	97	,	117	,	207	,	409	,	69	,	187	};
            const unsigned long	dim399KuoInit[]		=	{	1	,	1	,	5	,	9	,	27	,	59	,	123	,	157	,	409	,	999	,	905	,	1097	};
            const unsigned long	dim400KuoInit[]		=	{	1	,	1	,	7	,	1	,	1	,	51	,	99	,	143	,	255	,	817	,	179	,	3317	};
            const unsigned long	dim401KuoInit[]		=	{	1	,	1	,	1	,	1	,	23	,	15	,	117	,	139	,	245	,	865	,	1723	,	3501	};
            const unsigned long	dim402KuoInit[]		=	{	1	,	1	,	5	,	15	,	25	,	53	,	9	,	207	,	291	,	137	,	291	,	653	};
            const unsigned long	dim403KuoInit[]		=	{	1	,	1	,	3	,	9	,	9	,	63	,	55	,	147	,	199	,	455	,	2047	,	2903	};
            const unsigned long	dim404KuoInit[]		=	{	1	,	1	,	5	,	15	,	29	,	55	,	85	,	13	,	117	,	479	,	1371	,	2939	};
            const unsigned long	dim405KuoInit[]		=	{	1	,	3	,	3	,	5	,	17	,	15	,	33	,	83	,	359	,	469	,	463	,	491	};
            const unsigned long	dim406KuoInit[]		=	{	1	,	1	,	3	,	5	,	23	,	39	,	35	,	83	,	387	,	891	,	789	,	1469	};
            const unsigned long	dim407KuoInit[]		=	{	1	,	3	,	7	,	9	,	27	,	49	,	41	,	49	,	207	,	39	,	25	,	2355	};
            const unsigned long	dim408KuoInit[]		=	{	1	,	3	,	5	,	1	,	5	,	7	,	29	,	233	,	169	,	827	,	1631	,	2263	};
            const unsigned long	dim409KuoInit[]		=	{	1	,	1	,	1	,	1	,	1	,	13	,	97	,	109	,	255	,	449	,	345	,	2533	};
            const unsigned long	dim410KuoInit[]		=	{	1	,	3	,	1	,	5	,	3	,	15	,	77	,	217	,	237	,	57	,	1711	,	725	};
            const unsigned long	dim411KuoInit[]		=	{	1	,	3	,	3	,	7	,	9	,	49	,	93	,	213	,	487	,	811	,	1659	,	3019	};
            const unsigned long	dim412KuoInit[]		=	{	1	,	1	,	1	,	7	,	7	,	9	,	57	,	223	,	263	,	9	,	443	,	1103	};
            const unsigned long	dim413KuoInit[]		=	{	1	,	1	,	5	,	11	,	21	,	11	,	83	,	239	,	347	,	489	,	983	,	677	};
            const unsigned long	dim414KuoInit[]		=	{	1	,	1	,	1	,	11	,	9	,	29	,	25	,	105	,	391	,	1001	,	1807	,	1041	};
            const unsigned long	dim415KuoInit[]		=	{	1	,	3	,	3	,	1	,	11	,	21	,	89	,	85	,	259	,	237	,	1489	,	3825	};
            const unsigned long	dim416KuoInit[]		=	{	1	,	3	,	5	,	5	,	1	,	21	,	53	,	127	,	269	,	741	,	529	,	1649	};
            const unsigned long	dim417KuoInit[]		=	{	1	,	3	,	1	,	7	,	3	,	35	,	71	,	65	,	21	,	985	,	111	,	2199	};
            const unsigned long	dim418KuoInit[]		=	{	1	,	1	,	5	,	13	,	3	,	39	,	33	,	151	,	95	,	655	,	1805	,	1545	};
            const unsigned long	dim419KuoInit[]		=	{	1	,	3	,	1	,	11	,	17	,	53	,	63	,	171	,	275	,	649	,	1791	,	3309	};
            const unsigned long	dim420KuoInit[]		=	{	1	,	1	,	7	,	1	,	3	,	5	,	63	,	241	,	77	,	677	,	1713	,	1643	};
            const unsigned long	dim421KuoInit[]		=	{	1	,	3	,	3	,	3	,	9	,	25	,	7	,	33	,	277	,	837	,	1235	,	3289	};
            const unsigned long	dim422KuoInit[]		=	{	1	,	3	,	3	,	1	,	27	,	13	,	97	,	79	,	145	,	197	,	1237	,	1689	};
            const unsigned long	dim423KuoInit[]		=	{	1	,	3	,	5	,	7	,	27	,	45	,	71	,	165	,	405	,	251	,	383	,	737	};
            const unsigned long	dim424KuoInit[]		=	{	1	,	3	,	7	,	1	,	13	,	21	,	107	,	7	,	207	,	329	,	1671	,	2053	};
            const unsigned long	dim425KuoInit[]		=	{	1	,	3	,	7	,	1	,	31	,	33	,	25	,	223	,	37	,	693	,	1831	,	2249	};
            const unsigned long	dim426KuoInit[]		=	{	1	,	3	,	3	,	15	,	5	,	53	,	111	,	11	,	317	,	819	,	1345	,	401	};
            const unsigned long	dim427KuoInit[]		=	{	1	,	3	,	1	,	9	,	7	,	61	,	35	,	127	,	453	,	565	,	37	,	17	};
            const unsigned long	dim428KuoInit[]		=	{	1	,	3	,	1	,	1	,	15	,	3	,	37	,	133	,	339	,	181	,	377	,	2339	};
            const unsigned long	dim429KuoInit[]		=	{	1	,	3	,	7	,	3	,	27	,	57	,	27	,	23	,	45	,	219	,	455	,	2821	};
            const unsigned long	dim430KuoInit[]		=	{	1	,	3	,	7	,	9	,	3	,	43	,	79	,	67	,	15	,	797	,	129	,	1233	};
            const unsigned long	dim431KuoInit[]		=	{	1	,	3	,	5	,	11	,	5	,	45	,	123	,	161	,	355	,	447	,	1165	,	2437	};
            const unsigned long	dim432KuoInit[]		=	{	1	,	3	,	7	,	1	,	3	,	59	,	65	,	109	,	253	,	447	,	649	,	379	};
            const unsigned long	dim433KuoInit[]		=	{	1	,	3	,	1	,	5	,	21	,	23	,	45	,	47	,	213	,	813	,	1475	,	1271	};
            const unsigned long	dim434KuoInit[]		=	{	1	,	1	,	1	,	7	,	9	,	31	,	91	,	47	,	161	,	535	,	1529	,	881	};
            const unsigned long	dim435KuoInit[]		=	{	1	,	1	,	5	,	13	,	1	,	25	,	85	,	107	,	59	,	487	,	221	,	51	};
            const unsigned long	dim436KuoInit[]		=	{	1	,	1	,	1	,	1	,	1	,	55	,	93	,	95	,	267	,	157	,	1255	,	2115	};
            const unsigned long	dim437KuoInit[]		=	{	1	,	3	,	1	,	11	,	23	,	55	,	29	,	187	,	261	,	139	,	515	,	2627	};
            const unsigned long	dim438KuoInit[]		=	{	1	,	3	,	3	,	13	,	31	,	3	,	121	,	245	,	475	,	67	,	195	,	979	};
            const unsigned long	dim439KuoInit[]		=	{	1	,	1	,	3	,	7	,	25	,	13	,	79	,	129	,	475	,	177	,	1519	,	1069	};
            const unsigned long	dim440KuoInit[]		=	{	1	,	3	,	3	,	9	,	17	,	23	,	83	,	191	,	133	,	901	,	1161	,	1657	};
            const unsigned long	dim441KuoInit[]		=	{	1	,	3	,	7	,	15	,	23	,	61	,	111	,	135	,	107	,	27	,	583	,	225	};
            const unsigned long	dim442KuoInit[]		=	{	1	,	1	,	5	,	1	,	17	,	11	,	31	,	197	,	111	,	953	,	1227	,	3737	};
            const unsigned long	dim443KuoInit[]		=	{	1	,	1	,	3	,	9	,	19	,	25	,	83	,	97	,	489	,	781	,	1591	,	2061	};
            const unsigned long	dim444KuoInit[]		=	{	1	,	1	,	1	,	1	,	9	,	61	,	31	,	93	,	39	,	53	,	1613	,	3371	};
            const unsigned long	dim445KuoInit[]		=	{	1	,	1	,	1	,	7	,	3	,	5	,	103	,	247	,	255	,	333	,	31	,	3087	};
            const unsigned long	dim446KuoInit[]		=	{	1	,	1	,	7	,	7	,	17	,	35	,	51	,	153	,	363	,	897	,	395	,	1153	};
            const unsigned long	dim447KuoInit[]		=	{	1	,	3	,	7	,	9	,	3	,	59	,	127	,	177	,	191	,	255	,	1543	,	503	};
            const unsigned long	dim448KuoInit[]		=	{	1	,	1	,	7	,	3	,	1	,	7	,	81	,	199	,	411	,	565	,	1717	,	615	};
            const unsigned long	dim449KuoInit[]		=	{	1	,	1	,	1	,	11	,	17	,	35	,	35	,	245	,	507	,	439	,	523	,	2343	};
            const unsigned long	dim450KuoInit[]		=	{	1	,	1	,	7	,	15	,	13	,	61	,	31	,	99	,	241	,	605	,	1575	,	1321	};
            const unsigned long	dim451KuoInit[]		=	{	1	,	3	,	1	,	5	,	15	,	31	,	45	,	89	,	193	,	905	,	691	,	3917	};
            const unsigned long	dim452KuoInit[]		=	{	1	,	3	,	3	,	9	,	29	,	31	,	101	,	151	,	187	,	199	,	1543	,	3951	};
            const unsigned long	dim453KuoInit[]		=	{	1	,	1	,	7	,	9	,	21	,	45	,	71	,	19	,	395	,	175	,	503	,	3759	};
            const unsigned long	dim454KuoInit[]		=	{	1	,	3	,	7	,	7	,	17	,	31	,	47	,	7	,	135	,	719	,	567	,	1447	};
            const unsigned long	dim455KuoInit[]		=	{	1	,	3	,	3	,	9	,	1	,	39	,	1	,	53	,	5	,	553	,	371	,	3833	};
            const unsigned long	dim456KuoInit[]		=	{	1	,	1	,	1	,	11	,	1	,	33	,	97	,	209	,	377	,	187	,	1683	,	143	};
            const unsigned long	dim457KuoInit[]		=	{	1	,	1	,	1	,	3	,	29	,	33	,	111	,	35	,	343	,	511	,	273	,	445	};
            const unsigned long	dim458KuoInit[]		=	{	1	,	1	,	1	,	15	,	15	,	63	,	73	,	1	,	111	,	775	,	665	,	2737	};
            const unsigned long	dim459KuoInit[]		=	{	1	,	1	,	1	,	9	,	29	,	23	,	127	,	77	,	33	,	131	,	1055	,	2351	};
            const unsigned long	dim460KuoInit[]		=	{	1	,	3	,	5	,	7	,	31	,	53	,	117	,	57	,	287	,	221	,	1099	,	1147	};
            const unsigned long	dim461KuoInit[]		=	{	1	,	3	,	7	,	15	,	3	,	17	,	15	,	177	,	189	,	379	,	179	,	1095	};
            const unsigned long	dim462KuoInit[]		=	{	1	,	3	,	1	,	15	,	25	,	19	,	43	,	113	,	123	,	123	,	877	,	1869	};
            const unsigned long	dim463KuoInit[]		=	{	1	,	3	,	1	,	1	,	13	,	63	,	1	,	83	,	197	,	215	,	1005	,	3531	};
            const unsigned long	dim464KuoInit[]		=	{	1	,	3	,	7	,	1	,	27	,	25	,	111	,	199	,	239	,	909	,	485	,	3399	};
            const unsigned long	dim465KuoInit[]		=	{	1	,	1	,	7	,	3	,	19	,	57	,	43	,	75	,	163	,	111	,	359	,	1987	};
            const unsigned long	dim466KuoInit[]		=	{	1	,	1	,	3	,	7	,	11	,	63	,	79	,	235	,	197	,	757	,	293	,	3795	};
            const unsigned long	dim467KuoInit[]		=	{	1	,	3	,	5	,	9	,	13	,	55	,	49	,	29	,	133	,	949	,	1185	,	905	};
            const unsigned long	dim468KuoInit[]		=	{	1	,	1	,	7	,	1	,	15	,	51	,	13	,	223	,	243	,	831	,	845	,	2663	};
            const unsigned long	dim469KuoInit[]		=	{	1	,	1	,	3	,	11	,	25	,	21	,	65	,	57	,	483	,	665	,	741	,	2713	};
            const unsigned long	dim470KuoInit[]		=	{	1	,	3	,	5	,	3	,	21	,	3	,	81	,	57	,	133	,	553	,	683	,	425	};
            const unsigned long	dim471KuoInit[]		=	{	1	,	1	,	5	,	11	,	31	,	29	,	59	,	189	,	399	,	751	,	651	,	4093	};
            const unsigned long	dim472KuoInit[]		=	{	1	,	3	,	3	,	7	,	13	,	59	,	19	,	217	,	267	,	355	,	173	,	4051	};
            const unsigned long	dim473KuoInit[]		=	{	1	,	1	,	5	,	9	,	23	,	49	,	57	,	145	,	77	,	591	,	1913	,	779	};
            const unsigned long	dim474KuoInit[]		=	{	1	,	3	,	7	,	3	,	25	,	1	,	83	,	103	,	393	,	121	,	1415	,	1503	};
            const unsigned long	dim475KuoInit[]		=	{	1	,	1	,	7	,	9	,	23	,	41	,	35	,	157	,	63	,	293	,	1911	,	2303	};
            const unsigned long	dim476KuoInit[]		=	{	1	,	3	,	5	,	15	,	7	,	3	,	9	,	1	,	201	,	913	,	1275	,	3495	};
            const unsigned long	dim477KuoInit[]		=	{	1	,	1	,	7	,	11	,	15	,	5	,	107	,	57	,	53	,	451	,	351	,	2621	};
            const unsigned long	dim478KuoInit[]		=	{	1	,	3	,	7	,	15	,	23	,	25	,	97	,	29	,	111	,	531	,	1979	,	2115	};
            const unsigned long	dim479KuoInit[]		=	{	1	,	1	,	1	,	1	,	19	,	11	,	49	,	229	,	355	,	195	,	2033	,	1701	};
            const unsigned long	dim480KuoInit[]		=	{	1	,	3	,	3	,	3	,	15	,	35	,	17	,	13	,	425	,	285	,	965	,	159	};		
            const unsigned long	dim481KuoInit[]		=	{	1	,	3	,	1	,	11	,	11	,	11	,	1	,	145	,	365	,	471	,	1527	,	2309	,	5863	};
            const unsigned long	dim482KuoInit[]		=	{	1	,	1	,	3	,	9	,	21	,	59	,	35	,	123	,	91	,	741	,	527	,	2061	,	5505	};
            const unsigned long	dim483KuoInit[]		=	{	1	,	3	,	7	,	9	,	9	,	37	,	75	,	171	,	75	,	943	,	443	,	201	,	4861	};
            const unsigned long	dim484KuoInit[]		=	{	1	,	1	,	7	,	13	,	23	,	49	,	67	,	103	,	453	,	643	,	1117	,	1621	,	7135	};
            const unsigned long	dim485KuoInit[]		=	{	1	,	3	,	1	,	7	,	15	,	43	,	113	,	17	,	277	,	301	,	349	,	1155	,	5285	};
            const unsigned long	dim486KuoInit[]		=	{	1	,	3	,	5	,	1	,	25	,	61	,	103	,	81	,	141	,	347	,	1467	,	1535	,	421	};
            const unsigned long	dim487KuoInit[]		=	{	1	,	3	,	5	,	13	,	29	,	45	,	43	,	31	,	403	,	711	,	455	,	3387	,	3521	};
            const unsigned long	dim488KuoInit[]		=	{	1	,	3	,	3	,	7	,	7	,	41	,	71	,	251	,	147	,	241	,	1129	,	2093	,	7683	};
            const unsigned long	dim489KuoInit[]		=	{	1	,	1	,	1	,	5	,	23	,	61	,	57	,	97	,	177	,	387	,	1557	,	3587	,	751	};
            const unsigned long	dim490KuoInit[]		=	{	1	,	3	,	1	,	13	,	11	,	33	,	47	,	147	,	321	,	661	,	53	,	2437	,	3087	};
            const unsigned long	dim491KuoInit[]		=	{	1	,	3	,	1	,	3	,	17	,	21	,	79	,	91	,	77	,	1011	,	745	,	3551	,	1857	};
            const unsigned long	dim492KuoInit[]		=	{	1	,	1	,	3	,	15	,	19	,	27	,	63	,	231	,	267	,	879	,	1959	,	3019	,	1655	};
            const unsigned long	dim493KuoInit[]		=	{	1	,	1	,	7	,	15	,	27	,	55	,	21	,	37	,	417	,	69	,	327	,	2709	,	6335	};
            const unsigned long	dim494KuoInit[]		=	{	1	,	3	,	1	,	15	,	1	,	9	,	27	,	89	,	205	,	355	,	373	,	2191	,	311	};
            const unsigned long	dim495KuoInit[]		=	{	1	,	1	,	3	,	5	,	15	,	51	,	85	,	233	,	21	,	971	,	1823	,	1843	,	1417	};
            const unsigned long	dim496KuoInit[]		=	{	1	,	3	,	5	,	13	,	31	,	57	,	3	,	199	,	211	,	401	,	1559	,	3601	,	4569	};
            const unsigned long	dim497KuoInit[]		=	{	1	,	3	,	3	,	1	,	27	,	63	,	93	,	123	,	223	,	601	,	1023	,	2501	,	7105	};
            const unsigned long	dim498KuoInit[]		=	{	1	,	1	,	1	,	7	,	3	,	31	,	5	,	51	,	453	,	535	,	255	,	735	,	749	};
            const unsigned long	dim499KuoInit[]		=	{	1	,	3	,	5	,	9	,	23	,	63	,	109	,	163	,	221	,	845	,	911	,	933	,	361	};
            const unsigned long	dim500KuoInit[]		=	{	1	,	1	,	1	,	13	,	13	,	17	,	19	,	191	,	227	,	711	,	1523	,	1635	,	6397	};
            const unsigned long	dim501KuoInit[]		=	{	1	,	3	,	7	,	7	,	21	,	21	,	23	,	91	,	439	,	227	,	1831	,	1475	,	705	};
            const unsigned long	dim502KuoInit[]		=	{	1	,	3	,	7	,	7	,	13	,	39	,	99	,	199	,	149	,	1005	,	149	,	445	,	3367	};
            const unsigned long	dim503KuoInit[]		=	{	1	,	3	,	1	,	5	,	25	,	61	,	99	,	187	,	33	,	619	,	1833	,	2177	,	7631	};
            const unsigned long	dim504KuoInit[]		=	{	1	,	1	,	1	,	13	,	15	,	45	,	1	,	225	,	129	,	113	,	2009	,	1223	,	49	};
            const unsigned long	dim505KuoInit[]		=	{	1	,	3	,	1	,	9	,	7	,	43	,	57	,	193	,	409	,	265	,	1151	,	2635	,	6005	};
            const unsigned long	dim506KuoInit[]		=	{	1	,	3	,	5	,	5	,	13	,	13	,	99	,	163	,	1	,	629	,	87	,	925	,	137	};
            const unsigned long	dim507KuoInit[]		=	{	1	,	3	,	7	,	9	,	23	,	43	,	43	,	1	,	505	,	179	,	335	,	2033	,	8065	};
            const unsigned long	dim508KuoInit[]		=	{	1	,	3	,	5	,	1	,	7	,	55	,	99	,	159	,	19	,	883	,	1503	,	801	,	1433	};
            const unsigned long	dim509KuoInit[]		=	{	1	,	3	,	7	,	1	,	1	,	55	,	53	,	241	,	95	,	315	,	723	,	3571	,	4685	};
            const unsigned long	dim510KuoInit[]		=	{	1	,	3	,	5	,	15	,	29	,	43	,	97	,	159	,	427	,	87	,	1995	,	1849	,	6771	};
            const unsigned long	dim511KuoInit[]		=	{	1	,	1	,	1	,	7	,	5	,	55	,	55	,	203	,	119	,	185	,	1333	,	2107	,	4271	};
            const unsigned long	dim512KuoInit[]		=	{	1	,	1	,	3	,	15	,	5	,	7	,	19	,	185	,	75	,	601	,	2015	,	2175	,	4259	};
            const unsigned long	dim513KuoInit[]		=	{	1	,	3	,	3	,	15	,	3	,	49	,	33	,	115	,	413	,	321	,	1787	,	469	,	3721	};
            const unsigned long	dim514KuoInit[]		=	{	1	,	3	,	1	,	9	,	25	,	63	,	123	,	17	,	355	,	903	,	1269	,	3737	,	5333	};
            const unsigned long	dim515KuoInit[]		=	{	1	,	1	,	3	,	7	,	25	,	41	,	113	,	239	,	373	,	533	,	1823	,	3513	,	7745	};
            const unsigned long	dim516KuoInit[]		=	{	1	,	1	,	1	,	11	,	13	,	7	,	7	,	43	,	145	,	607	,	2043	,	2397	,	4051	};
            const unsigned long	dim517KuoInit[]		=	{	1	,	1	,	3	,	3	,	9	,	49	,	5	,	173	,	63	,	157	,	637	,	2307	,	6603	};
            const unsigned long	dim518KuoInit[]		=	{	1	,	3	,	1	,	3	,	23	,	33	,	89	,	139	,	207	,	389	,	1389	,	3385	,	2169	};
            const unsigned long	dim519KuoInit[]		=	{	1	,	1	,	7	,	3	,	7	,	7	,	19	,	7	,	217	,	189	,	739	,	2475	,	7925	};
            const unsigned long	dim520KuoInit[]		=	{	1	,	1	,	7	,	5	,	23	,	37	,	71	,	31	,	145	,	29	,	537	,	959	,	2227	};
            const unsigned long	dim521KuoInit[]		=	{	1	,	1	,	7	,	11	,	7	,	55	,	39	,	167	,	347	,	293	,	577	,	3077	,	531	};
            const unsigned long	dim522KuoInit[]		=	{	1	,	3	,	1	,	5	,	19	,	1	,	119	,	133	,	487	,	867	,	1331	,	1759	,	3683	};
            const unsigned long	dim523KuoInit[]		=	{	1	,	3	,	5	,	13	,	27	,	31	,	77	,	71	,	465	,	133	,	105	,	477	,	6301	};
            const unsigned long	dim524KuoInit[]		=	{	1	,	3	,	5	,	15	,	25	,	49	,	35	,	75	,	57	,	663	,	195	,	1041	,	5697	};
            const unsigned long	dim525KuoInit[]		=	{	1	,	3	,	7	,	9	,	9	,	7	,	47	,	233	,	397	,	155	,	439	,	2487	,	4697	};
            const unsigned long	dim526KuoInit[]		=	{	1	,	1	,	5	,	9	,	5	,	41	,	125	,	9	,	477	,	567	,	2033	,	769	,	1553	};
            const unsigned long	dim527KuoInit[]		=	{	1	,	1	,	3	,	7	,	7	,	25	,	95	,	45	,	211	,	453	,	483	,	3965	,	1865	};
            const unsigned long	dim528KuoInit[]		=	{	1	,	3	,	7	,	5	,	17	,	41	,	109	,	59	,	385	,	1023	,	1527	,	2445	,	6303	};
            const unsigned long	dim529KuoInit[]		=	{	1	,	3	,	3	,	9	,	7	,	29	,	91	,	245	,	189	,	251	,	1237	,	2825	,	7919	};
            const unsigned long	dim530KuoInit[]		=	{	1	,	3	,	7	,	1	,	25	,	43	,	27	,	11	,	63	,	641	,	1765	,	1683	,	2445	};
            const unsigned long	dim531KuoInit[]		=	{	1	,	1	,	1	,	7	,	3	,	59	,	19	,	81	,	387	,	207	,	31	,	1583	,	607	};
            const unsigned long	dim532KuoInit[]		=	{	1	,	1	,	3	,	1	,	3	,	33	,	99	,	135	,	267	,	355	,	585	,	4017	,	2385	};
            const unsigned long	dim533KuoInit[]		=	{	1	,	1	,	7	,	15	,	1	,	49	,	35	,	211	,	359	,	379	,	1173	,	2709	,	3945	};
            const unsigned long	dim534KuoInit[]		=	{	1	,	1	,	5	,	7	,	29	,	23	,	99	,	219	,	269	,	977	,	1175	,	2305	,	5633	};
            const unsigned long	dim535KuoInit[]		=	{	1	,	3	,	3	,	15	,	17	,	9	,	115	,	21	,	503	,	329	,	1725	,	289	,	4667	};
            const unsigned long	dim536KuoInit[]		=	{	1	,	1	,	7	,	1	,	9	,	1	,	31	,	145	,	209	,	833	,	1527	,	4021	,	2177	};
            const unsigned long	dim537KuoInit[]		=	{	1	,	3	,	3	,	1	,	25	,	13	,	37	,	143	,	217	,	891	,	1289	,	3635	,	6931	};
            const unsigned long	dim538KuoInit[]		=	{	1	,	1	,	3	,	9	,	9	,	49	,	33	,	123	,	435	,	993	,	389	,	2491	,	2097	};
            const unsigned long	dim539KuoInit[]		=	{	1	,	3	,	5	,	9	,	11	,	53	,	121	,	25	,	31	,	589	,	1939	,	3981	,	621	};
            const unsigned long	dim540KuoInit[]		=	{	1	,	1	,	1	,	1	,	27	,	53	,	9	,	217	,	135	,	605	,	1779	,	3053	,	2865	};
            const unsigned long	dim541KuoInit[]		=	{	1	,	1	,	7	,	5	,	3	,	53	,	9	,	15	,	275	,	613	,	1395	,	2287	,	1111	};
            const unsigned long	dim542KuoInit[]		=	{	1	,	1	,	3	,	1	,	13	,	17	,	3	,	103	,	195	,	509	,	493	,	2395	,	4669	};
            const unsigned long	dim543KuoInit[]		=	{	1	,	3	,	5	,	1	,	13	,	17	,	59	,	3	,	117	,	39	,	1699	,	2339	,	5203	};
            const unsigned long	dim544KuoInit[]		=	{	1	,	3	,	5	,	3	,	29	,	23	,	59	,	251	,	139	,	625	,	935	,	785	,	7547	};
            const unsigned long	dim545KuoInit[]		=	{	1	,	3	,	1	,	13	,	23	,	47	,	5	,	91	,	283	,	59	,	919	,	2427	,	1631	};
            const unsigned long	dim546KuoInit[]		=	{	1	,	1	,	3	,	9	,	15	,	35	,	67	,	131	,	183	,	891	,	345	,	3741	,	3175	};
            const unsigned long	dim547KuoInit[]		=	{	1	,	3	,	1	,	3	,	23	,	11	,	85	,	135	,	269	,	347	,	713	,	2947	,	1617	};
            const unsigned long	dim548KuoInit[]		=	{	1	,	3	,	1	,	3	,	27	,	33	,	77	,	189	,	243	,	893	,	283	,	885	,	1999	};
            const unsigned long	dim549KuoInit[]		=	{	1	,	1	,	5	,	1	,	31	,	39	,	99	,	21	,	437	,	979	,	919	,	3663	,	6689	};
            const unsigned long	dim550KuoInit[]		=	{	1	,	1	,	1	,	1	,	17	,	27	,	55	,	161	,	205	,	583	,	383	,	1681	,	6445	};
            const unsigned long	dim551KuoInit[]		=	{	1	,	3	,	7	,	1	,	31	,	59	,	111	,	129	,	121	,	555	,	653	,	2787	,	3899	};
            const unsigned long	dim552KuoInit[]		=	{	1	,	3	,	1	,	9	,	11	,	59	,	51	,	115	,	349	,	809	,	513	,	1585	,	31	};
            const unsigned long	dim553KuoInit[]		=	{	1	,	1	,	1	,	3	,	29	,	17	,	3	,	173	,	111	,	671	,	1289	,	397	,	799	};
            const unsigned long	dim554KuoInit[]		=	{	1	,	3	,	1	,	7	,	17	,	55	,	85	,	49	,	23	,	915	,	1929	,	685	,	7021	};
            const unsigned long	dim555KuoInit[]		=	{	1	,	3	,	7	,	11	,	3	,	41	,	123	,	227	,	111	,	805	,	1567	,	751	,	7927	};
            const unsigned long	dim556KuoInit[]		=	{	1	,	1	,	7	,	3	,	15	,	45	,	117	,	39	,	491	,	401	,	1775	,	2197	,	4303	};
            const unsigned long	dim557KuoInit[]		=	{	1	,	1	,	7	,	9	,	13	,	23	,	63	,	245	,	163	,	595	,	1271	,	2143	,	8105	};
            const unsigned long	dim558KuoInit[]		=	{	1	,	1	,	3	,	1	,	31	,	57	,	45	,	35	,	327	,	89	,	1845	,	1075	,	4649	};
            const unsigned long	dim559KuoInit[]		=	{	1	,	3	,	7	,	7	,	11	,	31	,	51	,	209	,	103	,	215	,	1693	,	3427	,	677	};
            const unsigned long	dim560KuoInit[]		=	{	1	,	1	,	5	,	13	,	3	,	7	,	83	,	199	,	325	,	461	,	1185	,	1997	,	2327	};
            const unsigned long	dim561KuoInit[]		=	{	1	,	1	,	3	,	15	,	23	,	13	,	23	,	39	,	209	,	267	,	1995	,	3999	,	6277	};
            const unsigned long	dim562KuoInit[]		=	{	1	,	1	,	7	,	1	,	5	,	37	,	99	,	117	,	509	,	303	,	655	,	935	,	6321	};
            const unsigned long	dim563KuoInit[]		=	{	1	,	1	,	1	,	15	,	23	,	39	,	123	,	11	,	445	,	897	,	1327	,	2819	,	1899	};
            const unsigned long	dim564KuoInit[]		=	{	1	,	1	,	5	,	5	,	7	,	7	,	7	,	61	,	349	,	865	,	707	,	2097	,	1217	};
            const unsigned long	dim565KuoInit[]		=	{	1	,	1	,	5	,	9	,	1	,	63	,	35	,	187	,	207	,	53	,	1917	,	621	,	2113	};
            const unsigned long	dim566KuoInit[]		=	{	1	,	1	,	1	,	15	,	21	,	61	,	101	,	169	,	129	,	813	,	453	,	323	,	7283	};
            const unsigned long	dim567KuoInit[]		=	{	1	,	1	,	7	,	7	,	27	,	1	,	27	,	141	,	33	,	307	,	2023	,	2561	,	8111	};
            const unsigned long	dim568KuoInit[]		=	{	1	,	1	,	5	,	13	,	3	,	9	,	3	,	87	,	451	,	691	,	973	,	539	,	4393	};
            const unsigned long	dim569KuoInit[]		=	{	1	,	1	,	5	,	1	,	23	,	23	,	9	,	101	,	349	,	299	,	583	,	2037	,	5247	};
            const unsigned long	dim570KuoInit[]		=	{	1	,	1	,	5	,	11	,	15	,	39	,	99	,	163	,	395	,	123	,	1067	,	2735	,	4407	};
            const unsigned long	dim571KuoInit[]		=	{	1	,	3	,	5	,	11	,	9	,	61	,	51	,	237	,	483	,	501	,	1767	,	1419	,	5047	};
            const unsigned long	dim572KuoInit[]		=	{	1	,	1	,	7	,	9	,	27	,	31	,	57	,	103	,	291	,	537	,	1759	,	1861	,	3815	};
            const unsigned long	dim573KuoInit[]		=	{	1	,	1	,	5	,	15	,	31	,	5	,	107	,	103	,	159	,	939	,	1331	,	1219	,	2045	};
            const unsigned long	dim574KuoInit[]		=	{	1	,	3	,	5	,	15	,	7	,	45	,	99	,	175	,	131	,	967	,	593	,	3191	,	5403	};
            const unsigned long	dim575KuoInit[]		=	{	1	,	3	,	7	,	11	,	19	,	43	,	57	,	129	,	285	,	943	,	1507	,	3979	,	2807	};
            const unsigned long	dim576KuoInit[]		=	{	1	,	1	,	5	,	3	,	11	,	7	,	117	,	241	,	393	,	1023	,	95	,	535	,	7235	};
            const unsigned long	dim577KuoInit[]		=	{	1	,	3	,	1	,	5	,	17	,	5	,	111	,	165	,	343	,	65	,	1393	,	2855	,	3801	};
            const unsigned long	dim578KuoInit[]		=	{	1	,	1	,	5	,	5	,	27	,	1	,	87	,	9	,	177	,	455	,	1915	,	1557	,	7175	};
            const unsigned long	dim579KuoInit[]		=	{	1	,	1	,	5	,	11	,	31	,	33	,	105	,	255	,	281	,	303	,	1831	,	1575	,	1179	};
            const unsigned long	dim580KuoInit[]		=	{	1	,	3	,	3	,	9	,	7	,	37	,	3	,	85	,	161	,	491	,	299	,	4003	,	7429	};
            const unsigned long	dim581KuoInit[]		=	{	1	,	1	,	5	,	9	,	1	,	19	,	23	,	175	,	497	,	613	,	2043	,	1467	,	2379	};
            const unsigned long	dim582KuoInit[]		=	{	1	,	3	,	7	,	13	,	5	,	57	,	93	,	17	,	383	,	801	,	1465	,	3151	,	4717	};
            const unsigned long	dim583KuoInit[]		=	{	1	,	3	,	7	,	5	,	3	,	19	,	87	,	25	,	95	,	643	,	79	,	123	,	7175	};
            const unsigned long	dim584KuoInit[]		=	{	1	,	1	,	1	,	5	,	3	,	27	,	49	,	155	,	45	,	655	,	1079	,	3243	,	8147	};
            const unsigned long	dim585KuoInit[]		=	{	1	,	3	,	1	,	11	,	13	,	57	,	107	,	37	,	147	,	119	,	887	,	1317	,	6687	};
            const unsigned long	dim586KuoInit[]		=	{	1	,	3	,	3	,	13	,	29	,	23	,	105	,	137	,	339	,	509	,	1543	,	2339	,	2167	};
            const unsigned long	dim587KuoInit[]		=	{	1	,	1	,	5	,	3	,	29	,	5	,	89	,	65	,	191	,	33	,	755	,	737	,	3495	};
            const unsigned long	dim588KuoInit[]		=	{	1	,	1	,	5	,	1	,	19	,	27	,	123	,	67	,	323	,	731	,	1753	,	1773	,	1731	};
            const unsigned long	dim589KuoInit[]		=	{	1	,	1	,	5	,	11	,	23	,	35	,	21	,	1	,	107	,	509	,	1617	,	1565	,	1617	};
            const unsigned long	dim590KuoInit[]		=	{	1	,	1	,	7	,	15	,	31	,	7	,	31	,	241	,	479	,	457	,	1353	,	1531	,	5939	};
            const unsigned long	dim591KuoInit[]		=	{	1	,	3	,	1	,	1	,	31	,	19	,	71	,	127	,	379	,	151	,	739	,	893	,	7857	};
            const unsigned long	dim592KuoInit[]		=	{	1	,	1	,	1	,	3	,	31	,	41	,	99	,	55	,	229	,	585	,	1881	,	2267	,	3555	};
            const unsigned long	dim593KuoInit[]		=	{	1	,	3	,	3	,	1	,	25	,	3	,	55	,	95	,	409	,	429	,	527	,	611	,	6551	};
            const unsigned long	dim594KuoInit[]		=	{	1	,	3	,	5	,	3	,	3	,	59	,	81	,	83	,	131	,	981	,	67	,	295	,	3313	};
            const unsigned long	dim595KuoInit[]		=	{	1	,	1	,	1	,	13	,	23	,	57	,	47	,	127	,	307	,	621	,	865	,	2589	,	4033	};
            const unsigned long	dim596KuoInit[]		=	{	1	,	1	,	7	,	11	,	7	,	17	,	15	,	207	,	89	,	77	,	1923	,	3649	,	119	};
            const unsigned long	dim597KuoInit[]		=	{	1	,	3	,	5	,	3	,	23	,	5	,	87	,	247	,	233	,	43	,	475	,	999	,	6999	};
            const unsigned long	dim598KuoInit[]		=	{	1	,	1	,	7	,	13	,	19	,	49	,	53	,	21	,	105	,	181	,	1569	,	3151	,	5089	};
            const unsigned long	dim599KuoInit[]		=	{	1	,	1	,	5	,	7	,	11	,	53	,	119	,	11	,	301	,	647	,	759	,	2849	,	4541	};
            const unsigned long	dim600KuoInit[]		=	{	1	,	1	,	7	,	11	,	31	,	31	,	83	,	107	,	373	,	41	,	921	,	3935	,	2859	};
            const unsigned long	dim601KuoInit[]		=	{	1	,	3	,	5	,	13	,	13	,	57	,	121	,	133	,	355	,	153	,	607	,	3199	,	8097	};
            const unsigned long	dim602KuoInit[]		=	{	1	,	1	,	5	,	9	,	5	,	25	,	113	,	21	,	17	,	643	,	415	,	2705	,	903	};
            const unsigned long	dim603KuoInit[]		=	{	1	,	1	,	5	,	1	,	19	,	3	,	119	,	199	,	241	,	957	,	1203	,	1291	,	6355	};
            const unsigned long	dim604KuoInit[]		=	{	1	,	1	,	3	,	13	,	19	,	1	,	11	,	73	,	387	,	97	,	783	,	3875	,	4573	};
            const unsigned long	dim605KuoInit[]		=	{	1	,	1	,	7	,	3	,	27	,	11	,	99	,	193	,	311	,	889	,	535	,	801	,	4467	};
            const unsigned long	dim606KuoInit[]		=	{	1	,	1	,	1	,	1	,	3	,	53	,	61	,	245	,	497	,	127	,	1213	,	1147	,	2225	};
            const unsigned long	dim607KuoInit[]		=	{	1	,	3	,	5	,	1	,	9	,	49	,	21	,	229	,	481	,	23	,	47	,	3959	,	2991	};
            const unsigned long	dim608KuoInit[]		=	{	1	,	1	,	5	,	1	,	9	,	21	,	123	,	107	,	359	,	759	,	993	,	695	,	5609	};
            const unsigned long	dim609KuoInit[]		=	{	1	,	3	,	5	,	11	,	25	,	55	,	73	,	245	,	373	,	525	,	1399	,	1727	,	1763	};
            const unsigned long	dim610KuoInit[]		=	{	1	,	1	,	5	,	3	,	21	,	41	,	83	,	161	,	43	,	563	,	1655	,	3539	,	7941	};
            const unsigned long	dim611KuoInit[]		=	{	1	,	3	,	1	,	13	,	29	,	59	,	25	,	119	,	199	,	375	,	1507	,	2095	,	3281	};
            const unsigned long	dim612KuoInit[]		=	{	1	,	3	,	3	,	5	,	21	,	23	,	63	,	85	,	115	,	77	,	745	,	771	,	3319	};
            const unsigned long	dim613KuoInit[]		=	{	1	,	1	,	7	,	3	,	3	,	37	,	11	,	233	,	173	,	51	,	1723	,	1441	,	6529	};
            const unsigned long	dim614KuoInit[]		=	{	1	,	1	,	1	,	11	,	1	,	55	,	113	,	25	,	369	,	1023	,	1691	,	1797	,	159	};
            const unsigned long	dim615KuoInit[]		=	{	1	,	3	,	1	,	15	,	13	,	43	,	63	,	113	,	55	,	351	,	1459	,	3117	,	3499	};
            const unsigned long	dim616KuoInit[]		=	{	1	,	3	,	7	,	1	,	19	,	61	,	17	,	243	,	203	,	349	,	1069	,	105	,	6015	};
            const unsigned long	dim617KuoInit[]		=	{	1	,	1	,	5	,	1	,	23	,	49	,	7	,	129	,	99	,	377	,	1989	,	531	,	6315	};
            const unsigned long	dim618KuoInit[]		=	{	1	,	1	,	7	,	7	,	11	,	23	,	107	,	63	,	163	,	351	,	1385	,	127	,	7439	};
            const unsigned long	dim619KuoInit[]		=	{	1	,	3	,	5	,	15	,	15	,	43	,	81	,	229	,	21	,	179	,	1357	,	3813	,	7345	};
            const unsigned long	dim620KuoInit[]		=	{	1	,	1	,	1	,	15	,	23	,	19	,	89	,	251	,	161	,	443	,	983	,	1059	,	1241	};
            const unsigned long	dim621KuoInit[]		=	{	1	,	3	,	3	,	9	,	7	,	17	,	11	,	239	,	325	,	117	,	893	,	917	,	6327	};
            const unsigned long	dim622KuoInit[]		=	{	1	,	1	,	7	,	1	,	31	,	33	,	23	,	241	,	255	,	991	,	903	,	3973	,	3935	};
            const unsigned long	dim623KuoInit[]		=	{	1	,	3	,	7	,	3	,	11	,	17	,	61	,	219	,	461	,	437	,	1843	,	1845	,	771	};
            const unsigned long	dim624KuoInit[]		=	{	1	,	3	,	7	,	5	,	7	,	61	,	79	,	225	,	215	,	411	,	885	,	3949	,	2757	};
            const unsigned long	dim625KuoInit[]		=	{	1	,	3	,	7	,	3	,	3	,	15	,	25	,	49	,	351	,	671	,	1573	,	3643	,	2383	};
            const unsigned long	dim626KuoInit[]		=	{	1	,	1	,	7	,	1	,	29	,	1	,	121	,	61	,	49	,	833	,	1363	,	427	,	3237	};
            const unsigned long	dim627KuoInit[]		=	{	1	,	3	,	3	,	5	,	11	,	9	,	79	,	113	,	341	,	25	,	1797	,	1641	,	6899	};
            const unsigned long	dim628KuoInit[]		=	{	1	,	1	,	5	,	9	,	11	,	15	,	71	,	169	,	269	,	359	,	1991	,	3673	,	601	};
            const unsigned long	dim629KuoInit[]		=	{	1	,	1	,	1	,	11	,	31	,	7	,	5	,	165	,	23	,	917	,	617	,	4059	,	2599	};
            const unsigned long	dim630KuoInit[]		=	{	1	,	1	,	7	,	9	,	19	,	25	,	119	,	103	,	115	,	247	,	1265	,	859	,	5543	};
            const unsigned long	dim631KuoInit[]		=	{	1	,	1	,	7	,	7	,	15	,	27	,	45	,	29	,	67	,	989	,	753	,	3215	,	6281	};
            const unsigned long	dim632KuoInit[]		=	{	1	,	3	,	1	,	7	,	11	,	15	,	35	,	39	,	61	,	483	,	1427	,	193	,	3469	};
            const unsigned long	dim633KuoInit[]		=	{	1	,	3	,	5	,	9	,	21	,	41	,	21	,	223	,	233	,	341	,	177	,	1167	,	3101	};
            const unsigned long	dim634KuoInit[]		=	{	1	,	3	,	3	,	5	,	19	,	25	,	53	,	247	,	29	,	909	,	2047	,	1683	,	4433	};
            const unsigned long	dim635KuoInit[]		=	{	1	,	3	,	3	,	9	,	31	,	7	,	71	,	85	,	501	,	447	,	113	,	3465	,	3445	};
            const unsigned long	dim636KuoInit[]		=	{	1	,	1	,	3	,	15	,	19	,	9	,	127	,	69	,	429	,	451	,	531	,	1319	,	507	};
            const unsigned long	dim637KuoInit[]		=	{	1	,	3	,	1	,	3	,	29	,	1	,	53	,	63	,	267	,	293	,	333	,	2749	,	1785	};
            const unsigned long	dim638KuoInit[]		=	{	1	,	3	,	1	,	7	,	5	,	43	,	71	,	165	,	325	,	309	,	921	,	2277	,	6423	};
            const unsigned long	dim639KuoInit[]		=	{	1	,	1	,	7	,	5	,	23	,	31	,	63	,	35	,	221	,	733	,	1901	,	153	,	3577	};
            const unsigned long	dim640KuoInit[]		=	{	1	,	3	,	7	,	11	,	7	,	5	,	39	,	141	,	293	,	237	,	501	,	1759	,	7763	};
            const unsigned long	dim641KuoInit[]		=	{	1	,	1	,	3	,	15	,	1	,	49	,	5	,	7	,	239	,	1021	,	1215	,	3857	,	4637	};
            const unsigned long	dim642KuoInit[]		=	{	1	,	1	,	5	,	1	,	1	,	31	,	77	,	243	,	155	,	505	,	855	,	3117	,	815	};
            const unsigned long	dim643KuoInit[]		=	{	1	,	1	,	7	,	3	,	3	,	33	,	55	,	249	,	185	,	443	,	543	,	1333	,	5041	};
            const unsigned long	dim644KuoInit[]		=	{	1	,	3	,	5	,	11	,	13	,	13	,	83	,	55	,	443	,	723	,	135	,	3073	,	4215	};
            const unsigned long	dim645KuoInit[]		=	{	1	,	3	,	5	,	9	,	15	,	5	,	13	,	209	,	313	,	891	,	431	,	4009	,	7531	};
            const unsigned long	dim646KuoInit[]		=	{	1	,	1	,	5	,	15	,	13	,	37	,	91	,	117	,	321	,	791	,	2045	,	4073	,	6787	};
            const unsigned long	dim647KuoInit[]		=	{	1	,	1	,	3	,	7	,	1	,	43	,	61	,	121	,	393	,	797	,	1347	,	491	,	341	};
            const unsigned long	dim648KuoInit[]		=	{	1	,	1	,	7	,	3	,	1	,	31	,	63	,	205	,	43	,	359	,	325	,	3329	,	4961	};
            const unsigned long	dim649KuoInit[]		=	{	1	,	1	,	7	,	11	,	27	,	21	,	83	,	151	,	97	,	45	,	1839	,	3221	,	2575	};
            const unsigned long	dim650KuoInit[]		=	{	1	,	3	,	7	,	3	,	13	,	7	,	97	,	89	,	277	,	351	,	1493	,	1161	,	6629	};
            const unsigned long	dim651KuoInit[]		=	{	1	,	3	,	7	,	11	,	27	,	17	,	19	,	211	,	373	,	73	,	1325	,	183	,	1073	};
            const unsigned long	dim652KuoInit[]		=	{	1	,	3	,	3	,	13	,	9	,	23	,	111	,	235	,	153	,	857	,	1669	,	3423	,	3817	};
            const unsigned long	dim653KuoInit[]		=	{	1	,	1	,	1	,	5	,	25	,	63	,	99	,	11	,	283	,	755	,	999	,	2795	,	7795	};
            const unsigned long	dim654KuoInit[]		=	{	1	,	1	,	5	,	11	,	3	,	53	,	127	,	73	,	61	,	91	,	1099	,	921	,	1787	};
            const unsigned long	dim655KuoInit[]		=	{	1	,	3	,	5	,	1	,	9	,	39	,	9	,	181	,	287	,	545	,	1343	,	2587	,	961	};
            const unsigned long	dim656KuoInit[]		=	{	1	,	3	,	7	,	5	,	9	,	47	,	7	,	113	,	69	,	531	,	753	,	2225	,	5705	};
            const unsigned long	dim657KuoInit[]		=	{	1	,	1	,	3	,	13	,	29	,	39	,	71	,	223	,	179	,	871	,	1743	,	2645	,	7355	};
            const unsigned long	dim658KuoInit[]		=	{	1	,	1	,	7	,	5	,	1	,	41	,	87	,	251	,	303	,	271	,	791	,	2825	,	2723	};
            const unsigned long	dim659KuoInit[]		=	{	1	,	3	,	1	,	11	,	31	,	15	,	119	,	127	,	451	,	161	,	1909	,	3591	,	2811	};
            const unsigned long	dim660KuoInit[]		=	{	1	,	1	,	3	,	13	,	31	,	27	,	111	,	119	,	423	,	419	,	1659	,	941	,	6123	};
            const unsigned long	dim661KuoInit[]		=	{	1	,	1	,	1	,	7	,	27	,	39	,	27	,	61	,	417	,	629	,	1411	,	1489	,	5915	};
            const unsigned long	dim662KuoInit[]		=	{	1	,	3	,	3	,	1	,	29	,	37	,	111	,	13	,	129	,	831	,	1423	,	3859	,	7009	};
            const unsigned long	dim663KuoInit[]		=	{	1	,	1	,	1	,	3	,	3	,	51	,	67	,	45	,	289	,	715	,	183	,	357	,	3661	};
            const unsigned long	dim664KuoInit[]		=	{	1	,	3	,	3	,	1	,	23	,	49	,	55	,	99	,	77	,	795	,	429	,	1815	,	1867	};
            const unsigned long	dim665KuoInit[]		=	{	1	,	1	,	5	,	7	,	1	,	37	,	45	,	27	,	57	,	937	,	1583	,	899	,	819	};
            const unsigned long	dim666KuoInit[]		=	{	1	,	3	,	5	,	7	,	7	,	37	,	1	,	109	,	217	,	203	,	207	,	2243	,	2337	};
            const unsigned long	dim667KuoInit[]		=	{	1	,	1	,	3	,	15	,	27	,	51	,	119	,	45	,	289	,	395	,	95	,	2425	,	7421	};
            const unsigned long	dim668KuoInit[]		=	{	1	,	3	,	1	,	9	,	27	,	61	,	41	,	227	,	279	,	31	,	1105	,	153	,	5045	};
            const unsigned long	dim669KuoInit[]		=	{	1	,	1	,	7	,	15	,	31	,	43	,	109	,	67	,	347	,	611	,	1811	,	2969	,	3545	};
            const unsigned long	dim670KuoInit[]		=	{	1	,	3	,	1	,	7	,	17	,	15	,	103	,	35	,	407	,	935	,	1159	,	2357	,	6559	};
            const unsigned long	dim671KuoInit[]		=	{	1	,	1	,	1	,	7	,	5	,	7	,	101	,	43	,	301	,	189	,	641	,	3015	,	6955	};
            const unsigned long	dim672KuoInit[]		=	{	1	,	3	,	1	,	1	,	3	,	41	,	61	,	43	,	213	,	595	,	955	,	377	,	7527	};
            const unsigned long	dim673KuoInit[]		=	{	1	,	3	,	3	,	15	,	19	,	45	,	59	,	57	,	425	,	683	,	1187	,	221	,	4265	};
            const unsigned long	dim674KuoInit[]		=	{	1	,	3	,	5	,	3	,	27	,	47	,	85	,	73	,	67	,	509	,	555	,	3887	,	1883	};
            const unsigned long	dim675KuoInit[]		=	{	1	,	3	,	5	,	13	,	19	,	45	,	83	,	191	,	117	,	723	,	701	,	2783	,	389	};
            const unsigned long	dim676KuoInit[]		=	{	1	,	1	,	1	,	3	,	17	,	21	,	125	,	13	,	387	,	613	,	1647	,	2555	,	2435	};
            const unsigned long	dim677KuoInit[]		=	{	1	,	3	,	3	,	11	,	21	,	53	,	105	,	5	,	493	,	167	,	1195	,	2815	,	5761	};
            const unsigned long	dim678KuoInit[]		=	{	1	,	1	,	1	,	7	,	27	,	17	,	107	,	83	,	31	,	787	,	1541	,	3761	,	4083	};
            const unsigned long	dim679KuoInit[]		=	{	1	,	3	,	5	,	11	,	5	,	15	,	17	,	219	,	273	,	207	,	119	,	1693	,	373	};
            const unsigned long	dim680KuoInit[]		=	{	1	,	1	,	5	,	15	,	7	,	11	,	9	,	103	,	253	,	377	,	1667	,	1159	,	5669	};
            const unsigned long	dim681KuoInit[]		=	{	1	,	3	,	7	,	5	,	17	,	55	,	23	,	77	,	341	,	419	,	1165	,	1625	,	6609	};
            const unsigned long	dim682KuoInit[]		=	{	1	,	3	,	1	,	15	,	29	,	7	,	101	,	183	,	181	,	111	,	751	,	247	,	4205	};
            const unsigned long	dim683KuoInit[]		=	{	1	,	1	,	1	,	11	,	3	,	5	,	91	,	43	,	247	,	661	,	621	,	2791	,	4245	};
            const unsigned long	dim684KuoInit[]		=	{	1	,	1	,	1	,	11	,	11	,	63	,	25	,	239	,	39	,	311	,	1603	,	1179	,	4959	};
            const unsigned long	dim685KuoInit[]		=	{	1	,	3	,	1	,	7	,	23	,	61	,	49	,	99	,	287	,	251	,	1053	,	3819	,	5575	};
            const unsigned long	dim686KuoInit[]		=	{	1	,	1	,	1	,	7	,	21	,	17	,	33	,	163	,	503	,	537	,	1159	,	2473	,	8113	};
            const unsigned long	dim687KuoInit[]		=	{	1	,	1	,	5	,	13	,	23	,	57	,	57	,	89	,	503	,	299	,	1319	,	2177	,	8155	};
            const unsigned long	dim688KuoInit[]		=	{	1	,	3	,	3	,	5	,	5	,	5	,	61	,	103	,	247	,	339	,	1137	,	3717	,	747	};
            const unsigned long	dim689KuoInit[]		=	{	1	,	3	,	1	,	7	,	17	,	55	,	41	,	155	,	363	,	873	,	59	,	2093	,	4255	};
            const unsigned long	dim690KuoInit[]		=	{	1	,	3	,	3	,	7	,	9	,	25	,	93	,	87	,	479	,	477	,	2047	,	321	,	2277	};
            const unsigned long	dim691KuoInit[]		=	{	1	,	1	,	3	,	11	,	7	,	37	,	41	,	237	,	121	,	187	,	929	,	1823	,	313	};
            const unsigned long	dim692KuoInit[]		=	{	1	,	1	,	1	,	11	,	29	,	41	,	79	,	11	,	295	,	715	,	979	,	2649	,	915	};
            const unsigned long	dim693KuoInit[]		=	{	1	,	1	,	3	,	13	,	5	,	9	,	55	,	159	,	343	,	887	,	235	,	53	,	593	};
            const unsigned long	dim694KuoInit[]		=	{	1	,	1	,	1	,	3	,	13	,	23	,	105	,	125	,	405	,	521	,	897	,	559	,	1561	};
            const unsigned long	dim695KuoInit[]		=	{	1	,	1	,	3	,	9	,	23	,	33	,	121	,	183	,	181	,	141	,	1103	,	821	,	7193	};
            const unsigned long	dim696KuoInit[]		=	{	1	,	1	,	3	,	9	,	23	,	1	,	113	,	63	,	59	,	783	,	1525	,	115	,	3193	};
            const unsigned long	dim697KuoInit[]		=	{	1	,	3	,	1	,	9	,	23	,	29	,	83	,	237	,	83	,	477	,	1163	,	2951	,	3523	};
            const unsigned long	dim698KuoInit[]		=	{	1	,	3	,	5	,	1	,	21	,	25	,	55	,	79	,	39	,	45	,	1509	,	2127	,	5259	};
            const unsigned long	dim699KuoInit[]		=	{	1	,	1	,	7	,	9	,	31	,	61	,	83	,	145	,	89	,	723	,	393	,	1543	,	7765	};
            const unsigned long	dim700KuoInit[]		=	{	1	,	1	,	5	,	15	,	21	,	17	,	73	,	189	,	473	,	1023	,	1627	,	2305	,	7127	};
            const unsigned long	dim701KuoInit[]		=	{	1	,	3	,	1	,	7	,	7	,	23	,	61	,	203	,	373	,	941	,	975	,	3225	,	7659	};
            const unsigned long	dim702KuoInit[]		=	{	1	,	3	,	1	,	15	,	31	,	19	,	99	,	197	,	113	,	131	,	263	,	2427	,	2085	};
            const unsigned long	dim703KuoInit[]		=	{	1	,	1	,	1	,	13	,	25	,	27	,	3	,	139	,	351	,	267	,	1583	,	913	,	5497	};
            const unsigned long	dim704KuoInit[]		=	{	1	,	3	,	5	,	11	,	7	,	1	,	65	,	85	,	271	,	151	,	1003	,	889	,	6127	};
            const unsigned long	dim705KuoInit[]		=	{	1	,	3	,	7	,	5	,	21	,	27	,	39	,	147	,	139	,	81	,	1377	,	1969	,	6171	};
            const unsigned long	dim706KuoInit[]		=	{	1	,	3	,	5	,	13	,	1	,	11	,	101	,	27	,	285	,	939	,	1055	,	3237	,	7063	};
            const unsigned long	dim707KuoInit[]		=	{	1	,	3	,	3	,	7	,	23	,	43	,	67	,	89	,	201	,	393	,	277	,	3531	,	4003	};
            const unsigned long	dim708KuoInit[]		=	{	1	,	3	,	7	,	9	,	3	,	13	,	103	,	241	,	161	,	111	,	61	,	4001	,	6291	};
            const unsigned long	dim709KuoInit[]		=	{	1	,	1	,	5	,	7	,	5	,	27	,	57	,	87	,	313	,	471	,	227	,	1395	,	2821	};
            const unsigned long	dim710KuoInit[]		=	{	1	,	3	,	1	,	5	,	11	,	35	,	11	,	157	,	257	,	679	,	245	,	329	,	2423	};
            const unsigned long	dim711KuoInit[]		=	{	1	,	1	,	7	,	11	,	15	,	5	,	29	,	229	,	365	,	911	,	477	,	219	,	4851	};
            const unsigned long	dim712KuoInit[]		=	{	1	,	3	,	1	,	9	,	5	,	57	,	87	,	19	,	93	,	403	,	1729	,	1843	,	7301	};
            const unsigned long	dim713KuoInit[]		=	{	1	,	1	,	3	,	3	,	27	,	29	,	11	,	99	,	149	,	917	,	709	,	3761	,	6947	};
            const unsigned long	dim714KuoInit[]		=	{	1	,	1	,	5	,	1	,	19	,	63	,	69	,	71	,	209	,	423	,	2015	,	3905	,	2077	};
            const unsigned long	dim715KuoInit[]		=	{	1	,	3	,	7	,	7	,	7	,	3	,	9	,	63	,	137	,	781	,	1825	,	1331	,	2987	};
            const unsigned long	dim716KuoInit[]		=	{	1	,	3	,	7	,	11	,	15	,	23	,	7	,	205	,	27	,	619	,	225	,	297	,	8179	};
            const unsigned long	dim717KuoInit[]		=	{	1	,	3	,	3	,	13	,	11	,	15	,	11	,	185	,	343	,	269	,	1415	,	843	,	4001	};
            const unsigned long	dim718KuoInit[]		=	{	1	,	3	,	3	,	1	,	15	,	47	,	101	,	49	,	25	,	933	,	903	,	1927	,	6641	};
            const unsigned long	dim719KuoInit[]		=	{	1	,	3	,	3	,	13	,	7	,	45	,	29	,	51	,	143	,	549	,	119	,	235	,	7345	};
            const unsigned long	dim720KuoInit[]		=	{	1	,	1	,	3	,	9	,	29	,	45	,	53	,	133	,	173	,	543	,	545	,	2015	,	149	};
            const unsigned long	dim721KuoInit[]		=	{	1	,	1	,	3	,	11	,	3	,	3	,	109	,	193	,	349	,	243	,	1381	,	3547	,	4387	};
            const unsigned long	dim722KuoInit[]		=	{	1	,	3	,	7	,	9	,	7	,	3	,	37	,	121	,	79	,	59	,	1453	,	3273	,	477	};
            const unsigned long	dim723KuoInit[]		=	{	1	,	3	,	5	,	15	,	19	,	15	,	83	,	5	,	203	,	603	,	373	,	955	,	1587	};
            const unsigned long	dim724KuoInit[]		=	{	1	,	1	,	5	,	11	,	25	,	41	,	89	,	47	,	313	,	443	,	1799	,	1827	,	5881	};
            const unsigned long	dim725KuoInit[]		=	{	1	,	1	,	3	,	5	,	9	,	7	,	15	,	87	,	285	,	509	,	289	,	855	,	3131	};
            const unsigned long	dim726KuoInit[]		=	{	1	,	1	,	3	,	11	,	13	,	57	,	41	,	235	,	357	,	213	,	1867	,	1961	,	2759	};
            const unsigned long	dim727KuoInit[]		=	{	1	,	3	,	5	,	9	,	25	,	15	,	111	,	197	,	479	,	539	,	1833	,	105	,	7245	};
            const unsigned long	dim728KuoInit[]		=	{	1	,	1	,	5	,	5	,	23	,	47	,	101	,	97	,	97	,	261	,	1301	,	3895	,	5425	};
            const unsigned long	dim729KuoInit[]		=	{	1	,	1	,	7	,	11	,	13	,	49	,	39	,	233	,	7	,	965	,	2015	,	1341	,	941	};
            const unsigned long	dim730KuoInit[]		=	{	1	,	1	,	5	,	7	,	19	,	63	,	91	,	77	,	469	,	367	,	1849	,	1213	,	7611	};
            const unsigned long	dim731KuoInit[]		=	{	1	,	3	,	3	,	15	,	17	,	57	,	27	,	107	,	133	,	431	,	445	,	2885	,	5963	};
            const unsigned long	dim732KuoInit[]		=	{	1	,	1	,	3	,	1	,	27	,	37	,	119	,	241	,	403	,	239	,	833	,	3057	,	5557	};
            const unsigned long	dim733KuoInit[]		=	{	1	,	3	,	3	,	13	,	11	,	47	,	35	,	41	,	243	,	143	,	1923	,	1883	,	2543	};
            const unsigned long	dim734KuoInit[]		=	{	1	,	1	,	5	,	5	,	3	,	15	,	45	,	205	,	179	,	515	,	161	,	895	,	3679	};
            const unsigned long	dim735KuoInit[]		=	{	1	,	3	,	1	,	1	,	5	,	39	,	99	,	221	,	115	,	983	,	1913	,	665	,	7835	};
            const unsigned long	dim736KuoInit[]		=	{	1	,	3	,	5	,	15	,	5	,	5	,	111	,	45	,	373	,	143	,	1107	,	1413	,	2935	};
            const unsigned long	dim737KuoInit[]		=	{	1	,	3	,	7	,	11	,	17	,	53	,	77	,	159	,	319	,	279	,	835	,	139	,	3633	};
            const unsigned long	dim738KuoInit[]		=	{	1	,	1	,	7	,	15	,	5	,	57	,	105	,	139	,	491	,	409	,	51	,	2875	,	427	};
            const unsigned long	dim739KuoInit[]		=	{	1	,	1	,	1	,	9	,	9	,	9	,	93	,	45	,	265	,	307	,	589	,	557	,	3971	};
            const unsigned long	dim740KuoInit[]		=	{	1	,	3	,	5	,	3	,	23	,	45	,	5	,	9	,	123	,	841	,	679	,	2213	,	4507	};
            const unsigned long	dim741KuoInit[]		=	{	1	,	3	,	5	,	3	,	27	,	55	,	55	,	19	,	405	,	43	,	1897	,	3515	,	683	};
            const unsigned long	dim742KuoInit[]		=	{	1	,	1	,	7	,	15	,	23	,	47	,	121	,	105	,	341	,	707	,	799	,	1843	,	7011	};
            const unsigned long	dim743KuoInit[]		=	{	1	,	1	,	3	,	13	,	23	,	51	,	71	,	161	,	415	,	819	,	729	,	297	,	4461	};
            const unsigned long	dim744KuoInit[]		=	{	1	,	3	,	5	,	15	,	13	,	39	,	43	,	103	,	251	,	583	,	775	,	877	,	4947	};
            const unsigned long	dim745KuoInit[]		=	{	1	,	3	,	7	,	5	,	19	,	61	,	121	,	49	,	75	,	963	,	297	,	3751	,	911	};
            const unsigned long	dim746KuoInit[]		=	{	1	,	3	,	5	,	7	,	15	,	51	,	23	,	23	,	453	,	675	,	1575	,	473	,	3763	};
            const unsigned long	dim747KuoInit[]		=	{	1	,	1	,	7	,	11	,	9	,	37	,	69	,	137	,	251	,	89	,	1823	,	1289	,	2983	};
            const unsigned long	dim748KuoInit[]		=	{	1	,	3	,	3	,	5	,	11	,	39	,	39	,	231	,	427	,	633	,	1453	,	579	,	5823	};
            const unsigned long	dim749KuoInit[]		=	{	1	,	3	,	7	,	11	,	31	,	57	,	87	,	81	,	161	,	929	,	1293	,	3215	,	4803	};
            const unsigned long	dim750KuoInit[]		=	{	1	,	3	,	5	,	5	,	27	,	33	,	11	,	151	,	25	,	1017	,	41	,	61	,	4381	};
            const unsigned long	dim751KuoInit[]		=	{	1	,	3	,	1	,	15	,	23	,	25	,	7	,	21	,	231	,	407	,	817	,	2089	,	839	};
            const unsigned long	dim752KuoInit[]		=	{	1	,	1	,	7	,	1	,	5	,	51	,	117	,	59	,	251	,	861	,	727	,	481	,	2207	};
            const unsigned long	dim753KuoInit[]		=	{	1	,	1	,	5	,	3	,	21	,	19	,	97	,	199	,	135	,	203	,	1177	,	2751	,	2863	};
            const unsigned long	dim754KuoInit[]		=	{	1	,	3	,	7	,	3	,	27	,	3	,	5	,	109	,	17	,	933	,	911	,	921	,	1173	};
            const unsigned long	dim755KuoInit[]		=	{	1	,	3	,	7	,	1	,	7	,	1	,	121	,	205	,	381	,	971	,	255	,	3313	,	5479	};
            const unsigned long	dim756KuoInit[]		=	{	1	,	3	,	1	,	1	,	17	,	59	,	3	,	5	,	271	,	149	,	111	,	3707	,	7893	};
            const unsigned long	dim757KuoInit[]		=	{	1	,	1	,	1	,	1	,	17	,	61	,	47	,	117	,	187	,	839	,	435	,	399	,	3173	};
            const unsigned long	dim758KuoInit[]		=	{	1	,	3	,	3	,	7	,	5	,	9	,	119	,	191	,	197	,	791	,	375	,	3361	,	6677	};
            const unsigned long	dim759KuoInit[]		=	{	1	,	1	,	5	,	3	,	21	,	49	,	69	,	249	,	39	,	69	,	673	,	109	,	7051	};
            const unsigned long	dim760KuoInit[]		=	{	1	,	1	,	3	,	11	,	21	,	37	,	121	,	93	,	261	,	5	,	1269	,	767	,	2801	};
            const unsigned long	dim761KuoInit[]		=	{	1	,	1	,	5	,	9	,	31	,	49	,	9	,	87	,	375	,	773	,	1517	,	3057	,	2169	};
            const unsigned long	dim762KuoInit[]		=	{	1	,	1	,	5	,	3	,	13	,	33	,	31	,	87	,	353	,	991	,	1809	,	859	,	6117	};
            const unsigned long	dim763KuoInit[]		=	{	1	,	1	,	5	,	11	,	3	,	21	,	103	,	41	,	129	,	137	,	1567	,	3483	,	5317	};
            const unsigned long	dim764KuoInit[]		=	{	1	,	3	,	5	,	1	,	3	,	31	,	5	,	15	,	109	,	723	,	1439	,	2543	,	2377	};
            const unsigned long	dim765KuoInit[]		=	{	1	,	3	,	3	,	13	,	11	,	17	,	9	,	243	,	233	,	543	,	903	,	2775	,	6877	};
            const unsigned long	dim766KuoInit[]		=	{	1	,	1	,	7	,	1	,	23	,	45	,	61	,	137	,	207	,	457	,	245	,	3833	,	6533	};
            const unsigned long	dim767KuoInit[]		=	{	1	,	3	,	1	,	1	,	31	,	7	,	71	,	153	,	501	,	749	,	1477	,	621	,	6577	};
            const unsigned long	dim768KuoInit[]		=	{	1	,	3	,	1	,	13	,	3	,	45	,	3	,	233	,	85	,	847	,	1031	,	141	,	5029	};
            const unsigned long	dim769KuoInit[]		=	{	1	,	3	,	1	,	13	,	17	,	35	,	49	,	157	,	243	,	175	,	905	,	539	,	8059	};
            const unsigned long	dim770KuoInit[]		=	{	1	,	3	,	1	,	11	,	19	,	45	,	31	,	49	,	399	,	341	,	2047	,	1527	,	5993	};
            const unsigned long	dim771KuoInit[]		=	{	1	,	3	,	1	,	3	,	5	,	5	,	33	,	163	,	239	,	607	,	1367	,	2747	,	1513	};
            const unsigned long	dim772KuoInit[]		=	{	1	,	3	,	3	,	11	,	9	,	29	,	33	,	255	,	29	,	955	,	367	,	1709	,	4993	};
            const unsigned long	dim773KuoInit[]		=	{	1	,	3	,	7	,	5	,	19	,	35	,	11	,	47	,	109	,	235	,	1125	,	4019	,	5395	};
            const unsigned long	dim774KuoInit[]		=	{	1	,	1	,	7	,	1	,	19	,	29	,	43	,	1	,	199	,	479	,	1025	,	1065	,	2203	};
            const unsigned long	dim775KuoInit[]		=	{	1	,	3	,	1	,	9	,	17	,	13	,	45	,	167	,	125	,	869	,	1667	,	1083	,	4015	};
            const unsigned long	dim776KuoInit[]		=	{	1	,	1	,	5	,	11	,	27	,	45	,	115	,	199	,	375	,	761	,	1641	,	3761	,	5509	};
            const unsigned long	dim777KuoInit[]		=	{	1	,	1	,	7	,	15	,	9	,	47	,	47	,	133	,	401	,	555	,	1077	,	1381	,	1903	};
            const unsigned long	dim778KuoInit[]		=	{	1	,	3	,	3	,	15	,	3	,	45	,	19	,	49	,	377	,	135	,	239	,	3605	,	1053	};
            const unsigned long	dim779KuoInit[]		=	{	1	,	3	,	1	,	11	,	17	,	5	,	83	,	101	,	247	,	267	,	1337	,	2649	,	513	};
            const unsigned long	dim780KuoInit[]		=	{	1	,	3	,	1	,	11	,	13	,	41	,	41	,	113	,	243	,	463	,	1077	,	565	,	2871	};
            const unsigned long	dim781KuoInit[]		=	{	1	,	3	,	5	,	11	,	19	,	63	,	81	,	51	,	139	,	921	,	2021	,	417	,	1305	};
            const unsigned long	dim782KuoInit[]		=	{	1	,	3	,	3	,	1	,	23	,	61	,	13	,	123	,	135	,	521	,	1963	,	3085	,	4395	};
            const unsigned long	dim783KuoInit[]		=	{	1	,	3	,	1	,	1	,	15	,	61	,	17	,	245	,	479	,	77	,	287	,	3043	,	2647	};
            const unsigned long	dim784KuoInit[]		=	{	1	,	1	,	7	,	15	,	29	,	5	,	125	,	123	,	149	,	919	,	1779	,	3763	,	1479	};
            const unsigned long	dim785KuoInit[]		=	{	1	,	1	,	3	,	1	,	19	,	49	,	27	,	127	,	253	,	279	,	739	,	147	,	6835	};
            const unsigned long	dim786KuoInit[]		=	{	1	,	3	,	5	,	11	,	21	,	27	,	33	,	85	,	419	,	441	,	1341	,	289	,	2759	};
            const unsigned long	dim787KuoInit[]		=	{	1	,	3	,	1	,	3	,	3	,	9	,	17	,	99	,	375	,	251	,	1823	,	2433	,	1525	};
            const unsigned long	dim788KuoInit[]		=	{	1	,	3	,	1	,	3	,	5	,	55	,	25	,	225	,	473	,	825	,	1061	,	2987	,	4397	};
            const unsigned long	dim789KuoInit[]		=	{	1	,	1	,	7	,	15	,	23	,	59	,	9	,	163	,	105	,	911	,	905	,	3715	,	4917	};
            const unsigned long	dim790KuoInit[]		=	{	1	,	3	,	1	,	11	,	21	,	53	,	127	,	3	,	507	,	467	,	547	,	3003	,	6939	};
            const unsigned long	dim791KuoInit[]		=	{	1	,	1	,	1	,	13	,	15	,	7	,	1	,	7	,	101	,	645	,	1469	,	843	,	2843	};
            const unsigned long	dim792KuoInit[]		=	{	1	,	3	,	3	,	9	,	11	,	35	,	95	,	53	,	291	,	459	,	669	,	1163	,	4893	};
            const unsigned long	dim793KuoInit[]		=	{	1	,	1	,	3	,	3	,	25	,	29	,	29	,	223	,	21	,	553	,	177	,	3347	,	3575	};
            const unsigned long	dim794KuoInit[]		=	{	1	,	1	,	5	,	1	,	23	,	17	,	21	,	57	,	383	,	427	,	397	,	1947	,	2627	};
            const unsigned long	dim795KuoInit[]		=	{	1	,	3	,	7	,	1	,	21	,	53	,	79	,	59	,	133	,	461	,	1703	,	3071	,	4679	};
            const unsigned long	dim796KuoInit[]		=	{	1	,	3	,	5	,	1	,	27	,	31	,	83	,	159	,	505	,	121	,	1969	,	1073	,	2117	};
            const unsigned long	dim797KuoInit[]		=	{	1	,	1	,	3	,	1	,	19	,	25	,	43	,	115	,	191	,	845	,	1923	,	903	,	2319	};
            const unsigned long	dim798KuoInit[]		=	{	1	,	1	,	3	,	9	,	27	,	37	,	31	,	111	,	437	,	925	,	501	,	883	,	3485	};
            const unsigned long	dim799KuoInit[]		=	{	1	,	3	,	5	,	5	,	15	,	25	,	51	,	131	,	149	,	811	,	1827	,	1519	,	1435	};
            const unsigned long	dim800KuoInit[]		=	{	1	,	1	,	7	,	13	,	3	,	39	,	103	,	107	,	473	,	553	,	1763	,	399	,	2137	};
            const unsigned long	dim801KuoInit[]		=	{	1	,	3	,	7	,	13	,	27	,	25	,	49	,	29	,	497	,	863	,	1715	,	99	,	1521	};
            const unsigned long	dim802KuoInit[]		=	{	1	,	1	,	1	,	9	,	27	,	37	,	67	,	59	,	11	,	369	,	893	,	2249	,	5303	};
            const unsigned long	dim803KuoInit[]		=	{	1	,	3	,	3	,	13	,	19	,	9	,	93	,	29	,	247	,	35	,	833	,	1037	,	7137	};
            const unsigned long	dim804KuoInit[]		=	{	1	,	3	,	7	,	3	,	25	,	17	,	63	,	239	,	395	,	367	,	1935	,	2095	,	5191	};
            const unsigned long	dim805KuoInit[]		=	{	1	,	3	,	5	,	15	,	23	,	51	,	13	,	101	,	477	,	511	,	1951	,	2505	,	4155	};
            const unsigned long	dim806KuoInit[]		=	{	1	,	3	,	3	,	15	,	3	,	55	,	15	,	173	,	21	,	699	,	1617	,	1573	,	2645	};
            const unsigned long	dim807KuoInit[]		=	{	1	,	1	,	3	,	11	,	7	,	55	,	69	,	165	,	255	,	397	,	2033	,	9	,	843	};
            const unsigned long	dim808KuoInit[]		=	{	1	,	3	,	1	,	13	,	5	,	3	,	37	,	99	,	165	,	971	,	555	,	2379	,	7879	};
            const unsigned long	dim809KuoInit[]		=	{	1	,	1	,	5	,	3	,	19	,	37	,	23	,	197	,	145	,	315	,	981	,	161	,	323	};
            const unsigned long	dim810KuoInit[]		=	{	1	,	1	,	7	,	7	,	17	,	37	,	27	,	189	,	371	,	145	,	1097	,	2801	,	5469	};
            const unsigned long	dim811KuoInit[]		=	{	1	,	1	,	3	,	9	,	23	,	15	,	39	,	7	,	25	,	221	,	757	,	1393	,	4629	};
            const unsigned long	dim812KuoInit[]		=	{	1	,	1	,	1	,	1	,	13	,	31	,	29	,	197	,	237	,	173	,	337	,	743	,	1459	};
            const unsigned long	dim813KuoInit[]		=	{	1	,	1	,	5	,	15	,	17	,	7	,	127	,	29	,	307	,	965	,	1567	,	2771	,	6053	};
            const unsigned long	dim814KuoInit[]		=	{	1	,	3	,	1	,	1	,	13	,	57	,	41	,	59	,	77	,	67	,	1985	,	3443	,	7495	};
            const unsigned long	dim815KuoInit[]		=	{	1	,	3	,	7	,	15	,	17	,	21	,	57	,	143	,	179	,	99	,	217	,	1763	,	873	};
            const unsigned long	dim816KuoInit[]		=	{	1	,	1	,	7	,	1	,	21	,	17	,	123	,	39	,	95	,	775	,	1865	,	341	,	6167	};
            const unsigned long	dim817KuoInit[]		=	{	1	,	3	,	7	,	3	,	31	,	33	,	33	,	163	,	281	,	765	,	1809	,	1469	,	7011	};
            const unsigned long	dim818KuoInit[]		=	{	1	,	3	,	7	,	7	,	25	,	35	,	45	,	109	,	221	,	25	,	757	,	597	,	3057	};
            const unsigned long	dim819KuoInit[]		=	{	1	,	3	,	5	,	3	,	27	,	25	,	97	,	21	,	351	,	841	,	1339	,	565	,	7181	};
            const unsigned long	dim820KuoInit[]		=	{	1	,	1	,	1	,	15	,	23	,	61	,	97	,	57	,	503	,	379	,	25	,	1197	,	6957	};
            const unsigned long	dim821KuoInit[]		=	{	1	,	3	,	3	,	3	,	13	,	49	,	127	,	89	,	329	,	503	,	1957	,	3569	,	7013	};
            const unsigned long	dim822KuoInit[]		=	{	1	,	1	,	7	,	7	,	7	,	15	,	43	,	61	,	227	,	859	,	279	,	3309	,	1941	};
            const unsigned long	dim823KuoInit[]		=	{	1	,	3	,	1	,	9	,	19	,	1	,	111	,	45	,	315	,	339	,	1049	,	121	,	2711	};
            const unsigned long	dim824KuoInit[]		=	{	1	,	1	,	3	,	11	,	7	,	33	,	121	,	253	,	267	,	845	,	1677	,	569	,	3743	};
            const unsigned long	dim825KuoInit[]		=	{	1	,	3	,	7	,	9	,	9	,	53	,	113	,	11	,	177	,	165	,	1707	,	2027	,	2099	};
            const unsigned long	dim826KuoInit[]		=	{	1	,	1	,	5	,	7	,	19	,	41	,	33	,	129	,	389	,	249	,	1945	,	1393	,	5941	};
            const unsigned long	dim827KuoInit[]		=	{	1	,	1	,	1	,	1	,	27	,	37	,	63	,	171	,	301	,	179	,	223	,	3775	,	1189	};
            const unsigned long	dim828KuoInit[]		=	{	1	,	3	,	3	,	5	,	5	,	33	,	101	,	37	,	505	,	561	,	1681	,	567	,	2197	};
            const unsigned long	dim829KuoInit[]		=	{	1	,	3	,	7	,	13	,	25	,	23	,	29	,	27	,	325	,	757	,	5	,	487	,	3241	};
            const unsigned long	dim830KuoInit[]		=	{	1	,	3	,	3	,	7	,	27	,	13	,	85	,	35	,	171	,	565	,	969	,	773	,	7173	};
            const unsigned long	dim831KuoInit[]		=	{	1	,	3	,	5	,	5	,	11	,	55	,	33	,	243	,	401	,	939	,	73	,	775	,	1159	};
            const unsigned long	dim832KuoInit[]		=	{	1	,	1	,	3	,	13	,	13	,	63	,	9	,	161	,	343	,	547	,	1659	,	1005	,	8041	};
            const unsigned long	dim833KuoInit[]		=	{	1	,	3	,	5	,	7	,	15	,	27	,	27	,	51	,	475	,	957	,	445	,	3745	,	5113	};
            const unsigned long	dim834KuoInit[]		=	{	1	,	1	,	3	,	7	,	15	,	49	,	63	,	181	,	15	,	1	,	1915	,	633	,	1039	};
            const unsigned long	dim835KuoInit[]		=	{	1	,	3	,	7	,	1	,	1	,	11	,	15	,	221	,	375	,	59	,	1471	,	113	,	5661	};
            const unsigned long	dim836KuoInit[]		=	{	1	,	3	,	5	,	5	,	11	,	41	,	115	,	87	,	263	,	173	,	1249	,	2235	,	6863	};
            const unsigned long	dim837KuoInit[]		=	{	1	,	1	,	5	,	11	,	19	,	1	,	61	,	25	,	239	,	5	,	1869	,	2269	,	3911	};
            const unsigned long	dim838KuoInit[]		=	{	1	,	3	,	5	,	7	,	23	,	59	,	99	,	47	,	439	,	105	,	455	,	547	,	2447	};
            const unsigned long	dim839KuoInit[]		=	{	1	,	1	,	7	,	15	,	25	,	15	,	117	,	185	,	83	,	243	,	1099	,	2613	,	7721	};
            const unsigned long	dim840KuoInit[]		=	{	1	,	3	,	1	,	9	,	5	,	49	,	27	,	237	,	239	,	177	,	1507	,	2059	,	1731	};
            const unsigned long	dim841KuoInit[]		=	{	1	,	3	,	3	,	11	,	29	,	47	,	119	,	243	,	475	,	443	,	111	,	787	,	2597	};
            const unsigned long	dim842KuoInit[]		=	{	1	,	1	,	7	,	1	,	25	,	39	,	95	,	107	,	391	,	149	,	397	,	3755	,	2735	};
            const unsigned long	dim843KuoInit[]		=	{	1	,	3	,	7	,	11	,	13	,	49	,	37	,	135	,	99	,	473	,	1961	,	1233	,	7359	};
            const unsigned long	dim844KuoInit[]		=	{	1	,	3	,	5	,	11	,	29	,	59	,	123	,	53	,	371	,	573	,	1099	,	3035	,	3217	};
            const unsigned long	dim845KuoInit[]		=	{	1	,	1	,	1	,	11	,	25	,	47	,	105	,	147	,	317	,	69	,	1201	,	3781	,	2457	};
            const unsigned long	dim846KuoInit[]		=	{	1	,	1	,	5	,	9	,	3	,	31	,	25	,	127	,	153	,	303	,	1367	,	3583	,	5229	};
            const unsigned long	dim847KuoInit[]		=	{	1	,	1	,	3	,	13	,	25	,	33	,	11	,	101	,	85	,	741	,	1191	,	1769	,	4119	};
            const unsigned long	dim848KuoInit[]		=	{	1	,	1	,	3	,	13	,	19	,	59	,	81	,	71	,	255	,	277	,	991	,	2183	,	3257	};
            const unsigned long	dim849KuoInit[]		=	{	1	,	3	,	1	,	3	,	23	,	27	,	89	,	237	,	249	,	837	,	759	,	43	,	5571	};
            const unsigned long	dim850KuoInit[]		=	{	1	,	1	,	1	,	9	,	15	,	35	,	45	,	213	,	423	,	159	,	353	,	3123	,	6399	};
            const unsigned long	dim851KuoInit[]		=	{	1	,	1	,	5	,	9	,	15	,	45	,	83	,	65	,	115	,	513	,	361	,	1877	,	4307	};
            const unsigned long	dim852KuoInit[]		=	{	1	,	1	,	7	,	11	,	17	,	61	,	15	,	143	,	491	,	253	,	443	,	3521	,	7611	};
            const unsigned long	dim853KuoInit[]		=	{	1	,	1	,	3	,	11	,	11	,	49	,	117	,	117	,	279	,	715	,	1365	,	689	,	4339	};
            const unsigned long	dim854KuoInit[]		=	{	1	,	1	,	3	,	13	,	3	,	53	,	123	,	89	,	91	,	471	,	1723	,	1811	,	1285	};
            const unsigned long	dim855KuoInit[]		=	{	1	,	3	,	7	,	13	,	7	,	5	,	99	,	235	,	465	,	915	,	1813	,	3137	,	7645	};
            const unsigned long	dim856KuoInit[]		=	{	1	,	3	,	3	,	13	,	5	,	45	,	37	,	99	,	459	,	541	,	225	,	2417	,	6385	};
            const unsigned long	dim857KuoInit[]		=	{	1	,	1	,	1	,	15	,	19	,	39	,	109	,	21	,	117	,	589	,	1261	,	3695	,	4251	};
            const unsigned long	dim858KuoInit[]		=	{	1	,	1	,	7	,	11	,	23	,	43	,	35	,	43	,	135	,	509	,	1331	,	1567	,	7085	};
            const unsigned long	dim859KuoInit[]		=	{	1	,	1	,	5	,	5	,	23	,	3	,	61	,	151	,	171	,	295	,	1377	,	2165	,	2125	};
            const unsigned long	dim860KuoInit[]		=	{	1	,	3	,	3	,	5	,	3	,	57	,	17	,	73	,	71	,	55	,	1373	,	2067	,	3249	};
            const unsigned long	dim861KuoInit[]		=	{	1	,	1	,	5	,	9	,	25	,	39	,	65	,	75	,	141	,	849	,	237	,	3719	,	6705	};
            const unsigned long	dim862KuoInit[]		=	{	1	,	1	,	7	,	1	,	17	,	17	,	23	,	91	,	333	,	749	,	657	,	3267	,	3343	};
            const unsigned long	dim863KuoInit[]		=	{	1	,	3	,	1	,	5	,	5	,	27	,	15	,	107	,	345	,	405	,	111	,	345	,	5505	};
            const unsigned long	dim864KuoInit[]		=	{	1	,	3	,	3	,	13	,	1	,	9	,	19	,	63	,	427	,	995	,	1257	,	923	,	267	};
            const unsigned long	dim865KuoInit[]		=	{	1	,	1	,	1	,	7	,	21	,	19	,	65	,	249	,	401	,	729	,	1403	,	1579	,	3443	};
            const unsigned long	dim866KuoInit[]		=	{	1	,	3	,	5	,	1	,	3	,	43	,	81	,	11	,	473	,	1011	,	1755	,	117	,	2881	};
            const unsigned long	dim867KuoInit[]		=	{	1	,	1	,	7	,	13	,	3	,	63	,	95	,	11	,	23	,	895	,	1591	,	2623	,	6887	};
            const unsigned long	dim868KuoInit[]		=	{	1	,	3	,	1	,	3	,	17	,	11	,	39	,	165	,	367	,	363	,	1207	,	2555	,	5437	};
            const unsigned long	dim869KuoInit[]		=	{	1	,	1	,	1	,	7	,	17	,	9	,	19	,	175	,	7	,	535	,	1779	,	3341	,	4825	};
            const unsigned long	dim870KuoInit[]		=	{	1	,	1	,	7	,	3	,	27	,	7	,	19	,	217	,	479	,	143	,	1189	,	2809	,	339	};
            const unsigned long	dim871KuoInit[]		=	{	1	,	3	,	7	,	9	,	25	,	41	,	9	,	39	,	391	,	331	,	1427	,	2321	,	1673	};
            const unsigned long	dim872KuoInit[]		=	{	1	,	3	,	1	,	13	,	7	,	17	,	11	,	27	,	125	,	799	,	1325	,	517	,	1043	};
            const unsigned long	dim873KuoInit[]		=	{	1	,	1	,	3	,	15	,	29	,	11	,	73	,	11	,	289	,	1	,	839	,	3311	,	7535	};
            const unsigned long	dim874KuoInit[]		=	{	1	,	1	,	1	,	11	,	11	,	59	,	25	,	47	,	153	,	297	,	679	,	1441	,	7105	};
            const unsigned long	dim875KuoInit[]		=	{	1	,	3	,	5	,	7	,	25	,	29	,	13	,	227	,	379	,	247	,	449	,	679	,	6581	};
            const unsigned long	dim876KuoInit[]		=	{	1	,	1	,	1	,	5	,	9	,	59	,	81	,	195	,	289	,	705	,	183	,	1	,	3703	};
            const unsigned long	dim877KuoInit[]		=	{	1	,	3	,	3	,	9	,	31	,	1	,	71	,	155	,	307	,	467	,	1699	,	269	,	6077	};
            const unsigned long	dim878KuoInit[]		=	{	1	,	3	,	1	,	11	,	31	,	21	,	59	,	225	,	385	,	623	,	117	,	1435	,	3195	};
            const unsigned long	dim879KuoInit[]		=	{	1	,	1	,	7	,	9	,	3	,	43	,	97	,	15	,	145	,	279	,	1785	,	545	,	3977	};
            const unsigned long	dim880KuoInit[]		=	{	1	,	1	,	5	,	9	,	25	,	27	,	113	,	193	,	31	,	255	,	691	,	3315	,	7005	};
            const unsigned long	dim881KuoInit[]		=	{	1	,	3	,	7	,	5	,	9	,	19	,	69	,	87	,	379	,	931	,	1567	,	1391	,	4765	};
            const unsigned long	dim882KuoInit[]		=	{	1	,	1	,	1	,	15	,	15	,	23	,	127	,	69	,	113	,	801	,	1923	,	2847	,	1073	};
            const unsigned long	dim883KuoInit[]		=	{	1	,	1	,	1	,	1	,	21	,	51	,	65	,	133	,	465	,	739	,	2029	,	2243	,	4023	};
            const unsigned long	dim884KuoInit[]		=	{	1	,	1	,	5	,	11	,	9	,	41	,	71	,	217	,	317	,	855	,	275	,	1965	,	3151	};
            const unsigned long	dim885KuoInit[]		=	{	1	,	1	,	7	,	5	,	19	,	3	,	81	,	127	,	423	,	415	,	565	,	1941	,	5079	};
            const unsigned long	dim886KuoInit[]		=	{	1	,	1	,	3	,	1	,	19	,	19	,	117	,	169	,	353	,	159	,	1235	,	845	,	2381	};
            const unsigned long	dim887KuoInit[]		=	{	1	,	3	,	1	,	15	,	21	,	27	,	77	,	5	,	113	,	201	,	311	,	2263	,	7855	};
            const unsigned long	dim888KuoInit[]		=	{	1	,	3	,	5	,	9	,	5	,	21	,	41	,	119	,	447	,	983	,	1703	,	2707	,	2789	};
            const unsigned long	dim889KuoInit[]		=	{	1	,	1	,	5	,	3	,	5	,	19	,	9	,	131	,	269	,	999	,	1259	,	3843	,	7753	};
            const unsigned long	dim890KuoInit[]		=	{	1	,	3	,	7	,	11	,	25	,	27	,	91	,	187	,	177	,	921	,	163	,	1141	,	4903	};
            const unsigned long	dim891KuoInit[]		=	{	1	,	3	,	1	,	7	,	15	,	21	,	67	,	155	,	335	,	149	,	2011	,	2483	,	7015	};
            const unsigned long	dim892KuoInit[]		=	{	1	,	1	,	1	,	7	,	3	,	53	,	29	,	119	,	21	,	933	,	595	,	3141	,	7925	};
            const unsigned long	dim893KuoInit[]		=	{	1	,	1	,	1	,	9	,	21	,	33	,	55	,	87	,	451	,	297	,	1615	,	697	,	2299	};
            const unsigned long	dim894KuoInit[]		=	{	1	,	3	,	1	,	9	,	9	,	5	,	3	,	113	,	197	,	137	,	1105	,	1949	,	4651	};
            const unsigned long	dim895KuoInit[]		=	{	1	,	1	,	7	,	3	,	7	,	27	,	109	,	99	,	121	,	463	,	621	,	887	,	5847	};
            const unsigned long	dim896KuoInit[]		=	{	1	,	3	,	7	,	7	,	31	,	61	,	21	,	69	,	443	,	81	,	151	,	3601	,	699	};
            const unsigned long	dim897KuoInit[]		=	{	1	,	1	,	7	,	11	,	7	,	63	,	97	,	241	,	87	,	961	,	1279	,	1715	,	3183	};
            const unsigned long	dim898KuoInit[]		=	{	1	,	1	,	3	,	5	,	7	,	41	,	5	,	123	,	189	,	283	,	1775	,	49	,	6107	};
            const unsigned long	dim899KuoInit[]		=	{	1	,	1	,	7	,	15	,	7	,	19	,	71	,	117	,	195	,	67	,	1583	,	3371	,	777	};
            const unsigned long	dim900KuoInit[]		=	{	1	,	1	,	1	,	15	,	19	,	43	,	81	,	103	,	173	,	209	,	447	,	1735	,	5023	};
            const unsigned long	dim901KuoInit[]		=	{	1	,	1	,	5	,	15	,	15	,	9	,	99	,	185	,	451	,	605	,	1429	,	1257	,	6825	};
            const unsigned long	dim902KuoInit[]		=	{	1	,	3	,	1	,	11	,	11	,	21	,	79	,	13	,	23	,	607	,	1601	,	3603	,	147	};
            const unsigned long	dim903KuoInit[]		=	{	1	,	1	,	3	,	1	,	27	,	51	,	83	,	191	,	59	,	281	,	1853	,	1441	,	5359	};
            const unsigned long	dim904KuoInit[]		=	{	1	,	1	,	1	,	11	,	31	,	17	,	117	,	239	,	385	,	465	,	1787	,	2645	,	6047	};
            const unsigned long	dim905KuoInit[]		=	{	1	,	1	,	7	,	5	,	17	,	59	,	111	,	143	,	419	,	163	,	763	,	2327	,	1741	};
            const unsigned long	dim906KuoInit[]		=	{	1	,	1	,	1	,	3	,	3	,	43	,	3	,	203	,	1	,	373	,	1829	,	3493	,	2347	};
            const unsigned long	dim907KuoInit[]		=	{	1	,	3	,	7	,	13	,	21	,	17	,	89	,	147	,	51	,	665	,	397	,	2475	,	3393	};
            const unsigned long	dim908KuoInit[]		=	{	1	,	3	,	7	,	15	,	19	,	33	,	1	,	211	,	155	,	355	,	1137	,	1633	,	5025	};
            const unsigned long	dim909KuoInit[]		=	{	1	,	3	,	3	,	9	,	7	,	63	,	127	,	59	,	205	,	645	,	2047	,	1187	,	2711	};
            const unsigned long	dim910KuoInit[]		=	{	1	,	3	,	3	,	5	,	7	,	59	,	125	,	35	,	285	,	811	,	1489	,	3269	,	2737	};
            const unsigned long	dim911KuoInit[]		=	{	1	,	1	,	7	,	3	,	15	,	39	,	75	,	229	,	213	,	23	,	1825	,	2269	,	6739	};
            const unsigned long	dim912KuoInit[]		=	{	1	,	1	,	3	,	7	,	1	,	5	,	73	,	173	,	305	,	681	,	1225	,	2475	,	6981	};
            const unsigned long	dim913KuoInit[]		=	{	1	,	1	,	5	,	3	,	21	,	3	,	27	,	241	,	475	,	543	,	1065	,	2731	,	1739	};
            const unsigned long	dim914KuoInit[]		=	{	1	,	3	,	7	,	13	,	19	,	35	,	57	,	19	,	307	,	113	,	451	,	1897	,	2135	};
            const unsigned long	dim915KuoInit[]		=	{	1	,	1	,	3	,	11	,	31	,	13	,	63	,	15	,	477	,	389	,	829	,	2167	,	3317	};
            const unsigned long	dim916KuoInit[]		=	{	1	,	1	,	7	,	11	,	9	,	39	,	1	,	41	,	99	,	295	,	1009	,	1411	,	3423	};
            const unsigned long	dim917KuoInit[]		=	{	1	,	1	,	5	,	9	,	15	,	17	,	93	,	171	,	361	,	51	,	455	,	2413	,	7295	};
            const unsigned long	dim918KuoInit[]		=	{	1	,	1	,	5	,	13	,	7	,	27	,	127	,	153	,	163	,	635	,	1065	,	2243	,	3813	};
            const unsigned long	dim919KuoInit[]		=	{	1	,	1	,	7	,	9	,	15	,	45	,	7	,	241	,	365	,	447	,	1435	,	3473	,	715	};
            const unsigned long	dim920KuoInit[]		=	{	1	,	1	,	5	,	9	,	9	,	11	,	21	,	21	,	255	,	717	,	1579	,	217	,	6921	};
            const unsigned long	dim921KuoInit[]		=	{	1	,	1	,	5	,	15	,	25	,	21	,	31	,	47	,	455	,	631	,	971	,	3105	,	2219	};
            const unsigned long	dim922KuoInit[]		=	{	1	,	3	,	1	,	7	,	9	,	1	,	75	,	85	,	351	,	555	,	1379	,	1361	,	6163	};
            const unsigned long	dim923KuoInit[]		=	{	1	,	3	,	5	,	7	,	9	,	63	,	19	,	99	,	335	,	173	,	1385	,	3011	,	5787	};
            const unsigned long	dim924KuoInit[]		=	{	1	,	3	,	5	,	1	,	23	,	35	,	79	,	163	,	97	,	213	,	1281	,	1739	,	477	};
            const unsigned long	dim925KuoInit[]		=	{	1	,	1	,	5	,	5	,	9	,	13	,	41	,	167	,	349	,	959	,	925	,	1383	,	883	};
            const unsigned long	dim926KuoInit[]		=	{	1	,	3	,	7	,	7	,	17	,	23	,	69	,	241	,	447	,	31	,	767	,	2981	,	253	};
            const unsigned long	dim927KuoInit[]		=	{	1	,	3	,	7	,	15	,	31	,	15	,	3	,	107	,	225	,	207	,	429	,	959	,	2627	};
            const unsigned long	dim928KuoInit[]		=	{	1	,	3	,	3	,	11	,	29	,	25	,	109	,	41	,	261	,	585	,	1049	,	3013	,	4361	};
            const unsigned long	dim929KuoInit[]		=	{	1	,	1	,	7	,	11	,	29	,	63	,	117	,	147	,	149	,	427	,	1371	,	2171	,	7797	};
            const unsigned long	dim930KuoInit[]		=	{	1	,	3	,	5	,	3	,	13	,	33	,	103	,	33	,	227	,	785	,	1277	,	27	,	6713	};
            const unsigned long	dim931KuoInit[]		=	{	1	,	3	,	1	,	3	,	19	,	3	,	121	,	123	,	313	,	283	,	1231	,	3331	,	7271	};
            const unsigned long	dim932KuoInit[]		=	{	1	,	1	,	7	,	3	,	21	,	57	,	53	,	9	,	275	,	661	,	1665	,	1557	,	3813	};
            const unsigned long	dim933KuoInit[]		=	{	1	,	3	,	5	,	1	,	19	,	63	,	69	,	149	,	373	,	667	,	975	,	3483	,	7605	};
            const unsigned long	dim934KuoInit[]		=	{	1	,	3	,	5	,	3	,	3	,	53	,	37	,	247	,	293	,	491	,	657	,	3123	,	1165	};
            const unsigned long	dim935KuoInit[]		=	{	1	,	1	,	1	,	13	,	27	,	41	,	23	,	185	,	405	,	215	,	455	,	433	,	7775	};
            const unsigned long	dim936KuoInit[]		=	{	1	,	1	,	3	,	13	,	23	,	61	,	63	,	61	,	195	,	83	,	33	,	1903	,	269	};
            const unsigned long	dim937KuoInit[]		=	{	1	,	1	,	3	,	7	,	5	,	7	,	1	,	161	,	295	,	539	,	1371	,	3911	,	4095	};
            const unsigned long	dim938KuoInit[]		=	{	1	,	1	,	7	,	9	,	7	,	7	,	63	,	115	,	323	,	95	,	1965	,	137	,	7147	};
            const unsigned long	dim939KuoInit[]		=	{	1	,	3	,	7	,	11	,	19	,	55	,	107	,	71	,	169	,	937	,	939	,	113	,	6187	};
            const unsigned long	dim940KuoInit[]		=	{	1	,	1	,	3	,	7	,	13	,	17	,	55	,	5	,	167	,	337	,	2043	,	1485	,	5069	};
            const unsigned long	dim941KuoInit[]		=	{	1	,	1	,	7	,	9	,	21	,	3	,	107	,	125	,	253	,	363	,	49	,	469	,	2873	};
            const unsigned long	dim942KuoInit[]		=	{	1	,	1	,	7	,	9	,	9	,	31	,	91	,	65	,	315	,	973	,	653	,	1023	,	6479	};
            const unsigned long	dim943KuoInit[]		=	{	1	,	3	,	5	,	11	,	1	,	5	,	19	,	109	,	143	,	335	,	1035	,	3893	,	4573	};
            const unsigned long	dim944KuoInit[]		=	{	1	,	3	,	3	,	13	,	27	,	53	,	37	,	177	,	407	,	315	,	1943	,	3111	,	1385	};
            const unsigned long	dim945KuoInit[]		=	{	1	,	3	,	5	,	15	,	27	,	47	,	57	,	105	,	131	,	31	,	551	,	3733	,	5157	};
            const unsigned long	dim946KuoInit[]		=	{	1	,	3	,	5	,	9	,	15	,	31	,	41	,	11	,	93	,	895	,	587	,	421	,	885	};
            const unsigned long	dim947KuoInit[]		=	{	1	,	3	,	7	,	1	,	5	,	35	,	107	,	189	,	287	,	457	,	645	,	519	,	7459	};
            const unsigned long	dim948KuoInit[]		=	{	1	,	1	,	7	,	5	,	23	,	7	,	79	,	45	,	73	,	711	,	575	,	3571	,	1453	};
            const unsigned long	dim949KuoInit[]		=	{	1	,	1	,	3	,	9	,	9	,	7	,	89	,	197	,	215	,	111	,	1099	,	3915	,	3709	};
            const unsigned long	dim950KuoInit[]		=	{	1	,	3	,	1	,	7	,	25	,	1	,	83	,	55	,	449	,	227	,	1563	,	1103	,	4129	};
            const unsigned long	dim951KuoInit[]		=	{	1	,	1	,	1	,	9	,	23	,	25	,	103	,	183	,	379	,	541	,	673	,	2711	,	7409	};
            const unsigned long	dim952KuoInit[]		=	{	1	,	3	,	1	,	11	,	27	,	21	,	83	,	61	,	65	,	867	,	1755	,	3815	,	7235	};
            const unsigned long	dim953KuoInit[]		=	{	1	,	1	,	3	,	15	,	15	,	55	,	13	,	185	,	145	,	955	,	785	,	189	,	6653	};
            const unsigned long	dim954KuoInit[]		=	{	1	,	3	,	5	,	15	,	5	,	19	,	83	,	157	,	429	,	57	,	1293	,	1551	,	581	};
            const unsigned long	dim955KuoInit[]		=	{	1	,	3	,	3	,	11	,	25	,	19	,	117	,	73	,	503	,	1015	,	203	,	2689	,	6697	};
            const unsigned long	dim956KuoInit[]		=	{	1	,	1	,	7	,	1	,	27	,	31	,	111	,	193	,	403	,	267	,	233	,	3031	,	5451	};
            const unsigned long	dim957KuoInit[]		=	{	1	,	3	,	7	,	5	,	9	,	7	,	73	,	221	,	27	,	483	,	185	,	1805	,	1813	};
            const unsigned long	dim958KuoInit[]		=	{	1	,	3	,	3	,	5	,	19	,	27	,	103	,	197	,	245	,	191	,	301	,	531	,	3677	};
            const unsigned long	dim959KuoInit[]		=	{	1	,	1	,	5	,	1	,	9	,	49	,	5	,	55	,	81	,	907	,	313	,	3379	,	2573	};
            const unsigned long	dim960KuoInit[]		=	{	1	,	3	,	5	,	3	,	13	,	11	,	75	,	145	,	143	,	991	,	201	,	3769	,	5041	};
            const unsigned long	dim961KuoInit[]		=	{	1	,	3	,	1	,	5	,	15	,	45	,	89	,	17	,	209	,	91	,	1635	,	3025	,	3885	};
            const unsigned long	dim962KuoInit[]		=	{	1	,	3	,	3	,	5	,	15	,	37	,	79	,	163	,	455	,	477	,	69	,	3617	,	1263	};
            const unsigned long	dim963KuoInit[]		=	{	1	,	1	,	3	,	11	,	15	,	47	,	77	,	73	,	199	,	493	,	207	,	2109	,	4991	};
            const unsigned long	dim964KuoInit[]		=	{	1	,	1	,	5	,	15	,	29	,	23	,	35	,	177	,	483	,	151	,	1497	,	631	,	1647	};
            const unsigned long	dim965KuoInit[]		=	{	1	,	3	,	7	,	5	,	29	,	45	,	127	,	199	,	491	,	965	,	371	,	3549	,	2757	};
            const unsigned long	dim966KuoInit[]		=	{	1	,	1	,	7	,	5	,	21	,	11	,	27	,	217	,	407	,	493	,	1979	,	359	,	6187	};
            const unsigned long	dim967KuoInit[]		=	{	1	,	3	,	5	,	7	,	31	,	61	,	113	,	197	,	109	,	781	,	1859	,	991	,	3617	};
            const unsigned long	dim968KuoInit[]		=	{	1	,	3	,	1	,	5	,	17	,	43	,	75	,	169	,	413	,	257	,	1751	,	4047	,	1951	};
            const unsigned long	dim969KuoInit[]		=	{	1	,	3	,	1	,	13	,	17	,	49	,	95	,	223	,	439	,	587	,	427	,	89	,	7723	};
            const unsigned long	dim970KuoInit[]		=	{	1	,	3	,	5	,	1	,	7	,	53	,	1	,	79	,	111	,	721	,	169	,	4087	,	5255	};
            const unsigned long	dim971KuoInit[]		=	{	1	,	1	,	7	,	15	,	31	,	7	,	111	,	163	,	491	,	559	,	259	,	3057	,	5669	};
            const unsigned long	dim972KuoInit[]		=	{	1	,	3	,	3	,	9	,	25	,	41	,	45	,	101	,	145	,	769	,	1313	,	3101	,	405	};
            const unsigned long	dim973KuoInit[]		=	{	1	,	3	,	3	,	11	,	3	,	39	,	67	,	201	,	171	,	79	,	1727	,	2331	,	8161	};
            const unsigned long	dim974KuoInit[]		=	{	1	,	3	,	5	,	5	,	31	,	29	,	13	,	111	,	449	,	579	,	1133	,	1321	,	2447	};
            const unsigned long	dim975KuoInit[]		=	{	1	,	3	,	3	,	15	,	21	,	39	,	57	,	203	,	39	,	733	,	1773	,	2867	,	6751	};
            const unsigned long	dim976KuoInit[]		=	{	1	,	3	,	5	,	3	,	19	,	33	,	73	,	219	,	159	,	901	,	919	,	1445	,	1387	};
            const unsigned long	dim977KuoInit[]		=	{	1	,	3	,	1	,	7	,	9	,	33	,	113	,	221	,	315	,	503	,	1795	,	577	,	1605	};
            const unsigned long	dim978KuoInit[]		=	{	1	,	1	,	7	,	11	,	31	,	47	,	51	,	53	,	159	,	731	,	1101	,	1895	,	2061	};
            const unsigned long	dim979KuoInit[]		=	{	1	,	3	,	5	,	7	,	29	,	11	,	107	,	39	,	89	,	357	,	13	,	805	,	5527	};
            const unsigned long	dim980KuoInit[]		=	{	1	,	3	,	1	,	9	,	13	,	15	,	59	,	95	,	467	,	875	,	217	,	3345	,	1579	};
            const unsigned long	dim981KuoInit[]		=	{	1	,	3	,	1	,	15	,	29	,	33	,	11	,	121	,	243	,	429	,	1017	,	1511	,	6289	};
            const unsigned long	dim982KuoInit[]		=	{	1	,	3	,	7	,	1	,	27	,	41	,	125	,	121	,	265	,	695	,	155	,	3933	,	661	};
            const unsigned long	dim983KuoInit[]		=	{	1	,	1	,	3	,	7	,	5	,	3	,	95	,	213	,	385	,	183	,	1693	,	2685	,	5647	};
            const unsigned long	dim984KuoInit[]		=	{	1	,	1	,	5	,	5	,	23	,	29	,	21	,	193	,	327	,	965	,	49	,	2157	,	1539	};
            const unsigned long	dim985KuoInit[]		=	{	1	,	3	,	1	,	7	,	15	,	15	,	103	,	191	,	249	,	347	,	283	,	2459	,	5957	};
            const unsigned long	dim986KuoInit[]		=	{	1	,	1	,	7	,	5	,	21	,	43	,	29	,	205	,	125	,	63	,	1645	,	3357	,	3333	};
            const unsigned long	dim987KuoInit[]		=	{	1	,	3	,	1	,	5	,	11	,	21	,	109	,	213	,	123	,	587	,	2047	,	343	,	6669	};
            const unsigned long	dim988KuoInit[]		=	{	1	,	1	,	5	,	5	,	25	,	63	,	75	,	237	,	421	,	883	,	801	,	2403	,	4723	};
            const unsigned long	dim989KuoInit[]		=	{	1	,	3	,	5	,	11	,	13	,	63	,	87	,	107	,	411	,	3	,	139	,	3097	,	6109	};
            const unsigned long	dim990KuoInit[]		=	{	1	,	3	,	7	,	9	,	19	,	31	,	33	,	23	,	499	,	105	,	471	,	2311	,	439	};
            const unsigned long	dim991KuoInit[]		=	{	1	,	3	,	3	,	13	,	13	,	39	,	19	,	253	,	53	,	315	,	1381	,	3875	,	5741	};
            const unsigned long	dim992KuoInit[]		=	{	1	,	1	,	1	,	7	,	19	,	9	,	47	,	169	,	265	,	1005	,	159	,	2957	,	2987	};
            const unsigned long	dim993KuoInit[]		=	{	1	,	3	,	3	,	3	,	1	,	35	,	107	,	77	,	47	,	89	,	1901	,	1719	,	2873	};
            const unsigned long	dim994KuoInit[]		=	{	1	,	1	,	5	,	7	,	7	,	51	,	87	,	91	,	45	,	991	,	1369	,	3759	,	3511	};
            const unsigned long	dim995KuoInit[]		=	{	1	,	3	,	7	,	15	,	17	,	51	,	93	,	43	,	71	,	213	,	1637	,	2617	,	1767	};
            const unsigned long	dim996KuoInit[]		=	{	1	,	1	,	5	,	15	,	29	,	63	,	81	,	15	,	409	,	779	,	1419	,	497	,	463	};
            const unsigned long	dim997KuoInit[]		=	{	1	,	1	,	7	,	1	,	1	,	31	,	21	,	167	,	445	,	893	,	1687	,	603	,	3211	};
            const unsigned long	dim998KuoInit[]		=	{	1	,	3	,	7	,	15	,	19	,	3	,	41	,	53	,	175	,	971	,	1873	,	43	,	4795	};
            const unsigned long	dim999KuoInit[]		=	{	1	,	3	,	5	,	15	,	21	,	59	,	77	,	67	,	495	,	547	,	411	,	2373	,	5187	};
            const unsigned long	dim1000KuoInit[]		=	{	1	,	1	,	5	,	1	,	27	,	45	,	119	,	201	,	237	,	499	,	261	,	3331	,	3973	};
            const unsigned long	dim1001KuoInit[]		=	{	1	,	3	,	5	,	9	,	17	,	31	,	33	,	15	,	237	,	135	,	1751	,	297	,	6331	};
            const unsigned long	dim1002KuoInit[]		=	{	1	,	3	,	7	,	9	,	15	,	1	,	11	,	135	,	355	,	617	,	805	,	1201	,	457	};
            const unsigned long	dim1003KuoInit[]		=	{	1	,	1	,	7	,	5	,	19	,	55	,	21	,	121	,	205	,	999	,	1719	,	2815	,	6753	};
            const unsigned long	dim1004KuoInit[]		=	{	1	,	1	,	5	,	3	,	9	,	5	,	7	,	231	,	323	,	953	,	1811	,	2847	,	2059	};
            const unsigned long	dim1005KuoInit[]		=	{	1	,	3	,	3	,	9	,	17	,	15	,	85	,	3	,	259	,	989	,	287	,	3995	,	2677	};
            const unsigned long	dim1006KuoInit[]		=	{	1	,	1	,	3	,	11	,	13	,	61	,	69	,	143	,	167	,	187	,	481	,	2359	,	1913	};
            const unsigned long	dim1007KuoInit[]		=	{	1	,	1	,	5	,	13	,	29	,	3	,	29	,	247	,	407	,	733	,	677	,	99	,	1625	};
            const unsigned long	dim1008KuoInit[]		=	{	1	,	1	,	7	,	13	,	7	,	17	,	67	,	115	,	299	,	765	,	1885	,	2769	,	1619	};
            const unsigned long	dim1009KuoInit[]		=	{	1	,	1	,	3	,	3	,	17	,	7	,	27	,	61	,	433	,	323	,	1091	,	1309	,	7239	};
            const unsigned long	dim1010KuoInit[]		=	{	1	,	1	,	7	,	3	,	19	,	63	,	41	,	157	,	43	,	743	,	675	,	2149	,	6729	};
            const unsigned long	dim1011KuoInit[]		=	{	1	,	1	,	5	,	1	,	21	,	35	,	45	,	145	,	235	,	185	,	1563	,	1117	,	3669	};
            const unsigned long	dim1012KuoInit[]		=	{	1	,	3	,	1	,	9	,	9	,	37	,	105	,	61	,	259	,	935	,	793	,	823	,	1765	};
            const unsigned long	dim1013KuoInit[]		=	{	1	,	3	,	3	,	11	,	25	,	41	,	35	,	11	,	389	,	399	,	339	,	859	,	4299	};
            const unsigned long	dim1014KuoInit[]		=	{	1	,	3	,	5	,	13	,	7	,	29	,	53	,	91	,	185	,	163	,	1689	,	983	,	4545	};
            const unsigned long	dim1015KuoInit[]		=	{	1	,	3	,	7	,	11	,	19	,	35	,	45	,	141	,	445	,	469	,	1827	,	3523	,	377	};
            const unsigned long	dim1016KuoInit[]		=	{	1	,	3	,	5	,	13	,	27	,	29	,	35	,	199	,	73	,	163	,	1591	,	1021	,	2867	};
            const unsigned long	dim1017KuoInit[]		=	{	1	,	1	,	1	,	15	,	7	,	61	,	95	,	95	,	147	,	959	,	971	,	649	,	5047	};
            const unsigned long	dim1018KuoInit[]		=	{	1	,	1	,	1	,	15	,	11	,	23	,	49	,	231	,	359	,	677	,	1401	,	2889	,	3799	};
            const unsigned long	dim1019KuoInit[]		=	{	1	,	3	,	7	,	11	,	11	,	15	,	119	,	103	,	403	,	983	,	399	,	321	,	437	};
            const unsigned long	dim1020KuoInit[]		=	{	1	,	3	,	7	,	1	,	29	,	13	,	83	,	55	,	175	,	835	,	1637	,	209	,	1923	};
            const unsigned long	dim1021KuoInit[]		=	{	1	,	1	,	5	,	13	,	25	,	37	,	5	,	239	,	227	,	229	,	243	,	1837	,	4821	};
            const unsigned long	dim1022KuoInit[]		=	{	1	,	1	,	7	,	9	,	9	,	59	,	29	,	135	,	227	,	473	,	1759	,	3711	,	4113	};
            const unsigned long	dim1023KuoInit[]		=	{	1	,	3	,	3	,	3	,	15	,	3	,	53	,	225	,	165	,	375	,	537	,	331	,	5085	};
            const unsigned long	dim1024KuoInit[]		=	{	1	,	1	,	1	,	5	,	15	,	41	,	57	,	165	,	45	,	137	,	931	,	1015	,	6451	};
            const unsigned long	dim1025KuoInit[]		=	{	1	,	3	,	5	,	7	,	5	,	47	,	35	,	249	,	89	,	111	,	1275	,	529	,	3297	};
            const unsigned long	dim1026KuoInit[]		=	{	1	,	3	,	1	,	1	,	27	,	21	,	65	,	193	,	381	,	861	,	59	,	1457	,	2447	};
            const unsigned long	dim1027KuoInit[]		=	{	1	,	1	,	7	,	15	,	17	,	25	,	75	,	171	,	263	,	401	,	1369	,	2609	,	6265	};
            const unsigned long	dim1028KuoInit[]		=	{	1	,	1	,	3	,	7	,	17	,	61	,	75	,	77	,	487	,	7	,	1565	,	527	,	7213	};
            const unsigned long	dim1029KuoInit[]		=	{	1	,	3	,	5	,	9	,	5	,	45	,	47	,	27	,	163	,	43	,	219	,	2119	,	1793	};
            const unsigned long	dim1030KuoInit[]		=	{	1	,	1	,	3	,	3	,	27	,	9	,	89	,	151	,	487	,	925	,	1257	,	1065	,	3413	};
            const unsigned long	dim1031KuoInit[]		=	{	1	,	1	,	5	,	3	,	11	,	27	,	9	,	161	,	303	,	457	,	1367	,	2319	,	6145	};
            const unsigned long	dim1032KuoInit[]		=	{	1	,	3	,	1	,	13	,	23	,	43	,	115	,	227	,	319	,	469	,	1651	,	1693	,	2095	};
            const unsigned long	dim1033KuoInit[]		=	{	1	,	1	,	1	,	11	,	23	,	5	,	91	,	103	,	393	,	787	,	1557	,	925	,	7189	};
            const unsigned long	dim1034KuoInit[]		=	{	1	,	3	,	5	,	7	,	31	,	5	,	51	,	171	,	375	,	309	,	199	,	1833	,	2741	};
            const unsigned long	dim1035KuoInit[]		=	{	1	,	1	,	5	,	13	,	27	,	59	,	15	,	255	,	205	,	807	,	1407	,	161	,	6955	};
            const unsigned long	dim1036KuoInit[]		=	{	1	,	3	,	5	,	5	,	5	,	35	,	7	,	167	,	265	,	67	,	1133	,	699	,	6225	};
            const unsigned long	dim1037KuoInit[]		=	{	1	,	1	,	1	,	7	,	19	,	31	,	5	,	243	,	279	,	505	,	303	,	2303	,	5367	};
            const unsigned long	dim1038KuoInit[]		=	{	1	,	3	,	1	,	3	,	19	,	47	,	105	,	159	,	309	,	783	,	1845	,	2603	,	4663	};
            const unsigned long	dim1039KuoInit[]		=	{	1	,	1	,	7	,	13	,	13	,	35	,	101	,	123	,	99	,	559	,	1681	,	2751	,	1965	};
            const unsigned long	dim1040KuoInit[]		=	{	1	,	3	,	7	,	5	,	1	,	63	,	29	,	245	,	105	,	469	,	939	,	721	,	6213	};
            const unsigned long	dim1041KuoInit[]		=	{	1	,	3	,	7	,	15	,	11	,	45	,	13	,	121	,	81	,	15	,	1503	,	2203	,	7467	};
            const unsigned long	dim1042KuoInit[]		=	{	1	,	1	,	7	,	11	,	29	,	25	,	59	,	93	,	5	,	551	,	1799	,	2251	,	115	};
            const unsigned long	dim1043KuoInit[]		=	{	1	,	1	,	1	,	7	,	17	,	37	,	81	,	117	,	183	,	301	,	1085	,	3925	,	697	};
            const unsigned long	dim1044KuoInit[]		=	{	1	,	3	,	7	,	7	,	13	,	13	,	5	,	207	,	443	,	723	,	897	,	3481	,	3377	};
            const unsigned long	dim1045KuoInit[]		=	{	1	,	3	,	7	,	13	,	9	,	41	,	25	,	161	,	283	,	3	,	1515	,	2445	,	2179	};
            const unsigned long	dim1046KuoInit[]		=	{	1	,	1	,	1	,	1	,	29	,	17	,	73	,	129	,	499	,	457	,	103	,	2287	,	525	};
            const unsigned long	dim1047KuoInit[]		=	{	1	,	3	,	7	,	11	,	9	,	13	,	95	,	31	,	411	,	185	,	931	,	119	,	711	};
            const unsigned long	dim1048KuoInit[]		=	{	1	,	1	,	3	,	13	,	19	,	53	,	49	,	209	,	217	,	593	,	733	,	4057	,	2853	};
            const unsigned long	dim1049KuoInit[]		=	{	1	,	3	,	5	,	1	,	7	,	63	,	95	,	45	,	209	,	641	,	1767	,	3001	,	4089	};
            const unsigned long	dim1050KuoInit[]		=	{	1	,	1	,	5	,	9	,	9	,	25	,	69	,	85	,	345	,	475	,	613	,	3975	,	2179	};
            const unsigned long	dim1051KuoInit[]		=	{	1	,	1	,	1	,	9	,	19	,	9	,	47	,	51	,	111	,	7	,	627	,	1635	,	3857	};
            const unsigned long	dim1052KuoInit[]		=	{	1	,	3	,	7	,	1	,	25	,	37	,	27	,	253	,	111	,	369	,	235	,	2887	,	5649	};
            const unsigned long	dim1053KuoInit[]		=	{	1	,	1	,	7	,	3	,	15	,	5	,	13	,	181	,	471	,	825	,	121	,	3007	,	777	};
            const unsigned long	dim1054KuoInit[]		=	{	1	,	1	,	1	,	9	,	27	,	41	,	59	,	9	,	391	,	779	,	915	,	3965	,	4431	};
            const unsigned long	dim1055KuoInit[]		=	{	1	,	3	,	1	,	1	,	25	,	27	,	75	,	123	,	385	,	923	,	141	,	3771	,	2227	};
            const unsigned long	dim1056KuoInit[]		=	{	1	,	3	,	3	,	15	,	17	,	29	,	85	,	253	,	261	,	517	,	87	,	621	,	7947	};
            const unsigned long	dim1057KuoInit[]		=	{	1	,	3	,	7	,	5	,	13	,	15	,	83	,	49	,	399	,	875	,	845	,	1731	,	7071	};
            const unsigned long	dim1058KuoInit[]		=	{	1	,	1	,	3	,	5	,	21	,	49	,	93	,	121	,	271	,	921	,	183	,	1533	,	1609	};
            const unsigned long	dim1059KuoInit[]		=	{	1	,	3	,	7	,	7	,	9	,	51	,	89	,	135	,	389	,	151	,	1813	,	3673	,	2099	};
            const unsigned long	dim1060KuoInit[]		=	{	1	,	1	,	1	,	11	,	25	,	63	,	127	,	239	,	329	,	455	,	403	,	363	,	2689	};
            const unsigned long	dim1061KuoInit[]		=	{	1	,	3	,	5	,	13	,	1	,	45	,	65	,	93	,	371	,	577	,	669	,	2433	,	3507	};
            const unsigned long	dim1062KuoInit[]		=	{	1	,	3	,	5	,	9	,	9	,	15	,	13	,	85	,	67	,	809	,	1133	,	129	,	3869	};
            const unsigned long	dim1063KuoInit[]		=	{	1	,	3	,	7	,	15	,	3	,	47	,	31	,	79	,	395	,	227	,	677	,	3399	,	8173	};
            const unsigned long	dim1064KuoInit[]		=	{	1	,	3	,	1	,	11	,	7	,	19	,	81	,	27	,	393	,	49	,	299	,	879	,	5401	};
            const unsigned long	dim1065KuoInit[]		=	{	1	,	1	,	1	,	1	,	3	,	25	,	105	,	67	,	147	,	23	,	749	,	2677	,	2799	};
            const unsigned long	dim1066KuoInit[]		=	{	1	,	1	,	3	,	15	,	3	,	49	,	55	,	131	,	249	,	183	,	399	,	4065	,	4575	};
            const unsigned long	dim1067KuoInit[]		=	{	1	,	3	,	7	,	13	,	21	,	1	,	105	,	167	,	379	,	567	,	1843	,	3019	,	7479	};
            const unsigned long	dim1068KuoInit[]		=	{	1	,	1	,	7	,	3	,	7	,	15	,	65	,	213	,	191	,	735	,	1087	,	1223	,	7827	};
            const unsigned long	dim1069KuoInit[]		=	{	1	,	1	,	3	,	3	,	13	,	35	,	79	,	55	,	169	,	513	,	639	,	1153	,	1129	};
            const unsigned long	dim1070KuoInit[]		=	{	1	,	3	,	5	,	15	,	1	,	37	,	49	,	209	,	355	,	533	,	1073	,	3249	,	4309	};
            const unsigned long	dim1071KuoInit[]		=	{	1	,	1	,	3	,	15	,	23	,	45	,	69	,	195	,	155	,	267	,	291	,	1773	,	3575	};
            const unsigned long	dim1072KuoInit[]		=	{	1	,	3	,	1	,	13	,	25	,	3	,	51	,	175	,	505	,	467	,	1909	,	1929	,	4909	};
            const unsigned long	dim1073KuoInit[]		=	{	1	,	3	,	3	,	1	,	31	,	51	,	99	,	119	,	505	,	57	,	1987	,	1303	,	4273	};
            const unsigned long	dim1074KuoInit[]		=	{	1	,	3	,	7	,	9	,	19	,	49	,	61	,	147	,	29	,	1001	,	633	,	963	,	5601	};
            const unsigned long	dim1075KuoInit[]		=	{	1	,	3	,	5	,	7	,	1	,	13	,	101	,	133	,	31	,	263	,	2033	,	575	,	983	};
            const unsigned long	dim1076KuoInit[]		=	{	1	,	1	,	1	,	15	,	21	,	29	,	101	,	143	,	263	,	601	,	1239	,	1187	,	3045	};
            const unsigned long	dim1077KuoInit[]		=	{	1	,	3	,	3	,	15	,	5	,	49	,	7	,	165	,	411	,	517	,	619	,	1517	,	1839	};
            const unsigned long	dim1078KuoInit[]		=	{	1	,	3	,	5	,	15	,	29	,	45	,	71	,	245	,	67	,	469	,	1793	,	3353	,	7799	};
            const unsigned long	dim1079KuoInit[]		=	{	1	,	1	,	5	,	7	,	15	,	49	,	5	,	183	,	345	,	585	,	1877	,	2205	,	7963	};
            const unsigned long	dim1080KuoInit[]		=	{	1	,	3	,	1	,	1	,	7	,	39	,	85	,	183	,	131	,	527	,	1731	,	1899	,	3813	};
            const unsigned long	dim1081KuoInit[]		=	{	1	,	1	,	7	,	9	,	29	,	23	,	77	,	233	,	77	,	699	,	1963	,	171	,	6557	};
            const unsigned long	dim1082KuoInit[]		=	{	1	,	3	,	5	,	15	,	5	,	3	,	79	,	199	,	279	,	279	,	399	,	769	,	1661	};
            const unsigned long	dim1083KuoInit[]		=	{	1	,	3	,	7	,	1	,	17	,	61	,	61	,	227	,	289	,	541	,	379	,	3155	,	5791	};
            const unsigned long	dim1084KuoInit[]		=	{	1	,	3	,	3	,	7	,	31	,	37	,	73	,	215	,	47	,	895	,	1511	,	305	,	5247	};
            const unsigned long	dim1085KuoInit[]		=	{	1	,	1	,	7	,	5	,	31	,	47	,	123	,	191	,	175	,	875	,	501	,	2203	,	1781	};
            const unsigned long	dim1086KuoInit[]		=	{	1	,	3	,	3	,	3	,	13	,	5	,	43	,	145	,	183	,	705	,	1143	,	3113	,	1959	};
            const unsigned long	dim1087KuoInit[]		=	{	1	,	3	,	7	,	1	,	23	,	53	,	93	,	133	,	13	,	987	,	1711	,	2241	,	7721	};
            const unsigned long	dim1088KuoInit[]		=	{	1	,	3	,	1	,	7	,	3	,	19	,	123	,	65	,	421	,	891	,	159	,	1633	,	733	};
            const unsigned long	dim1089KuoInit[]		=	{	1	,	3	,	3	,	3	,	23	,	59	,	127	,	109	,	315	,	939	,	1177	,	1237	,	1805	};
            const unsigned long	dim1090KuoInit[]		=	{	1	,	3	,	5	,	15	,	13	,	19	,	39	,	97	,	437	,	949	,	539	,	1171	,	151	};
            const unsigned long	dim1091KuoInit[]		=	{	1	,	3	,	5	,	9	,	13	,	45	,	107	,	159	,	363	,	727	,	509	,	3051	,	4071	};
            const unsigned long	dim1092KuoInit[]		=	{	1	,	3	,	3	,	1	,	15	,	11	,	93	,	199	,	137	,	947	,	479	,	2127	,	1377	};
            const unsigned long	dim1093KuoInit[]		=	{	1	,	3	,	7	,	15	,	13	,	5	,	67	,	227	,	69	,	795	,	299	,	1845	,	4765	};
            const unsigned long	dim1094KuoInit[]		=	{	1	,	1	,	5	,	7	,	31	,	15	,	11	,	255	,	361	,	599	,	1235	,	1785	,	1611	};
            const unsigned long	dim1095KuoInit[]		=	{	1	,	3	,	5	,	15	,	29	,	21	,	85	,	49	,	101	,	785	,	1071	,	1333	,	1691	};
            const unsigned long	dim1096KuoInit[]		=	{	1	,	3	,	1	,	1	,	19	,	11	,	23	,	157	,	201	,	67	,	789	,	999	,	4281	};
            const unsigned long	dim1097KuoInit[]		=	{	1	,	1	,	7	,	3	,	7	,	61	,	109	,	253	,	455	,	863	,	81	,	3185	,	7105	};
            const unsigned long	dim1098KuoInit[]		=	{	1	,	1	,	1	,	1	,	11	,	29	,	51	,	43	,	211	,	103	,	1333	,	3425	,	6669	};
            const unsigned long	dim1099KuoInit[]		=	{	1	,	3	,	3	,	3	,	19	,	15	,	25	,	21	,	9	,	563	,	1737	,	159	,	1827	};
            const unsigned long	dim1100KuoInit[]		=	{	1	,	3	,	5	,	11	,	9	,	35	,	19	,	145	,	159	,	345	,	475	,	1453	,	7807	};
            const unsigned long	dim1101KuoInit[]		=	{	1	,	3	,	7	,	9	,	19	,	41	,	61	,	63	,	77	,	3	,	997	,	223	,	5493	};
            const unsigned long	dim1102KuoInit[]		=	{	1	,	3	,	7	,	15	,	25	,	17	,	87	,	99	,	441	,	973	,	713	,	631	,	4039	};
            const unsigned long	dim1103KuoInit[]		=	{	1	,	3	,	5	,	15	,	17	,	19	,	89	,	11	,	321	,	143	,	11	,	915	,	3359	};
            const unsigned long	dim1104KuoInit[]		=	{	1	,	3	,	7	,	1	,	11	,	55	,	43	,	3	,	1	,	893	,	1845	,	1237	,	693	};		
            const unsigned long	dim1105KuoInit[]		=	{	1	,	3	,	7	,	13	,	25	,	21	,	93	,	43	,	127	,	197	,	1349	,	1727	,	1133	};		
            const unsigned long	dim1106KuoInit[]		=	{	1	,	3	,	7	,	15	,	29	,	57	,	19	,	35	,	37	,	81	,	1599	,	3469	,	4327	};		
            const unsigned long	dim1107KuoInit[]		=	{	1	,	1	,	7	,	9	,	15	,	47	,	119	,	203	,	301	,	835	,	1221	,	2771	,	2023	};		
            const unsigned long	dim1108KuoInit[]		=	{	1	,	3	,	5	,	9	,	25	,	43	,	109	,	51	,	431	,	189	,	255	,	2595	,	2447	};		
            const unsigned long	dim1109KuoInit[]		=	{	1	,	1	,	5	,	5	,	13	,	3	,	113	,	109	,	321	,	837	,	159	,	2617	,	2277	};		
            const unsigned long	dim1110KuoInit[]		=	{	1	,	1	,	1	,	11	,	29	,	43	,	33	,	173	,	47	,	395	,	1113	,	1711	,	7909	};		
            const unsigned long	dim1111KuoInit[]		=	{	1	,	1	,	3	,	15	,	31	,	45	,	37	,	95	,	13	,	449	,	1377	,	2117	,	6607	,	11053	};
            const unsigned long	dim1112KuoInit[]		=	{	1	,	1	,	1	,	9	,	21	,	7	,	127	,	71	,	11	,	271	,	65	,	3681	,	4209	,	3183	};
            const unsigned long	dim1113KuoInit[]		=	{	1	,	3	,	3	,	15	,	19	,	27	,	99	,	243	,	371	,	77	,	629	,	3989	,	3105	,	6287	};
            const unsigned long	dim1114KuoInit[]		=	{	1	,	3	,	1	,	15	,	7	,	35	,	115	,	237	,	403	,	273	,	697	,	117	,	6887	,	3243	};
            const unsigned long	dim1115KuoInit[]		=	{	1	,	1	,	5	,	9	,	29	,	1	,	117	,	101	,	483	,	909	,	867	,	2051	,	7081	,	14905	};
            const unsigned long	dim1116KuoInit[]		=	{	1	,	1	,	7	,	13	,	21	,	49	,	107	,	101	,	121	,	979	,	309	,	1871	,	7339	,	4213	};
            const unsigned long	dim1117KuoInit[]		=	{	1	,	1	,	7	,	13	,	9	,	25	,	13	,	61	,	409	,	469	,	411	,	2041	,	7685	,	15501	};
            const unsigned long	dim1118KuoInit[]		=	{	1	,	1	,	7	,	5	,	1	,	29	,	109	,	125	,	63	,	829	,	175	,	957	,	4781	,	12473	};
            const unsigned long	dim1119KuoInit[]		=	{	1	,	3	,	3	,	1	,	11	,	53	,	17	,	171	,	393	,	165	,	51	,	737	,	1697	,	4137	};
            const unsigned long	dim1120KuoInit[]		=	{	1	,	3	,	7	,	9	,	15	,	45	,	49	,	219	,	105	,	121	,	549	,	991	,	1337	,	11311	};
            const unsigned long	dim1121KuoInit[]		=	{	1	,	3	,	3	,	5	,	27	,	9	,	39	,	207	,	85	,	185	,	901	,	3077	,	5295	,	3933	};
            const unsigned long	dim1122KuoInit[]		=	{	1	,	1	,	5	,	7	,	23	,	29	,	95	,	63	,	125	,	447	,	573	,	2053	,	4621	,	10135	};
            const unsigned long	dim1123KuoInit[]		=	{	1	,	1	,	1	,	7	,	23	,	45	,	3	,	109	,	335	,	339	,	1453	,	3121	,	37	,	9155	};
            const unsigned long	dim1124KuoInit[]		=	{	1	,	3	,	1	,	13	,	9	,	25	,	27	,	67	,	35	,	573	,	215	,	1969	,	7083	,	15729	};
            const unsigned long	dim1125KuoInit[]		=	{	1	,	3	,	1	,	9	,	21	,	3	,	21	,	143	,	131	,	613	,	299	,	1127	,	5647	,	15007	};
            const unsigned long	dim1126KuoInit[]		=	{	1	,	3	,	7	,	9	,	29	,	47	,	53	,	223	,	409	,	633	,	535	,	2423	,	5763	,	11633	};
            const unsigned long	dim1127KuoInit[]		=	{	1	,	3	,	1	,	9	,	1	,	21	,	5	,	1	,	359	,	547	,	285	,	779	,	2091	,	11785	};
            const unsigned long	dim1128KuoInit[]		=	{	1	,	3	,	7	,	7	,	1	,	31	,	61	,	245	,	331	,	45	,	1559	,	3223	,	7577	,	2157	};
            const unsigned long	dim1129KuoInit[]		=	{	1	,	1	,	1	,	7	,	3	,	27	,	37	,	191	,	229	,	205	,	1535	,	887	,	701	,	10867	};
            const unsigned long	dim1130KuoInit[]		=	{	1	,	3	,	3	,	15	,	21	,	53	,	61	,	177	,	485	,	575	,	681	,	1447	,	1427	,	11279	};
            const unsigned long	dim1131KuoInit[]		=	{	1	,	1	,	1	,	3	,	17	,	3	,	5	,	49	,	311	,	507	,	995	,	3971	,	471	,	3045	};
            const unsigned long	dim1132KuoInit[]		=	{	1	,	1	,	3	,	9	,	7	,	45	,	73	,	183	,	419	,	635	,	523	,	1793	,	7549	,	9727	};
            const unsigned long	dim1133KuoInit[]		=	{	1	,	1	,	7	,	3	,	19	,	33	,	109	,	91	,	19	,	825	,	1629	,	2111	,	2673	,	6677	};
            const unsigned long	dim1134KuoInit[]		=	{	1	,	1	,	3	,	9	,	31	,	13	,	71	,	199	,	113	,	77	,	371	,	3117	,	7799	,	15919	};
            const unsigned long	dim1135KuoInit[]		=	{	1	,	1	,	7	,	5	,	27	,	41	,	91	,	183	,	75	,	159	,	911	,	2081	,	7065	,	16313	};
            const unsigned long	dim1136KuoInit[]		=	{	1	,	3	,	1	,	15	,	17	,	29	,	65	,	9	,	399	,	259	,	671	,	525	,	1393	,	13367	};
            const unsigned long	dim1137KuoInit[]		=	{	1	,	3	,	3	,	13	,	1	,	29	,	101	,	187	,	231	,	413	,	1245	,	269	,	201	,	1673	};
            const unsigned long	dim1138KuoInit[]		=	{	1	,	1	,	7	,	3	,	27	,	47	,	11	,	149	,	49	,	309	,	1923	,	1187	,	3165	,	7927	};
            const unsigned long	dim1139KuoInit[]		=	{	1	,	1	,	1	,	5	,	31	,	57	,	29	,	203	,	11	,	305	,	1771	,	2997	,	377	,	5777	};
            const unsigned long	dim1140KuoInit[]		=	{	1	,	1	,	7	,	5	,	1	,	29	,	29	,	205	,	93	,	821	,	1583	,	2991	,	4137	,	14967	};
            const unsigned long	dim1141KuoInit[]		=	{	1	,	1	,	3	,	3	,	7	,	57	,	13	,	179	,	261	,	547	,	219	,	1979	,	4951	,	2529	};
            const unsigned long	dim1142KuoInit[]		=	{	1	,	3	,	1	,	5	,	3	,	51	,	89	,	7	,	121	,	611	,	1655	,	3639	,	1991	,	9911	};
            const unsigned long	dim1143KuoInit[]		=	{	1	,	3	,	3	,	15	,	1	,	21	,	9	,	173	,	93	,	579	,	1685	,	767	,	5183	,	3965	};
            const unsigned long	dim1144KuoInit[]		=	{	1	,	1	,	3	,	7	,	15	,	47	,	55	,	113	,	27	,	941	,	1633	,	3791	,	1705	,	16293	};
            const unsigned long	dim1145KuoInit[]		=	{	1	,	3	,	3	,	1	,	7	,	41	,	11	,	229	,	367	,	997	,	543	,	933	,	4249	,	7049	};
            const unsigned long	dim1146KuoInit[]		=	{	1	,	1	,	1	,	13	,	7	,	7	,	25	,	87	,	213	,	277	,	627	,	721	,	6161	,	14877	};
            const unsigned long	dim1147KuoInit[]		=	{	1	,	3	,	5	,	7	,	25	,	61	,	47	,	103	,	199	,	983	,	543	,	1935	,	4663	,	3597	};
            const unsigned long	dim1148KuoInit[]		=	{	1	,	3	,	1	,	15	,	15	,	49	,	125	,	27	,	153	,	521	,	351	,	1129	,	5897	,	11325	};
            const unsigned long	dim1149KuoInit[]		=	{	1	,	3	,	1	,	13	,	13	,	35	,	81	,	53	,	217	,	739	,	963	,	3441	,	6843	,	3505	};
            const unsigned long	dim1150KuoInit[]		=	{	1	,	3	,	7	,	1	,	17	,	35	,	119	,	187	,	475	,	919	,	1325	,	3269	,	677	,	617	};
            const unsigned long	dim1151KuoInit[]		=	{	1	,	1	,	7	,	15	,	7	,	15	,	91	,	99	,	259	,	37	,	1461	,	1145	,	6973	,	15643	};
            const unsigned long	dim1152KuoInit[]		=	{	1	,	1	,	5	,	15	,	13	,	11	,	27	,	75	,	9	,	611	,	1331	,	2057	,	863	,	6245	};
            const unsigned long	dim1153KuoInit[]		=	{	1	,	1	,	1	,	15	,	3	,	25	,	117	,	45	,	141	,	839	,	1201	,	629	,	6289	,	12231	};
            const unsigned long	dim1154KuoInit[]		=	{	1	,	1	,	3	,	13	,	17	,	61	,	23	,	243	,	349	,	153	,	1747	,	3107	,	7763	,	10067	};
            const unsigned long	dim1155KuoInit[]		=	{	1	,	1	,	1	,	1	,	21	,	25	,	53	,	67	,	97	,	851	,	1921	,	2247	,	5769	,	5779	};
            const unsigned long	dim1156KuoInit[]		=	{	1	,	3	,	3	,	3	,	9	,	37	,	93	,	247	,	287	,	609	,	1643	,	427	,	4269	,	15923	};
            const unsigned long	dim1157KuoInit[]		=	{	1	,	3	,	3	,	15	,	7	,	35	,	111	,	105	,	357	,	539	,	319	,	993	,	6155	,	6587	};
            const unsigned long	dim1158KuoInit[]		=	{	1	,	1	,	3	,	1	,	13	,	33	,	99	,	73	,	89	,	231	,	1537	,	309	,	3837	,	6669	};
            const unsigned long	dim1159KuoInit[]		=	{	1	,	3	,	1	,	1	,	9	,	25	,	105	,	197	,	399	,	259	,	1381	,	1631	,	3551	,	4593	};
            const unsigned long	dim1160KuoInit[]		=	{	1	,	3	,	1	,	3	,	7	,	63	,	113	,	65	,	109	,	529	,	1451	,	1169	,	3321	,	6811	};
            const unsigned long	dim1161KuoInit[]		=	{	1	,	1	,	1	,	5	,	11	,	55	,	109	,	27	,	65	,	199	,	199	,	2745	,	2009	,	14853	};
            const unsigned long	dim1162KuoInit[]		=	{	1	,	3	,	1	,	9	,	13	,	31	,	109	,	119	,	431	,	363	,	1191	,	703	,	2617	,	6817	};
            const unsigned long	dim1163KuoInit[]		=	{	1	,	1	,	3	,	11	,	27	,	1	,	107	,	11	,	457	,	495	,	215	,	1459	,	4137	,	13007	};
            const unsigned long	dim1164KuoInit[]		=	{	1	,	1	,	7	,	3	,	7	,	29	,	11	,	19	,	159	,	821	,	1881	,	3277	,	1303	,	7145	};
            const unsigned long	dim1165KuoInit[]		=	{	1	,	3	,	3	,	11	,	25	,	11	,	125	,	241	,	197	,	863	,	423	,	2107	,	5885	,	14279	};
            const unsigned long	dim1166KuoInit[]		=	{	1	,	3	,	3	,	1	,	7	,	29	,	97	,	249	,	333	,	945	,	1081	,	2405	,	109	,	2869	};
            const unsigned long	dim1167KuoInit[]		=	{	1	,	3	,	3	,	3	,	27	,	57	,	89	,	243	,	481	,	431	,	269	,	3371	,	3667	,	13293	};
            const unsigned long	dim1168KuoInit[]		=	{	1	,	3	,	7	,	7	,	23	,	3	,	91	,	57	,	79	,	165	,	629	,	2771	,	7927	,	2689	};
            const unsigned long	dim1169KuoInit[]		=	{	1	,	1	,	1	,	11	,	17	,	51	,	81	,	147	,	19	,	501	,	861	,	159	,	2117	,	6777	};
            const unsigned long	dim1170KuoInit[]		=	{	1	,	3	,	3	,	15	,	7	,	9	,	51	,	197	,	97	,	983	,	133	,	1819	,	4449	,	7821	};
            const unsigned long	dim1171KuoInit[]		=	{	1	,	3	,	3	,	7	,	21	,	55	,	61	,	171	,	461	,	741	,	1901	,	2195	,	5477	,	1681	};
            const unsigned long	dim1172KuoInit[]		=	{	1	,	1	,	3	,	1	,	13	,	37	,	73	,	151	,	479	,	561	,	1483	,	2679	,	295	,	7325	};
            const unsigned long	dim1173KuoInit[]		=	{	1	,	3	,	3	,	9	,	11	,	17	,	127	,	115	,	371	,	979	,	1391	,	2189	,	2805	,	7471	};
            const unsigned long	dim1174KuoInit[]		=	{	1	,	1	,	3	,	13	,	31	,	25	,	65	,	249	,	91	,	649	,	1103	,	493	,	6341	,	10313	};
            const unsigned long	dim1175KuoInit[]		=	{	1	,	1	,	1	,	3	,	7	,	31	,	85	,	201	,	277	,	771	,	1007	,	69	,	3357	,	15059	};
            const unsigned long	dim1176KuoInit[]		=	{	1	,	1	,	1	,	7	,	17	,	17	,	9	,	229	,	269	,	591	,	945	,	3003	,	6723	,	16101	};
            const unsigned long	dim1177KuoInit[]		=	{	1	,	1	,	3	,	5	,	1	,	29	,	119	,	181	,	269	,	269	,	1781	,	2935	,	6265	,	13989	};
            const unsigned long	dim1178KuoInit[]		=	{	1	,	3	,	1	,	11	,	13	,	51	,	117	,	141	,	467	,	521	,	449	,	797	,	3447	,	11693	};
            const unsigned long	dim1179KuoInit[]		=	{	1	,	1	,	5	,	15	,	3	,	57	,	61	,	209	,	423	,	213	,	1663	,	3735	,	5229	,	11809	};
            const unsigned long	dim1180KuoInit[]		=	{	1	,	3	,	7	,	9	,	13	,	51	,	113	,	127	,	229	,	793	,	905	,	3271	,	7501	,	10801	};
            const unsigned long	dim1181KuoInit[]		=	{	1	,	3	,	7	,	7	,	5	,	19	,	43	,	87	,	9	,	981	,	221	,	3925	,	2095	,	7185	};
            const unsigned long	dim1182KuoInit[]		=	{	1	,	1	,	7	,	15	,	23	,	35	,	113	,	175	,	383	,	699	,	1617	,	3771	,	5847	,	9097	};
            const unsigned long	dim1183KuoInit[]		=	{	1	,	3	,	1	,	7	,	25	,	23	,	27	,	77	,	491	,	211	,	1785	,	199	,	4865	,	10147	};
            const unsigned long	dim1184KuoInit[]		=	{	1	,	3	,	1	,	11	,	25	,	39	,	15	,	15	,	191	,	889	,	457	,	2647	,	4555	,	4799	};
            const unsigned long	dim1185KuoInit[]		=	{	1	,	1	,	7	,	5	,	15	,	1	,	117	,	169	,	245	,	917	,	1025	,	3805	,	1455	,	3051	};
            const unsigned long	dim1186KuoInit[]		=	{	1	,	3	,	7	,	5	,	7	,	15	,	43	,	13	,	359	,	685	,	75	,	271	,	555	,	3595	};
            const unsigned long	dim1187KuoInit[]		=	{	1	,	3	,	7	,	5	,	19	,	9	,	29	,	15	,	139	,	13	,	753	,	205	,	677	,	5945	};
            const unsigned long	dim1188KuoInit[]		=	{	1	,	3	,	1	,	9	,	5	,	21	,	99	,	77	,	287	,	663	,	589	,	229	,	5255	,	4177	};
            const unsigned long	dim1189KuoInit[]		=	{	1	,	1	,	1	,	9	,	15	,	47	,	21	,	149	,	461	,	871	,	577	,	3393	,	6879	,	6353	};
            const unsigned long	dim1190KuoInit[]		=	{	1	,	3	,	3	,	3	,	21	,	33	,	29	,	25	,	195	,	1	,	1817	,	3437	,	4265	,	16121	};
            const unsigned long	dim1191KuoInit[]		=	{	1	,	3	,	1	,	7	,	27	,	31	,	93	,	141	,	471	,	735	,	1737	,	2997	,	3529	,	12285	};
            const unsigned long	dim1192KuoInit[]		=	{	1	,	1	,	3	,	15	,	15	,	49	,	105	,	157	,	165	,	593	,	651	,	3331	,	4333	,	8607	};
            const unsigned long	dim1193KuoInit[]		=	{	1	,	3	,	7	,	1	,	5	,	35	,	73	,	53	,	461	,	41	,	1407	,	357	,	3537	,	1123	};
            const unsigned long	dim1194KuoInit[]		=	{	1	,	1	,	7	,	9	,	15	,	39	,	83	,	23	,	501	,	3	,	743	,	3819	,	5491	,	15317	};
            const unsigned long	dim1195KuoInit[]		=	{	1	,	1	,	7	,	15	,	31	,	29	,	125	,	173	,	465	,	347	,	1515	,	2871	,	3861	,	9353	};
            const unsigned long	dim1196KuoInit[]		=	{	1	,	1	,	7	,	5	,	9	,	31	,	121	,	221	,	491	,	579	,	313	,	3549	,	711	,	12501	};
            const unsigned long	dim1197KuoInit[]		=	{	1	,	1	,	7	,	15	,	7	,	47	,	15	,	175	,	201	,	399	,	609	,	749	,	3587	,	10091	};
            const unsigned long	dim1198KuoInit[]		=	{	1	,	1	,	5	,	11	,	1	,	39	,	43	,	69	,	429	,	87	,	2017	,	1961	,	5679	,	3179	};
            const unsigned long	dim1199KuoInit[]		=	{	1	,	1	,	3	,	3	,	29	,	51	,	27	,	13	,	377	,	99	,	1291	,	247	,	2657	,	4793	};
            const unsigned long	dim1200KuoInit[]		=	{	1	,	1	,	1	,	1	,	23	,	63	,	119	,	33	,	405	,	689	,	517	,	1331	,	1047	,	403	};
            const unsigned long	dim1201KuoInit[]		=	{	1	,	1	,	1	,	5	,	9	,	27	,	121	,	49	,	317	,	181	,	623	,	1789	,	5867	,	11009	};
            const unsigned long	dim1202KuoInit[]		=	{	1	,	1	,	7	,	15	,	15	,	63	,	113	,	129	,	299	,	889	,	361	,	2339	,	3893	,	3953	};
            const unsigned long	dim1203KuoInit[]		=	{	1	,	1	,	1	,	15	,	9	,	51	,	73	,	77	,	511	,	745	,	639	,	1261	,	439	,	13409	};
            const unsigned long	dim1204KuoInit[]		=	{	1	,	3	,	5	,	3	,	29	,	15	,	37	,	31	,	47	,	901	,	1383	,	1979	,	7341	,	13411	};
            const unsigned long	dim1205KuoInit[]		=	{	1	,	3	,	7	,	5	,	31	,	59	,	87	,	35	,	123	,	943	,	1181	,	3877	,	4821	,	9231	};
            const unsigned long	dim1206KuoInit[]		=	{	1	,	3	,	5	,	13	,	3	,	47	,	123	,	89	,	41	,	883	,	241	,	2055	,	1723	,	6747	};
            const unsigned long	dim1207KuoInit[]		=	{	1	,	1	,	7	,	5	,	21	,	29	,	89	,	101	,	423	,	279	,	29	,	2089	,	6233	,	7343	};
            const unsigned long	dim1208KuoInit[]		=	{	1	,	1	,	7	,	5	,	29	,	25	,	23	,	179	,	13	,	707	,	1603	,	585	,	4159	,	10767	};
            const unsigned long	dim1209KuoInit[]		=	{	1	,	1	,	1	,	1	,	15	,	11	,	55	,	159	,	279	,	613	,	1445	,	613	,	499	,	7095	};
            const unsigned long	dim1210KuoInit[]		=	{	1	,	1	,	1	,	5	,	15	,	61	,	75	,	171	,	173	,	765	,	1763	,	2193	,	7897	,	4559	};
            const unsigned long	dim1211KuoInit[]		=	{	1	,	1	,	1	,	9	,	9	,	57	,	127	,	113	,	7	,	269	,	791	,	619	,	2923	,	6309	};
            const unsigned long	dim1212KuoInit[]		=	{	1	,	1	,	3	,	13	,	21	,	9	,	99	,	1	,	263	,	69	,	1879	,	4079	,	33	,	7313	};
            const unsigned long	dim1213KuoInit[]		=	{	1	,	1	,	7	,	9	,	17	,	1	,	69	,	93	,	147	,	461	,	1777	,	3375	,	4067	,	15307	};
            const unsigned long	dim1214KuoInit[]		=	{	1	,	3	,	7	,	11	,	11	,	7	,	77	,	159	,	125	,	159	,	625	,	1339	,	2301	,	11525	};
            const unsigned long	dim1215KuoInit[]		=	{	1	,	1	,	7	,	9	,	1	,	5	,	23	,	115	,	25	,	755	,	1523	,	2767	,	7667	,	7129	};
            const unsigned long	dim1216KuoInit[]		=	{	1	,	1	,	5	,	5	,	23	,	31	,	41	,	3	,	249	,	69	,	1821	,	155	,	585	,	7905	};
            const unsigned long	dim1217KuoInit[]		=	{	1	,	3	,	7	,	11	,	21	,	35	,	7	,	161	,	409	,	771	,	967	,	2429	,	1	,	5391	};
            const unsigned long	dim1218KuoInit[]		=	{	1	,	1	,	1	,	9	,	9	,	3	,	13	,	125	,	87	,	115	,	1013	,	2103	,	4769	,	7389	};
            const unsigned long	dim1219KuoInit[]		=	{	1	,	3	,	3	,	7	,	27	,	25	,	85	,	119	,	115	,	775	,	189	,	3577	,	5369	,	5213	};
            const unsigned long	dim1220KuoInit[]		=	{	1	,	3	,	7	,	3	,	27	,	49	,	81	,	23	,	223	,	523	,	1939	,	431	,	6247	,	10509	};
            const unsigned long	dim1221KuoInit[]		=	{	1	,	3	,	3	,	3	,	31	,	7	,	119	,	235	,	315	,	431	,	1669	,	1223	,	5785	,	6507	};
            const unsigned long	dim1222KuoInit[]		=	{	1	,	1	,	5	,	9	,	27	,	23	,	35	,	141	,	327	,	395	,	869	,	177	,	3997	,	11191	};
            const unsigned long	dim1223KuoInit[]		=	{	1	,	1	,	7	,	9	,	21	,	15	,	35	,	163	,	273	,	93	,	835	,	3339	,	6109	,	2617	};
            const unsigned long	dim1224KuoInit[]		=	{	1	,	3	,	5	,	9	,	1	,	29	,	107	,	161	,	301	,	27	,	1391	,	211	,	5263	,	10899	};
            const unsigned long	dim1225KuoInit[]		=	{	1	,	3	,	7	,	7	,	13	,	61	,	91	,	179	,	227	,	193	,	1909	,	3759	,	5875	,	6799	};
            const unsigned long	dim1226KuoInit[]		=	{	1	,	3	,	1	,	9	,	17	,	23	,	57	,	117	,	201	,	459	,	1841	,	2371	,	2641	,	13927	};
            const unsigned long	dim1227KuoInit[]		=	{	1	,	3	,	7	,	11	,	1	,	1	,	13	,	139	,	313	,	1	,	1551	,	2693	,	2785	,	6959	};
            const unsigned long	dim1228KuoInit[]		=	{	1	,	3	,	3	,	13	,	1	,	29	,	117	,	67	,	147	,	821	,	1935	,	2913	,	7911	,	4829	};
            const unsigned long	dim1229KuoInit[]		=	{	1	,	1	,	7	,	7	,	19	,	17	,	57	,	33	,	93	,	819	,	1035	,	3493	,	1831	,	5345	};
            const unsigned long	dim1230KuoInit[]		=	{	1	,	3	,	5	,	13	,	9	,	13	,	117	,	219	,	415	,	743	,	381	,	2343	,	3865	,	3915	};
            const unsigned long	dim1231KuoInit[]		=	{	1	,	1	,	5	,	15	,	3	,	25	,	67	,	197	,	59	,	419	,	461	,	1951	,	4657	,	9263	};
            const unsigned long	dim1232KuoInit[]		=	{	1	,	1	,	7	,	15	,	31	,	53	,	33	,	213	,	169	,	231	,	615	,	441	,	7055	,	16079	};
            const unsigned long	dim1233KuoInit[]		=	{	1	,	1	,	1	,	15	,	5	,	27	,	57	,	79	,	373	,	713	,	583	,	3773	,	4239	,	14231	};
            const unsigned long	dim1234KuoInit[]		=	{	1	,	1	,	7	,	15	,	21	,	55	,	119	,	219	,	371	,	321	,	185	,	1103	,	7783	,	8705	};
            const unsigned long	dim1235KuoInit[]		=	{	1	,	3	,	5	,	15	,	3	,	11	,	69	,	127	,	27	,	223	,	867	,	2249	,	597	,	10051	};
            const unsigned long	dim1236KuoInit[]		=	{	1	,	3	,	5	,	7	,	5	,	13	,	99	,	115	,	21	,	659	,	1287	,	1941	,	6505	,	1091	};
            const unsigned long	dim1237KuoInit[]		=	{	1	,	1	,	7	,	1	,	17	,	57	,	57	,	155	,	373	,	729	,	1521	,	3741	,	4935	,	13867	};
            const unsigned long	dim1238KuoInit[]		=	{	1	,	1	,	7	,	1	,	19	,	21	,	39	,	47	,	451	,	67	,	463	,	1849	,	5311	,	5831	};
            const unsigned long	dim1239KuoInit[]		=	{	1	,	3	,	7	,	7	,	29	,	11	,	59	,	161	,	29	,	107	,	1413	,	3915	,	6863	,	5095	};
            const unsigned long	dim1240KuoInit[]		=	{	1	,	1	,	7	,	15	,	13	,	37	,	29	,	91	,	83	,	663	,	1629	,	3005	,	655	,	7601	};
            const unsigned long	dim1241KuoInit[]		=	{	1	,	1	,	3	,	3	,	31	,	47	,	29	,	31	,	509	,	319	,	929	,	1481	,	6255	,	14003	};
            const unsigned long	dim1242KuoInit[]		=	{	1	,	3	,	3	,	3	,	5	,	7	,	97	,	253	,	163	,	801	,	283	,	813	,	6277	,	3201	};
            const unsigned long	dim1243KuoInit[]		=	{	1	,	3	,	3	,	15	,	5	,	59	,	49	,	47	,	259	,	837	,	2003	,	1489	,	3425	,	6863	};
            const unsigned long	dim1244KuoInit[]		=	{	1	,	3	,	5	,	13	,	25	,	55	,	37	,	139	,	347	,	95	,	259	,	3175	,	1043	,	1395	};
            const unsigned long	dim1245KuoInit[]		=	{	1	,	3	,	7	,	5	,	31	,	37	,	73	,	7	,	1	,	883	,	1641	,	1941	,	6885	,	23	};
            const unsigned long	dim1246KuoInit[]		=	{	1	,	1	,	3	,	9	,	13	,	45	,	85	,	67	,	55	,	11	,	1717	,	3461	,	3189	,	15989	};
            const unsigned long	dim1247KuoInit[]		=	{	1	,	1	,	1	,	13	,	27	,	15	,	93	,	39	,	329	,	43	,	175	,	2253	,	1193	,	13257	};
            const unsigned long	dim1248KuoInit[]		=	{	1	,	3	,	3	,	1	,	25	,	41	,	111	,	211	,	99	,	267	,	965	,	1311	,	2661	,	1981	};
            const unsigned long	dim1249KuoInit[]		=	{	1	,	3	,	7	,	15	,	31	,	15	,	37	,	73	,	239	,	505	,	983	,	3299	,	2891	,	8247	};
            const unsigned long	dim1250KuoInit[]		=	{	1	,	1	,	5	,	3	,	19	,	5	,	45	,	55	,	251	,	125	,	977	,	955	,	1169	,	8495	};
            const unsigned long	dim1251KuoInit[]		=	{	1	,	3	,	1	,	3	,	19	,	31	,	121	,	119	,	185	,	43	,	847	,	3939	,	2511	,	10889	};
            const unsigned long	dim1252KuoInit[]		=	{	1	,	3	,	1	,	7	,	7	,	17	,	81	,	37	,	425	,	663	,	1715	,	789	,	661	,	9089	};
            const unsigned long	dim1253KuoInit[]		=	{	1	,	3	,	3	,	11	,	1	,	3	,	17	,	247	,	311	,	369	,	211	,	97	,	6355	,	16297	};
            const unsigned long	dim1254KuoInit[]		=	{	1	,	3	,	7	,	13	,	21	,	53	,	33	,	71	,	73	,	629	,	1321	,	3181	,	6627	,	161	};
            const unsigned long	dim1255KuoInit[]		=	{	1	,	1	,	3	,	5	,	21	,	25	,	21	,	231	,	291	,	585	,	111	,	1651	,	1031	,	13705	};
            const unsigned long	dim1256KuoInit[]		=	{	1	,	3	,	3	,	9	,	5	,	31	,	59	,	51	,	245	,	465	,	37	,	3425	,	4251	,	1121	};
            const unsigned long	dim1257KuoInit[]		=	{	1	,	3	,	7	,	11	,	31	,	45	,	125	,	245	,	335	,	973	,	49	,	1557	,	4847	,	10739	};
            const unsigned long	dim1258KuoInit[]		=	{	1	,	1	,	1	,	3	,	21	,	39	,	127	,	67	,	101	,	9	,	1099	,	1159	,	2483	,	4283	};
            const unsigned long	dim1259KuoInit[]		=	{	1	,	1	,	7	,	13	,	29	,	53	,	49	,	65	,	5	,	775	,	433	,	351	,	493	,	7233	};
            const unsigned long	dim1260KuoInit[]		=	{	1	,	3	,	5	,	5	,	27	,	13	,	73	,	83	,	417	,	31	,	1891	,	1809	,	1103	,	4555	};
            const unsigned long	dim1261KuoInit[]		=	{	1	,	1	,	5	,	9	,	15	,	1	,	25	,	63	,	191	,	251	,	1759	,	429	,	2477	,	12577	};
            const unsigned long	dim1262KuoInit[]		=	{	1	,	1	,	3	,	5	,	3	,	21	,	61	,	245	,	191	,	217	,	845	,	2121	,	2133	,	6045	};
            const unsigned long	dim1263KuoInit[]		=	{	1	,	3	,	3	,	5	,	23	,	33	,	85	,	15	,	57	,	645	,	1109	,	3411	,	2895	,	6885	};
            const unsigned long	dim1264KuoInit[]		=	{	1	,	3	,	3	,	11	,	13	,	41	,	13	,	59	,	45	,	199	,	1145	,	2637	,	6741	,	1273	};
            const unsigned long	dim1265KuoInit[]		=	{	1	,	3	,	3	,	5	,	27	,	33	,	121	,	21	,	75	,	687	,	349	,	2097	,	1195	,	4227	};
            const unsigned long	dim1266KuoInit[]		=	{	1	,	1	,	7	,	9	,	25	,	51	,	71	,	85	,	281	,	825	,	1335	,	277	,	797	,	407	};
            const unsigned long	dim1267KuoInit[]		=	{	1	,	3	,	3	,	15	,	21	,	31	,	19	,	11	,	39	,	821	,	1623	,	97	,	1573	,	13297	};
            const unsigned long	dim1268KuoInit[]		=	{	1	,	1	,	3	,	1	,	25	,	39	,	67	,	113	,	109	,	719	,	1427	,	2539	,	5885	,	13537	};
            const unsigned long	dim1269KuoInit[]		=	{	1	,	1	,	5	,	3	,	15	,	53	,	11	,	139	,	393	,	365	,	709	,	1603	,	4929	,	2651	};
            const unsigned long	dim1270KuoInit[]		=	{	1	,	3	,	7	,	5	,	3	,	11	,	19	,	41	,	123	,	7	,	993	,	3627	,	2027	,	4009	};
            const unsigned long	dim1271KuoInit[]		=	{	1	,	1	,	3	,	7	,	3	,	17	,	63	,	129	,	323	,	303	,	1669	,	1023	,	1259	,	289	};
            const unsigned long	dim1272KuoInit[]		=	{	1	,	3	,	7	,	15	,	15	,	33	,	69	,	229	,	291	,	939	,	969	,	2497	,	4323	,	16299	};
            const unsigned long	dim1273KuoInit[]		=	{	1	,	3	,	5	,	5	,	17	,	35	,	79	,	33	,	471	,	857	,	975	,	297	,	7219	,	12903	};
            const unsigned long	dim1274KuoInit[]		=	{	1	,	1	,	7	,	7	,	21	,	5	,	65	,	53	,	11	,	265	,	1447	,	1405	,	3351	,	2661	};
            const unsigned long	dim1275KuoInit[]		=	{	1	,	3	,	3	,	3	,	1	,	21	,	123	,	123	,	361	,	71	,	1233	,	2673	,	7643	,	9519	};
            const unsigned long	dim1276KuoInit[]		=	{	1	,	1	,	5	,	7	,	27	,	41	,	117	,	239	,	125	,	183	,	1375	,	649	,	5267	,	1735	};
            const unsigned long	dim1277KuoInit[]		=	{	1	,	1	,	1	,	11	,	7	,	33	,	59	,	7	,	37	,	705	,	1055	,	1661	,	6213	,	2083	};
            const unsigned long	dim1278KuoInit[]		=	{	1	,	3	,	1	,	9	,	11	,	61	,	91	,	143	,	113	,	229	,	809	,	431	,	1971	,	12453	};
            const unsigned long	dim1279KuoInit[]		=	{	1	,	1	,	3	,	9	,	17	,	23	,	97	,	249	,	109	,	669	,	1643	,	3305	,	6141	,	1793	};
            const unsigned long	dim1280KuoInit[]		=	{	1	,	1	,	5	,	13	,	3	,	39	,	37	,	107	,	123	,	637	,	869	,	1969	,	4195	,	9431	};
            const unsigned long	dim1281KuoInit[]		=	{	1	,	1	,	7	,	13	,	7	,	25	,	97	,	63	,	395	,	1011	,	161	,	827	,	4001	,	15159	};
            const unsigned long	dim1282KuoInit[]		=	{	1	,	1	,	3	,	7	,	11	,	59	,	11	,	49	,	25	,	691	,	73	,	1171	,	2937	,	13877	};
            const unsigned long	dim1283KuoInit[]		=	{	1	,	3	,	7	,	15	,	1	,	19	,	119	,	129	,	63	,	451	,	1593	,	1207	,	67	,	3621	};
            const unsigned long	dim1284KuoInit[]		=	{	1	,	1	,	1	,	1	,	25	,	27	,	45	,	23	,	111	,	191	,	1205	,	4079	,	1821	,	5571	};
            const unsigned long	dim1285KuoInit[]		=	{	1	,	1	,	7	,	5	,	17	,	37	,	37	,	249	,	81	,	909	,	153	,	209	,	7919	,	10763	};
            const unsigned long	dim1286KuoInit[]		=	{	1	,	3	,	5	,	5	,	19	,	9	,	101	,	193	,	431	,	63	,	1309	,	2693	,	2413	,	14513	};
            const unsigned long	dim1287KuoInit[]		=	{	1	,	3	,	5	,	7	,	13	,	5	,	5	,	203	,	313	,	723	,	1637	,	2725	,	4197	,	3767	};
            const unsigned long	dim1288KuoInit[]		=	{	1	,	3	,	3	,	11	,	1	,	53	,	33	,	245	,	181	,	1001	,	1057	,	2701	,	5861	,	4199	};
            const unsigned long	dim1289KuoInit[]		=	{	1	,	1	,	5	,	3	,	31	,	35	,	19	,	253	,	403	,	429	,	1667	,	1671	,	7491	,	1023	};
            const unsigned long	dim1290KuoInit[]		=	{	1	,	3	,	5	,	7	,	15	,	51	,	41	,	253	,	247	,	883	,	1887	,	1067	,	3227	,	13259	};
            const unsigned long	dim1291KuoInit[]		=	{	1	,	3	,	7	,	15	,	5	,	63	,	9	,	97	,	87	,	11	,	1873	,	2529	,	833	,	5583	};
            const unsigned long	dim1292KuoInit[]		=	{	1	,	1	,	5	,	7	,	17	,	59	,	23	,	225	,	91	,	189	,	923	,	3177	,	599	,	4315	};
            const unsigned long	dim1293KuoInit[]		=	{	1	,	3	,	3	,	11	,	15	,	59	,	85	,	75	,	281	,	935	,	1219	,	3121	,	231	,	13839	};
            const unsigned long	dim1294KuoInit[]		=	{	1	,	1	,	5	,	9	,	15	,	61	,	105	,	27	,	179	,	145	,	827	,	2985	,	6907	,	14961	};
            const unsigned long	dim1295KuoInit[]		=	{	1	,	1	,	1	,	1	,	1	,	63	,	7	,	3	,	449	,	387	,	483	,	603	,	6607	,	13241	};
            const unsigned long	dim1296KuoInit[]		=	{	1	,	1	,	7	,	1	,	9	,	37	,	73	,	255	,	331	,	797	,	679	,	579	,	3015	,	2517	};
            const unsigned long	dim1297KuoInit[]		=	{	1	,	1	,	3	,	5	,	23	,	15	,	97	,	159	,	9	,	469	,	881	,	1935	,	1871	,	7943	};
            const unsigned long	dim1298KuoInit[]		=	{	1	,	1	,	7	,	15	,	31	,	33	,	103	,	247	,	7	,	217	,	397	,	3761	,	3071	,	15357	};
            const unsigned long	dim1299KuoInit[]		=	{	1	,	3	,	3	,	9	,	23	,	25	,	61	,	61	,	495	,	195	,	1615	,	923	,	6583	,	11169	};
            const unsigned long	dim1300KuoInit[]		=	{	1	,	1	,	3	,	9	,	25	,	7	,	67	,	105	,	423	,	519	,	327	,	2789	,	7527	,	13465	};
            const unsigned long	dim1301KuoInit[]		=	{	1	,	1	,	3	,	11	,	29	,	57	,	21	,	47	,	71	,	771	,	1255	,	1469	,	737	,	11533	};
            const unsigned long	dim1302KuoInit[]		=	{	1	,	1	,	5	,	11	,	23	,	31	,	103	,	145	,	19	,	931	,	963	,	2627	,	1419	,	14451	};
            const unsigned long	dim1303KuoInit[]		=	{	1	,	3	,	7	,	1	,	15	,	45	,	37	,	57	,	223	,	151	,	979	,	2569	,	2487	,	2877	};
            const unsigned long	dim1304KuoInit[]		=	{	1	,	1	,	3	,	7	,	13	,	39	,	83	,	127	,	341	,	687	,	1575	,	3193	,	2845	,	725	};
            const unsigned long	dim1305KuoInit[]		=	{	1	,	1	,	1	,	5	,	23	,	39	,	71	,	205	,	53	,	477	,	1961	,	3967	,	7169	,	5443	};
            const unsigned long	dim1306KuoInit[]		=	{	1	,	1	,	5	,	5	,	23	,	17	,	107	,	151	,	291	,	101	,	1917	,	3429	,	6279	,	1781	};
            const unsigned long	dim1307KuoInit[]		=	{	1	,	3	,	3	,	11	,	23	,	45	,	11	,	137	,	243	,	257	,	495	,	3267	,	1373	,	15477	};
            const unsigned long	dim1308KuoInit[]		=	{	1	,	1	,	7	,	5	,	23	,	13	,	37	,	139	,	473	,	215	,	1281	,	989	,	2187	,	1551	};
            const unsigned long	dim1309KuoInit[]		=	{	1	,	1	,	7	,	7	,	5	,	21	,	113	,	195	,	247	,	493	,	513	,	1775	,	1885	,	5131	};
            const unsigned long	dim1310KuoInit[]		=	{	1	,	1	,	7	,	15	,	29	,	31	,	9	,	59	,	329	,	11	,	1739	,	1657	,	1745	,	8969	};
            const unsigned long	dim1311KuoInit[]		=	{	1	,	1	,	3	,	7	,	19	,	29	,	121	,	171	,	33	,	393	,	1463	,	1817	,	393	,	2509	};
            const unsigned long	dim1312KuoInit[]		=	{	1	,	1	,	5	,	11	,	23	,	55	,	103	,	169	,	313	,	293	,	1791	,	2727	,	2311	,	14519	};
            const unsigned long	dim1313KuoInit[]		=	{	1	,	3	,	7	,	11	,	13	,	21	,	95	,	205	,	87	,	431	,	667	,	367	,	6879	,	9843	};
            const unsigned long	dim1314KuoInit[]		=	{	1	,	3	,	1	,	3	,	3	,	53	,	75	,	43	,	115	,	561	,	795	,	613	,	4491	,	913	};
            const unsigned long	dim1315KuoInit[]		=	{	1	,	1	,	5	,	5	,	25	,	37	,	71	,	13	,	499	,	105	,	9	,	3759	,	4997	,	14747	};
            const unsigned long	dim1316KuoInit[]		=	{	1	,	3	,	3	,	13	,	25	,	63	,	27	,	163	,	359	,	431	,	5	,	3371	,	6051	,	1099	};
            const unsigned long	dim1317KuoInit[]		=	{	1	,	1	,	3	,	7	,	5	,	45	,	19	,	217	,	423	,	171	,	933	,	3441	,	7451	,	11831	};
            const unsigned long	dim1318KuoInit[]		=	{	1	,	3	,	7	,	7	,	7	,	53	,	81	,	67	,	465	,	713	,	1761	,	3945	,	3051	,	8139	};
            const unsigned long	dim1319KuoInit[]		=	{	1	,	1	,	5	,	7	,	23	,	55	,	85	,	205	,	265	,	99	,	353	,	4061	,	3385	,	4273	};
            const unsigned long	dim1320KuoInit[]		=	{	1	,	3	,	7	,	7	,	9	,	19	,	7	,	81	,	9	,	373	,	837	,	1397	,	3945	,	14325	};
            const unsigned long	dim1321KuoInit[]		=	{	1	,	1	,	5	,	7	,	7	,	47	,	61	,	195	,	101	,	931	,	1667	,	3277	,	7701	,	14697	};
            const unsigned long	dim1322KuoInit[]		=	{	1	,	3	,	7	,	7	,	11	,	57	,	43	,	55	,	399	,	783	,	1647	,	269	,	6055	,	9935	};
            const unsigned long	dim1323KuoInit[]		=	{	1	,	1	,	5	,	3	,	5	,	57	,	87	,	193	,	15	,	515	,	1663	,	495	,	4725	,	7221	};
            const unsigned long	dim1324KuoInit[]		=	{	1	,	1	,	5	,	3	,	23	,	23	,	87	,	231	,	133	,	123	,	1477	,	1811	,	7383	,	16339	};
            const unsigned long	dim1325KuoInit[]		=	{	1	,	3	,	5	,	13	,	31	,	57	,	105	,	237	,	375	,	129	,	193	,	1293	,	2445	,	13383	};
            const unsigned long	dim1326KuoInit[]		=	{	1	,	3	,	7	,	13	,	19	,	51	,	91	,	27	,	309	,	421	,	647	,	3777	,	6779	,	2807	};
            const unsigned long	dim1327KuoInit[]		=	{	1	,	3	,	5	,	15	,	5	,	43	,	125	,	37	,	269	,	155	,	1733	,	447	,	7883	,	6085	};
            const unsigned long	dim1328KuoInit[]		=	{	1	,	3	,	7	,	7	,	27	,	63	,	91	,	7	,	117	,	369	,	1019	,	847	,	6747	,	5057	};
            const unsigned long	dim1329KuoInit[]		=	{	1	,	3	,	5	,	3	,	31	,	19	,	87	,	125	,	259	,	355	,	161	,	1421	,	2295	,	9683	};
            const unsigned long	dim1330KuoInit[]		=	{	1	,	3	,	3	,	13	,	25	,	37	,	63	,	169	,	477	,	633	,	231	,	2039	,	3469	,	2531	};
            const unsigned long	dim1331KuoInit[]		=	{	1	,	3	,	1	,	1	,	25	,	1	,	81	,	211	,	237	,	233	,	1743	,	961	,	645	,	2605	};
            const unsigned long	dim1332KuoInit[]		=	{	1	,	3	,	1	,	11	,	3	,	25	,	3	,	111	,	123	,	417	,	1799	,	1419	,	2501	,	5765	};
            const unsigned long	dim1333KuoInit[]		=	{	1	,	1	,	3	,	15	,	11	,	17	,	27	,	95	,	295	,	315	,	987	,	481	,	6627	,	16171	};
            const unsigned long	dim1334KuoInit[]		=	{	1	,	3	,	5	,	15	,	15	,	61	,	123	,	5	,	369	,	161	,	897	,	855	,	625	,	9975	};
            const unsigned long	dim1335KuoInit[]		=	{	1	,	1	,	5	,	3	,	9	,	45	,	59	,	53	,	43	,	117	,	789	,	21	,	7799	,	11061	};
            const unsigned long	dim1336KuoInit[]		=	{	1	,	1	,	3	,	7	,	3	,	17	,	113	,	159	,	115	,	965	,	1541	,	1037	,	4723	,	10151	};
            const unsigned long	dim1337KuoInit[]		=	{	1	,	3	,	1	,	9	,	7	,	15	,	19	,	161	,	123	,	421	,	109	,	433	,	7387	,	6047	};
            const unsigned long	dim1338KuoInit[]		=	{	1	,	3	,	1	,	13	,	19	,	49	,	31	,	215	,	25	,	289	,	1995	,	1135	,	3719	,	8453	};
            const unsigned long	dim1339KuoInit[]		=	{	1	,	3	,	3	,	9	,	3	,	33	,	15	,	191	,	83	,	963	,	199	,	2199	,	3273	,	16165	};
            const unsigned long	dim1340KuoInit[]		=	{	1	,	3	,	3	,	11	,	17	,	63	,	71	,	49	,	427	,	633	,	735	,	2009	,	5049	,	13649	};
            const unsigned long	dim1341KuoInit[]		=	{	1	,	1	,	3	,	7	,	25	,	41	,	79	,	161	,	293	,	967	,	969	,	3253	,	269	,	6971	};
            const unsigned long	dim1342KuoInit[]		=	{	1	,	1	,	5	,	7	,	25	,	9	,	43	,	237	,	283	,	135	,	669	,	2193	,	4193	,	4787	};
            const unsigned long	dim1343KuoInit[]		=	{	1	,	3	,	3	,	7	,	5	,	19	,	83	,	89	,	443	,	443	,	1511	,	2309	,	5483	,	7583	};
            const unsigned long	dim1344KuoInit[]		=	{	1	,	3	,	3	,	15	,	21	,	29	,	113	,	127	,	405	,	465	,	1135	,	2401	,	7715	,	13061	};
            const unsigned long	dim1345KuoInit[]		=	{	1	,	3	,	7	,	7	,	23	,	19	,	17	,	59	,	483	,	519	,	1439	,	3839	,	1691	,	5	};
            const unsigned long	dim1346KuoInit[]		=	{	1	,	1	,	1	,	13	,	23	,	43	,	115	,	103	,	155	,	769	,	455	,	17	,	7791	,	3333	};
            const unsigned long	dim1347KuoInit[]		=	{	1	,	1	,	7	,	9	,	3	,	25	,	31	,	7	,	125	,	357	,	759	,	1203	,	2107	,	5361	};
            const unsigned long	dim1348KuoInit[]		=	{	1	,	1	,	5	,	11	,	27	,	31	,	23	,	73	,	77	,	449	,	661	,	2337	,	941	,	9421	};
            const unsigned long	dim1349KuoInit[]		=	{	1	,	1	,	7	,	15	,	27	,	17	,	35	,	15	,	405	,	317	,	1977	,	2449	,	237	,	7741	};
            const unsigned long	dim1350KuoInit[]		=	{	1	,	1	,	7	,	3	,	1	,	15	,	63	,	251	,	61	,	123	,	855	,	773	,	4291	,	15581	};
            const unsigned long	dim1351KuoInit[]		=	{	1	,	1	,	5	,	9	,	15	,	49	,	101	,	215	,	239	,	31	,	1193	,	3839	,	1913	,	15471	};
            const unsigned long	dim1352KuoInit[]		=	{	1	,	3	,	3	,	5	,	29	,	1	,	109	,	211	,	97	,	1017	,	1477	,	2131	,	6539	,	4891	};
            const unsigned long	dim1353KuoInit[]		=	{	1	,	1	,	3	,	11	,	15	,	63	,	67	,	175	,	209	,	105	,	1427	,	769	,	7021	,	287	};
            const unsigned long	dim1354KuoInit[]		=	{	1	,	3	,	3	,	7	,	19	,	41	,	109	,	165	,	307	,	935	,	379	,	395	,	2533	,	6291	};
            const unsigned long	dim1355KuoInit[]		=	{	1	,	3	,	5	,	13	,	17	,	3	,	69	,	79	,	437	,	861	,	1115	,	1351	,	4951	,	1753	};
            const unsigned long	dim1356KuoInit[]		=	{	1	,	3	,	1	,	7	,	19	,	15	,	83	,	85	,	431	,	207	,	481	,	711	,	6715	,	13439	};
            const unsigned long	dim1357KuoInit[]		=	{	1	,	1	,	1	,	5	,	3	,	13	,	127	,	27	,	223	,	297	,	985	,	227	,	6665	,	13587	};
            const unsigned long	dim1358KuoInit[]		=	{	1	,	3	,	7	,	7	,	7	,	61	,	113	,	73	,	443	,	397	,	717	,	1151	,	3047	,	3247	};
            const unsigned long	dim1359KuoInit[]		=	{	1	,	1	,	1	,	9	,	1	,	1	,	103	,	239	,	351	,	305	,	1345	,	3543	,	4321	,	9147	};
            const unsigned long	dim1360KuoInit[]		=	{	1	,	1	,	3	,	1	,	11	,	53	,	57	,	85	,	95	,	707	,	1817	,	249	,	357	,	11769	};
            const unsigned long	dim1361KuoInit[]		=	{	1	,	3	,	1	,	15	,	15	,	25	,	7	,	169	,	499	,	717	,	915	,	365	,	3131	,	12487	};
            const unsigned long	dim1362KuoInit[]		=	{	1	,	1	,	3	,	11	,	15	,	47	,	109	,	9	,	171	,	67	,	1473	,	1779	,	5567	,	3799	};
            const unsigned long	dim1363KuoInit[]		=	{	1	,	3	,	5	,	15	,	23	,	21	,	31	,	59	,	1	,	505	,	1157	,	1209	,	7661	,	11903	};
            const unsigned long	dim1364KuoInit[]		=	{	1	,	3	,	1	,	13	,	11	,	5	,	75	,	53	,	481	,	433	,	915	,	2881	,	5299	,	2835	};
            const unsigned long	dim1365KuoInit[]		=	{	1	,	3	,	7	,	5	,	23	,	13	,	127	,	229	,	157	,	643	,	847	,	3613	,	1999	,	3807	};
            const unsigned long	dim1366KuoInit[]		=	{	1	,	3	,	5	,	11	,	13	,	39	,	79	,	11	,	131	,	603	,	1013	,	1197	,	7583	,	8195	};
            const unsigned long	dim1367KuoInit[]		=	{	1	,	3	,	3	,	7	,	27	,	29	,	99	,	19	,	503	,	455	,	1453	,	2233	,	2479	,	8489	};
            const unsigned long	dim1368KuoInit[]		=	{	1	,	3	,	5	,	1	,	21	,	37	,	27	,	207	,	405	,	87	,	319	,	3323	,	4809	,	12599	};
            const unsigned long	dim1369KuoInit[]		=	{	1	,	3	,	7	,	15	,	29	,	3	,	121	,	249	,	31	,	601	,	865	,	2819	,	2633	,	15541	};
            const unsigned long	dim1370KuoInit[]		=	{	1	,	3	,	5	,	1	,	25	,	53	,	121	,	57	,	75	,	327	,	2047	,	253	,	7945	,	14209	};
            const unsigned long	dim1371KuoInit[]		=	{	1	,	1	,	7	,	11	,	21	,	3	,	13	,	91	,	417	,	825	,	1381	,	3653	,	4187	,	4685	};
            const unsigned long	dim1372KuoInit[]		=	{	1	,	3	,	3	,	9	,	11	,	17	,	87	,	75	,	333	,	871	,	1679	,	2943	,	4803	,	8147	};
            const unsigned long	dim1373KuoInit[]		=	{	1	,	1	,	1	,	9	,	13	,	1	,	105	,	95	,	347	,	543	,	1435	,	2337	,	5971	,	7605	};
            const unsigned long	dim1374KuoInit[]		=	{	1	,	3	,	5	,	15	,	19	,	51	,	35	,	91	,	187	,	833	,	1921	,	573	,	3605	,	8627	};
            const unsigned long	dim1375KuoInit[]		=	{	1	,	3	,	5	,	7	,	25	,	7	,	115	,	93	,	87	,	13	,	1323	,	3821	,	587	,	5079	};
            const unsigned long	dim1376KuoInit[]		=	{	1	,	3	,	5	,	3	,	25	,	25	,	89	,	185	,	473	,	909	,	1245	,	1589	,	63	,	10765	};
            const unsigned long	dim1377KuoInit[]		=	{	1	,	3	,	5	,	5	,	15	,	3	,	9	,	149	,	237	,	107	,	879	,	2487	,	7995	,	12713	};
            const unsigned long	dim1378KuoInit[]		=	{	1	,	3	,	3	,	3	,	19	,	55	,	43	,	221	,	289	,	129	,	1123	,	3411	,	703	,	9585	};
            const unsigned long	dim1379KuoInit[]		=	{	1	,	1	,	5	,	3	,	7	,	63	,	13	,	243	,	235	,	253	,	357	,	563	,	519	,	3471	};
            const unsigned long	dim1380KuoInit[]		=	{	1	,	1	,	3	,	13	,	9	,	59	,	67	,	61	,	321	,	149	,	365	,	2645	,	2309	,	3303	};
            const unsigned long	dim1381KuoInit[]		=	{	1	,	1	,	5	,	5	,	13	,	7	,	53	,	175	,	207	,	507	,	1159	,	3869	,	8139	,	13899	};
            const unsigned long	dim1382KuoInit[]		=	{	1	,	3	,	5	,	3	,	23	,	9	,	123	,	37	,	325	,	41	,	679	,	513	,	4379	,	3493	};
            const unsigned long	dim1383KuoInit[]		=	{	1	,	1	,	7	,	3	,	29	,	9	,	117	,	203	,	461	,	831	,	1757	,	2815	,	1215	,	1725	};
            const unsigned long	dim1384KuoInit[]		=	{	1	,	1	,	7	,	7	,	23	,	45	,	75	,	159	,	395	,	315	,	709	,	887	,	6935	,	2307	};
            const unsigned long	dim1385KuoInit[]		=	{	1	,	1	,	7	,	7	,	5	,	51	,	29	,	141	,	215	,	783	,	1161	,	3887	,	8103	,	7461	};
            const unsigned long	dim1386KuoInit[]		=	{	1	,	1	,	7	,	3	,	21	,	5	,	59	,	123	,	283	,	921	,	225	,	453	,	2125	,	559	};
            const unsigned long	dim1387KuoInit[]		=	{	1	,	3	,	7	,	13	,	21	,	19	,	121	,	201	,	367	,	215	,	1727	,	1743	,	4875	,	2819	};
            const unsigned long	dim1388KuoInit[]		=	{	1	,	3	,	3	,	13	,	15	,	53	,	57	,	35	,	419	,	595	,	1581	,	2881	,	3473	,	7201	};
            const unsigned long	dim1389KuoInit[]		=	{	1	,	3	,	5	,	5	,	19	,	17	,	113	,	175	,	195	,	829	,	115	,	491	,	6439	,	4085	};
            const unsigned long	dim1390KuoInit[]		=	{	1	,	1	,	5	,	9	,	7	,	9	,	111	,	23	,	459	,	335	,	1529	,	1519	,	2855	,	11923	};
            const unsigned long	dim1391KuoInit[]		=	{	1	,	3	,	7	,	9	,	13	,	41	,	19	,	229	,	73	,	865	,	1379	,	1315	,	1403	,	10489	};
            const unsigned long	dim1392KuoInit[]		=	{	1	,	1	,	1	,	7	,	17	,	35	,	71	,	199	,	195	,	89	,	855	,	355	,	5087	,	13173	};
            const unsigned long	dim1393KuoInit[]		=	{	1	,	3	,	5	,	7	,	3	,	45	,	77	,	109	,	105	,	919	,	21	,	2749	,	5401	,	11701	};
            const unsigned long	dim1394KuoInit[]		=	{	1	,	3	,	1	,	15	,	5	,	9	,	29	,	213	,	489	,	455	,	791	,	215	,	1011	,	2501	};
            const unsigned long	dim1395KuoInit[]		=	{	1	,	3	,	1	,	9	,	17	,	7	,	61	,	99	,	381	,	669	,	1787	,	1443	,	6135	,	3273	};
            const unsigned long	dim1396KuoInit[]		=	{	1	,	3	,	1	,	3	,	27	,	47	,	31	,	153	,	365	,	271	,	1499	,	1229	,	5791	,	2251	};
            const unsigned long	dim1397KuoInit[]		=	{	1	,	1	,	1	,	3	,	9	,	31	,	109	,	53	,	293	,	135	,	1329	,	2795	,	4335	,	14817	};
            const unsigned long	dim1398KuoInit[]		=	{	1	,	3	,	1	,	15	,	19	,	7	,	43	,	223	,	483	,	237	,	803	,	1713	,	7969	,	9681	};
            const unsigned long	dim1399KuoInit[]		=	{	1	,	1	,	7	,	1	,	23	,	45	,	67	,	87	,	57	,	261	,	955	,	2917	,	549	,	4725	};
            const unsigned long	dim1400KuoInit[]		=	{	1	,	1	,	1	,	7	,	13	,	43	,	63	,	153	,	395	,	313	,	1219	,	655	,	6489	,	2129	};
            const unsigned long	dim1401KuoInit[]		=	{	1	,	3	,	3	,	3	,	7	,	51	,	35	,	233	,	289	,	715	,	1227	,	2041	,	3639	,	1465	};
            const unsigned long	dim1402KuoInit[]		=	{	1	,	3	,	1	,	7	,	3	,	37	,	97	,	149	,	51	,	313	,	1883	,	1411	,	3319	,	2599	};
            const unsigned long	dim1403KuoInit[]		=	{	1	,	1	,	7	,	13	,	25	,	13	,	89	,	245	,	251	,	841	,	1367	,	4039	,	4389	,	7129	};
            const unsigned long	dim1404KuoInit[]		=	{	1	,	3	,	5	,	15	,	3	,	55	,	113	,	97	,	159	,	621	,	251	,	2539	,	7017	,	1039	};
            const unsigned long	dim1405KuoInit[]		=	{	1	,	3	,	3	,	9	,	27	,	31	,	53	,	61	,	165	,	753	,	1733	,	671	,	665	,	4893	};
            const unsigned long	dim1406KuoInit[]		=	{	1	,	1	,	7	,	5	,	25	,	11	,	7	,	189	,	457	,	5	,	97	,	2399	,	3089	,	4811	};
            const unsigned long	dim1407KuoInit[]		=	{	1	,	1	,	3	,	13	,	29	,	19	,	73	,	105	,	187	,	871	,	499	,	443	,	6807	,	3753	};
            const unsigned long	dim1408KuoInit[]		=	{	1	,	3	,	1	,	5	,	9	,	15	,	53	,	83	,	91	,	763	,	1585	,	1675	,	5137	,	10483	};
            const unsigned long	dim1409KuoInit[]		=	{	1	,	1	,	7	,	5	,	25	,	51	,	43	,	47	,	151	,	13	,	1117	,	1465	,	2497	,	7677	};
            const unsigned long	dim1410KuoInit[]		=	{	1	,	1	,	1	,	3	,	15	,	15	,	43	,	53	,	161	,	689	,	2027	,	437	,	3599	,	6837	};
            const unsigned long	dim1411KuoInit[]		=	{	1	,	3	,	3	,	9	,	21	,	5	,	81	,	99	,	249	,	785	,	763	,	1203	,	1541	,	6497	};
            const unsigned long	dim1412KuoInit[]		=	{	1	,	1	,	5	,	7	,	27	,	7	,	121	,	91	,	271	,	461	,	1531	,	3255	,	847	,	14503	};
            const unsigned long	dim1413KuoInit[]		=	{	1	,	1	,	3	,	15	,	9	,	53	,	125	,	107	,	321	,	579	,	813	,	2295	,	2521	,	6773	};
            const unsigned long	dim1414KuoInit[]		=	{	1	,	3	,	3	,	1	,	15	,	55	,	71	,	61	,	307	,	917	,	217	,	1133	,	7255	,	16083	};
            const unsigned long	dim1415KuoInit[]		=	{	1	,	3	,	1	,	1	,	3	,	7	,	57	,	27	,	243	,	989	,	223	,	3857	,	2357	,	11315	};
            const unsigned long	dim1416KuoInit[]		=	{	1	,	3	,	5	,	5	,	23	,	41	,	55	,	75	,	241	,	829	,	1865	,	909	,	4509	,	5163	};
            const unsigned long	dim1417KuoInit[]		=	{	1	,	1	,	1	,	13	,	27	,	5	,	117	,	79	,	171	,	135	,	623	,	2515	,	3661	,	941	};
            const unsigned long	dim1418KuoInit[]		=	{	1	,	3	,	5	,	13	,	17	,	29	,	9	,	147	,	73	,	321	,	895	,	3263	,	6897	,	8551	};
            const unsigned long	dim1419KuoInit[]		=	{	1	,	1	,	5	,	11	,	21	,	45	,	57	,	153	,	83	,	971	,	403	,	617	,	545	,	6489	};
            const unsigned long	dim1420KuoInit[]		=	{	1	,	1	,	5	,	9	,	31	,	37	,	19	,	241	,	89	,	583	,	1365	,	2649	,	7011	,	6183	};
            const unsigned long	dim1421KuoInit[]		=	{	1	,	3	,	5	,	9	,	3	,	5	,	45	,	237	,	351	,	811	,	445	,	557	,	4103	,	11185	};
            const unsigned long	dim1422KuoInit[]		=	{	1	,	3	,	3	,	13	,	25	,	23	,	21	,	249	,	209	,	775	,	1397	,	2501	,	4535	,	4957	};
            const unsigned long	dim1423KuoInit[]		=	{	1	,	1	,	1	,	13	,	7	,	43	,	55	,	179	,	239	,	43	,	225	,	2827	,	955	,	665	};
            const unsigned long	dim1424KuoInit[]		=	{	1	,	1	,	3	,	5	,	15	,	35	,	77	,	153	,	429	,	35	,	539	,	3543	,	1971	,	2865	};
            const unsigned long	dim1425KuoInit[]		=	{	1	,	3	,	7	,	3	,	27	,	35	,	87	,	51	,	281	,	247	,	547	,	2185	,	2593	,	1301	};
            const unsigned long	dim1426KuoInit[]		=	{	1	,	3	,	5	,	11	,	19	,	11	,	79	,	241	,	467	,	515	,	531	,	3285	,	1575	,	7869	};
            const unsigned long	dim1427KuoInit[]		=	{	1	,	3	,	1	,	13	,	25	,	15	,	109	,	231	,	349	,	955	,	2019	,	3475	,	2927	,	14383	};
            const unsigned long	dim1428KuoInit[]		=	{	1	,	1	,	7	,	1	,	17	,	43	,	43	,	131	,	485	,	179	,	901	,	483	,	4879	,	15189	};
            const unsigned long	dim1429KuoInit[]		=	{	1	,	1	,	5	,	11	,	7	,	39	,	111	,	27	,	29	,	79	,	261	,	77	,	231	,	8577	};
            const unsigned long	dim1430KuoInit[]		=	{	1	,	3	,	1	,	11	,	25	,	15	,	25	,	41	,	457	,	677	,	1033	,	161	,	3215	,	105	};
            const unsigned long	dim1431KuoInit[]		=	{	1	,	3	,	1	,	15	,	3	,	57	,	125	,	41	,	325	,	467	,	1325	,	1287	,	51	,	9463	};
            const unsigned long	dim1432KuoInit[]		=	{	1	,	1	,	1	,	9	,	1	,	33	,	39	,	47	,	313	,	305	,	2023	,	1131	,	4167	,	4519	};
            const unsigned long	dim1433KuoInit[]		=	{	1	,	1	,	5	,	15	,	1	,	47	,	123	,	151	,	263	,	143	,	91	,	1673	,	4175	,	4575	};
            const unsigned long	dim1434KuoInit[]		=	{	1	,	1	,	7	,	3	,	7	,	41	,	95	,	27	,	183	,	437	,	587	,	3821	,	8035	,	7627	};
            const unsigned long	dim1435KuoInit[]		=	{	1	,	1	,	3	,	5	,	17	,	53	,	95	,	73	,	335	,	497	,	643	,	3309	,	7139	,	10811	};
            const unsigned long	dim1436KuoInit[]		=	{	1	,	3	,	1	,	13	,	7	,	29	,	83	,	15	,	29	,	881	,	881	,	1253	,	7267	,	4221	};
            const unsigned long	dim1437KuoInit[]		=	{	1	,	3	,	7	,	13	,	15	,	47	,	71	,	63	,	493	,	365	,	1209	,	2503	,	3963	,	5069	};
            const unsigned long	dim1438KuoInit[]		=	{	1	,	1	,	5	,	5	,	23	,	55	,	107	,	255	,	17	,	561	,	1131	,	497	,	5731	,	13287	};
            const unsigned long	dim1439KuoInit[]		=	{	1	,	3	,	5	,	11	,	5	,	59	,	25	,	131	,	375	,	165	,	1051	,	3129	,	6517	,	6317	};
            const unsigned long	dim1440KuoInit[]		=	{	1	,	3	,	5	,	9	,	17	,	29	,	57	,	215	,	77	,	197	,	1775	,	3959	,	185	,	11431	};
            const unsigned long	dim1441KuoInit[]		=	{	1	,	1	,	1	,	11	,	11	,	45	,	29	,	245	,	193	,	747	,	1449	,	2185	,	7751	,	10009	};
            const unsigned long	dim1442KuoInit[]		=	{	1	,	1	,	1	,	5	,	27	,	19	,	39	,	161	,	355	,	455	,	1625	,	1283	,	1547	,	8707	};
            const unsigned long	dim1443KuoInit[]		=	{	1	,	1	,	1	,	11	,	7	,	15	,	35	,	193	,	489	,	619	,	515	,	199	,	5439	,	15711	};
            const unsigned long	dim1444KuoInit[]		=	{	1	,	1	,	7	,	1	,	29	,	21	,	69	,	13	,	343	,	581	,	495	,	3963	,	911	,	3995	};
            const unsigned long	dim1445KuoInit[]		=	{	1	,	1	,	1	,	7	,	21	,	5	,	9	,	31	,	291	,	825	,	7	,	2621	,	1149	,	2709	};
            const unsigned long	dim1446KuoInit[]		=	{	1	,	3	,	3	,	13	,	7	,	35	,	69	,	43	,	397	,	641	,	89	,	3325	,	5567	,	3487	};
            const unsigned long	dim1447KuoInit[]		=	{	1	,	3	,	5	,	5	,	1	,	17	,	11	,	97	,	309	,	371	,	145	,	3255	,	5161	,	7787	};
            const unsigned long	dim1448KuoInit[]		=	{	1	,	3	,	7	,	7	,	27	,	1	,	1	,	235	,	47	,	251	,	1525	,	2661	,	3971	,	11319	};
            const unsigned long	dim1449KuoInit[]		=	{	1	,	3	,	5	,	3	,	11	,	21	,	69	,	97	,	13	,	931	,	1955	,	3497	,	3963	,	519	};
            const unsigned long	dim1450KuoInit[]		=	{	1	,	1	,	5	,	9	,	21	,	15	,	11	,	201	,	175	,	855	,	1867	,	3941	,	6505	,	3451	};
            const unsigned long	dim1451KuoInit[]		=	{	1	,	1	,	7	,	13	,	25	,	45	,	27	,	91	,	149	,	827	,	1791	,	139	,	6721	,	9	};
            const unsigned long	dim1452KuoInit[]		=	{	1	,	3	,	3	,	7	,	19	,	13	,	95	,	161	,	91	,	333	,	203	,	3195	,	517	,	3911	};
            const unsigned long	dim1453KuoInit[]		=	{	1	,	1	,	5	,	1	,	3	,	7	,	113	,	195	,	501	,	203	,	193	,	3585	,	3217	,	2009	};
            const unsigned long	dim1454KuoInit[]		=	{	1	,	1	,	5	,	13	,	21	,	29	,	9	,	209	,	381	,	245	,	1317	,	3577	,	3327	,	9253	};
            const unsigned long	dim1455KuoInit[]		=	{	1	,	3	,	3	,	15	,	1	,	5	,	75	,	215	,	349	,	143	,	1607	,	2817	,	8185	,	13991	};
            const unsigned long	dim1456KuoInit[]		=	{	1	,	3	,	3	,	11	,	23	,	29	,	73	,	15	,	159	,	425	,	1169	,	2989	,	5835	,	12687	};
            const unsigned long	dim1457KuoInit[]		=	{	1	,	1	,	1	,	11	,	13	,	13	,	79	,	97	,	351	,	455	,	1465	,	2013	,	5133	,	1747	};
            const unsigned long	dim1458KuoInit[]		=	{	1	,	3	,	3	,	13	,	3	,	29	,	53	,	37	,	357	,	55	,	195	,	43	,	3903	,	7013	};
            const unsigned long	dim1459KuoInit[]		=	{	1	,	3	,	5	,	13	,	25	,	43	,	19	,	233	,	17	,	31	,	1727	,	557	,	2581	,	847	};
            const unsigned long	dim1460KuoInit[]		=	{	1	,	1	,	7	,	5	,	21	,	27	,	7	,	27	,	487	,	961	,	1095	,	3813	,	7967	,	15283	};
            const unsigned long	dim1461KuoInit[]		=	{	1	,	3	,	7	,	7	,	27	,	49	,	15	,	199	,	413	,	295	,	441	,	2023	,	2453	,	2243	};
            const unsigned long	dim1462KuoInit[]		=	{	1	,	3	,	7	,	9	,	19	,	13	,	99	,	85	,	227	,	61	,	933	,	3897	,	2399	,	455	};
            const unsigned long	dim1463KuoInit[]		=	{	1	,	3	,	1	,	15	,	13	,	45	,	123	,	13	,	39	,	431	,	1573	,	2771	,	2273	,	12613	};
            const unsigned long	dim1464KuoInit[]		=	{	1	,	1	,	3	,	13	,	1	,	29	,	127	,	59	,	159	,	1005	,	483	,	3031	,	5029	,	1763	};
            const unsigned long	dim1465KuoInit[]		=	{	1	,	1	,	7	,	9	,	17	,	27	,	87	,	25	,	313	,	305	,	1611	,	693	,	3701	,	573	};
            const unsigned long	dim1466KuoInit[]		=	{	1	,	3	,	7	,	13	,	5	,	61	,	93	,	69	,	357	,	681	,	291	,	1251	,	3889	,	3687	};
            const unsigned long	dim1467KuoInit[]		=	{	1	,	1	,	3	,	15	,	9	,	31	,	121	,	213	,	115	,	231	,	349	,	2141	,	4443	,	12319	};
            const unsigned long	dim1468KuoInit[]		=	{	1	,	1	,	1	,	5	,	1	,	37	,	59	,	21	,	399	,	751	,	841	,	33	,	7195	,	8403	};
            const unsigned long	dim1469KuoInit[]		=	{	1	,	3	,	3	,	13	,	5	,	41	,	111	,	21	,	135	,	945	,	1915	,	2351	,	6429	,	10993	};
            const unsigned long	dim1470KuoInit[]		=	{	1	,	1	,	1	,	1	,	29	,	43	,	45	,	13	,	395	,	407	,	47	,	1007	,	7525	,	6707	};
            const unsigned long	dim1471KuoInit[]		=	{	1	,	3	,	5	,	15	,	23	,	3	,	49	,	149	,	391	,	689	,	147	,	1173	,	3793	,	4249	};
            const unsigned long	dim1472KuoInit[]		=	{	1	,	3	,	1	,	13	,	25	,	35	,	109	,	147	,	323	,	161	,	257	,	2023	,	5693	,	16199	};
            const unsigned long	dim1473KuoInit[]		=	{	1	,	3	,	3	,	9	,	17	,	3	,	9	,	191	,	19	,	405	,	1841	,	1669	,	4691	,	11907	};
            const unsigned long	dim1474KuoInit[]		=	{	1	,	1	,	3	,	5	,	13	,	35	,	45	,	27	,	127	,	329	,	931	,	1	,	2521	,	15307	};
            const unsigned long	dim1475KuoInit[]		=	{	1	,	1	,	5	,	3	,	11	,	25	,	31	,	233	,	1	,	769	,	433	,	3785	,	3631	,	8573	};
            const unsigned long	dim1476KuoInit[]		=	{	1	,	1	,	3	,	1	,	23	,	57	,	67	,	227	,	393	,	469	,	1579	,	2235	,	493	,	2477	};
            const unsigned long	dim1477KuoInit[]		=	{	1	,	3	,	7	,	3	,	9	,	19	,	69	,	123	,	205	,	553	,	1871	,	3807	,	1563	,	2771	};
            const unsigned long	dim1478KuoInit[]		=	{	1	,	3	,	3	,	11	,	11	,	33	,	55	,	27	,	51	,	903	,	1681	,	2613	,	2519	,	9529	};
            const unsigned long	dim1479KuoInit[]		=	{	1	,	1	,	7	,	11	,	29	,	55	,	31	,	185	,	85	,	929	,	233	,	2619	,	2989	,	191	};
            const unsigned long	dim1480KuoInit[]		=	{	1	,	1	,	1	,	3	,	1	,	47	,	47	,	225	,	387	,	717	,	815	,	853	,	819	,	5645	};
            const unsigned long	dim1481KuoInit[]		=	{	1	,	1	,	5	,	3	,	1	,	55	,	89	,	59	,	39	,	687	,	1395	,	535	,	4977	,	8155	};
            const unsigned long	dim1482KuoInit[]		=	{	1	,	3	,	7	,	1	,	11	,	57	,	107	,	137	,	407	,	155	,	313	,	2205	,	3095	,	15179	};
            const unsigned long	dim1483KuoInit[]		=	{	1	,	1	,	7	,	13	,	29	,	5	,	121	,	173	,	21	,	247	,	1663	,	1937	,	5113	,	7439	};
            const unsigned long	dim1484KuoInit[]		=	{	1	,	3	,	1	,	7	,	1	,	39	,	31	,	129	,	109	,	447	,	1269	,	3133	,	4933	,	11893	};
            const unsigned long	dim1485KuoInit[]		=	{	1	,	3	,	3	,	15	,	9	,	7	,	21	,	157	,	397	,	465	,	1057	,	2355	,	4663	,	13955	};
            const unsigned long	dim1486KuoInit[]		=	{	1	,	1	,	7	,	3	,	1	,	35	,	71	,	175	,	439	,	1019	,	1479	,	2519	,	5119	,	15245	};
            const unsigned long	dim1487KuoInit[]		=	{	1	,	1	,	7	,	3	,	23	,	49	,	55	,	91	,	269	,	407	,	393	,	1495	,	6153	,	15703	};
            const unsigned long	dim1488KuoInit[]		=	{	1	,	1	,	3	,	7	,	25	,	3	,	45	,	87	,	141	,	693	,	555	,	3629	,	2723	,	4357	};
            const unsigned long	dim1489KuoInit[]		=	{	1	,	3	,	3	,	7	,	15	,	27	,	103	,	161	,	423	,	785	,	1171	,	2335	,	1845	,	14373	};
            const unsigned long	dim1490KuoInit[]		=	{	1	,	3	,	5	,	5	,	1	,	31	,	117	,	35	,	181	,	263	,	1337	,	3239	,	617	,	11499	};
            const unsigned long	dim1491KuoInit[]		=	{	1	,	1	,	1	,	9	,	15	,	17	,	103	,	201	,	79	,	211	,	2043	,	3733	,	7593	,	6523	};
            const unsigned long	dim1492KuoInit[]		=	{	1	,	3	,	5	,	5	,	3	,	59	,	11	,	163	,	155	,	215	,	1621	,	2615	,	2915	,	2459	};
            const unsigned long	dim1493KuoInit[]		=	{	1	,	3	,	7	,	11	,	11	,	11	,	93	,	239	,	367	,	553	,	1341	,	129	,	7651	,	2841	};
            const unsigned long	dim1494KuoInit[]		=	{	1	,	3	,	7	,	7	,	7	,	21	,	45	,	99	,	63	,	601	,	1877	,	765	,	5831	,	1825	};
            const unsigned long	dim1495KuoInit[]		=	{	1	,	1	,	7	,	3	,	21	,	27	,	71	,	53	,	185	,	675	,	891	,	657	,	5343	,	3583	};
            const unsigned long	dim1496KuoInit[]		=	{	1	,	3	,	5	,	9	,	17	,	57	,	95	,	57	,	427	,	329	,	681	,	2663	,	3183	,	7429	};
            const unsigned long	dim1497KuoInit[]		=	{	1	,	3	,	7	,	3	,	17	,	9	,	93	,	105	,	409	,	975	,	205	,	429	,	4473	,	10269	};
            const unsigned long	dim1498KuoInit[]		=	{	1	,	3	,	1	,	1	,	11	,	51	,	25	,	97	,	333	,	71	,	1513	,	1273	,	1235	,	5023	};
            const unsigned long	dim1499KuoInit[]		=	{	1	,	1	,	7	,	9	,	17	,	59	,	75	,	51	,	277	,	377	,	3	,	3357	,	2595	,	7939	};
            const unsigned long	dim1500KuoInit[]		=	{	1	,	3	,	3	,	1	,	7	,	47	,	11	,	241	,	235	,	761	,	283	,	3277	,	2121	,	69	};
            const unsigned long	dim1501KuoInit[]		=	{	1	,	3	,	1	,	3	,	5	,	5	,	57	,	5	,	99	,	869	,	707	,	2379	,	5439	,	4981	};
            const unsigned long	dim1502KuoInit[]		=	{	1	,	3	,	7	,	9	,	5	,	9	,	115	,	155	,	191	,	905	,	1173	,	975	,	6825	,	8257	};
            const unsigned long	dim1503KuoInit[]		=	{	1	,	1	,	3	,	13	,	17	,	13	,	63	,	143	,	305	,	577	,	1895	,	1943	,	2335	,	8957	};
            const unsigned long	dim1504KuoInit[]		=	{	1	,	3	,	7	,	3	,	15	,	19	,	89	,	91	,	497	,	145	,	1419	,	1449	,	5305	,	1379	};
            const unsigned long	dim1505KuoInit[]		=	{	1	,	3	,	1	,	13	,	9	,	51	,	103	,	169	,	265	,	347	,	697	,	3735	,	6987	,	2565	};
            const unsigned long	dim1506KuoInit[]		=	{	1	,	3	,	3	,	5	,	1	,	19	,	83	,	211	,	247	,	81	,	131	,	2243	,	7563	,	14221	};
            const unsigned long	dim1507KuoInit[]		=	{	1	,	3	,	3	,	11	,	27	,	1	,	97	,	85	,	265	,	423	,	173	,	1873	,	3889	,	1845	};
            const unsigned long	dim1508KuoInit[]		=	{	1	,	1	,	1	,	11	,	15	,	25	,	25	,	155	,	111	,	327	,	1089	,	335	,	7635	,	6467	};
            const unsigned long	dim1509KuoInit[]		=	{	1	,	3	,	5	,	7	,	15	,	33	,	103	,	231	,	455	,	681	,	547	,	3389	,	4279	,	6037	};
            const unsigned long	dim1510KuoInit[]		=	{	1	,	3	,	3	,	7	,	3	,	11	,	11	,	165	,	113	,	499	,	977	,	3805	,	2733	,	15643	};
            const unsigned long	dim1511KuoInit[]		=	{	1	,	1	,	3	,	7	,	23	,	49	,	39	,	165	,	323	,	313	,	1731	,	2977	,	4795	,	12387	};
            const unsigned long	dim1512KuoInit[]		=	{	1	,	1	,	5	,	15	,	27	,	15	,	43	,	149	,	71	,	19	,	1025	,	2585	,	5975	,	12473	};
            const unsigned long	dim1513KuoInit[]		=	{	1	,	3	,	7	,	15	,	15	,	21	,	33	,	29	,	351	,	909	,	813	,	3901	,	3235	,	3719	};
            const unsigned long	dim1514KuoInit[]		=	{	1	,	3	,	3	,	1	,	29	,	63	,	55	,	173	,	405	,	841	,	871	,	213	,	4747	,	11611	};
            const unsigned long	dim1515KuoInit[]		=	{	1	,	1	,	1	,	7	,	15	,	11	,	83	,	21	,	123	,	929	,	1137	,	3843	,	5919	,	151	};
            const unsigned long	dim1516KuoInit[]		=	{	1	,	1	,	3	,	1	,	29	,	53	,	111	,	133	,	285	,	499	,	2011	,	121	,	2853	,	7587	};
            const unsigned long	dim1517KuoInit[]		=	{	1	,	3	,	7	,	1	,	15	,	33	,	87	,	139	,	429	,	97	,	375	,	1273	,	2745	,	12045	};
            const unsigned long	dim1518KuoInit[]		=	{	1	,	1	,	5	,	13	,	17	,	55	,	43	,	3	,	465	,	1	,	1065	,	647	,	2497	,	14775	};
            const unsigned long	dim1519KuoInit[]		=	{	1	,	1	,	5	,	5	,	15	,	41	,	39	,	37	,	397	,	621	,	1735	,	1009	,	1289	,	2375	};
            const unsigned long	dim1520KuoInit[]		=	{	1	,	3	,	5	,	11	,	1	,	45	,	73	,	207	,	375	,	825	,	697	,	3765	,	7181	,	4953	};
            const unsigned long	dim1521KuoInit[]		=	{	1	,	3	,	7	,	7	,	23	,	15	,	75	,	11	,	137	,	1017	,	203	,	1325	,	6661	,	8569	};
            const unsigned long	dim1522KuoInit[]		=	{	1	,	1	,	1	,	15	,	23	,	9	,	15	,	157	,	179	,	699	,	1493	,	2161	,	3159	,	2709	};
            const unsigned long	dim1523KuoInit[]		=	{	1	,	3	,	7	,	3	,	15	,	51	,	95	,	101	,	467	,	303	,	1867	,	3819	,	7593	,	6133	};
            const unsigned long	dim1524KuoInit[]		=	{	1	,	3	,	7	,	13	,	25	,	27	,	87	,	11	,	113	,	861	,	1393	,	1521	,	2109	,	11899	};
            const unsigned long	dim1525KuoInit[]		=	{	1	,	3	,	5	,	11	,	25	,	53	,	27	,	249	,	193	,	783	,	609	,	3069	,	3901	,	391	};
            const unsigned long	dim1526KuoInit[]		=	{	1	,	3	,	5	,	11	,	19	,	13	,	55	,	147	,	325	,	131	,	659	,	1277	,	6127	,	1033	};
            const unsigned long	dim1527KuoInit[]		=	{	1	,	3	,	1	,	7	,	31	,	13	,	1	,	141	,	377	,	159	,	1945	,	2031	,	8039	,	13851	};
            const unsigned long	dim1528KuoInit[]		=	{	1	,	3	,	5	,	7	,	27	,	17	,	79	,	47	,	421	,	517	,	751	,	3049	,	6109	,	11027	};
            const unsigned long	dim1529KuoInit[]		=	{	1	,	3	,	3	,	11	,	23	,	41	,	89	,	225	,	367	,	109	,	1295	,	2691	,	4677	,	9207	};
            const unsigned long	dim1530KuoInit[]		=	{	1	,	1	,	7	,	7	,	7	,	27	,	89	,	221	,	267	,	537	,	2003	,	3357	,	8113	,	8173	};
            const unsigned long	dim1531KuoInit[]		=	{	1	,	1	,	7	,	15	,	19	,	23	,	21	,	173	,	159	,	535	,	1685	,	2259	,	6515	,	15853	};
            const unsigned long	dim1532KuoInit[]		=	{	1	,	3	,	3	,	15	,	13	,	21	,	105	,	173	,	173	,	13	,	423	,	2259	,	4145	,	6543	};
            const unsigned long	dim1533KuoInit[]		=	{	1	,	1	,	3	,	11	,	13	,	41	,	35	,	239	,	307	,	893	,	73	,	1325	,	7975	,	8395	};
            const unsigned long	dim1534KuoInit[]		=	{	1	,	1	,	7	,	11	,	11	,	11	,	115	,	195	,	407	,	543	,	801	,	1449	,	8121	,	9627	};
            const unsigned long	dim1535KuoInit[]		=	{	1	,	1	,	1	,	1	,	13	,	33	,	41	,	207	,	347	,	49	,	897	,	557	,	5917	,	13163	};
            const unsigned long	dim1536KuoInit[]		=	{	1	,	1	,	7	,	13	,	7	,	9	,	51	,	241	,	47	,	479	,	1399	,	3229	,	5201	,	537	};
            const unsigned long	dim1537KuoInit[]		=	{	1	,	3	,	7	,	3	,	9	,	63	,	31	,	109	,	381	,	631	,	521	,	139	,	4229	,	1639	};
            const unsigned long	dim1538KuoInit[]		=	{	1	,	3	,	3	,	7	,	1	,	23	,	15	,	99	,	395	,	991	,	627	,	571	,	833	,	6631	};
            const unsigned long	dim1539KuoInit[]		=	{	1	,	1	,	7	,	3	,	31	,	9	,	99	,	145	,	237	,	181	,	1911	,	2985	,	2415	,	5363	};
            const unsigned long	dim1540KuoInit[]		=	{	1	,	3	,	5	,	3	,	27	,	1	,	111	,	133	,	307	,	211	,	1119	,	2289	,	5333	,	14747	};
            const unsigned long	dim1541KuoInit[]		=	{	1	,	3	,	7	,	5	,	9	,	27	,	5	,	101	,	113	,	505	,	931	,	677	,	6225	,	16279	};
            const unsigned long	dim1542KuoInit[]		=	{	1	,	1	,	5	,	9	,	19	,	47	,	57	,	3	,	281	,	207	,	621	,	939	,	839	,	4089	};
            const unsigned long	dim1543KuoInit[]		=	{	1	,	1	,	5	,	11	,	25	,	61	,	117	,	139	,	5	,	325	,	745	,	3423	,	31	,	1451	};
            const unsigned long	dim1544KuoInit[]		=	{	1	,	3	,	5	,	5	,	3	,	9	,	121	,	185	,	497	,	203	,	367	,	3049	,	4827	,	3363	};
            const unsigned long	dim1545KuoInit[]		=	{	1	,	3	,	7	,	5	,	1	,	45	,	91	,	199	,	183	,	577	,	625	,	3641	,	963	,	9379	};
            const unsigned long	dim1546KuoInit[]		=	{	1	,	3	,	5	,	9	,	5	,	53	,	17	,	205	,	331	,	329	,	669	,	2403	,	3027	,	297	};
            const unsigned long	dim1547KuoInit[]		=	{	1	,	3	,	5	,	9	,	19	,	31	,	43	,	249	,	361	,	923	,	635	,	3461	,	3901	,	439	};
            const unsigned long	dim1548KuoInit[]		=	{	1	,	3	,	7	,	15	,	21	,	25	,	115	,	115	,	457	,	607	,	699	,	2649	,	3003	,	1599	};
            const unsigned long	dim1549KuoInit[]		=	{	1	,	1	,	5	,	15	,	21	,	35	,	61	,	97	,	131	,	521	,	1045	,	2401	,	2885	,	11795	};
            const unsigned long	dim1550KuoInit[]		=	{	1	,	1	,	1	,	11	,	19	,	13	,	119	,	117	,	31	,	575	,	427	,	3689	,	3801	,	12267	};
            const unsigned long	dim1551KuoInit[]		=	{	1	,	1	,	3	,	7	,	27	,	45	,	15	,	235	,	281	,	777	,	1181	,	2445	,	7947	,	2081	};
            const unsigned long	dim1552KuoInit[]		=	{	1	,	1	,	5	,	3	,	31	,	57	,	57	,	219	,	135	,	717	,	1369	,	2221	,	4397	,	16195	};
            const unsigned long	dim1553KuoInit[]		=	{	1	,	3	,	5	,	13	,	11	,	17	,	7	,	189	,	97	,	7	,	397	,	1557	,	7181	,	16205	};
            const unsigned long	dim1554KuoInit[]		=	{	1	,	1	,	7	,	15	,	13	,	25	,	79	,	241	,	143	,	959	,	529	,	3899	,	5411	,	8769	};
            const unsigned long	dim1555KuoInit[]		=	{	1	,	1	,	7	,	1	,	3	,	17	,	109	,	25	,	389	,	463	,	1675	,	627	,	6383	,	5007	};
            const unsigned long	dim1556KuoInit[]		=	{	1	,	3	,	1	,	13	,	19	,	3	,	13	,	3	,	293	,	755	,	1827	,	755	,	3325	,	13447	};
            const unsigned long	dim1557KuoInit[]		=	{	1	,	1	,	7	,	15	,	17	,	5	,	87	,	17	,	423	,	901	,	777	,	919	,	2655	,	1423	};
            const unsigned long	dim1558KuoInit[]		=	{	1	,	1	,	3	,	13	,	19	,	41	,	89	,	9	,	475	,	479	,	981	,	1031	,	7207	,	3605	};
            const unsigned long	dim1559KuoInit[]		=	{	1	,	3	,	5	,	7	,	9	,	41	,	111	,	201	,	389	,	139	,	1853	,	1773	,	233	,	3695	};
            const unsigned long	dim1560KuoInit[]		=	{	1	,	1	,	3	,	1	,	3	,	47	,	83	,	15	,	49	,	867	,	1513	,	3203	,	3081	,	8429	};
            const unsigned long	dim1561KuoInit[]		=	{	1	,	1	,	5	,	11	,	11	,	33	,	67	,	225	,	303	,	825	,	1711	,	1521	,	1569	,	5233	};
            const unsigned long	dim1562KuoInit[]		=	{	1	,	3	,	3	,	11	,	27	,	41	,	37	,	247	,	211	,	143	,	855	,	3755	,	569	,	10155	};
            const unsigned long	dim1563KuoInit[]		=	{	1	,	1	,	7	,	3	,	21	,	57	,	7	,	83	,	133	,	791	,	565	,	2217	,	7621	,	14141	};
            const unsigned long	dim1564KuoInit[]		=	{	1	,	1	,	5	,	13	,	3	,	57	,	7	,	29	,	131	,	935	,	1555	,	1103	,	7395	,	4079	};
            const unsigned long	dim1565KuoInit[]		=	{	1	,	1	,	7	,	3	,	13	,	9	,	13	,	71	,	345	,	1003	,	1519	,	1757	,	539	,	9207	};
            const unsigned long	dim1566KuoInit[]		=	{	1	,	3	,	1	,	3	,	1	,	11	,	13	,	143	,	365	,	981	,	1691	,	2501	,	7155	,	4189	};
            const unsigned long	dim1567KuoInit[]		=	{	1	,	3	,	3	,	1	,	23	,	37	,	33	,	229	,	411	,	329	,	915	,	2373	,	2525	,	12319	};
            const unsigned long	dim1568KuoInit[]		=	{	1	,	1	,	3	,	3	,	5	,	45	,	77	,	23	,	317	,	311	,	425	,	1329	,	3619	,	11941	};
            const unsigned long	dim1569KuoInit[]		=	{	1	,	3	,	1	,	1	,	19	,	17	,	39	,	37	,	405	,	275	,	983	,	995	,	4925	,	1005	};
            const unsigned long	dim1570KuoInit[]		=	{	1	,	1	,	1	,	5	,	31	,	1	,	71	,	137	,	481	,	489	,	1381	,	3919	,	7841	,	14843	};
            const unsigned long	dim1571KuoInit[]		=	{	1	,	1	,	7	,	15	,	13	,	37	,	11	,	201	,	43	,	311	,	921	,	2755	,	2997	,	4753	};
            const unsigned long	dim1572KuoInit[]		=	{	1	,	1	,	3	,	7	,	27	,	35	,	87	,	203	,	197	,	27	,	1839	,	2847	,	3261	,	3139	};
            const unsigned long	dim1573KuoInit[]		=	{	1	,	1	,	7	,	1	,	25	,	5	,	9	,	9	,	497	,	125	,	1451	,	1381	,	1975	,	10493	};
            const unsigned long	dim1574KuoInit[]		=	{	1	,	1	,	1	,	11	,	11	,	25	,	3	,	9	,	39	,	989	,	975	,	1825	,	7089	,	10475	};
            const unsigned long	dim1575KuoInit[]		=	{	1	,	1	,	5	,	13	,	25	,	33	,	69	,	55	,	179	,	163	,	1233	,	1601	,	1893	,	8225	};
            const unsigned long	dim1576KuoInit[]		=	{	1	,	1	,	3	,	15	,	23	,	11	,	117	,	71	,	151	,	437	,	371	,	1543	,	2417	,	13195	};
            const unsigned long	dim1577KuoInit[]		=	{	1	,	3	,	7	,	9	,	25	,	33	,	63	,	215	,	193	,	415	,	1771	,	715	,	4011	,	11739	};
            const unsigned long	dim1578KuoInit[]		=	{	1	,	3	,	3	,	13	,	17	,	1	,	105	,	169	,	5	,	259	,	169	,	1207	,	5425	,	6091	};
            const unsigned long	dim1579KuoInit[]		=	{	1	,	1	,	5	,	11	,	29	,	43	,	25	,	245	,	33	,	995	,	1797	,	279	,	6443	,	14507	};
            const unsigned long	dim1580KuoInit[]		=	{	1	,	3	,	7	,	7	,	5	,	29	,	117	,	141	,	409	,	367	,	1223	,	1499	,	5651	,	3337	};
            const unsigned long	dim1581KuoInit[]		=	{	1	,	1	,	3	,	9	,	15	,	21	,	21	,	27	,	23	,	693	,	1431	,	2055	,	2959	,	1347	};
            const unsigned long	dim1582KuoInit[]		=	{	1	,	1	,	1	,	9	,	7	,	27	,	67	,	127	,	63	,	213	,	763	,	1221	,	2319	,	5313	};
            const unsigned long	dim1583KuoInit[]		=	{	1	,	1	,	3	,	11	,	21	,	33	,	43	,	243	,	113	,	223	,	1095	,	2433	,	1253	,	16303	};
            const unsigned long	dim1584KuoInit[]		=	{	1	,	1	,	5	,	15	,	21	,	61	,	39	,	147	,	263	,	943	,	321	,	2599	,	3513	,	6595	};
            const unsigned long	dim1585KuoInit[]		=	{	1	,	1	,	7	,	3	,	9	,	25	,	19	,	73	,	253	,	889	,	1699	,	1709	,	6451	,	2939	};
            const unsigned long	dim1586KuoInit[]		=	{	1	,	3	,	3	,	15	,	29	,	29	,	53	,	133	,	349	,	553	,	1677	,	3949	,	1701	,	1187	};
            const unsigned long	dim1587KuoInit[]		=	{	1	,	3	,	7	,	9	,	27	,	5	,	17	,	15	,	185	,	583	,	1117	,	2869	,	6929	,	8117	};
            const unsigned long	dim1588KuoInit[]		=	{	1	,	1	,	5	,	1	,	3	,	15	,	117	,	73	,	321	,	245	,	537	,	2689	,	4883	,	5993	};
            const unsigned long	dim1589KuoInit[]		=	{	1	,	3	,	7	,	15	,	31	,	57	,	103	,	65	,	89	,	323	,	1341	,	1951	,	4999	,	12663	};
            const unsigned long	dim1590KuoInit[]		=	{	1	,	3	,	5	,	5	,	7	,	23	,	77	,	133	,	413	,	157	,	1281	,	3779	,	2081	,	12201	};
            const unsigned long	dim1591KuoInit[]		=	{	1	,	3	,	7	,	13	,	15	,	49	,	119	,	45	,	27	,	621	,	2017	,	1351	,	7647	,	8845	};
            const unsigned long	dim1592KuoInit[]		=	{	1	,	1	,	7	,	15	,	7	,	51	,	77	,	181	,	25	,	769	,	1159	,	1505	,	5219	,	647	};
            const unsigned long	dim1593KuoInit[]		=	{	1	,	1	,	3	,	5	,	3	,	35	,	69	,	43	,	327	,	383	,	755	,	253	,	1137	,	16133	};
            const unsigned long	dim1594KuoInit[]		=	{	1	,	1	,	7	,	9	,	13	,	63	,	69	,	215	,	357	,	873	,	477	,	1243	,	5475	,	5335	};
            const unsigned long	dim1595KuoInit[]		=	{	1	,	3	,	1	,	9	,	27	,	13	,	115	,	197	,	131	,	651	,	741	,	2539	,	265	,	15757	};
            const unsigned long	dim1596KuoInit[]		=	{	1	,	1	,	5	,	11	,	13	,	47	,	45	,	235	,	103	,	597	,	523	,	3129	,	5113	,	7031	};
            const unsigned long	dim1597KuoInit[]		=	{	1	,	1	,	1	,	13	,	31	,	63	,	25	,	75	,	343	,	537	,	83	,	3165	,	3247	,	2589	};
            const unsigned long	dim1598KuoInit[]		=	{	1	,	3	,	5	,	5	,	11	,	13	,	29	,	79	,	233	,	901	,	627	,	1691	,	2659	,	10251	};
            const unsigned long	dim1599KuoInit[]		=	{	1	,	3	,	7	,	1	,	11	,	63	,	123	,	63	,	441	,	931	,	573	,	4083	,	4483	,	5685	};
            const unsigned long	dim1600KuoInit[]		=	{	1	,	3	,	7	,	1	,	23	,	51	,	107	,	113	,	233	,	921	,	1761	,	207	,	5083	,	9165	};
            const unsigned long	dim1601KuoInit[]		=	{	1	,	3	,	3	,	15	,	13	,	23	,	5	,	151	,	487	,	157	,	23	,	3967	,	6241	,	487	};
            const unsigned long	dim1602KuoInit[]		=	{	1	,	1	,	7	,	1	,	7	,	39	,	95	,	185	,	379	,	225	,	1685	,	513	,	2463	,	11033	};
            const unsigned long	dim1603KuoInit[]		=	{	1	,	1	,	5	,	11	,	19	,	43	,	55	,	69	,	433	,	933	,	811	,	3717	,	5555	,	15391	};
            const unsigned long	dim1604KuoInit[]		=	{	1	,	1	,	7	,	7	,	21	,	17	,	19	,	149	,	185	,	475	,	1817	,	913	,	4463	,	12837	};
            const unsigned long	dim1605KuoInit[]		=	{	1	,	1	,	7	,	5	,	27	,	63	,	5	,	131	,	217	,	271	,	889	,	3905	,	2881	,	419	};
            const unsigned long	dim1606KuoInit[]		=	{	1	,	3	,	7	,	11	,	5	,	55	,	103	,	135	,	59	,	775	,	1105	,	2017	,	493	,	12747	};
            const unsigned long	dim1607KuoInit[]		=	{	1	,	1	,	3	,	5	,	9	,	25	,	65	,	9	,	61	,	709	,	1925	,	1769	,	3273	,	1627	};
            const unsigned long	dim1608KuoInit[]		=	{	1	,	3	,	1	,	11	,	7	,	35	,	37	,	223	,	105	,	55	,	293	,	2103	,	7531	,	2795	};
            const unsigned long	dim1609KuoInit[]		=	{	1	,	3	,	7	,	13	,	17	,	23	,	35	,	163	,	177	,	797	,	1539	,	3843	,	6761	,	11817	};
            const unsigned long	dim1610KuoInit[]		=	{	1	,	1	,	3	,	15	,	11	,	19	,	55	,	161	,	33	,	701	,	601	,	1055	,	4949	,	10741	};
            const unsigned long	dim1611KuoInit[]		=	{	1	,	1	,	7	,	3	,	11	,	45	,	71	,	199	,	233	,	917	,	1447	,	3651	,	6021	,	4745	};
            const unsigned long	dim1612KuoInit[]		=	{	1	,	1	,	3	,	11	,	21	,	17	,	39	,	141	,	17	,	827	,	355	,	3337	,	6897	,	9005	};
            const unsigned long	dim1613KuoInit[]		=	{	1	,	3	,	3	,	1	,	11	,	61	,	67	,	103	,	377	,	351	,	1863	,	207	,	7333	,	4481	};
            const unsigned long	dim1614KuoInit[]		=	{	1	,	1	,	1	,	1	,	7	,	33	,	103	,	121	,	361	,	233	,	195	,	985	,	4743	,	8715	};
            const unsigned long	dim1615KuoInit[]		=	{	1	,	3	,	1	,	9	,	17	,	47	,	127	,	129	,	93	,	217	,	1049	,	689	,	5595	,	13625	};
            const unsigned long	dim1616KuoInit[]		=	{	1	,	1	,	5	,	11	,	13	,	33	,	59	,	29	,	439	,	447	,	21	,	621	,	3731	,	9371	};
            const unsigned long	dim1617KuoInit[]		=	{	1	,	3	,	1	,	7	,	15	,	31	,	41	,	217	,	275	,	97	,	143	,	3779	,	5453	,	13167	};
            const unsigned long	dim1618KuoInit[]		=	{	1	,	3	,	7	,	1	,	11	,	31	,	7	,	249	,	193	,	907	,	1275	,	691	,	5701	,	6215	};
            const unsigned long	dim1619KuoInit[]		=	{	1	,	3	,	5	,	1	,	11	,	63	,	7	,	209	,	227	,	959	,	1313	,	1107	,	2281	,	5567	};
            const unsigned long	dim1620KuoInit[]		=	{	1	,	3	,	7	,	9	,	25	,	5	,	103	,	193	,	137	,	345	,	1953	,	369	,	2201	,	9435	};
            const unsigned long	dim1621KuoInit[]		=	{	1	,	1	,	3	,	1	,	5	,	19	,	11	,	191	,	101	,	861	,	1665	,	1439	,	6889	,	2467	};
            const unsigned long	dim1622KuoInit[]		=	{	1	,	1	,	7	,	5	,	19	,	47	,	13	,	123	,	363	,	659	,	1011	,	2961	,	2231	,	11523	};
            const unsigned long	dim1623KuoInit[]		=	{	1	,	3	,	1	,	1	,	9	,	19	,	7	,	1	,	351	,	461	,	1323	,	1125	,	3063	,	9589	};
            const unsigned long	dim1624KuoInit[]		=	{	1	,	1	,	5	,	11	,	1	,	25	,	79	,	171	,	503	,	319	,	729	,	2133	,	1979	,	13769	};
            const unsigned long	dim1625KuoInit[]		=	{	1	,	3	,	1	,	3	,	17	,	45	,	55	,	109	,	143	,	541	,	65	,	431	,	2949	,	11501	};
            const unsigned long	dim1626KuoInit[]		=	{	1	,	3	,	7	,	7	,	7	,	7	,	85	,	81	,	317	,	245	,	1683	,	1009	,	3853	,	12597	};
            const unsigned long	dim1627KuoInit[]		=	{	1	,	1	,	1	,	7	,	9	,	25	,	85	,	19	,	59	,	5	,	1919	,	577	,	7723	,	9365	};
            const unsigned long	dim1628KuoInit[]		=	{	1	,	1	,	7	,	11	,	13	,	27	,	35	,	163	,	209	,	491	,	1759	,	383	,	5697	,	13131	};
            const unsigned long	dim1629KuoInit[]		=	{	1	,	1	,	7	,	13	,	19	,	57	,	121	,	61	,	143	,	735	,	721	,	3405	,	6939	,	4691	};
            const unsigned long	dim1630KuoInit[]		=	{	1	,	1	,	7	,	15	,	21	,	23	,	55	,	203	,	145	,	673	,	419	,	3023	,	6439	,	6361	};
            const unsigned long	dim1631KuoInit[]		=	{	1	,	1	,	5	,	15	,	5	,	21	,	11	,	153	,	223	,	229	,	721	,	715	,	7981	,	15459	};
            const unsigned long	dim1632KuoInit[]		=	{	1	,	3	,	5	,	7	,	21	,	59	,	47	,	15	,	213	,	549	,	465	,	1353	,	2611	,	11989	};
            const unsigned long	dim1633KuoInit[]		=	{	1	,	3	,	5	,	7	,	13	,	51	,	65	,	11	,	295	,	63	,	1887	,	3347	,	3061	,	2219	};
            const unsigned long	dim1634KuoInit[]		=	{	1	,	1	,	5	,	3	,	29	,	29	,	83	,	175	,	507	,	341	,	2031	,	3737	,	1741	,	7615	};
            const unsigned long	dim1635KuoInit[]		=	{	1	,	3	,	7	,	13	,	11	,	55	,	59	,	213	,	197	,	851	,	1723	,	2805	,	2461	,	8071	};
            const unsigned long	dim1636KuoInit[]		=	{	1	,	1	,	3	,	15	,	5	,	37	,	55	,	237	,	319	,	345	,	1677	,	1791	,	5063	,	4301	};
            const unsigned long	dim1637KuoInit[]		=	{	1	,	3	,	1	,	11	,	25	,	13	,	87	,	85	,	293	,	97	,	557	,	3583	,	7077	,	6433	};
            const unsigned long	dim1638KuoInit[]		=	{	1	,	3	,	3	,	1	,	31	,	9	,	63	,	29	,	193	,	351	,	1071	,	3587	,	4695	,	1771	};
            const unsigned long	dim1639KuoInit[]		=	{	1	,	3	,	7	,	11	,	11	,	3	,	45	,	119	,	215	,	367	,	677	,	3405	,	7177	,	14607	};
            const unsigned long	dim1640KuoInit[]		=	{	1	,	3	,	7	,	3	,	3	,	23	,	33	,	33	,	223	,	209	,	1033	,	3167	,	4189	,	2195	};
            const unsigned long	dim1641KuoInit[]		=	{	1	,	1	,	5	,	1	,	3	,	53	,	21	,	151	,	475	,	987	,	1671	,	393	,	7783	,	2561	};
            const unsigned long	dim1642KuoInit[]		=	{	1	,	3	,	7	,	13	,	15	,	5	,	21	,	247	,	225	,	861	,	1707	,	3237	,	5793	,	14557	};
            const unsigned long	dim1643KuoInit[]		=	{	1	,	3	,	5	,	9	,	13	,	3	,	37	,	49	,	119	,	33	,	805	,	681	,	7591	,	1707	};
            const unsigned long	dim1644KuoInit[]		=	{	1	,	1	,	3	,	15	,	27	,	55	,	99	,	17	,	361	,	773	,	1415	,	3547	,	5427	,	4579	};
            const unsigned long	dim1645KuoInit[]		=	{	1	,	3	,	5	,	9	,	17	,	55	,	89	,	111	,	189	,	719	,	1691	,	1565	,	941	,	15481	};
            const unsigned long	dim1646KuoInit[]		=	{	1	,	1	,	1	,	11	,	17	,	3	,	77	,	57	,	71	,	739	,	869	,	165	,	6411	,	1565	};
            const unsigned long	dim1647KuoInit[]		=	{	1	,	3	,	3	,	11	,	31	,	47	,	87	,	105	,	241	,	999	,	245	,	3695	,	5151	,	9087	};
            const unsigned long	dim1648KuoInit[]		=	{	1	,	3	,	1	,	15	,	27	,	59	,	15	,	45	,	413	,	291	,	87	,	1735	,	779	,	5977	};
            const unsigned long	dim1649KuoInit[]		=	{	1	,	3	,	7	,	1	,	9	,	19	,	7	,	183	,	133	,	833	,	1347	,	3559	,	7745	,	11071	};
            const unsigned long	dim1650KuoInit[]		=	{	1	,	1	,	3	,	13	,	23	,	59	,	13	,	227	,	407	,	661	,	217	,	299	,	6033	,	14519	};
            const unsigned long	dim1651KuoInit[]		=	{	1	,	1	,	5	,	5	,	27	,	33	,	105	,	15	,	141	,	555	,	397	,	3287	,	5859	,	9553	};
            const unsigned long	dim1652KuoInit[]		=	{	1	,	3	,	3	,	9	,	27	,	59	,	11	,	47	,	379	,	275	,	889	,	3069	,	4135	,	15735	};
            const unsigned long	dim1653KuoInit[]		=	{	1	,	1	,	5	,	13	,	27	,	47	,	107	,	165	,	487	,	463	,	425	,	3949	,	3213	,	4161	};
            const unsigned long	dim1654KuoInit[]		=	{	1	,	3	,	5	,	7	,	27	,	55	,	17	,	3	,	159	,	181	,	393	,	2441	,	2531	,	895	};
            const unsigned long	dim1655KuoInit[]		=	{	1	,	3	,	7	,	7	,	31	,	3	,	95	,	81	,	181	,	367	,	1689	,	2351	,	193	,	7165	};
            const unsigned long	dim1656KuoInit[]		=	{	1	,	3	,	7	,	15	,	1	,	17	,	103	,	181	,	29	,	851	,	15	,	1447	,	435	,	14059	};
            const unsigned long	dim1657KuoInit[]		=	{	1	,	1	,	1	,	15	,	21	,	31	,	83	,	41	,	237	,	437	,	509	,	3757	,	5609	,	15859	};
            const unsigned long	dim1658KuoInit[]		=	{	1	,	1	,	7	,	15	,	5	,	35	,	101	,	37	,	245	,	487	,	449	,	2089	,	27	,	13667	};
            const unsigned long	dim1659KuoInit[]		=	{	1	,	1	,	7	,	15	,	29	,	49	,	89	,	161	,	271	,	455	,	1599	,	3843	,	1673	,	3683	};
            const unsigned long	dim1660KuoInit[]		=	{	1	,	1	,	7	,	1	,	1	,	25	,	37	,	211	,	369	,	207	,	243	,	2113	,	2707	,	7951	};
            const unsigned long	dim1661KuoInit[]		=	{	1	,	1	,	7	,	5	,	19	,	21	,	25	,	161	,	97	,	505	,	273	,	1443	,	4461	,	1407	};
            const unsigned long	dim1662KuoInit[]		=	{	1	,	1	,	5	,	15	,	17	,	57	,	115	,	175	,	511	,	389	,	1333	,	2459	,	5561	,	4409	};
            const unsigned long	dim1663KuoInit[]		=	{	1	,	1	,	5	,	9	,	29	,	45	,	45	,	121	,	159	,	51	,	865	,	531	,	1387	,	3517	};
            const unsigned long	dim1664KuoInit[]		=	{	1	,	3	,	5	,	15	,	25	,	17	,	59	,	161	,	193	,	495	,	611	,	1009	,	5035	,	15325	};
            const unsigned long	dim1665KuoInit[]		=	{	1	,	1	,	1	,	7	,	7	,	53	,	99	,	241	,	305	,	989	,	175	,	2841	,	2891	,	4975	};
            const unsigned long	dim1666KuoInit[]		=	{	1	,	1	,	7	,	1	,	31	,	45	,	85	,	251	,	339	,	731	,	513	,	2557	,	3119	,	15961	};
            const unsigned long	dim1667KuoInit[]		=	{	1	,	3	,	7	,	15	,	11	,	37	,	73	,	93	,	83	,	931	,	1561	,	791	,	3263	,	10789	};
            const unsigned long	dim1668KuoInit[]		=	{	1	,	3	,	1	,	11	,	27	,	25	,	29	,	71	,	199	,	673	,	173	,	2267	,	1277	,	4373	};
            const unsigned long	dim1669KuoInit[]		=	{	1	,	1	,	5	,	5	,	7	,	19	,	79	,	143	,	63	,	139	,	977	,	845	,	5125	,	5635	};
            const unsigned long	dim1670KuoInit[]		=	{	1	,	1	,	3	,	15	,	3	,	27	,	117	,	219	,	393	,	457	,	1001	,	1235	,	6409	,	10883	};
            const unsigned long	dim1671KuoInit[]		=	{	1	,	1	,	3	,	3	,	19	,	31	,	61	,	225	,	391	,	719	,	1919	,	1843	,	4883	,	10499	};
            const unsigned long	dim1672KuoInit[]		=	{	1	,	3	,	1	,	3	,	13	,	63	,	73	,	103	,	15	,	777	,	1019	,	2677	,	7317	,	4349	};
            const unsigned long	dim1673KuoInit[]		=	{	1	,	3	,	5	,	15	,	5	,	7	,	31	,	33	,	237	,	1021	,	2045	,	2283	,	7045	,	13829	};
            const unsigned long	dim1674KuoInit[]		=	{	1	,	1	,	3	,	13	,	11	,	43	,	47	,	239	,	5	,	293	,	2015	,	3613	,	1551	,	3203	};
            const unsigned long	dim1675KuoInit[]		=	{	1	,	3	,	1	,	11	,	11	,	11	,	67	,	201	,	287	,	161	,	193	,	1815	,	1493	,	2863	};
            const unsigned long	dim1676KuoInit[]		=	{	1	,	3	,	5	,	13	,	11	,	31	,	119	,	57	,	305	,	1009	,	915	,	2855	,	3595	,	11653	};
            const unsigned long	dim1677KuoInit[]		=	{	1	,	1	,	1	,	9	,	17	,	7	,	59	,	27	,	189	,	227	,	763	,	2843	,	7335	,	13559	};
            const unsigned long	dim1678KuoInit[]		=	{	1	,	1	,	5	,	13	,	3	,	1	,	17	,	129	,	83	,	763	,	443	,	895	,	1573	,	7343	};
            const unsigned long	dim1679KuoInit[]		=	{	1	,	3	,	7	,	3	,	5	,	43	,	31	,	95	,	503	,	815	,	1725	,	2319	,	2323	,	12543	};
            const unsigned long	dim1680KuoInit[]		=	{	1	,	1	,	1	,	9	,	29	,	27	,	103	,	43	,	39	,	289	,	277	,	453	,	4297	,	1009	};
            const unsigned long	dim1681KuoInit[]		=	{	1	,	1	,	7	,	5	,	31	,	61	,	105	,	79	,	259	,	141	,	49	,	925	,	2793	,	13247	};
            const unsigned long	dim1682KuoInit[]		=	{	1	,	1	,	1	,	5	,	15	,	23	,	59	,	55	,	497	,	561	,	343	,	3633	,	2097	,	2035	};
            const unsigned long	dim1683KuoInit[]		=	{	1	,	1	,	3	,	13	,	17	,	23	,	13	,	23	,	15	,	519	,	105	,	3719	,	3639	,	6387	};
            const unsigned long	dim1684KuoInit[]		=	{	1	,	1	,	1	,	11	,	1	,	43	,	97	,	73	,	295	,	841	,	1033	,	11	,	3027	,	4073	};
            const unsigned long	dim1685KuoInit[]		=	{	1	,	3	,	1	,	3	,	21	,	15	,	67	,	95	,	425	,	979	,	1641	,	3059	,	4195	,	14291	};
            const unsigned long	dim1686KuoInit[]		=	{	1	,	3	,	5	,	3	,	9	,	23	,	83	,	203	,	479	,	999	,	1469	,	2227	,	7995	,	4053	};
            const unsigned long	dim1687KuoInit[]		=	{	1	,	1	,	5	,	7	,	1	,	13	,	17	,	191	,	297	,	397	,	329	,	1551	,	6885	,	2413	};
            const unsigned long	dim1688KuoInit[]		=	{	1	,	1	,	7	,	13	,	31	,	21	,	123	,	59	,	79	,	647	,	475	,	347	,	667	,	9871	};
            const unsigned long	dim1689KuoInit[]		=	{	1	,	1	,	7	,	15	,	25	,	43	,	35	,	117	,	323	,	805	,	1543	,	2631	,	2919	,	1377	};
            const unsigned long	dim1690KuoInit[]		=	{	1	,	1	,	3	,	11	,	17	,	55	,	45	,	213	,	507	,	641	,	1947	,	2439	,	4749	,	6865	};
            const unsigned long	dim1691KuoInit[]		=	{	1	,	3	,	7	,	5	,	13	,	31	,	51	,	31	,	167	,	431	,	1999	,	1041	,	3613	,	16289	};
            const unsigned long	dim1692KuoInit[]		=	{	1	,	3	,	7	,	9	,	1	,	15	,	19	,	191	,	181	,	247	,	11	,	2305	,	5451	,	1719	};
            const unsigned long	dim1693KuoInit[]		=	{	1	,	3	,	1	,	11	,	25	,	39	,	13	,	123	,	93	,	527	,	1359	,	1985	,	6789	,	2045	};
            const unsigned long	dim1694KuoInit[]		=	{	1	,	1	,	1	,	7	,	25	,	57	,	113	,	91	,	311	,	661	,	1461	,	2801	,	5281	,	13423	};
            const unsigned long	dim1695KuoInit[]		=	{	1	,	1	,	1	,	11	,	31	,	9	,	119	,	55	,	91	,	965	,	1973	,	2901	,	1147	,	9341	};
            const unsigned long	dim1696KuoInit[]		=	{	1	,	1	,	7	,	15	,	9	,	59	,	83	,	21	,	49	,	921	,	749	,	1249	,	3577	,	14341	};
            const unsigned long	dim1697KuoInit[]		=	{	1	,	3	,	5	,	13	,	25	,	63	,	65	,	135	,	337	,	743	,	1669	,	2519	,	341	,	16285	};
            const unsigned long	dim1698KuoInit[]		=	{	1	,	3	,	3	,	7	,	23	,	9	,	97	,	205	,	87	,	289	,	1195	,	2843	,	7419	,	4183	};
            const unsigned long	dim1699KuoInit[]		=	{	1	,	3	,	1	,	9	,	3	,	33	,	117	,	243	,	161	,	977	,	109	,	3333	,	4639	,	2299	};
            const unsigned long	dim1700KuoInit[]		=	{	1	,	1	,	3	,	7	,	1	,	53	,	79	,	73	,	71	,	953	,	349	,	2517	,	6281	,	2625	};
            const unsigned long	dim1701KuoInit[]		=	{	1	,	1	,	5	,	9	,	21	,	59	,	31	,	253	,	265	,	403	,	603	,	2185	,	7277	,	6057	};
            const unsigned long	dim1702KuoInit[]		=	{	1	,	1	,	1	,	13	,	27	,	35	,	63	,	109	,	483	,	205	,	1245	,	2599	,	1697	,	11381	};
            const unsigned long	dim1703KuoInit[]		=	{	1	,	1	,	7	,	15	,	25	,	29	,	107	,	205	,	411	,	627	,	745	,	1585	,	1175	,	3077	};
            const unsigned long	dim1704KuoInit[]		=	{	1	,	3	,	5	,	3	,	25	,	15	,	121	,	97	,	283	,	809	,	753	,	2577	,	1099	,	12155	};
            const unsigned long	dim1705KuoInit[]		=	{	1	,	1	,	7	,	15	,	29	,	21	,	55	,	53	,	101	,	69	,	657	,	2727	,	931	,	4873	};
            const unsigned long	dim1706KuoInit[]		=	{	1	,	3	,	7	,	15	,	5	,	57	,	9	,	11	,	269	,	261	,	1681	,	1025	,	4001	,	16111	};
            const unsigned long	dim1707KuoInit[]		=	{	1	,	1	,	3	,	7	,	9	,	51	,	29	,	77	,	313	,	199	,	1305	,	2665	,	5971	,	7525	};
            const unsigned long	dim1708KuoInit[]		=	{	1	,	1	,	3	,	15	,	3	,	29	,	99	,	255	,	331	,	925	,	1973	,	3825	,	3549	,	8537	};
            const unsigned long	dim1709KuoInit[]		=	{	1	,	1	,	3	,	1	,	27	,	33	,	19	,	225	,	209	,	895	,	47	,	4063	,	1873	,	15631	};
            const unsigned long	dim1710KuoInit[]		=	{	1	,	3	,	1	,	9	,	11	,	57	,	121	,	205	,	89	,	565	,	1387	,	1119	,	7375	,	6153	};
            const unsigned long	dim1711KuoInit[]		=	{	1	,	1	,	3	,	15	,	1	,	27	,	9	,	177	,	443	,	437	,	1487	,	2119	,	5299	,	16251	};
            const unsigned long	dim1712KuoInit[]		=	{	1	,	3	,	7	,	5	,	17	,	33	,	79	,	153	,	327	,	719	,	1587	,	407	,	1137	,	9607	};
            const unsigned long	dim1713KuoInit[]		=	{	1	,	1	,	3	,	15	,	7	,	31	,	109	,	251	,	501	,	553	,	785	,	3047	,	8141	,	6305	};
            const unsigned long	dim1714KuoInit[]		=	{	1	,	1	,	3	,	9	,	11	,	63	,	65	,	135	,	257	,	313	,	191	,	1073	,	6821	,	10629	};
            const unsigned long	dim1715KuoInit[]		=	{	1	,	1	,	7	,	1	,	19	,	55	,	7	,	69	,	503	,	131	,	1133	,	3325	,	347	,	9255	};
            const unsigned long	dim1716KuoInit[]		=	{	1	,	3	,	5	,	3	,	11	,	55	,	115	,	171	,	261	,	581	,	1169	,	3609	,	307	,	14055	};
            const unsigned long	dim1717KuoInit[]		=	{	1	,	3	,	7	,	15	,	29	,	3	,	33	,	229	,	431	,	969	,	281	,	1617	,	3685	,	13583	};
            const unsigned long	dim1718KuoInit[]		=	{	1	,	1	,	7	,	11	,	9	,	23	,	5	,	147	,	305	,	651	,	1419	,	3225	,	7029	,	15331	};
            const unsigned long	dim1719KuoInit[]		=	{	1	,	3	,	7	,	15	,	23	,	61	,	111	,	221	,	145	,	809	,	1127	,	2287	,	6023	,	6103	};
            const unsigned long	dim1720KuoInit[]		=	{	1	,	3	,	1	,	7	,	1	,	49	,	93	,	251	,	239	,	617	,	401	,	3419	,	8123	,	4855	};
            const unsigned long	dim1721KuoInit[]		=	{	1	,	3	,	5	,	5	,	15	,	9	,	69	,	5	,	509	,	83	,	405	,	3451	,	4401	,	3321	};
            const unsigned long	dim1722KuoInit[]		=	{	1	,	1	,	3	,	9	,	29	,	39	,	53	,	155	,	73	,	207	,	499	,	3179	,	6099	,	1377	};
            const unsigned long	dim1723KuoInit[]		=	{	1	,	1	,	1	,	11	,	9	,	43	,	101	,	211	,	439	,	865	,	621	,	3271	,	4699	,	5329	};
            const unsigned long	dim1724KuoInit[]		=	{	1	,	3	,	5	,	9	,	21	,	59	,	105	,	89	,	5	,	493	,	227	,	1177	,	6829	,	8659	};
            const unsigned long	dim1725KuoInit[]		=	{	1	,	1	,	1	,	3	,	27	,	19	,	49	,	209	,	337	,	961	,	93	,	81	,	4607	,	13579	};
            const unsigned long	dim1726KuoInit[]		=	{	1	,	3	,	7	,	1	,	31	,	53	,	77	,	213	,	3	,	555	,	1409	,	3917	,	23	,	7837	};
            const unsigned long	dim1727KuoInit[]		=	{	1	,	3	,	1	,	1	,	21	,	37	,	121	,	17	,	295	,	367	,	1105	,	3053	,	1453	,	8869	};
            const unsigned long	dim1728KuoInit[]		=	{	1	,	3	,	5	,	5	,	31	,	23	,	13	,	145	,	459	,	609	,	77	,	197	,	4457	,	5145	};
            const unsigned long	dim1729KuoInit[]		=	{	1	,	1	,	3	,	7	,	27	,	23	,	83	,	165	,	25	,	491	,	1487	,	2335	,	7019	,	499	};
            const unsigned long	dim1730KuoInit[]		=	{	1	,	1	,	7	,	15	,	29	,	53	,	87	,	47	,	21	,	233	,	253	,	2529	,	1651	,	16091	};
            const unsigned long	dim1731KuoInit[]		=	{	1	,	3	,	5	,	11	,	7	,	55	,	99	,	193	,	395	,	63	,	817	,	2109	,	1167	,	5739	};
            const unsigned long	dim1732KuoInit[]		=	{	1	,	1	,	7	,	15	,	13	,	21	,	61	,	31	,	45	,	723	,	103	,	1347	,	4259	,	13185	};
            const unsigned long	dim1733KuoInit[]		=	{	1	,	1	,	5	,	9	,	7	,	51	,	43	,	21	,	411	,	887	,	1303	,	3389	,	6653	,	9265	};
            const unsigned long	dim1734KuoInit[]		=	{	1	,	1	,	3	,	7	,	3	,	7	,	43	,	197	,	151	,	977	,	717	,	2293	,	3135	,	5223	};
            const unsigned long	dim1735KuoInit[]		=	{	1	,	1	,	1	,	1	,	23	,	49	,	23	,	105	,	279	,	401	,	1309	,	3485	,	4305	,	13181	};
            const unsigned long	dim1736KuoInit[]		=	{	1	,	3	,	1	,	15	,	23	,	15	,	89	,	171	,	237	,	513	,	369	,	3731	,	4753	,	4655	};
            const unsigned long	dim1737KuoInit[]		=	{	1	,	1	,	7	,	11	,	3	,	35	,	49	,	211	,	353	,	843	,	705	,	2237	,	7843	,	9009	};
            const unsigned long	dim1738KuoInit[]		=	{	1	,	3	,	3	,	9	,	29	,	25	,	63	,	29	,	369	,	777	,	477	,	547	,	6575	,	13769	};
            const unsigned long	dim1739KuoInit[]		=	{	1	,	1	,	3	,	1	,	3	,	5	,	101	,	107	,	11	,	817	,	413	,	3241	,	6275	,	4035	};
            const unsigned long	dim1740KuoInit[]		=	{	1	,	1	,	1	,	3	,	21	,	17	,	25	,	113	,	183	,	159	,	307	,	447	,	7881	,	7399	};
            const unsigned long	dim1741KuoInit[]		=	{	1	,	1	,	7	,	15	,	7	,	45	,	39	,	39	,	111	,	191	,	151	,	267	,	7133	,	5557	};
            const unsigned long	dim1742KuoInit[]		=	{	1	,	3	,	7	,	5	,	27	,	39	,	29	,	59	,	141	,	835	,	1307	,	1007	,	2101	,	7675	};
            const unsigned long	dim1743KuoInit[]		=	{	1	,	3	,	1	,	11	,	9	,	15	,	65	,	47	,	371	,	385	,	683	,	3495	,	711	,	3963	};
            const unsigned long	dim1744KuoInit[]		=	{	1	,	1	,	7	,	3	,	5	,	59	,	81	,	9	,	23	,	885	,	291	,	2349	,	5495	,	12985	};
            const unsigned long	dim1745KuoInit[]		=	{	1	,	3	,	5	,	1	,	21	,	49	,	99	,	101	,	447	,	289	,	659	,	1061	,	5771	,	7591	};
            const unsigned long	dim1746KuoInit[]		=	{	1	,	3	,	5	,	7	,	5	,	11	,	13	,	255	,	7	,	775	,	1919	,	1965	,	7069	,	10379	};
            const unsigned long	dim1747KuoInit[]		=	{	1	,	1	,	7	,	15	,	29	,	23	,	61	,	183	,	117	,	817	,	113	,	1665	,	2349	,	305	};
            const unsigned long	dim1748KuoInit[]		=	{	1	,	3	,	7	,	13	,	25	,	9	,	97	,	161	,	361	,	33	,	1781	,	3717	,	6319	,	13199	};
            const unsigned long	dim1749KuoInit[]		=	{	1	,	3	,	5	,	13	,	21	,	63	,	85	,	225	,	447	,	169	,	1601	,	2359	,	8087	,	2609	};
            const unsigned long	dim1750KuoInit[]		=	{	1	,	1	,	3	,	9	,	1	,	11	,	1	,	173	,	345	,	217	,	1131	,	493	,	4613	,	9223	};
            const unsigned long	dim1751KuoInit[]		=	{	1	,	3	,	7	,	3	,	5	,	51	,	55	,	235	,	129	,	451	,	1191	,	2065	,	6781	,	9135	};
            const unsigned long	dim1752KuoInit[]		=	{	1	,	3	,	7	,	1	,	27	,	41	,	123	,	49	,	495	,	999	,	1731	,	1597	,	7471	,	1129	};
            const unsigned long	dim1753KuoInit[]		=	{	1	,	1	,	5	,	3	,	13	,	3	,	123	,	83	,	199	,	325	,	1521	,	1023	,	2703	,	10915	};
            const unsigned long	dim1754KuoInit[]		=	{	1	,	1	,	3	,	1	,	15	,	19	,	45	,	223	,	17	,	643	,	661	,	633	,	6475	,	10153	};
            const unsigned long	dim1755KuoInit[]		=	{	1	,	1	,	7	,	9	,	17	,	21	,	101	,	137	,	39	,	975	,	1003	,	1419	,	353	,	11971	};
            const unsigned long	dim1756KuoInit[]		=	{	1	,	3	,	5	,	3	,	11	,	37	,	75	,	87	,	49	,	535	,	801	,	1689	,	8125	,	12419	};
            const unsigned long	dim1757KuoInit[]		=	{	1	,	3	,	3	,	3	,	19	,	11	,	99	,	97	,	285	,	641	,	1485	,	257	,	5417	,	8669	};
            const unsigned long	dim1758KuoInit[]		=	{	1	,	1	,	1	,	1	,	21	,	53	,	19	,	61	,	223	,	917	,	931	,	1779	,	5859	,	3111	};
            const unsigned long	dim1759KuoInit[]		=	{	1	,	3	,	1	,	13	,	25	,	17	,	55	,	227	,	61	,	953	,	255	,	3939	,	2523	,	2119	};
            const unsigned long	dim1760KuoInit[]		=	{	1	,	1	,	7	,	11	,	23	,	43	,	111	,	21	,	263	,	399	,	723	,	1067	,	3633	,	4165	};
            const unsigned long	dim1761KuoInit[]		=	{	1	,	3	,	7	,	3	,	31	,	29	,	69	,	135	,	105	,	839	,	1077	,	1479	,	643	,	1063	};
            const unsigned long	dim1762KuoInit[]		=	{	1	,	3	,	1	,	11	,	23	,	29	,	51	,	235	,	287	,	941	,	393	,	2241	,	1601	,	4629	};
            const unsigned long	dim1763KuoInit[]		=	{	1	,	3	,	3	,	1	,	1	,	7	,	47	,	119	,	369	,	789	,	1943	,	3417	,	4073	,	7495	};
            const unsigned long	dim1764KuoInit[]		=	{	1	,	1	,	7	,	7	,	21	,	37	,	25	,	217	,	267	,	275	,	903	,	865	,	6251	,	1323	};
            const unsigned long	dim1765KuoInit[]		=	{	1	,	3	,	7	,	15	,	13	,	61	,	77	,	181	,	479	,	1003	,	335	,	2917	,	1133	,	14321	};
            const unsigned long	dim1766KuoInit[]		=	{	1	,	1	,	7	,	7	,	9	,	53	,	47	,	197	,	257	,	823	,	421	,	2465	,	4333	,	3841	};
            const unsigned long	dim1767KuoInit[]		=	{	1	,	3	,	3	,	9	,	23	,	17	,	83	,	131	,	427	,	25	,	319	,	517	,	4109	,	15841	};
            const unsigned long	dim1768KuoInit[]		=	{	1	,	1	,	7	,	7	,	7	,	25	,	105	,	63	,	117	,	173	,	909	,	2731	,	877	,	1095	};
            const unsigned long	dim1769KuoInit[]		=	{	1	,	1	,	3	,	9	,	29	,	17	,	125	,	45	,	269	,	681	,	1389	,	283	,	7217	,	15315	};
            const unsigned long	dim1770KuoInit[]		=	{	1	,	1	,	7	,	3	,	3	,	59	,	29	,	177	,	191	,	803	,	809	,	131	,	857	,	16213	};
            const unsigned long	dim1771KuoInit[]		=	{	1	,	1	,	1	,	15	,	15	,	21	,	41	,	177	,	245	,	543	,	761	,	2423	,	1381	,	1125	};
            const unsigned long	dim1772KuoInit[]		=	{	1	,	3	,	5	,	13	,	31	,	5	,	121	,	183	,	477	,	143	,	559	,	765	,	703	,	12255	};
            const unsigned long	dim1773KuoInit[]		=	{	1	,	1	,	1	,	15	,	21	,	23	,	127	,	227	,	331	,	963	,	333	,	2101	,	2215	,	10107	};
            const unsigned long	dim1774KuoInit[]		=	{	1	,	3	,	7	,	3	,	1	,	47	,	53	,	189	,	331	,	587	,	1417	,	731	,	4317	,	11629	};
            const unsigned long	dim1775KuoInit[]		=	{	1	,	3	,	7	,	13	,	11	,	21	,	45	,	109	,	455	,	347	,	1483	,	2589	,	6933	,	8343	};
            const unsigned long	dim1776KuoInit[]		=	{	1	,	1	,	1	,	11	,	5	,	31	,	45	,	85	,	253	,	729	,	111	,	3315	,	3183	,	12847	};
            const unsigned long	dim1777KuoInit[]		=	{	1	,	3	,	1	,	1	,	25	,	5	,	3	,	219	,	263	,	517	,	247	,	2303	,	2709	,	11973	};
            const unsigned long	dim1778KuoInit[]		=	{	1	,	1	,	5	,	11	,	21	,	39	,	115	,	231	,	5	,	451	,	1503	,	1527	,	4071	,	5879	};
            const unsigned long	dim1779KuoInit[]		=	{	1	,	3	,	5	,	15	,	23	,	31	,	15	,	61	,	313	,	83	,	49	,	1377	,	7029	,	14187	};
            const unsigned long	dim1780KuoInit[]		=	{	1	,	3	,	7	,	9	,	29	,	7	,	117	,	111	,	493	,	159	,	513	,	3841	,	7011	,	14723	};
            const unsigned long	dim1781KuoInit[]		=	{	1	,	1	,	5	,	13	,	21	,	15	,	3	,	139	,	463	,	1023	,	1301	,	243	,	5105	,	2259	};
            const unsigned long	dim1782KuoInit[]		=	{	1	,	1	,	7	,	13	,	5	,	3	,	119	,	111	,	447	,	75	,	1161	,	1917	,	345	,	557	};
            const unsigned long	dim1783KuoInit[]		=	{	1	,	1	,	5	,	5	,	25	,	21	,	87	,	93	,	237	,	921	,	475	,	2555	,	4105	,	5691	};
            const unsigned long	dim1784KuoInit[]		=	{	1	,	1	,	3	,	15	,	29	,	51	,	5	,	73	,	471	,	421	,	861	,	3833	,	2531	,	1059	};
            const unsigned long	dim1785KuoInit[]		=	{	1	,	3	,	7	,	9	,	3	,	11	,	101	,	35	,	271	,	981	,	1661	,	1629	,	7321	,	1335	};
            const unsigned long	dim1786KuoInit[]		=	{	1	,	1	,	5	,	9	,	9	,	37	,	9	,	127	,	45	,	215	,	611	,	1193	,	659	,	5975	};
            const unsigned long	dim1787KuoInit[]		=	{	1	,	1	,	3	,	9	,	31	,	31	,	21	,	167	,	415	,	513	,	1917	,	829	,	1161	,	3585	};
            const unsigned long	dim1788KuoInit[]		=	{	1	,	1	,	7	,	1	,	19	,	23	,	49	,	55	,	169	,	351	,	1389	,	3251	,	5001	,	13445	};
            const unsigned long	dim1789KuoInit[]		=	{	1	,	3	,	1	,	15	,	3	,	3	,	113	,	193	,	391	,	851	,	681	,	2961	,	7239	,	12837	};
            const unsigned long	dim1790KuoInit[]		=	{	1	,	3	,	5	,	15	,	5	,	43	,	71	,	1	,	347	,	635	,	1123	,	3399	,	2087	,	8209	};
            const unsigned long	dim1791KuoInit[]		=	{	1	,	1	,	3	,	15	,	23	,	7	,	59	,	115	,	139	,	311	,	893	,	2421	,	405	,	6541	};
            const unsigned long	dim1792KuoInit[]		=	{	1	,	3	,	3	,	3	,	25	,	23	,	127	,	135	,	199	,	661	,	7	,	3279	,	91	,	11879	};
            const unsigned long	dim1793KuoInit[]		=	{	1	,	1	,	3	,	3	,	19	,	37	,	33	,	159	,	3	,	425	,	379	,	1321	,	369	,	13125	};
            const unsigned long	dim1794KuoInit[]		=	{	1	,	3	,	5	,	15	,	7	,	17	,	47	,	47	,	167	,	909	,	403	,	95	,	7861	,	231	};
            const unsigned long	dim1795KuoInit[]		=	{	1	,	1	,	5	,	1	,	15	,	5	,	69	,	179	,	229	,	597	,	1743	,	467	,	1859	,	2059	};
            const unsigned long	dim1796KuoInit[]		=	{	1	,	1	,	7	,	15	,	5	,	63	,	109	,	191	,	271	,	281	,	907	,	489	,	5863	,	12697	};
            const unsigned long	dim1797KuoInit[]		=	{	1	,	1	,	3	,	3	,	31	,	1	,	67	,	81	,	3	,	755	,	1839	,	1481	,	4691	,	14549	};
            const unsigned long	dim1798KuoInit[]		=	{	1	,	3	,	3	,	15	,	17	,	13	,	59	,	23	,	85	,	951	,	1259	,	363	,	5479	,	10705	};
            const unsigned long	dim1799KuoInit[]		=	{	1	,	1	,	3	,	15	,	11	,	61	,	111	,	109	,	361	,	483	,	1691	,	1039	,	3777	,	14613	};
            const unsigned long	dim1800KuoInit[]		=	{	1	,	1	,	3	,	5	,	29	,	55	,	101	,	55	,	407	,	61	,	607	,	829	,	8011	,	14021	};
            const unsigned long	dim1801KuoInit[]		=	{	1	,	1	,	1	,	13	,	1	,	39	,	29	,	19	,	389	,	85	,	313	,	1123	,	1303	,	11535	};
            const unsigned long	dim1802KuoInit[]		=	{	1	,	1	,	1	,	5	,	5	,	45	,	47	,	139	,	165	,	937	,	1677	,	2973	,	501	,	4387	};
            const unsigned long	dim1803KuoInit[]		=	{	1	,	3	,	5	,	9	,	5	,	55	,	89	,	235	,	29	,	75	,	1839	,	801	,	5753	,	4409	};
            const unsigned long	dim1804KuoInit[]		=	{	1	,	1	,	3	,	5	,	7	,	41	,	7	,	185	,	297	,	661	,	1955	,	2193	,	6001	,	12349	};
            const unsigned long	dim1805KuoInit[]		=	{	1	,	1	,	7	,	15	,	27	,	47	,	7	,	7	,	183	,	725	,	1511	,	3021	,	6091	,	10567	};
            const unsigned long	dim1806KuoInit[]		=	{	1	,	3	,	3	,	13	,	21	,	7	,	79	,	133	,	307	,	759	,	369	,	2779	,	7629	,	16191	};
            const unsigned long	dim1807KuoInit[]		=	{	1	,	1	,	5	,	9	,	5	,	47	,	15	,	141	,	293	,	339	,	1471	,	127	,	5805	,	5543	};
            const unsigned long	dim1808KuoInit[]		=	{	1	,	3	,	1	,	9	,	31	,	7	,	125	,	27	,	511	,	549	,	619	,	743	,	3471	,	8595	};
            const unsigned long	dim1809KuoInit[]		=	{	1	,	1	,	7	,	5	,	15	,	9	,	25	,	35	,	161	,	691	,	659	,	909	,	969	,	9285	};
            const unsigned long	dim1810KuoInit[]		=	{	1	,	1	,	7	,	11	,	31	,	17	,	59	,	255	,	281	,	7	,	1075	,	2561	,	6883	,	8193	};
            const unsigned long	dim1811KuoInit[]		=	{	1	,	1	,	7	,	1	,	9	,	15	,	83	,	253	,	415	,	601	,	417	,	3999	,	7513	,	3663	};
            const unsigned long	dim1812KuoInit[]		=	{	1	,	3	,	5	,	7	,	11	,	25	,	109	,	135	,	41	,	5	,	697	,	1703	,	8175	,	11369	};
            const unsigned long	dim1813KuoInit[]		=	{	1	,	1	,	7	,	15	,	7	,	39	,	1	,	253	,	373	,	273	,	857	,	401	,	1609	,	2391	};
            const unsigned long	dim1814KuoInit[]		=	{	1	,	1	,	5	,	1	,	1	,	61	,	85	,	213	,	183	,	695	,	523	,	1237	,	5485	,	6843	};
            const unsigned long	dim1815KuoInit[]		=	{	1	,	1	,	7	,	9	,	17	,	41	,	125	,	209	,	285	,	823	,	857	,	31	,	839	,	10847	};
            const unsigned long	dim1816KuoInit[]		=	{	1	,	3	,	5	,	9	,	31	,	23	,	65	,	75	,	243	,	471	,	669	,	539	,	3949	,	14913	};
            const unsigned long	dim1817KuoInit[]		=	{	1	,	1	,	7	,	3	,	25	,	23	,	109	,	19	,	89	,	983	,	193	,	1433	,	6401	,	663	};
            const unsigned long	dim1818KuoInit[]		=	{	1	,	3	,	1	,	15	,	25	,	9	,	93	,	13	,	133	,	561	,	1343	,	4049	,	6221	,	3137	};
            const unsigned long	dim1819KuoInit[]		=	{	1	,	1	,	3	,	3	,	31	,	47	,	105	,	225	,	231	,	75	,	187	,	963	,	3339	,	9705	};
            const unsigned long	dim1820KuoInit[]		=	{	1	,	3	,	7	,	1	,	9	,	31	,	21	,	221	,	309	,	791	,	2011	,	241	,	5507	,	11579	};
            const unsigned long	dim1821KuoInit[]		=	{	1	,	3	,	7	,	9	,	1	,	19	,	53	,	239	,	305	,	151	,	53	,	873	,	5235	,	13357	};
            const unsigned long	dim1822KuoInit[]		=	{	1	,	1	,	5	,	15	,	13	,	27	,	23	,	255	,	75	,	435	,	209	,	2001	,	2529	,	5363	};
            const unsigned long	dim1823KuoInit[]		=	{	1	,	1	,	7	,	5	,	25	,	53	,	71	,	203	,	259	,	403	,	1535	,	3381	,	6791	,	16061	};
            const unsigned long	dim1824KuoInit[]		=	{	1	,	3	,	5	,	3	,	29	,	5	,	93	,	5	,	179	,	813	,	1569	,	2875	,	7855	,	13863	};
            const unsigned long	dim1825KuoInit[]		=	{	1	,	1	,	3	,	7	,	1	,	13	,	97	,	125	,	479	,	209	,	1659	,	2997	,	3405	,	9707	};
            const unsigned long	dim1826KuoInit[]		=	{	1	,	3	,	1	,	9	,	5	,	43	,	15	,	29	,	317	,	317	,	1317	,	3177	,	519	,	8115	};
            const unsigned long	dim1827KuoInit[]		=	{	1	,	1	,	1	,	5	,	1	,	55	,	49	,	145	,	415	,	785	,	1489	,	1217	,	3829	,	10207	};
            const unsigned long	dim1828KuoInit[]		=	{	1	,	3	,	3	,	9	,	17	,	5	,	99	,	235	,	277	,	921	,	71	,	343	,	2935	,	12495	};
            const unsigned long	dim1829KuoInit[]		=	{	1	,	1	,	3	,	15	,	25	,	11	,	11	,	69	,	209	,	543	,	17	,	1489	,	8119	,	3557	};
            const unsigned long	dim1830KuoInit[]		=	{	1	,	1	,	7	,	9	,	29	,	43	,	125	,	87	,	11	,	947	,	1709	,	17	,	3467	,	3339	};
            const unsigned long	dim1831KuoInit[]		=	{	1	,	3	,	5	,	15	,	23	,	47	,	113	,	185	,	75	,	523	,	1253	,	859	,	6359	,	15563	};
            const unsigned long	dim1832KuoInit[]		=	{	1	,	1	,	7	,	11	,	1	,	25	,	25	,	23	,	267	,	275	,	1751	,	3237	,	3967	,	9943	};
            const unsigned long	dim1833KuoInit[]		=	{	1	,	1	,	1	,	13	,	7	,	35	,	43	,	151	,	489	,	97	,	143	,	257	,	771	,	8181	};
            const unsigned long	dim1834KuoInit[]		=	{	1	,	1	,	3	,	7	,	19	,	11	,	17	,	23	,	425	,	869	,	525	,	95	,	2217	,	3931	};
            const unsigned long	dim1835KuoInit[]		=	{	1	,	3	,	7	,	11	,	3	,	19	,	77	,	219	,	501	,	23	,	773	,	2421	,	3713	,	6209	};
            const unsigned long	dim1836KuoInit[]		=	{	1	,	1	,	3	,	1	,	7	,	1	,	33	,	35	,	229	,	329	,	201	,	291	,	4165	,	16059	};
            const unsigned long	dim1837KuoInit[]		=	{	1	,	3	,	5	,	9	,	17	,	23	,	107	,	23	,	3	,	673	,	609	,	2931	,	5989	,	3133	};
            const unsigned long	dim1838KuoInit[]		=	{	1	,	3	,	3	,	3	,	17	,	59	,	31	,	63	,	111	,	287	,	407	,	769	,	2381	,	2203	};
            const unsigned long	dim1839KuoInit[]		=	{	1	,	1	,	1	,	15	,	3	,	51	,	105	,	57	,	313	,	847	,	261	,	2697	,	749	,	15419	};
            const unsigned long	dim1840KuoInit[]		=	{	1	,	3	,	1	,	11	,	3	,	21	,	65	,	139	,	363	,	21	,	1889	,	1007	,	6969	,	3609	};
            const unsigned long	dim1841KuoInit[]		=	{	1	,	3	,	7	,	15	,	17	,	51	,	73	,	121	,	481	,	339	,	1409	,	131	,	2005	,	11847	};
            const unsigned long	dim1842KuoInit[]		=	{	1	,	3	,	5	,	3	,	31	,	13	,	107	,	253	,	125	,	973	,	1027	,	3473	,	6883	,	15061	};
            const unsigned long	dim1843KuoInit[]		=	{	1	,	3	,	1	,	5	,	25	,	17	,	11	,	255	,	209	,	35	,	1987	,	2275	,	2327	,	16079	};
            const unsigned long	dim1844KuoInit[]		=	{	1	,	3	,	7	,	5	,	3	,	1	,	25	,	93	,	397	,	467	,	1895	,	3199	,	1861	,	10943	};
            const unsigned long	dim1845KuoInit[]		=	{	1	,	3	,	7	,	1	,	23	,	27	,	13	,	211	,	299	,	117	,	1689	,	457	,	6335	,	11921	};
            const unsigned long	dim1846KuoInit[]		=	{	1	,	1	,	5	,	5	,	9	,	59	,	49	,	177	,	81	,	913	,	429	,	2705	,	339	,	9347	};
            const unsigned long	dim1847KuoInit[]		=	{	1	,	3	,	1	,	15	,	1	,	61	,	81	,	143	,	455	,	311	,	381	,	235	,	2537	,	12511	};
            const unsigned long	dim1848KuoInit[]		=	{	1	,	3	,	5	,	11	,	7	,	33	,	103	,	137	,	377	,	717	,	851	,	2939	,	303	,	11175	};
            const unsigned long	dim1849KuoInit[]		=	{	1	,	3	,	3	,	13	,	25	,	59	,	15	,	43	,	71	,	767	,	47	,	3425	,	7015	,	14431	};
            const unsigned long	dim1850KuoInit[]		=	{	1	,	3	,	3	,	9	,	25	,	37	,	37	,	39	,	279	,	491	,	1483	,	2633	,	3791	,	14311	};
            const unsigned long	dim1851KuoInit[]		=	{	1	,	1	,	1	,	13	,	29	,	63	,	83	,	89	,	339	,	923	,	455	,	965	,	7339	,	16383	};
            const unsigned long	dim1852KuoInit[]		=	{	1	,	1	,	3	,	1	,	9	,	33	,	7	,	63	,	385	,	717	,	1449	,	2705	,	6233	,	10753	};
            const unsigned long	dim1853KuoInit[]		=	{	1	,	3	,	1	,	11	,	15	,	13	,	11	,	181	,	271	,	749	,	1825	,	875	,	2497	,	12999	};
            const unsigned long	dim1854KuoInit[]		=	{	1	,	3	,	3	,	7	,	13	,	49	,	35	,	65	,	467	,	259	,	429	,	549	,	5481	,	11343	};
            const unsigned long	dim1855KuoInit[]		=	{	1	,	3	,	7	,	15	,	17	,	55	,	125	,	217	,	91	,	495	,	133	,	1985	,	4871	,	3415	};
            const unsigned long	dim1856KuoInit[]		=	{	1	,	3	,	3	,	13	,	11	,	33	,	29	,	53	,	253	,	935	,	1919	,	2607	,	3205	,	819	};		
            const unsigned long	dim1857KuoInit[]		=	{	1	,	1	,	1	,	3	,	25	,	23	,	127	,	151	,	39	,	583	,	985	,	715	,	765	,	16343	};		
            const unsigned long	dim1858KuoInit[]		=	{	1	,	3	,	5	,	3	,	7	,	21	,	127	,	179	,	239	,	353	,	221	,	531	,	5147	,	6699	};		
            const unsigned long	dim1859KuoInit[]		=	{	1	,	1	,	7	,	9	,	7	,	63	,	91	,	99	,	495	,	111	,	1295	,	2609	,	6389	,	7719	};		
            const unsigned long	dim1860KuoInit[]		=	{	1	,	3	,	5	,	7	,	9	,	27	,	123	,	171	,	155	,	683	,	1209	,	2081	,	2823	,	1007	};		
            const unsigned long	dim1861KuoInit[]		=	{	1	,	3	,	3	,	15	,	25	,	45	,	91	,	93	,	329	,	961	,	1843	,	2637	,	4333	,	8359	};		
            const unsigned long	dim1862KuoInit[]		=	{	1	,	3	,	1	,	5	,	9	,	5	,	73	,	221	,	71	,	951	,	1175	,	2489	,	4565	,	7221	};		
            const unsigned long	dim1863KuoInit[]		=	{	1	,	3	,	1	,	3	,	31	,	33	,	67	,	127	,	427	,	487	,	239	,	787	,	1731	,	11867	};		
            const unsigned long	dim1864KuoInit[]		=	{	1	,	1	,	1	,	9	,	17	,	23	,	5	,	201	,	175	,	661	,	1381	,	3017	,	4237	,	14403	};		
            const unsigned long	dim1865KuoInit[]		=	{	1	,	3	,	7	,	7	,	31	,	7	,	39	,	141	,	299	,	237	,	281	,	3057	,	5553	,	11339	};		
            const unsigned long	dim1866KuoInit[]		=	{	1	,	1	,	1	,	15	,	25	,	49	,	51	,	135	,	363	,	377	,	1767	,	4069	,	3001	,	7111	};		
            const unsigned long	dim1867KuoInit[]		=	{	1	,	3	,	7	,	11	,	29	,	7	,	91	,	145	,	263	,	581	,	35	,	3101	,	6227	,	12007	,	30807	};
            const unsigned long	dim1868KuoInit[]		=	{	1	,	1	,	1	,	13	,	11	,	41	,	61	,	135	,	61	,	501	,	189	,	1923	,	129	,	14563	,	12247	};
            const unsigned long	dim1869KuoInit[]		=	{	1	,	1	,	3	,	5	,	17	,	45	,	53	,	5	,	199	,	165	,	859	,	771	,	4553	,	14723	,	17293	};
            const unsigned long	dim1870KuoInit[]		=	{	1	,	1	,	3	,	15	,	27	,	57	,	41	,	3	,	437	,	543	,	1291	,	1083	,	1311	,	4407	,	9107	};
            const unsigned long	dim1871KuoInit[]		=	{	1	,	1	,	3	,	11	,	19	,	23	,	17	,	251	,	267	,	575	,	1675	,	775	,	1281	,	12389	,	11781	};
            const unsigned long	dim1872KuoInit[]		=	{	1	,	3	,	3	,	9	,	19	,	41	,	69	,	45	,	113	,	683	,	1843	,	1217	,	1075	,	7141	,	11259	};
            const unsigned long	dim1873KuoInit[]		=	{	1	,	3	,	5	,	5	,	21	,	45	,	75	,	199	,	227	,	717	,	267	,	3211	,	5893	,	6559	,	16445	};
            const unsigned long	dim1874KuoInit[]		=	{	1	,	1	,	3	,	15	,	25	,	19	,	13	,	3	,	477	,	511	,	1607	,	61	,	5937	,	6471	,	4561	};
            const unsigned long	dim1875KuoInit[]		=	{	1	,	1	,	7	,	9	,	25	,	9	,	59	,	209	,	181	,	671	,	1473	,	1863	,	6835	,	5915	,	21573	};
            const unsigned long	dim1876KuoInit[]		=	{	1	,	1	,	7	,	5	,	31	,	43	,	91	,	191	,	505	,	979	,	1431	,	1347	,	1445	,	9759	,	31653	};
            const unsigned long	dim1877KuoInit[]		=	{	1	,	1	,	5	,	3	,	11	,	63	,	35	,	1	,	313	,	863	,	577	,	3039	,	5205	,	5089	,	20009	};
            const unsigned long	dim1878KuoInit[]		=	{	1	,	3	,	3	,	7	,	1	,	27	,	79	,	21	,	179	,	187	,	917	,	1831	,	1737	,	2879	,	5801	};
            const unsigned long	dim1879KuoInit[]		=	{	1	,	1	,	1	,	3	,	19	,	51	,	77	,	201	,	499	,	345	,	617	,	33	,	543	,	6925	,	5945	};
            const unsigned long	dim1880KuoInit[]		=	{	1	,	3	,	1	,	1	,	19	,	49	,	67	,	51	,	181	,	1015	,	1159	,	2109	,	879	,	10257	,	23207	};
            const unsigned long	dim1881KuoInit[]		=	{	1	,	1	,	7	,	7	,	19	,	29	,	7	,	111	,	365	,	373	,	1309	,	3633	,	5951	,	2597	,	27947	};
            const unsigned long	dim1882KuoInit[]		=	{	1	,	3	,	5	,	13	,	21	,	27	,	85	,	71	,	149	,	665	,	1499	,	4025	,	3939	,	8947	,	15811	};
            const unsigned long	dim1883KuoInit[]		=	{	1	,	3	,	7	,	5	,	25	,	47	,	77	,	99	,	105	,	739	,	1077	,	3145	,	7859	,	1925	,	16765	};
            const unsigned long	dim1884KuoInit[]		=	{	1	,	1	,	3	,	11	,	25	,	9	,	113	,	105	,	309	,	397	,	1687	,	1171	,	455	,	3183	,	12487	};
            const unsigned long	dim1885KuoInit[]		=	{	1	,	1	,	7	,	15	,	7	,	9	,	51	,	239	,	469	,	415	,	1895	,	89	,	4243	,	967	,	7113	};
            const unsigned long	dim1886KuoInit[]		=	{	1	,	1	,	5	,	1	,	23	,	41	,	39	,	247	,	115	,	541	,	1461	,	2907	,	2175	,	3133	,	3171	};
            const unsigned long	dim1887KuoInit[]		=	{	1	,	3	,	7	,	11	,	7	,	53	,	43	,	187	,	213	,	549	,	947	,	3695	,	3931	,	699	,	21299	};
            const unsigned long	dim1888KuoInit[]		=	{	1	,	1	,	1	,	7	,	15	,	9	,	9	,	3	,	175	,	231	,	1529	,	337	,	5333	,	6945	,	1359	};
            const unsigned long	dim1889KuoInit[]		=	{	1	,	1	,	3	,	3	,	13	,	25	,	123	,	231	,	389	,	351	,	1695	,	2959	,	1037	,	2103	,	327	};
            const unsigned long	dim1890KuoInit[]		=	{	1	,	3	,	5	,	11	,	9	,	31	,	19	,	149	,	1	,	477	,	1819	,	3487	,	3299	,	8835	,	10351	};
            const unsigned long	dim1891KuoInit[]		=	{	1	,	1	,	5	,	7	,	9	,	37	,	123	,	93	,	267	,	481	,	901	,	3927	,	7295	,	13251	,	22551	};
            const unsigned long	dim1892KuoInit[]		=	{	1	,	1	,	7	,	11	,	21	,	13	,	127	,	173	,	459	,	169	,	1511	,	1649	,	5431	,	8637	,	3601	};
            const unsigned long	dim1893KuoInit[]		=	{	1	,	1	,	1	,	9	,	29	,	3	,	101	,	135	,	159	,	295	,	1767	,	3089	,	3753	,	4969	,	21523	};
            const unsigned long	dim1894KuoInit[]		=	{	1	,	3	,	5	,	3	,	3	,	9	,	45	,	143	,	301	,	307	,	191	,	3337	,	6721	,	12095	,	7017	};
            const unsigned long	dim1895KuoInit[]		=	{	1	,	3	,	7	,	13	,	17	,	25	,	55	,	37	,	379	,	725	,	1079	,	311	,	2853	,	15097	,	31857	};
            const unsigned long	dim1896KuoInit[]		=	{	1	,	3	,	1	,	13	,	19	,	57	,	39	,	99	,	39	,	283	,	425	,	2447	,	2077	,	815	,	7639	};
            const unsigned long	dim1897KuoInit[]		=	{	1	,	3	,	3	,	15	,	11	,	5	,	17	,	83	,	231	,	535	,	35	,	2773	,	4127	,	9951	,	5773	};
            const unsigned long	dim1898KuoInit[]		=	{	1	,	3	,	3	,	15	,	7	,	47	,	87	,	119	,	291	,	523	,	1325	,	1757	,	2377	,	6481	,	7901	};
            const unsigned long	dim1899KuoInit[]		=	{	1	,	3	,	7	,	15	,	25	,	21	,	29	,	87	,	241	,	241	,	1273	,	2813	,	2011	,	8549	,	7253	};
            const unsigned long	dim1900KuoInit[]		=	{	1	,	1	,	1	,	7	,	19	,	35	,	119	,	227	,	483	,	1015	,	685	,	1149	,	4823	,	4113	,	7169	};
            const unsigned long	dim1901KuoInit[]		=	{	1	,	3	,	3	,	5	,	11	,	59	,	125	,	99	,	255	,	149	,	829	,	2869	,	7927	,	6779	,	31949	};
            const unsigned long	dim1902KuoInit[]		=	{	1	,	3	,	7	,	3	,	3	,	49	,	21	,	111	,	219	,	33	,	91	,	1585	,	303	,	14931	,	11777	};
            const unsigned long	dim1903KuoInit[]		=	{	1	,	3	,	3	,	11	,	3	,	47	,	27	,	227	,	477	,	891	,	1281	,	1927	,	7325	,	14075	,	26633	};
            const unsigned long	dim1904KuoInit[]		=	{	1	,	1	,	5	,	5	,	5	,	29	,	25	,	23	,	223	,	65	,	1511	,	2563	,	7825	,	7339	,	32095	};
            const unsigned long	dim1905KuoInit[]		=	{	1	,	1	,	5	,	7	,	3	,	29	,	49	,	83	,	317	,	513	,	755	,	1339	,	4829	,	6213	,	29055	};
            const unsigned long	dim1906KuoInit[]		=	{	1	,	1	,	3	,	5	,	23	,	23	,	97	,	27	,	379	,	183	,	933	,	3431	,	797	,	9655	,	32061	};
            const unsigned long	dim1907KuoInit[]		=	{	1	,	1	,	7	,	1	,	29	,	61	,	31	,	75	,	331	,	487	,	229	,	2319	,	1385	,	14697	,	17081	};
            const unsigned long	dim1908KuoInit[]		=	{	1	,	1	,	1	,	5	,	19	,	1	,	43	,	27	,	433	,	751	,	1283	,	559	,	7941	,	7385	,	13809	};
            const unsigned long	dim1909KuoInit[]		=	{	1	,	1	,	3	,	15	,	19	,	37	,	97	,	219	,	217	,	39	,	1969	,	1909	,	2895	,	749	,	27607	};
            const unsigned long	dim1910KuoInit[]		=	{	1	,	3	,	5	,	9	,	25	,	17	,	79	,	213	,	321	,	73	,	533	,	3773	,	6989	,	10183	,	20179	};
            const unsigned long	dim1911KuoInit[]		=	{	1	,	1	,	5	,	7	,	15	,	51	,	71	,	161	,	107	,	531	,	461	,	15	,	2887	,	13271	,	24273	};
            const unsigned long	dim1912KuoInit[]		=	{	1	,	3	,	5	,	7	,	29	,	21	,	119	,	53	,	207	,	471	,	987	,	1771	,	7259	,	4377	,	15405	};
            const unsigned long	dim1913KuoInit[]		=	{	1	,	1	,	3	,	11	,	3	,	5	,	7	,	189	,	231	,	181	,	751	,	1	,	225	,	12575	,	4853	};
            const unsigned long	dim1914KuoInit[]		=	{	1	,	1	,	1	,	13	,	1	,	51	,	35	,	239	,	145	,	827	,	1169	,	3671	,	7585	,	4593	,	4433	};
            const unsigned long	dim1915KuoInit[]		=	{	1	,	1	,	3	,	13	,	15	,	55	,	43	,	157	,	13	,	517	,	1669	,	25	,	1175	,	3971	,	11165	};
            const unsigned long	dim1916KuoInit[]		=	{	1	,	3	,	7	,	15	,	15	,	37	,	33	,	155	,	243	,	375	,	947	,	1869	,	2693	,	5191	,	9865	};
            const unsigned long	dim1917KuoInit[]		=	{	1	,	3	,	7	,	1	,	15	,	57	,	73	,	15	,	389	,	341	,	207	,	3249	,	439	,	5975	,	23461	};
            const unsigned long	dim1918KuoInit[]		=	{	1	,	3	,	3	,	11	,	17	,	3	,	35	,	245	,	349	,	915	,	1221	,	1495	,	5525	,	10223	,	29109	};
            const unsigned long	dim1919KuoInit[]		=	{	1	,	1	,	7	,	9	,	19	,	17	,	39	,	49	,	159	,	163	,	419	,	1117	,	7947	,	7177	,	10945	};
            const unsigned long	dim1920KuoInit[]		=	{	1	,	3	,	7	,	5	,	23	,	35	,	31	,	33	,	325	,	565	,	1951	,	745	,	6867	,	13121	,	25831	};
            const unsigned long	dim1921KuoInit[]		=	{	1	,	3	,	7	,	7	,	27	,	11	,	89	,	191	,	233	,	57	,	1621	,	2167	,	7155	,	15613	,	14111	};
            const unsigned long	dim1922KuoInit[]		=	{	1	,	1	,	7	,	15	,	31	,	49	,	3	,	245	,	51	,	173	,	1809	,	379	,	8085	,	5125	,	10279	};
            const unsigned long	dim1923KuoInit[]		=	{	1	,	1	,	7	,	7	,	29	,	59	,	37	,	197	,	377	,	5	,	1851	,	3685	,	3537	,	1897	,	24257	};
            const unsigned long	dim1924KuoInit[]		=	{	1	,	1	,	1	,	11	,	13	,	9	,	11	,	19	,	377	,	435	,	869	,	497	,	8181	,	3411	,	4415	};
            const unsigned long	dim1925KuoInit[]		=	{	1	,	1	,	5	,	7	,	7	,	21	,	51	,	115	,	435	,	9	,	1099	,	987	,	1633	,	7689	,	19131	};
            const unsigned long	dim1926KuoInit[]		=	{	1	,	3	,	3	,	5	,	17	,	59	,	93	,	131	,	17	,	637	,	211	,	2759	,	3059	,	4691	,	10225	};
            const unsigned long	dim1927KuoInit[]		=	{	1	,	3	,	3	,	13	,	5	,	35	,	55	,	49	,	425	,	395	,	615	,	1505	,	1477	,	6689	,	28435	};
            const unsigned long	dim1928KuoInit[]		=	{	1	,	1	,	3	,	5	,	27	,	1	,	43	,	53	,	375	,	657	,	1307	,	1869	,	415	,	1143	,	16961	};
            const unsigned long	dim1929KuoInit[]		=	{	1	,	1	,	7	,	5	,	13	,	27	,	111	,	113	,	459	,	149	,	139	,	79	,	5031	,	15165	,	443	};
            const unsigned long	dim1930KuoInit[]		=	{	1	,	3	,	1	,	11	,	9	,	45	,	35	,	217	,	259	,	523	,	601	,	499	,	7683	,	1447	,	5267	};
            const unsigned long	dim1931KuoInit[]		=	{	1	,	3	,	1	,	7	,	3	,	51	,	101	,	191	,	469	,	435	,	883	,	343	,	1897	,	5245	,	1037	};
            const unsigned long	dim1932KuoInit[]		=	{	1	,	3	,	5	,	1	,	19	,	21	,	109	,	105	,	363	,	409	,	1651	,	3739	,	339	,	12915	,	31603	};
            const unsigned long	dim1933KuoInit[]		=	{	1	,	3	,	5	,	13	,	9	,	7	,	65	,	193	,	447	,	829	,	1123	,	2431	,	3357	,	6687	,	26903	};
            const unsigned long	dim1934KuoInit[]		=	{	1	,	3	,	5	,	5	,	11	,	9	,	69	,	147	,	331	,	657	,	1589	,	3417	,	5805	,	8673	,	2517	};
            const unsigned long	dim1935KuoInit[]		=	{	1	,	3	,	1	,	5	,	23	,	45	,	59	,	53	,	285	,	711	,	1447	,	927	,	8005	,	3513	,	18147	};
            const unsigned long	dim1936KuoInit[]		=	{	1	,	3	,	5	,	9	,	9	,	15	,	17	,	85	,	165	,	923	,	915	,	3263	,	1103	,	9445	,	15371	};
            const unsigned long	dim1937KuoInit[]		=	{	1	,	1	,	5	,	13	,	13	,	13	,	33	,	249	,	501	,	495	,	1771	,	1305	,	2821	,	9405	,	7193	};
            const unsigned long	dim1938KuoInit[]		=	{	1	,	1	,	3	,	5	,	9	,	11	,	23	,	5	,	359	,	977	,	613	,	2163	,	4511	,	7901	,	1609	};
            const unsigned long	dim1939KuoInit[]		=	{	1	,	1	,	3	,	5	,	29	,	29	,	73	,	69	,	155	,	863	,	243	,	2705	,	4005	,	11249	,	13983	};
            const unsigned long	dim1940KuoInit[]		=	{	1	,	3	,	7	,	9	,	9	,	15	,	75	,	171	,	213	,	931	,	1613	,	3801	,	4617	,	3929	,	24787	};
            const unsigned long	dim1941KuoInit[]		=	{	1	,	3	,	5	,	11	,	19	,	35	,	107	,	251	,	121	,	455	,	2027	,	2323	,	4297	,	7949	,	12043	};
            const unsigned long	dim1942KuoInit[]		=	{	1	,	3	,	3	,	7	,	3	,	59	,	13	,	151	,	161	,	615	,	1427	,	2469	,	3211	,	6601	,	26829	};
            const unsigned long	dim1943KuoInit[]		=	{	1	,	1	,	1	,	9	,	31	,	1	,	41	,	247	,	225	,	65	,	1751	,	3557	,	935	,	16115	,	3009	};
            const unsigned long	dim1944KuoInit[]		=	{	1	,	1	,	7	,	1	,	31	,	3	,	31	,	39	,	279	,	213	,	1141	,	371	,	7529	,	12485	,	6585	};
            const unsigned long	dim1945KuoInit[]		=	{	1	,	3	,	3	,	3	,	13	,	31	,	121	,	185	,	171	,	991	,	147	,	1913	,	475	,	15155	,	2427	};
            const unsigned long	dim1946KuoInit[]		=	{	1	,	3	,	5	,	5	,	7	,	13	,	59	,	127	,	175	,	717	,	759	,	3949	,	4229	,	2091	,	20267	};
            const unsigned long	dim1947KuoInit[]		=	{	1	,	3	,	1	,	15	,	21	,	33	,	45	,	57	,	1	,	463	,	1217	,	1499	,	269	,	11159	,	17175	};
            const unsigned long	dim1948KuoInit[]		=	{	1	,	1	,	1	,	9	,	21	,	29	,	51	,	63	,	299	,	775	,	239	,	575	,	1033	,	3627	,	12017	};
            const unsigned long	dim1949KuoInit[]		=	{	1	,	3	,	3	,	3	,	23	,	35	,	63	,	197	,	341	,	775	,	675	,	1835	,	4155	,	7439	,	1797	};
            const unsigned long	dim1950KuoInit[]		=	{	1	,	3	,	7	,	5	,	23	,	57	,	71	,	181	,	1	,	95	,	337	,	1971	,	5721	,	15013	,	3387	};
            const unsigned long	dim1951KuoInit[]		=	{	1	,	3	,	1	,	5	,	31	,	21	,	33	,	203	,	463	,	3	,	1261	,	555	,	3847	,	5653	,	18155	};
            const unsigned long	dim1952KuoInit[]		=	{	1	,	1	,	3	,	9	,	27	,	57	,	57	,	119	,	213	,	723	,	1695	,	2233	,	1605	,	8169	,	28485	};
            const unsigned long	dim1953KuoInit[]		=	{	1	,	1	,	7	,	11	,	17	,	35	,	119	,	167	,	391	,	111	,	229	,	695	,	4209	,	5457	,	6729	};
            const unsigned long	dim1954KuoInit[]		=	{	1	,	1	,	7	,	11	,	5	,	23	,	5	,	223	,	89	,	447	,	2015	,	3403	,	1335	,	15235	,	20897	};
            const unsigned long	dim1955KuoInit[]		=	{	1	,	1	,	1	,	7	,	1	,	63	,	55	,	233	,	351	,	323	,	497	,	1247	,	2455	,	2337	,	17523	};
            const unsigned long	dim1956KuoInit[]		=	{	1	,	3	,	1	,	11	,	7	,	21	,	127	,	207	,	197	,	479	,	611	,	3525	,	2093	,	2591	,	21085	};
            const unsigned long	dim1957KuoInit[]		=	{	1	,	3	,	5	,	5	,	1	,	37	,	107	,	199	,	29	,	995	,	1857	,	133	,	1423	,	9967	,	11367	};
            const unsigned long	dim1958KuoInit[]		=	{	1	,	1	,	7	,	11	,	25	,	25	,	17	,	155	,	221	,	539	,	1895	,	1909	,	2921	,	13107	,	21681	};
            const unsigned long	dim1959KuoInit[]		=	{	1	,	1	,	1	,	13	,	25	,	59	,	45	,	41	,	401	,	657	,	2043	,	1317	,	8065	,	11897	,	4767	};
            const unsigned long	dim1960KuoInit[]		=	{	1	,	1	,	7	,	15	,	21	,	29	,	43	,	125	,	407	,	851	,	1421	,	1101	,	811	,	9577	,	27	};
            const unsigned long	dim1961KuoInit[]		=	{	1	,	3	,	5	,	7	,	25	,	49	,	65	,	175	,	319	,	807	,	1691	,	357	,	4015	,	1059	,	599	};
            const unsigned long	dim1962KuoInit[]		=	{	1	,	1	,	7	,	13	,	23	,	5	,	65	,	79	,	43	,	665	,	1931	,	2143	,	4577	,	5905	,	14917	};
            const unsigned long	dim1963KuoInit[]		=	{	1	,	3	,	1	,	7	,	5	,	41	,	121	,	251	,	395	,	951	,	795	,	1589	,	4027	,	13671	,	18787	};
            const unsigned long	dim1964KuoInit[]		=	{	1	,	3	,	7	,	11	,	3	,	51	,	57	,	177	,	63	,	33	,	1085	,	1357	,	5039	,	2583	,	2805	};
            const unsigned long	dim1965KuoInit[]		=	{	1	,	1	,	1	,	1	,	29	,	53	,	117	,	225	,	373	,	1	,	901	,	3871	,	2815	,	9871	,	7737	};
            const unsigned long	dim1966KuoInit[]		=	{	1	,	3	,	7	,	7	,	31	,	61	,	89	,	253	,	291	,	829	,	9	,	1131	,	4951	,	14509	,	23491	};
            const unsigned long	dim1967KuoInit[]		=	{	1	,	1	,	7	,	5	,	25	,	43	,	35	,	63	,	425	,	757	,	217	,	821	,	1453	,	8413	,	7745	};
            const unsigned long	dim1968KuoInit[]		=	{	1	,	1	,	5	,	5	,	7	,	1	,	41	,	233	,	235	,	599	,	833	,	3637	,	7005	,	789	,	27709	};
            const unsigned long	dim1969KuoInit[]		=	{	1	,	1	,	3	,	13	,	7	,	57	,	69	,	115	,	417	,	99	,	1435	,	3119	,	1655	,	3515	,	7273	};
            const unsigned long	dim1970KuoInit[]		=	{	1	,	1	,	3	,	15	,	9	,	13	,	33	,	57	,	179	,	847	,	289	,	2213	,	4635	,	2349	,	20203	};
            const unsigned long	dim1971KuoInit[]		=	{	1	,	1	,	3	,	9	,	1	,	39	,	63	,	147	,	277	,	389	,	575	,	423	,	4943	,	4327	,	29067	};
            const unsigned long	dim1972KuoInit[]		=	{	1	,	1	,	3	,	13	,	9	,	9	,	69	,	167	,	119	,	591	,	1679	,	3543	,	4517	,	9263	,	17957	};
            const unsigned long	dim1973KuoInit[]		=	{	1	,	3	,	5	,	13	,	25	,	37	,	127	,	159	,	391	,	29	,	329	,	3477	,	6513	,	11971	,	18791	};
            const unsigned long	dim1974KuoInit[]		=	{	1	,	1	,	7	,	5	,	25	,	29	,	49	,	135	,	387	,	921	,	391	,	665	,	6315	,	16183	,	553	};
            const unsigned long	dim1975KuoInit[]		=	{	1	,	1	,	7	,	3	,	27	,	43	,	43	,	253	,	9	,	285	,	1073	,	2627	,	7417	,	3223	,	2649	};
            const unsigned long	dim1976KuoInit[]		=	{	1	,	1	,	5	,	3	,	13	,	57	,	119	,	37	,	475	,	471	,	1145	,	2709	,	2115	,	6507	,	12393	};
            const unsigned long	dim1977KuoInit[]		=	{	1	,	1	,	3	,	1	,	17	,	29	,	117	,	189	,	319	,	533	,	1299	,	1987	,	4971	,	15719	,	19395	};
            const unsigned long	dim1978KuoInit[]		=	{	1	,	1	,	1	,	15	,	11	,	63	,	77	,	245	,	57	,	9	,	1285	,	2069	,	579	,	5105	,	13797	};
            const unsigned long	dim1979KuoInit[]		=	{	1	,	3	,	1	,	11	,	19	,	49	,	1	,	241	,	89	,	519	,	591	,	3875	,	1585	,	7239	,	25867	};
            const unsigned long	dim1980KuoInit[]		=	{	1	,	1	,	5	,	11	,	27	,	47	,	1	,	143	,	209	,	695	,	539	,	3781	,	6317	,	14075	,	27849	};
            const unsigned long	dim1981KuoInit[]		=	{	1	,	1	,	3	,	5	,	17	,	3	,	17	,	201	,	105	,	771	,	497	,	735	,	3517	,	5207	,	20897	};
            const unsigned long	dim1982KuoInit[]		=	{	1	,	1	,	3	,	15	,	3	,	3	,	103	,	35	,	421	,	235	,	1527	,	3139	,	2693	,	9739	,	63	};
            const unsigned long	dim1983KuoInit[]		=	{	1	,	3	,	1	,	13	,	21	,	39	,	101	,	213	,	205	,	853	,	1455	,	39	,	7917	,	7893	,	4627	};
            const unsigned long	dim1984KuoInit[]		=	{	1	,	3	,	7	,	5	,	13	,	5	,	9	,	239	,	129	,	355	,	809	,	3413	,	7535	,	7063	,	29149	};
            const unsigned long	dim1985KuoInit[]		=	{	1	,	1	,	5	,	3	,	19	,	37	,	125	,	55	,	495	,	597	,	735	,	1729	,	619	,	12881	,	22913	};
            const unsigned long	dim1986KuoInit[]		=	{	1	,	1	,	5	,	11	,	27	,	31	,	87	,	65	,	413	,	421	,	575	,	2185	,	4217	,	12223	,	22487	};
            const unsigned long	dim1987KuoInit[]		=	{	1	,	3	,	3	,	7	,	23	,	51	,	115	,	221	,	369	,	27	,	229	,	613	,	3739	,	1425	,	10927	};
            const unsigned long	dim1988KuoInit[]		=	{	1	,	1	,	1	,	13	,	23	,	49	,	73	,	223	,	139	,	511	,	899	,	483	,	7747	,	7947	,	32357	};
            const unsigned long	dim1989KuoInit[]		=	{	1	,	3	,	5	,	3	,	7	,	31	,	9	,	119	,	417	,	723	,	2033	,	663	,	5927	,	1787	,	12841	};
            const unsigned long	dim1990KuoInit[]		=	{	1	,	1	,	5	,	9	,	7	,	5	,	55	,	201	,	245	,	221	,	1447	,	405	,	447	,	2447	,	24331	};
            const unsigned long	dim1991KuoInit[]		=	{	1	,	3	,	5	,	11	,	9	,	47	,	33	,	175	,	187	,	513	,	1613	,	2413	,	4207	,	6941	,	2341	};
            const unsigned long	dim1992KuoInit[]		=	{	1	,	1	,	5	,	9	,	19	,	33	,	33	,	181	,	505	,	923	,	1923	,	2471	,	6499	,	11243	,	29653	};
            const unsigned long	dim1993KuoInit[]		=	{	1	,	1	,	5	,	15	,	1	,	49	,	21	,	15	,	443	,	565	,	1955	,	3299	,	139	,	14389	,	28011	};
            const unsigned long	dim1994KuoInit[]		=	{	1	,	1	,	3	,	9	,	17	,	27	,	29	,	65	,	509	,	867	,	707	,	2425	,	7279	,	15271	,	18239	};
            const unsigned long	dim1995KuoInit[]		=	{	1	,	1	,	7	,	15	,	3	,	1	,	103	,	79	,	49	,	909	,	529	,	1207	,	231	,	11181	,	23319	};
            const unsigned long	dim1996KuoInit[]		=	{	1	,	1	,	5	,	1	,	13	,	23	,	33	,	29	,	189	,	491	,	129	,	501	,	2635	,	2071	,	18057	};
            const unsigned long	dim1997KuoInit[]		=	{	1	,	1	,	7	,	7	,	19	,	1	,	95	,	185	,	67	,	251	,	1017	,	105	,	3683	,	9797	,	24413	};
            const unsigned long	dim1998KuoInit[]		=	{	1	,	3	,	5	,	9	,	13	,	35	,	79	,	213	,	223	,	1013	,	1205	,	1777	,	1415	,	5023	,	15901	};
            const unsigned long	dim1999KuoInit[]		=	{	1	,	1	,	1	,	5	,	15	,	13	,	91	,	57	,	447	,	897	,	935	,	1543	,	5461	,	5091	,	12697	};
            const unsigned long	dim2000KuoInit[]		=	{	1	,	1	,	1	,	11	,	9	,	61	,	81	,	147	,	55	,	121	,	449	,	2833	,	2383	,	12065	,	19435	};
            const unsigned long	dim2001KuoInit[]		=	{	1	,	3	,	1	,	3	,	17	,	27	,	77	,	203	,	415	,	489	,	665	,	21	,	6453	,	5069	,	29629	};
            const unsigned long	dim2002KuoInit[]		=	{	1	,	1	,	1	,	9	,	7	,	3	,	33	,	209	,	85	,	851	,	367	,	783	,	309	,	14821	,	1217	};
            const unsigned long	dim2003KuoInit[]		=	{	1	,	1	,	1	,	15	,	5	,	39	,	15	,	225	,	241	,	477	,	999	,	841	,	4637	,	5383	,	18081	};
            const unsigned long	dim2004KuoInit[]		=	{	1	,	1	,	5	,	5	,	19	,	49	,	101	,	115	,	87	,	781	,	815	,	2405	,	2579	,	15043	,	31067	};
            const unsigned long	dim2005KuoInit[]		=	{	1	,	1	,	3	,	5	,	3	,	57	,	123	,	191	,	305	,	999	,	1091	,	733	,	3219	,	891	,	30495	};
            const unsigned long	dim2006KuoInit[]		=	{	1	,	1	,	5	,	3	,	27	,	17	,	95	,	121	,	39	,	41	,	171	,	97	,	7363	,	11279	,	28039	};
            const unsigned long	dim2007KuoInit[]		=	{	1	,	3	,	7	,	5	,	21	,	9	,	21	,	147	,	269	,	475	,	1825	,	1459	,	6337	,	3259	,	27731	};
            const unsigned long	dim2008KuoInit[]		=	{	1	,	1	,	3	,	5	,	9	,	17	,	107	,	171	,	253	,	71	,	629	,	1671	,	2587	,	11429	,	8985	};
            const unsigned long	dim2009KuoInit[]		=	{	1	,	3	,	3	,	9	,	25	,	1	,	63	,	15	,	173	,	79	,	1465	,	2647	,	6885	,	13043	,	24703	};
            const unsigned long	dim2010KuoInit[]		=	{	1	,	3	,	1	,	9	,	1	,	13	,	3	,	217	,	49	,	77	,	1635	,	3767	,	173	,	12697	,	11837	};
            const unsigned long	dim2011KuoInit[]		=	{	1	,	3	,	5	,	13	,	31	,	7	,	11	,	181	,	509	,	601	,	1267	,	1857	,	7871	,	1703	,	6091	};
            const unsigned long	dim2012KuoInit[]		=	{	1	,	1	,	1	,	11	,	29	,	61	,	117	,	151	,	487	,	185	,	1899	,	575	,	7341	,	3055	,	26513	};
            const unsigned long	dim2013KuoInit[]		=	{	1	,	3	,	7	,	15	,	1	,	49	,	87	,	23	,	483	,	603	,	1197	,	2589	,	1705	,	5759	,	19791	};
            const unsigned long	dim2014KuoInit[]		=	{	1	,	3	,	3	,	11	,	1	,	37	,	71	,	115	,	59	,	167	,	1907	,	2225	,	5673	,	14571	,	19389	};
            const unsigned long	dim2015KuoInit[]		=	{	1	,	1	,	3	,	7	,	15	,	57	,	33	,	143	,	45	,	321	,	779	,	543	,	3465	,	4769	,	19753	};
            const unsigned long	dim2016KuoInit[]		=	{	1	,	3	,	5	,	5	,	31	,	21	,	21	,	141	,	197	,	397	,	1041	,	317	,	109	,	9395	,	7141	};
            const unsigned long	dim2017KuoInit[]		=	{	1	,	3	,	7	,	11	,	1	,	61	,	45	,	215	,	405	,	365	,	1801	,	2767	,	6381	,	12975	,	41	};
            const unsigned long	dim2018KuoInit[]		=	{	1	,	3	,	7	,	7	,	23	,	13	,	37	,	193	,	305	,	883	,	1427	,	3859	,	2049	,	16065	,	2471	};
            const unsigned long	dim2019KuoInit[]		=	{	1	,	3	,	3	,	3	,	31	,	3	,	9	,	169	,	433	,	187	,	545	,	985	,	7699	,	1183	,	27773	};
            const unsigned long	dim2020KuoInit[]		=	{	1	,	3	,	5	,	15	,	11	,	15	,	9	,	171	,	89	,	711	,	163	,	3637	,	5725	,	5079	,	13373	};
            const unsigned long	dim2021KuoInit[]		=	{	1	,	1	,	7	,	1	,	1	,	29	,	123	,	235	,	229	,	667	,	1155	,	3387	,	129	,	13889	,	24547	};
            const unsigned long	dim2022KuoInit[]		=	{	1	,	3	,	3	,	7	,	27	,	15	,	75	,	117	,	335	,	821	,	609	,	3843	,	79	,	6787	,	5605	};
            const unsigned long	dim2023KuoInit[]		=	{	1	,	1	,	3	,	15	,	27	,	47	,	13	,	163	,	205	,	773	,	541	,	1155	,	4341	,	14155	,	6643	};
            const unsigned long	dim2024KuoInit[]		=	{	1	,	3	,	3	,	11	,	31	,	1	,	67	,	35	,	443	,	723	,	1089	,	2605	,	4193	,	6133	,	31127	};
            const unsigned long	dim2025KuoInit[]		=	{	1	,	1	,	1	,	15	,	27	,	57	,	11	,	153	,	199	,	219	,	1987	,	2517	,	1455	,	6051	,	9615	};
            const unsigned long	dim2026KuoInit[]		=	{	1	,	3	,	5	,	9	,	5	,	7	,	79	,	187	,	93	,	679	,	1607	,	799	,	2753	,	6513	,	11371	};
            const unsigned long	dim2027KuoInit[]		=	{	1	,	3	,	1	,	15	,	31	,	55	,	5	,	175	,	261	,	545	,	995	,	2225	,	2217	,	2037	,	14785	};
            const unsigned long	dim2028KuoInit[]		=	{	1	,	3	,	5	,	9	,	29	,	5	,	101	,	223	,	241	,	829	,	1625	,	3417	,	1335	,	10705	,	531	};
            const unsigned long	dim2029KuoInit[]		=	{	1	,	3	,	3	,	1	,	19	,	15	,	5	,	203	,	97	,	165	,	99	,	2371	,	3267	,	13169	,	22229	};
            const unsigned long	dim2030KuoInit[]		=	{	1	,	1	,	1	,	7	,	11	,	63	,	113	,	255	,	145	,	589	,	1149	,	683	,	2619	,	5569	,	5933	};
            const unsigned long	dim2031KuoInit[]		=	{	1	,	1	,	5	,	9	,	23	,	41	,	21	,	217	,	503	,	975	,	329	,	3433	,	6299	,	5337	,	199	};
            const unsigned long	dim2032KuoInit[]		=	{	1	,	1	,	7	,	13	,	3	,	39	,	109	,	223	,	241	,	521	,	1071	,	589	,	1199	,	13059	,	24921	};
            const unsigned long	dim2033KuoInit[]		=	{	1	,	1	,	3	,	7	,	11	,	23	,	27	,	15	,	107	,	679	,	87	,	3259	,	4369	,	7365	,	16293	};
            const unsigned long	dim2034KuoInit[]		=	{	1	,	3	,	3	,	11	,	11	,	5	,	75	,	111	,	21	,	587	,	1707	,	1915	,	6617	,	11143	,	24389	};
            const unsigned long	dim2035KuoInit[]		=	{	1	,	1	,	5	,	9	,	5	,	59	,	77	,	3	,	43	,	583	,	1601	,	2183	,	6209	,	5409	,	18175	};
            const unsigned long	dim2036KuoInit[]		=	{	1	,	1	,	1	,	1	,	15	,	1	,	55	,	95	,	437	,	731	,	1559	,	2421	,	7441	,	6567	,	1549	};
            const unsigned long	dim2037KuoInit[]		=	{	1	,	3	,	3	,	11	,	17	,	21	,	13	,	61	,	153	,	81	,	141	,	3095	,	5485	,	14461	,	8495	};
            const unsigned long	dim2038KuoInit[]		=	{	1	,	1	,	1	,	13	,	25	,	61	,	87	,	195	,	243	,	47	,	103	,	3451	,	5583	,	1611	,	11939	};
            const unsigned long	dim2039KuoInit[]		=	{	1	,	1	,	5	,	7	,	31	,	53	,	93	,	101	,	145	,	917	,	1669	,	867	,	6949	,	16333	,	12577	};
            const unsigned long	dim2040KuoInit[]		=	{	1	,	1	,	5	,	1	,	5	,	41	,	35	,	75	,	331	,	671	,	1623	,	1573	,	4275	,	9031	,	9047	};
            const unsigned long	dim2041KuoInit[]		=	{	1	,	3	,	5	,	13	,	15	,	39	,	29	,	183	,	477	,	787	,	1333	,	2499	,	2189	,	8387	,	2783	};
            const unsigned long	dim2042KuoInit[]		=	{	1	,	1	,	3	,	7	,	19	,	33	,	117	,	201	,	177	,	595	,	69	,	3341	,	1841	,	11091	,	16745	};
            const unsigned long	dim2043KuoInit[]		=	{	1	,	3	,	7	,	11	,	29	,	59	,	69	,	119	,	433	,	595	,	221	,	1245	,	7805	,	1761	,	27657	};
            const unsigned long	dim2044KuoInit[]		=	{	1	,	3	,	5	,	3	,	23	,	19	,	75	,	189	,	279	,	867	,	113	,	3817	,	6093	,	12237	,	20297	};
            const unsigned long	dim2045KuoInit[]		=	{	1	,	1	,	1	,	15	,	21	,	39	,	15	,	249	,	9	,	997	,	51	,	1317	,	1083	,	13683	,	5759	};
            const unsigned long	dim2046KuoInit[]		=	{	1	,	1	,	5	,	15	,	29	,	29	,	25	,	93	,	465	,	549	,	283	,	1673	,	1745	,	3913	,	7931	};
            const unsigned long	dim2047KuoInit[]		=	{	1	,	3	,	5	,	1	,	5	,	23	,	123	,	217	,	211	,	215	,	1999	,	1229	,	6969	,	13481	,	14795	};
            const unsigned long	dim2048KuoInit[]		=	{	1	,	1	,	3	,	5	,	17	,	35	,	107	,	163	,	103	,	807	,	463	,	2999	,	4889	,	12863	,	24619	};
            const unsigned long	dim2049KuoInit[]		=	{	1	,	3	,	1	,	15	,	5	,	17	,	121	,	209	,	49	,	871	,	1931	,	345	,	6887	,	1357	,	14693	};
            const unsigned long	dim2050KuoInit[]		=	{	1	,	1	,	1	,	1	,	23	,	63	,	53	,	35	,	511	,	261	,	11	,	283	,	3283	,	1963	,	11703	};
            const unsigned long	dim2051KuoInit[]		=	{	1	,	3	,	3	,	13	,	15	,	41	,	19	,	39	,	445	,	21	,	513	,	2957	,	2307	,	11451	,	6539	};
            const unsigned long	dim2052KuoInit[]		=	{	1	,	1	,	3	,	11	,	19	,	45	,	7	,	155	,	387	,	287	,	347	,	1557	,	7407	,	10885	,	31829	};
            const unsigned long	dim2053KuoInit[]		=	{	1	,	3	,	5	,	13	,	9	,	53	,	61	,	99	,	355	,	449	,	1353	,	3135	,	2329	,	733	,	22443	};
            const unsigned long	dim2054KuoInit[]		=	{	1	,	1	,	3	,	5	,	27	,	47	,	43	,	217	,	113	,	385	,	903	,	3015	,	4169	,	10705	,	11351	};
            const unsigned long	dim2055KuoInit[]		=	{	1	,	3	,	7	,	11	,	9	,	33	,	7	,	167	,	473	,	465	,	357	,	2387	,	2901	,	12995	,	16871	};
            const unsigned long	dim2056KuoInit[]		=	{	1	,	3	,	7	,	3	,	29	,	35	,	85	,	201	,	311	,	581	,	491	,	2977	,	7433	,	14615	,	29995	};
            const unsigned long	dim2057KuoInit[]		=	{	1	,	3	,	1	,	15	,	5	,	5	,	121	,	17	,	93	,	847	,	835	,	3885	,	2437	,	2237	,	15669	};
            const unsigned long	dim2058KuoInit[]		=	{	1	,	3	,	7	,	5	,	29	,	39	,	57	,	241	,	247	,	847	,	311	,	3115	,	7605	,	3117	,	30677	};
            const unsigned long	dim2059KuoInit[]		=	{	1	,	3	,	1	,	13	,	21	,	27	,	123	,	77	,	511	,	319	,	1699	,	709	,	7021	,	14377	,	9601	};
            const unsigned long	dim2060KuoInit[]		=	{	1	,	3	,	3	,	5	,	31	,	1	,	49	,	249	,	271	,	595	,	1069	,	2307	,	567	,	13145	,	8779	};
            const unsigned long	dim2061KuoInit[]		=	{	1	,	3	,	3	,	3	,	29	,	25	,	55	,	63	,	289	,	617	,	1209	,	3949	,	481	,	13495	,	16659	};
            const unsigned long	dim2062KuoInit[]		=	{	1	,	3	,	3	,	3	,	5	,	19	,	47	,	125	,	323	,	917	,	1875	,	1683	,	7451	,	16145	,	3029	};
            const unsigned long	dim2063KuoInit[]		=	{	1	,	1	,	7	,	5	,	27	,	33	,	3	,	133	,	477	,	457	,	803	,	1141	,	3965	,	9127	,	21173	};
            const unsigned long	dim2064KuoInit[]		=	{	1	,	3	,	5	,	7	,	19	,	61	,	17	,	27	,	297	,	995	,	1119	,	2555	,	6741	,	3233	,	13735	};
            const unsigned long	dim2065KuoInit[]		=	{	1	,	3	,	7	,	11	,	11	,	21	,	21	,	143	,	319	,	143	,	675	,	187	,	6909	,	7145	,	27029	};
            const unsigned long	dim2066KuoInit[]		=	{	1	,	3	,	3	,	1	,	29	,	5	,	1	,	65	,	123	,	801	,	933	,	3105	,	1163	,	15703	,	9567	};
            const unsigned long	dim2067KuoInit[]		=	{	1	,	1	,	3	,	11	,	3	,	15	,	11	,	109	,	117	,	1011	,	1407	,	3287	,	5799	,	147	,	32641	};
            const unsigned long	dim2068KuoInit[]		=	{	1	,	1	,	1	,	5	,	29	,	25	,	63	,	39	,	139	,	379	,	1887	,	417	,	2965	,	4409	,	19149	};
            const unsigned long	dim2069KuoInit[]		=	{	1	,	1	,	1	,	13	,	31	,	25	,	123	,	187	,	311	,	397	,	1467	,	15	,	4253	,	14565	,	32609	};
            const unsigned long	dim2070KuoInit[]		=	{	1	,	3	,	5	,	7	,	25	,	5	,	13	,	25	,	489	,	837	,	865	,	527	,	4965	,	2567	,	13505	};
            const unsigned long	dim2071KuoInit[]		=	{	1	,	1	,	3	,	9	,	3	,	15	,	59	,	115	,	339	,	459	,	2011	,	3311	,	4067	,	3437	,	21435	};
            const unsigned long	dim2072KuoInit[]		=	{	1	,	1	,	5	,	3	,	17	,	35	,	49	,	11	,	217	,	401	,	385	,	569	,	1727	,	9927	,	27229	};
            const unsigned long	dim2073KuoInit[]		=	{	1	,	3	,	1	,	5	,	27	,	43	,	21	,	11	,	459	,	555	,	237	,	87	,	7531	,	16117	,	23625	};
            const unsigned long	dim2074KuoInit[]		=	{	1	,	3	,	3	,	5	,	15	,	57	,	93	,	35	,	31	,	257	,	77	,	3437	,	3665	,	3137	,	2477	};
            const unsigned long	dim2075KuoInit[]		=	{	1	,	3	,	5	,	9	,	1	,	43	,	91	,	55	,	53	,	265	,	1495	,	551	,	4729	,	7839	,	23475	};
            const unsigned long	dim2076KuoInit[]		=	{	1	,	1	,	7	,	11	,	11	,	45	,	9	,	27	,	485	,	397	,	925	,	3601	,	4419	,	4563	,	6443	};
            const unsigned long	dim2077KuoInit[]		=	{	1	,	3	,	5	,	3	,	15	,	35	,	103	,	239	,	193	,	831	,	559	,	1299	,	4939	,	8051	,	6899	};
            const unsigned long	dim2078KuoInit[]		=	{	1	,	3	,	3	,	11	,	17	,	47	,	77	,	13	,	227	,	935	,	1973	,	1789	,	4357	,	5135	,	27643	};
            const unsigned long	dim2079KuoInit[]		=	{	1	,	1	,	3	,	7	,	11	,	51	,	109	,	143	,	101	,	131	,	565	,	3503	,	5649	,	10007	,	6283	};
            const unsigned long	dim2080KuoInit[]		=	{	1	,	1	,	3	,	15	,	19	,	5	,	15	,	241	,	399	,	341	,	943	,	587	,	4757	,	13941	,	7571	};
            const unsigned long	dim2081KuoInit[]		=	{	1	,	3	,	3	,	3	,	21	,	9	,	119	,	119	,	445	,	349	,	1517	,	17	,	3593	,	11983	,	9713	};
            const unsigned long	dim2082KuoInit[]		=	{	1	,	1	,	3	,	3	,	23	,	55	,	21	,	251	,	191	,	585	,	1619	,	431	,	4161	,	5667	,	17703	};
            const unsigned long	dim2083KuoInit[]		=	{	1	,	3	,	3	,	3	,	1	,	57	,	65	,	231	,	67	,	531	,	1403	,	3883	,	4463	,	12795	,	4427	};
            const unsigned long	dim2084KuoInit[]		=	{	1	,	1	,	5	,	15	,	19	,	7	,	9	,	163	,	487	,	455	,	1663	,	3141	,	5975	,	11001	,	24781	};
            const unsigned long	dim2085KuoInit[]		=	{	1	,	1	,	5	,	1	,	29	,	19	,	45	,	235	,	371	,	935	,	1757	,	187	,	7729	,	6241	,	27171	};
            const unsigned long	dim2086KuoInit[]		=	{	1	,	1	,	1	,	1	,	15	,	39	,	39	,	171	,	145	,	413	,	1545	,	3733	,	1815	,	9443	,	32705	};
            const unsigned long	dim2087KuoInit[]		=	{	1	,	1	,	3	,	9	,	31	,	1	,	87	,	11	,	219	,	913	,	131	,	3625	,	7927	,	9945	,	2595	};
            const unsigned long	dim2088KuoInit[]		=	{	1	,	3	,	3	,	13	,	21	,	15	,	39	,	29	,	267	,	275	,	973	,	701	,	1057	,	13641	,	22149	};
            const unsigned long	dim2089KuoInit[]		=	{	1	,	3	,	3	,	5	,	3	,	33	,	81	,	29	,	85	,	515	,	1719	,	3967	,	5145	,	12921	,	8303	};
            const unsigned long	dim2090KuoInit[]		=	{	1	,	3	,	7	,	11	,	17	,	23	,	113	,	129	,	395	,	153	,	427	,	1113	,	8157	,	9087	,	20471	};
            const unsigned long	dim2091KuoInit[]		=	{	1	,	1	,	5	,	9	,	23	,	5	,	119	,	67	,	215	,	581	,	1249	,	3949	,	731	,	10387	,	27979	};
            const unsigned long	dim2092KuoInit[]		=	{	1	,	1	,	5	,	5	,	5	,	53	,	7	,	39	,	51	,	519	,	225	,	119	,	4335	,	11723	,	27971	};
            const unsigned long	dim2093KuoInit[]		=	{	1	,	3	,	5	,	3	,	5	,	49	,	105	,	29	,	443	,	457	,	191	,	2679	,	5803	,	12017	,	2205	};
            const unsigned long	dim2094KuoInit[]		=	{	1	,	1	,	3	,	1	,	23	,	31	,	11	,	199	,	479	,	231	,	431	,	1989	,	705	,	6557	,	27763	};
            const unsigned long	dim2095KuoInit[]		=	{	1	,	1	,	3	,	1	,	27	,	9	,	39	,	105	,	149	,	1017	,	79	,	1575	,	6365	,	839	,	27601	};
            const unsigned long	dim2096KuoInit[]		=	{	1	,	1	,	3	,	7	,	1	,	29	,	61	,	129	,	253	,	541	,	1953	,	351	,	2895	,	14385	,	23685	};
            const unsigned long	dim2097KuoInit[]		=	{	1	,	3	,	1	,	7	,	19	,	15	,	55	,	133	,	195	,	881	,	311	,	2021	,	7935	,	4513	,	31065	};
            const unsigned long	dim2098KuoInit[]		=	{	1	,	1	,	5	,	13	,	23	,	55	,	53	,	169	,	79	,	707	,	1415	,	3999	,	299	,	6689	,	10219	};
            const unsigned long	dim2099KuoInit[]		=	{	1	,	3	,	3	,	7	,	7	,	47	,	77	,	209	,	467	,	39	,	291	,	3769	,	1295	,	12407	,	2205	};
            const unsigned long	dim2100KuoInit[]		=	{	1	,	1	,	3	,	9	,	13	,	3	,	63	,	241	,	251	,	939	,	1391	,	2053	,	719	,	13739	,	12167	};
            const unsigned long	dim2101KuoInit[]		=	{	1	,	1	,	7	,	9	,	21	,	31	,	37	,	11	,	187	,	201	,	1425	,	249	,	2475	,	12421	,	14443	};
            const unsigned long	dim2102KuoInit[]		=	{	1	,	1	,	3	,	9	,	31	,	51	,	53	,	119	,	19	,	13	,	1513	,	1115	,	1265	,	15707	,	22795	};
            const unsigned long	dim2103KuoInit[]		=	{	1	,	3	,	5	,	9	,	23	,	13	,	69	,	247	,	103	,	773	,	997	,	703	,	2921	,	3165	,	3027	};
            const unsigned long	dim2104KuoInit[]		=	{	1	,	3	,	7	,	7	,	15	,	27	,	81	,	221	,	259	,	405	,	391	,	2635	,	1747	,	7743	,	28833	};
            const unsigned long	dim2105KuoInit[]		=	{	1	,	1	,	3	,	9	,	29	,	47	,	23	,	127	,	49	,	81	,	543	,	919	,	5517	,	9573	,	1523	};
            const unsigned long	dim2106KuoInit[]		=	{	1	,	3	,	3	,	9	,	11	,	37	,	123	,	199	,	343	,	697	,	293	,	3123	,	4601	,	14209	,	30425	};
            const unsigned long	dim2107KuoInit[]		=	{	1	,	1	,	5	,	7	,	3	,	31	,	127	,	13	,	191	,	509	,	1999	,	3379	,	2365	,	10307	,	26693	};
            const unsigned long	dim2108KuoInit[]		=	{	1	,	3	,	7	,	3	,	3	,	41	,	71	,	153	,	119	,	9	,	1601	,	3839	,	369	,	16241	,	19115	};
            const unsigned long	dim2109KuoInit[]		=	{	1	,	1	,	5	,	5	,	3	,	17	,	7	,	65	,	191	,	143	,	127	,	1973	,	6885	,	13615	,	26289	};
            const unsigned long	dim2110KuoInit[]		=	{	1	,	1	,	3	,	5	,	13	,	5	,	87	,	17	,	445	,	975	,	1527	,	253	,	5983	,	7095	,	21271	};
            const unsigned long	dim2111KuoInit[]		=	{	1	,	3	,	1	,	3	,	7	,	49	,	29	,	19	,	37	,	695	,	603	,	3153	,	4065	,	5421	,	3285	};
            const unsigned long	dim2112KuoInit[]		=	{	1	,	1	,	5	,	13	,	25	,	47	,	43	,	35	,	205	,	629	,	1255	,	2843	,	7533	,	751	,	10119	};
            const unsigned long	dim2113KuoInit[]		=	{	1	,	3	,	1	,	11	,	13	,	27	,	127	,	221	,	67	,	523	,	217	,	1493	,	1793	,	10871	,	14289	};
            const unsigned long	dim2114KuoInit[]		=	{	1	,	1	,	1	,	5	,	21	,	47	,	45	,	73	,	479	,	653	,	1553	,	695	,	3131	,	8773	,	20303	};
            const unsigned long	dim2115KuoInit[]		=	{	1	,	3	,	1	,	1	,	19	,	31	,	25	,	207	,	361	,	961	,	1595	,	1807	,	3649	,	1029	,	31579	};
            const unsigned long	dim2116KuoInit[]		=	{	1	,	1	,	1	,	1	,	27	,	23	,	109	,	111	,	477	,	233	,	1305	,	51	,	4517	,	14867	,	4075	};
            const unsigned long	dim2117KuoInit[]		=	{	1	,	1	,	7	,	11	,	21	,	5	,	51	,	9	,	131	,	671	,	191	,	4045	,	7773	,	5037	,	23229	};
            const unsigned long	dim2118KuoInit[]		=	{	1	,	1	,	5	,	7	,	3	,	57	,	75	,	141	,	439	,	269	,	99	,	1595	,	3597	,	10183	,	19849	};
            const unsigned long	dim2119KuoInit[]		=	{	1	,	1	,	3	,	3	,	17	,	61	,	81	,	75	,	471	,	1023	,	1045	,	849	,	3741	,	12803	,	10735	};
            const unsigned long	dim2120KuoInit[]		=	{	1	,	1	,	7	,	11	,	3	,	33	,	79	,	113	,	335	,	245	,	1807	,	1927	,	7381	,	6405	,	20035	};
            const unsigned long	dim2121KuoInit[]		=	{	1	,	3	,	1	,	11	,	5	,	37	,	31	,	177	,	233	,	937	,	1655	,	511	,	1109	,	5083	,	31325	};
            const unsigned long	dim2122KuoInit[]		=	{	1	,	3	,	5	,	15	,	15	,	35	,	125	,	105	,	313	,	553	,	259	,	1433	,	327	,	2079	,	5279	};
            const unsigned long	dim2123KuoInit[]		=	{	1	,	3	,	1	,	1	,	25	,	57	,	121	,	189	,	409	,	459	,	749	,	3159	,	6389	,	4141	,	16463	};
            const unsigned long	dim2124KuoInit[]		=	{	1	,	1	,	7	,	15	,	7	,	21	,	113	,	139	,	241	,	617	,	1385	,	1387	,	145	,	6605	,	2057	};
            const unsigned long	dim2125KuoInit[]		=	{	1	,	3	,	3	,	11	,	17	,	33	,	89	,	17	,	341	,	369	,	719	,	2481	,	7589	,	16283	,	28157	};
            const unsigned long	dim2126KuoInit[]		=	{	1	,	3	,	7	,	1	,	11	,	17	,	31	,	135	,	333	,	335	,	197	,	2401	,	7637	,	703	,	3001	};
            const unsigned long	dim2127KuoInit[]		=	{	1	,	3	,	5	,	11	,	17	,	31	,	33	,	249	,	315	,	475	,	1743	,	2429	,	4923	,	2779	,	25677	};
            const unsigned long	dim2128KuoInit[]		=	{	1	,	1	,	5	,	5	,	11	,	29	,	111	,	103	,	143	,	779	,	1437	,	3759	,	5827	,	3129	,	26961	};
            const unsigned long	dim2129KuoInit[]		=	{	1	,	3	,	5	,	3	,	25	,	1	,	109	,	117	,	159	,	53	,	1291	,	1787	,	7851	,	14363	,	15271	};
            const unsigned long	dim2130KuoInit[]		=	{	1	,	1	,	1	,	9	,	9	,	1	,	31	,	131	,	163	,	723	,	1023	,	27	,	853	,	2623	,	16859	};
            const unsigned long	dim2131KuoInit[]		=	{	1	,	1	,	3	,	11	,	23	,	13	,	91	,	87	,	141	,	111	,	1677	,	1483	,	1461	,	15981	,	29417	};
            const unsigned long	dim2132KuoInit[]		=	{	1	,	3	,	7	,	9	,	17	,	17	,	115	,	43	,	397	,	683	,	2017	,	985	,	3703	,	16135	,	3661	};
            const unsigned long	dim2133KuoInit[]		=	{	1	,	1	,	7	,	13	,	17	,	45	,	71	,	149	,	317	,	559	,	279	,	2441	,	2323	,	16071	,	29709	};
            const unsigned long	dim2134KuoInit[]		=	{	1	,	3	,	5	,	11	,	31	,	33	,	105	,	127	,	375	,	1001	,	115	,	1429	,	7003	,	13369	,	23281	};
            const unsigned long	dim2135KuoInit[]		=	{	1	,	1	,	5	,	7	,	21	,	7	,	27	,	105	,	99	,	479	,	849	,	1361	,	4935	,	14831	,	29	};
            const unsigned long	dim2136KuoInit[]		=	{	1	,	3	,	7	,	9	,	15	,	53	,	63	,	109	,	171	,	323	,	289	,	2937	,	3177	,	4615	,	10711	};
            const unsigned long	dim2137KuoInit[]		=	{	1	,	1	,	1	,	1	,	27	,	41	,	29	,	101	,	209	,	673	,	1597	,	3743	,	827	,	4025	,	27267	};
            const unsigned long	dim2138KuoInit[]		=	{	1	,	3	,	7	,	13	,	25	,	17	,	19	,	219	,	233	,	769	,	909	,	3585	,	55	,	13349	,	13685	};
            const unsigned long	dim2139KuoInit[]		=	{	1	,	1	,	1	,	15	,	17	,	57	,	63	,	181	,	339	,	319	,	655	,	699	,	1633	,	8181	,	23039	};
            const unsigned long	dim2140KuoInit[]		=	{	1	,	1	,	3	,	1	,	25	,	11	,	61	,	195	,	311	,	723	,	365	,	2791	,	1281	,	9113	,	26741	};
            const unsigned long	dim2141KuoInit[]		=	{	1	,	1	,	7	,	11	,	19	,	43	,	65	,	37	,	89	,	847	,	951	,	851	,	4441	,	6681	,	917	};
            const unsigned long	dim2142KuoInit[]		=	{	1	,	3	,	5	,	13	,	1	,	11	,	55	,	29	,	419	,	769	,	815	,	1487	,	2151	,	7981	,	19509	};
            const unsigned long	dim2143KuoInit[]		=	{	1	,	1	,	5	,	13	,	11	,	55	,	127	,	19	,	419	,	679	,	119	,	1417	,	1717	,	5849	,	6365	};
            const unsigned long	dim2144KuoInit[]		=	{	1	,	1	,	7	,	5	,	29	,	5	,	9	,	241	,	405	,	253	,	1015	,	2027	,	7695	,	12003	,	659	};
            const unsigned long	dim2145KuoInit[]		=	{	1	,	3	,	7	,	5	,	27	,	63	,	87	,	221	,	219	,	355	,	941	,	3901	,	6535	,	4715	,	3571	};
            const unsigned long	dim2146KuoInit[]		=	{	1	,	3	,	3	,	9	,	13	,	1	,	113	,	21	,	71	,	353	,	1063	,	1447	,	2157	,	5195	,	21311	};
            const unsigned long	dim2147KuoInit[]		=	{	1	,	1	,	3	,	15	,	29	,	1	,	7	,	223	,	43	,	605	,	1063	,	67	,	2529	,	1125	,	16933	};
            const unsigned long	dim2148KuoInit[]		=	{	1	,	3	,	1	,	15	,	29	,	37	,	51	,	239	,	117	,	351	,	1953	,	1933	,	6371	,	13467	,	8265	};
            const unsigned long	dim2149KuoInit[]		=	{	1	,	1	,	3	,	5	,	7	,	25	,	19	,	221	,	409	,	121	,	89	,	219	,	105	,	4213	,	1903	};
            const unsigned long	dim2150KuoInit[]		=	{	1	,	3	,	7	,	1	,	13	,	3	,	79	,	123	,	423	,	589	,	1085	,	3387	,	6117	,	8749	,	8947	};
            const unsigned long	dim2151KuoInit[]		=	{	1	,	3	,	7	,	9	,	21	,	49	,	1	,	39	,	461	,	115	,	1131	,	719	,	3709	,	1495	,	23871	};
            const unsigned long	dim2152KuoInit[]		=	{	1	,	1	,	7	,	5	,	23	,	41	,	107	,	225	,	499	,	721	,	1977	,	2063	,	1825	,	9589	,	22137	};
            const unsigned long	dim2153KuoInit[]		=	{	1	,	1	,	5	,	7	,	15	,	53	,	33	,	145	,	179	,	109	,	425	,	2729	,	7637	,	6805	,	26223	};
            const unsigned long	dim2154KuoInit[]		=	{	1	,	1	,	5	,	13	,	1	,	1	,	31	,	117	,	25	,	427	,	681	,	2991	,	7681	,	15927	,	20913	};
            const unsigned long	dim2155KuoInit[]		=	{	1	,	1	,	5	,	13	,	31	,	53	,	23	,	71	,	397	,	661	,	131	,	2815	,	2351	,	15881	,	10321	};
            const unsigned long	dim2156KuoInit[]		=	{	1	,	3	,	1	,	15	,	19	,	41	,	39	,	57	,	349	,	761	,	1999	,	1151	,	201	,	1025	,	18821	};
            const unsigned long	dim2157KuoInit[]		=	{	1	,	1	,	3	,	3	,	17	,	43	,	73	,	163	,	187	,	913	,	31	,	81	,	4829	,	6993	,	32175	};
            const unsigned long	dim2158KuoInit[]		=	{	1	,	3	,	1	,	15	,	19	,	41	,	79	,	13	,	343	,	415	,	863	,	1943	,	2231	,	11353	,	15631	};
            const unsigned long	dim2159KuoInit[]		=	{	1	,	3	,	1	,	15	,	29	,	35	,	101	,	23	,	111	,	937	,	833	,	2369	,	3503	,	6605	,	14279	};
            const unsigned long	dim2160KuoInit[]		=	{	1	,	3	,	7	,	15	,	27	,	61	,	43	,	211	,	259	,	583	,	1529	,	1013	,	8005	,	7717	,	1085	};
            const unsigned long	dim2161KuoInit[]		=	{	1	,	1	,	7	,	15	,	23	,	39	,	17	,	201	,	465	,	289	,	1051	,	33	,	7403	,	7971	,	24905	};
            const unsigned long	dim2162KuoInit[]		=	{	1	,	1	,	5	,	13	,	23	,	41	,	5	,	239	,	9	,	23	,	1353	,	2005	,	2513	,	8707	,	3893	};
            const unsigned long	dim2163KuoInit[]		=	{	1	,	3	,	1	,	7	,	7	,	41	,	73	,	177	,	351	,	297	,	1057	,	3867	,	2481	,	15751	,	25231	};
            const unsigned long	dim2164KuoInit[]		=	{	1	,	1	,	3	,	1	,	17	,	59	,	45	,	171	,	367	,	575	,	1085	,	953	,	3467	,	8435	,	6345	};
            const unsigned long	dim2165KuoInit[]		=	{	1	,	3	,	1	,	9	,	5	,	21	,	125	,	137	,	415	,	101	,	121	,	3499	,	6671	,	15541	,	14081	};
            const unsigned long	dim2166KuoInit[]		=	{	1	,	3	,	5	,	15	,	15	,	19	,	111	,	235	,	53	,	119	,	1307	,	583	,	1695	,	211	,	5567	};
            const unsigned long	dim2167KuoInit[]		=	{	1	,	1	,	5	,	9	,	7	,	13	,	119	,	109	,	273	,	677	,	2001	,	3307	,	7999	,	11769	,	10069	};
            const unsigned long	dim2168KuoInit[]		=	{	1	,	1	,	1	,	15	,	11	,	15	,	57	,	233	,	77	,	815	,	31	,	669	,	6365	,	5085	,	13583	};
            const unsigned long	dim2169KuoInit[]		=	{	1	,	3	,	7	,	1	,	31	,	9	,	101	,	33	,	13	,	699	,	637	,	1159	,	5101	,	13805	,	32651	};
            const unsigned long	dim2170KuoInit[]		=	{	1	,	1	,	7	,	13	,	15	,	9	,	125	,	71	,	241	,	147	,	529	,	1863	,	1551	,	4981	,	30389	};
            const unsigned long	dim2171KuoInit[]		=	{	1	,	1	,	7	,	13	,	11	,	35	,	11	,	5	,	473	,	55	,	1589	,	2835	,	3771	,	11433	,	11099	};
            const unsigned long	dim2172KuoInit[]		=	{	1	,	1	,	7	,	15	,	11	,	11	,	35	,	207	,	267	,	289	,	999	,	3183	,	3827	,	6617	,	29015	};
            const unsigned long	dim2173KuoInit[]		=	{	1	,	1	,	3	,	1	,	3	,	11	,	119	,	223	,	255	,	319	,	1823	,	1301	,	4401	,	12827	,	889	};
            const unsigned long	dim2174KuoInit[]		=	{	1	,	3	,	5	,	1	,	1	,	5	,	5	,	109	,	27	,	963	,	727	,	335	,	6295	,	9775	,	32681	};
            const unsigned long	dim2175KuoInit[]		=	{	1	,	3	,	7	,	15	,	29	,	47	,	55	,	253	,	281	,	303	,	857	,	903	,	2977	,	14777	,	31689	};
            const unsigned long	dim2176KuoInit[]		=	{	1	,	3	,	1	,	7	,	17	,	49	,	105	,	155	,	467	,	467	,	1287	,	1281	,	3429	,	3195	,	15297	};
            const unsigned long	dim2177KuoInit[]		=	{	1	,	3	,	1	,	1	,	27	,	31	,	17	,	201	,	23	,	897	,	1991	,	2661	,	645	,	16127	,	30857	};
            const unsigned long	dim2178KuoInit[]		=	{	1	,	3	,	3	,	11	,	9	,	15	,	85	,	183	,	31	,	179	,	779	,	171	,	5989	,	7253	,	15077	};
            const unsigned long	dim2179KuoInit[]		=	{	1	,	1	,	1	,	5	,	19	,	23	,	37	,	39	,	281	,	707	,	17	,	2707	,	289	,	12081	,	4381	};
            const unsigned long	dim2180KuoInit[]		=	{	1	,	1	,	7	,	9	,	21	,	27	,	115	,	179	,	99	,	277	,	331	,	3407	,	2057	,	13893	,	11501	};
            const unsigned long	dim2181KuoInit[]		=	{	1	,	3	,	1	,	9	,	31	,	51	,	51	,	25	,	319	,	319	,	27	,	2161	,	89	,	3421	,	18273	};
            const unsigned long	dim2182KuoInit[]		=	{	1	,	1	,	7	,	15	,	13	,	21	,	11	,	189	,	63	,	15	,	363	,	1951	,	5751	,	5797	,	22123	};
            const unsigned long	dim2183KuoInit[]		=	{	1	,	1	,	3	,	1	,	25	,	47	,	99	,	77	,	77	,	595	,	1473	,	1653	,	6751	,	5005	,	29051	};
            const unsigned long	dim2184KuoInit[]		=	{	1	,	1	,	3	,	7	,	25	,	47	,	31	,	13	,	311	,	269	,	1095	,	1359	,	305	,	6811	,	16455	};
            const unsigned long	dim2185KuoInit[]		=	{	1	,	1	,	5	,	3	,	13	,	63	,	87	,	13	,	221	,	459	,	1177	,	891	,	5781	,	13803	,	18795	};
            const unsigned long	dim2186KuoInit[]		=	{	1	,	1	,	7	,	3	,	23	,	13	,	25	,	115	,	47	,	903	,	1551	,	2887	,	229	,	16067	,	9511	};
            const unsigned long	dim2187KuoInit[]		=	{	1	,	1	,	7	,	15	,	23	,	15	,	7	,	65	,	27	,	677	,	1503	,	125	,	7739	,	14655	,	29577	};
            const unsigned long	dim2188KuoInit[]		=	{	1	,	1	,	1	,	5	,	9	,	23	,	93	,	187	,	205	,	915	,	599	,	2105	,	7767	,	2411	,	15045	};
            const unsigned long	dim2189KuoInit[]		=	{	1	,	3	,	5	,	1	,	21	,	45	,	17	,	57	,	171	,	491	,	1465	,	2013	,	3253	,	5517	,	19113	};
            const unsigned long	dim2190KuoInit[]		=	{	1	,	1	,	7	,	5	,	5	,	25	,	87	,	247	,	301	,	385	,	1655	,	1351	,	6307	,	14695	,	29745	};
            const unsigned long	dim2191KuoInit[]		=	{	1	,	3	,	7	,	3	,	31	,	25	,	113	,	155	,	123	,	755	,	649	,	1809	,	1703	,	875	,	5235	};
            const unsigned long	dim2192KuoInit[]		=	{	1	,	1	,	1	,	7	,	7	,	43	,	121	,	47	,	499	,	63	,	449	,	2039	,	835	,	2883	,	2287	};
            const unsigned long	dim2193KuoInit[]		=	{	1	,	3	,	7	,	1	,	9	,	11	,	99	,	169	,	305	,	469	,	157	,	2043	,	4851	,	3367	,	8689	};
            const unsigned long	dim2194KuoInit[]		=	{	1	,	1	,	1	,	11	,	17	,	15	,	41	,	209	,	377	,	81	,	335	,	1685	,	143	,	1513	,	28465	};
            const unsigned long	dim2195KuoInit[]		=	{	1	,	3	,	1	,	15	,	9	,	45	,	83	,	123	,	347	,	437	,	783	,	3561	,	6003	,	9329	,	4729	};
            const unsigned long	dim2196KuoInit[]		=	{	1	,	1	,	1	,	1	,	13	,	33	,	109	,	217	,	403	,	539	,	1393	,	1069	,	235	,	11261	,	13423	};
            const unsigned long	dim2197KuoInit[]		=	{	1	,	1	,	5	,	13	,	27	,	1	,	81	,	139	,	421	,	301	,	55	,	1883	,	1525	,	6029	,	1879	};
            const unsigned long	dim2198KuoInit[]		=	{	1	,	1	,	1	,	15	,	15	,	55	,	49	,	249	,	487	,	373	,	737	,	2133	,	6883	,	1603	,	27607	};
            const unsigned long	dim2199KuoInit[]		=	{	1	,	1	,	1	,	13	,	25	,	17	,	17	,	15	,	119	,	571	,	1053	,	2153	,	5221	,	2681	,	24595	};
            const unsigned long	dim2200KuoInit[]		=	{	1	,	3	,	1	,	5	,	3	,	63	,	31	,	9	,	263	,	977	,	575	,	1225	,	4127	,	15649	,	1281	};
            const unsigned long	dim2201KuoInit[]		=	{	1	,	1	,	7	,	9	,	19	,	1	,	37	,	109	,	13	,	191	,	1483	,	465	,	1321	,	13697	,	26065	};
            const unsigned long	dim2202KuoInit[]		=	{	1	,	1	,	3	,	9	,	17	,	15	,	103	,	97	,	237	,	861	,	1251	,	2069	,	4459	,	9999	,	7369	};
            const unsigned long	dim2203KuoInit[]		=	{	1	,	3	,	7	,	9	,	15	,	33	,	55	,	5	,	355	,	519	,	445	,	4033	,	627	,	2375	,	31013	};
            const unsigned long	dim2204KuoInit[]		=	{	1	,	1	,	1	,	3	,	23	,	51	,	65	,	177	,	225	,	63	,	987	,	2921	,	823	,	1781	,	22923	};
            const unsigned long	dim2205KuoInit[]		=	{	1	,	3	,	5	,	7	,	27	,	33	,	111	,	47	,	465	,	379	,	1575	,	1793	,	513	,	14033	,	27409	};
            const unsigned long	dim2206KuoInit[]		=	{	1	,	3	,	5	,	7	,	1	,	57	,	31	,	195	,	393	,	257	,	1489	,	2589	,	5339	,	7487	,	29023	};
            const unsigned long	dim2207KuoInit[]		=	{	1	,	3	,	5	,	11	,	15	,	27	,	39	,	89	,	387	,	753	,	1213	,	2417	,	6287	,	129	,	12295	};
            const unsigned long	dim2208KuoInit[]		=	{	1	,	1	,	3	,	11	,	11	,	37	,	117	,	55	,	55	,	677	,	1057	,	3971	,	5017	,	12941	,	3001	};
            const unsigned long	dim2209KuoInit[]		=	{	1	,	3	,	1	,	7	,	21	,	33	,	103	,	19	,	427	,	637	,	393	,	2245	,	2853	,	15927	,	14633	};
            const unsigned long	dim2210KuoInit[]		=	{	1	,	1	,	5	,	7	,	31	,	11	,	75	,	191	,	367	,	659	,	121	,	2947	,	515	,	10625	,	19753	};
            const unsigned long	dim2211KuoInit[]		=	{	1	,	1	,	5	,	5	,	15	,	63	,	115	,	119	,	85	,	527	,	875	,	67	,	475	,	2207	,	4101	};
            const unsigned long	dim2212KuoInit[]		=	{	1	,	3	,	3	,	9	,	23	,	37	,	53	,	187	,	37	,	535	,	509	,	1765	,	6577	,	13249	,	3875	};
            const unsigned long	dim2213KuoInit[]		=	{	1	,	3	,	3	,	5	,	13	,	49	,	23	,	193	,	109	,	295	,	309	,	2457	,	4193	,	9927	,	16313	};
            const unsigned long	dim2214KuoInit[]		=	{	1	,	3	,	3	,	5	,	5	,	15	,	33	,	69	,	225	,	231	,	131	,	3135	,	3597	,	2989	,	12365	};
            const unsigned long	dim2215KuoInit[]		=	{	1	,	3	,	7	,	1	,	27	,	33	,	9	,	139	,	507	,	789	,	663	,	1693	,	949	,	9149	,	29529	};
            const unsigned long	dim2216KuoInit[]		=	{	1	,	1	,	7	,	7	,	11	,	1	,	57	,	159	,	511	,	929	,	1337	,	2181	,	5573	,	425	,	26935	};
            const unsigned long	dim2217KuoInit[]		=	{	1	,	1	,	5	,	9	,	15	,	53	,	57	,	81	,	105	,	389	,	1563	,	1	,	669	,	8403	,	15747	};
            const unsigned long	dim2218KuoInit[]		=	{	1	,	3	,	3	,	5	,	19	,	21	,	29	,	133	,	203	,	689	,	2011	,	1825	,	3365	,	1957	,	21393	};
            const unsigned long	dim2219KuoInit[]		=	{	1	,	1	,	3	,	15	,	7	,	11	,	73	,	189	,	63	,	81	,	1429	,	3403	,	4249	,	16265	,	22423	};
            const unsigned long	dim2220KuoInit[]		=	{	1	,	1	,	5	,	1	,	31	,	47	,	75	,	199	,	343	,	957	,	691	,	939	,	2165	,	10523	,	3577	};
            const unsigned long	dim2221KuoInit[]		=	{	1	,	1	,	7	,	13	,	3	,	27	,	5	,	131	,	437	,	405	,	1809	,	2637	,	5735	,	12897	,	22905	};
            const unsigned long	dim2222KuoInit[]		=	{	1	,	1	,	5	,	1	,	17	,	17	,	85	,	161	,	205	,	81	,	1125	,	1783	,	2241	,	6321	,	10891	};
            const unsigned long	dim2223KuoInit[]		=	{	1	,	3	,	7	,	15	,	27	,	7	,	119	,	61	,	383	,	171	,	1495	,	1237	,	729	,	1977	,	11779	};
            const unsigned long	dim2224KuoInit[]		=	{	1	,	1	,	5	,	7	,	21	,	3	,	63	,	167	,	493	,	117	,	855	,	143	,	2579	,	7527	,	7395	};
            const unsigned long	dim2225KuoInit[]		=	{	1	,	1	,	1	,	9	,	31	,	61	,	15	,	31	,	445	,	397	,	1369	,	1047	,	4751	,	10375	,	30319	};
            const unsigned long	dim2226KuoInit[]		=	{	1	,	3	,	1	,	15	,	7	,	13	,	87	,	99	,	333	,	433	,	287	,	2861	,	2085	,	14691	,	14801	};
            const unsigned long	dim2227KuoInit[]		=	{	1	,	3	,	3	,	7	,	27	,	45	,	41	,	69	,	391	,	259	,	899	,	1019	,	2999	,	6487	,	19831	};
            const unsigned long	dim2228KuoInit[]		=	{	1	,	3	,	5	,	11	,	19	,	33	,	49	,	87	,	89	,	649	,	1209	,	43	,	8035	,	12687	,	24783	};
            const unsigned long	dim2229KuoInit[]		=	{	1	,	3	,	1	,	9	,	13	,	11	,	55	,	5	,	315	,	347	,	1361	,	41	,	807	,	11551	,	499	};
            const unsigned long	dim2230KuoInit[]		=	{	1	,	1	,	7	,	1	,	25	,	15	,	11	,	19	,	233	,	269	,	135	,	2577	,	4385	,	8109	,	13161	};
            const unsigned long	dim2231KuoInit[]		=	{	1	,	3	,	1	,	9	,	19	,	13	,	127	,	79	,	49	,	525	,	1499	,	1209	,	3747	,	8311	,	15061	};
            const unsigned long	dim2232KuoInit[]		=	{	1	,	3	,	1	,	15	,	9	,	47	,	59	,	81	,	491	,	401	,	1691	,	543	,	7129	,	13617	,	17031	};
            const unsigned long	dim2233KuoInit[]		=	{	1	,	3	,	3	,	5	,	15	,	51	,	41	,	85	,	245	,	91	,	1293	,	2041	,	7205	,	12709	,	17421	};
            const unsigned long	dim2234KuoInit[]		=	{	1	,	3	,	1	,	5	,	3	,	19	,	29	,	149	,	51	,	133	,	953	,	2385	,	7471	,	14829	,	8197	};
            const unsigned long	dim2235KuoInit[]		=	{	1	,	3	,	1	,	11	,	31	,	15	,	5	,	139	,	299	,	595	,	1585	,	2115	,	5647	,	12125	,	1265	};
            const unsigned long	dim2236KuoInit[]		=	{	1	,	3	,	7	,	9	,	7	,	5	,	93	,	187	,	307	,	329	,	671	,	3983	,	817	,	3965	,	21561	};
            const unsigned long	dim2237KuoInit[]		=	{	1	,	3	,	7	,	1	,	21	,	57	,	67	,	159	,	467	,	65	,	1365	,	2797	,	7051	,	12603	,	10429	};
            const unsigned long	dim2238KuoInit[]		=	{	1	,	3	,	1	,	7	,	27	,	59	,	113	,	199	,	13	,	393	,	1605	,	3215	,	5233	,	12749	,	32365	};
            const unsigned long	dim2239KuoInit[]		=	{	1	,	3	,	1	,	1	,	1	,	13	,	15	,	217	,	317	,	945	,	1713	,	1951	,	5585	,	1529	,	4969	};
            const unsigned long	dim2240KuoInit[]		=	{	1	,	1	,	7	,	15	,	3	,	59	,	23	,	255	,	71	,	539	,	21	,	2603	,	5145	,	295	,	2681	};
            const unsigned long	dim2241KuoInit[]		=	{	1	,	1	,	1	,	13	,	11	,	41	,	85	,	145	,	209	,	1001	,	659	,	951	,	4039	,	1409	,	4059	};
            const unsigned long	dim2242KuoInit[]		=	{	1	,	1	,	7	,	13	,	15	,	41	,	15	,	121	,	485	,	515	,	325	,	3673	,	2883	,	8119	,	29183	};
            const unsigned long	dim2243KuoInit[]		=	{	1	,	3	,	3	,	13	,	21	,	39	,	31	,	163	,	13	,	219	,	411	,	2061	,	1999	,	2999	,	24835	};
            const unsigned long	dim2244KuoInit[]		=	{	1	,	3	,	7	,	7	,	21	,	49	,	9	,	75	,	401	,	339	,	191	,	2679	,	7483	,	1361	,	21399	};
            const unsigned long	dim2245KuoInit[]		=	{	1	,	3	,	1	,	13	,	5	,	29	,	9	,	77	,	459	,	247	,	1777	,	3873	,	6747	,	7541	,	12015	};
            const unsigned long	dim2246KuoInit[]		=	{	1	,	1	,	5	,	9	,	23	,	27	,	55	,	51	,	389	,	63	,	1163	,	255	,	6049	,	1737	,	12403	};
            const unsigned long	dim2247KuoInit[]		=	{	1	,	1	,	7	,	3	,	23	,	25	,	81	,	247	,	261	,	445	,	1297	,	1945	,	2769	,	1287	,	24403	};
            const unsigned long	dim2248KuoInit[]		=	{	1	,	1	,	5	,	15	,	19	,	13	,	127	,	105	,	475	,	859	,	683	,	941	,	2911	,	14571	,	22769	};
            const unsigned long	dim2249KuoInit[]		=	{	1	,	1	,	1	,	13	,	31	,	25	,	55	,	63	,	355	,	525	,	397	,	3213	,	2267	,	2731	,	32145	};
            const unsigned long	dim2250KuoInit[]		=	{	1	,	1	,	5	,	15	,	3	,	21	,	45	,	229	,	83	,	661	,	149	,	1615	,	5309	,	12233	,	23101	};
            const unsigned long	dim2251KuoInit[]		=	{	1	,	1	,	7	,	1	,	15	,	41	,	91	,	17	,	119	,	65	,	1921	,	97	,	6521	,	8969	,	5419	};
            const unsigned long	dim2252KuoInit[]		=	{	1	,	1	,	7	,	11	,	31	,	35	,	73	,	123	,	331	,	83	,	125	,	3001	,	5485	,	1693	,	24327	};
            const unsigned long	dim2253KuoInit[]		=	{	1	,	3	,	5	,	15	,	29	,	37	,	63	,	229	,	143	,	529	,	961	,	2845	,	8003	,	7741	,	23393	};
            const unsigned long	dim2254KuoInit[]		=	{	1	,	3	,	1	,	15	,	25	,	27	,	91	,	171	,	361	,	575	,	1977	,	2141	,	89	,	7097	,	8115	};
            const unsigned long	dim2255KuoInit[]		=	{	1	,	3	,	7	,	15	,	13	,	7	,	51	,	253	,	17	,	991	,	595	,	1819	,	7091	,	8619	,	1147	};
            const unsigned long	dim2256KuoInit[]		=	{	1	,	3	,	3	,	7	,	13	,	31	,	41	,	11	,	57	,	185	,	1353	,	907	,	6585	,	8597	,	10239	};
            const unsigned long	dim2257KuoInit[]		=	{	1	,	3	,	1	,	11	,	31	,	1	,	119	,	165	,	105	,	725	,	493	,	3279	,	7117	,	15141	,	5957	};
            const unsigned long	dim2258KuoInit[]		=	{	1	,	3	,	3	,	13	,	29	,	1	,	35	,	95	,	477	,	847	,	733	,	2599	,	581	,	6487	,	10879	};
            const unsigned long	dim2259KuoInit[]		=	{	1	,	3	,	7	,	3	,	15	,	23	,	115	,	177	,	409	,	711	,	1179	,	685	,	4071	,	15129	,	2951	};
            const unsigned long	dim2260KuoInit[]		=	{	1	,	1	,	3	,	11	,	19	,	5	,	69	,	33	,	41	,	501	,	135	,	1063	,	4857	,	15537	,	29247	};
            const unsigned long	dim2261KuoInit[]		=	{	1	,	3	,	5	,	13	,	1	,	25	,	123	,	1	,	431	,	399	,	483	,	3159	,	7359	,	6169	,	22525	};
            const unsigned long	dim2262KuoInit[]		=	{	1	,	3	,	5	,	7	,	1	,	9	,	125	,	131	,	99	,	911	,	421	,	1403	,	993	,	14155	,	20863	};
            const unsigned long	dim2263KuoInit[]		=	{	1	,	3	,	1	,	3	,	1	,	43	,	93	,	9	,	345	,	483	,	1809	,	2379	,	653	,	14851	,	20619	};
            const unsigned long	dim2264KuoInit[]		=	{	1	,	3	,	7	,	5	,	15	,	37	,	33	,	185	,	173	,	397	,	2021	,	613	,	37	,	7185	,	15399	};
            const unsigned long	dim2265KuoInit[]		=	{	1	,	3	,	5	,	5	,	17	,	55	,	119	,	207	,	301	,	461	,	1853	,	2471	,	3611	,	1149	,	3321	};
            const unsigned long	dim2266KuoInit[]		=	{	1	,	3	,	3	,	3	,	25	,	57	,	51	,	135	,	337	,	935	,	619	,	1165	,	1613	,	8711	,	2329	};
            const unsigned long	dim2267KuoInit[]		=	{	1	,	3	,	1	,	15	,	27	,	25	,	63	,	127	,	169	,	221	,	1103	,	1087	,	1729	,	3967	,	6323	};
            const unsigned long	dim2268KuoInit[]		=	{	1	,	1	,	3	,	11	,	13	,	29	,	105	,	189	,	321	,	103	,	2047	,	923	,	4443	,	2175	,	12411	};
            const unsigned long	dim2269KuoInit[]		=	{	1	,	1	,	7	,	9	,	7	,	3	,	3	,	61	,	479	,	533	,	227	,	3137	,	6065	,	3621	,	3583	};
            const unsigned long	dim2270KuoInit[]		=	{	1	,	1	,	5	,	13	,	11	,	31	,	23	,	147	,	67	,	823	,	663	,	3717	,	6747	,	10935	,	21931	};
            const unsigned long	dim2271KuoInit[]		=	{	1	,	1	,	5	,	13	,	29	,	9	,	75	,	31	,	349	,	603	,	1071	,	1767	,	1811	,	15921	,	25681	};
            const unsigned long	dim2272KuoInit[]		=	{	1	,	3	,	7	,	13	,	29	,	31	,	57	,	15	,	405	,	263	,	715	,	445	,	1825	,	15057	,	3449	};
            const unsigned long	dim2273KuoInit[]		=	{	1	,	3	,	5	,	5	,	5	,	21	,	61	,	137	,	15	,	935	,	897	,	125	,	6651	,	6813	,	27863	};
            const unsigned long	dim2274KuoInit[]		=	{	1	,	1	,	3	,	13	,	9	,	53	,	5	,	91	,	93	,	179	,	489	,	3615	,	5911	,	7501	,	14205	};
            const unsigned long	dim2275KuoInit[]		=	{	1	,	1	,	5	,	13	,	27	,	49	,	35	,	93	,	371	,	519	,	589	,	1563	,	2609	,	14055	,	28071	};
            const unsigned long	dim2276KuoInit[]		=	{	1	,	1	,	5	,	7	,	29	,	41	,	23	,	135	,	491	,	745	,	1327	,	4065	,	6327	,	6375	,	24901	};
            const unsigned long	dim2277KuoInit[]		=	{	1	,	3	,	3	,	5	,	23	,	47	,	113	,	85	,	453	,	321	,	597	,	1817	,	7579	,	11633	,	25569	};
            const unsigned long	dim2278KuoInit[]		=	{	1	,	3	,	1	,	13	,	13	,	55	,	127	,	47	,	159	,	885	,	235	,	199	,	3133	,	9793	,	25877	};
            const unsigned long	dim2279KuoInit[]		=	{	1	,	1	,	3	,	9	,	17	,	21	,	27	,	237	,	413	,	999	,	451	,	3279	,	1607	,	9625	,	16943	};
            const unsigned long	dim2280KuoInit[]		=	{	1	,	3	,	1	,	1	,	21	,	61	,	29	,	143	,	33	,	235	,	1591	,	2379	,	3573	,	1399	,	16215	};
            const unsigned long	dim2281KuoInit[]		=	{	1	,	1	,	7	,	1	,	29	,	23	,	95	,	207	,	87	,	199	,	1389	,	3587	,	71	,	14401	,	28431	};
            const unsigned long	dim2282KuoInit[]		=	{	1	,	1	,	5	,	13	,	23	,	43	,	109	,	119	,	151	,	451	,	23	,	2709	,	3125	,	11541	,	3923	};
            const unsigned long	dim2283KuoInit[]		=	{	1	,	3	,	7	,	3	,	11	,	55	,	5	,	227	,	171	,	465	,	1685	,	333	,	2605	,	15055	,	8333	};
            const unsigned long	dim2284KuoInit[]		=	{	1	,	3	,	5	,	3	,	21	,	11	,	57	,	37	,	67	,	667	,	521	,	2961	,	6145	,	12527	,	17433	};
            const unsigned long	dim2285KuoInit[]		=	{	1	,	1	,	7	,	11	,	9	,	39	,	103	,	83	,	313	,	827	,	721	,	2965	,	3231	,	10121	,	19991	};
            const unsigned long	dim2286KuoInit[]		=	{	1	,	1	,	5	,	7	,	23	,	9	,	31	,	39	,	7	,	767	,	153	,	2969	,	5539	,	6581	,	30515	};
            const unsigned long	dim2287KuoInit[]		=	{	1	,	1	,	3	,	13	,	3	,	43	,	49	,	143	,	3	,	227	,	99	,	597	,	7207	,	4737	,	2259	};
            const unsigned long	dim2288KuoInit[]		=	{	1	,	3	,	3	,	3	,	3	,	57	,	69	,	243	,	199	,	803	,	657	,	1209	,	1343	,	11673	,	21543	};
            const unsigned long	dim2289KuoInit[]		=	{	1	,	1	,	1	,	1	,	17	,	27	,	127	,	107	,	171	,	391	,	1737	,	1247	,	1389	,	14387	,	1671	};
            const unsigned long	dim2290KuoInit[]		=	{	1	,	3	,	3	,	9	,	23	,	47	,	97	,	139	,	435	,	943	,	1111	,	2723	,	1021	,	1155	,	10241	};
            const unsigned long	dim2291KuoInit[]		=	{	1	,	3	,	3	,	13	,	15	,	15	,	101	,	205	,	229	,	639	,	1077	,	1613	,	3035	,	12897	,	14303	};
            const unsigned long	dim2292KuoInit[]		=	{	1	,	1	,	1	,	9	,	17	,	43	,	3	,	81	,	149	,	931	,	731	,	4005	,	7397	,	8707	,	27927	};
            const unsigned long	dim2293KuoInit[]		=	{	1	,	3	,	7	,	15	,	19	,	59	,	97	,	97	,	347	,	489	,	1577	,	3927	,	3431	,	4051	,	8489	};
            const unsigned long	dim2294KuoInit[]		=	{	1	,	1	,	5	,	3	,	29	,	49	,	19	,	151	,	27	,	701	,	541	,	481	,	101	,	10701	,	31909	};
            const unsigned long	dim2295KuoInit[]		=	{	1	,	1	,	1	,	9	,	11	,	47	,	59	,	45	,	439	,	757	,	1579	,	1681	,	6701	,	11707	,	10715	};
            const unsigned long	dim2296KuoInit[]		=	{	1	,	3	,	3	,	11	,	27	,	55	,	1	,	35	,	491	,	767	,	1247	,	1143	,	5079	,	8029	,	10267	};
            const unsigned long	dim2297KuoInit[]		=	{	1	,	3	,	3	,	1	,	15	,	5	,	53	,	215	,	249	,	659	,	329	,	1959	,	249	,	11775	,	4815	};
            const unsigned long	dim2298KuoInit[]		=	{	1	,	3	,	7	,	15	,	19	,	17	,	41	,	29	,	157	,	995	,	235	,	1397	,	3983	,	11739	,	7523	};
            const unsigned long	dim2299KuoInit[]		=	{	1	,	3	,	5	,	5	,	13	,	3	,	55	,	211	,	59	,	717	,	1389	,	3219	,	2279	,	9583	,	16981	};
            const unsigned long	dim2300KuoInit[]		=	{	1	,	3	,	5	,	11	,	31	,	41	,	95	,	217	,	375	,	255	,	1433	,	1595	,	5795	,	11893	,	16665	};
            const unsigned long	dim2301KuoInit[]		=	{	1	,	3	,	1	,	13	,	15	,	45	,	19	,	125	,	387	,	1017	,	1075	,	3469	,	7873	,	9135	,	28215	};
            const unsigned long	dim2302KuoInit[]		=	{	1	,	3	,	7	,	5	,	23	,	41	,	35	,	171	,	159	,	545	,	2043	,	887	,	8167	,	8977	,	30985	};
            const unsigned long	dim2303KuoInit[]		=	{	1	,	3	,	1	,	3	,	21	,	17	,	31	,	171	,	45	,	979	,	757	,	3839	,	4221	,	8659	,	7005	};
            const unsigned long	dim2304KuoInit[]		=	{	1	,	3	,	3	,	1	,	3	,	23	,	81	,	157	,	385	,	233	,	527	,	2357	,	7299	,	10977	,	16285	};
            const unsigned long	dim2305KuoInit[]		=	{	1	,	1	,	7	,	13	,	23	,	27	,	31	,	211	,	451	,	211	,	45	,	2239	,	981	,	14279	,	14515	};
            const unsigned long	dim2306KuoInit[]		=	{	1	,	3	,	1	,	5	,	13	,	61	,	17	,	197	,	455	,	457	,	2023	,	1809	,	4243	,	3269	,	7159	};
            const unsigned long	dim2307KuoInit[]		=	{	1	,	1	,	5	,	5	,	9	,	61	,	49	,	111	,	281	,	827	,	1155	,	3107	,	5761	,	12043	,	3061	};
            const unsigned long	dim2308KuoInit[]		=	{	1	,	1	,	1	,	5	,	19	,	29	,	63	,	209	,	207	,	503	,	1563	,	2539	,	5115	,	12939	,	22977	};
            const unsigned long	dim2309KuoInit[]		=	{	1	,	3	,	7	,	5	,	31	,	33	,	91	,	45	,	399	,	263	,	53	,	2723	,	7909	,	7125	,	14795	};
            const unsigned long	dim2310KuoInit[]		=	{	1	,	1	,	7	,	11	,	9	,	5	,	119	,	91	,	87	,	847	,	85	,	4025	,	1891	,	791	,	30447	};
            const unsigned long	dim2311KuoInit[]		=	{	1	,	1	,	1	,	1	,	25	,	59	,	101	,	85	,	159	,	491	,	635	,	1047	,	6221	,	11579	,	13943	};
            const unsigned long	dim2312KuoInit[]		=	{	1	,	3	,	7	,	3	,	27	,	55	,	19	,	29	,	479	,	299	,	1705	,	3505	,	6811	,	14851	,	6827	};
            const unsigned long	dim2313KuoInit[]		=	{	1	,	1	,	1	,	9	,	23	,	19	,	47	,	235	,	81	,	35	,	1297	,	2029	,	1303	,	6877	,	30813	};
            const unsigned long	dim2314KuoInit[]		=	{	1	,	3	,	1	,	11	,	21	,	37	,	49	,	209	,	93	,	439	,	2023	,	437	,	6405	,	2407	,	10475	};
            const unsigned long	dim2315KuoInit[]		=	{	1	,	1	,	3	,	3	,	5	,	51	,	11	,	127	,	339	,	419	,	1495	,	43	,	5707	,	3905	,	331	};
            const unsigned long	dim2316KuoInit[]		=	{	1	,	1	,	5	,	15	,	27	,	7	,	73	,	137	,	111	,	283	,	1017	,	1487	,	7359	,	4921	,	3285	};
            const unsigned long	dim2317KuoInit[]		=	{	1	,	1	,	1	,	9	,	3	,	23	,	123	,	75	,	317	,	517	,	109	,	2307	,	1095	,	10309	,	27651	};
            const unsigned long	dim2318KuoInit[]		=	{	1	,	3	,	1	,	1	,	21	,	55	,	5	,	115	,	483	,	379	,	1935	,	3483	,	2485	,	1991	,	2403	};
            const unsigned long	dim2319KuoInit[]		=	{	1	,	3	,	5	,	9	,	27	,	57	,	1	,	227	,	235	,	373	,	241	,	1115	,	3037	,	6841	,	28711	};
            const unsigned long	dim2320KuoInit[]		=	{	1	,	3	,	1	,	13	,	21	,	27	,	19	,	217	,	449	,	727	,	475	,	515	,	7933	,	4401	,	5455	};
            const unsigned long	dim2321KuoInit[]		=	{	1	,	3	,	5	,	13	,	27	,	3	,	117	,	67	,	293	,	625	,	1463	,	3713	,	1271	,	11873	,	24239	};
            const unsigned long	dim2322KuoInit[]		=	{	1	,	1	,	3	,	1	,	7	,	25	,	5	,	187	,	377	,	15	,	1973	,	597	,	725	,	8809	,	2419	};
            const unsigned long	dim2323KuoInit[]		=	{	1	,	1	,	7	,	13	,	7	,	9	,	89	,	41	,	443	,	921	,	17	,	3675	,	715	,	7147	,	25345	};
            const unsigned long	dim2324KuoInit[]		=	{	1	,	1	,	5	,	1	,	31	,	27	,	127	,	139	,	101	,	349	,	1551	,	1703	,	5981	,	3259	,	31203	};
            const unsigned long	dim2325KuoInit[]		=	{	1	,	1	,	1	,	5	,	31	,	59	,	1	,	149	,	111	,	947	,	1919	,	2841	,	5383	,	6973	,	19565	};
            const unsigned long	dim2326KuoInit[]		=	{	1	,	3	,	3	,	11	,	5	,	57	,	65	,	197	,	413	,	741	,	1737	,	1333	,	5101	,	14375	,	28969	};
            const unsigned long	dim2327KuoInit[]		=	{	1	,	1	,	1	,	3	,	5	,	21	,	31	,	187	,	51	,	267	,	539	,	1305	,	4085	,	14735	,	9441	};
            const unsigned long	dim2328KuoInit[]		=	{	1	,	1	,	7	,	1	,	9	,	27	,	123	,	13	,	383	,	739	,	649	,	1253	,	4697	,	9571	,	16983	};
            const unsigned long	dim2329KuoInit[]		=	{	1	,	3	,	1	,	11	,	23	,	3	,	31	,	153	,	185	,	969	,	329	,	3919	,	3905	,	14321	,	16535	};
            const unsigned long	dim2330KuoInit[]		=	{	1	,	1	,	1	,	5	,	15	,	31	,	97	,	243	,	117	,	269	,	1545	,	1495	,	167	,	14281	,	1369	};
            const unsigned long	dim2331KuoInit[]		=	{	1	,	1	,	1	,	1	,	7	,	47	,	9	,	187	,	233	,	629	,	1981	,	2327	,	7473	,	14641	,	31829	};
            const unsigned long	dim2332KuoInit[]		=	{	1	,	1	,	1	,	11	,	11	,	43	,	117	,	193	,	395	,	501	,	1213	,	3499	,	3369	,	817	,	22069	};
            const unsigned long	dim2333KuoInit[]		=	{	1	,	3	,	7	,	1	,	13	,	63	,	67	,	67	,	119	,	371	,	1887	,	1281	,	129	,	14277	,	5213	};
            const unsigned long	dim2334KuoInit[]		=	{	1	,	3	,	1	,	11	,	11	,	57	,	47	,	149	,	119	,	547	,	389	,	1977	,	1709	,	2335	,	7023	};
            const unsigned long	dim2335KuoInit[]		=	{	1	,	1	,	7	,	7	,	15	,	41	,	19	,	57	,	279	,	737	,	29	,	2015	,	1103	,	4933	,	26145	};
            const unsigned long	dim2336KuoInit[]		=	{	1	,	3	,	1	,	1	,	23	,	61	,	17	,	1	,	309	,	637	,	809	,	3443	,	135	,	6851	,	27321	};
            const unsigned long	dim2337KuoInit[]		=	{	1	,	3	,	1	,	3	,	21	,	63	,	81	,	131	,	495	,	209	,	1901	,	2921	,	1021	,	7207	,	22053	};
            const unsigned long	dim2338KuoInit[]		=	{	1	,	1	,	7	,	9	,	7	,	63	,	1	,	119	,	329	,	553	,	1405	,	97	,	4751	,	6955	,	20049	};
            const unsigned long	dim2339KuoInit[]		=	{	1	,	3	,	7	,	15	,	1	,	59	,	99	,	201	,	487	,	271	,	2035	,	3261	,	4953	,	15257	,	11835	};
            const unsigned long	dim2340KuoInit[]		=	{	1	,	1	,	5	,	11	,	27	,	21	,	27	,	227	,	331	,	779	,	1003	,	2291	,	7139	,	14125	,	14371	};
            const unsigned long	dim2341KuoInit[]		=	{	1	,	1	,	3	,	13	,	15	,	43	,	23	,	183	,	463	,	273	,	1707	,	2001	,	3219	,	14481	,	751	};
            const unsigned long	dim2342KuoInit[]		=	{	1	,	1	,	5	,	5	,	23	,	31	,	51	,	39	,	221	,	687	,	2003	,	1251	,	3541	,	6347	,	551	};
            const unsigned long	dim2343KuoInit[]		=	{	1	,	1	,	7	,	3	,	17	,	51	,	3	,	57	,	131	,	13	,	2027	,	3301	,	4223	,	6499	,	30913	};
            const unsigned long	dim2344KuoInit[]		=	{	1	,	1	,	7	,	13	,	11	,	47	,	53	,	205	,	59	,	315	,	1595	,	3747	,	2623	,	7317	,	8309	};
            const unsigned long	dim2345KuoInit[]		=	{	1	,	3	,	1	,	11	,	13	,	45	,	83	,	131	,	193	,	739	,	61	,	1657	,	3789	,	9859	,	3755	};
            const unsigned long	dim2346KuoInit[]		=	{	1	,	3	,	7	,	11	,	25	,	7	,	123	,	103	,	307	,	463	,	1113	,	2245	,	2819	,	12983	,	18077	};
            const unsigned long	dim2347KuoInit[]		=	{	1	,	1	,	1	,	1	,	7	,	23	,	103	,	183	,	409	,	657	,	583	,	583	,	5449	,	7397	,	13085	};
            const unsigned long	dim2348KuoInit[]		=	{	1	,	3	,	3	,	5	,	3	,	61	,	31	,	147	,	31	,	449	,	1579	,	4021	,	1163	,	9719	,	27079	};
            const unsigned long	dim2349KuoInit[]		=	{	1	,	3	,	1	,	3	,	9	,	53	,	125	,	77	,	99	,	863	,	1719	,	51	,	4183	,	13009	,	791	};
            const unsigned long	dim2350KuoInit[]		=	{	1	,	3	,	3	,	11	,	27	,	33	,	43	,	5	,	173	,	775	,	593	,	3701	,	3323	,	6481	,	25765	};
            const unsigned long	dim2351KuoInit[]		=	{	1	,	1	,	7	,	15	,	11	,	3	,	109	,	221	,	375	,	883	,	903	,	2361	,	255	,	12851	,	16697	};
            const unsigned long	dim2352KuoInit[]		=	{	1	,	1	,	5	,	9	,	3	,	61	,	127	,	89	,	213	,	71	,	303	,	2525	,	3691	,	2945	,	12359	};
            const unsigned long	dim2353KuoInit[]		=	{	1	,	3	,	1	,	13	,	19	,	15	,	7	,	231	,	159	,	275	,	1161	,	819	,	3003	,	12707	,	20699	};
            const unsigned long	dim2354KuoInit[]		=	{	1	,	1	,	3	,	1	,	31	,	51	,	125	,	21	,	27	,	427	,	941	,	237	,	4343	,	9555	,	30511	};
            const unsigned long	dim2355KuoInit[]		=	{	1	,	3	,	3	,	9	,	17	,	23	,	105	,	21	,	471	,	357	,	773	,	1185	,	2655	,	2317	,	9969	};
            const unsigned long	dim2356KuoInit[]		=	{	1	,	1	,	5	,	7	,	15	,	55	,	87	,	249	,	111	,	319	,	985	,	2305	,	1641	,	12655	,	7321	};
            const unsigned long	dim2357KuoInit[]		=	{	1	,	3	,	5	,	3	,	23	,	49	,	85	,	31	,	299	,	391	,	549	,	3831	,	1413	,	13803	,	30599	};
            const unsigned long	dim2358KuoInit[]		=	{	1	,	1	,	3	,	3	,	31	,	39	,	43	,	253	,	67	,	321	,	463	,	2327	,	6831	,	10461	,	18077	};
            const unsigned long	dim2359KuoInit[]		=	{	1	,	3	,	1	,	9	,	13	,	13	,	127	,	175	,	425	,	567	,	1663	,	195	,	1763	,	13271	,	1739	};
            const unsigned long	dim2360KuoInit[]		=	{	1	,	1	,	7	,	9	,	9	,	33	,	11	,	167	,	87	,	901	,	1625	,	2055	,	2475	,	7241	,	29717	};
            const unsigned long	dim2361KuoInit[]		=	{	1	,	1	,	1	,	1	,	15	,	63	,	127	,	29	,	227	,	817	,	637	,	1873	,	4885	,	16299	,	31243	};
            const unsigned long	dim2362KuoInit[]		=	{	1	,	3	,	7	,	3	,	17	,	27	,	81	,	237	,	283	,	161	,	1905	,	1551	,	6171	,	4519	,	11047	};
            const unsigned long	dim2363KuoInit[]		=	{	1	,	1	,	7	,	9	,	25	,	23	,	25	,	1	,	461	,	623	,	493	,	827	,	5795	,	16065	,	8401	};
            const unsigned long	dim2364KuoInit[]		=	{	1	,	3	,	5	,	11	,	29	,	29	,	91	,	133	,	91	,	565	,	1821	,	269	,	1731	,	1499	,	7975	};
            const unsigned long	dim2365KuoInit[]		=	{	1	,	1	,	1	,	13	,	3	,	41	,	99	,	23	,	239	,	945	,	219	,	1127	,	4919	,	9713	,	3321	};
            const unsigned long	dim2366KuoInit[]		=	{	1	,	3	,	1	,	11	,	25	,	59	,	59	,	127	,	83	,	643	,	881	,	1671	,	3521	,	459	,	15479	};
            const unsigned long	dim2367KuoInit[]		=	{	1	,	1	,	5	,	11	,	15	,	23	,	111	,	225	,	145	,	703	,	1643	,	419	,	4853	,	7195	,	20879	};
            const unsigned long	dim2368KuoInit[]		=	{	1	,	3	,	7	,	15	,	5	,	31	,	111	,	151	,	489	,	629	,	473	,	1507	,	117	,	15305	,	11011	};
            const unsigned long	dim2369KuoInit[]		=	{	1	,	1	,	3	,	3	,	1	,	55	,	55	,	223	,	489	,	911	,	1721	,	3533	,	5893	,	2891	,	8929	};
            const unsigned long	dim2370KuoInit[]		=	{	1	,	1	,	5	,	13	,	7	,	43	,	113	,	99	,	19	,	681	,	1765	,	1983	,	5477	,	7531	,	26507	};
            const unsigned long	dim2371KuoInit[]		=	{	1	,	1	,	3	,	1	,	9	,	7	,	77	,	231	,	345	,	745	,	285	,	3213	,	2611	,	4449	,	24621	};
            const unsigned long	dim2372KuoInit[]		=	{	1	,	1	,	5	,	1	,	21	,	21	,	3	,	11	,	131	,	327	,	1893	,	33	,	6947	,	337	,	8877	};
            const unsigned long	dim2373KuoInit[]		=	{	1	,	1	,	5	,	13	,	5	,	11	,	41	,	7	,	447	,	625	,	1231	,	3621	,	3785	,	12351	,	13829	};
            const unsigned long	dim2374KuoInit[]		=	{	1	,	3	,	1	,	13	,	29	,	41	,	11	,	63	,	357	,	723	,	1657	,	91	,	1373	,	6383	,	8327	};
            const unsigned long	dim2375KuoInit[]		=	{	1	,	3	,	3	,	15	,	23	,	51	,	85	,	113	,	103	,	499	,	1669	,	3685	,	63	,	2853	,	20859	};
            const unsigned long	dim2376KuoInit[]		=	{	1	,	1	,	5	,	11	,	31	,	39	,	15	,	103	,	365	,	231	,	443	,	3213	,	6951	,	1715	,	32301	};
            const unsigned long	dim2377KuoInit[]		=	{	1	,	3	,	7	,	5	,	19	,	23	,	13	,	127	,	475	,	281	,	1095	,	473	,	4955	,	2217	,	10427	};
            const unsigned long	dim2378KuoInit[]		=	{	1	,	1	,	5	,	13	,	17	,	41	,	47	,	121	,	131	,	69	,	1747	,	3679	,	2765	,	4349	,	32041	};
            const unsigned long	dim2379KuoInit[]		=	{	1	,	1	,	7	,	9	,	3	,	47	,	11	,	127	,	321	,	669	,	1283	,	797	,	6393	,	12351	,	5161	};
            const unsigned long	dim2380KuoInit[]		=	{	1	,	3	,	7	,	15	,	21	,	37	,	73	,	15	,	393	,	173	,	1461	,	1437	,	2367	,	7171	,	15123	};
            const unsigned long	dim2381KuoInit[]		=	{	1	,	3	,	5	,	15	,	7	,	57	,	29	,	83	,	241	,	221	,	1607	,	1291	,	4839	,	11293	,	1837	};
            const unsigned long	dim2382KuoInit[]		=	{	1	,	3	,	5	,	7	,	1	,	31	,	59	,	143	,	487	,	317	,	535	,	1505	,	3705	,	9947	,	6149	};
            const unsigned long	dim2383KuoInit[]		=	{	1	,	3	,	1	,	3	,	29	,	31	,	69	,	63	,	183	,	173	,	1965	,	3331	,	4473	,	993	,	4937	};
            const unsigned long	dim2384KuoInit[]		=	{	1	,	1	,	1	,	15	,	13	,	13	,	55	,	43	,	283	,	897	,	1019	,	2761	,	3237	,	14661	,	12309	};
            const unsigned long	dim2385KuoInit[]		=	{	1	,	3	,	1	,	15	,	9	,	15	,	1	,	19	,	227	,	777	,	1713	,	3471	,	2917	,	1117	,	16397	};
            const unsigned long	dim2386KuoInit[]		=	{	1	,	1	,	1	,	1	,	21	,	19	,	49	,	3	,	477	,	659	,	61	,	2373	,	6363	,	13771	,	5675	};
            const unsigned long	dim2387KuoInit[]		=	{	1	,	3	,	3	,	3	,	13	,	23	,	43	,	253	,	483	,	975	,	1629	,	19	,	6339	,	13963	,	6065	};
            const unsigned long	dim2388KuoInit[]		=	{	1	,	3	,	7	,	3	,	9	,	31	,	65	,	1	,	311	,	67	,	293	,	667	,	3685	,	3589	,	25251	};
            const unsigned long	dim2389KuoInit[]		=	{	1	,	1	,	3	,	1	,	29	,	31	,	47	,	85	,	431	,	131	,	1495	,	1271	,	6121	,	7183	,	20477	};
            const unsigned long	dim2390KuoInit[]		=	{	1	,	1	,	7	,	5	,	13	,	37	,	83	,	201	,	277	,	643	,	19	,	2519	,	2589	,	2471	,	19727	};
            const unsigned long	dim2391KuoInit[]		=	{	1	,	3	,	5	,	13	,	27	,	13	,	97	,	183	,	463	,	639	,	87	,	181	,	747	,	3715	,	16207	};
            const unsigned long	dim2392KuoInit[]		=	{	1	,	1	,	7	,	3	,	9	,	29	,	21	,	191	,	485	,	211	,	541	,	69	,	511	,	9923	,	24543	};
            const unsigned long	dim2393KuoInit[]		=	{	1	,	3	,	5	,	9	,	19	,	43	,	85	,	189	,	47	,	723	,	1211	,	1259	,	1645	,	2983	,	27195	};
            const unsigned long	dim2394KuoInit[]		=	{	1	,	1	,	7	,	15	,	17	,	25	,	103	,	83	,	389	,	229	,	1457	,	4045	,	161	,	15471	,	19929	};
            const unsigned long	dim2395KuoInit[]		=	{	1	,	1	,	3	,	9	,	5	,	27	,	55	,	231	,	245	,	723	,	479	,	3761	,	787	,	12939	,	30227	};
            const unsigned long	dim2396KuoInit[]		=	{	1	,	3	,	3	,	1	,	19	,	39	,	63	,	27	,	137	,	399	,	1353	,	1143	,	5835	,	2013	,	4107	};
            const unsigned long	dim2397KuoInit[]		=	{	1	,	3	,	5	,	7	,	5	,	39	,	45	,	207	,	7	,	131	,	961	,	1715	,	3761	,	11501	,	9803	};
            const unsigned long	dim2398KuoInit[]		=	{	1	,	3	,	1	,	7	,	17	,	11	,	23	,	45	,	417	,	289	,	1545	,	2585	,	1345	,	15529	,	8365	};
            const unsigned long	dim2399KuoInit[]		=	{	1	,	1	,	1	,	7	,	21	,	27	,	97	,	23	,	461	,	983	,	997	,	1321	,	2551	,	5485	,	23565	};
            const unsigned long	dim2400KuoInit[]		=	{	1	,	1	,	5	,	5	,	23	,	19	,	77	,	103	,	171	,	553	,	1767	,	3255	,	7955	,	249	,	32681	};
            const unsigned long	dim2401KuoInit[]		=	{	1	,	1	,	7	,	1	,	23	,	43	,	73	,	15	,	381	,	245	,	1389	,	3693	,	1975	,	13629	,	2079	};
            const unsigned long	dim2402KuoInit[]		=	{	1	,	1	,	7	,	11	,	23	,	61	,	79	,	39	,	345	,	541	,	875	,	1375	,	123	,	15305	,	13731	};
            const unsigned long	dim2403KuoInit[]		=	{	1	,	1	,	3	,	15	,	3	,	17	,	19	,	11	,	79	,	415	,	1791	,	447	,	6529	,	11243	,	10427	};
            const unsigned long	dim2404KuoInit[]		=	{	1	,	1	,	7	,	15	,	9	,	31	,	121	,	247	,	403	,	935	,	705	,	2921	,	3769	,	625	,	22905	};
            const unsigned long	dim2405KuoInit[]		=	{	1	,	3	,	5	,	15	,	19	,	9	,	5	,	47	,	135	,	585	,	1155	,	1517	,	4961	,	15725	,	7585	};
            const unsigned long	dim2406KuoInit[]		=	{	1	,	3	,	3	,	7	,	19	,	57	,	37	,	69	,	117	,	485	,	115	,	2709	,	1909	,	12781	,	19323	};
            const unsigned long	dim2407KuoInit[]		=	{	1	,	1	,	1	,	5	,	7	,	1	,	37	,	39	,	141	,	1017	,	993	,	191	,	6173	,	8083	,	9733	};
            const unsigned long	dim2408KuoInit[]		=	{	1	,	1	,	7	,	3	,	3	,	21	,	17	,	55	,	129	,	353	,	673	,	3233	,	6289	,	10955	,	19357	};
            const unsigned long	dim2409KuoInit[]		=	{	1	,	3	,	7	,	11	,	31	,	31	,	31	,	105	,	187	,	839	,	733	,	1795	,	5075	,	1053	,	613	};
            const unsigned long	dim2410KuoInit[]		=	{	1	,	3	,	3	,	1	,	9	,	9	,	23	,	139	,	417	,	431	,	1943	,	3067	,	267	,	4401	,	9847	};
            const unsigned long	dim2411KuoInit[]		=	{	1	,	3	,	5	,	11	,	23	,	61	,	61	,	119	,	383	,	869	,	503	,	195	,	1935	,	4617	,	21393	};
            const unsigned long	dim2412KuoInit[]		=	{	1	,	1	,	1	,	5	,	7	,	15	,	51	,	53	,	427	,	499	,	1829	,	2533	,	3029	,	12485	,	28425	};
            const unsigned long	dim2413KuoInit[]		=	{	1	,	3	,	3	,	7	,	9	,	11	,	55	,	205	,	255	,	619	,	1323	,	1983	,	15	,	4237	,	20983	};
            const unsigned long	dim2414KuoInit[]		=	{	1	,	1	,	3	,	15	,	15	,	37	,	109	,	247	,	179	,	1005	,	1111	,	77	,	4533	,	12849	,	25601	};
            const unsigned long	dim2415KuoInit[]		=	{	1	,	1	,	5	,	3	,	15	,	25	,	59	,	117	,	55	,	755	,	681	,	649	,	6041	,	1695	,	13719	};
            const unsigned long	dim2416KuoInit[]		=	{	1	,	3	,	1	,	3	,	15	,	17	,	117	,	113	,	197	,	701	,	773	,	1097	,	1267	,	16093	,	21555	};
            const unsigned long	dim2417KuoInit[]		=	{	1	,	3	,	1	,	5	,	9	,	57	,	39	,	171	,	449	,	507	,	29	,	3581	,	1929	,	12715	,	24575	};
            const unsigned long	dim2418KuoInit[]		=	{	1	,	3	,	5	,	5	,	23	,	1	,	67	,	153	,	375	,	333	,	379	,	3163	,	8121	,	13465	,	27641	};
            const unsigned long	dim2419KuoInit[]		=	{	1	,	3	,	5	,	5	,	31	,	27	,	83	,	243	,	415	,	997	,	1787	,	4021	,	5955	,	1861	,	10511	};
            const unsigned long	dim2420KuoInit[]		=	{	1	,	3	,	1	,	3	,	13	,	47	,	69	,	169	,	185	,	645	,	1857	,	3487	,	1557	,	9589	,	7971	};
            const unsigned long	dim2421KuoInit[]		=	{	1	,	3	,	3	,	15	,	3	,	1	,	93	,	97	,	71	,	433	,	1371	,	2495	,	8171	,	9101	,	32435	};
            const unsigned long	dim2422KuoInit[]		=	{	1	,	3	,	7	,	11	,	3	,	29	,	37	,	167	,	169	,	15	,	1575	,	1331	,	7163	,	1555	,	32439	};
            const unsigned long	dim2423KuoInit[]		=	{	1	,	1	,	5	,	1	,	29	,	11	,	83	,	135	,	367	,	37	,	1455	,	3939	,	3383	,	5291	,	14269	};
            const unsigned long	dim2424KuoInit[]		=	{	1	,	1	,	5	,	15	,	7	,	63	,	17	,	45	,	259	,	57	,	1933	,	3101	,	7871	,	12633	,	15789	};
            const unsigned long	dim2425KuoInit[]		=	{	1	,	1	,	1	,	7	,	19	,	37	,	89	,	159	,	487	,	101	,	1551	,	3703	,	4323	,	9153	,	6155	};
            const unsigned long	dim2426KuoInit[]		=	{	1	,	3	,	3	,	13	,	15	,	49	,	81	,	51	,	267	,	761	,	863	,	2239	,	7683	,	15491	,	28655	};
            const unsigned long	dim2427KuoInit[]		=	{	1	,	3	,	1	,	1	,	25	,	53	,	113	,	117	,	11	,	1003	,	539	,	3603	,	1007	,	11329	,	15099	};
            const unsigned long	dim2428KuoInit[]		=	{	1	,	3	,	5	,	7	,	9	,	27	,	125	,	231	,	301	,	461	,	2031	,	183	,	6361	,	10811	,	29425	};
            const unsigned long	dim2429KuoInit[]		=	{	1	,	1	,	3	,	1	,	3	,	37	,	65	,	127	,	487	,	611	,	867	,	2361	,	5243	,	11829	,	31297	};
            const unsigned long	dim2430KuoInit[]		=	{	1	,	3	,	3	,	15	,	9	,	39	,	31	,	219	,	77	,	97	,	1061	,	435	,	2449	,	14751	,	20273	};
            const unsigned long	dim2431KuoInit[]		=	{	1	,	1	,	5	,	11	,	1	,	23	,	41	,	239	,	183	,	287	,	969	,	1325	,	3553	,	6751	,	14679	};
            const unsigned long	dim2432KuoInit[]		=	{	1	,	3	,	7	,	1	,	17	,	61	,	47	,	207	,	509	,	601	,	1539	,	345	,	3723	,	5415	,	11667	};
            const unsigned long	dim2433KuoInit[]		=	{	1	,	1	,	7	,	7	,	23	,	33	,	25	,	165	,	345	,	569	,	983	,	2857	,	1563	,	1737	,	19285	};
            const unsigned long	dim2434KuoInit[]		=	{	1	,	1	,	1	,	7	,	3	,	43	,	107	,	229	,	223	,	583	,	1495	,	3487	,	7497	,	10367	,	20575	};
            const unsigned long	dim2435KuoInit[]		=	{	1	,	3	,	3	,	13	,	21	,	49	,	17	,	195	,	115	,	797	,	1489	,	1587	,	7847	,	11013	,	14547	};
            const unsigned long	dim2436KuoInit[]		=	{	1	,	3	,	5	,	11	,	9	,	29	,	19	,	245	,	153	,	779	,	1311	,	425	,	5809	,	4457	,	6905	};
            const unsigned long	dim2437KuoInit[]		=	{	1	,	1	,	5	,	3	,	21	,	33	,	13	,	193	,	343	,	971	,	1315	,	1989	,	1969	,	9227	,	8881	};
            const unsigned long	dim2438KuoInit[]		=	{	1	,	3	,	7	,	15	,	3	,	59	,	87	,	205	,	261	,	567	,	557	,	1143	,	6923	,	9451	,	20239	};
            const unsigned long	dim2439KuoInit[]		=	{	1	,	1	,	1	,	15	,	29	,	35	,	127	,	123	,	173	,	807	,	1279	,	2413	,	1899	,	11657	,	22257	};
            const unsigned long	dim2440KuoInit[]		=	{	1	,	3	,	5	,	3	,	23	,	35	,	125	,	129	,	239	,	951	,	499	,	1609	,	4643	,	339	,	26701	};
            const unsigned long	dim2441KuoInit[]		=	{	1	,	3	,	3	,	13	,	9	,	15	,	23	,	55	,	105	,	81	,	591	,	947	,	3761	,	7351	,	25697	};
            const unsigned long	dim2442KuoInit[]		=	{	1	,	1	,	7	,	13	,	23	,	17	,	127	,	149	,	227	,	1005	,	839	,	3391	,	5761	,	10431	,	31899	};
            const unsigned long	dim2443KuoInit[]		=	{	1	,	3	,	1	,	1	,	23	,	7	,	11	,	3	,	77	,	457	,	375	,	3823	,	5447	,	8339	,	18847	};
            const unsigned long	dim2444KuoInit[]		=	{	1	,	3	,	7	,	3	,	29	,	1	,	19	,	39	,	23	,	285	,	569	,	2979	,	7029	,	11311	,	18269	};
            const unsigned long	dim2445KuoInit[]		=	{	1	,	1	,	5	,	9	,	21	,	19	,	85	,	211	,	437	,	185	,	597	,	787	,	4555	,	2853	,	18493	};
            const unsigned long	dim2446KuoInit[]		=	{	1	,	3	,	3	,	9	,	11	,	27	,	25	,	245	,	55	,	869	,	323	,	3999	,	2639	,	9385	,	26823	};
            const unsigned long	dim2447KuoInit[]		=	{	1	,	3	,	1	,	7	,	19	,	61	,	51	,	111	,	115	,	721	,	893	,	23	,	6375	,	5101	,	25567	};
            const unsigned long	dim2448KuoInit[]		=	{	1	,	3	,	5	,	9	,	9	,	13	,	9	,	241	,	297	,	391	,	1529	,	3843	,	737	,	5897	,	23469	};
            const unsigned long	dim2449KuoInit[]		=	{	1	,	3	,	7	,	3	,	31	,	53	,	43	,	219	,	81	,	507	,	1627	,	3503	,	817	,	6533	,	31633	};
            const unsigned long	dim2450KuoInit[]		=	{	1	,	1	,	7	,	1	,	13	,	59	,	13	,	185	,	387	,	869	,	1013	,	2291	,	4945	,	11493	,	7785	};
            const unsigned long	dim2451KuoInit[]		=	{	1	,	3	,	5	,	9	,	31	,	35	,	11	,	225	,	235	,	893	,	1749	,	2393	,	5317	,	10069	,	8569	};
            const unsigned long	dim2452KuoInit[]		=	{	1	,	1	,	1	,	3	,	5	,	15	,	93	,	25	,	503	,	205	,	65	,	2563	,	7971	,	2917	,	19687	};
            const unsigned long	dim2453KuoInit[]		=	{	1	,	3	,	3	,	13	,	1	,	37	,	127	,	63	,	339	,	121	,	1895	,	1943	,	7527	,	15869	,	20533	};
            const unsigned long	dim2454KuoInit[]		=	{	1	,	1	,	5	,	7	,	19	,	25	,	87	,	27	,	279	,	419	,	1261	,	613	,	7687	,	5301	,	6761	};
            const unsigned long	dim2455KuoInit[]		=	{	1	,	1	,	1	,	1	,	13	,	33	,	59	,	235	,	471	,	811	,	695	,	2727	,	5615	,	6269	,	13469	};
            const unsigned long	dim2456KuoInit[]		=	{	1	,	3	,	1	,	15	,	17	,	23	,	123	,	71	,	15	,	1005	,	973	,	411	,	2627	,	1967	,	25597	};
            const unsigned long	dim2457KuoInit[]		=	{	1	,	3	,	5	,	1	,	3	,	61	,	71	,	217	,	377	,	613	,	1819	,	141	,	6265	,	9223	,	20635	};
            const unsigned long	dim2458KuoInit[]		=	{	1	,	1	,	5	,	11	,	15	,	1	,	39	,	121	,	83	,	57	,	1133	,	3165	,	7675	,	13715	,	11383	};
            const unsigned long	dim2459KuoInit[]		=	{	1	,	3	,	7	,	15	,	9	,	3	,	97	,	223	,	493	,	77	,	159	,	2921	,	5475	,	9289	,	20925	};
            const unsigned long	dim2460KuoInit[]		=	{	1	,	1	,	3	,	7	,	29	,	13	,	87	,	219	,	327	,	879	,	883	,	1637	,	3451	,	13939	,	8203	};
            const unsigned long	dim2461KuoInit[]		=	{	1	,	3	,	7	,	15	,	25	,	21	,	95	,	129	,	343	,	9	,	1415	,	2965	,	747	,	7297	,	24293	};
            const unsigned long	dim2462KuoInit[]		=	{	1	,	1	,	1	,	3	,	23	,	47	,	57	,	31	,	359	,	1017	,	769	,	2761	,	1933	,	6091	,	6475	};
            const unsigned long	dim2463KuoInit[]		=	{	1	,	1	,	5	,	9	,	27	,	45	,	125	,	67	,	145	,	581	,	157	,	2153	,	2727	,	8025	,	28165	};
            const unsigned long	dim2464KuoInit[]		=	{	1	,	3	,	5	,	9	,	7	,	53	,	27	,	161	,	343	,	139	,	1651	,	1275	,	2981	,	9027	,	31175	};
            const unsigned long	dim2465KuoInit[]		=	{	1	,	3	,	3	,	11	,	7	,	15	,	81	,	229	,	115	,	849	,	459	,	1651	,	3867	,	7029	,	6741	};
            const unsigned long	dim2466KuoInit[]		=	{	1	,	1	,	3	,	5	,	31	,	45	,	17	,	107	,	467	,	401	,	1473	,	1953	,	3779	,	11237	,	31257	};
            const unsigned long	dim2467KuoInit[]		=	{	1	,	3	,	3	,	11	,	3	,	27	,	45	,	217	,	133	,	515	,	1487	,	1427	,	1999	,	12979	,	3543	};
            const unsigned long	dim2468KuoInit[]		=	{	1	,	3	,	7	,	3	,	17	,	31	,	123	,	205	,	263	,	963	,	1697	,	2453	,	1249	,	603	,	8581	};
            const unsigned long	dim2469KuoInit[]		=	{	1	,	3	,	1	,	15	,	23	,	9	,	23	,	251	,	489	,	393	,	1103	,	3273	,	5703	,	3033	,	22631	};
            const unsigned long	dim2470KuoInit[]		=	{	1	,	3	,	3	,	13	,	23	,	29	,	49	,	233	,	461	,	795	,	991	,	3695	,	4715	,	7211	,	3695	};
            const unsigned long	dim2471KuoInit[]		=	{	1	,	3	,	3	,	15	,	21	,	11	,	33	,	69	,	347	,	535	,	25	,	3465	,	2825	,	9345	,	4485	};
            const unsigned long	dim2472KuoInit[]		=	{	1	,	3	,	7	,	3	,	23	,	47	,	65	,	209	,	357	,	363	,	1979	,	289	,	35	,	4459	,	17929	};
            const unsigned long	dim2473KuoInit[]		=	{	1	,	3	,	1	,	11	,	19	,	17	,	125	,	231	,	221	,	953	,	2041	,	2359	,	5925	,	15693	,	30859	};
            const unsigned long	dim2474KuoInit[]		=	{	1	,	1	,	3	,	13	,	31	,	11	,	11	,	215	,	335	,	153	,	1459	,	2303	,	3471	,	7415	,	3783	};
            const unsigned long	dim2475KuoInit[]		=	{	1	,	1	,	7	,	5	,	11	,	55	,	47	,	43	,	355	,	469	,	963	,	383	,	667	,	2233	,	27759	};
            const unsigned long	dim2476KuoInit[]		=	{	1	,	3	,	1	,	9	,	23	,	29	,	27	,	135	,	359	,	111	,	387	,	3021	,	817	,	4211	,	17243	};
            const unsigned long	dim2477KuoInit[]		=	{	1	,	1	,	3	,	9	,	25	,	27	,	63	,	89	,	195	,	359	,	413	,	3509	,	3831	,	4785	,	30097	};
            const unsigned long	dim2478KuoInit[]		=	{	1	,	1	,	5	,	5	,	11	,	29	,	29	,	25	,	445	,	643	,	261	,	3901	,	3019	,	14213	,	16115	};
            const unsigned long	dim2479KuoInit[]		=	{	1	,	3	,	3	,	15	,	21	,	31	,	7	,	255	,	207	,	535	,	1033	,	1017	,	3791	,	11455	,	31701	};
            const unsigned long	dim2480KuoInit[]		=	{	1	,	3	,	5	,	13	,	15	,	37	,	79	,	205	,	439	,	609	,	1433	,	3333	,	6583	,	9689	,	14979	};
            const unsigned long	dim2481KuoInit[]		=	{	1	,	1	,	7	,	3	,	31	,	27	,	53	,	101	,	427	,	773	,	1877	,	1715	,	6177	,	11851	,	17989	};
            const unsigned long	dim2482KuoInit[]		=	{	1	,	3	,	1	,	3	,	21	,	11	,	101	,	203	,	49	,	543	,	1979	,	1469	,	6701	,	9963	,	7793	};
            const unsigned long	dim2483KuoInit[]		=	{	1	,	1	,	3	,	1	,	15	,	47	,	123	,	141	,	433	,	783	,	211	,	1489	,	4983	,	6417	,	29767	};
            const unsigned long	dim2484KuoInit[]		=	{	1	,	1	,	5	,	3	,	1	,	41	,	109	,	13	,	465	,	959	,	431	,	543	,	4681	,	8573	,	7889	};
            const unsigned long	dim2485KuoInit[]		=	{	1	,	1	,	1	,	3	,	15	,	43	,	15	,	63	,	145	,	859	,	939	,	629	,	7077	,	5617	,	16779	};
            const unsigned long	dim2486KuoInit[]		=	{	1	,	3	,	3	,	1	,	23	,	63	,	67	,	139	,	219	,	257	,	943	,	1501	,	827	,	10855	,	10035	};
            const unsigned long	dim2487KuoInit[]		=	{	1	,	3	,	1	,	15	,	9	,	47	,	27	,	119	,	231	,	767	,	999	,	3553	,	7683	,	5399	,	11553	};
            const unsigned long	dim2488KuoInit[]		=	{	1	,	3	,	3	,	11	,	15	,	13	,	35	,	139	,	463	,	985	,	553	,	3997	,	2935	,	7707	,	2511	};
            const unsigned long	dim2489KuoInit[]		=	{	1	,	1	,	3	,	11	,	23	,	53	,	103	,	1	,	409	,	853	,	737	,	2077	,	523	,	1839	,	32145	};
            const unsigned long	dim2490KuoInit[]		=	{	1	,	3	,	1	,	15	,	23	,	7	,	77	,	185	,	371	,	447	,	499	,	569	,	6647	,	501	,	27649	};
            const unsigned long	dim2491KuoInit[]		=	{	1	,	1	,	3	,	5	,	19	,	31	,	119	,	157	,	385	,	427	,	1411	,	4031	,	4199	,	14567	,	8133	};
            const unsigned long	dim2492KuoInit[]		=	{	1	,	3	,	7	,	5	,	31	,	3	,	123	,	149	,	209	,	467	,	1619	,	879	,	5949	,	7295	,	25165	};
            const unsigned long	dim2493KuoInit[]		=	{	1	,	3	,	1	,	15	,	31	,	47	,	111	,	129	,	359	,	385	,	463	,	3363	,	6823	,	10751	,	4473	};
            const unsigned long	dim2494KuoInit[]		=	{	1	,	3	,	3	,	7	,	5	,	39	,	71	,	45	,	439	,	161	,	597	,	2093	,	1725	,	11285	,	18149	};
            const unsigned long	dim2495KuoInit[]		=	{	1	,	3	,	3	,	7	,	21	,	55	,	115	,	199	,	339	,	425	,	71	,	305	,	1161	,	3359	,	16835	};
            const unsigned long	dim2496KuoInit[]		=	{	1	,	1	,	7	,	7	,	1	,	53	,	51	,	31	,	57	,	581	,	147	,	903	,	7269	,	1461	,	7417	};
            const unsigned long	dim2497KuoInit[]		=	{	1	,	3	,	7	,	3	,	17	,	31	,	65	,	195	,	47	,	575	,	1877	,	1603	,	3043	,	5385	,	15177	};
            const unsigned long	dim2498KuoInit[]		=	{	1	,	1	,	3	,	11	,	15	,	7	,	121	,	51	,	67	,	391	,	1553	,	1817	,	6503	,	2907	,	3611	};
            const unsigned long	dim2499KuoInit[]		=	{	1	,	1	,	1	,	9	,	13	,	51	,	95	,	19	,	315	,	549	,	1417	,	2073	,	485	,	5257	,	32419	};
            const unsigned long	dim2500KuoInit[]		=	{	1	,	3	,	1	,	9	,	31	,	63	,	107	,	103	,	173	,	681	,	211	,	3283	,	2413	,	2073	,	24497	};
            const unsigned long	dim2501KuoInit[]		=	{	1	,	3	,	1	,	7	,	3	,	41	,	49	,	81	,	289	,	177	,	699	,	1305	,	7867	,	4699	,	27669	};
            const unsigned long	dim2502KuoInit[]		=	{	1	,	1	,	5	,	3	,	7	,	15	,	73	,	57	,	319	,	959	,	701	,	2071	,	1587	,	4701	,	2595	};
            const unsigned long	dim2503KuoInit[]		=	{	1	,	1	,	5	,	7	,	7	,	1	,	29	,	129	,	361	,	619	,	1405	,	2185	,	6631	,	3505	,	391	};
            const unsigned long	dim2504KuoInit[]		=	{	1	,	3	,	3	,	5	,	19	,	9	,	31	,	63	,	149	,	181	,	299	,	2755	,	1511	,	5745	,	20323	};
            const unsigned long	dim2505KuoInit[]		=	{	1	,	1	,	3	,	11	,	23	,	5	,	29	,	175	,	119	,	25	,	319	,	835	,	1339	,	5429	,	7175	};
            const unsigned long	dim2506KuoInit[]		=	{	1	,	1	,	3	,	3	,	31	,	57	,	55	,	181	,	473	,	229	,	309	,	1567	,	5063	,	9745	,	3713	};
            const unsigned long	dim2507KuoInit[]		=	{	1	,	3	,	3	,	13	,	1	,	15	,	23	,	175	,	71	,	53	,	1749	,	743	,	285	,	49	,	7389	};
            const unsigned long	dim2508KuoInit[]		=	{	1	,	3	,	1	,	5	,	7	,	39	,	75	,	177	,	259	,	975	,	257	,	1497	,	1565	,	6973	,	3827	};
            const unsigned long	dim2509KuoInit[]		=	{	1	,	3	,	7	,	13	,	1	,	39	,	43	,	213	,	291	,	941	,	613	,	3041	,	1859	,	10755	,	29831	};
            const unsigned long	dim2510KuoInit[]		=	{	1	,	3	,	3	,	9	,	17	,	43	,	15	,	135	,	473	,	989	,	429	,	3287	,	3699	,	14349	,	16311	};
            const unsigned long	dim2511KuoInit[]		=	{	1	,	3	,	1	,	15	,	1	,	51	,	59	,	203	,	383	,	731	,	171	,	1259	,	5833	,	12029	,	18099	};
            const unsigned long	dim2512KuoInit[]		=	{	1	,	3	,	1	,	3	,	19	,	63	,	125	,	57	,	429	,	93	,	1179	,	2707	,	449	,	2213	,	4759	};
            const unsigned long	dim2513KuoInit[]		=	{	1	,	1	,	7	,	3	,	19	,	11	,	107	,	127	,	359	,	759	,	503	,	761	,	6817	,	15343	,	13777	};
            const unsigned long	dim2514KuoInit[]		=	{	1	,	1	,	1	,	7	,	21	,	49	,	67	,	179	,	241	,	403	,	769	,	3341	,	1845	,	2855	,	3825	};
            const unsigned long	dim2515KuoInit[]		=	{	1	,	3	,	3	,	3	,	7	,	29	,	71	,	35	,	389	,	297	,	673	,	1353	,	6781	,	5633	,	12459	};
            const unsigned long	dim2516KuoInit[]		=	{	1	,	1	,	1	,	15	,	13	,	15	,	11	,	57	,	143	,	35	,	809	,	2295	,	6497	,	455	,	23689	};
            const unsigned long	dim2517KuoInit[]		=	{	1	,	3	,	7	,	15	,	23	,	39	,	39	,	211	,	335	,	537	,	1569	,	3989	,	5777	,	12951	,	22841	};
            const unsigned long	dim2518KuoInit[]		=	{	1	,	1	,	7	,	3	,	23	,	17	,	107	,	237	,	241	,	533	,	57	,	3421	,	263	,	889	,	28841	};
            const unsigned long	dim2519KuoInit[]		=	{	1	,	1	,	5	,	1	,	11	,	29	,	17	,	141	,	431	,	809	,	1227	,	2835	,	2955	,	11823	,	23061	};
            const unsigned long	dim2520KuoInit[]		=	{	1	,	1	,	3	,	9	,	5	,	51	,	73	,	43	,	75	,	985	,	1665	,	1369	,	519	,	6379	,	8665	};
            const unsigned long	dim2521KuoInit[]		=	{	1	,	3	,	3	,	13	,	17	,	21	,	9	,	173	,	333	,	689	,	277	,	1755	,	5173	,	7651	,	30597	};
            const unsigned long	dim2522KuoInit[]		=	{	1	,	3	,	5	,	9	,	3	,	59	,	59	,	221	,	177	,	819	,	1483	,	1945	,	4725	,	4821	,	3709	};
            const unsigned long	dim2523KuoInit[]		=	{	1	,	1	,	7	,	7	,	19	,	35	,	93	,	47	,	49	,	861	,	561	,	753	,	8047	,	419	,	4335	};
            const unsigned long	dim2524KuoInit[]		=	{	1	,	3	,	3	,	15	,	31	,	3	,	97	,	177	,	443	,	55	,	1541	,	2627	,	6129	,	4165	,	3079	};
            const unsigned long	dim2525KuoInit[]		=	{	1	,	1	,	1	,	9	,	13	,	13	,	13	,	29	,	309	,	679	,	1893	,	3629	,	513	,	16333	,	25371	};
            const unsigned long	dim2526KuoInit[]		=	{	1	,	3	,	1	,	5	,	3	,	23	,	85	,	169	,	143	,	537	,	1387	,	3997	,	3659	,	12107	,	11689	};
            const unsigned long	dim2527KuoInit[]		=	{	1	,	1	,	1	,	5	,	9	,	7	,	11	,	59	,	325	,	855	,	1853	,	855	,	5743	,	7625	,	10381	};
            const unsigned long	dim2528KuoInit[]		=	{	1	,	3	,	3	,	3	,	5	,	37	,	109	,	101	,	243	,	301	,	1849	,	1633	,	4451	,	8859	,	22017	};
            const unsigned long	dim2529KuoInit[]		=	{	1	,	3	,	5	,	1	,	7	,	1	,	9	,	1	,	411	,	941	,	153	,	2709	,	389	,	9403	,	24103	};
            const unsigned long	dim2530KuoInit[]		=	{	1	,	1	,	1	,	15	,	17	,	19	,	105	,	127	,	271	,	543	,	449	,	1385	,	7387	,	8335	,	741	};
            const unsigned long	dim2531KuoInit[]		=	{	1	,	3	,	7	,	5	,	23	,	21	,	43	,	17	,	357	,	499	,	649	,	1121	,	7655	,	15623	,	12975	};
            const unsigned long	dim2532KuoInit[]		=	{	1	,	1	,	5	,	5	,	1	,	57	,	37	,	91	,	159	,	1013	,	653	,	3371	,	4857	,	441	,	17729	};
            const unsigned long	dim2533KuoInit[]		=	{	1	,	3	,	7	,	15	,	15	,	31	,	71	,	77	,	113	,	47	,	1597	,	43	,	1139	,	3191	,	31775	};
            const unsigned long	dim2534KuoInit[]		=	{	1	,	1	,	1	,	13	,	19	,	39	,	93	,	15	,	433	,	867	,	1091	,	2333	,	6113	,	13005	,	13309	};
            const unsigned long	dim2535KuoInit[]		=	{	1	,	1	,	1	,	9	,	5	,	57	,	21	,	7	,	267	,	79	,	1829	,	43	,	211	,	2795	,	18739	};
            const unsigned long	dim2536KuoInit[]		=	{	1	,	1	,	5	,	13	,	25	,	59	,	93	,	213	,	23	,	281	,	625	,	439	,	1449	,	2259	,	31713	};
            const unsigned long	dim2537KuoInit[]		=	{	1	,	1	,	1	,	3	,	9	,	13	,	41	,	171	,	365	,	907	,	395	,	3465	,	6433	,	33	,	17191	};
            const unsigned long	dim2538KuoInit[]		=	{	1	,	1	,	1	,	5	,	3	,	41	,	45	,	55	,	413	,	579	,	1215	,	3983	,	1201	,	3019	,	16401	};
            const unsigned long	dim2539KuoInit[]		=	{	1	,	1	,	3	,	3	,	9	,	25	,	111	,	33	,	359	,	427	,	63	,	4005	,	2753	,	15063	,	1925	};
            const unsigned long	dim2540KuoInit[]		=	{	1	,	1	,	5	,	3	,	21	,	55	,	15	,	5	,	121	,	867	,	1645	,	2387	,	1167	,	13865	,	15899	};
            const unsigned long	dim2541KuoInit[]		=	{	1	,	3	,	5	,	15	,	13	,	49	,	9	,	173	,	291	,	517	,	143	,	399	,	7545	,	7225	,	29753	};
            const unsigned long	dim2542KuoInit[]		=	{	1	,	1	,	3	,	5	,	29	,	23	,	105	,	165	,	35	,	325	,	983	,	215	,	2537	,	8661	,	1715	};
            const unsigned long	dim2543KuoInit[]		=	{	1	,	3	,	7	,	7	,	19	,	1	,	91	,	77	,	311	,	715	,	347	,	1705	,	1045	,	15003	,	10961	};
            const unsigned long	dim2544KuoInit[]		=	{	1	,	1	,	1	,	15	,	5	,	3	,	73	,	159	,	113	,	201	,	257	,	181	,	551	,	1189	,	30719	};
            const unsigned long	dim2545KuoInit[]		=	{	1	,	1	,	1	,	3	,	15	,	57	,	69	,	85	,	133	,	735	,	429	,	2087	,	1453	,	14277	,	15245	};
            const unsigned long	dim2546KuoInit[]		=	{	1	,	3	,	1	,	1	,	27	,	5	,	27	,	247	,	401	,	841	,	1723	,	3717	,	3545	,	3123	,	27441	};
            const unsigned long	dim2547KuoInit[]		=	{	1	,	3	,	1	,	15	,	5	,	11	,	97	,	117	,	331	,	843	,	1101	,	2977	,	5123	,	5761	,	2019	};
            const unsigned long	dim2548KuoInit[]		=	{	1	,	3	,	1	,	5	,	31	,	25	,	53	,	191	,	265	,	961	,	1695	,	1847	,	6377	,	13261	,	8853	};
            const unsigned long	dim2549KuoInit[]		=	{	1	,	3	,	1	,	5	,	5	,	29	,	85	,	3	,	127	,	407	,	1511	,	549	,	4015	,	15327	,	9167	};
            const unsigned long	dim2550KuoInit[]		=	{	1	,	3	,	5	,	3	,	9	,	11	,	1	,	217	,	53	,	201	,	1237	,	2663	,	7429	,	7349	,	5619	};
            const unsigned long	dim2551KuoInit[]		=	{	1	,	3	,	3	,	15	,	13	,	63	,	21	,	173	,	59	,	881	,	351	,	1285	,	7155	,	6665	,	25117	};
            const unsigned long	dim2552KuoInit[]		=	{	1	,	1	,	3	,	3	,	3	,	43	,	103	,	227	,	397	,	75	,	1211	,	2959	,	3507	,	3105	,	21739	};
            const unsigned long	dim2553KuoInit[]		=	{	1	,	1	,	3	,	11	,	1	,	1	,	5	,	3	,	41	,	243	,	1317	,	2205	,	4079	,	7447	,	22493	};
            const unsigned long	dim2554KuoInit[]		=	{	1	,	3	,	1	,	3	,	7	,	39	,	23	,	171	,	263	,	375	,	805	,	2309	,	7123	,	4935	,	4517	};
            const unsigned long	dim2555KuoInit[]		=	{	1	,	1	,	5	,	15	,	5	,	47	,	35	,	15	,	131	,	875	,	1771	,	3001	,	951	,	14269	,	6395	};
            const unsigned long	dim2556KuoInit[]		=	{	1	,	1	,	3	,	9	,	27	,	1	,	59	,	247	,	295	,	229	,	309	,	213	,	6661	,	10415	,	17855	};
            const unsigned long	dim2557KuoInit[]		=	{	1	,	3	,	5	,	1	,	15	,	61	,	47	,	175	,	279	,	221	,	1665	,	3475	,	4171	,	15515	,	30835	};
            const unsigned long	dim2558KuoInit[]		=	{	1	,	3	,	3	,	15	,	29	,	21	,	61	,	123	,	57	,	839	,	399	,	3069	,	4617	,	7051	,	3245	};
            const unsigned long	dim2559KuoInit[]		=	{	1	,	3	,	7	,	11	,	27	,	57	,	83	,	163	,	499	,	901	,	855	,	2049	,	2993	,	10323	,	3391	};
            const unsigned long	dim2560KuoInit[]		=	{	1	,	1	,	7	,	5	,	17	,	5	,	23	,	51	,	445	,	639	,	811	,	139	,	2863	,	9447	,	25971	};
            const unsigned long	dim2561KuoInit[]		=	{	1	,	3	,	3	,	13	,	31	,	43	,	75	,	251	,	9	,	239	,	1573	,	871	,	2211	,	15057	,	879	};
            const unsigned long	dim2562KuoInit[]		=	{	1	,	1	,	5	,	11	,	23	,	39	,	69	,	79	,	335	,	729	,	1807	,	1497	,	2363	,	8713	,	3987	};
            const unsigned long	dim2563KuoInit[]		=	{	1	,	3	,	5	,	11	,	27	,	9	,	1	,	5	,	207	,	217	,	1097	,	2737	,	6815	,	9969	,	21731	};
            const unsigned long	dim2564KuoInit[]		=	{	1	,	3	,	3	,	15	,	9	,	33	,	71	,	85	,	15	,	323	,	1955	,	1299	,	4439	,	6413	,	23055	};
            const unsigned long	dim2565KuoInit[]		=	{	1	,	3	,	5	,	13	,	15	,	11	,	75	,	201	,	5	,	69	,	1067	,	2287	,	8099	,	11293	,	22547	};
            const unsigned long	dim2566KuoInit[]		=	{	1	,	3	,	1	,	7	,	19	,	1	,	63	,	195	,	463	,	463	,	1741	,	915	,	1289	,	11275	,	16383	};
            const unsigned long	dim2567KuoInit[]		=	{	1	,	1	,	7	,	3	,	3	,	53	,	51	,	95	,	389	,	385	,	1621	,	955	,	137	,	8523	,	24087	};
            const unsigned long	dim2568KuoInit[]		=	{	1	,	3	,	5	,	11	,	25	,	27	,	49	,	27	,	459	,	979	,	277	,	2917	,	6639	,	3471	,	20407	};
            const unsigned long	dim2569KuoInit[]		=	{	1	,	1	,	5	,	3	,	19	,	25	,	75	,	47	,	395	,	269	,	1445	,	287	,	4563	,	8817	,	10645	};
            const unsigned long	dim2570KuoInit[]		=	{	1	,	1	,	1	,	15	,	5	,	9	,	59	,	177	,	463	,	683	,	1917	,	1189	,	2423	,	6967	,	433	};
            const unsigned long	dim2571KuoInit[]		=	{	1	,	3	,	5	,	7	,	11	,	53	,	101	,	81	,	89	,	419	,	1605	,	3719	,	5449	,	4303	,	11235	};
            const unsigned long	dim2572KuoInit[]		=	{	1	,	1	,	7	,	15	,	7	,	55	,	5	,	133	,	273	,	757	,	313	,	3451	,	7837	,	14543	,	1607	};
            const unsigned long	dim2573KuoInit[]		=	{	1	,	3	,	7	,	11	,	9	,	13	,	103	,	95	,	301	,	637	,	1951	,	2577	,	2571	,	7881	,	27057	};
            const unsigned long	dim2574KuoInit[]		=	{	1	,	3	,	3	,	9	,	25	,	17	,	119	,	11	,	7	,	141	,	1219	,	3091	,	1737	,	16355	,	2807	};
            const unsigned long	dim2575KuoInit[]		=	{	1	,	1	,	1	,	1	,	3	,	55	,	59	,	193	,	23	,	643	,	159	,	3393	,	2671	,	14737	,	21193	};
            const unsigned long	dim2576KuoInit[]		=	{	1	,	3	,	5	,	9	,	9	,	11	,	109	,	227	,	333	,	861	,	915	,	3239	,	6199	,	9555	,	28563	};
            const unsigned long	dim2577KuoInit[]		=	{	1	,	3	,	3	,	9	,	1	,	33	,	51	,	169	,	291	,	159	,	1983	,	173	,	4133	,	11477	,	31525	};
            const unsigned long	dim2578KuoInit[]		=	{	1	,	3	,	5	,	5	,	31	,	39	,	47	,	195	,	421	,	949	,	311	,	1537	,	8009	,	6273	,	10893	};
            const unsigned long	dim2579KuoInit[]		=	{	1	,	1	,	1	,	1	,	17	,	19	,	37	,	211	,	245	,	553	,	769	,	2131	,	2819	,	16119	,	25851	};
            const unsigned long	dim2580KuoInit[]		=	{	1	,	3	,	3	,	13	,	7	,	3	,	63	,	141	,	465	,	367	,	949	,	3997	,	5217	,	3425	,	31161	};
            const unsigned long	dim2581KuoInit[]		=	{	1	,	3	,	3	,	5	,	19	,	35	,	91	,	115	,	153	,	127	,	231	,	3723	,	7911	,	4421	,	2271	};
            const unsigned long	dim2582KuoInit[]		=	{	1	,	3	,	5	,	9	,	17	,	57	,	49	,	151	,	263	,	89	,	1635	,	2991	,	7011	,	13243	,	1473	};
            const unsigned long	dim2583KuoInit[]		=	{	1	,	1	,	1	,	15	,	21	,	59	,	53	,	179	,	113	,	839	,	649	,	2083	,	7015	,	5507	,	9539	};
            const unsigned long	dim2584KuoInit[]		=	{	1	,	1	,	1	,	1	,	13	,	49	,	97	,	81	,	255	,	973	,	275	,	2005	,	5493	,	11019	,	14427	};
            const unsigned long	dim2585KuoInit[]		=	{	1	,	1	,	7	,	5	,	5	,	53	,	27	,	197	,	269	,	699	,	285	,	3725	,	7681	,	3767	,	11881	};
            const unsigned long	dim2586KuoInit[]		=	{	1	,	1	,	1	,	13	,	15	,	15	,	11	,	245	,	419	,	559	,	727	,	1545	,	1821	,	5655	,	26971	};
            const unsigned long	dim2587KuoInit[]		=	{	1	,	1	,	3	,	3	,	5	,	1	,	77	,	211	,	139	,	949	,	1945	,	3327	,	6699	,	6227	,	9769	};
            const unsigned long	dim2588KuoInit[]		=	{	1	,	3	,	1	,	1	,	3	,	9	,	113	,	17	,	283	,	939	,	1941	,	19	,	7431	,	14503	,	24541	};
            const unsigned long	dim2589KuoInit[]		=	{	1	,	1	,	1	,	3	,	1	,	57	,	17	,	85	,	371	,	491	,	775	,	2093	,	5615	,	4977	,	30995	};
            const unsigned long	dim2590KuoInit[]		=	{	1	,	1	,	3	,	3	,	1	,	61	,	75	,	163	,	307	,	527	,	1821	,	1291	,	5761	,	9983	,	4647	};
            const unsigned long	dim2591KuoInit[]		=	{	1	,	1	,	1	,	13	,	7	,	61	,	9	,	203	,	255	,	293	,	1683	,	1081	,	3179	,	13283	,	13759	};
            const unsigned long	dim2592KuoInit[]		=	{	1	,	1	,	5	,	5	,	5	,	25	,	23	,	97	,	37	,	277	,	1787	,	2709	,	6201	,	8041	,	2301	};
            const unsigned long	dim2593KuoInit[]		=	{	1	,	3	,	1	,	1	,	21	,	57	,	33	,	13	,	307	,	427	,	1577	,	2043	,	8131	,	11723	,	14353	};
            const unsigned long	dim2594KuoInit[]		=	{	1	,	1	,	3	,	5	,	19	,	39	,	63	,	11	,	139	,	173	,	517	,	1597	,	3871	,	15533	,	4123	};
            const unsigned long	dim2595KuoInit[]		=	{	1	,	3	,	1	,	11	,	19	,	43	,	123	,	127	,	237	,	599	,	893	,	3645	,	947	,	12565	,	9367	};
            const unsigned long	dim2596KuoInit[]		=	{	1	,	1	,	7	,	11	,	27	,	9	,	119	,	251	,	499	,	777	,	1667	,	1699	,	3431	,	2727	,	7097	};
            const unsigned long	dim2597KuoInit[]		=	{	1	,	1	,	1	,	11	,	7	,	25	,	95	,	143	,	319	,	659	,	1567	,	1391	,	5149	,	15177	,	6583	};
            const unsigned long	dim2598KuoInit[]		=	{	1	,	3	,	1	,	13	,	7	,	39	,	125	,	219	,	405	,	739	,	1267	,	3065	,	7529	,	3195	,	21889	};
            const unsigned long	dim2599KuoInit[]		=	{	1	,	3	,	3	,	9	,	27	,	13	,	87	,	129	,	451	,	997	,	103	,	1667	,	545	,	6341	,	3105	};
            const unsigned long	dim2600KuoInit[]		=	{	1	,	1	,	5	,	3	,	11	,	43	,	95	,	205	,	445	,	919	,	1353	,	2435	,	269	,	3221	,	8983	};
            const unsigned long	dim2601KuoInit[]		=	{	1	,	1	,	3	,	1	,	5	,	47	,	15	,	127	,	47	,	89	,	1409	,	1935	,	2047	,	10629	,	4867	};
            const unsigned long	dim2602KuoInit[]		=	{	1	,	1	,	5	,	9	,	21	,	51	,	31	,	175	,	337	,	643	,	1233	,	1049	,	569	,	3383	,	18771	};
            const unsigned long	dim2603KuoInit[]		=	{	1	,	1	,	7	,	11	,	7	,	7	,	29	,	139	,	65	,	621	,	363	,	513	,	4149	,	5225	,	11595	};
            const unsigned long	dim2604KuoInit[]		=	{	1	,	1	,	7	,	7	,	11	,	31	,	43	,	223	,	249	,	661	,	123	,	2725	,	5179	,	13237	,	10483	};
            const unsigned long	dim2605KuoInit[]		=	{	1	,	1	,	5	,	3	,	5	,	35	,	121	,	193	,	33	,	343	,	413	,	1879	,	8013	,	11161	,	7945	};
            const unsigned long	dim2606KuoInit[]		=	{	1	,	3	,	7	,	5	,	27	,	43	,	123	,	25	,	45	,	279	,	979	,	1121	,	1041	,	3757	,	22217	};
            const unsigned long	dim2607KuoInit[]		=	{	1	,	3	,	5	,	1	,	29	,	45	,	29	,	215	,	39	,	679	,	799	,	1491	,	885	,	15475	,	29269	};
            const unsigned long	dim2608KuoInit[]		=	{	1	,	1	,	3	,	1	,	1	,	5	,	49	,	181	,	441	,	691	,	639	,	859	,	2785	,	3237	,	1703	};
            const unsigned long	dim2609KuoInit[]		=	{	1	,	1	,	1	,	9	,	19	,	1	,	41	,	131	,	469	,	999	,	1665	,	2309	,	4447	,	13851	,	7063	};
            const unsigned long	dim2610KuoInit[]		=	{	1	,	3	,	5	,	3	,	21	,	53	,	51	,	155	,	41	,	43	,	127	,	1653	,	6237	,	10481	,	22557	};
            const unsigned long	dim2611KuoInit[]		=	{	1	,	3	,	5	,	7	,	31	,	21	,	39	,	177	,	283	,	535	,	771	,	3783	,	3033	,	10971	,	8691	};
            const unsigned long	dim2612KuoInit[]		=	{	1	,	1	,	3	,	1	,	25	,	57	,	13	,	139	,	169	,	717	,	795	,	2337	,	7347	,	6951	,	20509	};
            const unsigned long	dim2613KuoInit[]		=	{	1	,	1	,	5	,	11	,	1	,	29	,	27	,	21	,	259	,	889	,	83	,	3295	,	3699	,	1523	,	6227	};
            const unsigned long	dim2614KuoInit[]		=	{	1	,	3	,	5	,	3	,	9	,	37	,	35	,	95	,	217	,	335	,	865	,	3093	,	3077	,	9831	,	12173	};
            const unsigned long	dim2615KuoInit[]		=	{	1	,	1	,	7	,	13	,	31	,	7	,	69	,	193	,	459	,	967	,	481	,	3271	,	677	,	13131	,	22665	};
            const unsigned long	dim2616KuoInit[]		=	{	1	,	3	,	3	,	9	,	13	,	31	,	43	,	155	,	465	,	303	,	1153	,	1591	,	7793	,	921	,	12805	};
            const unsigned long	dim2617KuoInit[]		=	{	1	,	1	,	5	,	5	,	23	,	59	,	3	,	151	,	391	,	729	,	913	,	899	,	1793	,	10173	,	19681	};
            const unsigned long	dim2618KuoInit[]		=	{	1	,	3	,	1	,	5	,	31	,	21	,	19	,	219	,	311	,	1013	,	227	,	1813	,	3103	,	14191	,	22995	};
            const unsigned long	dim2619KuoInit[]		=	{	1	,	1	,	3	,	5	,	3	,	59	,	11	,	191	,	433	,	63	,	465	,	3505	,	6801	,	3047	,	16913	};
            const unsigned long	dim2620KuoInit[]		=	{	1	,	1	,	7	,	9	,	19	,	3	,	95	,	203	,	1	,	679	,	1719	,	269	,	1747	,	9465	,	24445	};
            const unsigned long	dim2621KuoInit[]		=	{	1	,	1	,	7	,	9	,	9	,	17	,	79	,	45	,	179	,	863	,	801	,	2825	,	5345	,	637	,	9729	};
            const unsigned long	dim2622KuoInit[]		=	{	1	,	3	,	7	,	11	,	11	,	59	,	45	,	107	,	45	,	891	,	629	,	443	,	805	,	15277	,	1961	};
            const unsigned long	dim2623KuoInit[]		=	{	1	,	1	,	3	,	5	,	9	,	49	,	123	,	183	,	13	,	867	,	261	,	3883	,	2479	,	8221	,	3327	};
            const unsigned long	dim2624KuoInit[]		=	{	1	,	1	,	5	,	11	,	19	,	19	,	95	,	85	,	351	,	861	,	1499	,	3697	,	7369	,	10757	,	16445	};
            const unsigned long	dim2625KuoInit[]		=	{	1	,	3	,	5	,	5	,	27	,	13	,	119	,	243	,	437	,	655	,	955	,	1919	,	3643	,	12389	,	6865	};
            const unsigned long	dim2626KuoInit[]		=	{	1	,	3	,	3	,	1	,	23	,	15	,	43	,	65	,	87	,	833	,	503	,	999	,	703	,	4411	,	3343	};
            const unsigned long	dim2627KuoInit[]		=	{	1	,	3	,	7	,	1	,	31	,	13	,	67	,	199	,	261	,	257	,	1767	,	3983	,	823	,	149	,	32353	};
            const unsigned long	dim2628KuoInit[]		=	{	1	,	3	,	1	,	15	,	15	,	17	,	69	,	237	,	441	,	657	,	1125	,	3377	,	5971	,	15477	,	31363	};
            const unsigned long	dim2629KuoInit[]		=	{	1	,	3	,	1	,	13	,	3	,	55	,	67	,	93	,	315	,	1015	,	61	,	35	,	3059	,	15503	,	16811	};
            const unsigned long	dim2630KuoInit[]		=	{	1	,	1	,	3	,	9	,	1	,	51	,	91	,	213	,	3	,	37	,	849	,	2329	,	2237	,	6189	,	625	};
            const unsigned long	dim2631KuoInit[]		=	{	1	,	1	,	5	,	7	,	25	,	3	,	57	,	103	,	401	,	357	,	461	,	2677	,	6059	,	291	,	11461	};
            const unsigned long	dim2632KuoInit[]		=	{	1	,	1	,	5	,	7	,	23	,	29	,	119	,	61	,	121	,	535	,	251	,	565	,	7307	,	1085	,	8429	};
            const unsigned long	dim2633KuoInit[]		=	{	1	,	1	,	3	,	13	,	23	,	19	,	63	,	155	,	5	,	617	,	9	,	715	,	5399	,	1747	,	17633	};
            const unsigned long	dim2634KuoInit[]		=	{	1	,	1	,	1	,	13	,	3	,	57	,	13	,	161	,	123	,	389	,	1695	,	4083	,	1957	,	11359	,	18961	};
            const unsigned long	dim2635KuoInit[]		=	{	1	,	1	,	3	,	15	,	27	,	25	,	65	,	15	,	205	,	695	,	995	,	1471	,	1131	,	11435	,	22107	};
            const unsigned long	dim2636KuoInit[]		=	{	1	,	3	,	3	,	11	,	31	,	29	,	109	,	121	,	81	,	15	,	1087	,	299	,	3495	,	14137	,	19073	};
            const unsigned long	dim2637KuoInit[]		=	{	1	,	3	,	3	,	15	,	23	,	23	,	117	,	55	,	215	,	287	,	1421	,	731	,	279	,	15497	,	15545	};
            const unsigned long	dim2638KuoInit[]		=	{	1	,	3	,	5	,	15	,	1	,	31	,	95	,	249	,	403	,	239	,	771	,	1279	,	1863	,	12893	,	21043	};
            const unsigned long	dim2639KuoInit[]		=	{	1	,	3	,	1	,	9	,	21	,	23	,	123	,	169	,	69	,	591	,	621	,	1697	,	5867	,	5193	,	16573	};
            const unsigned long	dim2640KuoInit[]		=	{	1	,	3	,	3	,	5	,	29	,	19	,	105	,	177	,	143	,	141	,	1455	,	3119	,	347	,	8797	,	3925	};
            const unsigned long	dim2641KuoInit[]		=	{	1	,	3	,	3	,	15	,	9	,	11	,	41	,	169	,	141	,	555	,	1761	,	3441	,	2245	,	14205	,	9769	};
            const unsigned long	dim2642KuoInit[]		=	{	1	,	1	,	1	,	5	,	9	,	33	,	125	,	203	,	333	,	461	,	551	,	569	,	1305	,	10213	,	24549	};
            const unsigned long	dim2643KuoInit[]		=	{	1	,	1	,	7	,	13	,	21	,	9	,	99	,	43	,	395	,	667	,	121	,	1225	,	7835	,	8327	,	17717	};
            const unsigned long	dim2644KuoInit[]		=	{	1	,	3	,	3	,	7	,	21	,	9	,	31	,	59	,	321	,	505	,	1357	,	3213	,	4219	,	10637	,	11127	};
            const unsigned long	dim2645KuoInit[]		=	{	1	,	1	,	1	,	7	,	17	,	49	,	77	,	243	,	189	,	115	,	1339	,	2023	,	7431	,	5087	,	24495	};
            const unsigned long	dim2646KuoInit[]		=	{	1	,	1	,	1	,	11	,	1	,	39	,	41	,	51	,	315	,	383	,	237	,	3877	,	5731	,	11	,	21307	};
            const unsigned long	dim2647KuoInit[]		=	{	1	,	1	,	7	,	5	,	5	,	3	,	17	,	227	,	483	,	395	,	1251	,	1265	,	3965	,	15203	,	17163	};
            const unsigned long	dim2648KuoInit[]		=	{	1	,	3	,	7	,	7	,	23	,	41	,	53	,	45	,	43	,	989	,	629	,	1557	,	2891	,	4613	,	3647	};
            const unsigned long	dim2649KuoInit[]		=	{	1	,	3	,	3	,	5	,	15	,	39	,	35	,	127	,	267	,	45	,	1299	,	2251	,	7363	,	13595	,	31061	};
            const unsigned long	dim2650KuoInit[]		=	{	1	,	3	,	7	,	15	,	23	,	45	,	99	,	153	,	419	,	727	,	1077	,	3407	,	4861	,	3697	,	14479	};
            const unsigned long	dim2651KuoInit[]		=	{	1	,	3	,	5	,	15	,	19	,	21	,	123	,	173	,	295	,	47	,	1977	,	1185	,	7403	,	11901	,	29753	};
            const unsigned long	dim2652KuoInit[]		=	{	1	,	1	,	3	,	13	,	21	,	53	,	83	,	151	,	309	,	293	,	439	,	2381	,	7965	,	3449	,	8861	};
            const unsigned long	dim2653KuoInit[]		=	{	1	,	1	,	1	,	3	,	31	,	49	,	61	,	81	,	327	,	173	,	111	,	3893	,	2621	,	1409	,	13255	};
            const unsigned long	dim2654KuoInit[]		=	{	1	,	3	,	5	,	5	,	3	,	13	,	75	,	71	,	271	,	689	,	369	,	2719	,	759	,	8691	,	24611	};
            const unsigned long	dim2655KuoInit[]		=	{	1	,	1	,	1	,	5	,	21	,	7	,	69	,	17	,	347	,	699	,	1581	,	1095	,	1727	,	8213	,	5033	};
            const unsigned long	dim2656KuoInit[]		=	{	1	,	1	,	1	,	1	,	23	,	41	,	85	,	233	,	43	,	863	,	553	,	2625	,	2323	,	7471	,	12457	};
            const unsigned long	dim2657KuoInit[]		=	{	1	,	1	,	3	,	15	,	3	,	7	,	99	,	27	,	173	,	539	,	113	,	2309	,	1771	,	11709	,	29703	};
            const unsigned long	dim2658KuoInit[]		=	{	1	,	3	,	1	,	1	,	7	,	43	,	73	,	91	,	151	,	961	,	251	,	2089	,	4475	,	2745	,	6077	};
            const unsigned long	dim2659KuoInit[]		=	{	1	,	1	,	3	,	7	,	15	,	61	,	91	,	139	,	161	,	613	,	603	,	1587	,	3995	,	9241	,	24467	};
            const unsigned long	dim2660KuoInit[]		=	{	1	,	1	,	1	,	3	,	1	,	21	,	7	,	157	,	311	,	451	,	137	,	3289	,	5583	,	5107	,	24471	};
            const unsigned long	dim2661KuoInit[]		=	{	1	,	1	,	7	,	9	,	15	,	3	,	7	,	19	,	375	,	265	,	1781	,	335	,	233	,	3569	,	21407	};
            const unsigned long	dim2662KuoInit[]		=	{	1	,	1	,	5	,	13	,	7	,	53	,	81	,	79	,	355	,	89	,	1233	,	267	,	6523	,	8819	,	30595	};
            const unsigned long	dim2663KuoInit[]		=	{	1	,	1	,	7	,	7	,	1	,	59	,	65	,	93	,	251	,	565	,	1575	,	2709	,	7845	,	13355	,	6043	};
            const unsigned long	dim2664KuoInit[]		=	{	1	,	1	,	5	,	7	,	9	,	21	,	71	,	97	,	383	,	189	,	3	,	1841	,	1403	,	9649	,	25053	};
            const unsigned long	dim2665KuoInit[]		=	{	1	,	3	,	5	,	9	,	25	,	47	,	41	,	63	,	177	,	551	,	1389	,	1639	,	4209	,	15787	,	26499	};
            const unsigned long	dim2666KuoInit[]		=	{	1	,	1	,	3	,	15	,	29	,	53	,	119	,	3	,	33	,	677	,	207	,	1239	,	453	,	6281	,	26701	};
            const unsigned long	dim2667KuoInit[]		=	{	1	,	3	,	5	,	13	,	7	,	37	,	87	,	241	,	477	,	765	,	1511	,	2659	,	1043	,	313	,	27957	};
            const unsigned long	dim2668KuoInit[]		=	{	1	,	3	,	5	,	3	,	1	,	31	,	101	,	27	,	401	,	547	,	617	,	3605	,	4899	,	5093	,	19933	};
            const unsigned long	dim2669KuoInit[]		=	{	1	,	1	,	7	,	13	,	7	,	45	,	127	,	191	,	439	,	383	,	745	,	3143	,	2083	,	2941	,	31981	};
            const unsigned long	dim2670KuoInit[]		=	{	1	,	3	,	3	,	13	,	7	,	15	,	31	,	203	,	75	,	297	,	603	,	2945	,	1823	,	12359	,	29245	};
            const unsigned long	dim2671KuoInit[]		=	{	1	,	3	,	7	,	15	,	1	,	57	,	35	,	255	,	77	,	483	,	1051	,	1999	,	4975	,	16211	,	22903	};
            const unsigned long	dim2672KuoInit[]		=	{	1	,	1	,	5	,	5	,	23	,	53	,	13	,	251	,	181	,	1011	,	983	,	3735	,	4221	,	5557	,	25777	};
            const unsigned long	dim2673KuoInit[]		=	{	1	,	1	,	1	,	11	,	13	,	51	,	21	,	155	,	293	,	79	,	1015	,	2653	,	2959	,	9913	,	17751	};
            const unsigned long	dim2674KuoInit[]		=	{	1	,	1	,	5	,	3	,	17	,	33	,	27	,	125	,	437	,	663	,	343	,	1799	,	2129	,	7993	,	1373	};
            const unsigned long	dim2675KuoInit[]		=	{	1	,	3	,	3	,	3	,	9	,	17	,	31	,	123	,	453	,	853	,	1221	,	2863	,	4637	,	8239	,	27233	};
            const unsigned long	dim2676KuoInit[]		=	{	1	,	1	,	5	,	1	,	7	,	19	,	43	,	77	,	19	,	559	,	1397	,	1631	,	3187	,	1445	,	25437	};
            const unsigned long	dim2677KuoInit[]		=	{	1	,	1	,	3	,	5	,	31	,	21	,	71	,	127	,	223	,	127	,	717	,	3163	,	2079	,	2399	,	16415	};
            const unsigned long	dim2678KuoInit[]		=	{	1	,	1	,	1	,	13	,	17	,	13	,	99	,	227	,	127	,	625	,	1065	,	1779	,	7275	,	11865	,	17397	};
            const unsigned long	dim2679KuoInit[]		=	{	1	,	3	,	3	,	11	,	31	,	43	,	27	,	231	,	103	,	873	,	949	,	1179	,	3149	,	2251	,	22993	};
            const unsigned long	dim2680KuoInit[]		=	{	1	,	3	,	1	,	11	,	25	,	11	,	89	,	149	,	71	,	171	,	1985	,	3779	,	901	,	16307	,	5663	};
            const unsigned long	dim2681KuoInit[]		=	{	1	,	1	,	3	,	5	,	1	,	25	,	125	,	241	,	167	,	381	,	407	,	2801	,	635	,	313	,	10199	};
            const unsigned long	dim2682KuoInit[]		=	{	1	,	1	,	3	,	15	,	29	,	31	,	101	,	121	,	251	,	685	,	677	,	1683	,	3499	,	16193	,	7139	};
            const unsigned long	dim2683KuoInit[]		=	{	1	,	3	,	5	,	3	,	7	,	13	,	27	,	101	,	7	,	159	,	2005	,	3297	,	2775	,	11193	,	17385	};
            const unsigned long	dim2684KuoInit[]		=	{	1	,	1	,	5	,	5	,	25	,	31	,	119	,	43	,	317	,	371	,	1505	,	2631	,	1021	,	1297	,	22791	};
            const unsigned long	dim2685KuoInit[]		=	{	1	,	3	,	5	,	7	,	17	,	57	,	63	,	229	,	97	,	599	,	1485	,	1113	,	403	,	77	,	25541	};
            const unsigned long	dim2686KuoInit[]		=	{	1	,	1	,	7	,	13	,	11	,	43	,	29	,	85	,	335	,	943	,	1605	,	1757	,	2787	,	3313	,	14991	};
            const unsigned long	dim2687KuoInit[]		=	{	1	,	3	,	7	,	13	,	17	,	43	,	5	,	231	,	441	,	883	,	465	,	841	,	2785	,	8193	,	7951	};
            const unsigned long	dim2688KuoInit[]		=	{	1	,	3	,	1	,	3	,	27	,	33	,	1	,	215	,	295	,	381	,	283	,	219	,	3419	,	3343	,	17629	};
            const unsigned long	dim2689KuoInit[]		=	{	1	,	1	,	5	,	9	,	5	,	49	,	107	,	179	,	281	,	223	,	1845	,	2505	,	2281	,	12051	,	12609	};
            const unsigned long	dim2690KuoInit[]		=	{	1	,	1	,	7	,	13	,	21	,	13	,	109	,	183	,	385	,	293	,	773	,	3975	,	4737	,	8143	,	1335	};
            const unsigned long	dim2691KuoInit[]		=	{	1	,	1	,	1	,	13	,	13	,	47	,	23	,	27	,	271	,	957	,	1937	,	4091	,	3563	,	4055	,	17595	};
            const unsigned long	dim2692KuoInit[]		=	{	1	,	3	,	3	,	15	,	31	,	45	,	43	,	7	,	125	,	549	,	169	,	1567	,	5405	,	3919	,	21795	};
            const unsigned long	dim2693KuoInit[]		=	{	1	,	3	,	5	,	7	,	5	,	53	,	121	,	107	,	217	,	745	,	861	,	481	,	5503	,	3723	,	12215	};
            const unsigned long	dim2694KuoInit[]		=	{	1	,	3	,	7	,	3	,	23	,	27	,	91	,	175	,	477	,	289	,	817	,	2483	,	6071	,	5429	,	12923	};
            const unsigned long	dim2695KuoInit[]		=	{	1	,	1	,	3	,	15	,	7	,	63	,	29	,	157	,	323	,	851	,	1941	,	1927	,	5165	,	9875	,	11809	};
            const unsigned long	dim2696KuoInit[]		=	{	1	,	3	,	3	,	1	,	13	,	17	,	93	,	187	,	329	,	187	,	1417	,	1477	,	7891	,	11093	,	24705	};
            const unsigned long	dim2697KuoInit[]		=	{	1	,	3	,	3	,	13	,	5	,	41	,	119	,	135	,	255	,	47	,	803	,	3941	,	1647	,	7777	,	17515	};
            const unsigned long	dim2698KuoInit[]		=	{	1	,	1	,	5	,	9	,	25	,	11	,	15	,	231	,	81	,	267	,	1633	,	407	,	3319	,	12033	,	9651	};
            const unsigned long	dim2699KuoInit[]		=	{	1	,	3	,	1	,	1	,	31	,	43	,	3	,	101	,	435	,	869	,	159	,	441	,	613	,	10283	,	6873	};
            const unsigned long	dim2700KuoInit[]		=	{	1	,	1	,	5	,	1	,	11	,	55	,	7	,	65	,	357	,	595	,	1543	,	3957	,	6075	,	7265	,	27063	};
            const unsigned long	dim2701KuoInit[]		=	{	1	,	3	,	3	,	11	,	23	,	23	,	41	,	205	,	197	,	863	,	695	,	971	,	39	,	12389	,	2983	};
            const unsigned long	dim2702KuoInit[]		=	{	1	,	1	,	3	,	15	,	7	,	39	,	39	,	25	,	173	,	277	,	999	,	3797	,	3051	,	189	,	13559	};
            const unsigned long	dim2703KuoInit[]		=	{	1	,	1	,	7	,	5	,	3	,	7	,	47	,	79	,	411	,	109	,	55	,	3803	,	6617	,	14317	,	9533	};
            const unsigned long	dim2704KuoInit[]		=	{	1	,	1	,	3	,	9	,	31	,	17	,	53	,	97	,	495	,	995	,	1735	,	1055	,	6805	,	6865	,	23321	};
            const unsigned long	dim2705KuoInit[]		=	{	1	,	3	,	7	,	13	,	3	,	63	,	37	,	185	,	311	,	995	,	1819	,	2633	,	2593	,	2107	,	15755	};
            const unsigned long	dim2706KuoInit[]		=	{	1	,	3	,	7	,	11	,	25	,	37	,	79	,	179	,	435	,	167	,	147	,	2159	,	5909	,	10427	,	12487	};
            const unsigned long	dim2707KuoInit[]		=	{	1	,	3	,	5	,	7	,	31	,	9	,	69	,	93	,	477	,	283	,	479	,	1895	,	5693	,	4809	,	3157	};
            const unsigned long	dim2708KuoInit[]		=	{	1	,	1	,	3	,	11	,	27	,	21	,	11	,	3	,	157	,	1011	,	1473	,	2393	,	259	,	4155	,	28375	};
            const unsigned long	dim2709KuoInit[]		=	{	1	,	3	,	1	,	9	,	7	,	11	,	73	,	215	,	289	,	933	,	1369	,	2911	,	3251	,	727	,	19237	};
            const unsigned long	dim2710KuoInit[]		=	{	1	,	1	,	1	,	15	,	29	,	19	,	77	,	59	,	213	,	1005	,	2009	,	1899	,	5987	,	7429	,	5607	};
            const unsigned long	dim2711KuoInit[]		=	{	1	,	1	,	1	,	13	,	7	,	53	,	79	,	151	,	443	,	911	,	1777	,	1223	,	663	,	2099	,	17297	};
            const unsigned long	dim2712KuoInit[]		=	{	1	,	1	,	1	,	9	,	11	,	7	,	81	,	175	,	265	,	879	,	1867	,	539	,	2353	,	5667	,	4973	};
            const unsigned long	dim2713KuoInit[]		=	{	1	,	1	,	1	,	9	,	23	,	63	,	35	,	57	,	139	,	653	,	1837	,	681	,	6993	,	643	,	21771	};
            const unsigned long	dim2714KuoInit[]		=	{	1	,	3	,	7	,	9	,	11	,	13	,	123	,	199	,	405	,	373	,	1817	,	3331	,	5585	,	11721	,	9355	};
            const unsigned long	dim2715KuoInit[]		=	{	1	,	1	,	7	,	7	,	27	,	29	,	79	,	121	,	399	,	85	,	531	,	507	,	4361	,	6167	,	29317	};
            const unsigned long	dim2716KuoInit[]		=	{	1	,	1	,	7	,	11	,	13	,	19	,	75	,	245	,	197	,	731	,	233	,	487	,	2245	,	11413	,	31865	};
            const unsigned long	dim2717KuoInit[]		=	{	1	,	1	,	7	,	5	,	27	,	43	,	109	,	135	,	91	,	475	,	209	,	865	,	2239	,	8065	,	23295	};
            const unsigned long	dim2718KuoInit[]		=	{	1	,	1	,	1	,	9	,	13	,	27	,	101	,	121	,	23	,	319	,	873	,	251	,	299	,	7145	,	26225	};
            const unsigned long	dim2719KuoInit[]		=	{	1	,	1	,	7	,	1	,	31	,	51	,	85	,	91	,	299	,	545	,	1557	,	2447	,	4055	,	3911	,	12805	};
            const unsigned long	dim2720KuoInit[]		=	{	1	,	3	,	1	,	9	,	31	,	63	,	123	,	191	,	241	,	7	,	347	,	23	,	5135	,	7353	,	30865	};
            const unsigned long	dim2721KuoInit[]		=	{	1	,	1	,	5	,	1	,	3	,	13	,	81	,	249	,	65	,	853	,	893	,	219	,	6555	,	4941	,	21669	};
            const unsigned long	dim2722KuoInit[]		=	{	1	,	3	,	5	,	13	,	13	,	27	,	13	,	203	,	205	,	459	,	883	,	1723	,	2721	,	9173	,	9643	};
            const unsigned long	dim2723KuoInit[]		=	{	1	,	1	,	5	,	5	,	11	,	59	,	109	,	25	,	107	,	1007	,	357	,	2877	,	5075	,	3251	,	29541	};
            const unsigned long	dim2724KuoInit[]		=	{	1	,	1	,	1	,	15	,	19	,	37	,	99	,	183	,	267	,	571	,	49	,	975	,	2441	,	4459	,	15481	};
            const unsigned long	dim2725KuoInit[]		=	{	1	,	3	,	1	,	1	,	21	,	7	,	89	,	233	,	209	,	151	,	1919	,	31	,	4363	,	9781	,	4683	};
            const unsigned long	dim2726KuoInit[]		=	{	1	,	1	,	3	,	9	,	15	,	35	,	101	,	29	,	27	,	737	,	1577	,	2611	,	3727	,	2245	,	31889	};
            const unsigned long	dim2727KuoInit[]		=	{	1	,	3	,	5	,	9	,	19	,	19	,	125	,	229	,	313	,	239	,	67	,	1373	,	6459	,	1339	,	19909	};
            const unsigned long	dim2728KuoInit[]		=	{	1	,	3	,	3	,	13	,	19	,	35	,	17	,	31	,	183	,	391	,	1515	,	2357	,	2673	,	10055	,	943	};
            const unsigned long	dim2729KuoInit[]		=	{	1	,	1	,	3	,	11	,	31	,	17	,	37	,	239	,	79	,	869	,	629	,	2755	,	7431	,	127	,	19155	};
            const unsigned long	dim2730KuoInit[]		=	{	1	,	3	,	5	,	3	,	9	,	13	,	91	,	27	,	163	,	455	,	2007	,	2479	,	2387	,	1557	,	437	};
            const unsigned long	dim2731KuoInit[]		=	{	1	,	1	,	1	,	13	,	15	,	63	,	11	,	105	,	51	,	673	,	997	,	2487	,	35	,	8655	,	21481	};
            const unsigned long	dim2732KuoInit[]		=	{	1	,	3	,	5	,	11	,	1	,	3	,	27	,	117	,	195	,	233	,	269	,	2163	,	2083	,	8907	,	28005	};
            const unsigned long	dim2733KuoInit[]		=	{	1	,	1	,	1	,	9	,	9	,	17	,	63	,	211	,	87	,	107	,	1183	,	2705	,	6811	,	9959	,	13937	};
            const unsigned long	dim2734KuoInit[]		=	{	1	,	1	,	1	,	1	,	1	,	3	,	57	,	231	,	209	,	949	,	895	,	2773	,	237	,	8943	,	22453	};
            const unsigned long	dim2735KuoInit[]		=	{	1	,	3	,	7	,	11	,	7	,	53	,	1	,	247	,	279	,	411	,	1247	,	2121	,	3127	,	16279	,	277	};
            const unsigned long	dim2736KuoInit[]		=	{	1	,	3	,	5	,	5	,	15	,	35	,	15	,	15	,	485	,	763	,	1631	,	1717	,	191	,	6609	,	32383	};
            const unsigned long	dim2737KuoInit[]		=	{	1	,	1	,	5	,	11	,	27	,	5	,	73	,	143	,	127	,	477	,	535	,	3815	,	3135	,	9865	,	5375	};
            const unsigned long	dim2738KuoInit[]		=	{	1	,	1	,	7	,	9	,	21	,	31	,	83	,	63	,	313	,	775	,	875	,	4053	,	905	,	14255	,	31537	};
            const unsigned long	dim2739KuoInit[]		=	{	1	,	1	,	1	,	3	,	7	,	3	,	107	,	231	,	333	,	935	,	1625	,	2527	,	2293	,	10407	,	4795	};
            const unsigned long	dim2740KuoInit[]		=	{	1	,	3	,	5	,	7	,	25	,	19	,	123	,	193	,	353	,	353	,	1403	,	3007	,	677	,	15333	,	1263	};
            const unsigned long	dim2741KuoInit[]		=	{	1	,	1	,	3	,	9	,	1	,	39	,	79	,	251	,	59	,	973	,	1707	,	2153	,	7445	,	9083	,	6675	};
            const unsigned long	dim2742KuoInit[]		=	{	1	,	1	,	7	,	3	,	7	,	55	,	21	,	101	,	203	,	735	,	1789	,	3119	,	6633	,	3643	,	1947	};
            const unsigned long	dim2743KuoInit[]		=	{	1	,	1	,	5	,	9	,	13	,	49	,	103	,	141	,	181	,	893	,	385	,	1541	,	5887	,	10687	,	16183	};
            const unsigned long	dim2744KuoInit[]		=	{	1	,	3	,	3	,	7	,	17	,	59	,	35	,	119	,	467	,	77	,	1591	,	469	,	4991	,	8113	,	20231	};
            const unsigned long	dim2745KuoInit[]		=	{	1	,	3	,	1	,	15	,	19	,	41	,	5	,	71	,	247	,	955	,	565	,	1931	,	2895	,	13189	,	28461	};
            const unsigned long	dim2746KuoInit[]		=	{	1	,	3	,	5	,	9	,	19	,	21	,	123	,	39	,	175	,	35	,	673	,	107	,	3157	,	1495	,	16341	};
            const unsigned long	dim2747KuoInit[]		=	{	1	,	1	,	3	,	9	,	31	,	25	,	9	,	17	,	463	,	585	,	1701	,	3325	,	6071	,	2719	,	4479	};
            const unsigned long	dim2748KuoInit[]		=	{	1	,	3	,	3	,	15	,	25	,	51	,	63	,	241	,	289	,	787	,	1109	,	453	,	5119	,	139	,	377	};
            const unsigned long	dim2749KuoInit[]		=	{	1	,	3	,	5	,	7	,	29	,	3	,	53	,	119	,	219	,	367	,	1785	,	777	,	5255	,	10013	,	27979	};
            const unsigned long	dim2750KuoInit[]		=	{	1	,	1	,	1	,	3	,	1	,	45	,	3	,	239	,	429	,	595	,	1591	,	3111	,	2373	,	4121	,	12671	};
            const unsigned long	dim2751KuoInit[]		=	{	1	,	3	,	3	,	9	,	5	,	49	,	11	,	145	,	167	,	19	,	155	,	2017	,	3593	,	15275	,	22319	};
            const unsigned long	dim2752KuoInit[]		=	{	1	,	3	,	1	,	3	,	11	,	15	,	17	,	241	,	293	,	345	,	1663	,	301	,	627	,	12161	,	19939	};
            const unsigned long	dim2753KuoInit[]		=	{	1	,	3	,	5	,	1	,	3	,	19	,	41	,	75	,	121	,	611	,	1741	,	4055	,	5567	,	773	,	767	};
            const unsigned long	dim2754KuoInit[]		=	{	1	,	1	,	1	,	3	,	5	,	49	,	127	,	49	,	93	,	819	,	1391	,	2173	,	4021	,	11953	,	28465	};
            const unsigned long	dim2755KuoInit[]		=	{	1	,	1	,	3	,	5	,	9	,	59	,	63	,	85	,	249	,	239	,	521	,	3333	,	6301	,	15283	,	10645	};
            const unsigned long	dim2756KuoInit[]		=	{	1	,	1	,	1	,	7	,	9	,	53	,	107	,	201	,	151	,	79	,	1561	,	3497	,	4473	,	4369	,	6231	};
            const unsigned long	dim2757KuoInit[]		=	{	1	,	1	,	1	,	9	,	11	,	61	,	57	,	69	,	215	,	637	,	1087	,	2827	,	2495	,	7471	,	19127	};
            const unsigned long	dim2758KuoInit[]		=	{	1	,	1	,	1	,	13	,	3	,	25	,	87	,	131	,	171	,	577	,	957	,	989	,	3391	,	3803	,	27229	};
            const unsigned long	dim2759KuoInit[]		=	{	1	,	1	,	5	,	5	,	1	,	25	,	83	,	71	,	161	,	477	,	573	,	3709	,	4393	,	4607	,	32373	};
            const unsigned long	dim2760KuoInit[]		=	{	1	,	1	,	7	,	5	,	13	,	21	,	19	,	71	,	207	,	433	,	849	,	2685	,	4455	,	463	,	25163	};
            const unsigned long	dim2761KuoInit[]		=	{	1	,	1	,	7	,	3	,	5	,	19	,	35	,	41	,	487	,	3	,	865	,	25	,	7925	,	9015	,	29397	};
            const unsigned long	dim2762KuoInit[]		=	{	1	,	1	,	7	,	3	,	7	,	41	,	83	,	27	,	79	,	501	,	267	,	2015	,	8109	,	8877	,	4029	};
            const unsigned long	dim2763KuoInit[]		=	{	1	,	3	,	5	,	1	,	1	,	31	,	71	,	233	,	167	,	185	,	33	,	1249	,	3755	,	2805	,	29559	};
            const unsigned long	dim2764KuoInit[]		=	{	1	,	1	,	1	,	7	,	11	,	21	,	111	,	105	,	469	,	527	,	711	,	3551	,	521	,	10593	,	22701	};
            const unsigned long	dim2765KuoInit[]		=	{	1	,	1	,	1	,	1	,	29	,	1	,	79	,	109	,	279	,	643	,	1263	,	2405	,	3717	,	11395	,	4325	};
            const unsigned long	dim2766KuoInit[]		=	{	1	,	1	,	1	,	1	,	29	,	47	,	59	,	1	,	195	,	399	,	1963	,	265	,	317	,	15117	,	9935	};
            const unsigned long	dim2767KuoInit[]		=	{	1	,	1	,	7	,	5	,	21	,	43	,	93	,	113	,	273	,	67	,	281	,	477	,	6309	,	3649	,	20147	};
            const unsigned long	dim2768KuoInit[]		=	{	1	,	1	,	3	,	15	,	29	,	61	,	69	,	151	,	185	,	809	,	993	,	2383	,	7263	,	901	,	18607	};
            const unsigned long	dim2769KuoInit[]		=	{	1	,	1	,	3	,	9	,	27	,	33	,	25	,	103	,	387	,	131	,	1555	,	301	,	1471	,	11331	,	15091	};
            const unsigned long	dim2770KuoInit[]		=	{	1	,	1	,	7	,	7	,	19	,	55	,	115	,	121	,	99	,	371	,	1563	,	895	,	6207	,	93	,	8549	};
            const unsigned long	dim2771KuoInit[]		=	{	1	,	1	,	1	,	9	,	19	,	23	,	13	,	181	,	281	,	479	,	1	,	1515	,	3853	,	8601	,	19241	};
            const unsigned long	dim2772KuoInit[]		=	{	1	,	3	,	1	,	15	,	29	,	63	,	113	,	253	,	105	,	421	,	1679	,	2113	,	4671	,	7923	,	7671	};
            const unsigned long	dim2773KuoInit[]		=	{	1	,	3	,	1	,	3	,	17	,	9	,	15	,	241	,	339	,	619	,	417	,	3257	,	5917	,	9981	,	19033	};
            const unsigned long	dim2774KuoInit[]		=	{	1	,	3	,	3	,	13	,	21	,	11	,	97	,	201	,	257	,	307	,	133	,	1793	,	193	,	8609	,	27771	};
            const unsigned long	dim2775KuoInit[]		=	{	1	,	1	,	7	,	3	,	25	,	55	,	51	,	97	,	453	,	37	,	639	,	3451	,	6587	,	3835	,	13229	};
            const unsigned long	dim2776KuoInit[]		=	{	1	,	3	,	1	,	1	,	3	,	21	,	123	,	81	,	447	,	963	,	449	,	1329	,	4887	,	1141	,	3213	};
            const unsigned long	dim2777KuoInit[]		=	{	1	,	1	,	3	,	1	,	5	,	33	,	107	,	89	,	67	,	229	,	2025	,	1925	,	5369	,	14049	,	10461	};
            const unsigned long	dim2778KuoInit[]		=	{	1	,	1	,	7	,	3	,	23	,	17	,	75	,	79	,	51	,	25	,	1541	,	3475	,	6129	,	13391	,	31091	};
            const unsigned long	dim2779KuoInit[]		=	{	1	,	3	,	3	,	13	,	17	,	27	,	5	,	57	,	147	,	549	,	721	,	2477	,	893	,	6413	,	24053	};
            const unsigned long	dim2780KuoInit[]		=	{	1	,	3	,	1	,	15	,	11	,	29	,	119	,	149	,	185	,	713	,	1603	,	3671	,	3409	,	2689	,	7933	};
            const unsigned long	dim2781KuoInit[]		=	{	1	,	1	,	7	,	15	,	9	,	33	,	97	,	85	,	185	,	225	,	1747	,	1677	,	4133	,	11421	,	26591	};
            const unsigned long	dim2782KuoInit[]		=	{	1	,	1	,	5	,	15	,	29	,	43	,	75	,	251	,	157	,	861	,	517	,	3341	,	4447	,	14803	,	18465	};
            const unsigned long	dim2783KuoInit[]		=	{	1	,	3	,	5	,	15	,	11	,	37	,	57	,	189	,	423	,	881	,	1199	,	1263	,	2691	,	6969	,	21803	};
            const unsigned long	dim2784KuoInit[]		=	{	1	,	1	,	3	,	3	,	25	,	47	,	15	,	107	,	119	,	885	,	1149	,	673	,	4459	,	9997	,	4519	};
            const unsigned long	dim2785KuoInit[]		=	{	1	,	3	,	7	,	13	,	23	,	7	,	37	,	161	,	17	,	499	,	1675	,	2309	,	4159	,	13771	,	17741	};
            const unsigned long	dim2786KuoInit[]		=	{	1	,	3	,	1	,	13	,	7	,	51	,	111	,	163	,	313	,	835	,	1903	,	2201	,	321	,	14655	,	16845	};
            const unsigned long	dim2787KuoInit[]		=	{	1	,	1	,	3	,	3	,	7	,	41	,	23	,	87	,	101	,	371	,	257	,	1679	,	7449	,	12967	,	31	};
            const unsigned long	dim2788KuoInit[]		=	{	1	,	3	,	3	,	7	,	29	,	37	,	57	,	163	,	507	,	1	,	691	,	2933	,	4687	,	10621	,	32351	};
            const unsigned long	dim2789KuoInit[]		=	{	1	,	3	,	7	,	3	,	31	,	63	,	73	,	67	,	503	,	289	,	1533	,	1203	,	7931	,	11399	,	23463	};
            const unsigned long	dim2790KuoInit[]		=	{	1	,	1	,	5	,	7	,	23	,	39	,	19	,	73	,	467	,	65	,	791	,	2017	,	1763	,	11921	,	31643	};
            const unsigned long	dim2791KuoInit[]		=	{	1	,	3	,	3	,	1	,	19	,	21	,	53	,	61	,	401	,	429	,	1095	,	297	,	7931	,	13085	,	2013	};
            const unsigned long	dim2792KuoInit[]		=	{	1	,	1	,	5	,	11	,	27	,	41	,	23	,	225	,	299	,	943	,	823	,	3507	,	4333	,	817	,	26137	};
            const unsigned long	dim2793KuoInit[]		=	{	1	,	1	,	1	,	7	,	27	,	29	,	31	,	251	,	499	,	151	,	295	,	1731	,	7661	,	11825	,	29587	};
            const unsigned long	dim2794KuoInit[]		=	{	1	,	1	,	3	,	7	,	17	,	43	,	17	,	5	,	25	,	813	,	1875	,	2941	,	1407	,	11669	,	16753	};
            const unsigned long	dim2795KuoInit[]		=	{	1	,	3	,	1	,	1	,	17	,	59	,	93	,	173	,	335	,	933	,	369	,	3421	,	2769	,	16361	,	11891	};
            const unsigned long	dim2796KuoInit[]		=	{	1	,	1	,	1	,	3	,	27	,	5	,	63	,	7	,	415	,	533	,	179	,	851	,	1623	,	16375	,	1271	};
            const unsigned long	dim2797KuoInit[]		=	{	1	,	1	,	5	,	1	,	17	,	45	,	125	,	93	,	43	,	311	,	153	,	1225	,	4891	,	5621	,	14355	};
            const unsigned long	dim2798KuoInit[]		=	{	1	,	3	,	7	,	5	,	13	,	63	,	103	,	205	,	119	,	953	,	1087	,	703	,	3729	,	10249	,	19085	};
            const unsigned long	dim2799KuoInit[]		=	{	1	,	3	,	5	,	3	,	11	,	51	,	17	,	97	,	323	,	657	,	269	,	467	,	2747	,	13603	,	22803	};
            const unsigned long	dim2800KuoInit[]		=	{	1	,	3	,	3	,	15	,	3	,	5	,	35	,	23	,	433	,	259	,	1857	,	2313	,	7709	,	2297	,	31109	};
            const unsigned long	dim2801KuoInit[]		=	{	1	,	3	,	1	,	15	,	19	,	11	,	51	,	251	,	167	,	741	,	1705	,	3763	,	967	,	2593	,	28229	};
            const unsigned long	dim2802KuoInit[]		=	{	1	,	3	,	3	,	13	,	29	,	47	,	47	,	183	,	305	,	223	,	381	,	1753	,	2285	,	9113	,	21685	};
            const unsigned long	dim2803KuoInit[]		=	{	1	,	3	,	7	,	13	,	31	,	17	,	105	,	25	,	49	,	679	,	1501	,	3887	,	5007	,	4275	,	31161	};
            const unsigned long	dim2804KuoInit[]		=	{	1	,	3	,	7	,	5	,	27	,	59	,	17	,	183	,	477	,	191	,	1111	,	383	,	5519	,	7435	,	20341	};
            const unsigned long	dim2805KuoInit[]		=	{	1	,	1	,	7	,	13	,	25	,	15	,	67	,	57	,	5	,	955	,	731	,	3785	,	2055	,	13017	,	27843	};
            const unsigned long	dim2806KuoInit[]		=	{	1	,	1	,	5	,	1	,	27	,	45	,	67	,	93	,	311	,	635	,	1893	,	1753	,	7307	,	2765	,	6263	};
            const unsigned long	dim2807KuoInit[]		=	{	1	,	1	,	3	,	1	,	13	,	53	,	109	,	5	,	3	,	705	,	1745	,	2233	,	3425	,	6125	,	28839	};
            const unsigned long	dim2808KuoInit[]		=	{	1	,	3	,	5	,	5	,	5	,	39	,	5	,	251	,	431	,	85	,	599	,	3459	,	3211	,	15611	,	28739	};
            const unsigned long	dim2809KuoInit[]		=	{	1	,	1	,	7	,	7	,	25	,	5	,	25	,	31	,	283	,	909	,	399	,	1335	,	6973	,	15791	,	28351	};
            const unsigned long	dim2810KuoInit[]		=	{	1	,	1	,	5	,	5	,	3	,	29	,	47	,	227	,	463	,	815	,	1089	,	2217	,	4911	,	14743	,	17843	};
            const unsigned long	dim2811KuoInit[]		=	{	1	,	1	,	7	,	3	,	29	,	39	,	125	,	165	,	367	,	461	,	569	,	3441	,	1293	,	1577	,	10567	};
            const unsigned long	dim2812KuoInit[]		=	{	1	,	1	,	1	,	11	,	5	,	11	,	19	,	129	,	401	,	111	,	2019	,	899	,	2079	,	7363	,	32675	};
            const unsigned long	dim2813KuoInit[]		=	{	1	,	3	,	1	,	7	,	13	,	27	,	47	,	183	,	465	,	421	,	665	,	1279	,	5629	,	7559	,	21265	};
            const unsigned long	dim2814KuoInit[]		=	{	1	,	1	,	7	,	1	,	11	,	19	,	75	,	1	,	137	,	893	,	1709	,	385	,	2221	,	15185	,	10923	};
            const unsigned long	dim2815KuoInit[]		=	{	1	,	3	,	5	,	5	,	9	,	1	,	115	,	203	,	133	,	259	,	915	,	2477	,	3615	,	1749	,	7643	};
            const unsigned long	dim2816KuoInit[]		=	{	1	,	3	,	3	,	5	,	9	,	57	,	21	,	113	,	453	,	739	,	1857	,	685	,	5727	,	10625	,	23469	};
            const unsigned long	dim2817KuoInit[]		=	{	1	,	1	,	5	,	5	,	13	,	45	,	37	,	25	,	495	,	723	,	1805	,	1631	,	4889	,	12885	,	16895	};
            const unsigned long	dim2818KuoInit[]		=	{	1	,	1	,	5	,	7	,	17	,	59	,	111	,	29	,	143	,	863	,	1757	,	1621	,	3995	,	7315	,	30271	};
            const unsigned long	dim2819KuoInit[]		=	{	1	,	1	,	1	,	1	,	5	,	31	,	81	,	161	,	275	,	623	,	2017	,	967	,	2395	,	14923	,	23931	};
            const unsigned long	dim2820KuoInit[]		=	{	1	,	3	,	5	,	11	,	31	,	33	,	95	,	135	,	477	,	221	,	1603	,	231	,	5957	,	12461	,	2623	};
            const unsigned long	dim2821KuoInit[]		=	{	1	,	3	,	5	,	11	,	7	,	3	,	53	,	43	,	351	,	557	,	1981	,	2127	,	909	,	8943	,	21035	};
            const unsigned long	dim2822KuoInit[]		=	{	1	,	3	,	5	,	9	,	31	,	11	,	13	,	231	,	145	,	321	,	567	,	1919	,	7101	,	12089	,	8331	};
            const unsigned long	dim2823KuoInit[]		=	{	1	,	1	,	7	,	1	,	13	,	27	,	101	,	229	,	89	,	395	,	349	,	1785	,	6441	,	8241	,	30735	};
            const unsigned long	dim2824KuoInit[]		=	{	1	,	1	,	7	,	9	,	21	,	57	,	123	,	77	,	251	,	347	,	1367	,	2205	,	4017	,	16057	,	9247	};
            const unsigned long	dim2825KuoInit[]		=	{	1	,	3	,	3	,	11	,	5	,	1	,	47	,	57	,	69	,	381	,	1613	,	1525	,	5715	,	3677	,	13015	};
            const unsigned long	dim2826KuoInit[]		=	{	1	,	3	,	7	,	9	,	31	,	33	,	15	,	255	,	149	,	1021	,	1149	,	3609	,	1739	,	11867	,	3093	};
            const unsigned long	dim2827KuoInit[]		=	{	1	,	1	,	7	,	15	,	27	,	25	,	57	,	5	,	137	,	1005	,	343	,	2567	,	1025	,	8615	,	6357	};
            const unsigned long	dim2828KuoInit[]		=	{	1	,	3	,	5	,	1	,	29	,	57	,	85	,	65	,	85	,	789	,	1179	,	2523	,	7589	,	14657	,	23637	};
            const unsigned long	dim2829KuoInit[]		=	{	1	,	3	,	1	,	11	,	3	,	3	,	107	,	37	,	125	,	423	,	225	,	2991	,	4515	,	5057	,	31181	};
            const unsigned long	dim2830KuoInit[]		=	{	1	,	1	,	5	,	9	,	31	,	5	,	111	,	241	,	301	,	37	,	943	,	555	,	7051	,	11627	,	7125	};
            const unsigned long	dim2831KuoInit[]		=	{	1	,	3	,	3	,	15	,	17	,	31	,	119	,	185	,	187	,	621	,	395	,	3595	,	1055	,	7993	,	28441	};
            const unsigned long	dim2832KuoInit[]		=	{	1	,	1	,	5	,	1	,	31	,	25	,	113	,	223	,	493	,	919	,	1455	,	31	,	4743	,	14435	,	31627	};
            const unsigned long	dim2833KuoInit[]		=	{	1	,	1	,	1	,	1	,	27	,	59	,	45	,	75	,	343	,	701	,	1737	,	161	,	6919	,	13537	,	26835	};
            const unsigned long	dim2834KuoInit[]		=	{	1	,	3	,	1	,	9	,	31	,	45	,	89	,	215	,	137	,	237	,	1135	,	3239	,	3345	,	12063	,	14599	};
            const unsigned long	dim2835KuoInit[]		=	{	1	,	3	,	5	,	15	,	5	,	51	,	85	,	73	,	353	,	943	,	1981	,	127	,	2605	,	5673	,	10037	};
            const unsigned long	dim2836KuoInit[]		=	{	1	,	1	,	7	,	1	,	3	,	49	,	109	,	105	,	483	,	499	,	1581	,	793	,	5857	,	5509	,	4651	};
            const unsigned long	dim2837KuoInit[]		=	{	1	,	3	,	5	,	9	,	23	,	5	,	109	,	83	,	7	,	155	,	187	,	2205	,	4199	,	5487	,	31925	};
            const unsigned long	dim2838KuoInit[]		=	{	1	,	1	,	7	,	5	,	13	,	11	,	75	,	53	,	125	,	139	,	1241	,	203	,	7361	,	13783	,	4317	};
            const unsigned long	dim2839KuoInit[]		=	{	1	,	3	,	7	,	5	,	25	,	57	,	105	,	197	,	207	,	261	,	1251	,	4057	,	3877	,	13613	,	7829	};
            const unsigned long	dim2840KuoInit[]		=	{	1	,	1	,	5	,	13	,	31	,	21	,	89	,	73	,	163	,	941	,	1683	,	2239	,	1893	,	16067	,	609	};
            const unsigned long	dim2841KuoInit[]		=	{	1	,	1	,	1	,	3	,	5	,	9	,	41	,	149	,	43	,	333	,	1459	,	277	,	5687	,	14201	,	26879	};
            const unsigned long	dim2842KuoInit[]		=	{	1	,	1	,	3	,	15	,	21	,	37	,	67	,	227	,	443	,	767	,	1109	,	2151	,	7875	,	15519	,	13741	};
            const unsigned long	dim2843KuoInit[]		=	{	1	,	3	,	1	,	5	,	31	,	15	,	107	,	43	,	343	,	27	,	1321	,	3009	,	5751	,	2931	,	9733	};
            const unsigned long	dim2844KuoInit[]		=	{	1	,	1	,	1	,	11	,	5	,	27	,	49	,	33	,	439	,	555	,	97	,	59	,	4043	,	12063	,	27227	};
            const unsigned long	dim2845KuoInit[]		=	{	1	,	3	,	7	,	7	,	27	,	9	,	55	,	69	,	485	,	75	,	1255	,	101	,	7193	,	9427	,	31125	};
            const unsigned long	dim2846KuoInit[]		=	{	1	,	3	,	7	,	3	,	15	,	23	,	51	,	145	,	501	,	875	,	1611	,	1321	,	6809	,	5051	,	17135	};
            const unsigned long	dim2847KuoInit[]		=	{	1	,	3	,	3	,	13	,	5	,	45	,	35	,	115	,	3	,	639	,	1705	,	207	,	4235	,	3329	,	14481	};
            const unsigned long	dim2848KuoInit[]		=	{	1	,	3	,	3	,	1	,	5	,	15	,	17	,	115	,	175	,	583	,	1649	,	2195	,	3827	,	16227	,	21401	};
            const unsigned long	dim2849KuoInit[]		=	{	1	,	3	,	3	,	7	,	1	,	3	,	107	,	167	,	195	,	953	,	743	,	1375	,	701	,	1155	,	1027	};
            const unsigned long	dim2850KuoInit[]		=	{	1	,	1	,	7	,	1	,	31	,	31	,	67	,	133	,	219	,	307	,	1031	,	3321	,	4855	,	11285	,	16987	};
            const unsigned long	dim2851KuoInit[]		=	{	1	,	1	,	7	,	1	,	25	,	9	,	41	,	105	,	89	,	289	,	1851	,	2245	,	1647	,	2925	,	8637	};
            const unsigned long	dim2852KuoInit[]		=	{	1	,	3	,	7	,	15	,	29	,	29	,	107	,	21	,	101	,	419	,	1207	,	3301	,	1217	,	14157	,	17373	};
            const unsigned long	dim2853KuoInit[]		=	{	1	,	1	,	5	,	11	,	9	,	41	,	49	,	165	,	353	,	215	,	883	,	2031	,	7363	,	13221	,	8329	};
            const unsigned long	dim2854KuoInit[]		=	{	1	,	1	,	5	,	15	,	17	,	7	,	67	,	217	,	87	,	217	,	2013	,	3809	,	4983	,	5341	,	30353	};
            const unsigned long	dim2855KuoInit[]		=	{	1	,	1	,	7	,	9	,	1	,	37	,	97	,	109	,	485	,	777	,	171	,	1391	,	781	,	5019	,	19883	};
            const unsigned long	dim2856KuoInit[]		=	{	1	,	3	,	3	,	13	,	7	,	17	,	27	,	69	,	265	,	663	,	569	,	2909	,	7409	,	14813	,	5849	};
            const unsigned long	dim2857KuoInit[]		=	{	1	,	3	,	5	,	13	,	11	,	29	,	27	,	105	,	215	,	59	,	1251	,	2003	,	2201	,	7221	,	7549	};
            const unsigned long	dim2858KuoInit[]		=	{	1	,	1	,	1	,	3	,	15	,	7	,	79	,	237	,	327	,	839	,	1167	,	3211	,	275	,	16031	,	25691	};
            const unsigned long	dim2859KuoInit[]		=	{	1	,	1	,	5	,	3	,	3	,	33	,	117	,	113	,	361	,	381	,	1093	,	1791	,	2399	,	1245	,	23275	};
            const unsigned long	dim2860KuoInit[]		=	{	1	,	1	,	1	,	3	,	7	,	49	,	77	,	5	,	217	,	995	,	1127	,	3891	,	81	,	13331	,	12479	};
            const unsigned long	dim2861KuoInit[]		=	{	1	,	1	,	7	,	9	,	9	,	57	,	119	,	227	,	41	,	337	,	183	,	3201	,	3627	,	16263	,	28153	};
            const unsigned long	dim2862KuoInit[]		=	{	1	,	3	,	1	,	13	,	11	,	9	,	81	,	99	,	247	,	157	,	1421	,	1087	,	151	,	6529	,	13537	};
            const unsigned long	dim2863KuoInit[]		=	{	1	,	3	,	3	,	5	,	31	,	43	,	113	,	143	,	163	,	583	,	2031	,	3087	,	7565	,	4987	,	17131	};
            const unsigned long	dim2864KuoInit[]		=	{	1	,	1	,	3	,	1	,	9	,	1	,	121	,	93	,	113	,	849	,	679	,	1403	,	6815	,	15605	,	29785	};
            const unsigned long	dim2865KuoInit[]		=	{	1	,	3	,	7	,	11	,	27	,	63	,	53	,	83	,	397	,	935	,	23	,	125	,	5813	,	15765	,	2869	};
            const unsigned long	dim2866KuoInit[]		=	{	1	,	1	,	3	,	3	,	9	,	11	,	65	,	77	,	387	,	129	,	1039	,	1583	,	2235	,	3851	,	30603	};
            const unsigned long	dim2867KuoInit[]		=	{	1	,	1	,	7	,	3	,	5	,	27	,	45	,	233	,	351	,	827	,	215	,	1209	,	409	,	2185	,	3827	};
            const unsigned long	dim2868KuoInit[]		=	{	1	,	1	,	3	,	3	,	31	,	23	,	35	,	83	,	271	,	65	,	553	,	3189	,	2435	,	12865	,	28271	};
            const unsigned long	dim2869KuoInit[]		=	{	1	,	1	,	5	,	1	,	29	,	51	,	35	,	105	,	477	,	1001	,	1859	,	2063	,	6895	,	1855	,	13931	};
            const unsigned long	dim2870KuoInit[]		=	{	1	,	3	,	3	,	11	,	17	,	63	,	125	,	165	,	37	,	671	,	2001	,	1237	,	7355	,	339	,	24771	};
            const unsigned long	dim2871KuoInit[]		=	{	1	,	3	,	3	,	5	,	13	,	5	,	9	,	201	,	183	,	39	,	119	,	3889	,	949	,	7889	,	25951	};
            const unsigned long	dim2872KuoInit[]		=	{	1	,	1	,	3	,	5	,	13	,	63	,	71	,	117	,	209	,	955	,	1175	,	317	,	4261	,	2341	,	29819	};
            const unsigned long	dim2873KuoInit[]		=	{	1	,	3	,	5	,	5	,	9	,	59	,	109	,	137	,	353	,	355	,	1227	,	3183	,	1553	,	15039	,	27979	};
            const unsigned long	dim2874KuoInit[]		=	{	1	,	3	,	1	,	5	,	25	,	57	,	87	,	231	,	259	,	333	,	975	,	3413	,	6035	,	14783	,	22309	};
            const unsigned long	dim2875KuoInit[]		=	{	1	,	3	,	5	,	11	,	19	,	37	,	55	,	229	,	423	,	761	,	1669	,	3023	,	2517	,	2961	,	12043	};
            const unsigned long	dim2876KuoInit[]		=	{	1	,	1	,	3	,	13	,	23	,	5	,	87	,	193	,	113	,	577	,	269	,	983	,	5453	,	7827	,	28197	};
            const unsigned long	dim2877KuoInit[]		=	{	1	,	1	,	7	,	13	,	1	,	49	,	89	,	107	,	155	,	767	,	1547	,	2139	,	5951	,	3143	,	12735	};
            const unsigned long	dim2878KuoInit[]		=	{	1	,	3	,	1	,	13	,	13	,	57	,	53	,	243	,	231	,	131	,	1505	,	1255	,	7831	,	15615	,	28755	};
            const unsigned long	dim2879KuoInit[]		=	{	1	,	1	,	5	,	11	,	21	,	21	,	37	,	65	,	327	,	353	,	1625	,	543	,	669	,	1139	,	20745	};
            const unsigned long	dim2880KuoInit[]		=	{	1	,	3	,	1	,	1	,	5	,	33	,	87	,	211	,	21	,	581	,	397	,	2691	,	1069	,	10449	,	17727	};
            const unsigned long	dim2881KuoInit[]		=	{	1	,	1	,	7	,	7	,	1	,	5	,	115	,	143	,	99	,	523	,	51	,	939	,	4635	,	11491	,	32731	};
            const unsigned long	dim2882KuoInit[]		=	{	1	,	3	,	3	,	15	,	1	,	19	,	119	,	193	,	127	,	897	,	1337	,	3977	,	5027	,	5913	,	9329	};
            const unsigned long	dim2883KuoInit[]		=	{	1	,	1	,	5	,	15	,	15	,	1	,	93	,	209	,	427	,	625	,	1581	,	2639	,	1291	,	11177	,	26943	};
            const unsigned long	dim2884KuoInit[]		=	{	1	,	1	,	3	,	13	,	27	,	9	,	47	,	243	,	211	,	21	,	1159	,	3157	,	6729	,	219	,	19671	};
            const unsigned long	dim2885KuoInit[]		=	{	1	,	3	,	1	,	15	,	19	,	27	,	39	,	237	,	39	,	389	,	1201	,	2237	,	7623	,	12041	,	18491	};
            const unsigned long	dim2886KuoInit[]		=	{	1	,	1	,	3	,	5	,	25	,	43	,	47	,	101	,	319	,	749	,	659	,	2319	,	1221	,	3887	,	29927	};
            const unsigned long	dim2887KuoInit[]		=	{	1	,	3	,	3	,	9	,	1	,	55	,	57	,	179	,	163	,	695	,	611	,	2511	,	1261	,	15561	,	29535	};
            const unsigned long	dim2888KuoInit[]		=	{	1	,	1	,	1	,	13	,	7	,	31	,	79	,	21	,	143	,	993	,	377	,	791	,	187	,	6485	,	6827	};
            const unsigned long	dim2889KuoInit[]		=	{	1	,	3	,	5	,	1	,	11	,	57	,	39	,	159	,	373	,	179	,	1907	,	2475	,	1355	,	9885	,	18683	};
            const unsigned long	dim2890KuoInit[]		=	{	1	,	1	,	1	,	3	,	7	,	5	,	83	,	151	,	267	,	377	,	1463	,	365	,	4725	,	2319	,	18957	};
            const unsigned long	dim2891KuoInit[]		=	{	1	,	3	,	7	,	3	,	21	,	51	,	115	,	119	,	73	,	561	,	1513	,	403	,	4889	,	2329	,	20141	};
            const unsigned long	dim2892KuoInit[]		=	{	1	,	1	,	1	,	15	,	21	,	47	,	31	,	61	,	171	,	529	,	185	,	717	,	7065	,	4273	,	16643	};
            const unsigned long	dim2893KuoInit[]		=	{	1	,	3	,	5	,	9	,	3	,	55	,	41	,	213	,	489	,	379	,	1951	,	1957	,	7721	,	6415	,	27049	};
            const unsigned long	dim2894KuoInit[]		=	{	1	,	3	,	1	,	13	,	19	,	43	,	29	,	203	,	139	,	137	,	463	,	1447	,	2585	,	14381	,	29073	};
            const unsigned long	dim2895KuoInit[]		=	{	1	,	1	,	1	,	1	,	19	,	37	,	111	,	119	,	411	,	671	,	1135	,	2891	,	7929	,	7465	,	21699	};
            const unsigned long	dim2896KuoInit[]		=	{	1	,	1	,	5	,	13	,	21	,	31	,	83	,	227	,	435	,	61	,	969	,	2171	,	789	,	3415	,	18293	};
            const unsigned long	dim2897KuoInit[]		=	{	1	,	3	,	3	,	15	,	31	,	33	,	87	,	227	,	369	,	341	,	1515	,	25	,	2891	,	16199	,	22179	};
            const unsigned long	dim2898KuoInit[]		=	{	1	,	3	,	3	,	1	,	25	,	49	,	111	,	167	,	59	,	355	,	1687	,	1799	,	5963	,	6161	,	32157	};
            const unsigned long	dim2899KuoInit[]		=	{	1	,	1	,	1	,	5	,	7	,	57	,	95	,	171	,	417	,	277	,	47	,	847	,	3889	,	8645	,	3367	};
            const unsigned long	dim2900KuoInit[]		=	{	1	,	1	,	7	,	3	,	25	,	51	,	99	,	251	,	463	,	833	,	491	,	3595	,	2883	,	8183	,	28937	};
            const unsigned long	dim2901KuoInit[]		=	{	1	,	1	,	3	,	7	,	1	,	5	,	73	,	151	,	397	,	521	,	905	,	603	,	2315	,	15231	,	17955	};
            const unsigned long	dim2902KuoInit[]		=	{	1	,	1	,	1	,	13	,	23	,	59	,	9	,	187	,	163	,	727	,	1909	,	1135	,	6135	,	11563	,	2047	};
            const unsigned long	dim2903KuoInit[]		=	{	1	,	1	,	3	,	3	,	9	,	1	,	121	,	207	,	55	,	947	,	873	,	419	,	2799	,	3285	,	10237	};
            const unsigned long	dim2904KuoInit[]		=	{	1	,	1	,	1	,	3	,	21	,	27	,	27	,	241	,	509	,	295	,	1509	,	3527	,	4269	,	13579	,	29125	};
            const unsigned long	dim2905KuoInit[]		=	{	1	,	3	,	1	,	5	,	7	,	21	,	35	,	67	,	251	,	883	,	1117	,	1311	,	1251	,	10401	,	10615	};
            const unsigned long	dim2906KuoInit[]		=	{	1	,	3	,	7	,	3	,	21	,	13	,	23	,	75	,	401	,	279	,	1557	,	1419	,	7067	,	2563	,	22285	};
            const unsigned long	dim2907KuoInit[]		=	{	1	,	3	,	1	,	7	,	23	,	15	,	11	,	189	,	315	,	353	,	1377	,	3551	,	4939	,	14529	,	12459	};
            const unsigned long	dim2908KuoInit[]		=	{	1	,	3	,	1	,	3	,	21	,	41	,	111	,	67	,	311	,	95	,	1919	,	165	,	2097	,	12965	,	12517	};
            const unsigned long	dim2909KuoInit[]		=	{	1	,	3	,	3	,	11	,	29	,	61	,	33	,	35	,	209	,	541	,	149	,	3073	,	8187	,	15087	,	17617	};
            const unsigned long	dim2910KuoInit[]		=	{	1	,	1	,	7	,	5	,	9	,	53	,	73	,	145	,	205	,	157	,	1737	,	1737	,	6471	,	12327	,	11605	};
            const unsigned long	dim2911KuoInit[]		=	{	1	,	1	,	5	,	5	,	15	,	35	,	105	,	99	,	467	,	421	,	483	,	3191	,	2321	,	8953	,	22045	};
            const unsigned long	dim2912KuoInit[]		=	{	1	,	3	,	3	,	9	,	1	,	35	,	65	,	249	,	37	,	535	,	1301	,	2141	,	1629	,	9709	,	995	};
            const unsigned long	dim2913KuoInit[]		=	{	1	,	3	,	1	,	13	,	9	,	27	,	75	,	115	,	121	,	731	,	1567	,	2341	,	6657	,	14959	,	6781	};
            const unsigned long	dim2914KuoInit[]		=	{	1	,	1	,	1	,	5	,	13	,	33	,	89	,	215	,	189	,	693	,	357	,	3383	,	6345	,	14263	,	23973	};
            const unsigned long	dim2915KuoInit[]		=	{	1	,	3	,	1	,	9	,	21	,	5	,	59	,	11	,	21	,	703	,	1217	,	2623	,	6429	,	479	,	28659	};
            const unsigned long	dim2916KuoInit[]		=	{	1	,	3	,	3	,	9	,	11	,	15	,	113	,	89	,	453	,	233	,	1323	,	3859	,	1841	,	2753	,	2839	};
            const unsigned long	dim2917KuoInit[]		=	{	1	,	1	,	5	,	15	,	19	,	1	,	119	,	83	,	205	,	861	,	787	,	1039	,	5257	,	8945	,	27271	};
            const unsigned long	dim2918KuoInit[]		=	{	1	,	3	,	1	,	15	,	9	,	37	,	7	,	83	,	51	,	611	,	1121	,	3185	,	1603	,	5821	,	14533	};
            const unsigned long	dim2919KuoInit[]		=	{	1	,	1	,	7	,	7	,	27	,	61	,	115	,	191	,	357	,	93	,	1647	,	3249	,	2479	,	2423	,	649	};
            const unsigned long	dim2920KuoInit[]		=	{	1	,	1	,	7	,	11	,	25	,	17	,	13	,	213	,	37	,	571	,	1179	,	3897	,	3817	,	7597	,	26239	};
            const unsigned long	dim2921KuoInit[]		=	{	1	,	1	,	5	,	5	,	27	,	57	,	73	,	209	,	433	,	779	,	75	,	2135	,	8183	,	7343	,	23751	};
            const unsigned long	dim2922KuoInit[]		=	{	1	,	1	,	7	,	13	,	27	,	51	,	31	,	33	,	195	,	443	,	1639	,	1415	,	6203	,	4373	,	31271	};
            const unsigned long	dim2923KuoInit[]		=	{	1	,	1	,	7	,	11	,	9	,	9	,	75	,	103	,	237	,	521	,	1653	,	1209	,	5981	,	5961	,	15229	};
            const unsigned long	dim2924KuoInit[]		=	{	1	,	3	,	3	,	11	,	15	,	29	,	1	,	201	,	171	,	81	,	729	,	589	,	441	,	2453	,	25209	};
            const unsigned long	dim2925KuoInit[]		=	{	1	,	3	,	1	,	1	,	21	,	11	,	95	,	225	,	213	,	981	,	1479	,	3259	,	4415	,	8393	,	17647	};
            const unsigned long	dim2926KuoInit[]		=	{	1	,	1	,	7	,	1	,	7	,	11	,	95	,	135	,	329	,	483	,	1711	,	3223	,	4841	,	11083	,	4033	};
            const unsigned long	dim2927KuoInit[]		=	{	1	,	1	,	3	,	5	,	23	,	37	,	77	,	39	,	363	,	555	,	1451	,	99	,	6963	,	13279	,	24155	};
            const unsigned long	dim2928KuoInit[]		=	{	1	,	3	,	7	,	11	,	11	,	17	,	31	,	29	,	357	,	177	,	411	,	1439	,	815	,	7017	,	25527	};
            const unsigned long	dim2929KuoInit[]		=	{	1	,	3	,	3	,	15	,	25	,	7	,	105	,	9	,	301	,	209	,	917	,	1369	,	2941	,	11599	,	25847	};
            const unsigned long	dim2930KuoInit[]		=	{	1	,	1	,	7	,	7	,	11	,	33	,	121	,	9	,	465	,	945	,	201	,	3929	,	5521	,	4787	,	10713	};
            const unsigned long	dim2931KuoInit[]		=	{	1	,	1	,	7	,	9	,	27	,	3	,	77	,	11	,	103	,	181	,	1199	,	2833	,	6259	,	3879	,	13217	};
            const unsigned long	dim2932KuoInit[]		=	{	1	,	3	,	3	,	11	,	11	,	9	,	5	,	217	,	471	,	973	,	293	,	3693	,	5457	,	11359	,	30043	};
            const unsigned long	dim2933KuoInit[]		=	{	1	,	1	,	3	,	15	,	27	,	25	,	7	,	65	,	95	,	883	,	277	,	3147	,	6203	,	12201	,	23203	};
            const unsigned long	dim2934KuoInit[]		=	{	1	,	1	,	3	,	9	,	3	,	19	,	51	,	55	,	381	,	879	,	845	,	1605	,	2433	,	4781	,	5075	};
            const unsigned long	dim2935KuoInit[]		=	{	1	,	1	,	3	,	15	,	17	,	49	,	21	,	81	,	209	,	285	,	1483	,	1897	,	5591	,	131	,	30459	};
            const unsigned long	dim2936KuoInit[]		=	{	1	,	3	,	1	,	9	,	17	,	19	,	47	,	97	,	325	,	963	,	1665	,	3361	,	369	,	11023	,	23701	};
            const unsigned long	dim2937KuoInit[]		=	{	1	,	3	,	1	,	13	,	3	,	19	,	57	,	7	,	57	,	383	,	1945	,	3357	,	2553	,	5161	,	22363	};
            const unsigned long	dim2938KuoInit[]		=	{	1	,	1	,	3	,	15	,	15	,	37	,	41	,	123	,	453	,	879	,	337	,	251	,	7137	,	4117	,	30879	};
            const unsigned long	dim2939KuoInit[]		=	{	1	,	3	,	5	,	3	,	13	,	45	,	55	,	151	,	357	,	429	,	671	,	1183	,	3047	,	4991	,	15273	};
            const unsigned long	dim2940KuoInit[]		=	{	1	,	1	,	3	,	3	,	15	,	15	,	65	,	131	,	505	,	339	,	869	,	2983	,	4451	,	2375	,	26745	};
            const unsigned long	dim2941KuoInit[]		=	{	1	,	3	,	5	,	9	,	15	,	55	,	23	,	83	,	255	,	395	,	15	,	1175	,	849	,	5959	,	17355	};
            const unsigned long	dim2942KuoInit[]		=	{	1	,	1	,	3	,	7	,	9	,	3	,	57	,	95	,	7	,	267	,	801	,	3843	,	3139	,	12565	,	20737	};
            const unsigned long	dim2943KuoInit[]		=	{	1	,	3	,	7	,	7	,	9	,	41	,	121	,	77	,	489	,	975	,	215	,	325	,	7505	,	15517	,	8833	};
            const unsigned long	dim2944KuoInit[]		=	{	1	,	3	,	3	,	1	,	9	,	43	,	105	,	199	,	339	,	335	,	1719	,	1999	,	4153	,	9665	,	15457	};
            const unsigned long	dim2945KuoInit[]		=	{	1	,	3	,	1	,	13	,	1	,	35	,	47	,	157	,	3	,	93	,	1399	,	471	,	6337	,	2789	,	32727	};
            const unsigned long	dim2946KuoInit[]		=	{	1	,	3	,	5	,	1	,	29	,	9	,	101	,	195	,	371	,	573	,	1361	,	521	,	5737	,	4845	,	4871	};
            const unsigned long	dim2947KuoInit[]		=	{	1	,	1	,	7	,	9	,	9	,	11	,	43	,	129	,	295	,	1015	,	665	,	451	,	1249	,	7733	,	8861	};
            const unsigned long	dim2948KuoInit[]		=	{	1	,	1	,	1	,	11	,	19	,	31	,	127	,	171	,	341	,	865	,	1391	,	955	,	5585	,	1481	,	1311	};
            const unsigned long	dim2949KuoInit[]		=	{	1	,	3	,	1	,	11	,	19	,	43	,	113	,	83	,	285	,	865	,	815	,	807	,	779	,	6685	,	22315	};
            const unsigned long	dim2950KuoInit[]		=	{	1	,	1	,	1	,	11	,	1	,	19	,	99	,	19	,	455	,	155	,	637	,	2153	,	3013	,	4791	,	20619	};
            const unsigned long	dim2951KuoInit[]		=	{	1	,	1	,	1	,	1	,	13	,	39	,	95	,	229	,	35	,	515	,	1945	,	3631	,	3359	,	7913	,	8081	};
            const unsigned long	dim2952KuoInit[]		=	{	1	,	3	,	7	,	13	,	1	,	5	,	25	,	227	,	487	,	269	,	1213	,	303	,	4041	,	15847	,	1125	};
            const unsigned long	dim2953KuoInit[]		=	{	1	,	1	,	3	,	7	,	27	,	53	,	121	,	143	,	1	,	977	,	1963	,	807	,	253	,	7649	,	16047	};
            const unsigned long	dim2954KuoInit[]		=	{	1	,	3	,	5	,	9	,	29	,	17	,	97	,	93	,	83	,	739	,	1221	,	963	,	6849	,	4307	,	17277	};
            const unsigned long	dim2955KuoInit[]		=	{	1	,	1	,	7	,	1	,	27	,	55	,	71	,	177	,	63	,	11	,	1781	,	3341	,	2797	,	8035	,	6135	};
            const unsigned long	dim2956KuoInit[]		=	{	1	,	1	,	1	,	11	,	15	,	43	,	59	,	107	,	485	,	995	,	1571	,	1527	,	2195	,	4883	,	32563	};
            const unsigned long	dim2957KuoInit[]		=	{	1	,	3	,	5	,	15	,	7	,	45	,	55	,	211	,	425	,	771	,	621	,	1141	,	5277	,	13575	,	203	};
            const unsigned long	dim2958KuoInit[]		=	{	1	,	1	,	7	,	1	,	21	,	51	,	81	,	41	,	71	,	551	,	839	,	3315	,	5075	,	3651	,	6649	};
            const unsigned long	dim2959KuoInit[]		=	{	1	,	1	,	1	,	9	,	17	,	45	,	51	,	211	,	349	,	329	,	987	,	3153	,	7121	,	16101	,	1475	};
            const unsigned long	dim2960KuoInit[]		=	{	1	,	3	,	3	,	9	,	5	,	5	,	99	,	17	,	245	,	827	,	231	,	949	,	2577	,	12925	,	2055	};
            const unsigned long	dim2961KuoInit[]		=	{	1	,	1	,	5	,	1	,	19	,	59	,	75	,	97	,	303	,	649	,	901	,	3987	,	1209	,	14483	,	25855	};
            const unsigned long	dim2962KuoInit[]		=	{	1	,	3	,	7	,	15	,	25	,	51	,	5	,	69	,	199	,	845	,	1847	,	1491	,	6183	,	3359	,	11809	};
            const unsigned long	dim2963KuoInit[]		=	{	1	,	1	,	5	,	5	,	19	,	43	,	17	,	113	,	215	,	337	,	583	,	2199	,	6375	,	499	,	11283	};
            const unsigned long	dim2964KuoInit[]		=	{	1	,	1	,	5	,	15	,	5	,	3	,	45	,	219	,	375	,	269	,	1251	,	2711	,	7897	,	3379	,	11887	};
            const unsigned long	dim2965KuoInit[]		=	{	1	,	1	,	7	,	13	,	3	,	59	,	17	,	41	,	275	,	1007	,	711	,	1689	,	3727	,	14237	,	32057	};
            const unsigned long	dim2966KuoInit[]		=	{	1	,	3	,	5	,	7	,	5	,	27	,	73	,	167	,	295	,	581	,	233	,	585	,	7207	,	12709	,	3847	};
            const unsigned long	dim2967KuoInit[]		=	{	1	,	1	,	7	,	5	,	9	,	17	,	65	,	15	,	439	,	385	,	455	,	1073	,	5727	,	2679	,	3689	};
            const unsigned long	dim2968KuoInit[]		=	{	1	,	1	,	7	,	5	,	7	,	63	,	111	,	161	,	85	,	295	,	755	,	2805	,	3031	,	4833	,	11149	};
            const unsigned long	dim2969KuoInit[]		=	{	1	,	1	,	5	,	7	,	13	,	53	,	63	,	47	,	163	,	275	,	289	,	1377	,	639	,	15367	,	32517	};
            const unsigned long	dim2970KuoInit[]		=	{	1	,	1	,	7	,	9	,	21	,	43	,	123	,	221	,	451	,	901	,	1339	,	3367	,	2531	,	9319	,	4461	};
            const unsigned long	dim2971KuoInit[]		=	{	1	,	3	,	3	,	9	,	3	,	53	,	87	,	117	,	471	,	349	,	1487	,	2375	,	7399	,	8561	,	19611	};
            const unsigned long	dim2972KuoInit[]		=	{	1	,	3	,	1	,	7	,	29	,	55	,	53	,	197	,	301	,	701	,	357	,	3651	,	2233	,	6365	,	705	};
            const unsigned long	dim2973KuoInit[]		=	{	1	,	3	,	5	,	3	,	17	,	19	,	23	,	17	,	433	,	979	,	1377	,	213	,	3993	,	3773	,	24369	};
            const unsigned long	dim2974KuoInit[]		=	{	1	,	3	,	5	,	5	,	17	,	9	,	105	,	149	,	145	,	751	,	1173	,	1831	,	7005	,	3559	,	10913	};
            const unsigned long	dim2975KuoInit[]		=	{	1	,	3	,	1	,	9	,	23	,	47	,	83	,	213	,	135	,	223	,	937	,	3607	,	7375	,	4731	,	14307	};
            const unsigned long	dim2976KuoInit[]		=	{	1	,	1	,	7	,	3	,	23	,	21	,	71	,	241	,	399	,	199	,	289	,	2049	,	7959	,	14897	,	18661	};
            const unsigned long	dim2977KuoInit[]		=	{	1	,	3	,	3	,	9	,	13	,	45	,	125	,	217	,	325	,	893	,	1523	,	2477	,	7829	,	15737	,	2127	};
            const unsigned long	dim2978KuoInit[]		=	{	1	,	1	,	5	,	1	,	21	,	51	,	117	,	203	,	253	,	713	,	545	,	191	,	729	,	8403	,	8381	};
            const unsigned long	dim2979KuoInit[]		=	{	1	,	3	,	7	,	7	,	11	,	23	,	3	,	193	,	421	,	341	,	1309	,	2457	,	5805	,	7457	,	26857	};
            const unsigned long	dim2980KuoInit[]		=	{	1	,	3	,	7	,	15	,	1	,	17	,	27	,	159	,	127	,	713	,	1465	,	3551	,	7409	,	7957	,	2317	};
            const unsigned long	dim2981KuoInit[]		=	{	1	,	3	,	1	,	15	,	7	,	33	,	17	,	1	,	237	,	199	,	527	,	3361	,	3785	,	13111	,	19119	};
            const unsigned long	dim2982KuoInit[]		=	{	1	,	3	,	3	,	1	,	23	,	37	,	91	,	31	,	35	,	969	,	661	,	85	,	5917	,	10895	,	11049	};
            const unsigned long	dim2983KuoInit[]		=	{	1	,	3	,	1	,	7	,	23	,	53	,	101	,	181	,	381	,	159	,	1967	,	3427	,	4499	,	6877	,	12155	};
            const unsigned long	dim2984KuoInit[]		=	{	1	,	3	,	3	,	13	,	9	,	57	,	77	,	27	,	223	,	101	,	1709	,	3023	,	6655	,	6221	,	1027	};
            const unsigned long	dim2985KuoInit[]		=	{	1	,	3	,	5	,	7	,	19	,	47	,	73	,	175	,	103	,	35	,	1039	,	343	,	6757	,	11521	,	24495	};
            const unsigned long	dim2986KuoInit[]		=	{	1	,	3	,	1	,	9	,	17	,	19	,	9	,	205	,	279	,	373	,	173	,	3641	,	3945	,	10831	,	15621	};
            const unsigned long	dim2987KuoInit[]		=	{	1	,	1	,	3	,	13	,	29	,	63	,	71	,	217	,	367	,	381	,	647	,	9	,	5521	,	5209	,	24275	};
            const unsigned long	dim2988KuoInit[]		=	{	1	,	3	,	7	,	7	,	23	,	15	,	7	,	203	,	153	,	655	,	2047	,	287	,	973	,	6757	,	29207	};
            const unsigned long	dim2989KuoInit[]		=	{	1	,	1	,	1	,	9	,	17	,	33	,	63	,	97	,	49	,	31	,	1989	,	569	,	3321	,	4017	,	6437	};
            const unsigned long	dim2990KuoInit[]		=	{	1	,	1	,	7	,	11	,	15	,	15	,	57	,	255	,	381	,	115	,	573	,	3903	,	7141	,	1569	,	11789	};
            const unsigned long	dim2991KuoInit[]		=	{	1	,	1	,	5	,	13	,	9	,	45	,	69	,	111	,	91	,	389	,	1347	,	3351	,	3023	,	517	,	28373	};
            const unsigned long	dim2992KuoInit[]		=	{	1	,	3	,	3	,	15	,	9	,	11	,	81	,	205	,	299	,	903	,	661	,	3377	,	5355	,	9151	,	17597	};
            const unsigned long	dim2993KuoInit[]		=	{	1	,	1	,	3	,	9	,	19	,	55	,	45	,	123	,	285	,	161	,	1591	,	1971	,	5471	,	8221	,	2855	};
            const unsigned long	dim2994KuoInit[]		=	{	1	,	3	,	5	,	13	,	23	,	25	,	87	,	197	,	303	,	487	,	395	,	3137	,	4395	,	5861	,	5847	};
            const unsigned long	dim2995KuoInit[]		=	{	1	,	3	,	7	,	5	,	23	,	9	,	107	,	13	,	127	,	971	,	1701	,	2387	,	6195	,	5041	,	24857	};
            const unsigned long	dim2996KuoInit[]		=	{	1	,	1	,	3	,	3	,	19	,	63	,	91	,	205	,	207	,	897	,	295	,	2093	,	1223	,	14599	,	13803	};
            const unsigned long	dim2997KuoInit[]		=	{	1	,	1	,	5	,	1	,	1	,	25	,	93	,	33	,	393	,	221	,	1845	,	2517	,	1723	,	13881	,	15777	};
            const unsigned long	dim2998KuoInit[]		=	{	1	,	3	,	5	,	3	,	1	,	21	,	115	,	103	,	377	,	789	,	541	,	3423	,	3111	,	8077	,	25089	};
            const unsigned long	dim2999KuoInit[]		=	{	1	,	1	,	1	,	11	,	31	,	55	,	95	,	53	,	141	,	697	,	1085	,	3031	,	653	,	4145	,	28121	};
            const unsigned long	dim3000KuoInit[]		=	{	1	,	3	,	1	,	1	,	7	,	39	,	55	,	57	,	447	,	257	,	101	,	2307	,	5623	,	8505	,	995	};
            const unsigned long	dim3001KuoInit[]		=	{	1	,	3	,	5	,	11	,	25	,	37	,	11	,	203	,	361	,	281	,	701	,	3643	,	957	,	14541	,	8109	};
            const unsigned long	dim3002KuoInit[]		=	{	1	,	3	,	1	,	3	,	21	,	49	,	57	,	225	,	21	,	523	,	1275	,	3743	,	6259	,	13625	,	26721	};
            const unsigned long	dim3003KuoInit[]		=	{	1	,	1	,	1	,	1	,	13	,	53	,	9	,	167	,	117	,	971	,	875	,	2659	,	7589	,	13575	,	11659	};
            const unsigned long	dim3004KuoInit[]		=	{	1	,	1	,	7	,	13	,	21	,	1	,	35	,	201	,	261	,	533	,	1069	,	1459	,	5433	,	7211	,	3535	};
            const unsigned long	dim3005KuoInit[]		=	{	1	,	1	,	3	,	13	,	23	,	7	,	41	,	137	,	341	,	767	,	1823	,	3745	,	1415	,	4811	,	21669	};
            const unsigned long	dim3006KuoInit[]		=	{	1	,	3	,	3	,	5	,	5	,	11	,	71	,	117	,	451	,	1	,	381	,	967	,	7977	,	10741	,	20141	};
            const unsigned long	dim3007KuoInit[]		=	{	1	,	3	,	1	,	3	,	21	,	1	,	65	,	37	,	451	,	29	,	327	,	2471	,	7573	,	5051	,	23925	};
            const unsigned long	dim3008KuoInit[]		=	{	1	,	3	,	3	,	11	,	17	,	61	,	97	,	121	,	327	,	929	,	1165	,	185	,	2343	,	1479	,	3817	};
            const unsigned long	dim3009KuoInit[]		=	{	1	,	1	,	3	,	7	,	27	,	55	,	87	,	53	,	389	,	577	,	1051	,	2413	,	609	,	12651	,	15695	};
            const unsigned long	dim3010KuoInit[]		=	{	1	,	3	,	5	,	13	,	1	,	41	,	25	,	149	,	175	,	667	,	1219	,	2297	,	6599	,	1667	,	19647	};
            const unsigned long	dim3011KuoInit[]		=	{	1	,	1	,	3	,	11	,	17	,	63	,	83	,	37	,	425	,	675	,	1111	,	499	,	7727	,	7491	,	9759	};
            const unsigned long	dim3012KuoInit[]		=	{	1	,	1	,	3	,	1	,	21	,	23	,	17	,	125	,	485	,	643	,	341	,	1159	,	7073	,	14405	,	22409	};
            const unsigned long	dim3013KuoInit[]		=	{	1	,	3	,	7	,	15	,	15	,	53	,	23	,	17	,	365	,	101	,	685	,	1391	,	5123	,	14961	,	31179	};
            const unsigned long	dim3014KuoInit[]		=	{	1	,	3	,	3	,	3	,	23	,	11	,	101	,	139	,	401	,	779	,	1723	,	3421	,	623	,	3443	,	17179	};
            const unsigned long	dim3015KuoInit[]		=	{	1	,	1	,	1	,	7	,	13	,	19	,	93	,	129	,	127	,	389	,	1361	,	2341	,	6491	,	15063	,	8199	};
            const unsigned long	dim3016KuoInit[]		=	{	1	,	1	,	7	,	1	,	31	,	1	,	55	,	169	,	373	,	69	,	539	,	513	,	6329	,	393	,	21963	};
            const unsigned long	dim3017KuoInit[]		=	{	1	,	1	,	7	,	1	,	5	,	39	,	73	,	87	,	401	,	923	,	1747	,	565	,	2227	,	14509	,	7601	};
            const unsigned long	dim3018KuoInit[]		=	{	1	,	3	,	3	,	1	,	25	,	3	,	77	,	207	,	149	,	591	,	1165	,	3235	,	7615	,	12237	,	20849	};
            const unsigned long	dim3019KuoInit[]		=	{	1	,	3	,	1	,	11	,	21	,	41	,	83	,	109	,	265	,	349	,	485	,	1411	,	2705	,	13017	,	25205	};
            const unsigned long	dim3020KuoInit[]		=	{	1	,	1	,	7	,	7	,	31	,	27	,	55	,	23	,	129	,	369	,	1785	,	2031	,	1993	,	123	,	20063	};
            const unsigned long	dim3021KuoInit[]		=	{	1	,	3	,	5	,	9	,	9	,	45	,	75	,	37	,	69	,	957	,	2047	,	2515	,	5327	,	15345	,	2147	};
            const unsigned long	dim3022KuoInit[]		=	{	1	,	3	,	1	,	7	,	5	,	57	,	91	,	19	,	499	,	173	,	765	,	3653	,	6397	,	1509	,	11223	};
            const unsigned long	dim3023KuoInit[]		=	{	1	,	3	,	5	,	15	,	1	,	15	,	69	,	107	,	225	,	233	,	1015	,	807	,	4265	,	8823	,	22047	};
            const unsigned long	dim3024KuoInit[]		=	{	1	,	1	,	7	,	13	,	15	,	55	,	15	,	83	,	49	,	711	,	935	,	1865	,	29	,	607	,	8113	};
            const unsigned long	dim3025KuoInit[]		=	{	1	,	3	,	1	,	15	,	13	,	15	,	127	,	193	,	219	,	653	,	1787	,	905	,	3311	,	11595	,	21193	};
            const unsigned long	dim3026KuoInit[]		=	{	1	,	3	,	5	,	11	,	21	,	51	,	67	,	97	,	371	,	367	,	1763	,	2953	,	3701	,	15983	,	18631	};
            const unsigned long	dim3027KuoInit[]		=	{	1	,	1	,	5	,	9	,	13	,	1	,	83	,	15	,	305	,	939	,	1073	,	1577	,	2073	,	5335	,	17173	};
            const unsigned long	dim3028KuoInit[]		=	{	1	,	3	,	5	,	7	,	19	,	7	,	27	,	213	,	357	,	681	,	1969	,	4007	,	7887	,	11191	,	19395	};
            const unsigned long	dim3029KuoInit[]		=	{	1	,	1	,	5	,	3	,	19	,	29	,	119	,	239	,	123	,	559	,	1613	,	1563	,	8047	,	5753	,	26719	};
            const unsigned long	dim3030KuoInit[]		=	{	1	,	3	,	7	,	5	,	3	,	3	,	127	,	241	,	333	,	453	,	1407	,	2687	,	4603	,	3637	,	23041	};
            const unsigned long	dim3031KuoInit[]		=	{	1	,	1	,	1	,	15	,	17	,	9	,	7	,	31	,	145	,	613	,	1851	,	1981	,	3921	,	6469	,	32397	};
            const unsigned long	dim3032KuoInit[]		=	{	1	,	1	,	5	,	3	,	9	,	39	,	113	,	79	,	165	,	359	,	1573	,	2311	,	651	,	7047	,	15961	};
            const unsigned long	dim3033KuoInit[]		=	{	1	,	3	,	7	,	7	,	11	,	57	,	59	,	153	,	363	,	279	,	77	,	3085	,	5971	,	29	,	22087	};
            const unsigned long	dim3034KuoInit[]		=	{	1	,	1	,	7	,	11	,	25	,	47	,	49	,	229	,	147	,	777	,	1177	,	1117	,	3783	,	3681	,	1465	};
            const unsigned long	dim3035KuoInit[]		=	{	1	,	3	,	5	,	11	,	3	,	45	,	11	,	139	,	393	,	9	,	471	,	1643	,	4963	,	5837	,	17023	};
            const unsigned long	dim3036KuoInit[]		=	{	1	,	3	,	1	,	7	,	5	,	41	,	123	,	113	,	223	,	237	,	239	,	1099	,	6065	,	11105	,	10259	};
            const unsigned long	dim3037KuoInit[]		=	{	1	,	1	,	3	,	11	,	15	,	57	,	41	,	173	,	169	,	585	,	201	,	2133	,	2629	,	12861	,	16559	};
            const unsigned long	dim3038KuoInit[]		=	{	1	,	1	,	7	,	5	,	9	,	27	,	3	,	157	,	279	,	393	,	1849	,	437	,	5549	,	10009	,	11783	};
            const unsigned long	dim3039KuoInit[]		=	{	1	,	1	,	7	,	9	,	17	,	23	,	67	,	143	,	1	,	203	,	153	,	1699	,	1333	,	16001	,	11755	};
            const unsigned long	dim3040KuoInit[]		=	{	1	,	3	,	5	,	15	,	17	,	61	,	59	,	235	,	67	,	393	,	1597	,	3523	,	2049	,	11887	,	7737	};
            const unsigned long	dim3041KuoInit[]		=	{	1	,	3	,	5	,	13	,	21	,	45	,	5	,	221	,	299	,	887	,	699	,	959	,	1143	,	4879	,	8767	};
            const unsigned long	dim3042KuoInit[]		=	{	1	,	3	,	7	,	15	,	11	,	35	,	83	,	237	,	167	,	1	,	1429	,	2877	,	4611	,	14881	,	4453	};
            const unsigned long	dim3043KuoInit[]		=	{	1	,	3	,	5	,	3	,	7	,	55	,	21	,	237	,	9	,	743	,	1707	,	1229	,	6827	,	407	,	777	};
            const unsigned long	dim3044KuoInit[]		=	{	1	,	3	,	7	,	3	,	15	,	39	,	3	,	127	,	163	,	747	,	1773	,	1569	,	2377	,	15023	,	32249	};
            const unsigned long	dim3045KuoInit[]		=	{	1	,	1	,	7	,	7	,	19	,	33	,	1	,	55	,	319	,	585	,	413	,	1241	,	95	,	7163	,	29935	};
            const unsigned long	dim3046KuoInit[]		=	{	1	,	3	,	1	,	1	,	7	,	51	,	77	,	197	,	481	,	517	,	1441	,	1779	,	1405	,	3633	,	32545	};
            const unsigned long	dim3047KuoInit[]		=	{	1	,	3	,	7	,	5	,	13	,	51	,	69	,	35	,	131	,	115	,	1759	,	3991	,	8065	,	10311	,	12749	};
            const unsigned long	dim3048KuoInit[]		=	{	1	,	3	,	3	,	5	,	5	,	35	,	37	,	181	,	371	,	543	,	1389	,	815	,	3261	,	11997	,	29665	};
            const unsigned long	dim3049KuoInit[]		=	{	1	,	3	,	7	,	11	,	31	,	47	,	33	,	147	,	445	,	663	,	63	,	1033	,	6081	,	15523	,	28477	};
            const unsigned long	dim3050KuoInit[]		=	{	1	,	1	,	7	,	11	,	11	,	9	,	1	,	39	,	357	,	873	,	1829	,	3245	,	3219	,	601	,	1335	};
            const unsigned long	dim3051KuoInit[]		=	{	1	,	1	,	5	,	7	,	5	,	5	,	33	,	145	,	465	,	5	,	1959	,	3573	,	3791	,	8689	,	14513	};
            const unsigned long	dim3052KuoInit[]		=	{	1	,	1	,	1	,	13	,	31	,	37	,	103	,	255	,	87	,	903	,	1325	,	1423	,	8031	,	14845	,	12261	};
            const unsigned long	dim3053KuoInit[]		=	{	1	,	3	,	3	,	13	,	31	,	25	,	41	,	45	,	261	,	225	,	1711	,	641	,	2331	,	10825	,	21539	};
            const unsigned long	dim3054KuoInit[]		=	{	1	,	3	,	1	,	9	,	9	,	33	,	23	,	135	,	165	,	363	,	1435	,	3019	,	4907	,	9791	,	1337	};
            const unsigned long	dim3055KuoInit[]		=	{	1	,	3	,	3	,	3	,	13	,	23	,	109	,	161	,	107	,	829	,	1635	,	791	,	2757	,	15265	,	8771	};
            const unsigned long	dim3056KuoInit[]		=	{	1	,	1	,	1	,	11	,	7	,	17	,	45	,	167	,	69	,	437	,	1051	,	1443	,	875	,	2461	,	28471	};
            const unsigned long	dim3057KuoInit[]		=	{	1	,	3	,	5	,	11	,	23	,	43	,	55	,	69	,	365	,	497	,	1529	,	1101	,	211	,	8145	,	10579	};
            const unsigned long	dim3058KuoInit[]		=	{	1	,	1	,	5	,	3	,	19	,	9	,	91	,	91	,	453	,	875	,	1815	,	1455	,	7969	,	5621	,	4773	};
            const unsigned long	dim3059KuoInit[]		=	{	1	,	1	,	5	,	1	,	17	,	63	,	19	,	193	,	335	,	571	,	703	,	919	,	773	,	1789	,	17839	};
            const unsigned long	dim3060KuoInit[]		=	{	1	,	1	,	5	,	7	,	1	,	57	,	11	,	171	,	11	,	833	,	191	,	1719	,	7649	,	11005	,	20171	};
            const unsigned long	dim3061KuoInit[]		=	{	1	,	1	,	7	,	15	,	25	,	55	,	63	,	71	,	127	,	445	,	1151	,	2709	,	6633	,	4959	,	13511	};
            const unsigned long	dim3062KuoInit[]		=	{	1	,	1	,	3	,	1	,	3	,	41	,	43	,	59	,	211	,	123	,	987	,	3701	,	6391	,	6755	,	3069	};
            const unsigned long	dim3063KuoInit[]		=	{	1	,	1	,	3	,	11	,	5	,	3	,	93	,	29	,	233	,	365	,	117	,	1483	,	4917	,	1949	,	23697	};
            const unsigned long	dim3064KuoInit[]		=	{	1	,	3	,	5	,	1	,	15	,	53	,	21	,	9	,	23	,	701	,	1337	,	2591	,	7719	,	14063	,	28453	};
            const unsigned long	dim3065KuoInit[]		=	{	1	,	1	,	1	,	11	,	21	,	3	,	127	,	179	,	409	,	261	,	1683	,	3197	,	1999	,	10009	,	25031	};
            const unsigned long	dim3066KuoInit[]		=	{	1	,	3	,	5	,	13	,	31	,	57	,	9	,	91	,	31	,	125	,	1831	,	1943	,	5931	,	15015	,	22269	};
            const unsigned long	dim3067KuoInit[]		=	{	1	,	3	,	5	,	9	,	7	,	7	,	83	,	113	,	203	,	679	,	1279	,	3403	,	7069	,	8617	,	27827	};
            const unsigned long	dim3068KuoInit[]		=	{	1	,	3	,	7	,	5	,	29	,	47	,	93	,	59	,	315	,	107	,	807	,	2861	,	2345	,	11787	,	6543	};
            const unsigned long	dim3069KuoInit[]		=	{	1	,	3	,	1	,	7	,	21	,	25	,	3	,	35	,	345	,	833	,	2029	,	281	,	2389	,	11529	,	20647	};
            const unsigned long	dim3070KuoInit[]		=	{	1	,	1	,	1	,	13	,	19	,	47	,	109	,	21	,	251	,	625	,	1699	,	2169	,	7293	,	14035	,	24277	};
            const unsigned long	dim3071KuoInit[]		=	{	1	,	3	,	5	,	13	,	15	,	7	,	51	,	125	,	423	,	981	,	437	,	1021	,	5209	,	11447	,	26181	};
            const unsigned long	dim3072KuoInit[]		=	{	1	,	3	,	1	,	1	,	15	,	41	,	75	,	227	,	13	,	85	,	675	,	3993	,	8163	,	8095	,	23801	};
            const unsigned long	dim3073KuoInit[]		=	{	1	,	3	,	3	,	9	,	25	,	1	,	85	,	143	,	309	,	815	,	1271	,	1331	,	6565	,	369	,	7975	};
            const unsigned long	dim3074KuoInit[]		=	{	1	,	1	,	7	,	1	,	21	,	45	,	113	,	153	,	283	,	117	,	1481	,	3527	,	3371	,	15801	,	22337	};
            const unsigned long	dim3075KuoInit[]		=	{	1	,	3	,	5	,	1	,	17	,	47	,	107	,	25	,	373	,	549	,	1387	,	1247	,	2307	,	6469	,	24327	};
            const unsigned long	dim3076KuoInit[]		=	{	1	,	1	,	5	,	15	,	9	,	57	,	43	,	179	,	157	,	119	,	547	,	3665	,	1495	,	13405	,	2391	};
            const unsigned long	dim3077KuoInit[]		=	{	1	,	3	,	1	,	1	,	7	,	57	,	123	,	189	,	281	,	87	,	1559	,	2863	,	8017	,	10279	,	32005	};
            const unsigned long	dim3078KuoInit[]		=	{	1	,	1	,	5	,	1	,	29	,	21	,	51	,	123	,	351	,	683	,	1505	,	681	,	49	,	2929	,	7591	};
            const unsigned long	dim3079KuoInit[]		=	{	1	,	1	,	3	,	7	,	17	,	63	,	119	,	209	,	441	,	717	,	567	,	1283	,	1007	,	14661	,	32383	};
            const unsigned long	dim3080KuoInit[]		=	{	1	,	3	,	1	,	7	,	27	,	41	,	41	,	157	,	487	,	257	,	1267	,	165	,	1023	,	16065	,	16431	};
            const unsigned long	dim3081KuoInit[]		=	{	1	,	1	,	3	,	9	,	17	,	53	,	51	,	175	,	367	,	453	,	1445	,	1889	,	4567	,	1781	,	6543	};
            const unsigned long	dim3082KuoInit[]		=	{	1	,	1	,	7	,	1	,	27	,	47	,	95	,	51	,	483	,	581	,	1617	,	685	,	3017	,	4889	,	6185	};
            const unsigned long	dim3083KuoInit[]		=	{	1	,	3	,	3	,	9	,	29	,	23	,	9	,	23	,	165	,	3	,	279	,	861	,	1007	,	9607	,	24999	};
            const unsigned long	dim3084KuoInit[]		=	{	1	,	1	,	1	,	5	,	11	,	59	,	55	,	135	,	399	,	729	,	683	,	2119	,	3781	,	15525	,	1125	};
            const unsigned long	dim3085KuoInit[]		=	{	1	,	3	,	3	,	15	,	25	,	61	,	123	,	153	,	267	,	823	,	951	,	4069	,	8017	,	4899	,	6535	};
            const unsigned long	dim3086KuoInit[]		=	{	1	,	1	,	7	,	11	,	5	,	43	,	5	,	123	,	57	,	229	,	129	,	131	,	7491	,	2105	,	16411	};
            const unsigned long	dim3087KuoInit[]		=	{	1	,	1	,	5	,	11	,	9	,	37	,	123	,	211	,	331	,	623	,	1469	,	1155	,	6327	,	12591	,	18971	};
            const unsigned long	dim3088KuoInit[]		=	{	1	,	1	,	1	,	11	,	17	,	35	,	59	,	137	,	451	,	829	,	365	,	305	,	5857	,	2095	,	14597	};
            const unsigned long	dim3089KuoInit[]		=	{	1	,	3	,	7	,	7	,	15	,	11	,	23	,	201	,	247	,	883	,	1103	,	4035	,	5915	,	4275	,	12511	};
            const unsigned long	dim3090KuoInit[]		=	{	1	,	3	,	7	,	1	,	27	,	19	,	87	,	189	,	91	,	113	,	237	,	2073	,	419	,	1339	,	16705	};
            const unsigned long	dim3091KuoInit[]		=	{	1	,	3	,	7	,	7	,	15	,	59	,	99	,	237	,	167	,	327	,	1507	,	771	,	3743	,	1723	,	28873	};
            const unsigned long	dim3092KuoInit[]		=	{	1	,	3	,	5	,	15	,	27	,	19	,	35	,	175	,	403	,	1	,	617	,	2425	,	2021	,	12547	,	23655	};
            const unsigned long	dim3093KuoInit[]		=	{	1	,	1	,	7	,	1	,	7	,	1	,	45	,	169	,	43	,	501	,	1653	,	593	,	7219	,	5501	,	3671	};
            const unsigned long	dim3094KuoInit[]		=	{	1	,	1	,	5	,	3	,	23	,	53	,	117	,	165	,	329	,	177	,	89	,	2269	,	7897	,	11225	,	25807	};
            const unsigned long	dim3095KuoInit[]		=	{	1	,	1	,	5	,	11	,	25	,	41	,	91	,	43	,	107	,	699	,	1745	,	3117	,	7497	,	6579	,	21803	};
            const unsigned long	dim3096KuoInit[]		=	{	1	,	3	,	3	,	13	,	19	,	9	,	103	,	65	,	413	,	459	,	1751	,	4025	,	6695	,	9589	,	1443	};
            const unsigned long	dim3097KuoInit[]		=	{	1	,	3	,	5	,	15	,	31	,	31	,	23	,	251	,	343	,	809	,	283	,	1501	,	445	,	7945	,	26993	};
            const unsigned long	dim3098KuoInit[]		=	{	1	,	3	,	7	,	11	,	1	,	11	,	113	,	235	,	127	,	45	,	109	,	2421	,	3955	,	1017	,	21497	};
            const unsigned long	dim3099KuoInit[]		=	{	1	,	3	,	1	,	13	,	5	,	57	,	47	,	227	,	447	,	499	,	1755	,	3527	,	1625	,	13341	,	22301	};
            const unsigned long	dim3100KuoInit[]		=	{	1	,	3	,	1	,	9	,	13	,	9	,	113	,	101	,	377	,	39	,	991	,	3791	,	7351	,	12841	,	2911	};
            const unsigned long	dim3101KuoInit[]		=	{	1	,	1	,	3	,	1	,	1	,	31	,	111	,	85	,	247	,	387	,	1907	,	3061	,	569	,	5845	,	6729	};
            const unsigned long	dim3102KuoInit[]		=	{	1	,	3	,	7	,	15	,	31	,	17	,	77	,	33	,	109	,	661	,	67	,	3725	,	955	,	9475	,	14345	};
            const unsigned long	dim3103KuoInit[]		=	{	1	,	1	,	7	,	15	,	11	,	23	,	63	,	23	,	67	,	843	,	1747	,	2441	,	2063	,	8575	,	11921	};
            const unsigned long	dim3104KuoInit[]		=	{	1	,	3	,	5	,	13	,	25	,	19	,	43	,	177	,	67	,	57	,	1127	,	3817	,	3097	,	6143	,	27757	};
            const unsigned long	dim3105KuoInit[]		=	{	1	,	1	,	3	,	1	,	19	,	25	,	65	,	135	,	143	,	863	,	1013	,	1285	,	6133	,	13491	,	5093	};
            const unsigned long	dim3106KuoInit[]		=	{	1	,	3	,	5	,	15	,	7	,	17	,	61	,	199	,	39	,	767	,	295	,	127	,	335	,	10789	,	20473	};
            const unsigned long	dim3107KuoInit[]		=	{	1	,	3	,	3	,	15	,	15	,	51	,	87	,	75	,	439	,	71	,	1371	,	1911	,	1145	,	4201	,	26883	};
            const unsigned long	dim3108KuoInit[]		=	{	1	,	1	,	3	,	11	,	3	,	15	,	79	,	237	,	43	,	375	,	1925	,	1517	,	6415	,	303	,	2217	};
            const unsigned long	dim3109KuoInit[]		=	{	1	,	3	,	1	,	3	,	21	,	23	,	19	,	83	,	159	,	249	,	1297	,	1853	,	2609	,	13635	,	29119	};
            const unsigned long	dim3110KuoInit[]		=	{	1	,	1	,	3	,	9	,	29	,	53	,	125	,	73	,	299	,	45	,	557	,	3641	,	1593	,	10585	,	22545	};
            const unsigned long	dim3111KuoInit[]		=	{	1	,	1	,	3	,	9	,	13	,	45	,	53	,	245	,	401	,	635	,	785	,	2841	,	3033	,	12377	,	1235	};
            const unsigned long	dim3112KuoInit[]		=	{	1	,	1	,	3	,	11	,	23	,	35	,	71	,	207	,	211	,	901	,	351	,	1473	,	6683	,	981	,	17449	};
            const unsigned long	dim3113KuoInit[]		=	{	1	,	1	,	3	,	9	,	17	,	13	,	125	,	213	,	109	,	631	,	677	,	2141	,	1037	,	6119	,	25945	};
            const unsigned long	dim3114KuoInit[]		=	{	1	,	3	,	7	,	1	,	29	,	3	,	53	,	23	,	45	,	347	,	27	,	1537	,	7833	,	16315	,	12337	};
            const unsigned long	dim3115KuoInit[]		=	{	1	,	3	,	1	,	1	,	31	,	23	,	15	,	241	,	315	,	977	,	99	,	3067	,	3227	,	6153	,	9751	};
            const unsigned long	dim3116KuoInit[]		=	{	1	,	1	,	7	,	7	,	13	,	33	,	5	,	5	,	29	,	547	,	1105	,	1135	,	7263	,	7421	,	9545	};
            const unsigned long	dim3117KuoInit[]		=	{	1	,	3	,	7	,	13	,	19	,	25	,	53	,	239	,	65	,	417	,	1681	,	659	,	133	,	4113	,	16735	};
            const unsigned long	dim3118KuoInit[]		=	{	1	,	3	,	7	,	5	,	3	,	55	,	57	,	33	,	357	,	885	,	941	,	3471	,	6863	,	777	,	19111	};
            const unsigned long	dim3119KuoInit[]		=	{	1	,	1	,	1	,	9	,	27	,	47	,	65	,	161	,	115	,	577	,	1951	,	363	,	2045	,	2497	,	6931	};
            const unsigned long	dim3120KuoInit[]		=	{	1	,	3	,	3	,	11	,	29	,	37	,	21	,	235	,	233	,	833	,	81	,	2875	,	4759	,	8435	,	16017	};
            const unsigned long	dim3121KuoInit[]		=	{	1	,	1	,	3	,	7	,	9	,	57	,	89	,	21	,	291	,	445	,	1833	,	735	,	3935	,	14943	,	30987	};
            const unsigned long	dim3122KuoInit[]		=	{	1	,	1	,	5	,	13	,	23	,	9	,	37	,	71	,	175	,	899	,	1897	,	707	,	6201	,	15543	,	13801	};
            const unsigned long	dim3123KuoInit[]		=	{	1	,	3	,	7	,	1	,	13	,	25	,	17	,	209	,	455	,	969	,	755	,	397	,	4103	,	3159	,	28665	};
            const unsigned long	dim3124KuoInit[]		=	{	1	,	1	,	1	,	5	,	15	,	7	,	17	,	249	,	299	,	423	,	83	,	1203	,	7397	,	15657	,	15661	};
            const unsigned long	dim3125KuoInit[]		=	{	1	,	1	,	7	,	7	,	19	,	19	,	21	,	231	,	443	,	729	,	947	,	1451	,	3399	,	5691	,	11943	};
            const unsigned long	dim3126KuoInit[]		=	{	1	,	1	,	5	,	7	,	15	,	11	,	89	,	211	,	459	,	3	,	741	,	105	,	1147	,	927	,	31805	};
            const unsigned long	dim3127KuoInit[]		=	{	1	,	1	,	7	,	9	,	7	,	1	,	61	,	145	,	291	,	1021	,	221	,	821	,	4999	,	3165	,	22549	};
            const unsigned long	dim3128KuoInit[]		=	{	1	,	1	,	1	,	7	,	5	,	41	,	101	,	137	,	143	,	249	,	87	,	4045	,	2301	,	3833	,	9841	};
            const unsigned long	dim3129KuoInit[]		=	{	1	,	1	,	5	,	5	,	19	,	61	,	111	,	109	,	87	,	65	,	971	,	3943	,	6559	,	9647	,	3049	};
            const unsigned long	dim3130KuoInit[]		=	{	1	,	1	,	1	,	13	,	9	,	11	,	41	,	61	,	145	,	27	,	1287	,	3537	,	1943	,	10635	,	32557	};
            const unsigned long	dim3131KuoInit[]		=	{	1	,	3	,	5	,	7	,	17	,	35	,	29	,	177	,	5	,	131	,	465	,	1235	,	1383	,	14253	,	17595	};
            const unsigned long	dim3132KuoInit[]		=	{	1	,	1	,	3	,	7	,	15	,	63	,	21	,	119	,	195	,	627	,	1635	,	2369	,	5549	,	7641	,	29481	};
            const unsigned long	dim3133KuoInit[]		=	{	1	,	1	,	7	,	13	,	25	,	3	,	73	,	233	,	115	,	749	,	707	,	1939	,	3667	,	1899	,	22345	};
            const unsigned long	dim3134KuoInit[]		=	{	1	,	1	,	5	,	9	,	15	,	33	,	17	,	91	,	507	,	289	,	1929	,	2001	,	629	,	15559	,	12725	};
            const unsigned long	dim3135KuoInit[]		=	{	1	,	1	,	1	,	13	,	7	,	7	,	91	,	171	,	95	,	257	,	1611	,	3527	,	2769	,	5779	,	7783	};
            const unsigned long	dim3136KuoInit[]		=	{	1	,	1	,	1	,	13	,	27	,	27	,	55	,	113	,	171	,	61	,	1229	,	3681	,	4487	,	9815	,	20357	};
            const unsigned long	dim3137KuoInit[]		=	{	1	,	3	,	7	,	15	,	1	,	21	,	57	,	67	,	185	,	189	,	1673	,	3637	,	2987	,	4619	,	6339	};
            const unsigned long	dim3138KuoInit[]		=	{	1	,	3	,	7	,	3	,	9	,	45	,	71	,	183	,	107	,	145	,	821	,	1115	,	6067	,	8301	,	7565	};
            const unsigned long	dim3139KuoInit[]		=	{	1	,	3	,	5	,	1	,	21	,	29	,	85	,	9	,	427	,	553	,	887	,	2463	,	4023	,	9107	,	25405	};
            const unsigned long	dim3140KuoInit[]		=	{	1	,	1	,	5	,	13	,	25	,	45	,	89	,	235	,	349	,	671	,	1667	,	2023	,	2385	,	1831	,	515	};
            const unsigned long	dim3141KuoInit[]		=	{	1	,	1	,	1	,	11	,	17	,	37	,	95	,	77	,	445	,	169	,	829	,	9	,	2589	,	12809	,	32469	};
            const unsigned long	dim3142KuoInit[]		=	{	1	,	1	,	5	,	5	,	23	,	31	,	117	,	81	,	275	,	861	,	1439	,	4041	,	3337	,	14945	,	15921	};
            const unsigned long	dim3143KuoInit[]		=	{	1	,	1	,	3	,	13	,	9	,	21	,	121	,	23	,	133	,	563	,	1889	,	1939	,	4427	,	5501	,	4609	};
            const unsigned long	dim3144KuoInit[]		=	{	1	,	1	,	7	,	7	,	17	,	25	,	7	,	245	,	319	,	697	,	1561	,	3209	,	2369	,	16045	,	31799	};
            const unsigned long	dim3145KuoInit[]		=	{	1	,	1	,	7	,	5	,	13	,	19	,	83	,	165	,	457	,	1005	,	743	,	4049	,	2563	,	14487	,	23699	};
            const unsigned long	dim3146KuoInit[]		=	{	1	,	1	,	7	,	1	,	5	,	25	,	65	,	185	,	27	,	235	,	1771	,	3505	,	6807	,	14715	,	19841	};
            const unsigned long	dim3147KuoInit[]		=	{	1	,	1	,	7	,	11	,	15	,	15	,	19	,	111	,	99	,	397	,	315	,	2587	,	7335	,	4371	,	11753	};
            const unsigned long	dim3148KuoInit[]		=	{	1	,	1	,	7	,	9	,	1	,	21	,	55	,	255	,	105	,	865	,	179	,	2127	,	4485	,	1803	,	19503	};
            const unsigned long	dim3149KuoInit[]		=	{	1	,	3	,	7	,	13	,	27	,	23	,	9	,	9	,	323	,	421	,	671	,	3323	,	5263	,	2963	,	16065	};
            const unsigned long	dim3150KuoInit[]		=	{	1	,	1	,	5	,	7	,	17	,	47	,	79	,	255	,	259	,	913	,	1691	,	1553	,	2443	,	14167	,	24499	};
            const unsigned long	dim3151KuoInit[]		=	{	1	,	3	,	3	,	3	,	21	,	7	,	17	,	71	,	57	,	715	,	2023	,	2133	,	5439	,	12483	,	32547	};
            const unsigned long	dim3152KuoInit[]		=	{	1	,	3	,	3	,	15	,	1	,	37	,	23	,	235	,	35	,	739	,	1767	,	3531	,	597	,	13259	,	7869	};
            const unsigned long	dim3153KuoInit[]		=	{	1	,	3	,	3	,	9	,	7	,	5	,	29	,	129	,	223	,	991	,	31	,	375	,	1917	,	1157	,	9723	};
            const unsigned long	dim3154KuoInit[]		=	{	1	,	3	,	5	,	5	,	29	,	33	,	33	,	73	,	339	,	21	,	2037	,	1127	,	6385	,	16335	,	25825	};
            const unsigned long	dim3155KuoInit[]		=	{	1	,	3	,	7	,	7	,	7	,	49	,	103	,	235	,	503	,	687	,	277	,	2639	,	5997	,	11797	,	27141	};
            const unsigned long	dim3156KuoInit[]		=	{	1	,	1	,	1	,	13	,	27	,	31	,	103	,	145	,	245	,	493	,	867	,	2075	,	5369	,	12551	,	4907	};
            const unsigned long	dim3157KuoInit[]		=	{	1	,	3	,	1	,	5	,	19	,	57	,	101	,	165	,	61	,	201	,	1299	,	3081	,	8173	,	12835	,	7915	};
            const unsigned long	dim3158KuoInit[]		=	{	1	,	3	,	5	,	3	,	29	,	23	,	49	,	41	,	473	,	273	,	1493	,	1915	,	6669	,	14263	,	13385	};
            const unsigned long	dim3159KuoInit[]		=	{	1	,	1	,	5	,	9	,	21	,	27	,	49	,	73	,	281	,	1017	,	1905	,	1005	,	631	,	5963	,	5729	};
            const unsigned long	dim3160KuoInit[]		=	{	1	,	3	,	3	,	5	,	9	,	25	,	1	,	155	,	275	,	675	,	831	,	109	,	5349	,	9607	,	22431	};
            const unsigned long	dim3161KuoInit[]		=	{	1	,	1	,	1	,	3	,	11	,	33	,	43	,	63	,	293	,	425	,	1525	,	4091	,	1865	,	13187	,	25191	};
            const unsigned long	dim3162KuoInit[]		=	{	1	,	1	,	1	,	7	,	27	,	25	,	89	,	39	,	11	,	961	,	1601	,	785	,	3317	,	13121	,	13647	};
            const unsigned long	dim3163KuoInit[]		=	{	1	,	1	,	7	,	1	,	31	,	59	,	25	,	75	,	317	,	305	,	1585	,	3649	,	7099	,	1819	,	10977	};
            const unsigned long	dim3164KuoInit[]		=	{	1	,	3	,	7	,	5	,	7	,	29	,	85	,	189	,	137	,	783	,	395	,	1887	,	6253	,	4043	,	32279	};
            const unsigned long	dim3165KuoInit[]		=	{	1	,	1	,	3	,	7	,	15	,	47	,	53	,	125	,	325	,	579	,	1783	,	3393	,	485	,	13923	,	4947	};
            const unsigned long	dim3166KuoInit[]		=	{	1	,	3	,	5	,	1	,	19	,	53	,	67	,	47	,	503	,	875	,	1717	,	857	,	5151	,	12279	,	21249	};
            const unsigned long	dim3167KuoInit[]		=	{	1	,	1	,	5	,	11	,	1	,	53	,	49	,	211	,	139	,	955	,	1129	,	1967	,	6833	,	2923	,	13251	};
            const unsigned long	dim3168KuoInit[]		=	{	1	,	3	,	3	,	9	,	15	,	53	,	103	,	71	,	51	,	379	,	1565	,	2017	,	6013	,	13147	,	1227	};
            const unsigned long	dim3169KuoInit[]		=	{	1	,	3	,	3	,	1	,	17	,	25	,	105	,	177	,	275	,	969	,	1373	,	2471	,	6959	,	11107	,	23657	};
            const unsigned long	dim3170KuoInit[]		=	{	1	,	3	,	1	,	13	,	15	,	3	,	19	,	145	,	309	,	1021	,	985	,	2385	,	5727	,	10357	,	4915	};
            const unsigned long	dim3171KuoInit[]		=	{	1	,	3	,	1	,	9	,	27	,	33	,	63	,	115	,	369	,	613	,	277	,	751	,	1959	,	5709	,	1851	};
            const unsigned long	dim3172KuoInit[]		=	{	1	,	1	,	7	,	7	,	11	,	31	,	95	,	131	,	383	,	821	,	725	,	1381	,	3953	,	14945	,	29101	};
            const unsigned long	dim3173KuoInit[]		=	{	1	,	1	,	1	,	11	,	1	,	59	,	81	,	89	,	325	,	27	,	1865	,	987	,	3877	,	12509	,	4129	};
            const unsigned long	dim3174KuoInit[]		=	{	1	,	3	,	5	,	7	,	3	,	33	,	63	,	141	,	429	,	847	,	1355	,	1451	,	4741	,	5427	,	16907	};
            const unsigned long	dim3175KuoInit[]		=	{	1	,	3	,	5	,	9	,	25	,	45	,	27	,	249	,	453	,	673	,	1519	,	905	,	5235	,	10057	,	24965	};
            const unsigned long	dim3176KuoInit[]		=	{	1	,	1	,	5	,	3	,	27	,	43	,	91	,	61	,	121	,	485	,	1047	,	405	,	8159	,	8363	,	19695	};
            const unsigned long	dim3177KuoInit[]		=	{	1	,	1	,	3	,	9	,	17	,	5	,	107	,	139	,	293	,	685	,	521	,	253	,	3309	,	15335	,	4967	};
            const unsigned long	dim3178KuoInit[]		=	{	1	,	1	,	7	,	13	,	7	,	43	,	63	,	119	,	225	,	269	,	1489	,	2983	,	1041	,	15149	,	21039	};
            const unsigned long	dim3179KuoInit[]		=	{	1	,	3	,	1	,	5	,	5	,	1	,	123	,	255	,	469	,	139	,	1861	,	385	,	3123	,	14739	,	23049	};
            const unsigned long	dim3180KuoInit[]		=	{	1	,	3	,	5	,	9	,	25	,	47	,	107	,	55	,	435	,	935	,	1751	,	555	,	973	,	10123	,	31869	};
            const unsigned long	dim3181KuoInit[]		=	{	1	,	3	,	7	,	5	,	21	,	25	,	113	,	169	,	313	,	607	,	631	,	3409	,	6539	,	5781	,	12681	};
            const unsigned long	dim3182KuoInit[]		=	{	1	,	1	,	3	,	15	,	7	,	13	,	1	,	245	,	57	,	495	,	905	,	1627	,	7107	,	13939	,	22091	};
            const unsigned long	dim3183KuoInit[]		=	{	1	,	1	,	1	,	5	,	7	,	55	,	121	,	137	,	93	,	265	,	1263	,	2251	,	7101	,	3095	,	13779	};
            const unsigned long	dim3184KuoInit[]		=	{	1	,	1	,	5	,	13	,	21	,	11	,	37	,	193	,	481	,	551	,	1659	,	563	,	4405	,	9541	,	8495	};
            const unsigned long	dim3185KuoInit[]		=	{	1	,	1	,	3	,	15	,	5	,	7	,	23	,	19	,	147	,	779	,	1089	,	723	,	3837	,	16291	,	23911	};
            const unsigned long	dim3186KuoInit[]		=	{	1	,	1	,	1	,	11	,	27	,	17	,	57	,	191	,	337	,	361	,	1261	,	3605	,	8051	,	8909	,	14951	};
            const unsigned long	dim3187KuoInit[]		=	{	1	,	3	,	3	,	9	,	25	,	23	,	39	,	109	,	107	,	605	,	373	,	649	,	5747	,	2037	,	24933	};
            const unsigned long	dim3188KuoInit[]		=	{	1	,	3	,	5	,	7	,	9	,	27	,	91	,	53	,	153	,	497	,	941	,	2999	,	3721	,	11115	,	17589	};
            const unsigned long	dim3189KuoInit[]		=	{	1	,	3	,	1	,	7	,	31	,	7	,	101	,	115	,	419	,	479	,	1097	,	2253	,	4365	,	14285	,	26469	};
            const unsigned long	dim3190KuoInit[]		=	{	1	,	1	,	1	,	15	,	13	,	37	,	31	,	231	,	195	,	539	,	941	,	3723	,	2483	,	2603	,	18897	};
            const unsigned long	dim3191KuoInit[]		=	{	1	,	3	,	5	,	13	,	7	,	31	,	19	,	99	,	197	,	833	,	79	,	3899	,	7045	,	6913	,	17959	};
            const unsigned long	dim3192KuoInit[]		=	{	1	,	1	,	3	,	5	,	1	,	51	,	83	,	135	,	307	,	121	,	187	,	2135	,	2927	,	6183	,	20327	};
            const unsigned long	dim3193KuoInit[]		=	{	1	,	3	,	5	,	11	,	7	,	31	,	47	,	123	,	409	,	239	,	1787	,	3063	,	6685	,	7487	,	31243	};
            const unsigned long	dim3194KuoInit[]		=	{	1	,	3	,	7	,	9	,	9	,	31	,	57	,	5	,	493	,	771	,	1941	,	199	,	6389	,	681	,	19379	};
            const unsigned long	dim3195KuoInit[]		=	{	1	,	1	,	5	,	3	,	1	,	57	,	63	,	35	,	369	,	267	,	923	,	2019	,	6487	,	15555	,	6993	};
            const unsigned long	dim3196KuoInit[]		=	{	1	,	3	,	7	,	15	,	15	,	59	,	123	,	179	,	5	,	35	,	1061	,	125	,	1025	,	5907	,	29671	};
            const unsigned long	dim3197KuoInit[]		=	{	1	,	3	,	1	,	11	,	21	,	63	,	71	,	237	,	333	,	893	,	1139	,	1969	,	2007	,	10017	,	27597	};
            const unsigned long	dim3198KuoInit[]		=	{	1	,	1	,	1	,	13	,	15	,	29	,	103	,	241	,	25	,	135	,	1147	,	1263	,	4793	,	16109	,	22447	};
            const unsigned long	dim3199KuoInit[]		=	{	1	,	1	,	3	,	7	,	11	,	21	,	85	,	7	,	123	,	69	,	1379	,	3243	,	7907	,	15859	,	19993	};
            const unsigned long	dim3200KuoInit[]		=	{	1	,	3	,	5	,	3	,	17	,	49	,	125	,	211	,	165	,	307	,	1331	,	2287	,	1127	,	4031	,	13719	};
            const unsigned long	dim3201KuoInit[]		=	{	1	,	1	,	7	,	1	,	23	,	23	,	21	,	159	,	387	,	443	,	291	,	2843	,	4413	,	9035	,	28887	};
            const unsigned long	dim3202KuoInit[]		=	{	1	,	1	,	7	,	15	,	9	,	25	,	113	,	185	,	285	,	211	,	317	,	3721	,	6921	,	4921	,	2353	};
            const unsigned long	dim3203KuoInit[]		=	{	1	,	1	,	3	,	5	,	21	,	9	,	53	,	21	,	337	,	51	,	429	,	4035	,	4697	,	3695	,	22589	};
            const unsigned long	dim3204KuoInit[]		=	{	1	,	1	,	5	,	5	,	21	,	63	,	79	,	213	,	471	,	183	,	299	,	409	,	6125	,	3653	,	9785	};
            const unsigned long	dim3205KuoInit[]		=	{	1	,	3	,	3	,	3	,	27	,	27	,	9	,	73	,	129	,	391	,	1877	,	4023	,	1631	,	7101	,	1491	};
            const unsigned long	dim3206KuoInit[]		=	{	1	,	3	,	5	,	3	,	21	,	7	,	93	,	51	,	469	,	3	,	1343	,	3827	,	6623	,	10149	,	29233	};
            const unsigned long	dim3207KuoInit[]		=	{	1	,	1	,	1	,	9	,	31	,	25	,	35	,	249	,	469	,	393	,	1315	,	1247	,	275	,	15841	,	23877	};
            const unsigned long	dim3208KuoInit[]		=	{	1	,	3	,	5	,	11	,	25	,	47	,	105	,	45	,	79	,	131	,	499	,	1411	,	2077	,	9809	,	14917	};
            const unsigned long	dim3209KuoInit[]		=	{	1	,	3	,	3	,	3	,	13	,	15	,	117	,	27	,	499	,	885	,	301	,	935	,	3443	,	3845	,	26873	};
            const unsigned long	dim3210KuoInit[]		=	{	1	,	3	,	3	,	1	,	1	,	47	,	21	,	97	,	245	,	95	,	899	,	993	,	2493	,	5249	,	32695	};
            const unsigned long	dim3211KuoInit[]		=	{	1	,	3	,	3	,	11	,	21	,	39	,	53	,	15	,	53	,	687	,	1269	,	1673	,	2015	,	3205	,	21447	};
            const unsigned long	dim3212KuoInit[]		=	{	1	,	1	,	5	,	11	,	1	,	39	,	117	,	85	,	277	,	635	,	1459	,	3977	,	2449	,	13341	,	30607	};
            const unsigned long	dim3213KuoInit[]		=	{	1	,	1	,	1	,	13	,	27	,	45	,	7	,	255	,	281	,	807	,	533	,	2173	,	4561	,	2501	,	5767	};
            const unsigned long	dim3214KuoInit[]		=	{	1	,	3	,	5	,	1	,	21	,	17	,	65	,	123	,	239	,	431	,	1003	,	241	,	8115	,	11633	,	27709	};
            const unsigned long	dim3215KuoInit[]		=	{	1	,	3	,	1	,	1	,	23	,	39	,	81	,	35	,	33	,	905	,	2019	,	3327	,	1197	,	6891	,	5521	};
            const unsigned long	dim3216KuoInit[]		=	{	1	,	3	,	3	,	11	,	1	,	51	,	51	,	153	,	23	,	685	,	1663	,	3167	,	5803	,	11111	,	3371	};
            const unsigned long	dim3217KuoInit[]		=	{	1	,	3	,	3	,	11	,	31	,	29	,	7	,	173	,	27	,	129	,	341	,	2971	,	4907	,	13481	,	19619	};
            const unsigned long	dim3218KuoInit[]		=	{	1	,	3	,	3	,	9	,	29	,	29	,	115	,	3	,	45	,	765	,	385	,	3775	,	5349	,	8809	,	11019	};
            const unsigned long	dim3219KuoInit[]		=	{	1	,	1	,	5	,	7	,	15	,	19	,	21	,	29	,	377	,	915	,	1835	,	517	,	3763	,	5439	,	7649	};
            const unsigned long	dim3220KuoInit[]		=	{	1	,	3	,	7	,	13	,	25	,	27	,	33	,	225	,	135	,	869	,	1383	,	3817	,	2981	,	8185	,	5689	};
            const unsigned long	dim3221KuoInit[]		=	{	1	,	1	,	5	,	9	,	1	,	27	,	19	,	55	,	39	,	451	,	365	,	2905	,	4311	,	16005	,	4537	};
            const unsigned long	dim3222KuoInit[]		=	{	1	,	1	,	7	,	15	,	19	,	1	,	99	,	177	,	163	,	877	,	511	,	1977	,	3291	,	7739	,	6383	};
            const unsigned long	dim3223KuoInit[]		=	{	1	,	1	,	7	,	15	,	1	,	29	,	27	,	1	,	411	,	425	,	2027	,	1739	,	5137	,	107	,	3515	};
            const unsigned long	dim3224KuoInit[]		=	{	1	,	3	,	7	,	5	,	5	,	29	,	15	,	55	,	423	,	671	,	1145	,	2403	,	5429	,	11507	,	24747	};
            const unsigned long	dim3225KuoInit[]		=	{	1	,	1	,	1	,	13	,	9	,	61	,	93	,	215	,	447	,	969	,	853	,	487	,	5735	,	2153	,	27857	};
            const unsigned long	dim3226KuoInit[]		=	{	1	,	3	,	1	,	3	,	29	,	41	,	47	,	245	,	197	,	261	,	77	,	805	,	965	,	14089	,	29199	};
            const unsigned long	dim3227KuoInit[]		=	{	1	,	1	,	5	,	11	,	3	,	49	,	53	,	7	,	91	,	123	,	413	,	123	,	7611	,	11493	,	16417	};
            const unsigned long	dim3228KuoInit[]		=	{	1	,	3	,	7	,	9	,	23	,	59	,	105	,	191	,	13	,	421	,	1541	,	2045	,	2821	,	9769	,	19893	};
            const unsigned long	dim3229KuoInit[]		=	{	1	,	3	,	5	,	13	,	3	,	19	,	37	,	229	,	465	,	1015	,	1871	,	1101	,	7515	,	8547	,	7853	};
            const unsigned long	dim3230KuoInit[]		=	{	1	,	3	,	5	,	13	,	9	,	61	,	113	,	209	,	503	,	441	,	905	,	2693	,	1827	,	9551	,	26081	};
            const unsigned long	dim3231KuoInit[]		=	{	1	,	1	,	1	,	7	,	31	,	27	,	123	,	37	,	415	,	729	,	921	,	453	,	673	,	14393	,	9139	};
            const unsigned long	dim3232KuoInit[]		=	{	1	,	1	,	5	,	3	,	13	,	9	,	79	,	243	,	211	,	89	,	295	,	3663	,	8165	,	12099	,	1413	};
            const unsigned long	dim3233KuoInit[]		=	{	1	,	1	,	5	,	7	,	23	,	33	,	33	,	129	,	51	,	965	,	1145	,	713	,	8143	,	10045	,	5291	};
            const unsigned long	dim3234KuoInit[]		=	{	1	,	1	,	3	,	3	,	11	,	45	,	77	,	223	,	423	,	1021	,	343	,	3103	,	119	,	3371	,	24053	};
            const unsigned long	dim3235KuoInit[]		=	{	1	,	1	,	7	,	7	,	7	,	3	,	65	,	31	,	433	,	277	,	1541	,	2001	,	121	,	13543	,	6723	};
            const unsigned long	dim3236KuoInit[]		=	{	1	,	1	,	7	,	1	,	1	,	15	,	83	,	85	,	351	,	351	,	1063	,	3989	,	5739	,	11939	,	28009	};
            const unsigned long	dim3237KuoInit[]		=	{	1	,	1	,	5	,	11	,	25	,	57	,	73	,	127	,	25	,	67	,	1789	,	199	,	6373	,	10945	,	17839	};
            const unsigned long	dim3238KuoInit[]		=	{	1	,	3	,	5	,	11	,	17	,	15	,	5	,	191	,	209	,	455	,	89	,	2611	,	7557	,	16167	,	25993	};
            const unsigned long	dim3239KuoInit[]		=	{	1	,	1	,	7	,	7	,	29	,	21	,	79	,	81	,	257	,	373	,	557	,	2363	,	5497	,	8343	,	2011	};
            const unsigned long	dim3240KuoInit[]		=	{	1	,	1	,	5	,	11	,	25	,	63	,	83	,	31	,	477	,	201	,	2013	,	3349	,	3517	,	9527	,	14883	};
            const unsigned long	dim3241KuoInit[]		=	{	1	,	3	,	7	,	13	,	21	,	13	,	41	,	151	,	359	,	829	,	661	,	1959	,	6109	,	13757	,	17677	};
            const unsigned long	dim3242KuoInit[]		=	{	1	,	3	,	7	,	1	,	11	,	15	,	119	,	25	,	351	,	135	,	515	,	3727	,	5701	,	5283	,	18771	};
            const unsigned long	dim3243KuoInit[]		=	{	1	,	1	,	1	,	3	,	3	,	59	,	55	,	117	,	269	,	161	,	645	,	3459	,	8173	,	7193	,	10275	};
            const unsigned long	dim3244KuoInit[]		=	{	1	,	3	,	5	,	7	,	21	,	25	,	53	,	211	,	237	,	775	,	43	,	3613	,	5555	,	1689	,	1781	};
            const unsigned long	dim3245KuoInit[]		=	{	1	,	1	,	1	,	7	,	31	,	43	,	71	,	201	,	371	,	457	,	1757	,	2529	,	7067	,	2987	,	3945	};
            const unsigned long	dim3246KuoInit[]		=	{	1	,	1	,	5	,	7	,	15	,	47	,	29	,	83	,	259	,	813	,	881	,	1901	,	6635	,	8515	,	20385	};
            const unsigned long	dim3247KuoInit[]		=	{	1	,	1	,	5	,	5	,	3	,	45	,	55	,	43	,	17	,	179	,	1801	,	49	,	8121	,	9333	,	12701	};
            const unsigned long	dim3248KuoInit[]		=	{	1	,	1	,	3	,	9	,	21	,	9	,	55	,	59	,	275	,	775	,	1567	,	1557	,	3465	,	5727	,	12881	};
            const unsigned long	dim3249KuoInit[]		=	{	1	,	1	,	1	,	5	,	21	,	63	,	81	,	27	,	139	,	859	,	1211	,	3787	,	1179	,	4045	,	26225	};
            const unsigned long	dim3250KuoInit[]		=	{	1	,	3	,	7	,	3	,	7	,	5	,	99	,	125	,	499	,	437	,	1141	,	3557	,	5349	,	2701	,	25741	};
            const unsigned long	dim3251KuoInit[]		=	{	1	,	1	,	5	,	1	,	7	,	59	,	65	,	117	,	201	,	351	,	1063	,	3083	,	6637	,	5969	,	23037	};
            const unsigned long	dim3252KuoInit[]		=	{	1	,	3	,	7	,	7	,	25	,	45	,	115	,	31	,	351	,	203	,	1791	,	2931	,	7127	,	12117	,	10781	};
            const unsigned long	dim3253KuoInit[]		=	{	1	,	1	,	3	,	3	,	29	,	59	,	85	,	101	,	147	,	219	,	1293	,	1645	,	7653	,	649	,	32221	};
            const unsigned long	dim3254KuoInit[]		=	{	1	,	1	,	7	,	5	,	29	,	51	,	127	,	139	,	205	,	77	,	859	,	2277	,	2333	,	105	,	9053	};
            const unsigned long	dim3255KuoInit[]		=	{	1	,	3	,	5	,	3	,	29	,	23	,	119	,	71	,	263	,	551	,	1095	,	359	,	6223	,	7665	,	1095	};
            const unsigned long	dim3256KuoInit[]		=	{	1	,	3	,	1	,	11	,	13	,	47	,	11	,	207	,	507	,	139	,	1383	,	1339	,	4111	,	11685	,	12607	};
            const unsigned long	dim3257KuoInit[]		=	{	1	,	3	,	7	,	3	,	25	,	9	,	87	,	117	,	21	,	289	,	351	,	225	,	4045	,	5275	,	531	};
            const unsigned long	dim3258KuoInit[]		=	{	1	,	3	,	7	,	3	,	29	,	55	,	31	,	57	,	83	,	423	,	201	,	4055	,	1977	,	8891	,	17961	};
            const unsigned long	dim3259KuoInit[]		=	{	1	,	1	,	1	,	3	,	9	,	7	,	45	,	237	,	413	,	53	,	1063	,	23	,	6615	,	16359	,	4743	};
            const unsigned long	dim3260KuoInit[]		=	{	1	,	3	,	1	,	3	,	21	,	57	,	63	,	221	,	111	,	95	,	647	,	3419	,	1919	,	923	,	8357	};
            const unsigned long	dim3261KuoInit[]		=	{	1	,	1	,	1	,	3	,	27	,	25	,	119	,	237	,	397	,	73	,	41	,	17	,	6839	,	15695	,	30365	};
            const unsigned long	dim3262KuoInit[]		=	{	1	,	3	,	1	,	1	,	23	,	59	,	107	,	121	,	267	,	163	,	181	,	2289	,	167	,	7077	,	12895	};
            const unsigned long	dim3263KuoInit[]		=	{	1	,	1	,	5	,	5	,	15	,	45	,	45	,	179	,	195	,	95	,	43	,	377	,	629	,	11983	,	23439	};
            const unsigned long	dim3264KuoInit[]		=	{	1	,	3	,	3	,	9	,	17	,	43	,	67	,	143	,	223	,	243	,	209	,	3	,	7651	,	12607	,	3927	};
            const unsigned long	dim3265KuoInit[]		=	{	1	,	3	,	1	,	11	,	19	,	21	,	59	,	75	,	353	,	509	,	1215	,	3223	,	2373	,	3873	,	20391	};
            const unsigned long	dim3266KuoInit[]		=	{	1	,	1	,	7	,	3	,	7	,	9	,	53	,	89	,	407	,	987	,	1165	,	2473	,	4295	,	2531	,	5615	};
            const unsigned long	dim3267KuoInit[]		=	{	1	,	1	,	3	,	11	,	23	,	7	,	111	,	147	,	49	,	627	,	1769	,	3497	,	2807	,	4743	,	26491	};
            const unsigned long	dim3268KuoInit[]		=	{	1	,	3	,	3	,	13	,	29	,	29	,	83	,	57	,	377	,	581	,	1699	,	1689	,	2297	,	1377	,	24551	};
            const unsigned long	dim3269KuoInit[]		=	{	1	,	3	,	5	,	15	,	25	,	33	,	41	,	187	,	195	,	647	,	73	,	619	,	5585	,	8379	,	26079	};
            const unsigned long	dim3270KuoInit[]		=	{	1	,	1	,	3	,	9	,	3	,	47	,	105	,	83	,	501	,	843	,	1867	,	1855	,	6347	,	3269	,	26155	};
            const unsigned long	dim3271KuoInit[]		=	{	1	,	3	,	3	,	15	,	29	,	33	,	117	,	141	,	171	,	801	,	1489	,	1	,	4081	,	15529	,	4081	};
            const unsigned long	dim3272KuoInit[]		=	{	1	,	1	,	3	,	13	,	21	,	43	,	13	,	75	,	75	,	443	,	1443	,	3227	,	907	,	11821	,	24209	};
            const unsigned long	dim3273KuoInit[]		=	{	1	,	1	,	3	,	3	,	31	,	31	,	111	,	193	,	275	,	545	,	371	,	3071	,	3491	,	6661	,	1113	};
            const unsigned long	dim3274KuoInit[]		=	{	1	,	1	,	7	,	3	,	11	,	39	,	17	,	107	,	303	,	201	,	321	,	3083	,	6839	,	4381	,	3449	};
            const unsigned long	dim3275KuoInit[]		=	{	1	,	1	,	7	,	15	,	31	,	21	,	7	,	143	,	211	,	369	,	369	,	323	,	3811	,	5131	,	11425	};
            const unsigned long	dim3276KuoInit[]		=	{	1	,	1	,	1	,	3	,	9	,	27	,	23	,	21	,	349	,	607	,	529	,	1161	,	5753	,	6015	,	20489	};
            const unsigned long	dim3277KuoInit[]		=	{	1	,	3	,	7	,	11	,	23	,	17	,	103	,	97	,	227	,	933	,	2011	,	1705	,	5185	,	299	,	5673	};
            const unsigned long	dim3278KuoInit[]		=	{	1	,	3	,	5	,	9	,	7	,	45	,	63	,	43	,	339	,	299	,	1617	,	1393	,	4507	,	13513	,	29039	};
            const unsigned long	dim3279KuoInit[]		=	{	1	,	1	,	5	,	5	,	17	,	11	,	49	,	159	,	51	,	449	,	497	,	2627	,	1541	,	15711	,	16591	};
            const unsigned long	dim3280KuoInit[]		=	{	1	,	1	,	7	,	3	,	23	,	19	,	53	,	79	,	133	,	279	,	327	,	2847	,	5825	,	14429	,	26491	};
            const unsigned long	dim3281KuoInit[]		=	{	1	,	3	,	1	,	3	,	17	,	3	,	33	,	55	,	263	,	663	,	1253	,	1509	,	4487	,	1811	,	32123	};
            const unsigned long	dim3282KuoInit[]		=	{	1	,	3	,	7	,	9	,	17	,	7	,	23	,	87	,	221	,	183	,	1577	,	1373	,	2717	,	3323	,	24573	};
            const unsigned long	dim3283KuoInit[]		=	{	1	,	3	,	3	,	9	,	5	,	51	,	45	,	11	,	11	,	135	,	505	,	221	,	3847	,	13487	,	23843	};
            const unsigned long	dim3284KuoInit[]		=	{	1	,	1	,	5	,	13	,	15	,	61	,	85	,	53	,	321	,	433	,	61	,	3377	,	3993	,	15325	,	8641	};
            const unsigned long	dim3285KuoInit[]		=	{	1	,	3	,	7	,	13	,	27	,	35	,	57	,	43	,	509	,	595	,	2033	,	77	,	7533	,	5027	,	15567	};
            const unsigned long	dim3286KuoInit[]		=	{	1	,	1	,	1	,	1	,	31	,	51	,	69	,	157	,	105	,	797	,	249	,	2033	,	2457	,	10773	,	31537	};
            const unsigned long	dim3287KuoInit[]		=	{	1	,	3	,	3	,	13	,	23	,	25	,	75	,	11	,	145	,	361	,	819	,	3689	,	5751	,	6395	,	30685	};
            const unsigned long	dim3288KuoInit[]		=	{	1	,	1	,	7	,	11	,	13	,	25	,	13	,	175	,	11	,	685	,	835	,	2641	,	2037	,	2037	,	2807	};
            const unsigned long	dim3289KuoInit[]		=	{	1	,	1	,	7	,	11	,	21	,	27	,	115	,	91	,	21	,	105	,	1817	,	2057	,	2797	,	3825	,	23761	};
            const unsigned long	dim3290KuoInit[]		=	{	1	,	3	,	3	,	5	,	5	,	49	,	51	,	57	,	425	,	959	,	473	,	575	,	6851	,	3683	,	27429	};
            const unsigned long	dim3291KuoInit[]		=	{	1	,	1	,	7	,	5	,	29	,	1	,	65	,	13	,	447	,	467	,	1193	,	4021	,	275	,	13453	,	29749	};
            const unsigned long	dim3292KuoInit[]		=	{	1	,	3	,	5	,	11	,	27	,	53	,	71	,	63	,	125	,	545	,	1673	,	3917	,	6625	,	12805	,	2905	};
            const unsigned long	dim3293KuoInit[]		=	{	1	,	3	,	3	,	1	,	7	,	1	,	45	,	65	,	23	,	237	,	1573	,	3445	,	6307	,	5015	,	11263	};
            const unsigned long	dim3294KuoInit[]		=	{	1	,	1	,	5	,	1	,	7	,	29	,	67	,	35	,	307	,	675	,	727	,	2789	,	2353	,	13241	,	16621	};
            const unsigned long	dim3295KuoInit[]		=	{	1	,	3	,	1	,	15	,	13	,	5	,	115	,	75	,	21	,	745	,	2027	,	265	,	3583	,	1365	,	397	};
            const unsigned long	dim3296KuoInit[]		=	{	1	,	1	,	5	,	7	,	9	,	31	,	115	,	25	,	441	,	433	,	1665	,	2499	,	5765	,	4001	,	22105	};
            const unsigned long	dim3297KuoInit[]		=	{	1	,	3	,	7	,	9	,	23	,	21	,	71	,	165	,	353	,	101	,	1335	,	3411	,	1415	,	12641	,	21961	};
            const unsigned long	dim3298KuoInit[]		=	{	1	,	3	,	7	,	1	,	3	,	3	,	21	,	157	,	85	,	117	,	469	,	1573	,	2475	,	6471	,	15975	};
            const unsigned long	dim3299KuoInit[]		=	{	1	,	1	,	7	,	9	,	31	,	35	,	127	,	221	,	447	,	697	,	1393	,	701	,	7291	,	4279	,	2897	};
            const unsigned long	dim3300KuoInit[]		=	{	1	,	3	,	5	,	5	,	21	,	13	,	15	,	15	,	395	,	519	,	1985	,	2331	,	5483	,	13905	,	31549	};
            const unsigned long	dim3301KuoInit[]		=	{	1	,	1	,	7	,	5	,	1	,	43	,	57	,	131	,	7	,	893	,	1631	,	1581	,	6691	,	6249	,	4975	};
            const unsigned long	dim3302KuoInit[]		=	{	1	,	3	,	3	,	9	,	29	,	33	,	51	,	177	,	423	,	459	,	1293	,	2467	,	3877	,	15653	,	2727	};
            const unsigned long	dim3303KuoInit[]		=	{	1	,	3	,	5	,	1	,	5	,	45	,	11	,	27	,	311	,	645	,	283	,	65	,	5913	,	12811	,	20041	};
            const unsigned long	dim3304KuoInit[]		=	{	1	,	3	,	7	,	5	,	19	,	3	,	39	,	99	,	269	,	429	,	1901	,	2213	,	6755	,	1205	,	16073	};
            const unsigned long	dim3305KuoInit[]		=	{	1	,	3	,	5	,	13	,	7	,	37	,	109	,	113	,	395	,	981	,	1991	,	845	,	2473	,	3801	,	24405	};
            const unsigned long	dim3306KuoInit[]		=	{	1	,	1	,	1	,	15	,	9	,	31	,	117	,	213	,	141	,	867	,	387	,	1985	,	1985	,	12683	,	20501	};
            const unsigned long	dim3307KuoInit[]		=	{	1	,	1	,	1	,	15	,	11	,	3	,	25	,	31	,	375	,	437	,	645	,	335	,	207	,	14915	,	2577	};
            const unsigned long	dim3308KuoInit[]		=	{	1	,	1	,	7	,	7	,	19	,	61	,	87	,	107	,	53	,	777	,	31	,	3335	,	157	,	2927	,	25905	};
            const unsigned long	dim3309KuoInit[]		=	{	1	,	1	,	7	,	9	,	13	,	13	,	21	,	151	,	51	,	501	,	213	,	2177	,	2821	,	3081	,	5955	};
            const unsigned long	dim3310KuoInit[]		=	{	1	,	1	,	5	,	13	,	19	,	33	,	5	,	209	,	283	,	613	,	281	,	715	,	297	,	15033	,	20489	};
            const unsigned long	dim3311KuoInit[]		=	{	1	,	3	,	5	,	5	,	31	,	7	,	31	,	49	,	5	,	41	,	939	,	2359	,	3663	,	10151	,	20457	};
            const unsigned long	dim3312KuoInit[]		=	{	1	,	1	,	7	,	1	,	7	,	61	,	69	,	255	,	17	,	637	,	863	,	3351	,	6033	,	5005	,	23107	};
            const unsigned long	dim3313KuoInit[]		=	{	1	,	1	,	5	,	3	,	5	,	61	,	15	,	87	,	111	,	383	,	1577	,	3515	,	789	,	4219	,	2527	};
            const unsigned long	dim3314KuoInit[]		=	{	1	,	1	,	5	,	11	,	19	,	9	,	37	,	61	,	329	,	811	,	1829	,	3109	,	397	,	2065	,	32001	};
            const unsigned long	dim3315KuoInit[]		=	{	1	,	3	,	1	,	9	,	7	,	13	,	55	,	7	,	463	,	307	,	1551	,	3161	,	649	,	14073	,	20839	};
            const unsigned long	dim3316KuoInit[]		=	{	1	,	3	,	1	,	9	,	1	,	43	,	71	,	87	,	149	,	557	,	857	,	2739	,	6269	,	3297	,	26067	};
            const unsigned long	dim3317KuoInit[]		=	{	1	,	3	,	1	,	5	,	5	,	29	,	45	,	159	,	125	,	389	,	627	,	983	,	755	,	15585	,	12555	};
            const unsigned long	dim3318KuoInit[]		=	{	1	,	1	,	3	,	7	,	9	,	53	,	79	,	29	,	423	,	423	,	1493	,	61	,	1631	,	5157	,	29845	};
            const unsigned long	dim3319KuoInit[]		=	{	1	,	3	,	5	,	9	,	23	,	27	,	57	,	235	,	57	,	833	,	371	,	1797	,	4541	,	12491	,	23127	};
            const unsigned long	dim3320KuoInit[]		=	{	1	,	3	,	3	,	1	,	17	,	1	,	79	,	87	,	247	,	987	,	651	,	3167	,	4423	,	831	,	16811	};
            const unsigned long	dim3321KuoInit[]		=	{	1	,	3	,	3	,	5	,	15	,	23	,	51	,	167	,	421	,	493	,	1877	,	1079	,	4147	,	13485	,	28619	};
            const unsigned long	dim3322KuoInit[]		=	{	1	,	1	,	7	,	13	,	19	,	15	,	69	,	31	,	225	,	597	,	1477	,	2363	,	6969	,	13221	,	2561	};
            const unsigned long	dim3323KuoInit[]		=	{	1	,	3	,	1	,	7	,	19	,	61	,	75	,	199	,	103	,	823	,	375	,	1301	,	4349	,	135	,	5701	};
            const unsigned long	dim3324KuoInit[]		=	{	1	,	3	,	7	,	11	,	21	,	25	,	85	,	249	,	91	,	885	,	469	,	1809	,	4351	,	14393	,	19607	};
            const unsigned long	dim3325KuoInit[]		=	{	1	,	3	,	5	,	1	,	25	,	19	,	71	,	111	,	427	,	523	,	1691	,	4085	,	7587	,	1949	,	10471	};
            const unsigned long	dim3326KuoInit[]		=	{	1	,	3	,	1	,	11	,	27	,	59	,	13	,	117	,	155	,	369	,	1767	,	2951	,	6109	,	13867	,	13887	};
            const unsigned long	dim3327KuoInit[]		=	{	1	,	3	,	3	,	3	,	31	,	53	,	35	,	101	,	295	,	653	,	337	,	2281	,	7563	,	8847	,	15235	};
            const unsigned long	dim3328KuoInit[]		=	{	1	,	1	,	7	,	13	,	3	,	17	,	63	,	251	,	293	,	861	,	1539	,	2409	,	663	,	1637	,	22309	};
            const unsigned long	dim3329KuoInit[]		=	{	1	,	3	,	7	,	9	,	13	,	27	,	41	,	81	,	483	,	901	,	1963	,	1855	,	799	,	10933	,	14395	};
            const unsigned long	dim3330KuoInit[]		=	{	1	,	1	,	3	,	5	,	19	,	23	,	97	,	179	,	93	,	657	,	1361	,	4019	,	7577	,	355	,	4881	};
            const unsigned long	dim3331KuoInit[]		=	{	1	,	3	,	3	,	15	,	23	,	25	,	101	,	141	,	91	,	37	,	1165	,	3509	,	2445	,	7675	,	30803	};
            const unsigned long	dim3332KuoInit[]		=	{	1	,	1	,	1	,	5	,	13	,	53	,	87	,	197	,	321	,	297	,	277	,	2159	,	5457	,	6463	,	1797	};
            const unsigned long	dim3333KuoInit[]		=	{	1	,	3	,	3	,	7	,	3	,	3	,	117	,	5	,	95	,	527	,	755	,	2865	,	6359	,	4859	,	11395	};
            const unsigned long	dim3334KuoInit[]		=	{	1	,	3	,	3	,	13	,	1	,	49	,	43	,	247	,	369	,	351	,	1277	,	1639	,	3133	,	4193	,	26645	};
            const unsigned long	dim3335KuoInit[]		=	{	1	,	1	,	1	,	9	,	27	,	21	,	59	,	191	,	365	,	95	,	337	,	3279	,	4455	,	8235	,	12457	};
            const unsigned long	dim3336KuoInit[]		=	{	1	,	1	,	7	,	5	,	9	,	1	,	7	,	23	,	117	,	711	,	1027	,	613	,	4729	,	14415	,	15705	};
            const unsigned long	dim3337KuoInit[]		=	{	1	,	3	,	1	,	3	,	29	,	13	,	43	,	3	,	117	,	763	,	1787	,	2109	,	4949	,	7579	,	24083	};
            const unsigned long	dim3338KuoInit[]		=	{	1	,	1	,	5	,	11	,	7	,	61	,	107	,	113	,	127	,	495	,	1167	,	2871	,	7187	,	13235	,	2037	};
            const unsigned long	dim3339KuoInit[]		=	{	1	,	1	,	5	,	5	,	13	,	33	,	23	,	69	,	5	,	137	,	157	,	1107	,	1025	,	1907	,	2279	};
            const unsigned long	dim3340KuoInit[]		=	{	1	,	1	,	1	,	9	,	11	,	63	,	27	,	13	,	295	,	951	,	2027	,	3625	,	1447	,	8939	,	3965	};
            const unsigned long	dim3341KuoInit[]		=	{	1	,	3	,	3	,	13	,	29	,	31	,	67	,	221	,	413	,	13	,	1643	,	2847	,	4889	,	3733	,	25713	};
            const unsigned long	dim3342KuoInit[]		=	{	1	,	3	,	5	,	11	,	11	,	17	,	37	,	37	,	159	,	835	,	1775	,	2059	,	2995	,	6141	,	12277	};
            const unsigned long	dim3343KuoInit[]		=	{	1	,	3	,	3	,	7	,	11	,	25	,	107	,	239	,	361	,	835	,	1943	,	1375	,	7077	,	13843	,	5685	};
            const unsigned long	dim3344KuoInit[]		=	{	1	,	1	,	3	,	11	,	23	,	27	,	3	,	191	,	339	,	675	,	1181	,	2005	,	245	,	7305	,	5831	};
            const unsigned long	dim3345KuoInit[]		=	{	1	,	3	,	5	,	9	,	15	,	17	,	91	,	15	,	255	,	201	,	535	,	3063	,	4351	,	12527	,	15981	};
            const unsigned long	dim3346KuoInit[]		=	{	1	,	3	,	5	,	9	,	7	,	47	,	111	,	245	,	223	,	781	,	367	,	559	,	1433	,	9631	,	7207	};
            const unsigned long	dim3347KuoInit[]		=	{	1	,	3	,	7	,	5	,	23	,	5	,	111	,	237	,	371	,	541	,	663	,	693	,	6591	,	13089	,	11381	};
            const unsigned long	dim3348KuoInit[]		=	{	1	,	1	,	3	,	9	,	25	,	5	,	99	,	17	,	401	,	787	,	971	,	1035	,	1143	,	12041	,	22455	};
            const unsigned long	dim3349KuoInit[]		=	{	1	,	1	,	5	,	1	,	3	,	5	,	125	,	193	,	69	,	939	,	37	,	2843	,	3139	,	7967	,	15099	};
            const unsigned long	dim3350KuoInit[]		=	{	1	,	1	,	5	,	3	,	11	,	39	,	35	,	103	,	203	,	255	,	727	,	2513	,	1981	,	12855	,	20887	};
            const unsigned long	dim3351KuoInit[]		=	{	1	,	3	,	3	,	15	,	27	,	41	,	41	,	57	,	275	,	497	,	727	,	901	,	5653	,	12917	,	20783	};
            const unsigned long	dim3352KuoInit[]		=	{	1	,	3	,	1	,	5	,	17	,	13	,	101	,	95	,	465	,	155	,	973	,	1911	,	2719	,	157	,	19475	};
            const unsigned long	dim3353KuoInit[]		=	{	1	,	3	,	3	,	5	,	3	,	61	,	31	,	225	,	351	,	931	,	855	,	3871	,	1835	,	11133	,	25203	};
            const unsigned long	dim3354KuoInit[]		=	{	1	,	3	,	7	,	7	,	25	,	1	,	91	,	193	,	455	,	873	,	1255	,	1657	,	4143	,	433	,	22789	};
            const unsigned long	dim3355KuoInit[]		=	{	1	,	3	,	7	,	9	,	31	,	19	,	35	,	81	,	175	,	885	,	1007	,	3443	,	2063	,	3179	,	28563	};
            const unsigned long	dim3356KuoInit[]		=	{	1	,	1	,	3	,	9	,	1	,	25	,	109	,	249	,	341	,	393	,	497	,	1181	,	2943	,	9021	,	9985	};
            const unsigned long	dim3357KuoInit[]		=	{	1	,	1	,	7	,	13	,	15	,	49	,	27	,	145	,	351	,	119	,	257	,	151	,	4287	,	1709	,	2585	};
            const unsigned long	dim3358KuoInit[]		=	{	1	,	1	,	7	,	7	,	3	,	19	,	105	,	51	,	443	,	693	,	29	,	2657	,	6967	,	4797	,	14565	};
            const unsigned long	dim3359KuoInit[]		=	{	1	,	3	,	3	,	1	,	25	,	53	,	65	,	169	,	461	,	275	,	1191	,	731	,	6295	,	15083	,	28445	};
            const unsigned long	dim3360KuoInit[]		=	{	1	,	3	,	1	,	1	,	11	,	39	,	21	,	3	,	411	,	45	,	1325	,	3279	,	3153	,	289	,	30973	};
            const unsigned long	dim3361KuoInit[]		=	{	1	,	1	,	1	,	11	,	29	,	57	,	107	,	245	,	279	,	677	,	599	,	1145	,	5135	,	6131	,	12667	};
            const unsigned long	dim3362KuoInit[]		=	{	1	,	3	,	1	,	15	,	27	,	7	,	99	,	215	,	17	,	171	,	977	,	2065	,	1357	,	8705	,	29641	};
            const unsigned long	dim3363KuoInit[]		=	{	1	,	1	,	3	,	9	,	31	,	23	,	53	,	9	,	177	,	645	,	99	,	3175	,	3543	,	13637	,	9923	};
            const unsigned long	dim3364KuoInit[]		=	{	1	,	1	,	1	,	13	,	5	,	45	,	69	,	39	,	207	,	651	,	1507	,	2969	,	2743	,	4025	,	9983	};
            const unsigned long	dim3365KuoInit[]		=	{	1	,	1	,	5	,	13	,	13	,	57	,	27	,	215	,	51	,	927	,	1973	,	3127	,	3655	,	10653	,	5649	};
            const unsigned long	dim3366KuoInit[]		=	{	1	,	3	,	3	,	7	,	21	,	7	,	119	,	163	,	461	,	859	,	253	,	2817	,	8001	,	393	,	20171	};
            const unsigned long	dim3367KuoInit[]		=	{	1	,	3	,	3	,	3	,	25	,	7	,	101	,	109	,	57	,	185	,	2005	,	3167	,	3539	,	2465	,	783	};
            const unsigned long	dim3368KuoInit[]		=	{	1	,	3	,	1	,	3	,	31	,	43	,	69	,	189	,	471	,	669	,	339	,	1125	,	4273	,	10417	,	20129	};
            const unsigned long	dim3369KuoInit[]		=	{	1	,	1	,	3	,	7	,	19	,	37	,	115	,	111	,	439	,	19	,	425	,	995	,	3983	,	15523	,	25679	};
            const unsigned long	dim3370KuoInit[]		=	{	1	,	3	,	5	,	9	,	5	,	1	,	37	,	105	,	359	,	423	,	545	,	1099	,	6809	,	8531	,	25155	};
            const unsigned long	dim3371KuoInit[]		=	{	1	,	1	,	3	,	3	,	9	,	37	,	37	,	149	,	457	,	1017	,	25	,	3641	,	2673	,	1501	,	28811	};
            const unsigned long	dim3372KuoInit[]		=	{	1	,	3	,	5	,	11	,	25	,	31	,	57	,	233	,	125	,	755	,	1807	,	337	,	81	,	14709	,	4061	};
            const unsigned long	dim3373KuoInit[]		=	{	1	,	1	,	1	,	7	,	25	,	7	,	37	,	35	,	437	,	889	,	965	,	3997	,	4493	,	1357	,	26383	};
            const unsigned long	dim3374KuoInit[]		=	{	1	,	1	,	1	,	3	,	9	,	37	,	105	,	127	,	283	,	141	,	1451	,	2275	,	3261	,	8135	,	429	};
            const unsigned long	dim3375KuoInit[]		=	{	1	,	1	,	1	,	5	,	3	,	19	,	9	,	233	,	293	,	827	,	103	,	1963	,	2877	,	7701	,	17431	};
            const unsigned long	dim3376KuoInit[]		=	{	1	,	3	,	7	,	11	,	15	,	17	,	87	,	243	,	141	,	721	,	829	,	3569	,	4081	,	6039	,	21523	};
            const unsigned long	dim3377KuoInit[]		=	{	1	,	3	,	3	,	11	,	17	,	21	,	31	,	169	,	425	,	755	,	1391	,	3433	,	3275	,	15657	,	9085	};
            const unsigned long	dim3378KuoInit[]		=	{	1	,	3	,	1	,	7	,	25	,	23	,	49	,	227	,	103	,	5	,	1995	,	3803	,	1109	,	6015	,	2433	};
            const unsigned long	dim3379KuoInit[]		=	{	1	,	1	,	7	,	7	,	19	,	39	,	91	,	219	,	403	,	475	,	1629	,	117	,	2317	,	3991	,	23467	};
            const unsigned long	dim3380KuoInit[]		=	{	1	,	1	,	7	,	5	,	19	,	63	,	79	,	9	,	19	,	747	,	1045	,	875	,	5809	,	949	,	10943	};
            const unsigned long	dim3381KuoInit[]		=	{	1	,	1	,	3	,	11	,	3	,	29	,	83	,	103	,	481	,	565	,	451	,	1353	,	5417	,	2747	,	141	};
            const unsigned long	dim3382KuoInit[]		=	{	1	,	1	,	1	,	11	,	17	,	43	,	75	,	51	,	335	,	257	,	1077	,	517	,	4029	,	13761	,	8653	};
            const unsigned long	dim3383KuoInit[]		=	{	1	,	1	,	1	,	1	,	13	,	51	,	41	,	195	,	331	,	155	,	1711	,	2787	,	6163	,	12641	,	20731	};
            const unsigned long	dim3384KuoInit[]		=	{	1	,	1	,	1	,	11	,	5	,	37	,	117	,	97	,	133	,	793	,	491	,	2185	,	3671	,	12257	,	6239	};
            const unsigned long	dim3385KuoInit[]		=	{	1	,	1	,	1	,	5	,	9	,	33	,	69	,	153	,	29	,	443	,	921	,	3679	,	7385	,	7229	,	16443	};
            const unsigned long	dim3386KuoInit[]		=	{	1	,	3	,	3	,	9	,	19	,	19	,	87	,	147	,	113	,	577	,	505	,	343	,	7385	,	3247	,	6411	};
            const unsigned long	dim3387KuoInit[]		=	{	1	,	1	,	7	,	9	,	15	,	33	,	83	,	69	,	27	,	519	,	115	,	4059	,	5281	,	7995	,	25537	};
            const unsigned long	dim3388KuoInit[]		=	{	1	,	1	,	5	,	15	,	21	,	25	,	37	,	35	,	263	,	531	,	197	,	27	,	7871	,	7383	,	28467	};
            const unsigned long	dim3389KuoInit[]		=	{	1	,	3	,	5	,	1	,	1	,	49	,	87	,	173	,	37	,	633	,	65	,	2449	,	149	,	7145	,	6461	};
            const unsigned long	dim3390KuoInit[]		=	{	1	,	1	,	3	,	7	,	17	,	35	,	91	,	45	,	257	,	351	,	1569	,	3757	,	5203	,	8071	,	32211	};
            const unsigned long	dim3391KuoInit[]		=	{	1	,	1	,	3	,	9	,	9	,	39	,	51	,	33	,	77	,	663	,	287	,	1127	,	4549	,	6321	,	16199	};
            const unsigned long	dim3392KuoInit[]		=	{	1	,	1	,	5	,	9	,	3	,	3	,	123	,	35	,	75	,	253	,	1661	,	3191	,	1749	,	14421	,	15767	};
            const unsigned long	dim3393KuoInit[]		=	{	1	,	1	,	1	,	15	,	21	,	27	,	69	,	233	,	425	,	89	,	1069	,	603	,	5439	,	5597	,	24183	};
            const unsigned long	dim3394KuoInit[]		=	{	1	,	1	,	7	,	15	,	19	,	57	,	127	,	83	,	445	,	619	,	1541	,	2695	,	1657	,	10787	,	27703	};
            const unsigned long	dim3395KuoInit[]		=	{	1	,	3	,	1	,	3	,	31	,	31	,	29	,	61	,	245	,	449	,	145	,	3619	,	5267	,	9403	,	9637	};
            const unsigned long	dim3396KuoInit[]		=	{	1	,	1	,	5	,	9	,	29	,	61	,	33	,	245	,	341	,	991	,	639	,	3391	,	2971	,	857	,	26191	};
            const unsigned long	dim3397KuoInit[]		=	{	1	,	1	,	1	,	5	,	9	,	7	,	57	,	33	,	79	,	489	,	379	,	3907	,	3759	,	4243	,	9811	};
            const unsigned long	dim3398KuoInit[]		=	{	1	,	3	,	5	,	15	,	7	,	61	,	7	,	119	,	51	,	927	,	1505	,	2997	,	723	,	8987	,	29007	};
            const unsigned long	dim3399KuoInit[]		=	{	1	,	1	,	3	,	1	,	31	,	55	,	63	,	15	,	421	,	423	,	1041	,	3967	,	2471	,	13273	,	18587	};
            const unsigned long	dim3400KuoInit[]		=	{	1	,	3	,	3	,	13	,	9	,	49	,	111	,	211	,	87	,	887	,	1867	,	19	,	5329	,	12411	,	18537	};
            const unsigned long	dim3401KuoInit[]		=	{	1	,	3	,	1	,	1	,	27	,	35	,	89	,	81	,	119	,	635	,	63	,	2185	,	7001	,	11283	,	11883	};
            const unsigned long	dim3402KuoInit[]		=	{	1	,	1	,	3	,	9	,	31	,	61	,	55	,	237	,	327	,	537	,	123	,	3589	,	3019	,	705	,	16701	};
            const unsigned long	dim3403KuoInit[]		=	{	1	,	1	,	7	,	1	,	7	,	13	,	103	,	101	,	473	,	7	,	381	,	3643	,	1227	,	11335	,	615	};
            const unsigned long	dim3404KuoInit[]		=	{	1	,	3	,	5	,	13	,	1	,	9	,	127	,	65	,	479	,	623	,	469	,	3403	,	2423	,	4269	,	15195	};
            const unsigned long	dim3405KuoInit[]		=	{	1	,	1	,	7	,	15	,	3	,	23	,	125	,	195	,	267	,	923	,	961	,	799	,	783	,	893	,	22403	};
            const unsigned long	dim3406KuoInit[]		=	{	1	,	3	,	1	,	3	,	21	,	7	,	21	,	151	,	373	,	377	,	1685	,	3731	,	6019	,	1059	,	32457	};
            const unsigned long	dim3407KuoInit[]		=	{	1	,	3	,	3	,	11	,	9	,	31	,	55	,	207	,	109	,	945	,	1073	,	3545	,	4191	,	12965	,	20863	};
            const unsigned long	dim3408KuoInit[]		=	{	1	,	1	,	1	,	5	,	27	,	45	,	15	,	67	,	101	,	207	,	77	,	3309	,	747	,	13857	,	25737	};
            const unsigned long	dim3409KuoInit[]		=	{	1	,	1	,	1	,	5	,	1	,	31	,	15	,	235	,	57	,	449	,	411	,	3739	,	5093	,	12963	,	30649	};
            const unsigned long	dim3410KuoInit[]		=	{	1	,	3	,	7	,	5	,	5	,	33	,	87	,	13	,	57	,	727	,	1041	,	3369	,	6047	,	1007	,	4587	};
            const unsigned long	dim3411KuoInit[]		=	{	1	,	1	,	3	,	9	,	29	,	43	,	11	,	251	,	99	,	413	,	1943	,	3461	,	993	,	13983	,	8157	};
            const unsigned long	dim3412KuoInit[]		=	{	1	,	1	,	5	,	15	,	7	,	27	,	105	,	9	,	69	,	949	,	245	,	2537	,	5941	,	10961	,	17115	};
            const unsigned long	dim3413KuoInit[]		=	{	1	,	1	,	7	,	5	,	17	,	41	,	63	,	167	,	45	,	391	,	895	,	2565	,	2681	,	10855	,	6919	};
            const unsigned long	dim3414KuoInit[]		=	{	1	,	3	,	7	,	3	,	7	,	53	,	15	,	207	,	467	,	131	,	761	,	189	,	4977	,	4345	,	7539	};
            const unsigned long	dim3415KuoInit[]		=	{	1	,	1	,	5	,	5	,	23	,	17	,	127	,	3	,	455	,	787	,	383	,	2381	,	6407	,	3253	,	1787	};
            const unsigned long	dim3416KuoInit[]		=	{	1	,	3	,	5	,	15	,	5	,	7	,	37	,	75	,	351	,	141	,	93	,	2631	,	609	,	11455	,	20603	};
            const unsigned long	dim3417KuoInit[]		=	{	1	,	3	,	7	,	9	,	5	,	33	,	19	,	15	,	101	,	1005	,	7	,	1341	,	4577	,	11627	,	17061	};
            const unsigned long	dim3418KuoInit[]		=	{	1	,	3	,	3	,	9	,	5	,	7	,	87	,	181	,	197	,	423	,	361	,	545	,	2459	,	7419	,	14457	};
            const unsigned long	dim3419KuoInit[]		=	{	1	,	1	,	7	,	7	,	25	,	37	,	93	,	85	,	345	,	459	,	465	,	3377	,	795	,	13439	,	4765	};
            const unsigned long	dim3420KuoInit[]		=	{	1	,	1	,	7	,	9	,	25	,	55	,	109	,	63	,	381	,	329	,	1787	,	3163	,	279	,	4647	,	8723	};
            const unsigned long	dim3421KuoInit[]		=	{	1	,	3	,	3	,	13	,	23	,	33	,	85	,	41	,	151	,	319	,	265	,	259	,	29	,	4217	,	21025	};
            const unsigned long	dim3422KuoInit[]		=	{	1	,	1	,	5	,	9	,	31	,	17	,	93	,	27	,	455	,	1015	,	81	,	621	,	6511	,	7617	,	6775	};
            const unsigned long	dim3423KuoInit[]		=	{	1	,	1	,	1	,	11	,	25	,	21	,	73	,	161	,	267	,	911	,	25	,	1385	,	4073	,	14423	,	31473	};
            const unsigned long	dim3424KuoInit[]		=	{	1	,	3	,	5	,	11	,	13	,	33	,	95	,	113	,	371	,	291	,	1055	,	3779	,	991	,	9699	,	9495	};
            const unsigned long	dim3425KuoInit[]		=	{	1	,	1	,	1	,	7	,	31	,	55	,	111	,	215	,	399	,	721	,	1443	,	763	,	3119	,	11075	,	15489	};
            const unsigned long	dim3426KuoInit[]		=	{	1	,	1	,	1	,	3	,	17	,	57	,	119	,	171	,	299	,	299	,	1919	,	3103	,	5691	,	7927	,	18875	};
            const unsigned long	dim3427KuoInit[]		=	{	1	,	1	,	5	,	7	,	13	,	41	,	35	,	209	,	383	,	381	,	901	,	3341	,	3421	,	11501	,	2187	};
            const unsigned long	dim3428KuoInit[]		=	{	1	,	3	,	1	,	7	,	1	,	3	,	83	,	105	,	495	,	297	,	413	,	2941	,	4309	,	9829	,	32115	};
            const unsigned long	dim3429KuoInit[]		=	{	1	,	3	,	5	,	3	,	3	,	7	,	81	,	9	,	55	,	19	,	225	,	2915	,	6255	,	10967	,	8767	};
            const unsigned long	dim3430KuoInit[]		=	{	1	,	3	,	1	,	1	,	23	,	59	,	21	,	127	,	7	,	703	,	1189	,	2375	,	6273	,	1655	,	10115	};
            const unsigned long	dim3431KuoInit[]		=	{	1	,	1	,	1	,	15	,	19	,	35	,	39	,	185	,	143	,	163	,	1491	,	201	,	2473	,	661	,	12827	};
            const unsigned long	dim3432KuoInit[]		=	{	1	,	1	,	1	,	5	,	29	,	49	,	5	,	27	,	131	,	1005	,	991	,	1159	,	5437	,	5039	,	4037	};
            const unsigned long	dim3433KuoInit[]		=	{	1	,	3	,	7	,	3	,	11	,	41	,	123	,	43	,	11	,	443	,	109	,	3995	,	5377	,	2907	,	30035	};
            const unsigned long	dim3434KuoInit[]		=	{	1	,	3	,	1	,	1	,	31	,	59	,	53	,	219	,	185	,	31	,	641	,	1695	,	5817	,	9001	,	28827	};
            const unsigned long	dim3435KuoInit[]		=	{	1	,	1	,	1	,	3	,	13	,	37	,	37	,	119	,	123	,	57	,	1241	,	551	,	5825	,	11551	,	23521	};
            const unsigned long	dim3436KuoInit[]		=	{	1	,	1	,	3	,	15	,	13	,	37	,	33	,	61	,	209	,	375	,	335	,	1223	,	4023	,	12743	,	8375	};
            const unsigned long	dim3437KuoInit[]		=	{	1	,	3	,	3	,	3	,	31	,	7	,	103	,	43	,	451	,	517	,	1025	,	2853	,	5943	,	13655	,	21409	};
            const unsigned long	dim3438KuoInit[]		=	{	1	,	1	,	1	,	13	,	11	,	37	,	11	,	167	,	271	,	763	,	85	,	3727	,	641	,	2841	,	23803	};
            const unsigned long	dim3439KuoInit[]		=	{	1	,	1	,	7	,	5	,	3	,	19	,	117	,	225	,	275	,	327	,	1263	,	1597	,	1439	,	8531	,	18063	};
            const unsigned long	dim3440KuoInit[]		=	{	1	,	3	,	5	,	3	,	9	,	45	,	57	,	169	,	453	,	949	,	585	,	1365	,	4263	,	3461	,	7191	};
            const unsigned long	dim3441KuoInit[]		=	{	1	,	3	,	7	,	9	,	25	,	51	,	97	,	255	,	457	,	293	,	1027	,	1291	,	6685	,	2617	,	3055	};
            const unsigned long	dim3442KuoInit[]		=	{	1	,	3	,	7	,	5	,	27	,	33	,	47	,	65	,	43	,	545	,	123	,	1093	,	5563	,	3013	,	18451	};
            const unsigned long	dim3443KuoInit[]		=	{	1	,	1	,	3	,	11	,	9	,	25	,	71	,	241	,	495	,	561	,	1565	,	703	,	809	,	10961	,	3001	};
            const unsigned long	dim3444KuoInit[]		=	{	1	,	3	,	5	,	1	,	1	,	47	,	97	,	233	,	259	,	39	,	543	,	303	,	2705	,	3683	,	32123	};
            const unsigned long	dim3445KuoInit[]		=	{	1	,	1	,	5	,	7	,	1	,	35	,	91	,	43	,	417	,	299	,	1635	,	2163	,	7959	,	9015	,	5661	};
            const unsigned long	dim3446KuoInit[]		=	{	1	,	1	,	3	,	11	,	1	,	63	,	49	,	209	,	125	,	241	,	987	,	3173	,	631	,	9767	,	8467	};
            const unsigned long	dim3447KuoInit[]		=	{	1	,	1	,	1	,	5	,	25	,	49	,	43	,	117	,	65	,	81	,	313	,	1917	,	6087	,	1503	,	3383	};
            const unsigned long	dim3448KuoInit[]		=	{	1	,	3	,	5	,	1	,	23	,	53	,	43	,	29	,	91	,	727	,	1157	,	2985	,	3851	,	15897	,	17577	};
            const unsigned long	dim3449KuoInit[]		=	{	1	,	1	,	3	,	15	,	15	,	27	,	35	,	129	,	161	,	679	,	1001	,	2681	,	5287	,	10367	,	21017	};
            const unsigned long	dim3450KuoInit[]		=	{	1	,	3	,	3	,	1	,	23	,	51	,	127	,	119	,	467	,	161	,	1229	,	2969	,	6321	,	1141	,	7827	};
            const unsigned long	dim3451KuoInit[]		=	{	1	,	3	,	5	,	15	,	19	,	45	,	33	,	173	,	205	,	815	,	783	,	1323	,	69	,	5191	,	23795	};
            const unsigned long	dim3452KuoInit[]		=	{	1	,	3	,	5	,	9	,	19	,	1	,	111	,	253	,	463	,	247	,	1877	,	1831	,	8037	,	12769	,	8111	};
            const unsigned long	dim3453KuoInit[]		=	{	1	,	3	,	1	,	1	,	21	,	7	,	19	,	241	,	297	,	953	,	1759	,	2187	,	1587	,	14857	,	13245	};
            const unsigned long	dim3454KuoInit[]		=	{	1	,	1	,	1	,	15	,	27	,	21	,	111	,	37	,	333	,	485	,	127	,	1781	,	545	,	251	,	2757	};
            const unsigned long	dim3455KuoInit[]		=	{	1	,	3	,	1	,	1	,	3	,	7	,	21	,	69	,	171	,	225	,	1505	,	2725	,	923	,	9451	,	29443	};
            const unsigned long	dim3456KuoInit[]		=	{	1	,	3	,	7	,	7	,	13	,	57	,	97	,	175	,	429	,	529	,	677	,	1501	,	4717	,	12705	,	31473	};
            const unsigned long	dim3457KuoInit[]		=	{	1	,	3	,	7	,	11	,	29	,	7	,	57	,	115	,	55	,	167	,	903	,	3855	,	8051	,	15567	,	31733	};
            const unsigned long	dim3458KuoInit[]		=	{	1	,	1	,	1	,	1	,	1	,	1	,	1	,	1	,	1	,	1	,	1	,	1	,	1	,	1	,	1	};
            const unsigned long	dim3459KuoInit[]		=	{	1	,	1	,	1	,	13	,	11	,	17	,	73	,	193	,	151	,	927	,	1375	,	4037	,	6855	,	10481	,	8973	};
            const unsigned long	dim3460KuoInit[]		=	{	1	,	3	,	7	,	1	,	25	,	7	,	115	,	39	,	469	,	903	,	655	,	299	,	6007	,	6629	,	11871	};
            const unsigned long	dim3461KuoInit[]		=	{	1	,	3	,	7	,	5	,	15	,	49	,	79	,	115	,	55	,	841	,	1091	,	1641	,	5693	,	3607	,	13699	};
            const unsigned long	dim3462KuoInit[]		=	{	1	,	1	,	1	,	15	,	5	,	33	,	99	,	239	,	157	,	133	,	2023	,	683	,	3419	,	10993	,	32187	};
            const unsigned long	dim3463KuoInit[]		=	{	1	,	1	,	7	,	1	,	3	,	13	,	95	,	233	,	341	,	243	,	2043	,	653	,	4673	,	1441	,	19087	};
            const unsigned long	dim3464KuoInit[]		=	{	1	,	3	,	3	,	3	,	29	,	51	,	117	,	199	,	49	,	467	,	461	,	2465	,	7541	,	6815	,	28711	};
            const unsigned long	dim3465KuoInit[]		=	{	1	,	3	,	1	,	1	,	25	,	47	,	31	,	79	,	9	,	39	,	733	,	3567	,	5965	,	6627	,	1109	};
            const unsigned long	dim3466KuoInit[]		=	{	1	,	3	,	1	,	13	,	9	,	53	,	27	,	101	,	341	,	369	,	1659	,	1645	,	4465	,	10175	,	4565	};
            const unsigned long	dim3467KuoInit[]		=	{	1	,	3	,	5	,	7	,	13	,	61	,	67	,	125	,	427	,	105	,	133	,	2303	,	5595	,	4527	,	23151	};
            const unsigned long	dim3468KuoInit[]		=	{	1	,	1	,	5	,	7	,	19	,	29	,	87	,	167	,	141	,	657	,	947	,	367	,	2617	,	9287	,	629	};
            const unsigned long	dim3469KuoInit[]		=	{	1	,	3	,	7	,	5	,	21	,	25	,	35	,	21	,	391	,	1019	,	991	,	3343	,	1221	,	1591	,	18095	};
            const unsigned long	dim3470KuoInit[]		=	{	1	,	3	,	7	,	9	,	31	,	49	,	99	,	185	,	405	,	261	,	935	,	3447	,	4081	,	5537	,	26329	};
            const unsigned long	dim3471KuoInit[]		=	{	1	,	1	,	3	,	7	,	25	,	51	,	107	,	83	,	449	,	733	,	597	,	1023	,	7457	,	14343	,	7119	};
            const unsigned long	dim3472KuoInit[]		=	{	1	,	1	,	1	,	5	,	23	,	21	,	35	,	83	,	185	,	531	,	1283	,	2235	,	7775	,	15895	,	32651	};
            const unsigned long	dim3473KuoInit[]		=	{	1	,	1	,	5	,	15	,	27	,	13	,	23	,	227	,	491	,	701	,	341	,	2109	,	8045	,	1529	,	10589	};
            const unsigned long	dim3474KuoInit[]		=	{	1	,	1	,	5	,	5	,	21	,	51	,	31	,	101	,	31	,	861	,	401	,	1519	,	5349	,	2623	,	22667	};
            const unsigned long	dim3475KuoInit[]		=	{	1	,	3	,	5	,	5	,	1	,	41	,	43	,	25	,	491	,	329	,	537	,	1357	,	5567	,	3951	,	15293	};
            const unsigned long	dim3476KuoInit[]		=	{	1	,	3	,	7	,	3	,	15	,	39	,	113	,	105	,	51	,	483	,	1417	,	2971	,	3	,	9673	,	4331	};
            const unsigned long	dim3477KuoInit[]		=	{	1	,	3	,	5	,	13	,	17	,	3	,	19	,	55	,	443	,	967	,	327	,	3463	,	5427	,	195	,	7495	};
            const unsigned long	dim3478KuoInit[]		=	{	1	,	1	,	3	,	3	,	15	,	15	,	11	,	157	,	311	,	967	,	89	,	3691	,	7307	,	12309	,	1915	};
            const unsigned long	dim3479KuoInit[]		=	{	1	,	3	,	5	,	7	,	25	,	7	,	125	,	163	,	231	,	177	,	1497	,	3119	,	3281	,	15033	,	9457	};
            const unsigned long	dim3480KuoInit[]		=	{	1	,	3	,	3	,	9	,	25	,	51	,	51	,	243	,	427	,	831	,	709	,	3741	,	445	,	14275	,	32323	};
            const unsigned long	dim3481KuoInit[]		=	{	1	,	3	,	3	,	13	,	1	,	41	,	123	,	237	,	401	,	19	,	1161	,	985	,	5119	,	14879	,	5785	};
            const unsigned long	dim3482KuoInit[]		=	{	1	,	3	,	1	,	3	,	3	,	11	,	121	,	209	,	507	,	271	,	1045	,	2547	,	1019	,	7107	,	31823	};
            const unsigned long	dim3483KuoInit[]		=	{	1	,	1	,	5	,	3	,	21	,	3	,	33	,	73	,	79	,	607	,	47	,	3045	,	2471	,	2023	,	8115	};
            const unsigned long	dim3484KuoInit[]		=	{	1	,	3	,	7	,	13	,	19	,	49	,	111	,	185	,	209	,	683	,	1459	,	3827	,	6591	,	13139	,	2057	};
            const unsigned long	dim3485KuoInit[]		=	{	1	,	3	,	7	,	9	,	17	,	63	,	77	,	217	,	161	,	287	,	335	,	3947	,	5255	,	12297	,	16829	};
            const unsigned long	dim3486KuoInit[]		=	{	1	,	3	,	1	,	11	,	17	,	17	,	31	,	231	,	483	,	437	,	431	,	957	,	6499	,	12947	,	29751	};
            const unsigned long	dim3487KuoInit[]		=	{	1	,	3	,	7	,	15	,	5	,	57	,	119	,	43	,	385	,	743	,	249	,	3479	,	7751	,	6321	,	31747	};
            const unsigned long	dim3488KuoInit[]		=	{	1	,	1	,	5	,	3	,	5	,	1	,	113	,	87	,	439	,	507	,	1517	,	2081	,	5947	,	10967	,	10219	};
            const unsigned long	dim3489KuoInit[]		=	{	1	,	3	,	1	,	13	,	23	,	29	,	39	,	33	,	129	,	493	,	1119	,	2177	,	4135	,	5051	,	20241	};
            const unsigned long	dim3490KuoInit[]		=	{	1	,	3	,	7	,	13	,	19	,	13	,	71	,	163	,	107	,	285	,	1999	,	3371	,	3555	,	16265	,	2825	};
            const unsigned long	dim3491KuoInit[]		=	{	1	,	3	,	7	,	11	,	5	,	19	,	67	,	201	,	203	,	619	,	927	,	1769	,	5655	,	4539	,	1719	};
            const unsigned long	dim3492KuoInit[]		=	{	1	,	3	,	5	,	7	,	15	,	43	,	107	,	91	,	315	,	281	,	525	,	3243	,	7751	,	8991	,	29195	};
            const unsigned long	dim3493KuoInit[]		=	{	1	,	1	,	7	,	7	,	21	,	43	,	35	,	81	,	199	,	51	,	935	,	357	,	1723	,	8183	,	6209	};
            const unsigned long	dim3494KuoInit[]		=	{	1	,	3	,	5	,	5	,	9	,	31	,	75	,	71	,	375	,	181	,	1503	,	3479	,	2255	,	1371	,	16127	};
            const unsigned long	dim3495KuoInit[]		=	{	1	,	1	,	7	,	11	,	29	,	61	,	57	,	145	,	149	,	673	,	1953	,	2111	,	3525	,	1007	,	15927	};
            const unsigned long	dim3496KuoInit[]		=	{	1	,	1	,	7	,	15	,	25	,	25	,	103	,	175	,	273	,	193	,	1215	,	3059	,	7805	,	1455	,	1627	};
            const unsigned long	dim3497KuoInit[]		=	{	1	,	3	,	1	,	1	,	23	,	45	,	61	,	227	,	487	,	89	,	739	,	1867	,	3643	,	9879	,	17307	};
            const unsigned long	dim3498KuoInit[]		=	{	1	,	1	,	5	,	1	,	23	,	5	,	117	,	25	,	325	,	725	,	1999	,	3453	,	2353	,	5467	,	2065	};
            const unsigned long	dim3499KuoInit[]		=	{	1	,	1	,	5	,	5	,	25	,	3	,	95	,	243	,	175	,	291	,	821	,	3531	,	4873	,	9975	,	30437	};
            const unsigned long	dim3500KuoInit[]		=	{	1	,	3	,	3	,	11	,	5	,	41	,	121	,	23	,	15	,	195	,	1141	,	171	,	2245	,	8043	,	2201	};
            const unsigned long	dim3501KuoInit[]		=	{	1	,	3	,	7	,	1	,	29	,	1	,	41	,	233	,	217	,	501	,	913	,	3589	,	1153	,	4601	,	23147	};
            const unsigned long	dim3502KuoInit[]		=	{	1	,	1	,	5	,	15	,	1	,	41	,	71	,	255	,	283	,	125	,	845	,	1261	,	7519	,	15623	,	13301	};
            const unsigned long	dim3503KuoInit[]		=	{	1	,	3	,	3	,	15	,	11	,	3	,	73	,	161	,	203	,	775	,	905	,	411	,	4139	,	8289	,	8581	};
            const unsigned long	dim3504KuoInit[]		=	{	1	,	1	,	7	,	13	,	7	,	57	,	17	,	115	,	273	,	949	,	913	,	3267	,	6949	,	6785	,	30449	};
            const unsigned long	dim3505KuoInit[]		=	{	1	,	1	,	7	,	5	,	3	,	49	,	55	,	191	,	505	,	807	,	287	,	3595	,	2261	,	13751	,	15339	};
            const unsigned long	dim3506KuoInit[]		=	{	1	,	1	,	5	,	11	,	23	,	13	,	83	,	145	,	387	,	37	,	341	,	111	,	3309	,	4481	,	21679	};
            const unsigned long	dim3507KuoInit[]		=	{	1	,	1	,	7	,	15	,	3	,	51	,	13	,	247	,	419	,	555	,	431	,	187	,	2753	,	9071	,	25189	};
            const unsigned long	dim3508KuoInit[]		=	{	1	,	3	,	7	,	3	,	29	,	33	,	119	,	63	,	363	,	197	,	1901	,	2231	,	1453	,	3901	,	13713	};
            const unsigned long	dim3509KuoInit[]		=	{	1	,	1	,	5	,	7	,	9	,	15	,	91	,	107	,	397	,	739	,	1419	,	481	,	5713	,	14089	,	11699	};
            const unsigned long	dim3510KuoInit[]		=	{	1	,	3	,	3	,	7	,	9	,	37	,	47	,	183	,	45	,	135	,	903	,	2769	,	5929	,	8175	,	8661	};
            const unsigned long	dim3511KuoInit[]		=	{	1	,	1	,	3	,	9	,	7	,	63	,	117	,	81	,	385	,	459	,	1455	,	4089	,	395	,	7569	,	18089	};
            const unsigned long	dim3512KuoInit[]		=	{	1	,	1	,	1	,	3	,	15	,	29	,	71	,	47	,	423	,	177	,	1481	,	1087	,	3703	,	5857	,	9631	};
            const unsigned long	dim3513KuoInit[]		=	{	1	,	3	,	1	,	7	,	23	,	45	,	127	,	91	,	457	,	81	,	681	,	257	,	361	,	11739	,	1503	};
            const unsigned long	dim3514KuoInit[]		=	{	1	,	3	,	3	,	9	,	29	,	29	,	107	,	125	,	55	,	759	,	1287	,	1937	,	353	,	1275	,	14349	};
            const unsigned long	dim3515KuoInit[]		=	{	1	,	3	,	5	,	11	,	15	,	53	,	35	,	1	,	419	,	255	,	1891	,	7	,	3005	,	1493	,	165	};
            const unsigned long	dim3516KuoInit[]		=	{	1	,	3	,	5	,	1	,	5	,	19	,	45	,	109	,	479	,	309	,	277	,	2749	,	4859	,	4301	,	17799	};
            const unsigned long	dim3517KuoInit[]		=	{	1	,	3	,	7	,	15	,	21	,	57	,	39	,	19	,	433	,	77	,	1999	,	3467	,	2097	,	6853	,	1763	};
            const unsigned long	dim3518KuoInit[]		=	{	1	,	1	,	5	,	5	,	7	,	39	,	109	,	53	,	1	,	543	,	623	,	573	,	4039	,	1367	,	23177	};
            const unsigned long	dim3519KuoInit[]		=	{	1	,	3	,	1	,	11	,	1	,	31	,	3	,	203	,	409	,	409	,	1471	,	1245	,	3881	,	14467	,	22731	};
            const unsigned long	dim3520KuoInit[]		=	{	1	,	3	,	1	,	11	,	19	,	41	,	39	,	95	,	187	,	823	,	973	,	3055	,	3803	,	4229	,	29363	};
            const unsigned long	dim3521KuoInit[]		=	{	1	,	1	,	5	,	15	,	29	,	41	,	69	,	121	,	385	,	717	,	1601	,	3061	,	6523	,	13423	,	12559	};
            const unsigned long	dim3522KuoInit[]		=	{	1	,	3	,	7	,	1	,	5	,	45	,	85	,	153	,	337	,	587	,	595	,	1183	,	3799	,	12775	,	19537	};
            const unsigned long	dim3523KuoInit[]		=	{	1	,	3	,	7	,	15	,	11	,	45	,	97	,	37	,	167	,	227	,	1003	,	2991	,	2291	,	15865	,	6645	};
            const unsigned long	dim3524KuoInit[]		=	{	1	,	3	,	3	,	7	,	31	,	41	,	123	,	225	,	275	,	733	,	1611	,	1599	,	7345	,	297	,	22551	};
            const unsigned long	dim3525KuoInit[]		=	{	1	,	1	,	5	,	3	,	7	,	45	,	45	,	155	,	41	,	581	,	635	,	4065	,	3993	,	7233	,	27097	};
            const unsigned long	dim3526KuoInit[]		=	{	1	,	1	,	7	,	11	,	9	,	39	,	17	,	19	,	335	,	819	,	1845	,	1199	,	591	,	7743	,	26797	};
            const unsigned long	dim3527KuoInit[]		=	{	1	,	3	,	7	,	9	,	29	,	33	,	35	,	19	,	287	,	935	,	1463	,	3639	,	5537	,	2751	,	20073	};
            const unsigned long	dim3528KuoInit[]		=	{	1	,	3	,	5	,	5	,	3	,	45	,	59	,	117	,	431	,	419	,	1325	,	983	,	6065	,	3501	,	15765	};
            const unsigned long	dim3529KuoInit[]		=	{	1	,	1	,	1	,	9	,	27	,	7	,	123	,	205	,	155	,	59	,	1139	,	1147	,	4845	,	15131	,	10155	};
            const unsigned long	dim3530KuoInit[]		=	{	1	,	3	,	7	,	5	,	23	,	37	,	97	,	33	,	337	,	837	,	1379	,	2459	,	469	,	10615	,	16475	};
            const unsigned long	dim3531KuoInit[]		=	{	1	,	3	,	1	,	7	,	1	,	23	,	27	,	63	,	503	,	635	,	935	,	1767	,	6887	,	6675	,	10973	};
            const unsigned long	dim3532KuoInit[]		=	{	1	,	3	,	1	,	9	,	13	,	29	,	15	,	109	,	185	,	33	,	1039	,	959	,	1745	,	10587	,	9269	};
            const unsigned long	dim3533KuoInit[]		=	{	1	,	3	,	3	,	1	,	5	,	29	,	89	,	63	,	341	,	773	,	1851	,	775	,	1717	,	6041	,	11901	};
            const unsigned long	dim3534KuoInit[]		=	{	1	,	1	,	7	,	9	,	23	,	47	,	21	,	111	,	289	,	997	,	1559	,	501	,	5913	,	4737	,	4623	};
            const unsigned long	dim3535KuoInit[]		=	{	1	,	1	,	1	,	15	,	3	,	3	,	87	,	197	,	391	,	121	,	627	,	3705	,	4093	,	3933	,	13787	};
            const unsigned long	dim3536KuoInit[]		=	{	1	,	3	,	3	,	15	,	31	,	9	,	117	,	47	,	373	,	697	,	131	,	3933	,	3135	,	9155	,	3937	};
            const unsigned long	dim3537KuoInit[]		=	{	1	,	1	,	7	,	3	,	11	,	39	,	5	,	27	,	253	,	301	,	1151	,	3771	,	1581	,	9811	,	17617	};
            const unsigned long	dim3538KuoInit[]		=	{	1	,	3	,	7	,	15	,	25	,	59	,	97	,	229	,	291	,	349	,	1153	,	1859	,	3357	,	7919	,	2733	};
            const unsigned long	dim3539KuoInit[]		=	{	1	,	1	,	5	,	13	,	5	,	7	,	37	,	177	,	177	,	969	,	29	,	4017	,	6791	,	2823	,	10607	};
            const unsigned long	dim3540KuoInit[]		=	{	1	,	1	,	7	,	13	,	27	,	17	,	25	,	137	,	325	,	949	,	1519	,	3711	,	4399	,	9501	,	2989	};
            const unsigned long	dim3541KuoInit[]		=	{	1	,	1	,	5	,	5	,	29	,	27	,	75	,	195	,	417	,	945	,	1851	,	2887	,	2379	,	15229	,	4805	};
            const unsigned long	dim3542KuoInit[]		=	{	1	,	1	,	3	,	11	,	31	,	45	,	105	,	87	,	241	,	153	,	1941	,	3459	,	4809	,	9823	,	19419	};
            const unsigned long	dim3543KuoInit[]		=	{	1	,	1	,	1	,	1	,	1	,	59	,	75	,	51	,	309	,	671	,	1039	,	2999	,	1025	,	15823	,	4835	};
            const unsigned long	dim3544KuoInit[]		=	{	1	,	3	,	7	,	13	,	19	,	37	,	41	,	85	,	11	,	713	,	1047	,	989	,	2941	,	6423	,	7983	};
            const unsigned long	dim3545KuoInit[]		=	{	1	,	3	,	5	,	13	,	1	,	55	,	103	,	207	,	275	,	431	,	563	,	101	,	691	,	1997	,	17227	};
            const unsigned long	dim3546KuoInit[]		=	{	1	,	3	,	1	,	3	,	19	,	15	,	67	,	197	,	125	,	377	,	367	,	601	,	4983	,	1699	,	29315	};
            const unsigned long	dim3547KuoInit[]		=	{	1	,	1	,	5	,	3	,	23	,	19	,	19	,	133	,	83	,	625	,	757	,	1349	,	5821	,	2483	,	26653	};
            const unsigned long	dim3548KuoInit[]		=	{	1	,	1	,	3	,	3	,	19	,	17	,	19	,	83	,	235	,	657	,	733	,	1773	,	7011	,	3893	,	8495	};
            const unsigned long	dim3549KuoInit[]		=	{	1	,	1	,	3	,	15	,	19	,	33	,	37	,	69	,	385	,	835	,	1025	,	1585	,	6685	,	10627	,	19203	};
            const unsigned long	dim3550KuoInit[]		=	{	1	,	1	,	5	,	11	,	9	,	31	,	65	,	177	,	153	,	781	,	1183	,	1215	,	6271	,	13099	,	12449	};
            const unsigned long	dim3551KuoInit[]		=	{	1	,	1	,	7	,	9	,	13	,	49	,	41	,	57	,	443	,	633	,	1953	,	2423	,	789	,	5591	,	20891	};
            const unsigned long	dim3552KuoInit[]		=	{	1	,	1	,	1	,	15	,	17	,	21	,	69	,	177	,	57	,	797	,	13	,	3987	,	6597	,	9935	,	26559	};
            const unsigned long	dim3553KuoInit[]		=	{	1	,	3	,	3	,	1	,	21	,	27	,	17	,	207	,	139	,	43	,	1935	,	3285	,	4251	,	4411	,	24469	};
            const unsigned long	dim3554KuoInit[]		=	{	1	,	3	,	3	,	11	,	11	,	47	,	63	,	243	,	19	,	801	,	971	,	1925	,	5981	,	13055	,	12185	};
            const unsigned long	dim3555KuoInit[]		=	{	1	,	3	,	5	,	5	,	9	,	13	,	49	,	251	,	315	,	889	,	1613	,	1007	,	7849	,	3121	,	4279	};
            const unsigned long	dim3556KuoInit[]		=	{	1	,	3	,	1	,	15	,	7	,	37	,	95	,	253	,	65	,	177	,	1551	,	2307	,	2325	,	9975	,	18277	};
            const unsigned long	dim3557KuoInit[]		=	{	1	,	3	,	1	,	15	,	7	,	53	,	45	,	5	,	447	,	1005	,	279	,	2141	,	4271	,	4707	,	11225	};
            const unsigned long	dim3558KuoInit[]		=	{	1	,	1	,	3	,	13	,	31	,	11	,	15	,	133	,	313	,	249	,	1971	,	2905	,	819	,	11603	,	25679	};
            const unsigned long	dim3559KuoInit[]		=	{	1	,	3	,	3	,	15	,	21	,	49	,	107	,	229	,	101	,	449	,	245	,	1401	,	391	,	3749	,	14555	};
            const unsigned long	dim3560KuoInit[]		=	{	1	,	1	,	1	,	15	,	7	,	39	,	69	,	223	,	37	,	771	,	1469	,	2085	,	1075	,	14485	,	10019	};
            const unsigned long	dim3561KuoInit[]		=	{	1	,	3	,	1	,	13	,	3	,	27	,	37	,	59	,	69	,	675	,	807	,	2711	,	3867	,	9737	,	21913	};
            const unsigned long	dim3562KuoInit[]		=	{	1	,	1	,	3	,	9	,	25	,	45	,	109	,	113	,	267	,	867	,	771	,	1647	,	2063	,	14063	,	27737	};
            const unsigned long	dim3563KuoInit[]		=	{	1	,	1	,	7	,	15	,	23	,	51	,	71	,	33	,	103	,	71	,	1177	,	2353	,	4741	,	6907	,	11121	};
            const unsigned long	dim3564KuoInit[]		=	{	1	,	3	,	3	,	7	,	21	,	31	,	77	,	255	,	341	,	249	,	1263	,	505	,	1845	,	12867	,	11393	};
            const unsigned long	dim3565KuoInit[]		=	{	1	,	3	,	1	,	1	,	3	,	1	,	5	,	215	,	155	,	107	,	1283	,	3501	,	8079	,	13921	,	22415	};
            const unsigned long	dim3566KuoInit[]		=	{	1	,	1	,	7	,	9	,	31	,	21	,	21	,	29	,	341	,	13	,	155	,	2161	,	517	,	13135	,	15893	};
            const unsigned long	dim3567KuoInit[]		=	{	1	,	1	,	7	,	5	,	21	,	33	,	23	,	131	,	367	,	939	,	1139	,	865	,	6015	,	6945	,	2301	};
            const unsigned long	dim3568KuoInit[]		=	{	1	,	3	,	7	,	3	,	1	,	25	,	109	,	183	,	267	,	757	,	371	,	1459	,	3671	,	2925	,	10345	};
            const unsigned long	dim3569KuoInit[]		=	{	1	,	1	,	5	,	15	,	29	,	57	,	95	,	169	,	301	,	645	,	5	,	225	,	5711	,	305	,	16515	};
            const unsigned long	dim3570KuoInit[]		=	{	1	,	3	,	7	,	9	,	27	,	47	,	95	,	121	,	203	,	341	,	657	,	2467	,	7619	,	9587	,	7981	};
            const unsigned long	dim3571KuoInit[]		=	{	1	,	3	,	1	,	5	,	27	,	11	,	103	,	67	,	19	,	289	,	187	,	3857	,	8117	,	12931	,	11279	};
            const unsigned long	dim3572KuoInit[]		=	{	1	,	1	,	5	,	1	,	31	,	9	,	57	,	67	,	507	,	405	,	181	,	2301	,	715	,	12165	,	18415	};
            const unsigned long	dim3573KuoInit[]		=	{	1	,	1	,	7	,	1	,	11	,	3	,	35	,	215	,	177	,	679	,	735	,	1569	,	6231	,	5141	,	28291	};
            const unsigned long	dim3574KuoInit[]		=	{	1	,	3	,	1	,	9	,	7	,	7	,	101	,	247	,	103	,	373	,	1485	,	2515	,	3981	,	12029	,	5845	};
            const unsigned long	dim3575KuoInit[]		=	{	1	,	1	,	1	,	9	,	29	,	27	,	117	,	111	,	107	,	737	,	629	,	1331	,	7495	,	9813	,	24925	};
            const unsigned long	dim3576KuoInit[]		=	{	1	,	1	,	1	,	13	,	17	,	11	,	95	,	11	,	1	,	525	,	1677	,	2673	,	2353	,	7211	,	1075	};
            const unsigned long	dim3577KuoInit[]		=	{	1	,	3	,	3	,	7	,	9	,	29	,	9	,	125	,	249	,	815	,	105	,	1649	,	2789	,	6041	,	15287	};
            const unsigned long	dim3578KuoInit[]		=	{	1	,	3	,	3	,	3	,	25	,	53	,	87	,	99	,	427	,	255	,	285	,	2339	,	5003	,	10395	,	20969	};
            const unsigned long	dim3579KuoInit[]		=	{	1	,	3	,	1	,	5	,	25	,	33	,	97	,	149	,	207	,	925	,	645	,	1867	,	4591	,	10823	,	16603	};
            const unsigned long	dim3580KuoInit[]		=	{	1	,	3	,	3	,	13	,	1	,	33	,	71	,	49	,	417	,	535	,	923	,	525	,	811	,	7443	,	12971	};
            const unsigned long	dim3581KuoInit[]		=	{	1	,	3	,	5	,	3	,	5	,	9	,	39	,	121	,	87	,	883	,	1957	,	1375	,	4109	,	12691	,	15557	};
            const unsigned long	dim3582KuoInit[]		=	{	1	,	1	,	7	,	9	,	23	,	11	,	59	,	41	,	117	,	243	,	1953	,	3967	,	7303	,	10767	,	1345	};
            const unsigned long	dim3583KuoInit[]		=	{	1	,	3	,	1	,	7	,	29	,	59	,	19	,	177	,	405	,	197	,	59	,	855	,	5009	,	11887	,	6361	};
            const unsigned long	dim3584KuoInit[]		=	{	1	,	3	,	7	,	7	,	19	,	59	,	71	,	199	,	207	,	757	,	1147	,	1857	,	3787	,	16081	,	29557	};
            const unsigned long	dim3585KuoInit[]		=	{	1	,	1	,	5	,	9	,	25	,	13	,	37	,	27	,	511	,	983	,	1087	,	371	,	4953	,	8987	,	3155	};
            const unsigned long	dim3586KuoInit[]		=	{	1	,	3	,	7	,	15	,	21	,	43	,	75	,	227	,	441	,	523	,	551	,	3517	,	77	,	5869	,	24607	};
            const unsigned long	dim3587KuoInit[]		=	{	1	,	1	,	5	,	15	,	1	,	33	,	25	,	217	,	277	,	73	,	105	,	1697	,	1545	,	10205	,	4275	};
            const unsigned long	dim3588KuoInit[]		=	{	1	,	1	,	5	,	7	,	3	,	53	,	99	,	19	,	121	,	427	,	585	,	2471	,	103	,	3771	,	23525	};
            const unsigned long	dim3589KuoInit[]		=	{	1	,	3	,	5	,	3	,	15	,	17	,	35	,	205	,	9	,	941	,	1281	,	1339	,	7421	,	3341	,	4393	};
            const unsigned long	dim3590KuoInit[]		=	{	1	,	1	,	1	,	13	,	27	,	13	,	35	,	99	,	87	,	157	,	951	,	2739	,	1993	,	6107	,	29023	};
            const unsigned long	dim3591KuoInit[]		=	{	1	,	3	,	5	,	15	,	15	,	5	,	83	,	5	,	283	,	393	,	1273	,	2251	,	4181	,	3851	,	6845	};
            const unsigned long	dim3592KuoInit[]		=	{	1	,	1	,	5	,	3	,	1	,	9	,	63	,	97	,	83	,	93	,	293	,	2087	,	2259	,	10313	,	1037	};
            const unsigned long	dim3593KuoInit[]		=	{	1	,	1	,	1	,	5	,	11	,	19	,	39	,	19	,	327	,	329	,	189	,	3851	,	4839	,	1669	,	25805	};
            const unsigned long	dim3594KuoInit[]		=	{	1	,	3	,	3	,	7	,	21	,	1	,	41	,	233	,	497	,	265	,	1041	,	4003	,	8029	,	15	,	14437	};
            const unsigned long	dim3595KuoInit[]		=	{	1	,	3	,	7	,	5	,	7	,	15	,	123	,	215	,	403	,	931	,	277	,	3501	,	3667	,	13313	,	23977	};
            const unsigned long	dim3596KuoInit[]		=	{	1	,	1	,	3	,	9	,	5	,	23	,	99	,	161	,	25	,	403	,	1695	,	2493	,	199	,	15745	,	9167	};
            const unsigned long	dim3597KuoInit[]		=	{	1	,	1	,	1	,	15	,	1	,	25	,	93	,	31	,	227	,	833	,	1891	,	1171	,	4551	,	3329	,	30593	};
            const unsigned long	dim3598KuoInit[]		=	{	1	,	3	,	1	,	11	,	29	,	51	,	109	,	27	,	475	,	511	,	1763	,	2285	,	3191	,	7943	,	8373	};
            const unsigned long	dim3599KuoInit[]		=	{	1	,	1	,	5	,	3	,	21	,	63	,	89	,	9	,	429	,	613	,	141	,	4011	,	6563	,	5871	,	30701	};
            const unsigned long	dim3600KuoInit[]		=	{	1	,	1	,	5	,	1	,	31	,	49	,	87	,	15	,	481	,	237	,	133	,	3267	,	7385	,	4733	,	16853	};
            const unsigned long	dim3601KuoInit[]		=	{	1	,	3	,	7	,	9	,	1	,	63	,	95	,	17	,	311	,	29	,	1543	,	2971	,	695	,	3641	,	28155	};
            const unsigned long	dim3602KuoInit[]		=	{	1	,	3	,	3	,	7	,	27	,	1	,	59	,	181	,	321	,	431	,	143	,	213	,	1471	,	4707	,	22897	};
            const unsigned long	dim3603KuoInit[]		=	{	1	,	1	,	3	,	11	,	19	,	3	,	95	,	1	,	407	,	755	,	1581	,	3435	,	2025	,	7781	,	5573	};
            const unsigned long	dim3604KuoInit[]		=	{	1	,	1	,	1	,	1	,	19	,	15	,	119	,	169	,	109	,	1003	,	403	,	1633	,	4963	,	9785	,	18483	};
            const unsigned long	dim3605KuoInit[]		=	{	1	,	3	,	7	,	1	,	23	,	5	,	77	,	79	,	429	,	921	,	1913	,	2075	,	7923	,	5515	,	19257	};
            const unsigned long	dim3606KuoInit[]		=	{	1	,	3	,	1	,	11	,	1	,	45	,	99	,	249	,	385	,	427	,	1201	,	1363	,	335	,	12561	,	18753	};
            const unsigned long	dim3607KuoInit[]		=	{	1	,	1	,	1	,	11	,	27	,	41	,	9	,	13	,	267	,	987	,	1023	,	2723	,	3873	,	15911	,	3165	};
            const unsigned long	dim3608KuoInit[]		=	{	1	,	3	,	5	,	1	,	17	,	7	,	117	,	241	,	509	,	757	,	1301	,	3935	,	3361	,	13745	,	27239	};
            const unsigned long	dim3609KuoInit[]		=	{	1	,	1	,	5	,	15	,	21	,	33	,	55	,	245	,	1	,	263	,	437	,	2279	,	1201	,	2043	,	20473	};
            const unsigned long	dim3610KuoInit[]		=	{	1	,	3	,	7	,	5	,	29	,	9	,	31	,	165	,	299	,	923	,	847	,	769	,	5683	,	1805	,	13433	};
            const unsigned long	dim3611KuoInit[]		=	{	1	,	1	,	1	,	11	,	29	,	13	,	49	,	173	,	269	,	35	,	1057	,	973	,	1853	,	12677	,	13565	};
            const unsigned long	dim3612KuoInit[]		=	{	1	,	3	,	1	,	11	,	11	,	21	,	19	,	3	,	127	,	133	,	193	,	2665	,	7187	,	15425	,	23805	};
            const unsigned long	dim3613KuoInit[]		=	{	1	,	3	,	5	,	7	,	13	,	61	,	95	,	17	,	197	,	381	,	865	,	1241	,	685	,	9525	,	2503	};
            const unsigned long	dim3614KuoInit[]		=	{	1	,	1	,	5	,	7	,	13	,	19	,	83	,	141	,	179	,	201	,	463	,	2083	,	3617	,	1945	,	20371	};
            const unsigned long	dim3615KuoInit[]		=	{	1	,	3	,	5	,	13	,	9	,	39	,	47	,	3	,	13	,	739	,	1501	,	1007	,	6963	,	15819	,	29739	};
            const unsigned long	dim3616KuoInit[]		=	{	1	,	1	,	5	,	7	,	3	,	17	,	105	,	27	,	165	,	515	,	949	,	81	,	947	,	9615	,	24403	};
            const unsigned long	dim3617KuoInit[]		=	{	1	,	3	,	7	,	5	,	23	,	43	,	81	,	141	,	181	,	1007	,	1343	,	1919	,	5971	,	8563	,	17091	};
            const unsigned long	dim3618KuoInit[]		=	{	1	,	3	,	3	,	11	,	1	,	25	,	19	,	117	,	225	,	347	,	785	,	791	,	7321	,	15647	,	25491	};
            const unsigned long	dim3619KuoInit[]		=	{	1	,	1	,	5	,	5	,	11	,	61	,	25	,	205	,	145	,	695	,	713	,	1787	,	8087	,	5935	,	5105	};
            const unsigned long	dim3620KuoInit[]		=	{	1	,	1	,	7	,	1	,	15	,	17	,	25	,	141	,	75	,	657	,	1641	,	575	,	3387	,	6093	,	15565	};
            const unsigned long	dim3621KuoInit[]		=	{	1	,	3	,	7	,	11	,	31	,	49	,	113	,	37	,	321	,	547	,	819	,	2079	,	7231	,	7497	,	29877	};
            const unsigned long	dim3622KuoInit[]		=	{	1	,	1	,	3	,	11	,	31	,	19	,	39	,	99	,	271	,	527	,	1551	,	2491	,	5515	,	5541	,	6131	};
            const unsigned long	dim3623KuoInit[]		=	{	1	,	1	,	1	,	7	,	23	,	35	,	31	,	35	,	315	,	911	,	1133	,	3509	,	1381	,	15869	,	23211	};
            const unsigned long	dim3624KuoInit[]		=	{	1	,	3	,	3	,	5	,	1	,	33	,	127	,	175	,	311	,	497	,	187	,	2983	,	6553	,	9653	,	5711	};
            const unsigned long	dim3625KuoInit[]		=	{	1	,	1	,	3	,	13	,	19	,	3	,	65	,	91	,	265	,	199	,	2035	,	1277	,	4575	,	5517	,	15247	};
            const unsigned long	dim3626KuoInit[]		=	{	1	,	1	,	5	,	15	,	31	,	39	,	71	,	167	,	337	,	245	,	399	,	2681	,	1257	,	15117	,	24145	};
            const unsigned long	dim3627KuoInit[]		=	{	1	,	1	,	1	,	11	,	7	,	19	,	113	,	133	,	83	,	851	,	1543	,	1699	,	4527	,	3167	,	10793	};
            const unsigned long	dim3628KuoInit[]		=	{	1	,	3	,	3	,	3	,	31	,	55	,	35	,	161	,	133	,	83	,	469	,	3011	,	4513	,	4677	,	2363	};
            const unsigned long	dim3629KuoInit[]		=	{	1	,	1	,	3	,	1	,	29	,	51	,	29	,	163	,	95	,	221	,	973	,	2819	,	3103	,	9969	,	10347	};
            const unsigned long	dim3630KuoInit[]		=	{	1	,	1	,	5	,	7	,	31	,	5	,	73	,	159	,	339	,	523	,	399	,	4041	,	4341	,	11527	,	505	};
            const unsigned long	dim3631KuoInit[]		=	{	1	,	1	,	3	,	7	,	15	,	55	,	91	,	117	,	233	,	497	,	485	,	1187	,	5185	,	13127	,	14393	};
            const unsigned long	dim3632KuoInit[]		=	{	1	,	3	,	7	,	9	,	27	,	19	,	23	,	197	,	99	,	779	,	423	,	2735	,	3513	,	8227	,	31127	};
            const unsigned long	dim3633KuoInit[]		=	{	1	,	3	,	3	,	1	,	3	,	61	,	89	,	137	,	189	,	325	,	1797	,	2287	,	5033	,	7351	,	1131	};
            const unsigned long	dim3634KuoInit[]		=	{	1	,	3	,	3	,	15	,	19	,	9	,	83	,	87	,	451	,	317	,	667	,	3379	,	2225	,	6437	,	31515	};
            const unsigned long	dim3635KuoInit[]		=	{	1	,	1	,	1	,	15	,	13	,	31	,	121	,	131	,	473	,	717	,	1507	,	1483	,	4819	,	14375	,	24667	};
            const unsigned long	dim3636KuoInit[]		=	{	1	,	1	,	1	,	5	,	5	,	55	,	101	,	111	,	89	,	481	,	2031	,	3043	,	5727	,	1253	,	11951	};
            const unsigned long	dim3637KuoInit[]		=	{	1	,	3	,	1	,	1	,	3	,	13	,	67	,	129	,	311	,	125	,	121	,	2833	,	3555	,	7761	,	27343	};
            const unsigned long	dim3638KuoInit[]		=	{	1	,	3	,	7	,	7	,	29	,	37	,	63	,	59	,	5	,	691	,	1625	,	97	,	6113	,	14795	,	31471	};
            const unsigned long	dim3639KuoInit[]		=	{	1	,	3	,	3	,	3	,	25	,	7	,	65	,	207	,	371	,	29	,	1627	,	1785	,	5203	,	5153	,	1461	};
            const unsigned long	dim3640KuoInit[]		=	{	1	,	3	,	3	,	11	,	1	,	37	,	121	,	249	,	381	,	309	,	1545	,	4039	,	5657	,	6783	,	1889	};
            const unsigned long	dim3641KuoInit[]		=	{	1	,	1	,	1	,	9	,	29	,	19	,	51	,	213	,	185	,	481	,	1929	,	2475	,	567	,	10045	,	32619	};
            const unsigned long	dim3642KuoInit[]		=	{	1	,	1	,	3	,	7	,	13	,	13	,	17	,	47	,	55	,	521	,	473	,	495	,	7807	,	1645	,	17713	};
            const unsigned long	dim3643KuoInit[]		=	{	1	,	1	,	1	,	3	,	5	,	63	,	119	,	167	,	357	,	231	,	1915	,	3605	,	3579	,	3041	,	603	};
            const unsigned long	dim3644KuoInit[]		=	{	1	,	3	,	3	,	7	,	31	,	5	,	93	,	193	,	175	,	801	,	1963	,	1413	,	1641	,	4073	,	16017	};
            const unsigned long	dim3645KuoInit[]		=	{	1	,	3	,	3	,	11	,	11	,	49	,	53	,	57	,	1	,	799	,	301	,	2611	,	2975	,	7595	,	25891	};
            const unsigned long	dim3646KuoInit[]		=	{	1	,	3	,	1	,	3	,	19	,	41	,	63	,	173	,	257	,	903	,	1977	,	3799	,	3985	,	14807	,	24903	};
            const unsigned long	dim3647KuoInit[]		=	{	1	,	3	,	3	,	11	,	17	,	53	,	61	,	115	,	453	,	395	,	993	,	3365	,	4071	,	4737	,	29469	};
            const unsigned long	dim3648KuoInit[]		=	{	1	,	3	,	1	,	13	,	3	,	55	,	109	,	149	,	313	,	621	,	345	,	4037	,	677	,	3787	,	6701	};
            const unsigned long	dim3649KuoInit[]		=	{	1	,	3	,	3	,	9	,	23	,	11	,	63	,	21	,	47	,	949	,	47	,	3055	,	3403	,	7581	,	12853	};
            const unsigned long	dim3650KuoInit[]		=	{	1	,	1	,	1	,	9	,	7	,	21	,	15	,	203	,	167	,	677	,	49	,	2777	,	7817	,	13207	,	31485	};
            const unsigned long	dim3651KuoInit[]		=	{	1	,	1	,	1	,	3	,	29	,	41	,	37	,	61	,	343	,	279	,	127	,	925	,	7091	,	14477	,	24149	};
            const unsigned long	dim3652KuoInit[]		=	{	1	,	1	,	1	,	11	,	3	,	33	,	35	,	237	,	133	,	653	,	1491	,	2183	,	3565	,	14773	,	4521	};
            const unsigned long	dim3653KuoInit[]		=	{	1	,	1	,	5	,	3	,	31	,	43	,	13	,	79	,	465	,	785	,	1913	,	1133	,	2701	,	9007	,	15695	};
            const unsigned long	dim3654KuoInit[]		=	{	1	,	3	,	3	,	1	,	23	,	43	,	5	,	155	,	369	,	131	,	671	,	1109	,	7951	,	1929	,	23965	};
            const unsigned long	dim3655KuoInit[]		=	{	1	,	1	,	5	,	7	,	9	,	15	,	23	,	59	,	511	,	931	,	259	,	1953	,	1197	,	9441	,	25545	};
            const unsigned long	dim3656KuoInit[]		=	{	1	,	1	,	7	,	7	,	27	,	25	,	51	,	81	,	479	,	491	,	203	,	4009	,	4621	,	11153	,	12049	};
            const unsigned long	dim3657KuoInit[]		=	{	1	,	3	,	5	,	15	,	25	,	11	,	77	,	253	,	249	,	761	,	707	,	161	,	7519	,	5381	,	1549	};
            const unsigned long	dim3658KuoInit[]		=	{	1	,	1	,	1	,	13	,	11	,	39	,	59	,	191	,	443	,	471	,	53	,	1639	,	505	,	1281	,	1919	};
            const unsigned long	dim3659KuoInit[]		=	{	1	,	1	,	3	,	7	,	29	,	47	,	121	,	135	,	411	,	95	,	1255	,	635	,	835	,	7905	,	18435	};
            const unsigned long	dim3660KuoInit[]		=	{	1	,	1	,	3	,	15	,	21	,	57	,	65	,	75	,	333	,	61	,	1861	,	3903	,	6051	,	8311	,	2791	};
            const unsigned long	dim3661KuoInit[]		=	{	1	,	3	,	3	,	7	,	7	,	49	,	121	,	233	,	305	,	697	,	525	,	3921	,	5747	,	2951	,	24321	};
            const unsigned long	dim3662KuoInit[]		=	{	1	,	1	,	3	,	3	,	9	,	23	,	53	,	121	,	187	,	35	,	1973	,	829	,	575	,	4003	,	32159	};
            const unsigned long	dim3663KuoInit[]		=	{	1	,	1	,	3	,	1	,	3	,	57	,	25	,	99	,	383	,	97	,	1751	,	1799	,	2001	,	8335	,	20349	};
            const unsigned long	dim3664KuoInit[]		=	{	1	,	1	,	7	,	9	,	11	,	17	,	25	,	107	,	483	,	183	,	559	,	2531	,	6509	,	9449	,	1793	};		
            const unsigned long	dim3665KuoInit[]		=	{	1	,	3	,	3	,	1	,	3	,	43	,	89	,	57	,	15	,	269	,	521	,	1119	,	3573	,	9801	,	30943	};		
            const unsigned long	dim3666KuoInit[]		=	{	1	,	3	,	7	,	3	,	7	,	41	,	101	,	177	,	47	,	47	,	2009	,	2127	,	5113	,	13541	,	4133	};		
            const unsigned long	dim3667KuoInit[]		=	{	1	,	1	,	3	,	7	,	23	,	63	,	67	,	73	,	335	,	471	,	1137	,	3101	,	5539	,	16203	,	15395	,	12455	};
            const unsigned long	dim3668KuoInit[]		=	{	1	,	3	,	1	,	11	,	5	,	9	,	43	,	123	,	91	,	23	,	1199	,	287	,	5343	,	1937	,	12115	,	4749	};
            const unsigned long	dim3669KuoInit[]		=	{	1	,	1	,	7	,	11	,	27	,	3	,	29	,	251	,	413	,	147	,	403	,	2065	,	2695	,	11657	,	15519	,	49391	};
            const unsigned long	dim3670KuoInit[]		=	{	1	,	3	,	7	,	11	,	25	,	7	,	125	,	137	,	479	,	753	,	309	,	1075	,	7991	,	2629	,	4039	,	6749	};
            const unsigned long	dim3671KuoInit[]		=	{	1	,	1	,	7	,	1	,	3	,	27	,	69	,	227	,	503	,	915	,	569	,	115	,	4299	,	10799	,	13745	,	7973	};
            const unsigned long	dim3672KuoInit[]		=	{	1	,	3	,	1	,	11	,	17	,	49	,	33	,	161	,	429	,	153	,	1179	,	4005	,	6507	,	10507	,	1779	,	5331	};
            const unsigned long	dim3673KuoInit[]		=	{	1	,	3	,	1	,	13	,	25	,	45	,	117	,	47	,	253	,	851	,	627	,	593	,	7555	,	3677	,	30695	,	38851	};
            const unsigned long	dim3674KuoInit[]		=	{	1	,	3	,	1	,	11	,	25	,	19	,	95	,	191	,	505	,	683	,	67	,	2375	,	995	,	8647	,	4853	,	17483	};
            const unsigned long	dim3675KuoInit[]		=	{	1	,	3	,	3	,	15	,	7	,	57	,	101	,	151	,	477	,	343	,	879	,	2161	,	1585	,	1903	,	6459	,	45285	};
            const unsigned long	dim3676KuoInit[]		=	{	1	,	1	,	5	,	3	,	7	,	7	,	31	,	219	,	137	,	107	,	119	,	2959	,	7453	,	9215	,	20463	,	61165	};
            const unsigned long	dim3677KuoInit[]		=	{	1	,	1	,	3	,	15	,	25	,	39	,	41	,	165	,	421	,	567	,	1095	,	4055	,	7321	,	831	,	81	,	50377	};
            const unsigned long	dim3678KuoInit[]		=	{	1	,	1	,	3	,	1	,	21	,	23	,	65	,	31	,	331	,	875	,	1449	,	653	,	1975	,	783	,	29021	,	47225	};
            const unsigned long	dim3679KuoInit[]		=	{	1	,	1	,	7	,	1	,	21	,	21	,	39	,	39	,	289	,	129	,	345	,	2983	,	2419	,	133	,	2519	,	33307	};
            const unsigned long	dim3680KuoInit[]		=	{	1	,	3	,	5	,	1	,	27	,	23	,	53	,	63	,	315	,	93	,	335	,	3239	,	6749	,	12985	,	5863	,	49565	};
            const unsigned long	dim3681KuoInit[]		=	{	1	,	3	,	7	,	11	,	17	,	23	,	89	,	253	,	329	,	837	,	589	,	1135	,	4099	,	3123	,	25567	,	21017	};
            const unsigned long	dim3682KuoInit[]		=	{	1	,	3	,	7	,	3	,	21	,	17	,	111	,	21	,	73	,	901	,	2017	,	3711	,	159	,	6703	,	32209	,	34411	};
            const unsigned long	dim3683KuoInit[]		=	{	1	,	1	,	1	,	9	,	19	,	59	,	1	,	31	,	273	,	683	,	1155	,	2543	,	477	,	8479	,	25235	,	61595	};
            const unsigned long	dim3684KuoInit[]		=	{	1	,	1	,	7	,	1	,	13	,	17	,	97	,	49	,	221	,	63	,	197	,	465	,	1801	,	5389	,	10395	,	54157	};
            const unsigned long	dim3685KuoInit[]		=	{	1	,	1	,	5	,	9	,	11	,	59	,	79	,	139	,	249	,	185	,	1131	,	507	,	3325	,	2713	,	27293	,	21539	};
            const unsigned long	dim3686KuoInit[]		=	{	1	,	1	,	3	,	5	,	9	,	45	,	103	,	145	,	327	,	675	,	1609	,	1703	,	761	,	4947	,	187	,	53903	};
            const unsigned long	dim3687KuoInit[]		=	{	1	,	1	,	1	,	9	,	19	,	13	,	77	,	143	,	303	,	119	,	1985	,	1347	,	6661	,	6081	,	9723	,	52129	};
            const unsigned long	dim3688KuoInit[]		=	{	1	,	3	,	3	,	3	,	27	,	27	,	15	,	163	,	139	,	159	,	1921	,	3549	,	6477	,	11423	,	16055	,	32399	};
            const unsigned long	dim3689KuoInit[]		=	{	1	,	3	,	1	,	13	,	19	,	59	,	5	,	73	,	481	,	945	,	187	,	1223	,	1771	,	12961	,	13853	,	14513	};
            const unsigned long	dim3690KuoInit[]		=	{	1	,	1	,	7	,	11	,	5	,	63	,	33	,	185	,	477	,	857	,	1057	,	2509	,	7341	,	7669	,	13829	,	51965	};
            const unsigned long	dim3691KuoInit[]		=	{	1	,	1	,	3	,	5	,	23	,	17	,	7	,	87	,	101	,	817	,	241	,	853	,	4067	,	6409	,	28217	,	54395	};
            const unsigned long	dim3692KuoInit[]		=	{	1	,	3	,	3	,	3	,	21	,	55	,	47	,	159	,	407	,	675	,	1805	,	2079	,	1113	,	149	,	26071	,	30829	};
            const unsigned long	dim3693KuoInit[]		=	{	1	,	1	,	3	,	13	,	7	,	3	,	89	,	161	,	95	,	333	,	525	,	3979	,	6831	,	8007	,	7063	,	45299	};
            const unsigned long	dim3694KuoInit[]		=	{	1	,	1	,	7	,	5	,	17	,	37	,	95	,	129	,	215	,	313	,	1395	,	2415	,	6191	,	6409	,	29717	,	4199	};
            const unsigned long	dim3695KuoInit[]		=	{	1	,	1	,	1	,	7	,	7	,	45	,	75	,	175	,	407	,	763	,	1037	,	2477	,	3167	,	6991	,	23905	,	14707	};
            const unsigned long	dim3696KuoInit[]		=	{	1	,	3	,	7	,	7	,	23	,	23	,	103	,	217	,	479	,	779	,	941	,	3173	,	3639	,	12833	,	7133	,	39269	};
            const unsigned long	dim3697KuoInit[]		=	{	1	,	1	,	1	,	5	,	9	,	55	,	19	,	147	,	287	,	291	,	1751	,	2475	,	6639	,	5933	,	27837	,	33713	};
            const unsigned long	dim3698KuoInit[]		=	{	1	,	1	,	5	,	5	,	11	,	39	,	65	,	109	,	95	,	637	,	1897	,	2395	,	6975	,	12431	,	25097	,	15649	};
            const unsigned long	dim3699KuoInit[]		=	{	1	,	1	,	3	,	7	,	25	,	37	,	27	,	53	,	373	,	881	,	1221	,	2127	,	1775	,	2025	,	10249	,	55325	};
            const unsigned long	dim3700KuoInit[]		=	{	1	,	1	,	7	,	15	,	5	,	37	,	1	,	99	,	249	,	209	,	1177	,	2953	,	4455	,	12125	,	7679	,	25543	};
            const unsigned long	dim3701KuoInit[]		=	{	1	,	3	,	3	,	13	,	9	,	19	,	97	,	11	,	331	,	107	,	1475	,	769	,	1749	,	9375	,	19477	,	47559	};
            const unsigned long	dim3702KuoInit[]		=	{	1	,	3	,	7	,	15	,	25	,	57	,	69	,	45	,	21	,	455	,	1841	,	1101	,	2931	,	2845	,	21925	,	43731	};
            const unsigned long	dim3703KuoInit[]		=	{	1	,	1	,	3	,	11	,	27	,	33	,	31	,	33	,	9	,	825	,	883	,	2259	,	7317	,	5507	,	3931	,	61075	};
            const unsigned long	dim3704KuoInit[]		=	{	1	,	3	,	7	,	5	,	7	,	21	,	97	,	203	,	449	,	671	,	1037	,	3985	,	5655	,	11099	,	21981	,	50149	};
            const unsigned long	dim3705KuoInit[]		=	{	1	,	1	,	1	,	1	,	5	,	19	,	109	,	247	,	73	,	981	,	1981	,	3613	,	6575	,	3493	,	9553	,	259	};
            const unsigned long	dim3706KuoInit[]		=	{	1	,	1	,	7	,	9	,	3	,	55	,	13	,	175	,	285	,	865	,	1817	,	911	,	6393	,	4843	,	31569	,	30115	};
            const unsigned long	dim3707KuoInit[]		=	{	1	,	1	,	3	,	15	,	21	,	51	,	85	,	239	,	171	,	741	,	881	,	749	,	4021	,	2049	,	25495	,	62913	};
            const unsigned long	dim3708KuoInit[]		=	{	1	,	3	,	1	,	1	,	19	,	9	,	111	,	119	,	171	,	903	,	567	,	1513	,	8179	,	10553	,	5315	,	63291	};
            const unsigned long	dim3709KuoInit[]		=	{	1	,	3	,	5	,	1	,	1	,	61	,	21	,	197	,	165	,	371	,	3	,	2085	,	669	,	4497	,	27467	,	52995	};
            const unsigned long	dim3710KuoInit[]		=	{	1	,	3	,	5	,	13	,	29	,	37	,	95	,	247	,	161	,	621	,	83	,	2597	,	6779	,	13747	,	18873	,	6173	};
            const unsigned long	dim3711KuoInit[]		=	{	1	,	1	,	3	,	5	,	21	,	5	,	41	,	137	,	77	,	829	,	689	,	563	,	5331	,	8061	,	21371	,	58565	};
            const unsigned long	dim3712KuoInit[]		=	{	1	,	3	,	7	,	13	,	1	,	19	,	11	,	111	,	35	,	39	,	1513	,	1241	,	3739	,	16191	,	25097	,	25947	};
            const unsigned long	dim3713KuoInit[]		=	{	1	,	1	,	5	,	1	,	27	,	45	,	11	,	29	,	153	,	321	,	1657	,	1419	,	6983	,	11935	,	8295	,	24313	};
            const unsigned long	dim3714KuoInit[]		=	{	1	,	3	,	3	,	11	,	1	,	57	,	109	,	139	,	53	,	883	,	81	,	333	,	5133	,	13891	,	7775	,	13401	};
            const unsigned long	dim3715KuoInit[]		=	{	1	,	3	,	5	,	15	,	5	,	35	,	121	,	15	,	73	,	313	,	1903	,	437	,	1567	,	11905	,	7679	,	14265	};
            const unsigned long	dim3716KuoInit[]		=	{	1	,	3	,	7	,	3	,	31	,	63	,	69	,	135	,	75	,	489	,	1271	,	585	,	7361	,	15965	,	3099	,	37309	};
            const unsigned long	dim3717KuoInit[]		=	{	1	,	1	,	7	,	11	,	15	,	21	,	97	,	73	,	115	,	795	,	421	,	69	,	7795	,	9895	,	1755	,	44375	};
            const unsigned long	dim3718KuoInit[]		=	{	1	,	3	,	5	,	9	,	17	,	13	,	49	,	61	,	395	,	637	,	1737	,	3103	,	5197	,	7799	,	25425	,	56313	};
            const unsigned long	dim3719KuoInit[]		=	{	1	,	3	,	1	,	9	,	17	,	19	,	87	,	131	,	315	,	865	,	813	,	163	,	1045	,	12573	,	12657	,	43715	};
            const unsigned long	dim3720KuoInit[]		=	{	1	,	3	,	7	,	3	,	27	,	23	,	9	,	185	,	291	,	421	,	603	,	1339	,	1085	,	15449	,	1543	,	5815	};
            const unsigned long	dim3721KuoInit[]		=	{	1	,	3	,	1	,	5	,	29	,	35	,	15	,	75	,	135	,	911	,	1153	,	3067	,	3475	,	11653	,	27485	,	8459	};
            const unsigned long	dim3722KuoInit[]		=	{	1	,	3	,	5	,	7	,	3	,	9	,	43	,	29	,	77	,	551	,	973	,	2165	,	1499	,	9185	,	27025	,	31605	};
            const unsigned long	dim3723KuoInit[]		=	{	1	,	1	,	1	,	9	,	23	,	49	,	13	,	229	,	295	,	497	,	187	,	1995	,	1883	,	9109	,	2689	,	37275	};
            const unsigned long	dim3724KuoInit[]		=	{	1	,	1	,	7	,	15	,	11	,	1	,	61	,	183	,	129	,	869	,	1271	,	3439	,	7743	,	5831	,	12829	,	1341	};
            const unsigned long	dim3725KuoInit[]		=	{	1	,	3	,	7	,	1	,	17	,	45	,	31	,	29	,	441	,	533	,	1479	,	3519	,	2083	,	149	,	22119	,	37615	};
            const unsigned long	dim3726KuoInit[]		=	{	1	,	1	,	3	,	7	,	31	,	39	,	101	,	209	,	237	,	211	,	1015	,	1339	,	857	,	3833	,	16875	,	38147	};
            const unsigned long	dim3727KuoInit[]		=	{	1	,	3	,	7	,	13	,	29	,	23	,	9	,	79	,	71	,	363	,	487	,	595	,	259	,	12425	,	28181	,	20717	};
            const unsigned long	dim3728KuoInit[]		=	{	1	,	3	,	3	,	11	,	1	,	21	,	97	,	39	,	101	,	1	,	121	,	3083	,	109	,	7889	,	29005	,	57081	};
            const unsigned long	dim3729KuoInit[]		=	{	1	,	3	,	3	,	15	,	19	,	19	,	91	,	17	,	281	,	121	,	1653	,	3099	,	181	,	15911	,	29553	,	10047	};
            const unsigned long	dim3730KuoInit[]		=	{	1	,	1	,	3	,	9	,	27	,	39	,	69	,	57	,	25	,	595	,	723	,	1779	,	7537	,	9397	,	5563	,	8197	};
            const unsigned long	dim3731KuoInit[]		=	{	1	,	1	,	1	,	1	,	11	,	61	,	127	,	81	,	439	,	723	,	1147	,	3639	,	1207	,	5127	,	10209	,	33629	};
            const unsigned long	dim3732KuoInit[]		=	{	1	,	3	,	1	,	9	,	27	,	55	,	69	,	147	,	477	,	663	,	833	,	3113	,	2585	,	9497	,	679	,	62759	};
            const unsigned long	dim3733KuoInit[]		=	{	1	,	3	,	1	,	15	,	9	,	51	,	67	,	191	,	105	,	3	,	1439	,	1915	,	3059	,	3001	,	4035	,	551	};
            const unsigned long	dim3734KuoInit[]		=	{	1	,	1	,	3	,	15	,	1	,	3	,	11	,	171	,	189	,	659	,	1279	,	3995	,	6729	,	6187	,	22213	,	22709	};
            const unsigned long	dim3735KuoInit[]		=	{	1	,	1	,	7	,	11	,	27	,	31	,	3	,	183	,	209	,	931	,	99	,	1901	,	113	,	15779	,	11525	,	29949	};
            const unsigned long	dim3736KuoInit[]		=	{	1	,	3	,	7	,	13	,	11	,	51	,	1	,	173	,	271	,	785	,	1875	,	2115	,	1977	,	189	,	7819	,	8133	};
            const unsigned long	dim3737KuoInit[]		=	{	1	,	3	,	7	,	7	,	21	,	9	,	5	,	253	,	495	,	415	,	3	,	1979	,	2331	,	12229	,	29901	,	7115	};
            const unsigned long	dim3738KuoInit[]		=	{	1	,	3	,	1	,	15	,	21	,	11	,	105	,	81	,	269	,	347	,	901	,	81	,	7077	,	11813	,	18739	,	57959	};
            const unsigned long	dim3739KuoInit[]		=	{	1	,	1	,	1	,	1	,	19	,	57	,	17	,	87	,	325	,	129	,	1907	,	1641	,	6247	,	12073	,	31629	,	28503	};
            const unsigned long	dim3740KuoInit[]		=	{	1	,	1	,	5	,	15	,	29	,	27	,	81	,	11	,	383	,	881	,	1385	,	3787	,	503	,	1369	,	25031	,	42049	};
            const unsigned long	dim3741KuoInit[]		=	{	1	,	3	,	7	,	15	,	27	,	15	,	13	,	13	,	279	,	785	,	163	,	1907	,	4523	,	12133	,	1497	,	18845	};
            const unsigned long	dim3742KuoInit[]		=	{	1	,	3	,	7	,	9	,	13	,	37	,	51	,	83	,	309	,	645	,	383	,	1607	,	2087	,	14287	,	14375	,	39619	};
            const unsigned long	dim3743KuoInit[]		=	{	1	,	3	,	7	,	5	,	11	,	13	,	21	,	45	,	95	,	667	,	1597	,	1823	,	5887	,	10909	,	26279	,	29845	};
            const unsigned long	dim3744KuoInit[]		=	{	1	,	3	,	1	,	1	,	25	,	61	,	21	,	91	,	81	,	173	,	1271	,	339	,	2185	,	16033	,	3687	,	53797	};
            const unsigned long	dim3745KuoInit[]		=	{	1	,	3	,	3	,	15	,	19	,	27	,	59	,	153	,	453	,	523	,	85	,	3367	,	4439	,	13605	,	6219	,	25593	};
            const unsigned long	dim3746KuoInit[]		=	{	1	,	3	,	5	,	3	,	27	,	57	,	127	,	225	,	199	,	823	,	431	,	2911	,	4421	,	14063	,	1467	,	46787	};
            const unsigned long	dim3747KuoInit[]		=	{	1	,	3	,	5	,	1	,	31	,	33	,	17	,	73	,	357	,	199	,	677	,	2015	,	1213	,	8503	,	4785	,	54835	};
            const unsigned long	dim3748KuoInit[]		=	{	1	,	1	,	1	,	11	,	15	,	13	,	65	,	1	,	311	,	469	,	1307	,	2605	,	5281	,	15665	,	6349	,	61753	};
            const unsigned long	dim3749KuoInit[]		=	{	1	,	1	,	1	,	3	,	19	,	33	,	49	,	141	,	229	,	925	,	1059	,	1295	,	3093	,	6207	,	7607	,	63987	};
            const unsigned long	dim3750KuoInit[]		=	{	1	,	1	,	3	,	1	,	21	,	63	,	17	,	131	,	27	,	295	,	1969	,	3259	,	2011	,	11005	,	32249	,	57641	};
            const unsigned long	dim3751KuoInit[]		=	{	1	,	3	,	7	,	15	,	25	,	27	,	11	,	105	,	99	,	923	,	1003	,	2765	,	49	,	10367	,	22119	,	27199	};
            const unsigned long	dim3752KuoInit[]		=	{	1	,	1	,	1	,	3	,	31	,	11	,	57	,	253	,	31	,	259	,	1687	,	1135	,	1675	,	12051	,	3543	,	19087	};
            const unsigned long	dim3753KuoInit[]		=	{	1	,	3	,	5	,	9	,	9	,	53	,	119	,	215	,	213	,	67	,	343	,	2743	,	7303	,	14225	,	13379	,	51001	};
            const unsigned long	dim3754KuoInit[]		=	{	1	,	3	,	7	,	9	,	1	,	45	,	11	,	125	,	209	,	459	,	1037	,	673	,	2497	,	669	,	11251	,	19387	};
            const unsigned long	dim3755KuoInit[]		=	{	1	,	1	,	1	,	15	,	21	,	47	,	63	,	45	,	349	,	561	,	1823	,	2005	,	4173	,	5595	,	3789	,	34133	};
            const unsigned long	dim3756KuoInit[]		=	{	1	,	1	,	3	,	3	,	5	,	7	,	83	,	63	,	305	,	679	,	879	,	3613	,	7627	,	9849	,	3987	,	28795	};
            const unsigned long	dim3757KuoInit[]		=	{	1	,	3	,	7	,	11	,	7	,	51	,	119	,	1	,	21	,	213	,	1399	,	3715	,	7775	,	1003	,	6749	,	5547	};
            const unsigned long	dim3758KuoInit[]		=	{	1	,	1	,	7	,	9	,	21	,	53	,	51	,	137	,	379	,	439	,	1497	,	3455	,	2509	,	2539	,	3583	,	5107	};
            const unsigned long	dim3759KuoInit[]		=	{	1	,	3	,	5	,	3	,	9	,	13	,	73	,	237	,	65	,	53	,	663	,	3895	,	7579	,	10027	,	23969	,	34767	};
            const unsigned long	dim3760KuoInit[]		=	{	1	,	3	,	7	,	3	,	13	,	57	,	125	,	245	,	243	,	561	,	91	,	863	,	1925	,	13553	,	4113	,	31077	};
            const unsigned long	dim3761KuoInit[]		=	{	1	,	1	,	5	,	7	,	9	,	31	,	9	,	83	,	355	,	249	,	1741	,	3593	,	425	,	6377	,	10607	,	48275	};
            const unsigned long	dim3762KuoInit[]		=	{	1	,	3	,	7	,	3	,	19	,	17	,	123	,	75	,	413	,	981	,	877	,	3489	,	5907	,	6141	,	2729	,	49219	};
            const unsigned long	dim3763KuoInit[]		=	{	1	,	3	,	1	,	3	,	31	,	39	,	97	,	233	,	99	,	17	,	105	,	2911	,	347	,	4069	,	32535	,	47179	};
            const unsigned long	dim3764KuoInit[]		=	{	1	,	1	,	5	,	7	,	1	,	51	,	1	,	59	,	197	,	487	,	1763	,	2449	,	4249	,	55	,	11885	,	57597	};
            const unsigned long	dim3765KuoInit[]		=	{	1	,	3	,	5	,	3	,	25	,	49	,	43	,	101	,	353	,	759	,	1295	,	3441	,	1521	,	2879	,	9815	,	27091	};
            const unsigned long	dim3766KuoInit[]		=	{	1	,	3	,	5	,	11	,	1	,	7	,	69	,	205	,	271	,	511	,	1157	,	3597	,	2857	,	10997	,	15525	,	6807	};
            const unsigned long	dim3767KuoInit[]		=	{	1	,	1	,	1	,	15	,	11	,	1	,	45	,	213	,	463	,	389	,	1761	,	3005	,	3359	,	12829	,	18211	,	2107	};
            const unsigned long	dim3768KuoInit[]		=	{	1	,	1	,	7	,	13	,	27	,	59	,	115	,	203	,	321	,	711	,	1489	,	2893	,	1427	,	4599	,	17105	,	10063	};
            const unsigned long	dim3769KuoInit[]		=	{	1	,	3	,	1	,	13	,	5	,	5	,	53	,	221	,	97	,	535	,	1967	,	1951	,	4077	,	327	,	18243	,	43285	};
            const unsigned long	dim3770KuoInit[]		=	{	1	,	3	,	5	,	13	,	9	,	61	,	59	,	255	,	143	,	21	,	1859	,	3509	,	4295	,	5107	,	11117	,	29431	};
            const unsigned long	dim3771KuoInit[]		=	{	1	,	3	,	3	,	13	,	19	,	13	,	33	,	169	,	17	,	773	,	1693	,	147	,	5143	,	2357	,	2295	,	50187	};
            const unsigned long	dim3772KuoInit[]		=	{	1	,	1	,	5	,	11	,	11	,	19	,	79	,	151	,	441	,	825	,	665	,	2947	,	5391	,	8695	,	155	,	26751	};
            const unsigned long	dim3773KuoInit[]		=	{	1	,	1	,	7	,	15	,	13	,	31	,	77	,	61	,	249	,	341	,	1737	,	3567	,	6337	,	14457	,	3799	,	20283	};
            const unsigned long	dim3774KuoInit[]		=	{	1	,	1	,	7	,	11	,	13	,	5	,	83	,	59	,	185	,	325	,	2029	,	1329	,	1125	,	10799	,	27107	,	2817	};
            const unsigned long	dim3775KuoInit[]		=	{	1	,	3	,	5	,	7	,	17	,	43	,	21	,	113	,	57	,	503	,	1647	,	393	,	4525	,	8385	,	27153	,	37175	};
            const unsigned long	dim3776KuoInit[]		=	{	1	,	3	,	1	,	15	,	23	,	57	,	3	,	177	,	419	,	65	,	1895	,	1107	,	7705	,	7915	,	20677	,	27931	};
            const unsigned long	dim3777KuoInit[]		=	{	1	,	3	,	7	,	13	,	13	,	63	,	53	,	237	,	237	,	161	,	265	,	1053	,	161	,	11635	,	1455	,	55551	};
            const unsigned long	dim3778KuoInit[]		=	{	1	,	3	,	7	,	5	,	15	,	41	,	7	,	77	,	93	,	737	,	1195	,	1381	,	399	,	7829	,	6251	,	34883	};
            const unsigned long	dim3779KuoInit[]		=	{	1	,	1	,	5	,	7	,	23	,	63	,	119	,	35	,	115	,	829	,	441	,	4059	,	4755	,	11331	,	18071	,	6911	};
            const unsigned long	dim3780KuoInit[]		=	{	1	,	3	,	3	,	7	,	21	,	33	,	49	,	47	,	329	,	341	,	45	,	3093	,	5937	,	6803	,	31681	,	9575	};
            const unsigned long	dim3781KuoInit[]		=	{	1	,	1	,	1	,	13	,	13	,	19	,	121	,	71	,	231	,	915	,	981	,	93	,	5879	,	15645	,	8555	,	58411	};
            const unsigned long	dim3782KuoInit[]		=	{	1	,	3	,	7	,	5	,	31	,	45	,	71	,	189	,	337	,	955	,	1263	,	2271	,	1615	,	15137	,	29933	,	16585	};
            const unsigned long	dim3783KuoInit[]		=	{	1	,	1	,	7	,	15	,	29	,	49	,	85	,	255	,	143	,	585	,	1099	,	1159	,	1519	,	15593	,	6177	,	16083	};
            const unsigned long	dim3784KuoInit[]		=	{	1	,	3	,	5	,	1	,	13	,	59	,	79	,	67	,	341	,	815	,	703	,	3857	,	4711	,	15155	,	15997	,	31699	};
            const unsigned long	dim3785KuoInit[]		=	{	1	,	3	,	3	,	7	,	11	,	51	,	77	,	231	,	241	,	261	,	1607	,	4031	,	3779	,	4289	,	22039	,	47177	};
            const unsigned long	dim3786KuoInit[]		=	{	1	,	3	,	7	,	1	,	21	,	23	,	31	,	233	,	225	,	883	,	715	,	777	,	2433	,	3665	,	26695	,	60997	};
            const unsigned long	dim3787KuoInit[]		=	{	1	,	3	,	1	,	1	,	27	,	17	,	55	,	33	,	133	,	899	,	1443	,	283	,	6839	,	9497	,	22399	,	60527	};
            const unsigned long	dim3788KuoInit[]		=	{	1	,	1	,	3	,	3	,	5	,	63	,	91	,	5	,	275	,	219	,	1679	,	2385	,	4727	,	9995	,	14079	,	24695	};
            const unsigned long	dim3789KuoInit[]		=	{	1	,	3	,	7	,	15	,	9	,	3	,	77	,	215	,	265	,	317	,	1655	,	1027	,	5029	,	6867	,	9465	,	64257	};
            const unsigned long	dim3790KuoInit[]		=	{	1	,	3	,	1	,	3	,	23	,	7	,	67	,	63	,	503	,	269	,	85	,	3669	,	5941	,	13833	,	25857	,	34221	};
            const unsigned long	dim3791KuoInit[]		=	{	1	,	1	,	1	,	7	,	13	,	31	,	83	,	113	,	389	,	923	,	1173	,	2457	,	997	,	9761	,	17033	,	50521	};
            const unsigned long	dim3792KuoInit[]		=	{	1	,	3	,	1	,	1	,	15	,	31	,	93	,	243	,	221	,	331	,	1961	,	3251	,	6459	,	3341	,	28535	,	27909	};
            const unsigned long	dim3793KuoInit[]		=	{	1	,	3	,	1	,	15	,	27	,	1	,	75	,	169	,	277	,	921	,	5	,	1391	,	5665	,	14659	,	6895	,	37927	};
            const unsigned long	dim3794KuoInit[]		=	{	1	,	3	,	1	,	9	,	17	,	63	,	93	,	35	,	225	,	109	,	977	,	2623	,	3779	,	8011	,	11643	,	57789	};
            const unsigned long	dim3795KuoInit[]		=	{	1	,	1	,	5	,	3	,	17	,	5	,	63	,	109	,	449	,	695	,	1021	,	2749	,	3981	,	2073	,	24537	,	43643	};
            const unsigned long	dim3796KuoInit[]		=	{	1	,	1	,	5	,	1	,	1	,	19	,	15	,	249	,	3	,	431	,	743	,	3813	,	4013	,	4027	,	1943	,	23569	};
            const unsigned long	dim3797KuoInit[]		=	{	1	,	3	,	7	,	1	,	9	,	49	,	93	,	59	,	425	,	325	,	1925	,	495	,	6611	,	2425	,	19847	,	58683	};
            const unsigned long	dim3798KuoInit[]		=	{	1	,	3	,	5	,	5	,	7	,	21	,	71	,	51	,	335	,	67	,	373	,	1805	,	1805	,	4069	,	6585	,	56579	};
            const unsigned long	dim3799KuoInit[]		=	{	1	,	3	,	1	,	7	,	25	,	27	,	49	,	131	,	491	,	105	,	1467	,	665	,	6777	,	15467	,	13597	,	49141	};
            const unsigned long	dim3800KuoInit[]		=	{	1	,	3	,	5	,	13	,	9	,	9	,	37	,	159	,	19	,	91	,	1963	,	3133	,	5147	,	8655	,	11153	,	44491	};
            const unsigned long	dim3801KuoInit[]		=	{	1	,	3	,	5	,	7	,	13	,	55	,	41	,	67	,	155	,	853	,	1651	,	3511	,	4391	,	6359	,	9129	,	32359	};
            const unsigned long	dim3802KuoInit[]		=	{	1	,	3	,	5	,	5	,	9	,	3	,	27	,	101	,	467	,	185	,	1071	,	1477	,	1523	,	9535	,	15433	,	7493	};
            const unsigned long	dim3803KuoInit[]		=	{	1	,	1	,	1	,	3	,	15	,	19	,	3	,	97	,	241	,	995	,	451	,	1651	,	241	,	1063	,	2521	,	65531	};
            const unsigned long	dim3804KuoInit[]		=	{	1	,	3	,	3	,	5	,	23	,	13	,	117	,	65	,	119	,	871	,	1245	,	3577	,	7303	,	9433	,	13701	,	21321	};
            const unsigned long	dim3805KuoInit[]		=	{	1	,	1	,	1	,	11	,	25	,	33	,	121	,	203	,	325	,	775	,	1835	,	1385	,	2541	,	3399	,	275	,	54311	};
            const unsigned long	dim3806KuoInit[]		=	{	1	,	1	,	3	,	3	,	13	,	41	,	5	,	63	,	415	,	771	,	375	,	3933	,	5239	,	1731	,	7675	,	11855	};
            const unsigned long	dim3807KuoInit[]		=	{	1	,	1	,	5	,	7	,	23	,	49	,	27	,	119	,	231	,	897	,	1995	,	3847	,	25	,	7505	,	2823	,	10115	};
            const unsigned long	dim3808KuoInit[]		=	{	1	,	3	,	5	,	1	,	5	,	41	,	41	,	129	,	339	,	861	,	147	,	3299	,	1123	,	2175	,	27111	,	2067	};
            const unsigned long	dim3809KuoInit[]		=	{	1	,	1	,	3	,	5	,	5	,	39	,	71	,	53	,	483	,	585	,	1167	,	2037	,	3125	,	499	,	29491	,	43467	};
            const unsigned long	dim3810KuoInit[]		=	{	1	,	1	,	1	,	3	,	25	,	9	,	85	,	197	,	271	,	457	,	1159	,	3805	,	2423	,	11777	,	4383	,	57495	};
            const unsigned long	dim3811KuoInit[]		=	{	1	,	3	,	7	,	9	,	25	,	5	,	33	,	255	,	325	,	643	,	499	,	3233	,	4477	,	14189	,	12183	,	7667	};
            const unsigned long	dim3812KuoInit[]		=	{	1	,	3	,	3	,	5	,	1	,	63	,	3	,	51	,	173	,	1011	,	1423	,	3453	,	2471	,	7801	,	29989	,	8347	};
            const unsigned long	dim3813KuoInit[]		=	{	1	,	1	,	7	,	11	,	17	,	39	,	23	,	13	,	413	,	503	,	321	,	3699	,	6441	,	11953	,	24253	,	2561	};
            const unsigned long	dim3814KuoInit[]		=	{	1	,	1	,	1	,	3	,	31	,	45	,	21	,	215	,	55	,	555	,	771	,	2703	,	8113	,	3279	,	6053	,	48617	};
            const unsigned long	dim3815KuoInit[]		=	{	1	,	1	,	7	,	9	,	25	,	25	,	33	,	101	,	111	,	643	,	1277	,	3833	,	7431	,	14395	,	20849	,	14455	};
            const unsigned long	dim3816KuoInit[]		=	{	1	,	3	,	7	,	11	,	21	,	15	,	95	,	179	,	437	,	747	,	1131	,	83	,	415	,	12605	,	29245	,	28021	};
            const unsigned long	dim3817KuoInit[]		=	{	1	,	3	,	7	,	5	,	13	,	59	,	9	,	217	,	287	,	929	,	691	,	3201	,	4915	,	3301	,	15371	,	17063	};
            const unsigned long	dim3818KuoInit[]		=	{	1	,	1	,	1	,	7	,	31	,	3	,	27	,	229	,	151	,	321	,	801	,	1271	,	1195	,	11221	,	16903	,	27919	};
            const unsigned long	dim3819KuoInit[]		=	{	1	,	1	,	5	,	9	,	5	,	41	,	105	,	133	,	265	,	167	,	505	,	3939	,	771	,	1291	,	1109	,	52601	};
            const unsigned long	dim3820KuoInit[]		=	{	1	,	3	,	5	,	11	,	21	,	23	,	23	,	229	,	197	,	623	,	1973	,	901	,	147	,	15751	,	29429	,	13159	};
            const unsigned long	dim3821KuoInit[]		=	{	1	,	1	,	5	,	11	,	25	,	53	,	79	,	123	,	245	,	939	,	1889	,	4073	,	417	,	13625	,	14691	,	56965	};
            const unsigned long	dim3822KuoInit[]		=	{	1	,	1	,	1	,	11	,	1	,	39	,	51	,	231	,	367	,	233	,	1453	,	3095	,	6071	,	7521	,	469	,	8407	};
            const unsigned long	dim3823KuoInit[]		=	{	1	,	3	,	5	,	1	,	9	,	17	,	109	,	135	,	85	,	151	,	1217	,	1643	,	4455	,	3923	,	11681	,	32807	};
            const unsigned long	dim3824KuoInit[]		=	{	1	,	1	,	1	,	13	,	11	,	39	,	27	,	205	,	85	,	1019	,	1759	,	1705	,	4585	,	8311	,	29123	,	44045	};
            const unsigned long	dim3825KuoInit[]		=	{	1	,	3	,	1	,	3	,	9	,	33	,	65	,	253	,	319	,	65	,	851	,	3357	,	2827	,	10107	,	10197	,	19111	};
            const unsigned long	dim3826KuoInit[]		=	{	1	,	3	,	7	,	3	,	3	,	53	,	67	,	99	,	501	,	603	,	1409	,	359	,	7395	,	12827	,	10867	,	17473	};
            const unsigned long	dim3827KuoInit[]		=	{	1	,	1	,	7	,	5	,	5	,	35	,	97	,	49	,	365	,	971	,	1707	,	1305	,	6801	,	14451	,	32473	,	57269	};
            const unsigned long	dim3828KuoInit[]		=	{	1	,	1	,	5	,	11	,	25	,	57	,	115	,	195	,	47	,	923	,	1515	,	2043	,	803	,	6371	,	25341	,	63263	};
            const unsigned long	dim3829KuoInit[]		=	{	1	,	1	,	5	,	3	,	27	,	53	,	83	,	91	,	367	,	695	,	1809	,	927	,	6815	,	1229	,	27359	,	28937	};
            const unsigned long	dim3830KuoInit[]		=	{	1	,	1	,	3	,	7	,	3	,	3	,	17	,	203	,	95	,	577	,	1531	,	1645	,	7271	,	5541	,	30905	,	38825	};
            const unsigned long	dim3831KuoInit[]		=	{	1	,	1	,	7	,	9	,	7	,	57	,	97	,	185	,	75	,	681	,	1197	,	3713	,	6429	,	4723	,	26275	,	23495	};
            const unsigned long	dim3832KuoInit[]		=	{	1	,	3	,	1	,	3	,	5	,	25	,	71	,	171	,	143	,	603	,	1349	,	1723	,	7493	,	8417	,	24171	,	13421	};
            const unsigned long	dim3833KuoInit[]		=	{	1	,	3	,	1	,	15	,	23	,	57	,	33	,	247	,	409	,	511	,	29	,	1403	,	5831	,	1749	,	28441	,	4117	};
            const unsigned long	dim3834KuoInit[]		=	{	1	,	1	,	3	,	15	,	31	,	11	,	105	,	239	,	431	,	127	,	343	,	961	,	6449	,	1677	,	24937	,	15859	};
            const unsigned long	dim3835KuoInit[]		=	{	1	,	1	,	5	,	3	,	27	,	57	,	61	,	245	,	425	,	417	,	301	,	521	,	2425	,	2547	,	11349	,	47931	};
            const unsigned long	dim3836KuoInit[]		=	{	1	,	3	,	5	,	3	,	15	,	19	,	19	,	127	,	151	,	913	,	1061	,	1795	,	2519	,	6297	,	2519	,	56677	};
            const unsigned long	dim3837KuoInit[]		=	{	1	,	1	,	3	,	9	,	25	,	3	,	67	,	53	,	305	,	609	,	1067	,	1003	,	6933	,	14009	,	24475	,	23183	};
            const unsigned long	dim3838KuoInit[]		=	{	1	,	1	,	5	,	7	,	21	,	35	,	89	,	95	,	399	,	341	,	1337	,	2815	,	3605	,	3209	,	9881	,	6399	};
            const unsigned long	dim3839KuoInit[]		=	{	1	,	1	,	7	,	1	,	29	,	63	,	23	,	219	,	145	,	583	,	985	,	2577	,	3951	,	15199	,	26981	,	2755	};
            const unsigned long	dim3840KuoInit[]		=	{	1	,	3	,	7	,	1	,	13	,	45	,	105	,	139	,	103	,	83	,	819	,	2655	,	39	,	11661	,	11057	,	38189	};
            const unsigned long	dim3841KuoInit[]		=	{	1	,	3	,	1	,	15	,	23	,	41	,	79	,	237	,	353	,	67	,	1551	,	2345	,	1169	,	4623	,	1391	,	18607	};
            const unsigned long	dim3842KuoInit[]		=	{	1	,	1	,	1	,	13	,	29	,	31	,	43	,	65	,	25	,	387	,	1109	,	2515	,	3967	,	2221	,	27321	,	20511	};
            const unsigned long	dim3843KuoInit[]		=	{	1	,	1	,	1	,	3	,	17	,	45	,	35	,	211	,	31	,	45	,	1791	,	2225	,	6995	,	15241	,	24555	,	45993	};
            const unsigned long	dim3844KuoInit[]		=	{	1	,	1	,	5	,	5	,	31	,	25	,	71	,	57	,	87	,	531	,	117	,	267	,	2415	,	2213	,	603	,	23369	};
            const unsigned long	dim3845KuoInit[]		=	{	1	,	1	,	1	,	5	,	23	,	53	,	39	,	3	,	93	,	697	,	25	,	465	,	1517	,	6061	,	11759	,	34863	};
            const unsigned long	dim3846KuoInit[]		=	{	1	,	1	,	1	,	13	,	29	,	29	,	41	,	27	,	499	,	285	,	355	,	3915	,	1683	,	11933	,	24685	,	59013	};
            const unsigned long	dim3847KuoInit[]		=	{	1	,	3	,	1	,	3	,	21	,	21	,	25	,	251	,	109	,	469	,	1841	,	889	,	6541	,	12961	,	9449	,	48755	};
            const unsigned long	dim3848KuoInit[]		=	{	1	,	3	,	1	,	1	,	21	,	59	,	41	,	159	,	159	,	105	,	999	,	2535	,	3251	,	12213	,	24005	,	17145	};
            const unsigned long	dim3849KuoInit[]		=	{	1	,	1	,	7	,	9	,	1	,	39	,	43	,	235	,	447	,	767	,	871	,	453	,	7285	,	3387	,	14625	,	40099	};
            const unsigned long	dim3850KuoInit[]		=	{	1	,	1	,	3	,	3	,	5	,	11	,	15	,	205	,	183	,	309	,	1747	,	1089	,	3141	,	9129	,	25149	,	63109	};
            const unsigned long	dim3851KuoInit[]		=	{	1	,	3	,	7	,	15	,	23	,	15	,	7	,	157	,	45	,	263	,	1383	,	735	,	205	,	16021	,	13233	,	23425	};
            const unsigned long	dim3852KuoInit[]		=	{	1	,	1	,	7	,	13	,	11	,	31	,	53	,	23	,	357	,	953	,	611	,	2579	,	475	,	15183	,	12755	,	10347	};
            const unsigned long	dim3853KuoInit[]		=	{	1	,	1	,	1	,	5	,	7	,	27	,	41	,	215	,	247	,	1001	,	1369	,	3105	,	3195	,	13417	,	6967	,	52501	};
            const unsigned long	dim3854KuoInit[]		=	{	1	,	3	,	7	,	3	,	3	,	61	,	35	,	175	,	415	,	835	,	1381	,	3951	,	6857	,	10273	,	26657	,	5239	};
            const unsigned long	dim3855KuoInit[]		=	{	1	,	3	,	7	,	5	,	1	,	27	,	3	,	251	,	499	,	637	,	1101	,	3817	,	4823	,	3065	,	29919	,	4151	};
            const unsigned long	dim3856KuoInit[]		=	{	1	,	3	,	7	,	13	,	19	,	39	,	1	,	141	,	353	,	853	,	295	,	1201	,	6895	,	3965	,	6417	,	47379	};
            const unsigned long	dim3857KuoInit[]		=	{	1	,	1	,	7	,	13	,	11	,	9	,	99	,	225	,	161	,	349	,	421	,	2803	,	4461	,	2319	,	2495	,	62429	};
            const unsigned long	dim3858KuoInit[]		=	{	1	,	3	,	1	,	13	,	31	,	1	,	47	,	115	,	299	,	331	,	1917	,	2547	,	969	,	4841	,	8237	,	2405	};
            const unsigned long	dim3859KuoInit[]		=	{	1	,	1	,	1	,	9	,	9	,	19	,	77	,	35	,	225	,	211	,	1875	,	3531	,	2753	,	12679	,	6863	,	42143	};
            const unsigned long	dim3860KuoInit[]		=	{	1	,	1	,	1	,	3	,	29	,	25	,	105	,	19	,	269	,	863	,	811	,	2741	,	143	,	6353	,	25109	,	62251	};
            const unsigned long	dim3861KuoInit[]		=	{	1	,	3	,	5	,	3	,	27	,	51	,	11	,	61	,	245	,	517	,	891	,	2081	,	4523	,	11703	,	31477	,	12113	};
            const unsigned long	dim3862KuoInit[]		=	{	1	,	1	,	1	,	1	,	27	,	29	,	25	,	47	,	143	,	489	,	1475	,	1139	,	1855	,	8031	,	21341	,	23619	};
            const unsigned long	dim3863KuoInit[]		=	{	1	,	1	,	5	,	1	,	3	,	45	,	103	,	59	,	123	,	831	,	1883	,	149	,	5843	,	10275	,	3121	,	21411	};
            const unsigned long	dim3864KuoInit[]		=	{	1	,	1	,	7	,	1	,	21	,	11	,	1	,	35	,	285	,	501	,	1407	,	1307	,	1371	,	15057	,	14091	,	52279	};
            const unsigned long	dim3865KuoInit[]		=	{	1	,	1	,	3	,	13	,	3	,	5	,	9	,	167	,	139	,	255	,	1561	,	2183	,	1079	,	2055	,	24779	,	19737	};
            const unsigned long	dim3866KuoInit[]		=	{	1	,	1	,	7	,	13	,	31	,	29	,	59	,	157	,	311	,	447	,	21	,	2177	,	3679	,	7519	,	987	,	44221	};
            const unsigned long	dim3867KuoInit[]		=	{	1	,	3	,	3	,	1	,	11	,	31	,	95	,	23	,	457	,	733	,	1413	,	273	,	333	,	14391	,	16557	,	35227	};
            const unsigned long	dim3868KuoInit[]		=	{	1	,	1	,	3	,	9	,	5	,	3	,	103	,	59	,	21	,	475	,	1455	,	2995	,	7651	,	4893	,	21739	,	21379	};
            const unsigned long	dim3869KuoInit[]		=	{	1	,	3	,	5	,	1	,	9	,	63	,	55	,	127	,	283	,	603	,	1477	,	331	,	2115	,	15965	,	22433	,	17311	};
            const unsigned long	dim3870KuoInit[]		=	{	1	,	1	,	3	,	3	,	5	,	61	,	105	,	175	,	245	,	431	,	479	,	821	,	3835	,	3585	,	16533	,	63233	};
            const unsigned long	dim3871KuoInit[]		=	{	1	,	1	,	7	,	1	,	31	,	57	,	9	,	11	,	145	,	519	,	1915	,	2809	,	2943	,	13885	,	31223	,	32897	};
            const unsigned long	dim3872KuoInit[]		=	{	1	,	3	,	1	,	15	,	5	,	39	,	67	,	179	,	285	,	91	,	1429	,	3571	,	1925	,	9737	,	28647	,	8189	};
            const unsigned long	dim3873KuoInit[]		=	{	1	,	3	,	7	,	11	,	21	,	25	,	51	,	173	,	151	,	283	,	765	,	2717	,	3541	,	10317	,	9827	,	24445	};
            const unsigned long	dim3874KuoInit[]		=	{	1	,	3	,	1	,	13	,	31	,	25	,	121	,	71	,	347	,	129	,	159	,	1845	,	475	,	12777	,	19607	,	7163	};
            const unsigned long	dim3875KuoInit[]		=	{	1	,	3	,	1	,	9	,	29	,	37	,	59	,	117	,	375	,	411	,	1031	,	3781	,	3119	,	6851	,	30759	,	46181	};
            const unsigned long	dim3876KuoInit[]		=	{	1	,	3	,	1	,	7	,	5	,	41	,	81	,	141	,	377	,	817	,	1007	,	2211	,	7303	,	5833	,	6117	,	52265	};
            const unsigned long	dim3877KuoInit[]		=	{	1	,	3	,	5	,	3	,	7	,	63	,	83	,	181	,	169	,	19	,	1589	,	3793	,	7003	,	12145	,	25625	,	35731	};
            const unsigned long	dim3878KuoInit[]		=	{	1	,	1	,	1	,	5	,	21	,	39	,	15	,	225	,	231	,	497	,	917	,	559	,	6515	,	13637	,	17721	,	4725	};
            const unsigned long	dim3879KuoInit[]		=	{	1	,	3	,	1	,	15	,	17	,	19	,	93	,	65	,	281	,	775	,	613	,	2453	,	1969	,	2499	,	5373	,	28207	};
            const unsigned long	dim3880KuoInit[]		=	{	1	,	1	,	5	,	11	,	9	,	37	,	103	,	211	,	357	,	409	,	457	,	1527	,	3289	,	14293	,	7681	,	58069	};
            const unsigned long	dim3881KuoInit[]		=	{	1	,	3	,	5	,	9	,	1	,	29	,	83	,	1	,	35	,	655	,	491	,	1447	,	725	,	7807	,	13311	,	345	};
            const unsigned long	dim3882KuoInit[]		=	{	1	,	3	,	1	,	7	,	19	,	59	,	103	,	243	,	157	,	209	,	399	,	1113	,	1595	,	15075	,	22475	,	42835	};
            const unsigned long	dim3883KuoInit[]		=	{	1	,	3	,	5	,	5	,	27	,	53	,	69	,	73	,	413	,	443	,	559	,	427	,	2841	,	13107	,	8909	,	35265	};
            const unsigned long	dim3884KuoInit[]		=	{	1	,	3	,	3	,	7	,	21	,	39	,	99	,	75	,	341	,	917	,	11	,	1247	,	247	,	14285	,	24531	,	58523	};
            const unsigned long	dim3885KuoInit[]		=	{	1	,	1	,	7	,	9	,	29	,	1	,	55	,	143	,	363	,	527	,	103	,	3735	,	6373	,	7837	,	24451	,	36999	};
            const unsigned long	dim3886KuoInit[]		=	{	1	,	1	,	3	,	3	,	3	,	51	,	71	,	103	,	173	,	397	,	1683	,	489	,	6515	,	2531	,	12517	,	63165	};
            const unsigned long	dim3887KuoInit[]		=	{	1	,	3	,	7	,	3	,	27	,	15	,	31	,	63	,	323	,	903	,	1025	,	2761	,	1813	,	2839	,	2065	,	20013	};
            const unsigned long	dim3888KuoInit[]		=	{	1	,	1	,	1	,	13	,	19	,	37	,	49	,	207	,	57	,	45	,	1517	,	2715	,	6315	,	10499	,	27195	,	56175	};
            const unsigned long	dim3889KuoInit[]		=	{	1	,	3	,	5	,	11	,	13	,	47	,	91	,	81	,	459	,	801	,	1111	,	3023	,	5901	,	12735	,	14233	,	24383	};
            const unsigned long	dim3890KuoInit[]		=	{	1	,	3	,	1	,	5	,	19	,	39	,	11	,	69	,	333	,	17	,	1339	,	2991	,	4411	,	2749	,	15423	,	13805	};
            const unsigned long	dim3891KuoInit[]		=	{	1	,	3	,	5	,	9	,	19	,	3	,	109	,	45	,	461	,	103	,	811	,	3089	,	893	,	16121	,	1337	,	31929	};
            const unsigned long	dim3892KuoInit[]		=	{	1	,	3	,	7	,	11	,	3	,	21	,	125	,	241	,	355	,	519	,	1551	,	3273	,	2413	,	3371	,	11435	,	2277	};
            const unsigned long	dim3893KuoInit[]		=	{	1	,	1	,	5	,	9	,	7	,	21	,	93	,	79	,	25	,	265	,	1045	,	2011	,	4883	,	8631	,	22461	,	31117	};
            const unsigned long	dim3894KuoInit[]		=	{	1	,	1	,	3	,	9	,	13	,	35	,	61	,	203	,	371	,	185	,	1445	,	2017	,	363	,	13603	,	23097	,	22617	};
            const unsigned long	dim3895KuoInit[]		=	{	1	,	1	,	1	,	1	,	29	,	33	,	9	,	139	,	73	,	119	,	755	,	2117	,	3949	,	3403	,	30007	,	58147	};
            const unsigned long	dim3896KuoInit[]		=	{	1	,	1	,	3	,	5	,	9	,	29	,	85	,	1	,	349	,	573	,	1191	,	133	,	235	,	1557	,	18147	,	5839	};
            const unsigned long	dim3897KuoInit[]		=	{	1	,	3	,	3	,	5	,	15	,	45	,	5	,	95	,	359	,	223	,	37	,	547	,	3407	,	4927	,	32417	,	49205	};
            const unsigned long	dim3898KuoInit[]		=	{	1	,	1	,	7	,	11	,	21	,	57	,	127	,	95	,	355	,	903	,	1555	,	9	,	1249	,	2463	,	7921	,	20633	};
            const unsigned long	dim3899KuoInit[]		=	{	1	,	3	,	3	,	11	,	29	,	35	,	95	,	237	,	43	,	289	,	1547	,	3951	,	5489	,	2699	,	347	,	14405	};
            const unsigned long	dim3900KuoInit[]		=	{	1	,	3	,	3	,	11	,	11	,	23	,	33	,	171	,	37	,	989	,	917	,	139	,	5497	,	617	,	25555	,	27031	};
            const unsigned long	dim3901KuoInit[]		=	{	1	,	1	,	5	,	1	,	21	,	55	,	63	,	191	,	93	,	207	,	1799	,	2143	,	4147	,	6853	,	18129	,	18287	};
            const unsigned long	dim3902KuoInit[]		=	{	1	,	3	,	7	,	15	,	23	,	39	,	59	,	131	,	51	,	113	,	941	,	2599	,	5593	,	11311	,	30561	,	22307	};
            const unsigned long	dim3903KuoInit[]		=	{	1	,	3	,	7	,	5	,	21	,	49	,	93	,	5	,	399	,	173	,	1017	,	3551	,	937	,	13653	,	21411	,	56835	};
            const unsigned long	dim3904KuoInit[]		=	{	1	,	3	,	1	,	1	,	15	,	47	,	57	,	19	,	93	,	931	,	1803	,	2947	,	1291	,	10979	,	22069	,	47485	};
            const unsigned long	dim3905KuoInit[]		=	{	1	,	3	,	7	,	11	,	31	,	19	,	75	,	195	,	23	,	235	,	137	,	2521	,	4109	,	233	,	26961	,	19185	};
            const unsigned long	dim3906KuoInit[]		=	{	1	,	3	,	3	,	15	,	9	,	17	,	87	,	239	,	233	,	939	,	1643	,	1189	,	7875	,	5199	,	26603	,	52223	};
            const unsigned long	dim3907KuoInit[]		=	{	1	,	3	,	5	,	7	,	3	,	31	,	21	,	255	,	41	,	95	,	1267	,	1873	,	5535	,	8419	,	19791	,	14811	};
            const unsigned long	dim3908KuoInit[]		=	{	1	,	1	,	7	,	15	,	7	,	51	,	27	,	35	,	17	,	1015	,	661	,	2679	,	1269	,	15729	,	17329	,	33555	};
            const unsigned long	dim3909KuoInit[]		=	{	1	,	1	,	5	,	1	,	13	,	19	,	39	,	105	,	173	,	255	,	1873	,	1527	,	253	,	3517	,	19867	,	56765	};
            const unsigned long	dim3910KuoInit[]		=	{	1	,	1	,	7	,	1	,	15	,	13	,	85	,	27	,	491	,	827	,	1151	,	3591	,	7819	,	10085	,	11307	,	45059	};
            const unsigned long	dim3911KuoInit[]		=	{	1	,	3	,	5	,	15	,	21	,	17	,	23	,	207	,	223	,	721	,	655	,	243	,	4289	,	4973	,	29709	,	9203	};
            const unsigned long	dim3912KuoInit[]		=	{	1	,	1	,	5	,	11	,	27	,	51	,	3	,	199	,	277	,	121	,	1637	,	3973	,	861	,	7215	,	32577	,	4391	};
            const unsigned long	dim3913KuoInit[]		=	{	1	,	1	,	7	,	9	,	21	,	27	,	33	,	199	,	29	,	917	,	99	,	2755	,	821	,	16003	,	8325	,	55587	};
            const unsigned long	dim3914KuoInit[]		=	{	1	,	1	,	7	,	1	,	19	,	17	,	61	,	39	,	377	,	739	,	13	,	3435	,	5081	,	10591	,	11957	,	30049	};
            const unsigned long	dim3915KuoInit[]		=	{	1	,	1	,	5	,	1	,	25	,	13	,	117	,	5	,	479	,	579	,	907	,	3135	,	1011	,	3389	,	135	,	41711	};
            const unsigned long	dim3916KuoInit[]		=	{	1	,	3	,	5	,	5	,	19	,	15	,	89	,	169	,	321	,	981	,	365	,	333	,	4913	,	9577	,	13835	,	23439	};
            const unsigned long	dim3917KuoInit[]		=	{	1	,	1	,	5	,	11	,	31	,	27	,	103	,	25	,	129	,	207	,	259	,	2033	,	7931	,	8357	,	17451	,	60583	};
            const unsigned long	dim3918KuoInit[]		=	{	1	,	1	,	7	,	11	,	15	,	47	,	59	,	151	,	291	,	669	,	663	,	111	,	4809	,	14179	,	22313	,	51969	};
            const unsigned long	dim3919KuoInit[]		=	{	1	,	3	,	3	,	5	,	1	,	5	,	87	,	25	,	179	,	641	,	1133	,	629	,	2281	,	6213	,	14281	,	54147	};
            const unsigned long	dim3920KuoInit[]		=	{	1	,	1	,	5	,	15	,	29	,	33	,	89	,	63	,	331	,	379	,	453	,	1903	,	7319	,	15063	,	31383	,	23099	};
            const unsigned long	dim3921KuoInit[]		=	{	1	,	3	,	1	,	15	,	23	,	7	,	13	,	71	,	367	,	53	,	983	,	3615	,	5601	,	2569	,	18173	,	40563	};
            const unsigned long	dim3922KuoInit[]		=	{	1	,	3	,	1	,	15	,	23	,	41	,	35	,	215	,	401	,	807	,	1809	,	3919	,	5651	,	1249	,	9699	,	14275	};
            const unsigned long	dim3923KuoInit[]		=	{	1	,	1	,	3	,	1	,	15	,	55	,	7	,	235	,	171	,	911	,	1351	,	2863	,	5735	,	8577	,	7489	,	42047	};
            const unsigned long	dim3924KuoInit[]		=	{	1	,	3	,	1	,	5	,	29	,	35	,	11	,	211	,	133	,	507	,	1127	,	2443	,	7895	,	10519	,	13035	,	26165	};
            const unsigned long	dim3925KuoInit[]		=	{	1	,	1	,	1	,	9	,	21	,	39	,	107	,	55	,	235	,	409	,	39	,	1681	,	2699	,	3143	,	847	,	35635	};
            const unsigned long	dim3926KuoInit[]		=	{	1	,	3	,	7	,	3	,	27	,	57	,	79	,	173	,	273	,	919	,	1917	,	4005	,	1299	,	14743	,	24093	,	64047	};
            const unsigned long	dim3927KuoInit[]		=	{	1	,	1	,	7	,	11	,	13	,	53	,	45	,	195	,	39	,	379	,	1699	,	2401	,	7131	,	7205	,	24419	,	32161	};
            const unsigned long	dim3928KuoInit[]		=	{	1	,	3	,	3	,	15	,	31	,	31	,	35	,	199	,	421	,	367	,	1517	,	1559	,	3627	,	10281	,	27963	,	31115	};
            const unsigned long	dim3929KuoInit[]		=	{	1	,	1	,	5	,	15	,	5	,	59	,	89	,	195	,	263	,	853	,	1043	,	3699	,	2543	,	9893	,	727	,	62533	};
            const unsigned long	dim3930KuoInit[]		=	{	1	,	1	,	7	,	5	,	29	,	49	,	101	,	219	,	135	,	643	,	321	,	83	,	1651	,	2347	,	1519	,	21683	};
            const unsigned long	dim3931KuoInit[]		=	{	1	,	1	,	3	,	5	,	11	,	17	,	3	,	75	,	255	,	359	,	1231	,	2333	,	6745	,	2393	,	4213	,	7689	};
            const unsigned long	dim3932KuoInit[]		=	{	1	,	1	,	5	,	13	,	23	,	27	,	53	,	51	,	307	,	87	,	1409	,	3555	,	8139	,	8597	,	27357	,	28489	};
            const unsigned long	dim3933KuoInit[]		=	{	1	,	3	,	3	,	1	,	29	,	31	,	53	,	175	,	245	,	815	,	461	,	1681	,	2255	,	761	,	18061	,	13273	};
            const unsigned long	dim3934KuoInit[]		=	{	1	,	3	,	3	,	7	,	21	,	41	,	81	,	243	,	9	,	973	,	1411	,	2327	,	4575	,	2659	,	22903	,	27605	};
            const unsigned long	dim3935KuoInit[]		=	{	1	,	1	,	3	,	7	,	1	,	3	,	91	,	203	,	149	,	101	,	1745	,	1427	,	4107	,	11967	,	13545	,	19249	};
            const unsigned long	dim3936KuoInit[]		=	{	1	,	3	,	1	,	1	,	3	,	13	,	47	,	13	,	291	,	813	,	97	,	2741	,	2847	,	2845	,	6437	,	1497	};
            const unsigned long	dim3937KuoInit[]		=	{	1	,	3	,	3	,	1	,	15	,	7	,	121	,	173	,	333	,	977	,	357	,	3439	,	8023	,	8741	,	205	,	11447	};
            const unsigned long	dim3938KuoInit[]		=	{	1	,	1	,	3	,	13	,	17	,	57	,	79	,	3	,	455	,	673	,	1877	,	2019	,	4809	,	10635	,	19927	,	38703	};
            const unsigned long	dim3939KuoInit[]		=	{	1	,	1	,	1	,	15	,	17	,	23	,	65	,	157	,	265	,	177	,	697	,	3281	,	409	,	471	,	113	,	16349	};
            const unsigned long	dim3940KuoInit[]		=	{	1	,	3	,	3	,	11	,	19	,	57	,	125	,	55	,	343	,	177	,	621	,	1827	,	983	,	2491	,	10389	,	49879	};
            const unsigned long	dim3941KuoInit[]		=	{	1	,	1	,	5	,	1	,	23	,	21	,	125	,	13	,	155	,	459	,	1677	,	2585	,	6703	,	945	,	5341	,	54821	};
            const unsigned long	dim3942KuoInit[]		=	{	1	,	3	,	3	,	1	,	1	,	59	,	75	,	137	,	261	,	563	,	1145	,	759	,	2363	,	12949	,	16677	,	43535	};
            const unsigned long	dim3943KuoInit[]		=	{	1	,	3	,	1	,	1	,	31	,	13	,	29	,	59	,	263	,	879	,	49	,	1365	,	4977	,	6981	,	32683	,	25063	};
            const unsigned long	dim3944KuoInit[]		=	{	1	,	3	,	3	,	11	,	27	,	63	,	41	,	101	,	353	,	611	,	349	,	1947	,	6473	,	3099	,	28855	,	27945	};
            const unsigned long	dim3945KuoInit[]		=	{	1	,	1	,	5	,	3	,	13	,	49	,	59	,	5	,	401	,	679	,	2045	,	2267	,	5879	,	12809	,	23767	,	47855	};
            const unsigned long	dim3946KuoInit[]		=	{	1	,	1	,	5	,	11	,	17	,	1	,	25	,	235	,	67	,	155	,	629	,	25	,	2565	,	12261	,	28847	,	19987	};
            const unsigned long	dim3947KuoInit[]		=	{	1	,	3	,	1	,	1	,	23	,	39	,	117	,	147	,	155	,	797	,	1487	,	921	,	1205	,	9947	,	14603	,	21335	};
            const unsigned long	dim3948KuoInit[]		=	{	1	,	1	,	3	,	13	,	21	,	45	,	49	,	175	,	447	,	303	,	531	,	2923	,	3887	,	16273	,	5121	,	38965	};
            const unsigned long	dim3949KuoInit[]		=	{	1	,	1	,	7	,	11	,	5	,	21	,	53	,	201	,	1	,	207	,	181	,	1263	,	833	,	5435	,	28363	,	62913	};
            const unsigned long	dim3950KuoInit[]		=	{	1	,	1	,	3	,	5	,	11	,	31	,	1	,	63	,	343	,	221	,	23	,	1315	,	7639	,	9983	,	9167	,	63489	};
            const unsigned long	dim3951KuoInit[]		=	{	1	,	3	,	3	,	1	,	21	,	43	,	7	,	161	,	471	,	749	,	1547	,	1645	,	1557	,	9719	,	14017	,	43281	};
            const unsigned long	dim3952KuoInit[]		=	{	1	,	1	,	1	,	7	,	3	,	23	,	49	,	29	,	259	,	367	,	1929	,	3925	,	4197	,	8353	,	28995	,	41225	};
            const unsigned long	dim3953KuoInit[]		=	{	1	,	3	,	1	,	9	,	5	,	31	,	33	,	195	,	33	,	171	,	1021	,	127	,	5275	,	10751	,	12483	,	7097	};
            const unsigned long	dim3954KuoInit[]		=	{	1	,	3	,	5	,	3	,	19	,	21	,	3	,	113	,	217	,	953	,	419	,	1689	,	8011	,	805	,	20691	,	39159	};
            const unsigned long	dim3955KuoInit[]		=	{	1	,	1	,	5	,	1	,	27	,	35	,	83	,	173	,	299	,	687	,	493	,	3965	,	5963	,	1307	,	16379	,	34297	};
            const unsigned long	dim3956KuoInit[]		=	{	1	,	3	,	7	,	3	,	11	,	63	,	103	,	199	,	207	,	25	,	1471	,	2047	,	7271	,	7735	,	4313	,	45205	};
            const unsigned long	dim3957KuoInit[]		=	{	1	,	3	,	1	,	7	,	9	,	23	,	33	,	207	,	213	,	125	,	1871	,	2073	,	53	,	9953	,	1327	,	41943	};
            const unsigned long	dim3958KuoInit[]		=	{	1	,	3	,	7	,	3	,	19	,	55	,	3	,	159	,	147	,	649	,	1161	,	2245	,	4303	,	9971	,	29633	,	56535	};
            const unsigned long	dim3959KuoInit[]		=	{	1	,	3	,	3	,	5	,	1	,	1	,	57	,	101	,	265	,	859	,	137	,	763	,	6657	,	8659	,	21037	,	22757	};
            const unsigned long	dim3960KuoInit[]		=	{	1	,	1	,	1	,	7	,	11	,	23	,	39	,	129	,	49	,	987	,	2013	,	4071	,	1289	,	6471	,	6231	,	23987	};
            const unsigned long	dim3961KuoInit[]		=	{	1	,	3	,	7	,	3	,	29	,	7	,	87	,	253	,	111	,	55	,	1719	,	321	,	3579	,	4489	,	28323	,	35091	};
            const unsigned long	dim3962KuoInit[]		=	{	1	,	1	,	5	,	13	,	17	,	1	,	39	,	73	,	455	,	31	,	1215	,	2683	,	6193	,	15725	,	2737	,	24927	};
            const unsigned long	dim3963KuoInit[]		=	{	1	,	3	,	1	,	7	,	15	,	63	,	69	,	125	,	127	,	337	,	129	,	2667	,	4723	,	8489	,	6129	,	14973	};
            const unsigned long	dim3964KuoInit[]		=	{	1	,	3	,	1	,	9	,	21	,	37	,	109	,	91	,	499	,	767	,	1095	,	2731	,	6129	,	2167	,	21311	,	43503	};
            const unsigned long	dim3965KuoInit[]		=	{	1	,	3	,	7	,	7	,	29	,	11	,	87	,	101	,	19	,	699	,	1601	,	2341	,	3623	,	1799	,	2253	,	40705	};
            const unsigned long	dim3966KuoInit[]		=	{	1	,	1	,	7	,	1	,	27	,	25	,	111	,	91	,	49	,	3	,	13	,	3851	,	3735	,	13387	,	4501	,	64303	};
            const unsigned long	dim3967KuoInit[]		=	{	1	,	1	,	3	,	13	,	21	,	63	,	7	,	65	,	241	,	925	,	705	,	3745	,	827	,	7227	,	1869	,	60375	};
            const unsigned long	dim3968KuoInit[]		=	{	1	,	3	,	1	,	7	,	31	,	45	,	5	,	51	,	45	,	595	,	1069	,	3139	,	5843	,	2581	,	22573	,	53297	};
            const unsigned long	dim3969KuoInit[]		=	{	1	,	1	,	5	,	3	,	5	,	9	,	89	,	91	,	349	,	241	,	369	,	279	,	617	,	4479	,	12761	,	14473	};
            const unsigned long	dim3970KuoInit[]		=	{	1	,	3	,	1	,	1	,	13	,	45	,	91	,	29	,	219	,	341	,	1831	,	1993	,	3661	,	1855	,	7243	,	3477	};
            const unsigned long	dim3971KuoInit[]		=	{	1	,	3	,	3	,	13	,	15	,	17	,	27	,	123	,	223	,	127	,	111	,	2375	,	3919	,	7569	,	4419	,	24957	};
            const unsigned long	dim3972KuoInit[]		=	{	1	,	3	,	7	,	1	,	17	,	1	,	49	,	111	,	103	,	67	,	75	,	3109	,	1783	,	13291	,	5409	,	33123	};
            const unsigned long	dim3973KuoInit[]		=	{	1	,	3	,	5	,	3	,	1	,	51	,	107	,	89	,	233	,	51	,	745	,	2005	,	5485	,	7537	,	13353	,	34579	};
            const unsigned long	dim3974KuoInit[]		=	{	1	,	1	,	7	,	3	,	9	,	3	,	25	,	95	,	413	,	461	,	1597	,	767	,	3183	,	10285	,	10579	,	26581	};
            const unsigned long	dim3975KuoInit[]		=	{	1	,	1	,	5	,	7	,	25	,	3	,	27	,	93	,	433	,	59	,	701	,	1825	,	6375	,	6477	,	6743	,	42101	};
            const unsigned long	dim3976KuoInit[]		=	{	1	,	3	,	1	,	3	,	19	,	45	,	95	,	207	,	267	,	623	,	1523	,	1541	,	3955	,	4681	,	2365	,	16661	};
            const unsigned long	dim3977KuoInit[]		=	{	1	,	1	,	3	,	13	,	19	,	35	,	83	,	151	,	159	,	825	,	205	,	2215	,	4767	,	797	,	12821	,	14951	};
            const unsigned long	dim3978KuoInit[]		=	{	1	,	1	,	5	,	13	,	15	,	5	,	99	,	141	,	21	,	537	,	657	,	2055	,	1571	,	12725	,	4227	,	21061	};
            const unsigned long	dim3979KuoInit[]		=	{	1	,	1	,	1	,	13	,	1	,	39	,	85	,	229	,	9	,	721	,	1117	,	519	,	1763	,	13211	,	32533	,	41393	};
            const unsigned long	dim3980KuoInit[]		=	{	1	,	1	,	3	,	15	,	23	,	11	,	125	,	59	,	439	,	887	,	1273	,	3707	,	7737	,	4115	,	7013	,	47453	};
            const unsigned long	dim3981KuoInit[]		=	{	1	,	1	,	1	,	5	,	9	,	25	,	53	,	203	,	9	,	189	,	471	,	637	,	5983	,	1693	,	8425	,	52181	};
            const unsigned long	dim3982KuoInit[]		=	{	1	,	1	,	1	,	1	,	29	,	61	,	33	,	87	,	153	,	625	,	539	,	3595	,	3161	,	3929	,	22825	,	50113	};
            const unsigned long	dim3983KuoInit[]		=	{	1	,	3	,	7	,	7	,	1	,	31	,	105	,	169	,	475	,	631	,	1511	,	3687	,	5321	,	3859	,	19329	,	9571	};
            const unsigned long	dim3984KuoInit[]		=	{	1	,	3	,	7	,	1	,	9	,	13	,	111	,	79	,	245	,	351	,	69	,	59	,	5561	,	4713	,	22789	,	14929	};
            const unsigned long	dim3985KuoInit[]		=	{	1	,	1	,	3	,	1	,	1	,	55	,	87	,	147	,	37	,	587	,	479	,	3175	,	5437	,	12267	,	7057	,	57899	};
            const unsigned long	dim3986KuoInit[]		=	{	1	,	1	,	5	,	11	,	15	,	55	,	87	,	53	,	359	,	491	,	343	,	621	,	1055	,	8697	,	28993	,	20473	};
            const unsigned long	dim3987KuoInit[]		=	{	1	,	3	,	3	,	15	,	13	,	11	,	63	,	179	,	293	,	639	,	809	,	1365	,	7043	,	13269	,	5423	,	36465	};
            const unsigned long	dim3988KuoInit[]		=	{	1	,	1	,	5	,	11	,	31	,	49	,	27	,	71	,	187	,	377	,	283	,	2407	,	2987	,	8565	,	15159	,	44501	};
            const unsigned long	dim3989KuoInit[]		=	{	1	,	3	,	1	,	1	,	27	,	5	,	79	,	211	,	315	,	645	,	951	,	4025	,	1647	,	2893	,	23829	,	23941	};
            const unsigned long	dim3990KuoInit[]		=	{	1	,	1	,	7	,	15	,	15	,	29	,	81	,	133	,	345	,	573	,	1605	,	2693	,	5293	,	13579	,	21793	,	30981	};
            const unsigned long	dim3991KuoInit[]		=	{	1	,	3	,	7	,	5	,	31	,	19	,	109	,	141	,	389	,	473	,	1359	,	1695	,	6761	,	3701	,	27583	,	47907	};
            const unsigned long	dim3992KuoInit[]		=	{	1	,	1	,	3	,	5	,	7	,	43	,	51	,	139	,	81	,	111	,	417	,	2627	,	2297	,	12493	,	24401	,	42525	};
            const unsigned long	dim3993KuoInit[]		=	{	1	,	3	,	7	,	3	,	21	,	9	,	65	,	141	,	119	,	971	,	589	,	2635	,	6365	,	1925	,	15853	,	47123	};
            const unsigned long	dim3994KuoInit[]		=	{	1	,	1	,	3	,	1	,	25	,	15	,	93	,	151	,	443	,	325	,	457	,	3779	,	7669	,	2063	,	20005	,	6513	};
            const unsigned long	dim3995KuoInit[]		=	{	1	,	1	,	5	,	11	,	7	,	13	,	127	,	55	,	183	,	663	,	437	,	3475	,	6391	,	8423	,	8473	,	41971	};
            const unsigned long	dim3996KuoInit[]		=	{	1	,	1	,	5	,	9	,	7	,	15	,	113	,	225	,	259	,	85	,	945	,	645	,	5129	,	3657	,	931	,	30429	};
            const unsigned long	dim3997KuoInit[]		=	{	1	,	3	,	3	,	15	,	9	,	63	,	23	,	117	,	205	,	295	,	1325	,	1349	,	5521	,	9399	,	7907	,	26373	};
            const unsigned long	dim3998KuoInit[]		=	{	1	,	3	,	7	,	5	,	17	,	23	,	47	,	75	,	97	,	35	,	1363	,	793	,	2811	,	3491	,	18173	,	8353	};
            const unsigned long	dim3999KuoInit[]		=	{	1	,	3	,	3	,	11	,	25	,	27	,	107	,	9	,	331	,	1017	,	615	,	1123	,	1303	,	5705	,	14013	,	39207	};
            const unsigned long	dim4000KuoInit[]		=	{	1	,	3	,	1	,	7	,	3	,	17	,	101	,	17	,	461	,	781	,	345	,	2687	,	5867	,	1367	,	17245	,	18929	};
            const unsigned long	dim4001KuoInit[]		=	{	1	,	1	,	1	,	13	,	3	,	53	,	55	,	37	,	257	,	101	,	195	,	1193	,	5419	,	4243	,	6079	,	32461	};
            const unsigned long	dim4002KuoInit[]		=	{	1	,	1	,	1	,	13	,	29	,	21	,	51	,	123	,	145	,	105	,	639	,	2389	,	6985	,	2519	,	12351	,	45859	};
            const unsigned long	dim4003KuoInit[]		=	{	1	,	1	,	3	,	13	,	31	,	17	,	75	,	121	,	11	,	945	,	1917	,	601	,	3765	,	14215	,	19075	,	26317	};
            const unsigned long	dim4004KuoInit[]		=	{	1	,	3	,	7	,	13	,	21	,	3	,	57	,	17	,	97	,	567	,	839	,	135	,	2441	,	4613	,	10511	,	21927	};
            const unsigned long	dim4005KuoInit[]		=	{	1	,	3	,	5	,	1	,	21	,	27	,	1	,	109	,	381	,	241	,	1701	,	1255	,	1469	,	8415	,	18789	,	1769	};
            const unsigned long	dim4006KuoInit[]		=	{	1	,	3	,	1	,	11	,	11	,	45	,	77	,	209	,	55	,	371	,	1323	,	2899	,	7387	,	10317	,	32695	,	52199	};
            const unsigned long	dim4007KuoInit[]		=	{	1	,	3	,	5	,	9	,	15	,	59	,	45	,	97	,	369	,	247	,	1621	,	2037	,	4569	,	16043	,	2225	,	39055	};
            const unsigned long	dim4008KuoInit[]		=	{	1	,	1	,	7	,	1	,	21	,	43	,	85	,	195	,	99	,	85	,	1759	,	547	,	4371	,	14467	,	17551	,	52185	};
            const unsigned long	dim4009KuoInit[]		=	{	1	,	1	,	3	,	1	,	17	,	53	,	79	,	203	,	49	,	289	,	1547	,	1173	,	1445	,	11593	,	29927	,	63479	};
            const unsigned long	dim4010KuoInit[]		=	{	1	,	3	,	3	,	5	,	3	,	61	,	31	,	71	,	321	,	923	,	1337	,	3465	,	3305	,	10841	,	31921	,	3057	};
            const unsigned long	dim4011KuoInit[]		=	{	1	,	3	,	5	,	11	,	31	,	1	,	39	,	201	,	57	,	913	,	1953	,	193	,	4185	,	5895	,	27027	,	10889	};
            const unsigned long	dim4012KuoInit[]		=	{	1	,	3	,	1	,	9	,	7	,	9	,	89	,	145	,	445	,	643	,	279	,	1983	,	491	,	11825	,	1857	,	11201	};
            const unsigned long	dim4013KuoInit[]		=	{	1	,	3	,	1	,	5	,	9	,	39	,	21	,	245	,	403	,	393	,	779	,	1419	,	967	,	14309	,	6789	,	54477	};
            const unsigned long	dim4014KuoInit[]		=	{	1	,	3	,	5	,	7	,	31	,	21	,	65	,	211	,	447	,	263	,	665	,	3311	,	405	,	1775	,	31121	,	54025	};
            const unsigned long	dim4015KuoInit[]		=	{	1	,	3	,	5	,	11	,	13	,	9	,	95	,	243	,	139	,	733	,	813	,	3855	,	2731	,	11651	,	2175	,	9655	};
            const unsigned long	dim4016KuoInit[]		=	{	1	,	1	,	1	,	1	,	21	,	5	,	5	,	241	,	415	,	241	,	1223	,	3237	,	2265	,	3761	,	26883	,	37875	};
            const unsigned long	dim4017KuoInit[]		=	{	1	,	1	,	1	,	13	,	25	,	45	,	97	,	73	,	353	,	393	,	1217	,	563	,	5813	,	2791	,	16477	,	4401	};
            const unsigned long	dim4018KuoInit[]		=	{	1	,	3	,	5	,	13	,	17	,	61	,	1	,	247	,	439	,	1013	,	501	,	881	,	353	,	4689	,	18733	,	46731	};
            const unsigned long	dim4019KuoInit[]		=	{	1	,	3	,	1	,	11	,	11	,	29	,	87	,	53	,	461	,	751	,	1503	,	2851	,	2891	,	15231	,	1513	,	12573	};
            const unsigned long	dim4020KuoInit[]		=	{	1	,	3	,	1	,	3	,	1	,	13	,	95	,	239	,	151	,	159	,	55	,	1203	,	1331	,	10429	,	19159	,	18431	};
            const unsigned long	dim4021KuoInit[]		=	{	1	,	3	,	3	,	15	,	11	,	43	,	81	,	133	,	449	,	523	,	667	,	2759	,	6829	,	15129	,	19829	,	31745	};
            const unsigned long	dim4022KuoInit[]		=	{	1	,	1	,	1	,	13	,	25	,	43	,	115	,	171	,	475	,	553	,	1615	,	1421	,	2363	,	13091	,	27505	,	21247	};
            const unsigned long	dim4023KuoInit[]		=	{	1	,	3	,	5	,	13	,	1	,	23	,	15	,	25	,	205	,	797	,	1013	,	1903	,	1595	,	16373	,	29983	,	23009	};
            const unsigned long	dim4024KuoInit[]		=	{	1	,	1	,	1	,	11	,	3	,	27	,	79	,	179	,	147	,	905	,	1893	,	2459	,	3063	,	7447	,	16293	,	14331	};
            const unsigned long	dim4025KuoInit[]		=	{	1	,	3	,	7	,	1	,	19	,	49	,	41	,	19	,	97	,	559	,	1213	,	847	,	3715	,	13963	,	20273	,	25565	};
            const unsigned long	dim4026KuoInit[]		=	{	1	,	3	,	7	,	3	,	27	,	21	,	75	,	1	,	157	,	715	,	41	,	2383	,	4609	,	9875	,	21509	,	57199	};
            const unsigned long	dim4027KuoInit[]		=	{	1	,	1	,	1	,	9	,	5	,	51	,	93	,	115	,	495	,	687	,	1805	,	1369	,	1525	,	15841	,	8897	,	21023	};
            const unsigned long	dim4028KuoInit[]		=	{	1	,	1	,	3	,	11	,	21	,	41	,	93	,	107	,	391	,	567	,	223	,	2331	,	6009	,	9405	,	13137	,	54727	};
            const unsigned long	dim4029KuoInit[]		=	{	1	,	3	,	1	,	15	,	5	,	59	,	119	,	165	,	485	,	765	,	243	,	2589	,	4631	,	4045	,	26867	,	55329	};
            const unsigned long	dim4030KuoInit[]		=	{	1	,	1	,	7	,	9	,	17	,	45	,	35	,	219	,	137	,	315	,	293	,	1137	,	6675	,	10693	,	17455	,	35603	};
            const unsigned long	dim4031KuoInit[]		=	{	1	,	3	,	7	,	13	,	11	,	43	,	65	,	205	,	351	,	155	,	1371	,	3069	,	1345	,	859	,	21385	,	25923	};
            const unsigned long	dim4032KuoInit[]		=	{	1	,	1	,	3	,	3	,	11	,	21	,	39	,	195	,	349	,	333	,	687	,	1155	,	1491	,	10453	,	17615	,	64801	};
            const unsigned long	dim4033KuoInit[]		=	{	1	,	3	,	1	,	9	,	17	,	45	,	39	,	137	,	25	,	105	,	441	,	2437	,	7715	,	8501	,	3999	,	15913	};
            const unsigned long	dim4034KuoInit[]		=	{	1	,	1	,	1	,	9	,	31	,	21	,	93	,	31	,	63	,	635	,	331	,	149	,	1683	,	6183	,	19333	,	34947	};
            const unsigned long	dim4035KuoInit[]		=	{	1	,	3	,	3	,	5	,	23	,	11	,	15	,	239	,	365	,	983	,	389	,	945	,	1765	,	6417	,	4471	,	41431	};
            const unsigned long	dim4036KuoInit[]		=	{	1	,	3	,	1	,	5	,	5	,	49	,	49	,	27	,	417	,	605	,	1495	,	4021	,	1775	,	3691	,	21803	,	60999	};
            const unsigned long	dim4037KuoInit[]		=	{	1	,	3	,	3	,	5	,	23	,	21	,	33	,	205	,	481	,	105	,	359	,	1407	,	4007	,	35	,	10789	,	27721	};
            const unsigned long	dim4038KuoInit[]		=	{	1	,	1	,	5	,	3	,	5	,	33	,	51	,	61	,	29	,	337	,	1523	,	955	,	4923	,	1977	,	16505	,	21249	};
            const unsigned long	dim4039KuoInit[]		=	{	1	,	3	,	1	,	11	,	17	,	19	,	39	,	7	,	443	,	557	,	611	,	4037	,	5873	,	15161	,	449	,	41747	};
            const unsigned long	dim4040KuoInit[]		=	{	1	,	1	,	7	,	11	,	5	,	53	,	91	,	227	,	217	,	689	,	7	,	775	,	8139	,	10997	,	29539	,	60185	};
            const unsigned long	dim4041KuoInit[]		=	{	1	,	1	,	5	,	1	,	7	,	59	,	117	,	101	,	145	,	779	,	1359	,	3375	,	2493	,	13075	,	11475	,	49507	};
            const unsigned long	dim4042KuoInit[]		=	{	1	,	1	,	1	,	3	,	13	,	31	,	39	,	219	,	181	,	203	,	19	,	659	,	533	,	2541	,	7285	,	29353	};
            const unsigned long	dim4043KuoInit[]		=	{	1	,	3	,	3	,	1	,	29	,	33	,	1	,	125	,	283	,	61	,	303	,	1613	,	4309	,	1891	,	7107	,	25977	};
            const unsigned long	dim4044KuoInit[]		=	{	1	,	3	,	5	,	9	,	21	,	59	,	41	,	63	,	123	,	715	,	893	,	3509	,	6785	,	11009	,	17749	,	18349	};
            const unsigned long	dim4045KuoInit[]		=	{	1	,	3	,	1	,	1	,	25	,	23	,	15	,	1	,	147	,	709	,	957	,	1101	,	2733	,	14069	,	18905	,	12467	};
            const unsigned long	dim4046KuoInit[]		=	{	1	,	3	,	3	,	13	,	9	,	63	,	109	,	39	,	123	,	919	,	427	,	435	,	5241	,	14169	,	31179	,	15887	};
            const unsigned long	dim4047KuoInit[]		=	{	1	,	1	,	7	,	15	,	13	,	25	,	29	,	187	,	433	,	633	,	1203	,	113	,	4577	,	13349	,	18407	,	62825	};
            const unsigned long	dim4048KuoInit[]		=	{	1	,	1	,	5	,	1	,	15	,	33	,	53	,	209	,	415	,	711	,	1543	,	1529	,	4991	,	9545	,	19655	,	40427	};
            const unsigned long	dim4049KuoInit[]		=	{	1	,	1	,	5	,	5	,	23	,	59	,	27	,	23	,	7	,	841	,	1715	,	43	,	4987	,	2073	,	13765	,	36105	};
            const unsigned long	dim4050KuoInit[]		=	{	1	,	3	,	3	,	15	,	31	,	5	,	51	,	183	,	319	,	443	,	621	,	349	,	2835	,	10847	,	19059	,	51689	};
            const unsigned long	dim4051KuoInit[]		=	{	1	,	3	,	1	,	9	,	5	,	3	,	21	,	81	,	357	,	609	,	2019	,	2613	,	333	,	5699	,	2485	,	52105	};
            const unsigned long	dim4052KuoInit[]		=	{	1	,	1	,	3	,	15	,	3	,	11	,	73	,	121	,	263	,	833	,	65	,	347	,	4821	,	4631	,	29929	,	42157	};
            const unsigned long	dim4053KuoInit[]		=	{	1	,	1	,	7	,	3	,	1	,	63	,	7	,	191	,	47	,	405	,	43	,	1499	,	5539	,	12607	,	26095	,	20759	};
            const unsigned long	dim4054KuoInit[]		=	{	1	,	3	,	3	,	5	,	15	,	21	,	109	,	3	,	15	,	57	,	1669	,	4033	,	5251	,	4465	,	28057	,	64943	};
            const unsigned long	dim4055KuoInit[]		=	{	1	,	1	,	3	,	9	,	7	,	61	,	31	,	17	,	331	,	671	,	1955	,	1035	,	7183	,	6559	,	19299	,	10225	};
            const unsigned long	dim4056KuoInit[]		=	{	1	,	1	,	5	,	11	,	31	,	15	,	97	,	3	,	361	,	541	,	323	,	1463	,	6867	,	12717	,	9593	,	15407	};
            const unsigned long	dim4057KuoInit[]		=	{	1	,	1	,	1	,	5	,	29	,	39	,	71	,	27	,	305	,	589	,	1441	,	2349	,	185	,	447	,	24223	,	53591	};
            const unsigned long	dim4058KuoInit[]		=	{	1	,	1	,	7	,	15	,	15	,	59	,	81	,	77	,	371	,	891	,	31	,	457	,	3073	,	8427	,	8225	,	53157	};
            const unsigned long	dim4059KuoInit[]		=	{	1	,	3	,	3	,	11	,	23	,	61	,	71	,	85	,	39	,	77	,	1583	,	933	,	6147	,	2271	,	24761	,	15835	};
            const unsigned long	dim4060KuoInit[]		=	{	1	,	3	,	7	,	11	,	25	,	55	,	41	,	131	,	275	,	603	,	43	,	761	,	5071	,	14149	,	4001	,	6545	};
            const unsigned long	dim4061KuoInit[]		=	{	1	,	1	,	5	,	3	,	5	,	1	,	25	,	217	,	445	,	197	,	863	,	3557	,	585	,	13599	,	12329	,	17331	};
            const unsigned long	dim4062KuoInit[]		=	{	1	,	3	,	3	,	13	,	9	,	21	,	21	,	89	,	121	,	1017	,	1559	,	83	,	5225	,	13247	,	12003	,	10049	};
            const unsigned long	dim4063KuoInit[]		=	{	1	,	3	,	3	,	9	,	31	,	15	,	71	,	45	,	161	,	895	,	1431	,	1277	,	2547	,	1943	,	17631	,	59133	};
            const unsigned long	dim4064KuoInit[]		=	{	1	,	1	,	1	,	9	,	11	,	17	,	29	,	157	,	355	,	89	,	401	,	983	,	8175	,	11687	,	20087	,	7541	};
            const unsigned long	dim4065KuoInit[]		=	{	1	,	3	,	7	,	5	,	15	,	45	,	23	,	141	,	365	,	593	,	1137	,	873	,	2903	,	8861	,	25805	,	22601	};
            const unsigned long	dim4066KuoInit[]		=	{	1	,	1	,	3	,	11	,	27	,	17	,	97	,	3	,	359	,	531	,	329	,	437	,	3483	,	8653	,	15953	,	48333	};
            const unsigned long	dim4067KuoInit[]		=	{	1	,	3	,	5	,	3	,	9	,	43	,	79	,	223	,	149	,	177	,	1075	,	545	,	7235	,	4271	,	20153	,	65481	};
            const unsigned long	dim4068KuoInit[]		=	{	1	,	3	,	1	,	13	,	5	,	55	,	29	,	51	,	497	,	677	,	167	,	391	,	1371	,	8753	,	23771	,	48167	};
            const unsigned long	dim4069KuoInit[]		=	{	1	,	1	,	5	,	3	,	7	,	17	,	7	,	227	,	21	,	521	,	221	,	3529	,	5641	,	4139	,	14839	,	49863	};
            const unsigned long	dim4070KuoInit[]		=	{	1	,	1	,	3	,	9	,	7	,	41	,	17	,	151	,	401	,	407	,	689	,	507	,	6683	,	12515	,	26895	,	60863	};
            const unsigned long	dim4071KuoInit[]		=	{	1	,	1	,	3	,	7	,	3	,	53	,	27	,	185	,	119	,	891	,	2025	,	607	,	6961	,	4263	,	7795	,	34157	};
            const unsigned long	dim4072KuoInit[]		=	{	1	,	1	,	3	,	3	,	25	,	19	,	15	,	131	,	323	,	27	,	1749	,	2429	,	2207	,	9207	,	18333	,	25857	};
            const unsigned long	dim4073KuoInit[]		=	{	1	,	1	,	5	,	5	,	21	,	57	,	61	,	93	,	305	,	497	,	1465	,	3639	,	7695	,	3903	,	31201	,	45699	};
            const unsigned long	dim4074KuoInit[]		=	{	1	,	1	,	5	,	11	,	25	,	35	,	93	,	55	,	415	,	397	,	1375	,	809	,	6205	,	9761	,	23129	,	28757	};
            const unsigned long	dim4075KuoInit[]		=	{	1	,	3	,	5	,	9	,	13	,	21	,	1	,	167	,	65	,	689	,	525	,	2839	,	4069	,	10123	,	24403	,	4121	};
            const unsigned long	dim4076KuoInit[]		=	{	1	,	1	,	3	,	13	,	11	,	21	,	67	,	159	,	131	,	579	,	1397	,	251	,	3933	,	14737	,	22047	,	4173	};
            const unsigned long	dim4077KuoInit[]		=	{	1	,	1	,	3	,	13	,	9	,	47	,	71	,	81	,	141	,	607	,	1879	,	3057	,	6493	,	1177	,	27261	,	59313	};
            const unsigned long	dim4078KuoInit[]		=	{	1	,	1	,	3	,	11	,	25	,	45	,	35	,	193	,	251	,	223	,	1725	,	1533	,	5799	,	12417	,	29765	,	50203	};
            const unsigned long	dim4079KuoInit[]		=	{	1	,	3	,	1	,	13	,	27	,	1	,	41	,	171	,	293	,	65	,	607	,	2843	,	2955	,	8783	,	21249	,	41695	};
            const unsigned long	dim4080KuoInit[]		=	{	1	,	3	,	1	,	3	,	3	,	57	,	39	,	31	,	507	,	687	,	2009	,	2881	,	4595	,	9275	,	9773	,	31489	};
            const unsigned long	dim4081KuoInit[]		=	{	1	,	3	,	1	,	7	,	17	,	11	,	47	,	233	,	193	,	395	,	1879	,	1983	,	4811	,	1221	,	27279	,	52089	};
            const unsigned long	dim4082KuoInit[]		=	{	1	,	3	,	1	,	1	,	13	,	57	,	51	,	253	,	381	,	779	,	897	,	2893	,	4373	,	6599	,	7533	,	30359	};
            const unsigned long	dim4083KuoInit[]		=	{	1	,	3	,	3	,	3	,	5	,	35	,	1	,	117	,	81	,	35	,	2005	,	2133	,	4493	,	9773	,	24265	,	45945	};
            const unsigned long	dim4084KuoInit[]		=	{	1	,	1	,	3	,	1	,	21	,	57	,	67	,	157	,	111	,	857	,	899	,	2081	,	2097	,	16005	,	20535	,	2955	};
            const unsigned long	dim4085KuoInit[]		=	{	1	,	1	,	1	,	11	,	19	,	45	,	109	,	83	,	375	,	1003	,	79	,	3801	,	3433	,	4429	,	32385	,	31551	};
            const unsigned long	dim4086KuoInit[]		=	{	1	,	1	,	3	,	3	,	1	,	15	,	115	,	47	,	511	,	533	,	1711	,	2433	,	3027	,	13511	,	23743	,	23805	};
            const unsigned long	dim4087KuoInit[]		=	{	1	,	1	,	7	,	15	,	31	,	11	,	107	,	215	,	431	,	935	,	1033	,	213	,	7913	,	557	,	22063	,	7947	};
            const unsigned long	dim4088KuoInit[]		=	{	1	,	3	,	3	,	1	,	23	,	25	,	109	,	183	,	445	,	477	,	1297	,	2269	,	3593	,	5657	,	995	,	19821	};
            const unsigned long	dim4089KuoInit[]		=	{	1	,	3	,	3	,	9	,	5	,	19	,	93	,	137	,	155	,	5	,	1905	,	1551	,	6321	,	11213	,	6305	,	5063	};
            const unsigned long	dim4090KuoInit[]		=	{	1	,	1	,	3	,	15	,	9	,	5	,	123	,	203	,	379	,	159	,	1871	,	2377	,	665	,	4877	,	17663	,	60371	};
            const unsigned long	dim4091KuoInit[]		=	{	1	,	1	,	1	,	15	,	3	,	7	,	109	,	15	,	109	,	117	,	79	,	3061	,	667	,	6963	,	30565	,	25745	};
            const unsigned long	dim4092KuoInit[]		=	{	1	,	3	,	5	,	1	,	15	,	7	,	105	,	139	,	29	,	923	,	1449	,	1349	,	2951	,	7443	,	10243	,	51739	};
            const unsigned long	dim4093KuoInit[]		=	{	1	,	1	,	7	,	5	,	5	,	47	,	91	,	77	,	437	,	483	,	1309	,	1559	,	6491	,	13469	,	9461	,	40993	};
            const unsigned long	dim4094KuoInit[]		=	{	1	,	1	,	7	,	5	,	5	,	35	,	57	,	139	,	207	,	267	,	707	,	1887	,	3447	,	3959	,	8169	,	14511	};
            const unsigned long	dim4095KuoInit[]		=	{	1	,	1	,	7	,	15	,	23	,	9	,	57	,	183	,	437	,	595	,	1317	,	3847	,	2515	,	13415	,	26651	,	41885	};
            const unsigned long	dim4096KuoInit[]		=	{	1	,	3	,	3	,	3	,	21	,	49	,	71	,	129	,	15	,	183	,	315	,	57	,	1739	,	15119	,	15293	,	8489	};
            const unsigned long	dim4097KuoInit[]		=	{	1	,	3	,	1	,	3	,	7	,	63	,	91	,	241	,	355	,	921	,	1827	,	1	,	5713	,	3233	,	21901	,	48009	};
            const unsigned long	dim4098KuoInit[]		=	{	1	,	3	,	7	,	13	,	31	,	9	,	103	,	209	,	289	,	575	,	1543	,	653	,	7189	,	1843	,	5205	,	27325	};
            const unsigned long	dim4099KuoInit[]		=	{	1	,	1	,	7	,	7	,	15	,	63	,	7	,	71	,	95	,	861	,	857	,	701	,	6843	,	10609	,	11177	,	21043	};
            const unsigned long	dim4100KuoInit[]		=	{	1	,	3	,	3	,	9	,	25	,	31	,	85	,	255	,	247	,	773	,	1503	,	2379	,	7231	,	6013	,	16193	,	33855	};
            const unsigned long	dim4101KuoInit[]		=	{	1	,	1	,	3	,	11	,	5	,	27	,	125	,	159	,	381	,	665	,	1059	,	2527	,	1423	,	3055	,	8363	,	57053	};
            const unsigned long	dim4102KuoInit[]		=	{	1	,	1	,	5	,	3	,	25	,	57	,	107	,	89	,	499	,	761	,	909	,	1487	,	1569	,	9353	,	27477	,	39373	};
            const unsigned long	dim4103KuoInit[]		=	{	1	,	3	,	5	,	9	,	1	,	57	,	81	,	27	,	41	,	371	,	1111	,	2437	,	3669	,	6033	,	28601	,	38985	};
            const unsigned long	dim4104KuoInit[]		=	{	1	,	3	,	5	,	5	,	25	,	63	,	85	,	171	,	187	,	387	,	1971	,	579	,	2779	,	7667	,	25423	,	61005	};
            const unsigned long	dim4105KuoInit[]		=	{	1	,	1	,	5	,	1	,	25	,	37	,	87	,	93	,	387	,	95	,	65	,	3345	,	6581	,	13325	,	9683	,	25399	};
            const unsigned long	dim4106KuoInit[]		=	{	1	,	3	,	5	,	11	,	9	,	33	,	81	,	69	,	483	,	79	,	123	,	3241	,	2599	,	7545	,	17775	,	34933	};
            const unsigned long	dim4107KuoInit[]		=	{	1	,	1	,	7	,	1	,	9	,	41	,	27	,	191	,	385	,	439	,	347	,	1279	,	4491	,	8883	,	31599	,	27167	};
            const unsigned long	dim4108KuoInit[]		=	{	1	,	3	,	5	,	13	,	1	,	57	,	51	,	81	,	161	,	15	,	7	,	3695	,	1777	,	14947	,	27853	,	22383	};
            const unsigned long	dim4109KuoInit[]		=	{	1	,	1	,	5	,	5	,	15	,	21	,	79	,	159	,	85	,	677	,	401	,	665	,	3547	,	13659	,	18035	,	38181	};
            const unsigned long	dim4110KuoInit[]		=	{	1	,	3	,	3	,	15	,	11	,	55	,	29	,	219	,	45	,	683	,	673	,	2653	,	6841	,	7809	,	8969	,	17773	};
            const unsigned long	dim4111KuoInit[]		=	{	1	,	3	,	5	,	13	,	25	,	61	,	19	,	67	,	469	,	265	,	1835	,	2733	,	4537	,	8113	,	27089	,	59191	};
            const unsigned long	dim4112KuoInit[]		=	{	1	,	1	,	5	,	11	,	9	,	37	,	99	,	31	,	229	,	635	,	1615	,	3073	,	2161	,	13917	,	22197	,	4331	};
            const unsigned long	dim4113KuoInit[]		=	{	1	,	3	,	1	,	11	,	13	,	59	,	119	,	65	,	55	,	113	,	189	,	57	,	903	,	11119	,	3835	,	30549	};
            const unsigned long	dim4114KuoInit[]		=	{	1	,	1	,	1	,	5	,	23	,	25	,	29	,	167	,	199	,	669	,	691	,	3047	,	7433	,	15043	,	22471	,	39473	};
            const unsigned long	dim4115KuoInit[]		=	{	1	,	1	,	5	,	13	,	13	,	15	,	121	,	127	,	343	,	293	,	511	,	1873	,	4535	,	6395	,	10161	,	52113	};
            const unsigned long	dim4116KuoInit[]		=	{	1	,	3	,	1	,	11	,	13	,	7	,	95	,	151	,	187	,	221	,	843	,	2433	,	5083	,	15319	,	5763	,	46801	};
            const unsigned long	dim4117KuoInit[]		=	{	1	,	3	,	5	,	3	,	3	,	57	,	109	,	65	,	203	,	583	,	209	,	2395	,	5615	,	14949	,	10819	,	5109	};
            const unsigned long	dim4118KuoInit[]		=	{	1	,	3	,	7	,	15	,	23	,	41	,	65	,	17	,	351	,	89	,	1451	,	219	,	5863	,	15197	,	22741	,	16599	};
            const unsigned long	dim4119KuoInit[]		=	{	1	,	1	,	3	,	13	,	23	,	49	,	51	,	9	,	365	,	595	,	1619	,	933	,	2231	,	15219	,	105	,	1657	};
            const unsigned long	dim4120KuoInit[]		=	{	1	,	3	,	5	,	7	,	27	,	33	,	39	,	243	,	257	,	299	,	479	,	1999	,	1781	,	9267	,	3631	,	21449	};
            const unsigned long	dim4121KuoInit[]		=	{	1	,	3	,	1	,	3	,	13	,	1	,	83	,	9	,	373	,	99	,	1873	,	3959	,	1009	,	31	,	29329	,	1489	};
            const unsigned long	dim4122KuoInit[]		=	{	1	,	1	,	7	,	3	,	15	,	21	,	99	,	25	,	265	,	461	,	467	,	2475	,	8149	,	5685	,	22679	,	9787	};
            const unsigned long	dim4123KuoInit[]		=	{	1	,	1	,	3	,	1	,	5	,	43	,	109	,	55	,	451	,	11	,	1631	,	999	,	1181	,	3377	,	4807	,	14675	};
            const unsigned long	dim4124KuoInit[]		=	{	1	,	3	,	7	,	11	,	5	,	43	,	27	,	253	,	73	,	697	,	81	,	3739	,	4267	,	3801	,	2913	,	33117	};
            const unsigned long	dim4125KuoInit[]		=	{	1	,	3	,	1	,	5	,	11	,	55	,	97	,	3	,	3	,	949	,	155	,	3575	,	941	,	3727	,	4047	,	26071	};
            const unsigned long	dim4126KuoInit[]		=	{	1	,	1	,	5	,	3	,	17	,	35	,	3	,	9	,	185	,	1	,	421	,	2831	,	957	,	4505	,	6421	,	16279	};
            const unsigned long	dim4127KuoInit[]		=	{	1	,	1	,	3	,	9	,	21	,	43	,	115	,	183	,	231	,	13	,	1711	,	3403	,	5447	,	2761	,	27591	,	431	};
            const unsigned long	dim4128KuoInit[]		=	{	1	,	3	,	3	,	15	,	1	,	25	,	111	,	157	,	429	,	981	,	1117	,	2767	,	3107	,	2203	,	15335	,	62597	};
            const unsigned long	dim4129KuoInit[]		=	{	1	,	1	,	1	,	9	,	5	,	11	,	59	,	163	,	37	,	101	,	63	,	2829	,	5583	,	12695	,	21683	,	62563	};
            const unsigned long	dim4130KuoInit[]		=	{	1	,	3	,	3	,	15	,	7	,	29	,	39	,	241	,	333	,	321	,	931	,	3811	,	1981	,	229	,	18877	,	6597	};
            const unsigned long	dim4131KuoInit[]		=	{	1	,	3	,	3	,	13	,	31	,	3	,	83	,	21	,	253	,	117	,	757	,	2335	,	291	,	3723	,	24961	,	41985	};
            const unsigned long	dim4132KuoInit[]		=	{	1	,	1	,	1	,	1	,	23	,	37	,	71	,	227	,	211	,	671	,	757	,	597	,	4015	,	7297	,	26275	,	46051	};
            const unsigned long	dim4133KuoInit[]		=	{	1	,	3	,	3	,	11	,	9	,	15	,	23	,	143	,	207	,	235	,	1215	,	1401	,	7487	,	13397	,	30443	,	43735	};
            const unsigned long	dim4134KuoInit[]		=	{	1	,	1	,	7	,	7	,	23	,	5	,	57	,	169	,	3	,	609	,	1077	,	109	,	6029	,	14891	,	6641	,	26525	};
            const unsigned long	dim4135KuoInit[]		=	{	1	,	3	,	7	,	13	,	7	,	45	,	99	,	187	,	43	,	751	,	1189	,	1787	,	4903	,	10605	,	15799	,	61769	};
            const unsigned long	dim4136KuoInit[]		=	{	1	,	3	,	1	,	5	,	31	,	51	,	95	,	127	,	149	,	557	,	1027	,	3377	,	1461	,	15217	,	16955	,	27339	};
            const unsigned long	dim4137KuoInit[]		=	{	1	,	3	,	5	,	11	,	31	,	29	,	11	,	143	,	47	,	709	,	1413	,	343	,	5421	,	3567	,	5383	,	62347	};
            const unsigned long	dim4138KuoInit[]		=	{	1	,	3	,	3	,	11	,	13	,	3	,	33	,	19	,	79	,	751	,	1979	,	785	,	6339	,	4133	,	31633	,	1963	};
            const unsigned long	dim4139KuoInit[]		=	{	1	,	3	,	3	,	9	,	15	,	21	,	117	,	13	,	105	,	519	,	1575	,	3041	,	5465	,	7167	,	17443	,	15781	};
            const unsigned long	dim4140KuoInit[]		=	{	1	,	1	,	1	,	7	,	29	,	25	,	51	,	237	,	11	,	431	,	1597	,	751	,	4115	,	12345	,	9451	,	1417	};
            const unsigned long	dim4141KuoInit[]		=	{	1	,	3	,	5	,	13	,	1	,	43	,	61	,	141	,	279	,	541	,	149	,	3783	,	1503	,	7597	,	25469	,	21143	};
            const unsigned long	dim4142KuoInit[]		=	{	1	,	1	,	3	,	13	,	23	,	13	,	21	,	53	,	49	,	491	,	135	,	3079	,	2593	,	10611	,	24467	,	18627	};
            const unsigned long	dim4143KuoInit[]		=	{	1	,	3	,	3	,	5	,	19	,	31	,	59	,	139	,	137	,	839	,	799	,	1081	,	251	,	2685	,	15993	,	15909	};
            const unsigned long	dim4144KuoInit[]		=	{	1	,	3	,	5	,	1	,	3	,	1	,	33	,	131	,	259	,	137	,	651	,	3879	,	3089	,	6957	,	24589	,	8945	};
            const unsigned long	dim4145KuoInit[]		=	{	1	,	1	,	3	,	1	,	23	,	13	,	127	,	255	,	325	,	451	,	275	,	1425	,	3649	,	8149	,	28913	,	16481	};
            const unsigned long	dim4146KuoInit[]		=	{	1	,	1	,	5	,	3	,	19	,	35	,	55	,	183	,	383	,	419	,	551	,	961	,	5413	,	3779	,	10935	,	30523	};
            const unsigned long	dim4147KuoInit[]		=	{	1	,	1	,	7	,	3	,	19	,	35	,	125	,	165	,	21	,	23	,	1119	,	4061	,	8057	,	5129	,	6035	,	29907	};
            const unsigned long	dim4148KuoInit[]		=	{	1	,	1	,	1	,	11	,	31	,	5	,	9	,	165	,	189	,	117	,	1371	,	3171	,	3723	,	9051	,	11071	,	1669	};
            const unsigned long	dim4149KuoInit[]		=	{	1	,	1	,	1	,	7	,	23	,	37	,	61	,	241	,	253	,	79	,	241	,	2629	,	6791	,	2305	,	17085	,	17163	};
            const unsigned long	dim4150KuoInit[]		=	{	1	,	1	,	7	,	11	,	25	,	49	,	109	,	139	,	271	,	325	,	1673	,	1827	,	319	,	10729	,	15739	,	19971	};
            const unsigned long	dim4151KuoInit[]		=	{	1	,	3	,	3	,	9	,	15	,	3	,	21	,	153	,	229	,	781	,	2029	,	3169	,	2191	,	7861	,	26817	,	44695	};
            const unsigned long	dim4152KuoInit[]		=	{	1	,	1	,	1	,	11	,	9	,	45	,	93	,	117	,	361	,	319	,	1747	,	2379	,	259	,	15451	,	2431	,	29633	};
            const unsigned long	dim4153KuoInit[]		=	{	1	,	1	,	3	,	1	,	21	,	3	,	87	,	157	,	389	,	373	,	1235	,	1347	,	6573	,	3447	,	1569	,	42287	};
            const unsigned long	dim4154KuoInit[]		=	{	1	,	3	,	5	,	1	,	3	,	35	,	27	,	49	,	283	,	281	,	733	,	1557	,	3589	,	4047	,	4589	,	981	};
            const unsigned long	dim4155KuoInit[]		=	{	1	,	3	,	5	,	5	,	11	,	15	,	69	,	209	,	319	,	601	,	1559	,	35	,	185	,	8659	,	26943	,	10885	};
            const unsigned long	dim4156KuoInit[]		=	{	1	,	3	,	3	,	15	,	15	,	25	,	105	,	241	,	153	,	813	,	481	,	2269	,	5677	,	11121	,	21781	,	29427	};
            const unsigned long	dim4157KuoInit[]		=	{	1	,	1	,	1	,	13	,	1	,	41	,	123	,	235	,	487	,	339	,	1815	,	241	,	5673	,	9319	,	6031	,	3785	};
            const unsigned long	dim4158KuoInit[]		=	{	1	,	1	,	3	,	9	,	3	,	15	,	111	,	207	,	281	,	7	,	1089	,	709	,	8001	,	14277	,	27351	,	21503	};
            const unsigned long	dim4159KuoInit[]		=	{	1	,	3	,	1	,	11	,	29	,	11	,	69	,	111	,	471	,	549	,	1731	,	115	,	1067	,	8417	,	19377	,	18729	};
            const unsigned long	dim4160KuoInit[]		=	{	1	,	1	,	7	,	1	,	29	,	11	,	31	,	251	,	437	,	217	,	1815	,	1229	,	5735	,	11813	,	20831	,	27237	};
            const unsigned long	dim4161KuoInit[]		=	{	1	,	1	,	3	,	9	,	25	,	63	,	115	,	45	,	71	,	359	,	1313	,	543	,	1837	,	5359	,	13635	,	47057	};
            const unsigned long	dim4162KuoInit[]		=	{	1	,	3	,	1	,	13	,	5	,	21	,	73	,	47	,	13	,	553	,	471	,	465	,	591	,	1943	,	12549	,	20245	};
            const unsigned long	dim4163KuoInit[]		=	{	1	,	3	,	5	,	1	,	5	,	35	,	1	,	5	,	403	,	797	,	987	,	1227	,	2847	,	931	,	7417	,	7551	};
            const unsigned long	dim4164KuoInit[]		=	{	1	,	3	,	3	,	11	,	15	,	13	,	7	,	201	,	145	,	475	,	1557	,	2871	,	3161	,	3489	,	7893	,	1745	};
            const unsigned long	dim4165KuoInit[]		=	{	1	,	1	,	1	,	7	,	15	,	27	,	9	,	129	,	83	,	31	,	1351	,	647	,	6217	,	15201	,	21019	,	39925	};
            const unsigned long	dim4166KuoInit[]		=	{	1	,	1	,	7	,	3	,	19	,	39	,	99	,	253	,	491	,	455	,	405	,	131	,	4755	,	8395	,	26647	,	65287	};
            const unsigned long	dim4167KuoInit[]		=	{	1	,	3	,	5	,	13	,	13	,	13	,	15	,	49	,	397	,	237	,	1491	,	1881	,	4233	,	15801	,	25591	,	47061	};
            const unsigned long	dim4168KuoInit[]		=	{	1	,	1	,	1	,	15	,	11	,	11	,	11	,	161	,	225	,	257	,	231	,	3303	,	5145	,	5217	,	21527	,	50231	};
            const unsigned long	dim4169KuoInit[]		=	{	1	,	3	,	1	,	13	,	21	,	51	,	11	,	161	,	55	,	945	,	1835	,	1493	,	5069	,	9443	,	31181	,	2157	};
            const unsigned long	dim4170KuoInit[]		=	{	1	,	3	,	1	,	15	,	29	,	45	,	75	,	141	,	475	,	43	,	219	,	2689	,	7579	,	523	,	27125	,	48615	};
            const unsigned long	dim4171KuoInit[]		=	{	1	,	3	,	7	,	13	,	5	,	7	,	125	,	13	,	73	,	231	,	1397	,	295	,	3197	,	4023	,	2167	,	42101	};
            const unsigned long	dim4172KuoInit[]		=	{	1	,	1	,	5	,	11	,	5	,	15	,	99	,	241	,	1	,	49	,	1635	,	2467	,	4939	,	9411	,	145	,	62117	};
            const unsigned long	dim4173KuoInit[]		=	{	1	,	3	,	7	,	1	,	3	,	43	,	91	,	179	,	379	,	997	,	1839	,	3463	,	4967	,	12773	,	26273	,	52999	};
            const unsigned long	dim4174KuoInit[]		=	{	1	,	1	,	1	,	5	,	7	,	45	,	127	,	213	,	63	,	93	,	1735	,	623	,	153	,	7039	,	4233	,	49775	};
            const unsigned long	dim4175KuoInit[]		=	{	1	,	3	,	1	,	9	,	19	,	57	,	27	,	225	,	225	,	361	,	1349	,	829	,	1441	,	3557	,	7595	,	48405	};
            const unsigned long	dim4176KuoInit[]		=	{	1	,	1	,	7	,	13	,	27	,	19	,	119	,	115	,	15	,	929	,	345	,	2199	,	8101	,	5787	,	3105	,	58313	};
            const unsigned long	dim4177KuoInit[]		=	{	1	,	3	,	3	,	9	,	23	,	41	,	81	,	19	,	445	,	11	,	771	,	3767	,	6781	,	6969	,	28385	,	8473	};
            const unsigned long	dim4178KuoInit[]		=	{	1	,	1	,	1	,	3	,	1	,	49	,	5	,	149	,	383	,	567	,	1663	,	1277	,	733	,	7651	,	4835	,	56429	};
            const unsigned long	dim4179KuoInit[]		=	{	1	,	1	,	7	,	1	,	29	,	53	,	33	,	135	,	491	,	957	,	1215	,	687	,	1025	,	607	,	459	,	58433	};
            const unsigned long	dim4180KuoInit[]		=	{	1	,	1	,	1	,	5	,	17	,	29	,	67	,	149	,	93	,	169	,	851	,	2311	,	4971	,	13403	,	15023	,	31645	};
            const unsigned long	dim4181KuoInit[]		=	{	1	,	1	,	7	,	15	,	25	,	61	,	127	,	73	,	379	,	279	,	1407	,	285	,	5697	,	11141	,	28961	,	35551	};
            const unsigned long	dim4182KuoInit[]		=	{	1	,	3	,	1	,	3	,	17	,	17	,	123	,	113	,	349	,	203	,	685	,	687	,	6957	,	13033	,	25147	,	61541	};
            const unsigned long	dim4183KuoInit[]		=	{	1	,	3	,	7	,	3	,	29	,	47	,	67	,	161	,	163	,	427	,	241	,	2767	,	6855	,	4975	,	19081	,	4613	};
            const unsigned long	dim4184KuoInit[]		=	{	1	,	1	,	1	,	3	,	7	,	51	,	81	,	57	,	107	,	171	,	1671	,	2373	,	341	,	4071	,	12877	,	55775	};
            const unsigned long	dim4185KuoInit[]		=	{	1	,	1	,	7	,	15	,	29	,	63	,	111	,	255	,	189	,	33	,	1613	,	113	,	4347	,	549	,	9793	,	52611	};
            const unsigned long	dim4186KuoInit[]		=	{	1	,	3	,	3	,	3	,	17	,	11	,	41	,	103	,	509	,	387	,	1341	,	663	,	611	,	12977	,	6435	,	40351	};
            const unsigned long	dim4187KuoInit[]		=	{	1	,	1	,	1	,	5	,	5	,	39	,	13	,	229	,	473	,	159	,	585	,	2905	,	3467	,	8331	,	11539	,	57525	};
            const unsigned long	dim4188KuoInit[]		=	{	1	,	3	,	1	,	7	,	13	,	25	,	87	,	115	,	199	,	611	,	635	,	2653	,	5559	,	7959	,	9545	,	14829	};
            const unsigned long	dim4189KuoInit[]		=	{	1	,	1	,	3	,	7	,	5	,	57	,	53	,	155	,	57	,	301	,	263	,	1597	,	7805	,	8327	,	20457	,	6761	};
            const unsigned long	dim4190KuoInit[]		=	{	1	,	3	,	3	,	5	,	17	,	39	,	105	,	177	,	317	,	681	,	679	,	691	,	7751	,	959	,	2989	,	31533	};
            const unsigned long	dim4191KuoInit[]		=	{	1	,	1	,	3	,	1	,	23	,	15	,	65	,	223	,	379	,	523	,	1285	,	3175	,	1821	,	15655	,	25183	,	9415	};
            const unsigned long	dim4192KuoInit[]		=	{	1	,	3	,	3	,	1	,	11	,	15	,	49	,	33	,	243	,	755	,	2027	,	3115	,	4665	,	12511	,	8653	,	35519	};
            const unsigned long	dim4193KuoInit[]		=	{	1	,	3	,	3	,	7	,	5	,	45	,	45	,	183	,	409	,	107	,	1277	,	2975	,	2291	,	13839	,	18513	,	62993	};
            const unsigned long	dim4194KuoInit[]		=	{	1	,	3	,	5	,	3	,	21	,	39	,	47	,	175	,	197	,	737	,	707	,	2769	,	171	,	12341	,	22361	,	16647	};
            const unsigned long	dim4195KuoInit[]		=	{	1	,	3	,	3	,	1	,	3	,	35	,	127	,	23	,	349	,	477	,	1347	,	1939	,	1199	,	14441	,	9795	,	48051	};
            const unsigned long	dim4196KuoInit[]		=	{	1	,	3	,	1	,	15	,	23	,	59	,	69	,	227	,	429	,	359	,	507	,	3615	,	2331	,	5741	,	23905	,	977	};
            const unsigned long	dim4197KuoInit[]		=	{	1	,	3	,	3	,	1	,	7	,	5	,	111	,	17	,	425	,	697	,	1957	,	1771	,	3495	,	14373	,	8385	,	59295	};
            const unsigned long	dim4198KuoInit[]		=	{	1	,	3	,	3	,	1	,	1	,	25	,	113	,	237	,	393	,	23	,	575	,	1117	,	665	,	3803	,	28291	,	60053	};
            const unsigned long	dim4199KuoInit[]		=	{	1	,	1	,	7	,	7	,	5	,	33	,	43	,	121	,	109	,	201	,	877	,	3203	,	7967	,	11671	,	13397	,	9063	};
            const unsigned long	dim4200KuoInit[]		=	{	1	,	1	,	1	,	9	,	31	,	57	,	31	,	75	,	429	,	471	,	683	,	3593	,	4277	,	10315	,	1913	,	54527	};
            const unsigned long	dim4201KuoInit[]		=	{	1	,	1	,	1	,	3	,	29	,	17	,	69	,	75	,	251	,	595	,	1127	,	3015	,	5621	,	389	,	2245	,	43007	};
            const unsigned long	dim4202KuoInit[]		=	{	1	,	1	,	1	,	5	,	7	,	41	,	127	,	57	,	321	,	971	,	1217	,	3731	,	4667	,	5919	,	29871	,	60487	};
            const unsigned long	dim4203KuoInit[]		=	{	1	,	3	,	3	,	7	,	21	,	9	,	83	,	109	,	489	,	689	,	1601	,	2979	,	405	,	3131	,	19409	,	20739	};
            const unsigned long	dim4204KuoInit[]		=	{	1	,	3	,	7	,	15	,	31	,	45	,	77	,	129	,	269	,	473	,	427	,	2751	,	6841	,	5957	,	25297	,	2301	};
            const unsigned long	dim4205KuoInit[]		=	{	1	,	3	,	7	,	3	,	27	,	3	,	107	,	99	,	37	,	421	,	1853	,	2683	,	7305	,	467	,	32481	,	1085	};
            const unsigned long	dim4206KuoInit[]		=	{	1	,	3	,	7	,	15	,	25	,	3	,	79	,	7	,	47	,	299	,	731	,	1321	,	1917	,	4315	,	17177	,	26499	};
            const unsigned long	dim4207KuoInit[]		=	{	1	,	3	,	7	,	15	,	5	,	1	,	19	,	163	,	335	,	13	,	1855	,	3805	,	6707	,	2133	,	19671	,	53653	};
            const unsigned long	dim4208KuoInit[]		=	{	1	,	1	,	3	,	7	,	3	,	5	,	77	,	69	,	31	,	959	,	1815	,	669	,	5965	,	2925	,	16285	,	42043	};
            const unsigned long	dim4209KuoInit[]		=	{	1	,	3	,	5	,	15	,	25	,	5	,	35	,	175	,	55	,	765	,	597	,	703	,	3141	,	9825	,	29067	,	32817	};
            const unsigned long	dim4210KuoInit[]		=	{	1	,	1	,	3	,	5	,	1	,	57	,	119	,	85	,	167	,	299	,	561	,	1045	,	4255	,	14663	,	27787	,	38785	};
            const unsigned long	dim4211KuoInit[]		=	{	1	,	3	,	5	,	13	,	5	,	41	,	97	,	31	,	55	,	773	,	707	,	1083	,	359	,	15955	,	11203	,	50129	};
            const unsigned long	dim4212KuoInit[]		=	{	1	,	3	,	7	,	5	,	9	,	35	,	101	,	231	,	23	,	301	,	683	,	3075	,	3221	,	7763	,	5585	,	35205	};
            const unsigned long	dim4213KuoInit[]		=	{	1	,	3	,	1	,	13	,	23	,	29	,	83	,	227	,	111	,	307	,	837	,	465	,	5933	,	5265	,	4507	,	8719	};
            const unsigned long	dim4214KuoInit[]		=	{	1	,	3	,	3	,	11	,	21	,	9	,	49	,	157	,	271	,	809	,	1467	,	1337	,	5893	,	13681	,	11261	,	56709	};
            const unsigned long	dim4215KuoInit[]		=	{	1	,	3	,	7	,	1	,	29	,	33	,	89	,	39	,	131	,	339	,	359	,	3087	,	5167	,	5659	,	629	,	53865	};
            const unsigned long	dim4216KuoInit[]		=	{	1	,	3	,	1	,	15	,	11	,	23	,	59	,	105	,	201	,	17	,	923	,	613	,	1665	,	8931	,	28457	,	63933	};
            const unsigned long	dim4217KuoInit[]		=	{	1	,	1	,	5	,	13	,	17	,	33	,	67	,	73	,	367	,	263	,	1377	,	393	,	1561	,	3409	,	24453	,	61637	};
            const unsigned long	dim4218KuoInit[]		=	{	1	,	1	,	5	,	1	,	29	,	9	,	31	,	51	,	485	,	775	,	741	,	1427	,	1857	,	10243	,	18101	,	29795	};
            const unsigned long	dim4219KuoInit[]		=	{	1	,	1	,	1	,	5	,	5	,	39	,	29	,	137	,	171	,	837	,	347	,	1271	,	7365	,	3783	,	22731	,	18131	};
            const unsigned long	dim4220KuoInit[]		=	{	1	,	1	,	5	,	11	,	1	,	45	,	89	,	175	,	11	,	859	,	355	,	1033	,	4767	,	11495	,	28761	,	11335	};
            const unsigned long	dim4221KuoInit[]		=	{	1	,	1	,	7	,	1	,	25	,	7	,	85	,	229	,	243	,	259	,	383	,	1163	,	4833	,	11283	,	31573	,	30891	};
            const unsigned long	dim4222KuoInit[]		=	{	1	,	3	,	7	,	5	,	11	,	41	,	111	,	87	,	203	,	177	,	1113	,	415	,	3613	,	1327	,	20103	,	61295	};
            const unsigned long	dim4223KuoInit[]		=	{	1	,	3	,	3	,	9	,	15	,	53	,	25	,	245	,	335	,	31	,	1335	,	3339	,	2337	,	5719	,	3333	,	51325	};
            const unsigned long	dim4224KuoInit[]		=	{	1	,	1	,	5	,	3	,	5	,	29	,	117	,	11	,	239	,	135	,	1501	,	2707	,	2775	,	2135	,	11477	,	44363	};
            const unsigned long	dim4225KuoInit[]		=	{	1	,	1	,	3	,	1	,	23	,	33	,	3	,	219	,	301	,	643	,	1555	,	1	,	709	,	11	,	30649	,	19729	};
            const unsigned long	dim4226KuoInit[]		=	{	1	,	1	,	5	,	7	,	31	,	9	,	45	,	185	,	203	,	741	,	1851	,	981	,	6437	,	1559	,	27245	,	36797	};
            const unsigned long	dim4227KuoInit[]		=	{	1	,	1	,	3	,	7	,	1	,	41	,	11	,	141	,	53	,	533	,	927	,	2733	,	1049	,	16191	,	1241	,	49695	};
            const unsigned long	dim4228KuoInit[]		=	{	1	,	3	,	7	,	1	,	31	,	21	,	105	,	181	,	487	,	1009	,	135	,	3649	,	6459	,	3397	,	1805	,	29793	};
            const unsigned long	dim4229KuoInit[]		=	{	1	,	1	,	3	,	1	,	1	,	29	,	65	,	127	,	461	,	297	,	1151	,	2403	,	4875	,	343	,	24293	,	47203	};
            const unsigned long	dim4230KuoInit[]		=	{	1	,	1	,	7	,	3	,	9	,	1	,	23	,	139	,	337	,	591	,	1557	,	1993	,	6927	,	5791	,	29199	,	2991	};
            const unsigned long	dim4231KuoInit[]		=	{	1	,	1	,	7	,	9	,	7	,	1	,	81	,	129	,	469	,	215	,	695	,	2691	,	6165	,	14741	,	24339	,	58681	};
            const unsigned long	dim4232KuoInit[]		=	{	1	,	3	,	5	,	13	,	29	,	59	,	113	,	143	,	15	,	399	,	1141	,	975	,	1285	,	12773	,	593	,	15481	};
            const unsigned long	dim4233KuoInit[]		=	{	1	,	1	,	3	,	11	,	19	,	41	,	85	,	95	,	479	,	311	,	1721	,	1089	,	7199	,	15683	,	15909	,	40911	};
            const unsigned long	dim4234KuoInit[]		=	{	1	,	1	,	1	,	3	,	25	,	55	,	23	,	195	,	413	,	623	,	411	,	279	,	3381	,	15313	,	19323	,	22829	};
            const unsigned long	dim4235KuoInit[]		=	{	1	,	3	,	5	,	15	,	13	,	1	,	55	,	91	,	493	,	967	,	1571	,	2887	,	2791	,	1331	,	20565	,	61267	};
            const unsigned long	dim4236KuoInit[]		=	{	1	,	3	,	5	,	1	,	1	,	17	,	127	,	253	,	249	,	559	,	739	,	2921	,	1315	,	13699	,	22501	,	9699	};
            const unsigned long	dim4237KuoInit[]		=	{	1	,	3	,	7	,	13	,	19	,	53	,	31	,	103	,	307	,	821	,	661	,	591	,	5379	,	6401	,	10109	,	7325	};
            const unsigned long	dim4238KuoInit[]		=	{	1	,	1	,	1	,	5	,	15	,	35	,	59	,	149	,	443	,	413	,	139	,	1851	,	3679	,	10721	,	31393	,	37329	};
            const unsigned long	dim4239KuoInit[]		=	{	1	,	1	,	3	,	3	,	15	,	47	,	43	,	241	,	323	,	357	,	1567	,	3183	,	6869	,	4615	,	5617	,	28691	};
            const unsigned long	dim4240KuoInit[]		=	{	1	,	3	,	3	,	1	,	25	,	11	,	55	,	55	,	105	,	277	,	1317	,	3631	,	5351	,	12747	,	1685	,	61239	};
            const unsigned long	dim4241KuoInit[]		=	{	1	,	1	,	7	,	7	,	31	,	19	,	87	,	119	,	409	,	817	,	97	,	3549	,	3517	,	13901	,	11707	,	9501	};
            const unsigned long	dim4242KuoInit[]		=	{	1	,	1	,	7	,	11	,	13	,	17	,	55	,	195	,	457	,	223	,	837	,	453	,	2953	,	2269	,	10149	,	48941	};
            const unsigned long	dim4243KuoInit[]		=	{	1	,	3	,	3	,	13	,	5	,	31	,	63	,	117	,	419	,	329	,	1759	,	1167	,	7967	,	8975	,	11247	,	37159	};
            const unsigned long	dim4244KuoInit[]		=	{	1	,	1	,	7	,	1	,	17	,	19	,	41	,	61	,	349	,	299	,	1449	,	2329	,	471	,	5119	,	29595	,	43335	};
            const unsigned long	dim4245KuoInit[]		=	{	1	,	1	,	1	,	13	,	13	,	63	,	99	,	239	,	239	,	903	,	55	,	1347	,	969	,	9409	,	11603	,	28553	};
            const unsigned long	dim4246KuoInit[]		=	{	1	,	3	,	1	,	7	,	25	,	59	,	23	,	241	,	361	,	1005	,	771	,	923	,	3315	,	4201	,	32695	,	54151	};
            const unsigned long	dim4247KuoInit[]		=	{	1	,	3	,	3	,	5	,	19	,	1	,	113	,	103	,	469	,	591	,	319	,	2297	,	1033	,	9273	,	20529	,	58301	};
            const unsigned long	dim4248KuoInit[]		=	{	1	,	3	,	7	,	11	,	23	,	1	,	77	,	161	,	321	,	855	,	1987	,	3735	,	4533	,	12061	,	19305	,	31679	};
            const unsigned long	dim4249KuoInit[]		=	{	1	,	1	,	3	,	7	,	27	,	15	,	73	,	83	,	113	,	455	,	1855	,	2677	,	3243	,	14961	,	27147	,	21865	};
            const unsigned long	dim4250KuoInit[]		=	{	1	,	3	,	5	,	11	,	15	,	33	,	61	,	87	,	237	,	775	,	1765	,	2783	,	3051	,	3617	,	32519	,	43991	};
            const unsigned long	dim4251KuoInit[]		=	{	1	,	3	,	5	,	1	,	9	,	57	,	21	,	177	,	499	,	605	,	391	,	381	,	2027	,	9221	,	11213	,	10365	};
            const unsigned long	dim4252KuoInit[]		=	{	1	,	1	,	7	,	5	,	31	,	29	,	57	,	159	,	413	,	321	,	245	,	2007	,	2817	,	5173	,	23227	,	19885	};
            const unsigned long	dim4253KuoInit[]		=	{	1	,	1	,	7	,	7	,	29	,	43	,	113	,	173	,	505	,	449	,	409	,	981	,	2257	,	13261	,	85	,	6477	};
            const unsigned long	dim4254KuoInit[]		=	{	1	,	1	,	5	,	13	,	9	,	51	,	1	,	127	,	65	,	963	,	1541	,	1321	,	7849	,	6481	,	1583	,	14737	};
            const unsigned long	dim4255KuoInit[]		=	{	1	,	1	,	3	,	3	,	1	,	45	,	11	,	157	,	375	,	439	,	1673	,	271	,	3853	,	8795	,	23287	,	45365	};
            const unsigned long	dim4256KuoInit[]		=	{	1	,	1	,	3	,	11	,	17	,	7	,	43	,	3	,	3	,	1005	,	523	,	2553	,	2457	,	259	,	20999	,	23611	};
            const unsigned long	dim4257KuoInit[]		=	{	1	,	3	,	3	,	11	,	17	,	33	,	97	,	227	,	19	,	19	,	2039	,	3181	,	1393	,	1427	,	215	,	49915	};
            const unsigned long	dim4258KuoInit[]		=	{	1	,	1	,	5	,	9	,	19	,	1	,	83	,	95	,	7	,	571	,	655	,	1281	,	3841	,	3925	,	24603	,	38413	};
            const unsigned long	dim4259KuoInit[]		=	{	1	,	3	,	1	,	3	,	19	,	47	,	43	,	105	,	431	,	921	,	927	,	3713	,	8181	,	4177	,	9411	,	1481	};
            const unsigned long	dim4260KuoInit[]		=	{	1	,	3	,	1	,	9	,	3	,	29	,	11	,	121	,	505	,	629	,	959	,	523	,	257	,	3089	,	15075	,	14087	};
            const unsigned long	dim4261KuoInit[]		=	{	1	,	3	,	5	,	13	,	9	,	3	,	5	,	167	,	7	,	705	,	1737	,	583	,	2297	,	13073	,	27617	,	22083	};
            const unsigned long	dim4262KuoInit[]		=	{	1	,	3	,	3	,	13	,	9	,	51	,	33	,	191	,	263	,	961	,	1019	,	2771	,	3487	,	7471	,	10939	,	23831	};
            const unsigned long	dim4263KuoInit[]		=	{	1	,	1	,	5	,	5	,	25	,	19	,	75	,	115	,	221	,	93	,	1135	,	257	,	5597	,	14715	,	2385	,	46659	};
            const unsigned long	dim4264KuoInit[]		=	{	1	,	1	,	3	,	5	,	31	,	13	,	63	,	231	,	463	,	501	,	1437	,	1777	,	6161	,	9885	,	10543	,	26031	};
            const unsigned long	dim4265KuoInit[]		=	{	1	,	1	,	5	,	5	,	5	,	1	,	51	,	141	,	311	,	929	,	261	,	2959	,	59	,	4163	,	9855	,	54825	};
            const unsigned long	dim4266KuoInit[]		=	{	1	,	3	,	1	,	9	,	1	,	57	,	31	,	21	,	395	,	211	,	89	,	3703	,	7565	,	2095	,	16273	,	52105	};
            const unsigned long	dim4267KuoInit[]		=	{	1	,	1	,	3	,	11	,	25	,	57	,	27	,	163	,	459	,	849	,	955	,	2437	,	7115	,	8029	,	165	,	12007	};
            const unsigned long	dim4268KuoInit[]		=	{	1	,	1	,	3	,	7	,	29	,	57	,	49	,	71	,	389	,	483	,	991	,	629	,	1517	,	2481	,	4881	,	39431	};
            const unsigned long	dim4269KuoInit[]		=	{	1	,	3	,	5	,	1	,	1	,	11	,	29	,	81	,	323	,	581	,	1515	,	905	,	2813	,	12221	,	20181	,	9855	};
            const unsigned long	dim4270KuoInit[]		=	{	1	,	3	,	1	,	5	,	27	,	3	,	41	,	65	,	369	,	925	,	1449	,	2711	,	3333	,	6263	,	10639	,	43537	};
            const unsigned long	dim4271KuoInit[]		=	{	1	,	1	,	1	,	15	,	17	,	5	,	31	,	181	,	77	,	999	,	1011	,	2631	,	1761	,	10139	,	24369	,	39093	};
            const unsigned long	dim4272KuoInit[]		=	{	1	,	3	,	3	,	11	,	11	,	23	,	65	,	165	,	41	,	591	,	1045	,	263	,	1707	,	4419	,	24005	,	47489	};
            const unsigned long	dim4273KuoInit[]		=	{	1	,	1	,	7	,	13	,	11	,	43	,	65	,	161	,	229	,	35	,	637	,	3693	,	721	,	6863	,	12001	,	48067	};
            const unsigned long	dim4274KuoInit[]		=	{	1	,	3	,	3	,	15	,	9	,	39	,	27	,	65	,	407	,	103	,	1437	,	3403	,	7355	,	2009	,	5979	,	37959	};
            const unsigned long	dim4275KuoInit[]		=	{	1	,	1	,	5	,	5	,	13	,	23	,	65	,	145	,	395	,	603	,	1711	,	1529	,	6309	,	14631	,	4615	,	24459	};
            const unsigned long	dim4276KuoInit[]		=	{	1	,	3	,	7	,	13	,	25	,	27	,	109	,	91	,	205	,	53	,	199	,	1215	,	4371	,	4821	,	11793	,	20719	};
            const unsigned long	dim4277KuoInit[]		=	{	1	,	1	,	5	,	5	,	9	,	9	,	115	,	77	,	223	,	381	,	1035	,	1961	,	5285	,	12465	,	20531	,	51677	};
            const unsigned long	dim4278KuoInit[]		=	{	1	,	3	,	3	,	9	,	13	,	31	,	123	,	123	,	325	,	719	,	1683	,	1253	,	4241	,	1443	,	7507	,	8253	};
            const unsigned long	dim4279KuoInit[]		=	{	1	,	3	,	3	,	7	,	5	,	1	,	71	,	145	,	299	,	1021	,	677	,	1733	,	2791	,	12337	,	24395	,	64145	};
            const unsigned long	dim4280KuoInit[]		=	{	1	,	1	,	7	,	11	,	21	,	31	,	113	,	79	,	1	,	29	,	1171	,	4081	,	7599	,	11661	,	4305	,	11383	};
            const unsigned long	dim4281KuoInit[]		=	{	1	,	3	,	3	,	7	,	27	,	9	,	97	,	117	,	205	,	391	,	1571	,	1801	,	6085	,	15867	,	8987	,	29925	};
            const unsigned long	dim4282KuoInit[]		=	{	1	,	3	,	3	,	3	,	5	,	35	,	27	,	143	,	503	,	533	,	1293	,	3737	,	3011	,	4457	,	18381	,	45513	};
            const unsigned long	dim4283KuoInit[]		=	{	1	,	3	,	1	,	1	,	21	,	55	,	113	,	127	,	365	,	265	,	1211	,	841	,	807	,	3901	,	28993	,	28467	};
            const unsigned long	dim4284KuoInit[]		=	{	1	,	1	,	1	,	7	,	21	,	17	,	101	,	217	,	413	,	273	,	53	,	2415	,	655	,	9939	,	11233	,	31631	};
            const unsigned long	dim4285KuoInit[]		=	{	1	,	3	,	7	,	13	,	11	,	11	,	111	,	63	,	301	,	343	,	1757	,	2615	,	1683	,	11143	,	16405	,	16671	};
            const unsigned long	dim4286KuoInit[]		=	{	1	,	1	,	5	,	15	,	17	,	19	,	83	,	195	,	475	,	189	,	1881	,	699	,	4853	,	7849	,	15063	,	5517	};
            const unsigned long	dim4287KuoInit[]		=	{	1	,	1	,	5	,	5	,	27	,	25	,	19	,	239	,	153	,	725	,	709	,	3717	,	4801	,	1433	,	17789	,	34549	};
            const unsigned long	dim4288KuoInit[]		=	{	1	,	3	,	1	,	9	,	9	,	5	,	47	,	215	,	123	,	161	,	583	,	917	,	5373	,	6839	,	20747	,	47179	};
            const unsigned long	dim4289KuoInit[]		=	{	1	,	1	,	1	,	15	,	1	,	25	,	31	,	217	,	185	,	331	,	261	,	3877	,	2339	,	11557	,	22779	,	58509	};
            const unsigned long	dim4290KuoInit[]		=	{	1	,	1	,	7	,	15	,	11	,	1	,	13	,	53	,	319	,	237	,	763	,	2411	,	7953	,	2117	,	24947	,	26643	};
            const unsigned long	dim4291KuoInit[]		=	{	1	,	3	,	5	,	5	,	17	,	53	,	9	,	253	,	219	,	715	,	1923	,	823	,	1495	,	1975	,	3739	,	61343	};
            const unsigned long	dim4292KuoInit[]		=	{	1	,	1	,	1	,	11	,	27	,	31	,	121	,	253	,	319	,	1001	,	125	,	567	,	215	,	6879	,	22581	,	38495	};
            const unsigned long	dim4293KuoInit[]		=	{	1	,	1	,	1	,	11	,	23	,	17	,	83	,	65	,	279	,	725	,	1611	,	3911	,	509	,	1729	,	30025	,	10791	};
            const unsigned long	dim4294KuoInit[]		=	{	1	,	3	,	1	,	11	,	19	,	17	,	67	,	101	,	129	,	607	,	557	,	3475	,	1445	,	8789	,	1815	,	18171	};
            const unsigned long	dim4295KuoInit[]		=	{	1	,	3	,	7	,	7	,	23	,	57	,	69	,	127	,	147	,	743	,	549	,	3269	,	6193	,	12667	,	17081	,	6899	};
            const unsigned long	dim4296KuoInit[]		=	{	1	,	3	,	1	,	9	,	9	,	29	,	83	,	175	,	389	,	289	,	367	,	1259	,	6631	,	15675	,	20975	,	54067	};
            const unsigned long	dim4297KuoInit[]		=	{	1	,	1	,	7	,	1	,	27	,	11	,	31	,	209	,	159	,	615	,	1813	,	3235	,	7487	,	7103	,	5857	,	19435	};
            const unsigned long	dim4298KuoInit[]		=	{	1	,	3	,	7	,	7	,	17	,	3	,	13	,	255	,	405	,	925	,	55	,	3539	,	3351	,	359	,	7633	,	38209	};
            const unsigned long	dim4299KuoInit[]		=	{	1	,	3	,	5	,	5	,	5	,	51	,	123	,	61	,	485	,	785	,	1975	,	121	,	7841	,	7253	,	11769	,	7389	};
            const unsigned long	dim4300KuoInit[]		=	{	1	,	1	,	1	,	9	,	1	,	3	,	51	,	169	,	163	,	875	,	35	,	1573	,	7931	,	13611	,	14697	,	64843	};
            const unsigned long	dim4301KuoInit[]		=	{	1	,	1	,	5	,	13	,	11	,	35	,	47	,	135	,	475	,	893	,	39	,	2763	,	2671	,	7417	,	22905	,	60593	};
            const unsigned long	dim4302KuoInit[]		=	{	1	,	3	,	5	,	3	,	27	,	27	,	85	,	219	,	341	,	509	,	1131	,	3607	,	1307	,	15265	,	28589	,	61113	};
            const unsigned long	dim4303KuoInit[]		=	{	1	,	1	,	5	,	11	,	11	,	37	,	53	,	67	,	177	,	951	,	237	,	3865	,	2233	,	1405	,	28373	,	47767	};
            const unsigned long	dim4304KuoInit[]		=	{	1	,	3	,	7	,	9	,	17	,	49	,	35	,	167	,	67	,	397	,	927	,	3305	,	2791	,	3609	,	3111	,	4555	};
            const unsigned long	dim4305KuoInit[]		=	{	1	,	1	,	5	,	13	,	13	,	41	,	123	,	89	,	65	,	931	,	1959	,	1751	,	2765	,	8361	,	23653	,	45677	};
            const unsigned long	dim4306KuoInit[]		=	{	1	,	1	,	1	,	3	,	25	,	37	,	91	,	225	,	181	,	275	,	1515	,	1355	,	7971	,	16081	,	13715	,	63963	};
            const unsigned long	dim4307KuoInit[]		=	{	1	,	3	,	3	,	3	,	7	,	11	,	125	,	181	,	127	,	855	,	1277	,	3587	,	1351	,	4907	,	16985	,	357	};
            const unsigned long	dim4308KuoInit[]		=	{	1	,	3	,	5	,	13	,	13	,	57	,	23	,	99	,	115	,	923	,	1395	,	3083	,	3939	,	475	,	12385	,	30113	};
            const unsigned long	dim4309KuoInit[]		=	{	1	,	3	,	1	,	1	,	1	,	19	,	27	,	73	,	91	,	155	,	1145	,	467	,	5655	,	341	,	8875	,	61583	};
            const unsigned long	dim4310KuoInit[]		=	{	1	,	3	,	1	,	1	,	17	,	33	,	113	,	219	,	99	,	817	,	1731	,	3379	,	5729	,	13615	,	19047	,	14143	};
            const unsigned long	dim4311KuoInit[]		=	{	1	,	1	,	3	,	5	,	29	,	7	,	35	,	111	,	405	,	207	,	1811	,	3701	,	6101	,	7147	,	13825	,	52081	};
            const unsigned long	dim4312KuoInit[]		=	{	1	,	1	,	7	,	9	,	19	,	23	,	89	,	95	,	503	,	311	,	1615	,	371	,	5853	,	4635	,	31911	,	65345	};
            const unsigned long	dim4313KuoInit[]		=	{	1	,	1	,	7	,	5	,	17	,	41	,	49	,	143	,	445	,	855	,	1311	,	3961	,	3559	,	5483	,	31609	,	34147	};
            const unsigned long	dim4314KuoInit[]		=	{	1	,	3	,	1	,	15	,	19	,	49	,	67	,	141	,	293	,	757	,	1905	,	2513	,	4405	,	41	,	1191	,	15259	};
            const unsigned long	dim4315KuoInit[]		=	{	1	,	1	,	3	,	11	,	11	,	39	,	113	,	73	,	23	,	117	,	1315	,	725	,	7429	,	5483	,	26213	,	9331	};
            const unsigned long	dim4316KuoInit[]		=	{	1	,	1	,	3	,	3	,	3	,	9	,	119	,	35	,	309	,	515	,	751	,	323	,	2521	,	12093	,	1123	,	43651	};
            const unsigned long	dim4317KuoInit[]		=	{	1	,	1	,	3	,	3	,	29	,	43	,	49	,	53	,	481	,	269	,	915	,	1845	,	3341	,	511	,	23845	,	34623	};
            const unsigned long	dim4318KuoInit[]		=	{	1	,	1	,	3	,	1	,	23	,	23	,	71	,	225	,	269	,	713	,	21	,	1051	,	7635	,	14245	,	3947	,	41757	};
            const unsigned long	dim4319KuoInit[]		=	{	1	,	3	,	5	,	11	,	5	,	19	,	39	,	99	,	309	,	531	,	1645	,	787	,	6913	,	8159	,	1359	,	61973	};
            const unsigned long	dim4320KuoInit[]		=	{	1	,	1	,	5	,	5	,	5	,	61	,	107	,	91	,	197	,	109	,	369	,	1053	,	1995	,	11551	,	4751	,	4981	};
            const unsigned long	dim4321KuoInit[]		=	{	1	,	3	,	5	,	11	,	29	,	31	,	103	,	21	,	393	,	63	,	1465	,	2813	,	995	,	67	,	11997	,	34887	};
            const unsigned long	dim4322KuoInit[]		=	{	1	,	3	,	1	,	7	,	9	,	49	,	111	,	135	,	83	,	79	,	353	,	2703	,	5027	,	10287	,	3753	,	60785	};
            const unsigned long	dim4323KuoInit[]		=	{	1	,	1	,	7	,	15	,	17	,	17	,	51	,	69	,	465	,	25	,	1231	,	995	,	4643	,	3405	,	2237	,	54357	};
            const unsigned long	dim4324KuoInit[]		=	{	1	,	1	,	1	,	1	,	5	,	39	,	31	,	187	,	459	,	197	,	905	,	279	,	2279	,	6585	,	7073	,	1897	};
            const unsigned long	dim4325KuoInit[]		=	{	1	,	3	,	3	,	15	,	13	,	17	,	105	,	29	,	183	,	193	,	541	,	977	,	2247	,	5173	,	10511	,	59969	};
            const unsigned long	dim4326KuoInit[]		=	{	1	,	1	,	1	,	15	,	3	,	11	,	89	,	1	,	411	,	503	,	267	,	2875	,	2585	,	9391	,	19395	,	30107	};
            const unsigned long	dim4327KuoInit[]		=	{	1	,	1	,	7	,	9	,	7	,	19	,	45	,	251	,	143	,	729	,	1825	,	2571	,	6277	,	12759	,	5057	,	58455	};
            const unsigned long	dim4328KuoInit[]		=	{	1	,	3	,	3	,	9	,	7	,	49	,	119	,	3	,	127	,	1015	,	937	,	2585	,	1229	,	16289	,	30087	,	45069	};
            const unsigned long	dim4329KuoInit[]		=	{	1	,	3	,	5	,	13	,	15	,	39	,	39	,	13	,	315	,	533	,	143	,	2107	,	805	,	1487	,	8049	,	3643	};
            const unsigned long	dim4330KuoInit[]		=	{	1	,	3	,	1	,	15	,	23	,	35	,	127	,	143	,	247	,	491	,	837	,	2263	,	1375	,	8283	,	24527	,	37967	};
            const unsigned long	dim4331KuoInit[]		=	{	1	,	3	,	7	,	11	,	13	,	9	,	51	,	171	,	421	,	161	,	1777	,	3679	,	4821	,	10113	,	28459	,	55339	};
            const unsigned long	dim4332KuoInit[]		=	{	1	,	3	,	3	,	1	,	25	,	61	,	53	,	145	,	81	,	509	,	2001	,	315	,	7775	,	7741	,	9239	,	10041	};
            const unsigned long	dim4333KuoInit[]		=	{	1	,	1	,	3	,	3	,	11	,	31	,	21	,	91	,	425	,	415	,	523	,	3649	,	6595	,	9409	,	26631	,	8761	};
            const unsigned long	dim4334KuoInit[]		=	{	1	,	3	,	7	,	7	,	13	,	1	,	51	,	53	,	123	,	125	,	1055	,	3951	,	6621	,	9207	,	25991	,	36435	};
            const unsigned long	dim4335KuoInit[]		=	{	1	,	3	,	5	,	3	,	21	,	63	,	57	,	35	,	197	,	601	,	665	,	3223	,	817	,	3467	,	2667	,	2811	};
            const unsigned long	dim4336KuoInit[]		=	{	1	,	3	,	1	,	11	,	7	,	29	,	51	,	29	,	383	,	969	,	557	,	795	,	7535	,	513	,	1329	,	57567	};
            const unsigned long	dim4337KuoInit[]		=	{	1	,	1	,	1	,	13	,	11	,	59	,	85	,	183	,	163	,	445	,	1595	,	133	,	4277	,	3323	,	6833	,	50359	};
            const unsigned long	dim4338KuoInit[]		=	{	1	,	3	,	1	,	13	,	9	,	11	,	65	,	209	,	509	,	65	,	1299	,	2225	,	7727	,	3851	,	17661	,	39777	};
            const unsigned long	dim4339KuoInit[]		=	{	1	,	1	,	5	,	5	,	13	,	31	,	15	,	189	,	43	,	435	,	707	,	83	,	6713	,	13721	,	24013	,	47037	};
            const unsigned long	dim4340KuoInit[]		=	{	1	,	1	,	1	,	9	,	29	,	49	,	101	,	205	,	7	,	539	,	1809	,	4095	,	7581	,	8187	,	16475	,	10923	};
            const unsigned long	dim4341KuoInit[]		=	{	1	,	3	,	3	,	7	,	27	,	39	,	65	,	117	,	183	,	439	,	997	,	1389	,	2945	,	13303	,	10621	,	63085	};
            const unsigned long	dim4342KuoInit[]		=	{	1	,	1	,	3	,	15	,	11	,	21	,	121	,	179	,	37	,	993	,	1299	,	2741	,	5797	,	10677	,	32519	,	45061	};
            const unsigned long	dim4343KuoInit[]		=	{	1	,	1	,	7	,	1	,	23	,	47	,	107	,	131	,	145	,	679	,	1519	,	1147	,	887	,	2017	,	31191	,	4047	};
            const unsigned long	dim4344KuoInit[]		=	{	1	,	1	,	3	,	11	,	1	,	51	,	1	,	105	,	245	,	269	,	1067	,	3551	,	6323	,	3729	,	16351	,	62965	};
            const unsigned long	dim4345KuoInit[]		=	{	1	,	1	,	3	,	3	,	17	,	11	,	53	,	163	,	387	,	601	,	75	,	3219	,	5683	,	783	,	28143	,	20695	};
            const unsigned long	dim4346KuoInit[]		=	{	1	,	1	,	5	,	15	,	21	,	21	,	33	,	171	,	73	,	157	,	49	,	3443	,	4057	,	8631	,	17555	,	8959	};
            const unsigned long	dim4347KuoInit[]		=	{	1	,	1	,	5	,	9	,	3	,	35	,	13	,	211	,	181	,	425	,	1367	,	2217	,	5949	,	2741	,	8141	,	45769	};
            const unsigned long	dim4348KuoInit[]		=	{	1	,	3	,	1	,	11	,	27	,	43	,	117	,	3	,	331	,	553	,	1847	,	2085	,	7125	,	10827	,	10583	,	60169	};
            const unsigned long	dim4349KuoInit[]		=	{	1	,	1	,	3	,	1	,	15	,	35	,	81	,	15	,	259	,	89	,	1071	,	3807	,	3149	,	3309	,	17791	,	63869	};
            const unsigned long	dim4350KuoInit[]		=	{	1	,	3	,	3	,	5	,	19	,	7	,	113	,	131	,	399	,	691	,	57	,	1837	,	3613	,	1755	,	521	,	64867	};
            const unsigned long	dim4351KuoInit[]		=	{	1	,	3	,	1	,	11	,	25	,	53	,	11	,	241	,	187	,	237	,	1591	,	4059	,	6021	,	8451	,	14305	,	30965	};
            const unsigned long	dim4352KuoInit[]		=	{	1	,	1	,	7	,	11	,	27	,	15	,	9	,	209	,	43	,	137	,	1351	,	2553	,	2841	,	6375	,	20291	,	2523	};
            const unsigned long	dim4353KuoInit[]		=	{	1	,	3	,	3	,	5	,	27	,	37	,	83	,	61	,	365	,	739	,	69	,	725	,	6745	,	7353	,	2621	,	57747	};
            const unsigned long	dim4354KuoInit[]		=	{	1	,	1	,	1	,	1	,	17	,	9	,	53	,	93	,	103	,	467	,	1543	,	2353	,	4901	,	13269	,	21635	,	63425	};
            const unsigned long	dim4355KuoInit[]		=	{	1	,	3	,	1	,	9	,	15	,	19	,	61	,	127	,	423	,	749	,	809	,	515	,	7819	,	1209	,	22879	,	54069	};
            const unsigned long	dim4356KuoInit[]		=	{	1	,	1	,	3	,	7	,	11	,	23	,	93	,	5	,	91	,	613	,	1529	,	2621	,	2689	,	2251	,	16049	,	60773	};
            const unsigned long	dim4357KuoInit[]		=	{	1	,	1	,	5	,	5	,	15	,	27	,	57	,	177	,	477	,	585	,	249	,	3673	,	2651	,	10137	,	30395	,	57475	};
            const unsigned long	dim4358KuoInit[]		=	{	1	,	3	,	1	,	5	,	31	,	39	,	25	,	185	,	233	,	915	,	1335	,	2889	,	1135	,	12515	,	27385	,	54785	};
            const unsigned long	dim4359KuoInit[]		=	{	1	,	3	,	5	,	15	,	3	,	19	,	13	,	105	,	465	,	11	,	799	,	1699	,	6495	,	14307	,	18071	,	55405	};
            const unsigned long	dim4360KuoInit[]		=	{	1	,	1	,	1	,	15	,	31	,	49	,	27	,	209	,	503	,	979	,	877	,	3121	,	3545	,	12875	,	799	,	44427	};
            const unsigned long	dim4361KuoInit[]		=	{	1	,	1	,	5	,	3	,	23	,	17	,	113	,	109	,	301	,	587	,	1865	,	1623	,	453	,	14449	,	26901	,	34687	};
            const unsigned long	dim4362KuoInit[]		=	{	1	,	3	,	7	,	11	,	1	,	29	,	25	,	113	,	147	,	53	,	27	,	2971	,	5663	,	12013	,	1281	,	65173	};
            const unsigned long	dim4363KuoInit[]		=	{	1	,	3	,	1	,	11	,	13	,	47	,	13	,	209	,	339	,	393	,	1389	,	2205	,	537	,	10911	,	20093	,	61499	};
            const unsigned long	dim4364KuoInit[]		=	{	1	,	1	,	7	,	9	,	21	,	45	,	75	,	43	,	425	,	89	,	1319	,	2245	,	6519	,	12819	,	30653	,	18927	};
            const unsigned long	dim4365KuoInit[]		=	{	1	,	1	,	3	,	11	,	1	,	31	,	55	,	29	,	337	,	733	,	1303	,	1721	,	7163	,	13807	,	5865	,	20009	};
            const unsigned long	dim4366KuoInit[]		=	{	1	,	1	,	3	,	15	,	1	,	5	,	101	,	211	,	263	,	865	,	59	,	2473	,	3857	,	9909	,	21353	,	8807	};
            const unsigned long	dim4367KuoInit[]		=	{	1	,	1	,	3	,	9	,	25	,	29	,	5	,	157	,	17	,	985	,	369	,	1903	,	2431	,	15395	,	20245	,	51253	};
            const unsigned long	dim4368KuoInit[]		=	{	1	,	3	,	3	,	11	,	7	,	31	,	45	,	209	,	287	,	13	,	591	,	3907	,	7501	,	10003	,	21815	,	43213	};
            const unsigned long	dim4369KuoInit[]		=	{	1	,	3	,	7	,	5	,	27	,	25	,	73	,	191	,	361	,	731	,	757	,	1907	,	9	,	16357	,	4241	,	40187	};
            const unsigned long	dim4370KuoInit[]		=	{	1	,	1	,	7	,	1	,	5	,	55	,	121	,	89	,	39	,	531	,	493	,	3453	,	5709	,	8215	,	21831	,	5661	};
            const unsigned long	dim4371KuoInit[]		=	{	1	,	3	,	7	,	11	,	27	,	59	,	49	,	251	,	17	,	223	,	875	,	1133	,	3161	,	10273	,	26463	,	45987	};
            const unsigned long	dim4372KuoInit[]		=	{	1	,	3	,	5	,	9	,	17	,	39	,	117	,	153	,	243	,	449	,	1261	,	3917	,	1359	,	4525	,	22119	,	1781	};
            const unsigned long	dim4373KuoInit[]		=	{	1	,	1	,	3	,	7	,	15	,	15	,	19	,	151	,	411	,	693	,	445	,	2525	,	2867	,	12809	,	1487	,	7019	};
            const unsigned long	dim4374KuoInit[]		=	{	1	,	1	,	7	,	7	,	15	,	57	,	61	,	243	,	427	,	529	,	1051	,	3391	,	6259	,	8791	,	3405	,	54757	};
            const unsigned long	dim4375KuoInit[]		=	{	1	,	3	,	1	,	5	,	13	,	41	,	105	,	73	,	485	,	881	,	1943	,	1419	,	5761	,	11807	,	16497	,	12763	};
            const unsigned long	dim4376KuoInit[]		=	{	1	,	3	,	3	,	15	,	7	,	47	,	61	,	99	,	473	,	487	,	1973	,	1675	,	3163	,	4033	,	5323	,	31313	};
            const unsigned long	dim4377KuoInit[]		=	{	1	,	1	,	1	,	3	,	21	,	33	,	23	,	129	,	469	,	743	,	769	,	1561	,	235	,	13713	,	6827	,	61273	};
            const unsigned long	dim4378KuoInit[]		=	{	1	,	3	,	1	,	13	,	11	,	19	,	59	,	37	,	431	,	5	,	1333	,	2381	,	5707	,	2499	,	9339	,	35803	};
            const unsigned long	dim4379KuoInit[]		=	{	1	,	1	,	5	,	1	,	17	,	11	,	77	,	137	,	111	,	567	,	1469	,	543	,	5633	,	1927	,	26491	,	37955	};
            const unsigned long	dim4380KuoInit[]		=	{	1	,	3	,	5	,	15	,	21	,	9	,	57	,	35	,	291	,	371	,	65	,	2891	,	841	,	11859	,	16685	,	5641	};
            const unsigned long	dim4381KuoInit[]		=	{	1	,	3	,	7	,	1	,	3	,	35	,	121	,	153	,	353	,	633	,	995	,	121	,	7317	,	273	,	23329	,	32591	};
            const unsigned long	dim4382KuoInit[]		=	{	1	,	3	,	5	,	1	,	25	,	19	,	35	,	221	,	295	,	543	,	1269	,	3901	,	2109	,	4463	,	4275	,	89	};
            const unsigned long	dim4383KuoInit[]		=	{	1	,	3	,	7	,	13	,	1	,	13	,	85	,	97	,	155	,	731	,	1017	,	3713	,	1613	,	805	,	8927	,	32165	};
            const unsigned long	dim4384KuoInit[]		=	{	1	,	3	,	3	,	13	,	17	,	1	,	21	,	235	,	103	,	875	,	1849	,	2669	,	3193	,	9053	,	16123	,	29481	};
            const unsigned long	dim4385KuoInit[]		=	{	1	,	3	,	7	,	9	,	23	,	21	,	37	,	251	,	441	,	437	,	629	,	1009	,	5663	,	14447	,	23437	,	32521	};
            const unsigned long	dim4386KuoInit[]		=	{	1	,	1	,	7	,	5	,	9	,	5	,	7	,	123	,	483	,	433	,	867	,	149	,	1975	,	5497	,	7887	,	25779	};
            const unsigned long	dim4387KuoInit[]		=	{	1	,	3	,	3	,	3	,	31	,	17	,	81	,	167	,	399	,	1	,	1777	,	833	,	8035	,	8597	,	28371	,	19293	};
            const unsigned long	dim4388KuoInit[]		=	{	1	,	3	,	3	,	11	,	9	,	45	,	125	,	121	,	147	,	613	,	599	,	111	,	7583	,	3189	,	13185	,	35603	};
            const unsigned long	dim4389KuoInit[]		=	{	1	,	1	,	1	,	15	,	31	,	51	,	9	,	191	,	55	,	837	,	1773	,	2781	,	6351	,	1909	,	5407	,	7533	};
            const unsigned long	dim4390KuoInit[]		=	{	1	,	1	,	3	,	13	,	13	,	35	,	71	,	25	,	465	,	377	,	613	,	2487	,	6297	,	8299	,	27541	,	17779	};
            const unsigned long	dim4391KuoInit[]		=	{	1	,	1	,	3	,	15	,	21	,	59	,	43	,	115	,	369	,	315	,	315	,	3451	,	5785	,	6353	,	22609	,	42793	};
            const unsigned long	dim4392KuoInit[]		=	{	1	,	1	,	5	,	15	,	13	,	3	,	127	,	87	,	211	,	981	,	629	,	2683	,	5683	,	15475	,	20431	,	32977	};
            const unsigned long	dim4393KuoInit[]		=	{	1	,	1	,	3	,	7	,	31	,	3	,	23	,	7	,	117	,	1001	,	431	,	2215	,	5817	,	16283	,	12781	,	63883	};
            const unsigned long	dim4394KuoInit[]		=	{	1	,	1	,	3	,	1	,	27	,	11	,	43	,	195	,	329	,	375	,	655	,	3815	,	5699	,	11259	,	6733	,	26015	};
            const unsigned long	dim4395KuoInit[]		=	{	1	,	3	,	3	,	11	,	19	,	3	,	63	,	187	,	47	,	891	,	1463	,	3259	,	1479	,	15109	,	25387	,	43753	};
            const unsigned long	dim4396KuoInit[]		=	{	1	,	1	,	5	,	11	,	31	,	43	,	59	,	149	,	205	,	447	,	1787	,	627	,	6227	,	9951	,	19069	,	50193	};
            const unsigned long	dim4397KuoInit[]		=	{	1	,	3	,	3	,	7	,	7	,	1	,	41	,	33	,	395	,	139	,	689	,	1597	,	529	,	6861	,	2025	,	19569	};
            const unsigned long	dim4398KuoInit[]		=	{	1	,	1	,	1	,	7	,	17	,	63	,	17	,	73	,	459	,	1007	,	1563	,	879	,	1441	,	525	,	32679	,	56409	};
            const unsigned long	dim4399KuoInit[]		=	{	1	,	3	,	3	,	13	,	3	,	5	,	23	,	239	,	7	,	807	,	1999	,	1427	,	6647	,	2249	,	17687	,	5519	};
            const unsigned long	dim4400KuoInit[]		=	{	1	,	3	,	5	,	1	,	7	,	15	,	7	,	243	,	347	,	415	,	1519	,	2267	,	3119	,	3145	,	27799	,	24453	};
            const unsigned long	dim4401KuoInit[]		=	{	1	,	3	,	3	,	3	,	5	,	1	,	93	,	215	,	475	,	209	,	361	,	951	,	147	,	1401	,	6851	,	59257	};
            const unsigned long	dim4402KuoInit[]		=	{	1	,	1	,	7	,	3	,	29	,	39	,	67	,	195	,	287	,	587	,	315	,	1747	,	7169	,	11967	,	4893	,	797	};
            const unsigned long	dim4403KuoInit[]		=	{	1	,	3	,	5	,	5	,	13	,	49	,	47	,	97	,	105	,	203	,	1237	,	805	,	3595	,	9225	,	23105	,	52679	};
            const unsigned long	dim4404KuoInit[]		=	{	1	,	1	,	5	,	1	,	23	,	41	,	39	,	85	,	7	,	237	,	1545	,	2591	,	1771	,	2291	,	20185	,	46995	};
            const unsigned long	dim4405KuoInit[]		=	{	1	,	1	,	1	,	3	,	15	,	5	,	43	,	211	,	109	,	599	,	77	,	703	,	5749	,	15909	,	12619	,	41635	};
            const unsigned long	dim4406KuoInit[]		=	{	1	,	3	,	1	,	9	,	19	,	9	,	49	,	211	,	149	,	883	,	1617	,	497	,	7689	,	4253	,	18737	,	25293	};
            const unsigned long	dim4407KuoInit[]		=	{	1	,	1	,	7	,	11	,	25	,	3	,	57	,	83	,	181	,	801	,	1515	,	3821	,	5287	,	15243	,	2347	,	42333	};
            const unsigned long	dim4408KuoInit[]		=	{	1	,	3	,	7	,	15	,	15	,	25	,	107	,	79	,	249	,	721	,	1237	,	1587	,	3469	,	10319	,	6421	,	59103	};
            const unsigned long	dim4409KuoInit[]		=	{	1	,	1	,	3	,	3	,	17	,	21	,	81	,	35	,	173	,	101	,	1879	,	3301	,	4409	,	8563	,	27981	,	64269	};
            const unsigned long	dim4410KuoInit[]		=	{	1	,	1	,	7	,	3	,	29	,	17	,	103	,	17	,	443	,	821	,	1133	,	73	,	2587	,	16119	,	17385	,	2293	};
            const unsigned long	dim4411KuoInit[]		=	{	1	,	3	,	7	,	13	,	9	,	3	,	41	,	125	,	149	,	403	,	537	,	695	,	3885	,	15769	,	11315	,	53979	};
            const unsigned long	dim4412KuoInit[]		=	{	1	,	3	,	5	,	7	,	11	,	47	,	57	,	105	,	433	,	1009	,	1749	,	2453	,	2083	,	1543	,	25941	,	58037	};
            const unsigned long	dim4413KuoInit[]		=	{	1	,	1	,	1	,	3	,	19	,	63	,	45	,	251	,	265	,	591	,	1235	,	1349	,	6069	,	14139	,	4299	,	32719	};
            const unsigned long	dim4414KuoInit[]		=	{	1	,	3	,	5	,	9	,	1	,	49	,	125	,	217	,	247	,	915	,	1289	,	3589	,	7	,	5285	,	32443	,	56439	};
            const unsigned long	dim4415KuoInit[]		=	{	1	,	3	,	5	,	3	,	31	,	31	,	67	,	55	,	203	,	487	,	983	,	1841	,	1587	,	14881	,	29187	,	59293	};
            const unsigned long	dim4416KuoInit[]		=	{	1	,	1	,	1	,	13	,	1	,	59	,	121	,	57	,	99	,	857	,	1085	,	3505	,	3615	,	13725	,	10363	,	16841	};
            const unsigned long	dim4417KuoInit[]		=	{	1	,	1	,	5	,	3	,	19	,	55	,	43	,	95	,	141	,	217	,	1087	,	1129	,	3417	,	515	,	23815	,	13135	};
            const unsigned long	dim4418KuoInit[]		=	{	1	,	1	,	1	,	15	,	9	,	63	,	5	,	139	,	327	,	743	,	1321	,	4055	,	4337	,	15115	,	8635	,	53571	};
            const unsigned long	dim4419KuoInit[]		=	{	1	,	1	,	5	,	11	,	31	,	47	,	105	,	91	,	493	,	77	,	1767	,	1647	,	4287	,	5455	,	1875	,	6669	};
            const unsigned long	dim4420KuoInit[]		=	{	1	,	3	,	1	,	1	,	25	,	47	,	19	,	243	,	131	,	387	,	69	,	2057	,	7151	,	8497	,	10975	,	64383	};
            const unsigned long	dim4421KuoInit[]		=	{	1	,	3	,	3	,	1	,	13	,	19	,	93	,	35	,	63	,	881	,	1787	,	331	,	57	,	12875	,	5689	,	56411	};
            const unsigned long	dim4422KuoInit[]		=	{	1	,	3	,	5	,	1	,	1	,	33	,	39	,	193	,	17	,	291	,	1507	,	1719	,	3507	,	12683	,	16467	,	21453	};
            const unsigned long	dim4423KuoInit[]		=	{	1	,	1	,	7	,	7	,	15	,	41	,	113	,	23	,	499	,	641	,	937	,	2505	,	4093	,	4897	,	2493	,	28043	};
            const unsigned long	dim4424KuoInit[]		=	{	1	,	3	,	1	,	15	,	13	,	11	,	59	,	175	,	299	,	233	,	189	,	675	,	2623	,	15695	,	2895	,	29213	};
            const unsigned long	dim4425KuoInit[]		=	{	1	,	1	,	3	,	3	,	25	,	33	,	113	,	81	,	485	,	615	,	1451	,	1331	,	607	,	13451	,	5853	,	5051	};
            const unsigned long	dim4426KuoInit[]		=	{	1	,	1	,	7	,	15	,	13	,	55	,	107	,	43	,	343	,	383	,	63	,	2761	,	5543	,	10249	,	2903	,	28567	};
            const unsigned long	dim4427KuoInit[]		=	{	1	,	1	,	7	,	13	,	7	,	23	,	13	,	199	,	109	,	87	,	389	,	561	,	2403	,	12411	,	18247	,	40239	};
            const unsigned long	dim4428KuoInit[]		=	{	1	,	1	,	1	,	13	,	21	,	31	,	3	,	109	,	97	,	803	,	991	,	847	,	6517	,	3389	,	15347	,	18295	};
            const unsigned long	dim4429KuoInit[]		=	{	1	,	1	,	1	,	1	,	27	,	41	,	115	,	59	,	189	,	105	,	63	,	3169	,	2293	,	6051	,	26017	,	49947	};
            const unsigned long	dim4430KuoInit[]		=	{	1	,	3	,	5	,	13	,	19	,	27	,	47	,	13	,	9	,	135	,	637	,	2167	,	2865	,	13353	,	1725	,	41671	};
            const unsigned long	dim4431KuoInit[]		=	{	1	,	1	,	5	,	7	,	17	,	61	,	37	,	247	,	447	,	633	,	1761	,	2075	,	6385	,	12393	,	31483	,	1857	};
            const unsigned long	dim4432KuoInit[]		=	{	1	,	3	,	5	,	13	,	17	,	17	,	103	,	47	,	391	,	725	,	1379	,	3939	,	6467	,	6723	,	4229	,	50221	};
            const unsigned long	dim4433KuoInit[]		=	{	1	,	1	,	5	,	5	,	1	,	61	,	13	,	157	,	379	,	935	,	105	,	419	,	147	,	6369	,	15413	,	38553	};
            const unsigned long	dim4434KuoInit[]		=	{	1	,	1	,	5	,	1	,	13	,	49	,	63	,	165	,	207	,	741	,	1285	,	3247	,	4053	,	1647	,	6571	,	61075	};
            const unsigned long	dim4435KuoInit[]		=	{	1	,	1	,	5	,	15	,	29	,	41	,	51	,	5	,	123	,	45	,	537	,	2887	,	1329	,	9447	,	22285	,	45247	};
            const unsigned long	dim4436KuoInit[]		=	{	1	,	3	,	1	,	3	,	29	,	51	,	65	,	91	,	153	,	673	,	1759	,	401	,	2969	,	6631	,	16965	,	49027	};
            const unsigned long	dim4437KuoInit[]		=	{	1	,	3	,	3	,	15	,	29	,	61	,	31	,	85	,	13	,	427	,	1833	,	2145	,	4229	,	14543	,	22741	,	24913	};
            const unsigned long	dim4438KuoInit[]		=	{	1	,	3	,	5	,	3	,	21	,	9	,	45	,	253	,	89	,	397	,	965	,	685	,	4013	,	11731	,	2947	,	11995	};
            const unsigned long	dim4439KuoInit[]		=	{	1	,	3	,	3	,	11	,	27	,	45	,	101	,	151	,	21	,	563	,	203	,	445	,	6863	,	2151	,	4999	,	8839	};
            const unsigned long	dim4440KuoInit[]		=	{	1	,	3	,	5	,	9	,	25	,	57	,	15	,	29	,	3	,	127	,	871	,	1851	,	7825	,	1593	,	11143	,	24507	};
            const unsigned long	dim4441KuoInit[]		=	{	1	,	1	,	1	,	7	,	17	,	27	,	49	,	159	,	425	,	773	,	1841	,	1693	,	7829	,	2905	,	16347	,	5581	};
            const unsigned long	dim4442KuoInit[]		=	{	1	,	3	,	5	,	7	,	13	,	29	,	81	,	77	,	349	,	747	,	119	,	1195	,	5	,	8481	,	1543	,	50033	};
            const unsigned long	dim4443KuoInit[]		=	{	1	,	1	,	7	,	15	,	1	,	25	,	33	,	13	,	9	,	565	,	555	,	699	,	4691	,	1749	,	9531	,	30133	};
            const unsigned long	dim4444KuoInit[]		=	{	1	,	1	,	5	,	5	,	13	,	43	,	17	,	31	,	199	,	381	,	393	,	3511	,	6331	,	10377	,	31467	,	25305	};
            const unsigned long	dim4445KuoInit[]		=	{	1	,	3	,	3	,	13	,	1	,	49	,	23	,	241	,	499	,	791	,	1851	,	321	,	6057	,	2141	,	8825	,	1829	};
            const unsigned long	dim4446KuoInit[]		=	{	1	,	1	,	1	,	1	,	13	,	23	,	77	,	103	,	123	,	247	,	1655	,	1403	,	6059	,	14505	,	1579	,	48169	};
            const unsigned long	dim4447KuoInit[]		=	{	1	,	1	,	7	,	13	,	9	,	23	,	125	,	15	,	479	,	447	,	327	,	3743	,	1199	,	5015	,	19009	,	50929	};
            const unsigned long	dim4448KuoInit[]		=	{	1	,	1	,	7	,	5	,	5	,	55	,	39	,	131	,	229	,	607	,	1591	,	769	,	2899	,	9681	,	11789	,	62135	};
            const unsigned long	dim4449KuoInit[]		=	{	1	,	3	,	3	,	13	,	17	,	59	,	65	,	117	,	299	,	85	,	1187	,	3249	,	295	,	15603	,	28909	,	60317	};
            const unsigned long	dim4450KuoInit[]		=	{	1	,	1	,	5	,	13	,	13	,	31	,	7	,	5	,	65	,	785	,	985	,	1637	,	3015	,	14261	,	15089	,	37337	};
            const unsigned long	dim4451KuoInit[]		=	{	1	,	3	,	3	,	3	,	17	,	49	,	123	,	151	,	41	,	419	,	391	,	1265	,	6225	,	2037	,	8409	,	13437	};
            const unsigned long	dim4452KuoInit[]		=	{	1	,	1	,	5	,	13	,	1	,	23	,	111	,	21	,	381	,	343	,	519	,	1071	,	3889	,	14897	,	9611	,	10657	};
            const unsigned long	dim4453KuoInit[]		=	{	1	,	3	,	5	,	13	,	25	,	15	,	79	,	215	,	99	,	783	,	107	,	3845	,	443	,	1115	,	24657	,	54263	};
            const unsigned long	dim4454KuoInit[]		=	{	1	,	3	,	3	,	7	,	7	,	29	,	15	,	213	,	165	,	911	,	351	,	4025	,	7657	,	2079	,	21525	,	60751	};
            const unsigned long	dim4455KuoInit[]		=	{	1	,	1	,	3	,	3	,	15	,	19	,	107	,	181	,	177	,	279	,	803	,	2825	,	4609	,	9955	,	7725	,	44161	};
            const unsigned long	dim4456KuoInit[]		=	{	1	,	1	,	3	,	7	,	21	,	9	,	69	,	107	,	43	,	239	,	397	,	2567	,	1375	,	12483	,	19783	,	27889	};
            const unsigned long	dim4457KuoInit[]		=	{	1	,	3	,	7	,	5	,	7	,	39	,	39	,	71	,	397	,	829	,	373	,	1869	,	6229	,	2299	,	19737	,	58479	};
            const unsigned long	dim4458KuoInit[]		=	{	1	,	1	,	3	,	7	,	13	,	53	,	39	,	41	,	105	,	547	,	1687	,	153	,	4067	,	16317	,	15	,	52949	};
            const unsigned long	dim4459KuoInit[]		=	{	1	,	3	,	7	,	11	,	9	,	19	,	97	,	45	,	225	,	977	,	545	,	3691	,	5315	,	6239	,	26833	,	28115	};
            const unsigned long	dim4460KuoInit[]		=	{	1	,	3	,	7	,	3	,	23	,	11	,	17	,	13	,	437	,	145	,	1153	,	1193	,	835	,	5095	,	5535	,	17033	};
            const unsigned long	dim4461KuoInit[]		=	{	1	,	3	,	1	,	3	,	7	,	13	,	17	,	197	,	289	,	875	,	97	,	1721	,	2369	,	8351	,	10813	,	10129	};
            const unsigned long	dim4462KuoInit[]		=	{	1	,	3	,	7	,	13	,	7	,	59	,	11	,	37	,	77	,	541	,	837	,	2943	,	423	,	6093	,	32495	,	59203	};
            const unsigned long	dim4463KuoInit[]		=	{	1	,	1	,	5	,	9	,	13	,	17	,	117	,	239	,	499	,	819	,	1585	,	1755	,	219	,	4681	,	10771	,	14713	};
            const unsigned long	dim4464KuoInit[]		=	{	1	,	3	,	1	,	9	,	15	,	21	,	3	,	123	,	303	,	223	,	1579	,	1959	,	7767	,	6079	,	2085	,	7683	};
            const unsigned long	dim4465KuoInit[]		=	{	1	,	1	,	7	,	15	,	13	,	37	,	35	,	37	,	479	,	341	,	1315	,	3339	,	3059	,	251	,	31629	,	32757	};
            const unsigned long	dim4466KuoInit[]		=	{	1	,	1	,	5	,	11	,	19	,	23	,	55	,	109	,	443	,	77	,	577	,	1963	,	3769	,	1783	,	5059	,	35905	};
            const unsigned long	dim4467KuoInit[]		=	{	1	,	1	,	5	,	15	,	17	,	45	,	53	,	139	,	261	,	147	,	1117	,	2255	,	1389	,	6505	,	21785	,	52763	};
            const unsigned long	dim4468KuoInit[]		=	{	1	,	1	,	7	,	15	,	21	,	61	,	97	,	147	,	397	,	231	,	775	,	3297	,	4883	,	7059	,	16769	,	22765	};
            const unsigned long	dim4469KuoInit[]		=	{	1	,	3	,	7	,	13	,	13	,	29	,	67	,	117	,	381	,	505	,	1437	,	939	,	4551	,	8767	,	7355	,	10507	};
            const unsigned long	dim4470KuoInit[]		=	{	1	,	3	,	3	,	7	,	5	,	57	,	115	,	205	,	159	,	587	,	169	,	3787	,	1271	,	9583	,	12205	,	63993	};
            const unsigned long	dim4471KuoInit[]		=	{	1	,	3	,	3	,	13	,	9	,	59	,	51	,	165	,	159	,	391	,	421	,	2179	,	2943	,	505	,	19315	,	16175	};
            const unsigned long	dim4472KuoInit[]		=	{	1	,	3	,	5	,	13	,	21	,	53	,	95	,	131	,	275	,	421	,	435	,	2655	,	4999	,	14225	,	26417	,	57465	};
            const unsigned long	dim4473KuoInit[]		=	{	1	,	1	,	1	,	5	,	29	,	39	,	115	,	183	,	507	,	849	,	1935	,	2849	,	6301	,	11937	,	19519	,	37019	};
            const unsigned long	dim4474KuoInit[]		=	{	1	,	1	,	5	,	1	,	13	,	15	,	37	,	25	,	113	,	885	,	1265	,	2287	,	4535	,	12265	,	10433	,	56641	};
            const unsigned long	dim4475KuoInit[]		=	{	1	,	3	,	1	,	9	,	29	,	11	,	67	,	19	,	11	,	347	,	1659	,	3251	,	3179	,	2583	,	10127	,	21549	};
            const unsigned long	dim4476KuoInit[]		=	{	1	,	3	,	7	,	7	,	13	,	27	,	101	,	101	,	197	,	381	,	987	,	1699	,	2913	,	5179	,	3423	,	30389	};
            const unsigned long	dim4477KuoInit[]		=	{	1	,	1	,	7	,	13	,	29	,	39	,	87	,	65	,	265	,	891	,	973	,	1487	,	6687	,	7875	,	13389	,	39141	};
            const unsigned long	dim4478KuoInit[]		=	{	1	,	1	,	1	,	3	,	7	,	13	,	101	,	223	,	181	,	143	,	1291	,	2803	,	251	,	12541	,	20713	,	18137	};
            const unsigned long	dim4479KuoInit[]		=	{	1	,	1	,	1	,	15	,	3	,	9	,	103	,	137	,	121	,	563	,	1079	,	3119	,	5619	,	2175	,	27223	,	25289	};
            const unsigned long	dim4480KuoInit[]		=	{	1	,	3	,	3	,	15	,	7	,	13	,	109	,	201	,	385	,	347	,	319	,	883	,	13	,	3683	,	19615	,	9781	};
            const unsigned long	dim4481KuoInit[]		=	{	1	,	1	,	1	,	5	,	13	,	15	,	127	,	161	,	353	,	567	,	555	,	2287	,	1791	,	9535	,	26227	,	15335	};
            const unsigned long	dim4482KuoInit[]		=	{	1	,	1	,	5	,	9	,	3	,	63	,	11	,	83	,	235	,	741	,	977	,	2265	,	449	,	10293	,	20451	,	25143	};
            const unsigned long	dim4483KuoInit[]		=	{	1	,	1	,	3	,	3	,	23	,	25	,	119	,	1	,	381	,	463	,	67	,	257	,	7087	,	10553	,	10781	,	60003	};
            const unsigned long	dim4484KuoInit[]		=	{	1	,	3	,	7	,	15	,	17	,	11	,	81	,	17	,	363	,	163	,	1765	,	285	,	4239	,	14777	,	15081	,	54151	};
            const unsigned long	dim4485KuoInit[]		=	{	1	,	3	,	3	,	13	,	17	,	9	,	73	,	237	,	273	,	439	,	1419	,	1539	,	667	,	13833	,	31783	,	49459	};
            const unsigned long	dim4486KuoInit[]		=	{	1	,	1	,	3	,	3	,	15	,	11	,	85	,	3	,	481	,	169	,	1443	,	3403	,	1157	,	595	,	6851	,	59343	};
            const unsigned long	dim4487KuoInit[]		=	{	1	,	3	,	7	,	1	,	23	,	21	,	13	,	251	,	403	,	859	,	175	,	1373	,	3711	,	1233	,	16879	,	4659	};
            const unsigned long	dim4488KuoInit[]		=	{	1	,	1	,	5	,	7	,	21	,	7	,	15	,	165	,	265	,	117	,	385	,	2661	,	1481	,	1709	,	19843	,	29639	};
            const unsigned long	dim4489KuoInit[]		=	{	1	,	1	,	1	,	11	,	3	,	7	,	81	,	3	,	429	,	997	,	1759	,	285	,	375	,	3487	,	5257	,	14203	};
            const unsigned long	dim4490KuoInit[]		=	{	1	,	3	,	7	,	1	,	15	,	17	,	93	,	69	,	303	,	809	,	929	,	3025	,	6333	,	4525	,	6441	,	32265	};
            const unsigned long	dim4491KuoInit[]		=	{	1	,	3	,	1	,	5	,	27	,	41	,	119	,	25	,	461	,	347	,	369	,	489	,	377	,	3899	,	12711	,	24475	};
            const unsigned long	dim4492KuoInit[]		=	{	1	,	3	,	7	,	5	,	29	,	59	,	101	,	159	,	305	,	337	,	1371	,	891	,	6013	,	2987	,	27201	,	61055	};
            const unsigned long	dim4493KuoInit[]		=	{	1	,	1	,	1	,	3	,	19	,	5	,	3	,	219	,	71	,	213	,	137	,	1925	,	2365	,	583	,	11591	,	9495	};
            const unsigned long	dim4494KuoInit[]		=	{	1	,	3	,	7	,	3	,	31	,	31	,	85	,	115	,	169	,	499	,	1669	,	499	,	1987	,	2505	,	435	,	6377	};
            const unsigned long	dim4495KuoInit[]		=	{	1	,	1	,	3	,	5	,	3	,	33	,	33	,	41	,	199	,	187	,	1023	,	771	,	3447	,	4355	,	29353	,	42293	};
            const unsigned long	dim4496KuoInit[]		=	{	1	,	1	,	5	,	7	,	1	,	41	,	17	,	231	,	305	,	975	,	975	,	1697	,	7267	,	9567	,	9975	,	51313	};
            const unsigned long	dim4497KuoInit[]		=	{	1	,	3	,	5	,	3	,	9	,	41	,	27	,	29	,	285	,	469	,	1845	,	665	,	3007	,	4199	,	3367	,	50849	};
            const unsigned long	dim4498KuoInit[]		=	{	1	,	1	,	5	,	9	,	29	,	5	,	93	,	63	,	227	,	751	,	1591	,	649	,	6219	,	13371	,	3473	,	34033	};
            const unsigned long	dim4499KuoInit[]		=	{	1	,	3	,	3	,	1	,	13	,	5	,	27	,	239	,	89	,	893	,	1869	,	3789	,	7569	,	525	,	32019	,	17501	};
            const unsigned long	dim4500KuoInit[]		=	{	1	,	1	,	5	,	11	,	9	,	5	,	51	,	149	,	351	,	81	,	1895	,	3011	,	2521	,	4185	,	1277	,	65397	};
            const unsigned long	dim4501KuoInit[]		=	{	1	,	3	,	5	,	3	,	21	,	13	,	121	,	213	,	67	,	825	,	1493	,	2933	,	6189	,	14135	,	7021	,	47435	};
            const unsigned long	dim4502KuoInit[]		=	{	1	,	3	,	7	,	15	,	11	,	51	,	63	,	163	,	371	,	363	,	1885	,	1535	,	629	,	2939	,	12547	,	24779	};
            const unsigned long	dim4503KuoInit[]		=	{	1	,	3	,	7	,	3	,	3	,	3	,	49	,	47	,	409	,	963	,	1499	,	1107	,	8063	,	11137	,	21251	,	4695	};
            const unsigned long	dim4504KuoInit[]		=	{	1	,	1	,	3	,	1	,	31	,	17	,	61	,	149	,	263	,	503	,	219	,	3419	,	4525	,	14689	,	30841	,	60799	};
            const unsigned long	dim4505KuoInit[]		=	{	1	,	3	,	3	,	5	,	31	,	25	,	5	,	247	,	219	,	79	,	1605	,	3523	,	4815	,	12907	,	22017	,	16145	};
            const unsigned long	dim4506KuoInit[]		=	{	1	,	1	,	7	,	11	,	21	,	51	,	47	,	85	,	37	,	461	,	1299	,	2007	,	2093	,	10707	,	17997	,	4221	};
            const unsigned long	dim4507KuoInit[]		=	{	1	,	3	,	3	,	11	,	11	,	59	,	63	,	191	,	359	,	859	,	1265	,	853	,	6733	,	4605	,	27559	,	32557	};
            const unsigned long	dim4508KuoInit[]		=	{	1	,	3	,	7	,	11	,	29	,	61	,	119	,	73	,	73	,	259	,	1939	,	59	,	5177	,	12375	,	22143	,	28073	};
            const unsigned long	dim4509KuoInit[]		=	{	1	,	3	,	7	,	7	,	21	,	29	,	19	,	33	,	59	,	539	,	281	,	2989	,	2239	,	6671	,	31629	,	16065	};
            const unsigned long	dim4510KuoInit[]		=	{	1	,	3	,	5	,	7	,	17	,	27	,	39	,	57	,	483	,	463	,	51	,	3771	,	5641	,	5875	,	2369	,	21619	};
            const unsigned long	dim4511KuoInit[]		=	{	1	,	3	,	1	,	1	,	15	,	9	,	121	,	217	,	107	,	615	,	1413	,	2083	,	231	,	9177	,	12275	,	20069	};
            const unsigned long	dim4512KuoInit[]		=	{	1	,	3	,	5	,	15	,	17	,	55	,	85	,	27	,	477	,	687	,	1295	,	3471	,	6951	,	2645	,	30051	,	36149	};
            const unsigned long	dim4513KuoInit[]		=	{	1	,	3	,	3	,	1	,	1	,	27	,	9	,	183	,	39	,	559	,	1639	,	5	,	1177	,	16011	,	1447	,	42897	};
            const unsigned long	dim4514KuoInit[]		=	{	1	,	1	,	1	,	5	,	21	,	9	,	75	,	217	,	311	,	679	,	1913	,	2623	,	3853	,	10889	,	22501	,	40089	};
            const unsigned long	dim4515KuoInit[]		=	{	1	,	3	,	5	,	7	,	17	,	59	,	23	,	199	,	147	,	339	,	1027	,	1471	,	6539	,	7963	,	30713	,	1939	};
            const unsigned long	dim4516KuoInit[]		=	{	1	,	1	,	3	,	11	,	23	,	51	,	111	,	175	,	433	,	803	,	1577	,	4091	,	2197	,	10003	,	2597	,	53553	};
            const unsigned long	dim4517KuoInit[]		=	{	1	,	3	,	5	,	1	,	3	,	11	,	125	,	11	,	281	,	969	,	1943	,	1003	,	2189	,	3551	,	25831	,	64261	};
            const unsigned long	dim4518KuoInit[]		=	{	1	,	3	,	7	,	1	,	17	,	19	,	111	,	235	,	511	,	723	,	1779	,	1041	,	4067	,	3827	,	17285	,	63075	};
            const unsigned long	dim4519KuoInit[]		=	{	1	,	1	,	3	,	15	,	3	,	63	,	7	,	165	,	153	,	829	,	221	,	1485	,	4319	,	9865	,	937	,	65171	};
            const unsigned long	dim4520KuoInit[]		=	{	1	,	1	,	7	,	1	,	21	,	21	,	89	,	233	,	417	,	15	,	367	,	311	,	3575	,	11285	,	5245	,	3947	};
            const unsigned long	dim4521KuoInit[]		=	{	1	,	3	,	7	,	9	,	27	,	55	,	3	,	71	,	227	,	397	,	561	,	4075	,	5495	,	16071	,	11495	,	48733	};
            const unsigned long	dim4522KuoInit[]		=	{	1	,	3	,	3	,	3	,	3	,	39	,	57	,	219	,	237	,	151	,	1305	,	2485	,	5045	,	5065	,	21041	,	20055	};
            const unsigned long	dim4523KuoInit[]		=	{	1	,	1	,	3	,	11	,	15	,	33	,	75	,	241	,	335	,	207	,	1695	,	119	,	839	,	12449	,	28467	,	7445	};
            const unsigned long	dim4524KuoInit[]		=	{	1	,	1	,	7	,	3	,	21	,	3	,	43	,	137	,	321	,	763	,	575	,	2317	,	8073	,	1225	,	8257	,	11639	};
            const unsigned long	dim4525KuoInit[]		=	{	1	,	3	,	5	,	11	,	27	,	21	,	17	,	115	,	265	,	1023	,	1403	,	853	,	757	,	1799	,	25505	,	49207	};
            const unsigned long	dim4526KuoInit[]		=	{	1	,	1	,	7	,	5	,	9	,	45	,	57	,	77	,	127	,	447	,	1097	,	1729	,	2361	,	1599	,	15727	,	49413	};
            const unsigned long	dim4527KuoInit[]		=	{	1	,	1	,	1	,	15	,	21	,	41	,	13	,	41	,	183	,	151	,	441	,	3077	,	4205	,	125	,	23767	,	44667	};
            const unsigned long	dim4528KuoInit[]		=	{	1	,	3	,	3	,	9	,	3	,	37	,	5	,	203	,	39	,	49	,	1793	,	3523	,	7541	,	23	,	22527	,	35999	};
            const unsigned long	dim4529KuoInit[]		=	{	1	,	3	,	7	,	13	,	13	,	5	,	127	,	169	,	465	,	759	,	393	,	1543	,	3327	,	6223	,	18675	,	36075	};
            const unsigned long	dim4530KuoInit[]		=	{	1	,	3	,	5	,	15	,	17	,	57	,	83	,	237	,	307	,	495	,	135	,	1461	,	4353	,	15251	,	10005	,	12769	};
            const unsigned long	dim4531KuoInit[]		=	{	1	,	1	,	7	,	13	,	27	,	49	,	57	,	19	,	463	,	97	,	217	,	2825	,	7729	,	2375	,	8339	,	17563	};
            const unsigned long	dim4532KuoInit[]		=	{	1	,	3	,	7	,	1	,	31	,	13	,	17	,	245	,	329	,	1021	,	619	,	3149	,	7089	,	4381	,	14079	,	35659	};
            const unsigned long	dim4533KuoInit[]		=	{	1	,	3	,	5	,	15	,	31	,	19	,	15	,	217	,	115	,	159	,	281	,	2233	,	4315	,	7429	,	25527	,	44671	};
            const unsigned long	dim4534KuoInit[]		=	{	1	,	1	,	7	,	1	,	11	,	23	,	7	,	181	,	43	,	271	,	607	,	389	,	357	,	3003	,	15899	,	50849	};
            const unsigned long	dim4535KuoInit[]		=	{	1	,	1	,	7	,	15	,	11	,	15	,	41	,	247	,	37	,	163	,	665	,	3059	,	5151	,	15335	,	13167	,	18541	};
            const unsigned long	dim4536KuoInit[]		=	{	1	,	1	,	7	,	15	,	25	,	33	,	33	,	135	,	425	,	785	,	1557	,	793	,	7153	,	13473	,	8833	,	21335	};
            const unsigned long	dim4537KuoInit[]		=	{	1	,	3	,	3	,	1	,	17	,	27	,	7	,	75	,	205	,	71	,	581	,	733	,	5009	,	1029	,	27537	,	41905	};
            const unsigned long	dim4538KuoInit[]		=	{	1	,	3	,	3	,	3	,	5	,	63	,	121	,	3	,	381	,	607	,	1099	,	1571	,	1077	,	15491	,	13785	,	31537	};
            const unsigned long	dim4539KuoInit[]		=	{	1	,	1	,	5	,	7	,	17	,	15	,	87	,	81	,	313	,	683	,	31	,	3301	,	1683	,	12667	,	17867	,	38405	};
            const unsigned long	dim4540KuoInit[]		=	{	1	,	1	,	1	,	13	,	3	,	23	,	51	,	95	,	409	,	711	,	381	,	245	,	7	,	15679	,	14281	,	39789	};
            const unsigned long	dim4541KuoInit[]		=	{	1	,	3	,	1	,	1	,	31	,	35	,	97	,	35	,	121	,	45	,	1397	,	4009	,	2829	,	3273	,	2725	,	913	};
            const unsigned long	dim4542KuoInit[]		=	{	1	,	3	,	1	,	11	,	29	,	29	,	111	,	153	,	499	,	371	,	1767	,	3063	,	6175	,	8205	,	3485	,	8951	};
            const unsigned long	dim4543KuoInit[]		=	{	1	,	3	,	5	,	5	,	27	,	9	,	113	,	141	,	295	,	901	,	1367	,	791	,	1645	,	5993	,	6139	,	12853	};
            const unsigned long	dim4544KuoInit[]		=	{	1	,	1	,	1	,	7	,	27	,	17	,	113	,	187	,	293	,	517	,	1327	,	579	,	1191	,	12039	,	13973	,	51129	};
            const unsigned long	dim4545KuoInit[]		=	{	1	,	1	,	7	,	3	,	11	,	9	,	55	,	71	,	47	,	787	,	1311	,	2567	,	7555	,	2825	,	11485	,	16787	};
            const unsigned long	dim4546KuoInit[]		=	{	1	,	3	,	1	,	7	,	29	,	33	,	115	,	115	,	69	,	155	,	1185	,	2653	,	7127	,	1449	,	8875	,	34819	};
            const unsigned long	dim4547KuoInit[]		=	{	1	,	3	,	5	,	15	,	9	,	61	,	61	,	161	,	29	,	33	,	1155	,	613	,	7233	,	2811	,	4837	,	27781	};
            const unsigned long	dim4548KuoInit[]		=	{	1	,	1	,	5	,	3	,	25	,	57	,	85	,	121	,	505	,	45	,	1143	,	3237	,	6403	,	2377	,	30791	,	26637	};
            const unsigned long	dim4549KuoInit[]		=	{	1	,	3	,	3	,	5	,	31	,	39	,	63	,	255	,	153	,	417	,	865	,	441	,	7649	,	16321	,	29147	,	11089	};
            const unsigned long	dim4550KuoInit[]		=	{	1	,	1	,	7	,	5	,	25	,	19	,	43	,	45	,	457	,	353	,	25	,	1135	,	4663	,	11419	,	25373	,	8411	};
            const unsigned long	dim4551KuoInit[]		=	{	1	,	3	,	7	,	11	,	11	,	47	,	101	,	255	,	437	,	937	,	263	,	1171	,	7893	,	277	,	4413	,	20197	};
            const unsigned long	dim4552KuoInit[]		=	{	1	,	1	,	5	,	13	,	21	,	61	,	1	,	31	,	227	,	345	,	1535	,	1597	,	7061	,	11595	,	12055	,	29611	};
            const unsigned long	dim4553KuoInit[]		=	{	1	,	1	,	3	,	1	,	19	,	49	,	83	,	61	,	305	,	611	,	399	,	1397	,	4753	,	2309	,	15097	,	13439	};
            const unsigned long	dim4554KuoInit[]		=	{	1	,	1	,	3	,	9	,	27	,	37	,	77	,	165	,	273	,	85	,	483	,	299	,	5073	,	1255	,	4073	,	56333	};
            const unsigned long	dim4555KuoInit[]		=	{	1	,	1	,	7	,	3	,	5	,	11	,	127	,	157	,	489	,	653	,	165	,	2071	,	4819	,	121	,	32755	,	21623	};
            const unsigned long	dim4556KuoInit[]		=	{	1	,	3	,	5	,	11	,	23	,	5	,	111	,	25	,	173	,	661	,	353	,	1167	,	2625	,	8021	,	18731	,	42705	};
            const unsigned long	dim4557KuoInit[]		=	{	1	,	3	,	5	,	15	,	9	,	33	,	31	,	137	,	283	,	925	,	1641	,	115	,	7047	,	11365	,	13097	,	4711	};
            const unsigned long	dim4558KuoInit[]		=	{	1	,	3	,	7	,	13	,	13	,	13	,	29	,	135	,	273	,	325	,	1085	,	1683	,	1365	,	9931	,	8557	,	3855	};
            const unsigned long	dim4559KuoInit[]		=	{	1	,	3	,	5	,	13	,	17	,	27	,	103	,	183	,	237	,	343	,	907	,	2945	,	3039	,	12873	,	28635	,	14791	};
            const unsigned long	dim4560KuoInit[]		=	{	1	,	3	,	3	,	13	,	5	,	51	,	13	,	57	,	131	,	1021	,	1995	,	921	,	6911	,	14381	,	17113	,	56783	};
            const unsigned long	dim4561KuoInit[]		=	{	1	,	3	,	5	,	5	,	5	,	55	,	89	,	29	,	409	,	687	,	1151	,	3423	,	1893	,	12153	,	8167	,	14849	};
            const unsigned long	dim4562KuoInit[]		=	{	1	,	3	,	5	,	1	,	1	,	53	,	41	,	53	,	81	,	797	,	1665	,	85	,	8131	,	3853	,	17815	,	7771	};
            const unsigned long	dim4563KuoInit[]		=	{	1	,	1	,	7	,	3	,	31	,	41	,	37	,	107	,	319	,	421	,	279	,	2061	,	1981	,	6917	,	25545	,	46053	};
            const unsigned long	dim4564KuoInit[]		=	{	1	,	3	,	3	,	11	,	13	,	25	,	85	,	203	,	399	,	863	,	1577	,	1445	,	8093	,	1845	,	16307	,	61075	};
            const unsigned long	dim4565KuoInit[]		=	{	1	,	1	,	5	,	5	,	1	,	35	,	117	,	201	,	271	,	901	,	1939	,	2291	,	343	,	13591	,	28371	,	61429	};
            const unsigned long	dim4566KuoInit[]		=	{	1	,	3	,	3	,	7	,	21	,	55	,	85	,	123	,	183	,	787	,	1121	,	447	,	5761	,	6925	,	22407	,	53149	};
            const unsigned long	dim4567KuoInit[]		=	{	1	,	3	,	1	,	13	,	31	,	25	,	63	,	247	,	109	,	741	,	231	,	2621	,	4939	,	11155	,	21625	,	30433	};
            const unsigned long	dim4568KuoInit[]		=	{	1	,	1	,	5	,	15	,	3	,	49	,	63	,	147	,	117	,	703	,	1317	,	2803	,	4655	,	5981	,	14473	,	52739	};
            const unsigned long	dim4569KuoInit[]		=	{	1	,	1	,	7	,	1	,	27	,	45	,	117	,	55	,	391	,	275	,	1793	,	1279	,	4799	,	15509	,	17173	,	25103	};
            const unsigned long	dim4570KuoInit[]		=	{	1	,	3	,	3	,	1	,	27	,	15	,	35	,	43	,	209	,	869	,	1833	,	3625	,	5973	,	8965	,	1443	,	59053	};
            const unsigned long	dim4571KuoInit[]		=	{	1	,	1	,	1	,	1	,	1	,	19	,	93	,	63	,	407	,	735	,	1821	,	2875	,	1263	,	1461	,	7811	,	17257	};
            const unsigned long	dim4572KuoInit[]		=	{	1	,	1	,	7	,	5	,	15	,	35	,	3	,	89	,	355	,	983	,	1183	,	2709	,	127	,	10671	,	31223	,	26689	};
            const unsigned long	dim4573KuoInit[]		=	{	1	,	3	,	3	,	3	,	15	,	29	,	105	,	239	,	293	,	57	,	515	,	363	,	5227	,	14693	,	18437	,	13107	};
            const unsigned long	dim4574KuoInit[]		=	{	1	,	1	,	5	,	1	,	29	,	61	,	65	,	77	,	23	,	311	,	1581	,	215	,	1409	,	2575	,	13915	,	63179	};
            const unsigned long	dim4575KuoInit[]		=	{	1	,	3	,	1	,	5	,	27	,	17	,	113	,	255	,	347	,	493	,	1795	,	817	,	7483	,	10611	,	9941	,	56425	};
            const unsigned long	dim4576KuoInit[]		=	{	1	,	3	,	1	,	11	,	1	,	29	,	79	,	153	,	183	,	975	,	1127	,	881	,	1319	,	11481	,	16807	,	23167	};
            const unsigned long	dim4577KuoInit[]		=	{	1	,	1	,	5	,	15	,	7	,	31	,	107	,	245	,	59	,	579	,	423	,	2165	,	5137	,	14735	,	13187	,	28781	};
            const unsigned long	dim4578KuoInit[]		=	{	1	,	3	,	1	,	13	,	13	,	55	,	63	,	155	,	63	,	581	,	1083	,	2249	,	7397	,	2081	,	23727	,	23981	};
            const unsigned long	dim4579KuoInit[]		=	{	1	,	3	,	7	,	9	,	23	,	29	,	93	,	215	,	361	,	71	,	119	,	3215	,	3529	,	14249	,	29593	,	27305	};
            const unsigned long	dim4580KuoInit[]		=	{	1	,	1	,	7	,	9	,	5	,	63	,	45	,	181	,	463	,	21	,	229	,	4069	,	4889	,	2141	,	25623	,	6191	};
            const unsigned long	dim4581KuoInit[]		=	{	1	,	3	,	1	,	1	,	17	,	31	,	105	,	153	,	301	,	433	,	517	,	585	,	285	,	12805	,	30173	,	21363	};
            const unsigned long	dim4582KuoInit[]		=	{	1	,	3	,	3	,	15	,	29	,	23	,	117	,	19	,	415	,	389	,	881	,	2067	,	583	,	11383	,	24951	,	34745	};
            const unsigned long	dim4583KuoInit[]		=	{	1	,	1	,	3	,	13	,	27	,	55	,	37	,	5	,	337	,	943	,	1851	,	3227	,	3881	,	7365	,	10853	,	23325	};
            const unsigned long	dim4584KuoInit[]		=	{	1	,	3	,	1	,	11	,	25	,	5	,	39	,	189	,	503	,	745	,	1349	,	3881	,	5037	,	13855	,	19537	,	22329	};
            const unsigned long	dim4585KuoInit[]		=	{	1	,	3	,	7	,	9	,	7	,	33	,	63	,	61	,	39	,	825	,	53	,	1471	,	3589	,	13473	,	29241	,	40829	};
            const unsigned long	dim4586KuoInit[]		=	{	1	,	1	,	7	,	1	,	1	,	37	,	111	,	251	,	427	,	427	,	937	,	3921	,	8033	,	269	,	14261	,	37345	};
            const unsigned long	dim4587KuoInit[]		=	{	1	,	1	,	7	,	7	,	31	,	59	,	111	,	205	,	285	,	113	,	951	,	2867	,	3769	,	5237	,	693	,	42097	};
            const unsigned long	dim4588KuoInit[]		=	{	1	,	1	,	1	,	15	,	29	,	43	,	15	,	253	,	383	,	783	,	1407	,	3851	,	4333	,	11607	,	12777	,	55499	};
            const unsigned long	dim4589KuoInit[]		=	{	1	,	1	,	3	,	15	,	15	,	27	,	75	,	57	,	87	,	185	,	1079	,	2721	,	915	,	13633	,	26439	,	10697	};
            const unsigned long	dim4590KuoInit[]		=	{	1	,	1	,	7	,	13	,	17	,	63	,	43	,	65	,	5	,	699	,	1157	,	2271	,	837	,	687	,	29551	,	22109	};
            const unsigned long	dim4591KuoInit[]		=	{	1	,	3	,	7	,	3	,	5	,	47	,	103	,	143	,	281	,	81	,	283	,	3073	,	3741	,	11525	,	4499	,	41325	};
            const unsigned long	dim4592KuoInit[]		=	{	1	,	3	,	7	,	11	,	13	,	19	,	63	,	215	,	215	,	21	,	1443	,	2485	,	2661	,	8123	,	11739	,	48729	};
            const unsigned long	dim4593KuoInit[]		=	{	1	,	3	,	3	,	3	,	27	,	13	,	111	,	153	,	17	,	715	,	1453	,	1827	,	5201	,	1139	,	19531	,	18853	};
            const unsigned long	dim4594KuoInit[]		=	{	1	,	3	,	7	,	15	,	7	,	61	,	109	,	213	,	383	,	483	,	301	,	1791	,	559	,	14205	,	28441	,	13179	};
            const unsigned long	dim4595KuoInit[]		=	{	1	,	1	,	7	,	7	,	23	,	55	,	119	,	45	,	467	,	775	,	951	,	2533	,	6703	,	15791	,	12891	,	54513	};
            const unsigned long	dim4596KuoInit[]		=	{	1	,	1	,	7	,	15	,	19	,	49	,	71	,	127	,	455	,	881	,	329	,	663	,	4997	,	8755	,	17517	,	31507	};
            const unsigned long	dim4597KuoInit[]		=	{	1	,	1	,	5	,	9	,	9	,	13	,	75	,	161	,	109	,	631	,	153	,	15	,	1181	,	14299	,	11815	,	32017	};
            const unsigned long	dim4598KuoInit[]		=	{	1	,	3	,	5	,	15	,	9	,	63	,	101	,	85	,	487	,	213	,	1743	,	2575	,	6979	,	14421	,	18051	,	14505	};
            const unsigned long	dim4599KuoInit[]		=	{	1	,	1	,	5	,	7	,	5	,	5	,	109	,	245	,	427	,	765	,	2013	,	2929	,	6737	,	14561	,	32227	,	60605	};
            const unsigned long	dim4600KuoInit[]		=	{	1	,	3	,	1	,	3	,	1	,	7	,	17	,	169	,	27	,	925	,	1711	,	101	,	2065	,	4857	,	863	,	38371	};
            const unsigned long	dim4601KuoInit[]		=	{	1	,	1	,	7	,	5	,	5	,	25	,	119	,	145	,	27	,	377	,	1535	,	441	,	5181	,	9789	,	26757	,	46977	};
            const unsigned long	dim4602KuoInit[]		=	{	1	,	1	,	7	,	5	,	31	,	1	,	79	,	223	,	199	,	409	,	2033	,	1781	,	5269	,	2631	,	10627	,	41785	};
            const unsigned long	dim4603KuoInit[]		=	{	1	,	3	,	5	,	3	,	29	,	7	,	91	,	123	,	407	,	19	,	847	,	4019	,	1029	,	15763	,	30265	,	49287	};
            const unsigned long	dim4604KuoInit[]		=	{	1	,	1	,	3	,	9	,	1	,	37	,	33	,	53	,	49	,	949	,	1737	,	229	,	3419	,	5125	,	16365	,	48867	};
            const unsigned long	dim4605KuoInit[]		=	{	1	,	3	,	7	,	9	,	25	,	17	,	13	,	31	,	27	,	53	,	287	,	4021	,	7687	,	14511	,	12627	,	54409	};
            const unsigned long	dim4606KuoInit[]		=	{	1	,	1	,	3	,	9	,	21	,	29	,	1	,	193	,	479	,	895	,	605	,	2013	,	6571	,	14073	,	28185	,	58845	};
            const unsigned long	dim4607KuoInit[]		=	{	1	,	3	,	5	,	11	,	3	,	21	,	111	,	215	,	259	,	941	,	1729	,	645	,	695	,	3295	,	23825	,	1009	};
            const unsigned long	dim4608KuoInit[]		=	{	1	,	3	,	3	,	9	,	25	,	17	,	103	,	187	,	153	,	287	,	911	,	1265	,	41	,	1687	,	22443	,	21117	};
            const unsigned long	dim4609KuoInit[]		=	{	1	,	3	,	5	,	5	,	31	,	25	,	3	,	107	,	271	,	531	,	1413	,	2813	,	5075	,	2127	,	18647	,	5257	};
            const unsigned long	dim4610KuoInit[]		=	{	1	,	1	,	1	,	7	,	5	,	25	,	71	,	225	,	391	,	877	,	1713	,	1841	,	3239	,	9063	,	11445	,	48693	};
            const unsigned long	dim4611KuoInit[]		=	{	1	,	1	,	1	,	13	,	21	,	25	,	15	,	157	,	59	,	37	,	1349	,	413	,	6401	,	3533	,	6019	,	40373	};
            const unsigned long	dim4612KuoInit[]		=	{	1	,	1	,	5	,	5	,	5	,	3	,	31	,	185	,	59	,	933	,	1415	,	901	,	6075	,	4839	,	32477	,	43085	};
            const unsigned long	dim4613KuoInit[]		=	{	1	,	3	,	1	,	5	,	11	,	7	,	81	,	109	,	167	,	277	,	1803	,	39	,	7421	,	15573	,	2233	,	8193	};
            const unsigned long	dim4614KuoInit[]		=	{	1	,	3	,	1	,	1	,	13	,	15	,	47	,	109	,	279	,	515	,	1393	,	1785	,	4297	,	813	,	19177	,	5951	};
            const unsigned long	dim4615KuoInit[]		=	{	1	,	3	,	3	,	1	,	21	,	23	,	3	,	27	,	317	,	677	,	1775	,	1109	,	93	,	9007	,	8767	,	42181	};
            const unsigned long	dim4616KuoInit[]		=	{	1	,	3	,	5	,	13	,	23	,	15	,	49	,	9	,	369	,	293	,	529	,	2115	,	4781	,	1955	,	21897	,	22871	};
            const unsigned long	dim4617KuoInit[]		=	{	1	,	1	,	7	,	9	,	5	,	39	,	85	,	85	,	405	,	837	,	1891	,	27	,	509	,	14643	,	10497	,	34021	};
            const unsigned long	dim4618KuoInit[]		=	{	1	,	1	,	7	,	9	,	7	,	47	,	49	,	87	,	481	,	1003	,	741	,	329	,	2951	,	5317	,	21601	,	20485	};
            const unsigned long	dim4619KuoInit[]		=	{	1	,	3	,	5	,	11	,	27	,	61	,	45	,	251	,	489	,	623	,	1673	,	891	,	5429	,	4457	,	23147	,	5201	};
            const unsigned long	dim4620KuoInit[]		=	{	1	,	3	,	3	,	7	,	25	,	27	,	3	,	233	,	193	,	253	,	483	,	1881	,	6753	,	7843	,	3975	,	23413	};
            const unsigned long	dim4621KuoInit[]		=	{	1	,	3	,	1	,	15	,	27	,	31	,	51	,	213	,	463	,	995	,	1127	,	3589	,	1409	,	15743	,	22755	,	39913	};
            const unsigned long	dim4622KuoInit[]		=	{	1	,	3	,	1	,	13	,	21	,	17	,	35	,	157	,	233	,	899	,	1003	,	201	,	3425	,	6703	,	26585	,	18389	};
            const unsigned long	dim4623KuoInit[]		=	{	1	,	3	,	1	,	9	,	11	,	23	,	69	,	93	,	361	,	177	,	1071	,	3523	,	4117	,	6107	,	3701	,	64213	};
            const unsigned long	dim4624KuoInit[]		=	{	1	,	3	,	5	,	1	,	19	,	1	,	35	,	95	,	385	,	853	,	1635	,	1815	,	5307	,	3155	,	11347	,	39289	};
            const unsigned long	dim4625KuoInit[]		=	{	1	,	1	,	7	,	9	,	3	,	41	,	37	,	161	,	293	,	1005	,	1915	,	2867	,	841	,	195	,	4247	,	8621	};
            const unsigned long	dim4626KuoInit[]		=	{	1	,	3	,	7	,	11	,	19	,	43	,	69	,	95	,	455	,	669	,	217	,	129	,	1275	,	8431	,	22519	,	39261	};
            const unsigned long	dim4627KuoInit[]		=	{	1	,	3	,	7	,	13	,	31	,	49	,	105	,	61	,	437	,	923	,	1579	,	1695	,	7129	,	10555	,	3177	,	6667	};
            const unsigned long	dim4628KuoInit[]		=	{	1	,	1	,	5	,	7	,	3	,	47	,	29	,	159	,	207	,	969	,	433	,	3541	,	689	,	1255	,	1359	,	38349	};
            const unsigned long	dim4629KuoInit[]		=	{	1	,	1	,	3	,	9	,	9	,	31	,	23	,	37	,	435	,	729	,	121	,	2483	,	8009	,	14229	,	6961	,	40681	};
            const unsigned long	dim4630KuoInit[]		=	{	1	,	3	,	1	,	7	,	19	,	33	,	111	,	131	,	155	,	641	,	795	,	3753	,	4381	,	11135	,	8381	,	62553	};
            const unsigned long	dim4631KuoInit[]		=	{	1	,	3	,	7	,	13	,	29	,	37	,	15	,	245	,	341	,	1023	,	1123	,	3317	,	2409	,	8957	,	4607	,	53947	};
            const unsigned long	dim4632KuoInit[]		=	{	1	,	1	,	7	,	15	,	7	,	19	,	105	,	187	,	407	,	339	,	607	,	11	,	1679	,	13701	,	2385	,	6203	};
            const unsigned long	dim4633KuoInit[]		=	{	1	,	3	,	3	,	9	,	7	,	63	,	7	,	173	,	475	,	87	,	807	,	1467	,	791	,	1949	,	20839	,	54179	};
            const unsigned long	dim4634KuoInit[]		=	{	1	,	3	,	3	,	3	,	25	,	25	,	111	,	155	,	249	,	55	,	1455	,	2503	,	6869	,	14463	,	10519	,	56761	};
            const unsigned long	dim4635KuoInit[]		=	{	1	,	3	,	1	,	9	,	9	,	31	,	89	,	235	,	499	,	61	,	747	,	487	,	6767	,	4777	,	12513	,	43677	};
            const unsigned long	dim4636KuoInit[]		=	{	1	,	3	,	3	,	5	,	25	,	47	,	13	,	3	,	313	,	541	,	1323	,	3551	,	7971	,	1453	,	1531	,	38915	};
            const unsigned long	dim4637KuoInit[]		=	{	1	,	1	,	1	,	5	,	7	,	63	,	125	,	243	,	199	,	875	,	791	,	103	,	3997	,	16199	,	20615	,	18359	};
            const unsigned long	dim4638KuoInit[]		=	{	1	,	1	,	7	,	1	,	21	,	19	,	13	,	9	,	117	,	625	,	1333	,	561	,	889	,	10961	,	11313	,	48139	};
            const unsigned long	dim4639KuoInit[]		=	{	1	,	3	,	5	,	9	,	19	,	57	,	121	,	163	,	333	,	681	,	1065	,	885	,	5961	,	6385	,	20135	,	19733	};
            const unsigned long	dim4640KuoInit[]		=	{	1	,	1	,	7	,	13	,	27	,	11	,	113	,	187	,	91	,	83	,	591	,	517	,	7379	,	12823	,	31815	,	14721	};
            const unsigned long	dim4641KuoInit[]		=	{	1	,	3	,	5	,	7	,	27	,	21	,	47	,	203	,	411	,	191	,	881	,	2597	,	6107	,	135	,	14525	,	4059	};
            const unsigned long	dim4642KuoInit[]		=	{	1	,	3	,	5	,	7	,	29	,	3	,	61	,	231	,	259	,	425	,	1953	,	803	,	7921	,	2321	,	28919	,	41349	};
            const unsigned long	dim4643KuoInit[]		=	{	1	,	3	,	1	,	1	,	27	,	7	,	87	,	197	,	287	,	345	,	457	,	1813	,	2701	,	2095	,	32705	,	52579	};
            const unsigned long	dim4644KuoInit[]		=	{	1	,	3	,	3	,	9	,	13	,	59	,	87	,	105	,	49	,	387	,	73	,	2983	,	6453	,	10977	,	8851	,	61031	};
            const unsigned long	dim4645KuoInit[]		=	{	1	,	3	,	5	,	7	,	1	,	53	,	121	,	129	,	307	,	251	,	1405	,	3093	,	4487	,	1953	,	2681	,	4391	};
            const unsigned long	dim4646KuoInit[]		=	{	1	,	1	,	3	,	1	,	1	,	29	,	119	,	125	,	95	,	425	,	385	,	1577	,	3387	,	14709	,	25143	,	4317	};
            const unsigned long	dim4647KuoInit[]		=	{	1	,	3	,	1	,	3	,	27	,	59	,	53	,	203	,	75	,	731	,	1019	,	3179	,	4533	,	3217	,	30801	,	53439	};
            const unsigned long	dim4648KuoInit[]		=	{	1	,	1	,	3	,	11	,	29	,	53	,	93	,	251	,	441	,	75	,	363	,	2405	,	6263	,	12121	,	11963	,	13219	};
            const unsigned long	dim4649KuoInit[]		=	{	1	,	3	,	1	,	13	,	3	,	7	,	57	,	169	,	387	,	543	,	1027	,	1169	,	5089	,	5077	,	17933	,	8131	};
            const unsigned long	dim4650KuoInit[]		=	{	1	,	1	,	3	,	5	,	17	,	17	,	123	,	161	,	21	,	641	,	795	,	2415	,	2743	,	1489	,	25137	,	19393	};
            const unsigned long	dim4651KuoInit[]		=	{	1	,	3	,	3	,	1	,	1	,	47	,	37	,	95	,	315	,	37	,	1481	,	2189	,	687	,	2859	,	5339	,	26907	};
            const unsigned long	dim4652KuoInit[]		=	{	1	,	3	,	7	,	15	,	23	,	51	,	71	,	203	,	237	,	777	,	417	,	2767	,	5243	,	1453	,	17269	,	27679	};
            const unsigned long	dim4653KuoInit[]		=	{	1	,	3	,	1	,	15	,	17	,	41	,	29	,	45	,	363	,	77	,	1071	,	3419	,	6453	,	13897	,	25275	,	26379	};
            const unsigned long	dim4654KuoInit[]		=	{	1	,	3	,	7	,	11	,	21	,	5	,	89	,	133	,	339	,	971	,	1581	,	377	,	6563	,	13073	,	24521	,	41107	};
            const unsigned long	dim4655KuoInit[]		=	{	1	,	1	,	7	,	15	,	1	,	27	,	57	,	249	,	511	,	833	,	269	,	2771	,	4127	,	12567	,	27359	,	22105	};
            const unsigned long	dim4656KuoInit[]		=	{	1	,	1	,	3	,	5	,	31	,	47	,	63	,	221	,	25	,	869	,	1947	,	3075	,	6857	,	13507	,	16601	,	29621	};
            const unsigned long	dim4657KuoInit[]		=	{	1	,	1	,	3	,	7	,	17	,	23	,	109	,	169	,	211	,	407	,	1147	,	2453	,	2281	,	10763	,	11539	,	49127	};
            const unsigned long	dim4658KuoInit[]		=	{	1	,	1	,	3	,	11	,	27	,	25	,	111	,	75	,	465	,	67	,	1641	,	3623	,	7957	,	14307	,	17469	,	52229	};
            const unsigned long	dim4659KuoInit[]		=	{	1	,	3	,	1	,	5	,	23	,	31	,	49	,	131	,	351	,	311	,	1937	,	2181	,	7485	,	10883	,	20843	,	33447	};
            const unsigned long	dim4660KuoInit[]		=	{	1	,	3	,	1	,	3	,	5	,	47	,	81	,	47	,	305	,	567	,	219	,	2495	,	1207	,	14359	,	4683	,	53717	};
            const unsigned long	dim4661KuoInit[]		=	{	1	,	1	,	3	,	3	,	29	,	43	,	107	,	81	,	227	,	795	,	1261	,	3887	,	6177	,	6219	,	2557	,	53203	};
            const unsigned long	dim4662KuoInit[]		=	{	1	,	3	,	1	,	9	,	25	,	47	,	87	,	177	,	511	,	899	,	1967	,	601	,	3659	,	3961	,	27301	,	9497	};
            const unsigned long	dim4663KuoInit[]		=	{	1	,	1	,	5	,	11	,	7	,	19	,	69	,	95	,	319	,	811	,	1539	,	931	,	6609	,	4325	,	4639	,	60125	};
            const unsigned long	dim4664KuoInit[]		=	{	1	,	3	,	3	,	13	,	31	,	25	,	65	,	137	,	67	,	903	,	1519	,	3	,	81	,	9991	,	24985	,	12571	};
            const unsigned long	dim4665KuoInit[]		=	{	1	,	3	,	1	,	15	,	13	,	45	,	87	,	95	,	185	,	531	,	591	,	3649	,	2289	,	6353	,	4305	,	12801	};
            const unsigned long	dim4666KuoInit[]		=	{	1	,	3	,	7	,	11	,	13	,	49	,	79	,	205	,	9	,	457	,	151	,	3363	,	1865	,	12337	,	3601	,	23781	};
            const unsigned long	dim4667KuoInit[]		=	{	1	,	3	,	3	,	7	,	21	,	7	,	117	,	177	,	327	,	547	,	385	,	301	,	5595	,	14389	,	8243	,	3133	};
            const unsigned long	dim4668KuoInit[]		=	{	1	,	3	,	1	,	3	,	17	,	39	,	61	,	45	,	489	,	769	,	1893	,	1633	,	2645	,	10849	,	3827	,	41365	};
            const unsigned long	dim4669KuoInit[]		=	{	1	,	1	,	1	,	11	,	11	,	43	,	93	,	81	,	497	,	731	,	1049	,	91	,	7979	,	4707	,	8757	,	54837	};
            const unsigned long	dim4670KuoInit[]		=	{	1	,	1	,	3	,	7	,	1	,	63	,	127	,	173	,	137	,	291	,	473	,	3095	,	6291	,	14415	,	20763	,	45687	};
            const unsigned long	dim4671KuoInit[]		=	{	1	,	1	,	5	,	13	,	29	,	59	,	73	,	47	,	171	,	1001	,	537	,	2009	,	5057	,	15013	,	22195	,	56067	};
            const unsigned long	dim4672KuoInit[]		=	{	1	,	1	,	3	,	5	,	21	,	35	,	121	,	169	,	377	,	389	,	1209	,	415	,	2641	,	10423	,	11327	,	64157	};
            const unsigned long	dim4673KuoInit[]		=	{	1	,	1	,	7	,	1	,	23	,	43	,	59	,	205	,	39	,	455	,	633	,	1965	,	7955	,	305	,	12245	,	45229	};
            const unsigned long	dim4674KuoInit[]		=	{	1	,	3	,	1	,	15	,	15	,	27	,	49	,	77	,	407	,	549	,	1159	,	1701	,	7331	,	6421	,	1799	,	43539	};
            const unsigned long	dim4675KuoInit[]		=	{	1	,	3	,	5	,	3	,	5	,	33	,	41	,	127	,	141	,	149	,	1957	,	2823	,	1303	,	7339	,	25253	,	32329	};
            const unsigned long	dim4676KuoInit[]		=	{	1	,	1	,	5	,	15	,	5	,	37	,	119	,	111	,	329	,	781	,	983	,	2021	,	1191	,	1579	,	4191	,	24065	};
            const unsigned long	dim4677KuoInit[]		=	{	1	,	3	,	7	,	9	,	7	,	11	,	25	,	111	,	301	,	819	,	1073	,	2311	,	2357	,	6385	,	17015	,	8051	};
            const unsigned long	dim4678KuoInit[]		=	{	1	,	1	,	1	,	9	,	29	,	63	,	91	,	101	,	71	,	679	,	267	,	2909	,	5359	,	2305	,	2523	,	32423	};
            const unsigned long	dim4679KuoInit[]		=	{	1	,	3	,	1	,	5	,	11	,	45	,	107	,	95	,	487	,	605	,	1647	,	1977	,	5305	,	8855	,	9057	,	7809	};
            const unsigned long	dim4680KuoInit[]		=	{	1	,	1	,	1	,	1	,	5	,	45	,	51	,	51	,	97	,	775	,	1559	,	935	,	7421	,	2823	,	11415	,	49193	};
            const unsigned long	dim4681KuoInit[]		=	{	1	,	3	,	3	,	5	,	23	,	63	,	99	,	189	,	175	,	77	,	675	,	4021	,	899	,	11315	,	18627	,	46175	};
            const unsigned long	dim4682KuoInit[]		=	{	1	,	1	,	3	,	13	,	9	,	31	,	67	,	235	,	409	,	541	,	2013	,	2715	,	2121	,	3695	,	11953	,	5745	};
            const unsigned long	dim4683KuoInit[]		=	{	1	,	3	,	5	,	1	,	13	,	3	,	15	,	111	,	455	,	849	,	231	,	2711	,	6831	,	7813	,	17173	,	26019	};
            const unsigned long	dim4684KuoInit[]		=	{	1	,	1	,	1	,	15	,	19	,	3	,	21	,	135	,	275	,	891	,	61	,	5	,	2901	,	15727	,	18015	,	55741	};
            const unsigned long	dim4685KuoInit[]		=	{	1	,	3	,	1	,	5	,	5	,	31	,	5	,	147	,	353	,	199	,	229	,	3821	,	537	,	11667	,	649	,	33923	};
            const unsigned long	dim4686KuoInit[]		=	{	1	,	1	,	7	,	13	,	3	,	9	,	75	,	217	,	423	,	487	,	723	,	4091	,	7075	,	819	,	2755	,	13593	};
            const unsigned long	dim4687KuoInit[]		=	{	1	,	1	,	1	,	9	,	25	,	45	,	69	,	41	,	183	,	547	,	69	,	3591	,	1331	,	5701	,	25891	,	26553	};
            const unsigned long	dim4688KuoInit[]		=	{	1	,	3	,	3	,	13	,	29	,	51	,	79	,	153	,	145	,	923	,	1345	,	3113	,	93	,	10617	,	12865	,	27341	};
            const unsigned long	dim4689KuoInit[]		=	{	1	,	3	,	3	,	13	,	13	,	61	,	67	,	115	,	295	,	767	,	1077	,	3959	,	7741	,	16145	,	3571	,	38941	};
            const unsigned long	dim4690KuoInit[]		=	{	1	,	1	,	3	,	3	,	1	,	11	,	25	,	33	,	481	,	901	,	135	,	2093	,	3531	,	12137	,	4521	,	42301	};
            const unsigned long	dim4691KuoInit[]		=	{	1	,	1	,	3	,	1	,	9	,	59	,	105	,	71	,	57	,	131	,	1491	,	3259	,	4125	,	10279	,	7537	,	38827	};
            const unsigned long	dim4692KuoInit[]		=	{	1	,	3	,	7	,	13	,	7	,	53	,	83	,	43	,	243	,	423	,	43	,	2021	,	4269	,	1793	,	9969	,	55429	};
            const unsigned long	dim4693KuoInit[]		=	{	1	,	1	,	3	,	11	,	17	,	57	,	73	,	55	,	369	,	975	,	603	,	1893	,	3299	,	12177	,	8965	,	17159	};
            const unsigned long	dim4694KuoInit[]		=	{	1	,	3	,	5	,	11	,	23	,	11	,	61	,	11	,	119	,	905	,	1841	,	2597	,	691	,	6237	,	2285	,	33503	};
            const unsigned long	dim4695KuoInit[]		=	{	1	,	3	,	5	,	3	,	21	,	17	,	35	,	169	,	511	,	1009	,	1287	,	3967	,	1543	,	6983	,	6895	,	24959	};
            const unsigned long	dim4696KuoInit[]		=	{	1	,	1	,	1	,	13	,	23	,	9	,	21	,	47	,	119	,	323	,	1255	,	3601	,	3157	,	14041	,	32065	,	4759	};
            const unsigned long	dim4697KuoInit[]		=	{	1	,	1	,	7	,	15	,	9	,	1	,	103	,	43	,	485	,	281	,	1651	,	1957	,	4215	,	2105	,	24455	,	51001	};
            const unsigned long	dim4698KuoInit[]		=	{	1	,	1	,	5	,	7	,	11	,	11	,	59	,	31	,	351	,	275	,	573	,	2481	,	2545	,	15767	,	4319	,	63375	};
            const unsigned long	dim4699KuoInit[]		=	{	1	,	3	,	1	,	1	,	1	,	41	,	41	,	13	,	33	,	693	,	1035	,	905	,	6129	,	11131	,	23571	,	28789	};
            const unsigned long	dim4700KuoInit[]		=	{	1	,	1	,	7	,	11	,	9	,	39	,	119	,	31	,	109	,	837	,	1905	,	763	,	3737	,	16031	,	8751	,	17307	};
            const unsigned long	dim4701KuoInit[]		=	{	1	,	3	,	3	,	7	,	17	,	49	,	77	,	199	,	33	,	185	,	1489	,	445	,	5495	,	13693	,	21453	,	21039	};
            const unsigned long	dim4702KuoInit[]		=	{	1	,	3	,	5	,	15	,	5	,	9	,	113	,	179	,	315	,	85	,	1559	,	1505	,	593	,	259	,	10393	,	39025	};
            const unsigned long	dim4703KuoInit[]		=	{	1	,	1	,	1	,	5	,	31	,	33	,	41	,	45	,	313	,	513	,	861	,	2697	,	4767	,	10737	,	25019	,	12715	};
            const unsigned long	dim4704KuoInit[]		=	{	1	,	3	,	3	,	1	,	19	,	21	,	19	,	19	,	127	,	1009	,	859	,	829	,	6887	,	5407	,	28491	,	61079	};
            const unsigned long	dim4705KuoInit[]		=	{	1	,	1	,	3	,	15	,	15	,	35	,	69	,	255	,	367	,	889	,	49	,	3237	,	3269	,	1559	,	26337	,	16967	};
            const unsigned long	dim4706KuoInit[]		=	{	1	,	1	,	5	,	3	,	17	,	5	,	125	,	171	,	505	,	981	,	773	,	2973	,	2905	,	1537	,	17729	,	52289	};
            const unsigned long	dim4707KuoInit[]		=	{	1	,	1	,	3	,	3	,	29	,	5	,	57	,	247	,	149	,	707	,	673	,	429	,	7583	,	12135	,	5405	,	313	};
            const unsigned long	dim4708KuoInit[]		=	{	1	,	1	,	3	,	13	,	31	,	35	,	117	,	69	,	413	,	723	,	645	,	3989	,	6461	,	77	,	29713	,	32381	};
            const unsigned long	dim4709KuoInit[]		=	{	1	,	3	,	3	,	13	,	11	,	59	,	5	,	53	,	507	,	355	,	335	,	4027	,	3111	,	8385	,	22369	,	42907	};
            const unsigned long	dim4710KuoInit[]		=	{	1	,	1	,	5	,	9	,	1	,	47	,	87	,	13	,	397	,	693	,	1745	,	2907	,	3919	,	14843	,	869	,	24161	};
            const unsigned long	dim4711KuoInit[]		=	{	1	,	3	,	7	,	13	,	11	,	25	,	25	,	153	,	501	,	305	,	941	,	2379	,	2587	,	8225	,	27951	,	14809	};
            const unsigned long	dim4712KuoInit[]		=	{	1	,	1	,	7	,	3	,	31	,	7	,	9	,	47	,	129	,	611	,	1367	,	1575	,	4421	,	2277	,	8113	,	34229	};
            const unsigned long	dim4713KuoInit[]		=	{	1	,	1	,	5	,	11	,	7	,	41	,	65	,	5	,	289	,	799	,	1143	,	291	,	243	,	12957	,	2729	,	14589	};
            const unsigned long	dim4714KuoInit[]		=	{	1	,	3	,	3	,	7	,	15	,	29	,	49	,	243	,	191	,	565	,	271	,	2485	,	1507	,	13479	,	3541	,	10039	};
            const unsigned long	dim4715KuoInit[]		=	{	1	,	3	,	5	,	3	,	25	,	29	,	113	,	157	,	357	,	891	,	199	,	3113	,	7133	,	11047	,	27517	,	26795	};
            const unsigned long	dim4716KuoInit[]		=	{	1	,	1	,	3	,	15	,	27	,	23	,	5	,	207	,	183	,	85	,	247	,	15	,	5559	,	8005	,	32485	,	26403	};
            const unsigned long	dim4717KuoInit[]		=	{	1	,	1	,	3	,	9	,	13	,	9	,	87	,	9	,	237	,	875	,	1621	,	2955	,	4105	,	9645	,	9613	,	31235	};
            const unsigned long	dim4718KuoInit[]		=	{	1	,	1	,	7	,	11	,	17	,	3	,	77	,	241	,	75	,	433	,	1481	,	1901	,	6249	,	12411	,	18975	,	18911	};
            const unsigned long	dim4719KuoInit[]		=	{	1	,	3	,	5	,	15	,	3	,	43	,	99	,	197	,	407	,	161	,	1103	,	587	,	6695	,	255	,	15571	,	7523	};
            const unsigned long	dim4720KuoInit[]		=	{	1	,	1	,	1	,	13	,	1	,	21	,	9	,	155	,	447	,	785	,	1573	,	2497	,	1515	,	6031	,	23509	,	38199	};
            const unsigned long	dim4721KuoInit[]		=	{	1	,	1	,	5	,	15	,	23	,	43	,	125	,	187	,	57	,	259	,	941	,	1213	,	2303	,	1127	,	31819	,	53539	};
            const unsigned long	dim4722KuoInit[]		=	{	1	,	3	,	3	,	9	,	23	,	19	,	29	,	83	,	431	,	79	,	781	,	1887	,	4857	,	2079	,	11521	,	64925	};
            const unsigned long	dim4723KuoInit[]		=	{	1	,	3	,	5	,	5	,	23	,	11	,	33	,	107	,	127	,	173	,	223	,	491	,	1529	,	14655	,	18857	,	25247	};
            const unsigned long	dim4724KuoInit[]		=	{	1	,	3	,	3	,	9	,	17	,	53	,	119	,	159	,	503	,	153	,	697	,	657	,	6207	,	7659	,	23899	,	10347	};
            const unsigned long	dim4725KuoInit[]		=	{	1	,	3	,	1	,	15	,	13	,	37	,	117	,	219	,	341	,	983	,	193	,	1641	,	7811	,	11961	,	12553	,	9467	};
            const unsigned long	dim4726KuoInit[]		=	{	1	,	1	,	7	,	5	,	27	,	11	,	35	,	45	,	89	,	535	,	965	,	1269	,	1243	,	3987	,	4629	,	45335	};
            const unsigned long	dim4727KuoInit[]		=	{	1	,	1	,	7	,	1	,	17	,	25	,	79	,	133	,	287	,	7	,	269	,	697	,	2537	,	241	,	15747	,	49429	};
            const unsigned long	dim4728KuoInit[]		=	{	1	,	3	,	3	,	3	,	27	,	63	,	23	,	229	,	3	,	625	,	453	,	3037	,	5747	,	13525	,	28835	,	13411	};
            const unsigned long	dim4729KuoInit[]		=	{	1	,	3	,	5	,	5	,	25	,	57	,	101	,	13	,	81	,	917	,	1849	,	3931	,	3319	,	1353	,	8253	,	28897	};
            const unsigned long	dim4730KuoInit[]		=	{	1	,	1	,	3	,	15	,	3	,	41	,	89	,	165	,	1	,	935	,	1859	,	2881	,	4351	,	6737	,	12361	,	27821	};
            const unsigned long	dim4731KuoInit[]		=	{	1	,	3	,	5	,	3	,	9	,	45	,	89	,	251	,	77	,	899	,	777	,	4063	,	8111	,	5953	,	26645	,	62567	};
            const unsigned long	dim4732KuoInit[]		=	{	1	,	3	,	5	,	1	,	25	,	7	,	15	,	189	,	157	,	751	,	627	,	629	,	2875	,	11143	,	23841	,	58751	};
            const unsigned long	dim4733KuoInit[]		=	{	1	,	1	,	1	,	9	,	7	,	43	,	95	,	171	,	311	,	809	,	971	,	1317	,	3521	,	15227	,	18155	,	11409	};
            const unsigned long	dim4734KuoInit[]		=	{	1	,	3	,	3	,	7	,	29	,	7	,	69	,	1	,	235	,	699	,	971	,	3149	,	71	,	3609	,	859	,	17059	};
            const unsigned long	dim4735KuoInit[]		=	{	1	,	3	,	1	,	1	,	3	,	43	,	3	,	181	,	465	,	843	,	1391	,	3313	,	8115	,	1835	,	32365	,	31303	};
            const unsigned long	dim4736KuoInit[]		=	{	1	,	1	,	5	,	1	,	27	,	49	,	15	,	37	,	257	,	365	,	1577	,	3037	,	2767	,	8183	,	173	,	43089	};
            const unsigned long	dim4737KuoInit[]		=	{	1	,	1	,	1	,	7	,	7	,	29	,	81	,	33	,	299	,	827	,	221	,	2049	,	1163	,	13023	,	6755	,	53987	};
            const unsigned long	dim4738KuoInit[]		=	{	1	,	3	,	1	,	11	,	19	,	17	,	63	,	19	,	469	,	925	,	37	,	3291	,	2699	,	10083	,	32197	,	53749	};
            const unsigned long	dim4739KuoInit[]		=	{	1	,	3	,	5	,	15	,	1	,	23	,	71	,	173	,	441	,	109	,	391	,	3611	,	7515	,	5095	,	26379	,	55999	};
            const unsigned long	dim4740KuoInit[]		=	{	1	,	3	,	3	,	13	,	29	,	13	,	23	,	57	,	329	,	285	,	1595	,	2681	,	5427	,	3251	,	7025	,	3921	};
            const unsigned long	dim4741KuoInit[]		=	{	1	,	1	,	1	,	13	,	7	,	45	,	121	,	19	,	19	,	317	,	1619	,	2245	,	7151	,	5797	,	31369	,	7765	};
            const unsigned long	dim4742KuoInit[]		=	{	1	,	3	,	7	,	13	,	9	,	17	,	11	,	127	,	337	,	271	,	2005	,	787	,	1017	,	3075	,	29453	,	62275	};
            const unsigned long	dim4743KuoInit[]		=	{	1	,	3	,	3	,	13	,	9	,	51	,	77	,	31	,	395	,	175	,	247	,	1021	,	4343	,	3829	,	959	,	12069	};
            const unsigned long	dim4744KuoInit[]		=	{	1	,	3	,	1	,	9	,	21	,	33	,	3	,	129	,	327	,	13	,	1943	,	3305	,	6173	,	6903	,	32539	,	9525	};
            const unsigned long	dim4745KuoInit[]		=	{	1	,	1	,	3	,	15	,	23	,	5	,	49	,	125	,	445	,	849	,	547	,	2911	,	7611	,	8137	,	28701	,	6883	};
            const unsigned long	dim4746KuoInit[]		=	{	1	,	1	,	3	,	13	,	11	,	15	,	21	,	35	,	303	,	401	,	187	,	1981	,	7919	,	6179	,	15643	,	10327	};
            const unsigned long	dim4747KuoInit[]		=	{	1	,	1	,	5	,	7	,	9	,	45	,	95	,	205	,	137	,	425	,	353	,	673	,	1337	,	4357	,	12521	,	55229	};
            const unsigned long	dim4748KuoInit[]		=	{	1	,	3	,	5	,	5	,	29	,	47	,	93	,	213	,	357	,	821	,	467	,	395	,	5079	,	10341	,	24117	,	59757	};
            const unsigned long	dim4749KuoInit[]		=	{	1	,	3	,	7	,	5	,	29	,	39	,	25	,	85	,	437	,	873	,	795	,	91	,	1979	,	15503	,	12157	,	6803	};
            const unsigned long	dim4750KuoInit[]		=	{	1	,	3	,	7	,	1	,	31	,	21	,	31	,	245	,	79	,	661	,	323	,	1843	,	2225	,	15383	,	17507	,	30667	};
            const unsigned long	dim4751KuoInit[]		=	{	1	,	3	,	5	,	11	,	1	,	11	,	119	,	151	,	489	,	303	,	835	,	319	,	2071	,	1497	,	10343	,	40641	};
            const unsigned long	dim4752KuoInit[]		=	{	1	,	1	,	1	,	13	,	7	,	7	,	125	,	237	,	103	,	65	,	1917	,	2203	,	5895	,	8651	,	7331	,	27591	};
            const unsigned long	dim4753KuoInit[]		=	{	1	,	3	,	3	,	15	,	21	,	17	,	73	,	21	,	485	,	985	,	253	,	1877	,	7423	,	6569	,	25971	,	57821	};
            const unsigned long	dim4754KuoInit[]		=	{	1	,	3	,	5	,	13	,	3	,	59	,	9	,	77	,	99	,	851	,	343	,	2655	,	367	,	7253	,	16357	,	61563	};
            const unsigned long	dim4755KuoInit[]		=	{	1	,	3	,	7	,	13	,	11	,	1	,	15	,	227	,	223	,	317	,	1111	,	2287	,	4035	,	6803	,	16787	,	27797	};
            const unsigned long	dim4756KuoInit[]		=	{	1	,	3	,	3	,	5	,	17	,	49	,	103	,	191	,	245	,	473	,	2007	,	3297	,	6487	,	7371	,	30497	,	40255	};
            const unsigned long	dim4757KuoInit[]		=	{	1	,	3	,	5	,	13	,	1	,	61	,	39	,	1	,	501	,	665	,	1085	,	3933	,	855	,	883	,	26949	,	48761	};
            const unsigned long	dim4758KuoInit[]		=	{	1	,	3	,	3	,	13	,	29	,	39	,	5	,	43	,	39	,	529	,	1605	,	2235	,	5081	,	7467	,	20727	,	48611	};
            const unsigned long	dim4759KuoInit[]		=	{	1	,	1	,	3	,	11	,	19	,	61	,	89	,	15	,	149	,	639	,	337	,	2953	,	2229	,	11161	,	21981	,	26003	};
            const unsigned long	dim4760KuoInit[]		=	{	1	,	1	,	3	,	5	,	29	,	47	,	71	,	65	,	107	,	485	,	57	,	2255	,	4383	,	12383	,	18161	,	40233	};
            const unsigned long	dim4761KuoInit[]		=	{	1	,	1	,	7	,	3	,	21	,	41	,	75	,	133	,	141	,	343	,	633	,	2043	,	2441	,	5479	,	2493	,	45707	};
            const unsigned long	dim4762KuoInit[]		=	{	1	,	1	,	7	,	15	,	21	,	37	,	19	,	73	,	361	,	195	,	1007	,	1265	,	4075	,	1383	,	11809	,	64265	};
            const unsigned long	dim4763KuoInit[]		=	{	1	,	1	,	5	,	11	,	19	,	63	,	21	,	163	,	349	,	961	,	1221	,	2265	,	4801	,	14049	,	8655	,	32823	};
            const unsigned long	dim4764KuoInit[]		=	{	1	,	1	,	5	,	1	,	9	,	1	,	101	,	37	,	473	,	293	,	1191	,	791	,	5327	,	13739	,	21761	,	5755	};
            const unsigned long	dim4765KuoInit[]		=	{	1	,	1	,	1	,	15	,	23	,	57	,	63	,	195	,	451	,	627	,	1725	,	2405	,	6943	,	6855	,	31563	,	44699	};
            const unsigned long	dim4766KuoInit[]		=	{	1	,	3	,	7	,	1	,	31	,	41	,	51	,	177	,	95	,	961	,	1407	,	2847	,	7	,	8975	,	4017	,	46229	};
            const unsigned long	dim4767KuoInit[]		=	{	1	,	1	,	3	,	13	,	19	,	9	,	67	,	183	,	255	,	1	,	1769	,	2585	,	2157	,	9747	,	32047	,	13063	};
            const unsigned long	dim4768KuoInit[]		=	{	1	,	3	,	3	,	1	,	21	,	39	,	31	,	117	,	397	,	441	,	1443	,	3849	,	5993	,	13981	,	23579	,	40637	};
            const unsigned long	dim4769KuoInit[]		=	{	1	,	1	,	7	,	1	,	31	,	23	,	61	,	203	,	111	,	273	,	1693	,	3031	,	4125	,	1527	,	30525	,	17465	};
            const unsigned long	dim4770KuoInit[]		=	{	1	,	3	,	5	,	9	,	19	,	39	,	127	,	51	,	419	,	205	,	425	,	1939	,	3703	,	12009	,	6291	,	48639	};
            const unsigned long	dim4771KuoInit[]		=	{	1	,	3	,	5	,	13	,	15	,	27	,	63	,	239	,	483	,	181	,	1621	,	3789	,	1929	,	11115	,	29385	,	53075	};
            const unsigned long	dim4772KuoInit[]		=	{	1	,	1	,	1	,	9	,	13	,	9	,	61	,	15	,	221	,	163	,	1645	,	419	,	639	,	15081	,	2097	,	42133	};
            const unsigned long	dim4773KuoInit[]		=	{	1	,	3	,	1	,	7	,	7	,	31	,	27	,	141	,	215	,	645	,	1293	,	2237	,	7129	,	12619	,	14563	,	46081	};
            const unsigned long	dim4774KuoInit[]		=	{	1	,	3	,	1	,	13	,	3	,	41	,	85	,	37	,	295	,	815	,	1417	,	3083	,	5873	,	2037	,	29909	,	3817	};
            const unsigned long	dim4775KuoInit[]		=	{	1	,	3	,	3	,	9	,	11	,	49	,	77	,	103	,	189	,	319	,	439	,	3547	,	173	,	1105	,	12125	,	49989	};
            const unsigned long	dim4776KuoInit[]		=	{	1	,	3	,	1	,	15	,	21	,	9	,	5	,	37	,	87	,	905	,	145	,	1151	,	2213	,	4157	,	16171	,	56121	};
            const unsigned long	dim4777KuoInit[]		=	{	1	,	3	,	5	,	3	,	15	,	21	,	91	,	103	,	367	,	423	,	101	,	327	,	3819	,	3141	,	3515	,	11433	};
            const unsigned long	dim4778KuoInit[]		=	{	1	,	3	,	1	,	13	,	13	,	11	,	61	,	175	,	265	,	1011	,	1825	,	937	,	983	,	9575	,	6053	,	19633	};
            const unsigned long	dim4779KuoInit[]		=	{	1	,	3	,	3	,	9	,	25	,	47	,	55	,	233	,	397	,	325	,	1217	,	1691	,	2751	,	5339	,	14269	,	28791	};
            const unsigned long	dim4780KuoInit[]		=	{	1	,	1	,	7	,	15	,	15	,	51	,	29	,	215	,	47	,	713	,	1879	,	1981	,	3087	,	9945	,	8959	,	59367	};
            const unsigned long	dim4781KuoInit[]		=	{	1	,	1	,	1	,	9	,	1	,	33	,	97	,	7	,	247	,	893	,	655	,	3137	,	6593	,	7261	,	11829	,	22101	};
            const unsigned long	dim4782KuoInit[]		=	{	1	,	1	,	1	,	7	,	11	,	23	,	75	,	55	,	313	,	533	,	529	,	1929	,	3647	,	3317	,	2987	,	11917	};
            const unsigned long	dim4783KuoInit[]		=	{	1	,	1	,	1	,	11	,	17	,	35	,	105	,	221	,	211	,	545	,	1595	,	1949	,	1895	,	39	,	7163	,	33303	};
            const unsigned long	dim4784KuoInit[]		=	{	1	,	1	,	1	,	9	,	21	,	29	,	73	,	235	,	189	,	11	,	1747	,	1023	,	3987	,	10337	,	1689	,	64043	};
            const unsigned long	dim4785KuoInit[]		=	{	1	,	3	,	1	,	5	,	11	,	17	,	115	,	57	,	465	,	899	,	1421	,	1329	,	6099	,	8081	,	11297	,	13217	};
            const unsigned long	dim4786KuoInit[]		=	{	1	,	3	,	3	,	9	,	5	,	23	,	53	,	23	,	11	,	177	,	817	,	3003	,	5073	,	12071	,	11263	,	43159	};
            const unsigned long	dim4787KuoInit[]		=	{	1	,	1	,	3	,	13	,	1	,	63	,	127	,	41	,	303	,	681	,	1559	,	2385	,	6265	,	2417	,	7063	,	57965	};
            const unsigned long	dim4788KuoInit[]		=	{	1	,	3	,	1	,	1	,	3	,	21	,	21	,	37	,	485	,	771	,	2043	,	2065	,	7047	,	8867	,	7019	,	31437	};
            const unsigned long	dim4789KuoInit[]		=	{	1	,	3	,	1	,	11	,	27	,	49	,	77	,	89	,	391	,	623	,	1675	,	1919	,	3575	,	2187	,	22801	,	11059	};
            const unsigned long	dim4790KuoInit[]		=	{	1	,	1	,	3	,	5	,	9	,	57	,	51	,	255	,	287	,	851	,	1999	,	811	,	7769	,	11185	,	19279	,	65491	};
            const unsigned long	dim4791KuoInit[]		=	{	1	,	3	,	1	,	15	,	31	,	43	,	113	,	11	,	461	,	95	,	841	,	2937	,	3969	,	5901	,	32681	,	9231	};
            const unsigned long	dim4792KuoInit[]		=	{	1	,	3	,	3	,	5	,	31	,	47	,	21	,	187	,	171	,	249	,	787	,	459	,	6897	,	12131	,	14843	,	11805	};
            const unsigned long	dim4793KuoInit[]		=	{	1	,	1	,	7	,	13	,	27	,	61	,	91	,	77	,	9	,	393	,	1231	,	1901	,	291	,	1487	,	22139	,	30569	};
            const unsigned long	dim4794KuoInit[]		=	{	1	,	1	,	1	,	1	,	27	,	33	,	55	,	151	,	89	,	603	,	551	,	3767	,	695	,	15225	,	9665	,	60459	};
            const unsigned long	dim4795KuoInit[]		=	{	1	,	1	,	7	,	13	,	31	,	19	,	83	,	27	,	381	,	105	,	481	,	3095	,	137	,	10433	,	12967	,	19555	};
            const unsigned long	dim4796KuoInit[]		=	{	1	,	1	,	1	,	11	,	15	,	1	,	3	,	121	,	107	,	435	,	1571	,	3205	,	5213	,	3689	,	31365	,	14353	};
            const unsigned long	dim4797KuoInit[]		=	{	1	,	3	,	7	,	11	,	23	,	61	,	93	,	59	,	179	,	361	,	965	,	413	,	7757	,	15779	,	1119	,	34673	};
            const unsigned long	dim4798KuoInit[]		=	{	1	,	1	,	3	,	13	,	29	,	63	,	79	,	135	,	431	,	723	,	727	,	2685	,	1775	,	9027	,	27051	,	42581	};
            const unsigned long	dim4799KuoInit[]		=	{	1	,	3	,	5	,	9	,	27	,	19	,	17	,	141	,	367	,	1021	,	651	,	2549	,	4003	,	15737	,	22063	,	12443	};
            const unsigned long	dim4800KuoInit[]		=	{	1	,	1	,	1	,	11	,	23	,	27	,	35	,	225	,	399	,	173	,	451	,	1701	,	1093	,	5639	,	1855	,	1817	};
            const unsigned long	dim4801KuoInit[]		=	{	1	,	3	,	7	,	11	,	23	,	63	,	119	,	13	,	147	,	997	,	53	,	3127	,	101	,	6633	,	14377	,	61843	};
            const unsigned long	dim4802KuoInit[]		=	{	1	,	1	,	1	,	15	,	23	,	21	,	65	,	213	,	311	,	789	,	1395	,	1239	,	241	,	15041	,	14683	,	27805	};
            const unsigned long	dim4803KuoInit[]		=	{	1	,	3	,	3	,	9	,	7	,	29	,	83	,	153	,	231	,	337	,	153	,	3097	,	6137	,	11003	,	31091	,	57005	};
            const unsigned long	dim4804KuoInit[]		=	{	1	,	3	,	7	,	9	,	19	,	33	,	91	,	235	,	123	,	747	,	1331	,	2307	,	3791	,	2063	,	23613	,	3589	};
            const unsigned long	dim4805KuoInit[]		=	{	1	,	3	,	3	,	15	,	9	,	25	,	95	,	237	,	381	,	349	,	1339	,	3187	,	2387	,	3487	,	4149	,	39343	};
            const unsigned long	dim4806KuoInit[]		=	{	1	,	1	,	1	,	13	,	5	,	3	,	43	,	193	,	431	,	853	,	1419	,	905	,	2395	,	11459	,	26109	,	31277	};
            const unsigned long	dim4807KuoInit[]		=	{	1	,	3	,	1	,	15	,	13	,	29	,	13	,	253	,	475	,	81	,	1199	,	841	,	7385	,	2609	,	10445	,	11151	};
            const unsigned long	dim4808KuoInit[]		=	{	1	,	1	,	7	,	15	,	29	,	11	,	81	,	91	,	287	,	615	,	949	,	2541	,	5059	,	12567	,	9547	,	45647	};
            const unsigned long	dim4809KuoInit[]		=	{	1	,	3	,	7	,	13	,	9	,	27	,	123	,	165	,	249	,	457	,	607	,	1203	,	987	,	4477	,	21205	,	44547	};
            const unsigned long	dim4810KuoInit[]		=	{	1	,	1	,	3	,	15	,	3	,	1	,	37	,	43	,	445	,	205	,	1849	,	3461	,	7667	,	209	,	8773	,	1507	};
            const unsigned long	dim4811KuoInit[]		=	{	1	,	3	,	1	,	3	,	27	,	43	,	75	,	199	,	185	,	249	,	725	,	301	,	2181	,	6859	,	16825	,	3471	};
            const unsigned long	dim4812KuoInit[]		=	{	1	,	3	,	1	,	9	,	29	,	5	,	55	,	5	,	345	,	325	,	1853	,	3925	,	1739	,	14991	,	27845	,	6931	};
            const unsigned long	dim4813KuoInit[]		=	{	1	,	1	,	3	,	5	,	31	,	15	,	33	,	47	,	201	,	649	,	415	,	639	,	391	,	12065	,	17343	,	3063	};
            const unsigned long	dim4814KuoInit[]		=	{	1	,	1	,	7	,	11	,	19	,	37	,	39	,	77	,	367	,	29	,	1251	,	2637	,	5601	,	10661	,	17647	,	22131	};
            const unsigned long	dim4815KuoInit[]		=	{	1	,	1	,	7	,	15	,	29	,	51	,	13	,	129	,	279	,	427	,	1535	,	2185	,	2651	,	8251	,	14857	,	61467	};
            const unsigned long	dim4816KuoInit[]		=	{	1	,	1	,	7	,	5	,	25	,	55	,	45	,	105	,	213	,	31	,	735	,	3795	,	6627	,	14299	,	3155	,	3659	};
            const unsigned long	dim4817KuoInit[]		=	{	1	,	1	,	1	,	9	,	21	,	27	,	21	,	85	,	315	,	793	,	1663	,	437	,	4113	,	4639	,	11321	,	13633	};
            const unsigned long	dim4818KuoInit[]		=	{	1	,	3	,	5	,	9	,	23	,	17	,	49	,	35	,	451	,	799	,	1451	,	539	,	4549	,	43	,	1901	,	21155	};
            const unsigned long	dim4819KuoInit[]		=	{	1	,	3	,	7	,	9	,	23	,	53	,	25	,	71	,	371	,	1005	,	129	,	157	,	2977	,	15823	,	30393	,	55629	};
            const unsigned long	dim4820KuoInit[]		=	{	1	,	1	,	7	,	7	,	9	,	27	,	79	,	101	,	147	,	511	,	199	,	2843	,	101	,	14975	,	3455	,	843	};
            const unsigned long	dim4821KuoInit[]		=	{	1	,	3	,	1	,	7	,	11	,	33	,	43	,	7	,	445	,	501	,	1071	,	1851	,	5763	,	5233	,	16121	,	23357	};
            const unsigned long	dim4822KuoInit[]		=	{	1	,	1	,	1	,	13	,	11	,	21	,	75	,	153	,	421	,	311	,	1709	,	3745	,	2971	,	11219	,	23107	,	40057	};
            const unsigned long	dim4823KuoInit[]		=	{	1	,	1	,	3	,	15	,	17	,	39	,	17	,	165	,	103	,	31	,	575	,	3945	,	1921	,	11305	,	13417	,	20603	};
            const unsigned long	dim4824KuoInit[]		=	{	1	,	3	,	3	,	3	,	5	,	49	,	37	,	61	,	177	,	595	,	1133	,	1883	,	819	,	5137	,	10423	,	42009	};
            const unsigned long	dim4825KuoInit[]		=	{	1	,	1	,	3	,	5	,	5	,	15	,	89	,	191	,	97	,	1017	,	205	,	4037	,	3431	,	13877	,	29867	,	26041	};
            const unsigned long	dim4826KuoInit[]		=	{	1	,	3	,	3	,	7	,	9	,	9	,	109	,	157	,	357	,	815	,	1539	,	3507	,	1379	,	1105	,	14387	,	46689	};
            const unsigned long	dim4827KuoInit[]		=	{	1	,	3	,	3	,	9	,	3	,	11	,	77	,	161	,	473	,	619	,	741	,	2945	,	5123	,	3325	,	8679	,	36167	};
            const unsigned long	dim4828KuoInit[]		=	{	1	,	1	,	1	,	9	,	9	,	53	,	3	,	77	,	319	,	223	,	875	,	425	,	1369	,	509	,	25895	,	32803	};
            const unsigned long	dim4829KuoInit[]		=	{	1	,	1	,	3	,	13	,	31	,	15	,	113	,	133	,	473	,	29	,	1009	,	1385	,	871	,	15785	,	1015	,	42627	};
            const unsigned long	dim4830KuoInit[]		=	{	1	,	3	,	7	,	13	,	5	,	53	,	123	,	7	,	465	,	485	,	1413	,	1663	,	6541	,	13051	,	12251	,	36151	};
            const unsigned long	dim4831KuoInit[]		=	{	1	,	1	,	1	,	13	,	9	,	21	,	123	,	223	,	483	,	849	,	489	,	3905	,	4697	,	7583	,	5373	,	62397	};
            const unsigned long	dim4832KuoInit[]		=	{	1	,	1	,	7	,	9	,	29	,	53	,	85	,	151	,	511	,	917	,	1719	,	937	,	2035	,	6385	,	23529	,	62213	};
            const unsigned long	dim4833KuoInit[]		=	{	1	,	1	,	3	,	15	,	11	,	47	,	111	,	79	,	377	,	585	,	475	,	3201	,	7025	,	1483	,	8815	,	47235	};
            const unsigned long	dim4834KuoInit[]		=	{	1	,	1	,	3	,	13	,	13	,	5	,	39	,	31	,	71	,	39	,	941	,	2215	,	2195	,	2827	,	24353	,	18435	};
            const unsigned long	dim4835KuoInit[]		=	{	1	,	1	,	7	,	15	,	13	,	31	,	31	,	15	,	421	,	81	,	895	,	1345	,	7049	,	4373	,	28005	,	41781	};
            const unsigned long	dim4836KuoInit[]		=	{	1	,	1	,	3	,	15	,	17	,	57	,	21	,	175	,	453	,	755	,	1295	,	353	,	5257	,	15901	,	15643	,	31725	};
            const unsigned long	dim4837KuoInit[]		=	{	1	,	1	,	1	,	9	,	17	,	17	,	71	,	209	,	155	,	511	,	1461	,	3313	,	7515	,	8407	,	9561	,	48169	};
            const unsigned long	dim4838KuoInit[]		=	{	1	,	3	,	7	,	15	,	23	,	59	,	89	,	39	,	85	,	857	,	1285	,	3099	,	6497	,	5501	,	21321	,	34319	};
            const unsigned long	dim4839KuoInit[]		=	{	1	,	3	,	5	,	5	,	27	,	13	,	41	,	1	,	191	,	275	,	1567	,	895	,	13	,	673	,	10079	,	55075	};
            const unsigned long	dim4840KuoInit[]		=	{	1	,	1	,	7	,	15	,	9	,	33	,	33	,	35	,	55	,	597	,	365	,	3205	,	6427	,	837	,	16011	,	23279	};
            const unsigned long	dim4841KuoInit[]		=	{	1	,	3	,	5	,	11	,	23	,	23	,	9	,	79	,	383	,	303	,	1715	,	1651	,	7507	,	14553	,	30715	,	56659	};
            const unsigned long	dim4842KuoInit[]		=	{	1	,	3	,	1	,	11	,	1	,	61	,	31	,	85	,	181	,	593	,	1783	,	901	,	6069	,	1521	,	13275	,	18057	};
            const unsigned long	dim4843KuoInit[]		=	{	1	,	3	,	1	,	13	,	23	,	59	,	111	,	221	,	47	,	599	,	193	,	1547	,	4077	,	15271	,	19315	,	1115	};
            const unsigned long	dim4844KuoInit[]		=	{	1	,	3	,	7	,	7	,	23	,	53	,	115	,	69	,	19	,	409	,	1041	,	3395	,	6429	,	9263	,	21637	,	35533	};
            const unsigned long	dim4845KuoInit[]		=	{	1	,	1	,	7	,	1	,	31	,	45	,	43	,	251	,	163	,	907	,	761	,	2009	,	3807	,	1529	,	12001	,	38823	};
            const unsigned long	dim4846KuoInit[]		=	{	1	,	3	,	5	,	3	,	3	,	59	,	87	,	247	,	179	,	931	,	709	,	53	,	6283	,	13723	,	15407	,	22417	};
            const unsigned long	dim4847KuoInit[]		=	{	1	,	1	,	1	,	5	,	27	,	17	,	21	,	183	,	393	,	177	,	1187	,	19	,	5409	,	2539	,	17401	,	29287	};
            const unsigned long	dim4848KuoInit[]		=	{	1	,	1	,	5	,	15	,	3	,	51	,	91	,	95	,	389	,	855	,	1453	,	2815	,	7507	,	5449	,	12459	,	59111	};
            const unsigned long	dim4849KuoInit[]		=	{	1	,	1	,	5	,	3	,	7	,	53	,	101	,	255	,	395	,	345	,	915	,	1649	,	6641	,	7771	,	20747	,	17067	};
            const unsigned long	dim4850KuoInit[]		=	{	1	,	3	,	1	,	7	,	5	,	35	,	69	,	63	,	249	,	189	,	1937	,	965	,	3463	,	13615	,	12449	,	11891	};
            const unsigned long	dim4851KuoInit[]		=	{	1	,	3	,	3	,	9	,	5	,	59	,	57	,	187	,	403	,	825	,	1291	,	2995	,	6365	,	15995	,	6871	,	25785	};
            const unsigned long	dim4852KuoInit[]		=	{	1	,	3	,	5	,	15	,	17	,	23	,	117	,	103	,	131	,	601	,	1259	,	1331	,	3039	,	4761	,	8105	,	14521	};
            const unsigned long	dim4853KuoInit[]		=	{	1	,	1	,	3	,	15	,	27	,	51	,	97	,	221	,	345	,	615	,	487	,	2535	,	139	,	16233	,	16271	,	713	};
            const unsigned long	dim4854KuoInit[]		=	{	1	,	3	,	7	,	13	,	25	,	49	,	99	,	71	,	353	,	1015	,	1889	,	689	,	1925	,	6523	,	30995	,	23057	};
            const unsigned long	dim4855KuoInit[]		=	{	1	,	3	,	5	,	9	,	21	,	55	,	11	,	29	,	477	,	581	,	1719	,	2603	,	1757	,	3995	,	27025	,	62833	};
            const unsigned long	dim4856KuoInit[]		=	{	1	,	3	,	1	,	9	,	9	,	39	,	9	,	121	,	459	,	991	,	2021	,	1469	,	2399	,	4441	,	11829	,	36865	};
            const unsigned long	dim4857KuoInit[]		=	{	1	,	3	,	1	,	5	,	9	,	13	,	115	,	27	,	487	,	307	,	453	,	351	,	885	,	12539	,	39	,	33887	};
            const unsigned long	dim4858KuoInit[]		=	{	1	,	3	,	1	,	7	,	23	,	35	,	87	,	131	,	129	,	1	,	2015	,	3921	,	1309	,	7915	,	16661	,	45945	};
            const unsigned long	dim4859KuoInit[]		=	{	1	,	3	,	1	,	3	,	1	,	15	,	93	,	255	,	285	,	915	,	563	,	527	,	7763	,	4581	,	11161	,	64619	};
            const unsigned long	dim4860KuoInit[]		=	{	1	,	3	,	3	,	9	,	31	,	37	,	87	,	81	,	313	,	647	,	1065	,	863	,	6121	,	1385	,	26171	,	56323	};
            const unsigned long	dim4861KuoInit[]		=	{	1	,	1	,	3	,	7	,	25	,	49	,	23	,	107	,	507	,	511	,	951	,	669	,	67	,	14319	,	17669	,	32497	};
            const unsigned long	dim4862KuoInit[]		=	{	1	,	3	,	1	,	13	,	9	,	33	,	93	,	171	,	457	,	687	,	703	,	1073	,	3699	,	7029	,	28983	,	14501	};
            const unsigned long	dim4863KuoInit[]		=	{	1	,	1	,	7	,	13	,	29	,	3	,	19	,	173	,	131	,	285	,	469	,	1223	,	7779	,	15367	,	643	,	63715	};
            const unsigned long	dim4864KuoInit[]		=	{	1	,	3	,	7	,	5	,	5	,	37	,	21	,	223	,	227	,	939	,	1205	,	3167	,	5535	,	11543	,	7411	,	30841	};
            const unsigned long	dim4865KuoInit[]		=	{	1	,	3	,	5	,	11	,	31	,	37	,	75	,	39	,	171	,	135	,	1487	,	3503	,	7939	,	77	,	19745	,	2551	};
            const unsigned long	dim4866KuoInit[]		=	{	1	,	3	,	5	,	13	,	25	,	33	,	79	,	213	,	289	,	27	,	1435	,	1839	,	3089	,	8173	,	27555	,	20599	};
            const unsigned long	dim4867KuoInit[]		=	{	1	,	1	,	3	,	13	,	19	,	31	,	67	,	9	,	229	,	591	,	561	,	3797	,	1833	,	3311	,	16053	,	61183	};
            const unsigned long	dim4868KuoInit[]		=	{	1	,	1	,	3	,	3	,	27	,	27	,	79	,	95	,	7	,	1023	,	517	,	3627	,	4117	,	8165	,	17961	,	39011	};
            const unsigned long	dim4869KuoInit[]		=	{	1	,	3	,	5	,	3	,	3	,	43	,	37	,	231	,	389	,	367	,	517	,	3117	,	2643	,	2103	,	127	,	42669	};
            const unsigned long	dim4870KuoInit[]		=	{	1	,	1	,	1	,	11	,	11	,	49	,	35	,	71	,	295	,	87	,	1653	,	1899	,	1177	,	16071	,	8429	,	54751	};
            const unsigned long	dim4871KuoInit[]		=	{	1	,	1	,	7	,	13	,	5	,	47	,	7	,	229	,	261	,	837	,	645	,	1581	,	159	,	73	,	10233	,	18557	};
            const unsigned long	dim4872KuoInit[]		=	{	1	,	3	,	1	,	9	,	11	,	45	,	25	,	3	,	271	,	1005	,	1193	,	507	,	269	,	14689	,	7601	,	56781	};
            const unsigned long	dim4873KuoInit[]		=	{	1	,	1	,	5	,	11	,	13	,	61	,	107	,	29	,	155	,	415	,	1435	,	1979	,	7339	,	1839	,	21097	,	5851	};
            const unsigned long	dim4874KuoInit[]		=	{	1	,	1	,	5	,	9	,	9	,	27	,	83	,	163	,	15	,	539	,	765	,	463	,	7493	,	1413	,	15597	,	42819	};
            const unsigned long	dim4875KuoInit[]		=	{	1	,	3	,	1	,	9	,	21	,	7	,	91	,	213	,	463	,	827	,	1847	,	2825	,	2115	,	14549	,	29923	,	61355	};
            const unsigned long	dim4876KuoInit[]		=	{	1	,	1	,	3	,	15	,	1	,	55	,	69	,	85	,	7	,	847	,	1935	,	3225	,	693	,	12069	,	26303	,	19143	};
            const unsigned long	dim4877KuoInit[]		=	{	1	,	3	,	1	,	11	,	15	,	47	,	53	,	181	,	327	,	493	,	483	,	999	,	4421	,	6707	,	27933	,	11533	};
            const unsigned long	dim4878KuoInit[]		=	{	1	,	3	,	3	,	3	,	23	,	7	,	7	,	63	,	235	,	493	,	937	,	2935	,	2869	,	14243	,	30975	,	12881	};
            const unsigned long	dim4879KuoInit[]		=	{	1	,	1	,	3	,	9	,	9	,	7	,	113	,	203	,	297	,	183	,	1687	,	3211	,	8131	,	7033	,	15007	,	1307	};
            const unsigned long	dim4880KuoInit[]		=	{	1	,	3	,	1	,	9	,	9	,	39	,	99	,	5	,	61	,	435	,	427	,	2719	,	7741	,	4267	,	2423	,	37573	};
            const unsigned long	dim4881KuoInit[]		=	{	1	,	3	,	7	,	3	,	17	,	1	,	105	,	13	,	89	,	693	,	1835	,	2455	,	1135	,	15545	,	2447	,	25673	};
            const unsigned long	dim4882KuoInit[]		=	{	1	,	3	,	5	,	5	,	15	,	47	,	103	,	167	,	21	,	791	,	1297	,	3523	,	4427	,	7013	,	1845	,	44461	};
            const unsigned long	dim4883KuoInit[]		=	{	1	,	1	,	5	,	7	,	11	,	17	,	55	,	247	,	317	,	229	,	1465	,	3781	,	2397	,	10541	,	8943	,	29155	};
            const unsigned long	dim4884KuoInit[]		=	{	1	,	1	,	5	,	3	,	17	,	17	,	97	,	141	,	153	,	883	,	449	,	1163	,	3165	,	125	,	4689	,	44461	};
            const unsigned long	dim4885KuoInit[]		=	{	1	,	3	,	7	,	1	,	17	,	13	,	9	,	235	,	87	,	251	,	57	,	2769	,	4845	,	5733	,	4305	,	49489	};
            const unsigned long	dim4886KuoInit[]		=	{	1	,	1	,	7	,	1	,	9	,	41	,	79	,	97	,	201	,	789	,	37	,	2593	,	8023	,	139	,	30623	,	7667	};
            const unsigned long	dim4887KuoInit[]		=	{	1	,	1	,	7	,	7	,	7	,	27	,	9	,	99	,	199	,	163	,	1175	,	729	,	2731	,	4391	,	27597	,	23087	};
            const unsigned long	dim4888KuoInit[]		=	{	1	,	3	,	5	,	5	,	29	,	39	,	5	,	129	,	395	,	1007	,	1751	,	3951	,	5879	,	13105	,	26223	,	31361	};
            const unsigned long	dim4889KuoInit[]		=	{	1	,	3	,	5	,	3	,	3	,	55	,	89	,	253	,	207	,	553	,	1407	,	2277	,	6879	,	16027	,	23477	,	43633	};
            const unsigned long	dim4890KuoInit[]		=	{	1	,	1	,	1	,	3	,	13	,	21	,	9	,	149	,	31	,	359	,	1631	,	2199	,	2937	,	15663	,	5899	,	59959	};
            const unsigned long	dim4891KuoInit[]		=	{	1	,	1	,	1	,	7	,	25	,	63	,	87	,	13	,	179	,	237	,	359	,	3841	,	6467	,	5621	,	29459	,	31339	};
            const unsigned long	dim4892KuoInit[]		=	{	1	,	1	,	1	,	15	,	25	,	45	,	77	,	17	,	419	,	759	,	1045	,	1285	,	6739	,	11035	,	11401	,	44821	};
            const unsigned long	dim4893KuoInit[]		=	{	1	,	3	,	7	,	1	,	31	,	47	,	17	,	237	,	69	,	389	,	1251	,	1205	,	833	,	2313	,	17005	,	32683	};
            const unsigned long	dim4894KuoInit[]		=	{	1	,	1	,	7	,	9	,	13	,	19	,	51	,	69	,	261	,	85	,	215	,	2215	,	1643	,	12985	,	10363	,	17123	};
            const unsigned long	dim4895KuoInit[]		=	{	1	,	3	,	1	,	3	,	3	,	51	,	41	,	173	,	161	,	633	,	1727	,	3089	,	7791	,	7367	,	32641	,	14361	};
            const unsigned long	dim4896KuoInit[]		=	{	1	,	3	,	5	,	3	,	21	,	49	,	19	,	243	,	427	,	589	,	1699	,	2187	,	4177	,	927	,	6201	,	46319	};
            const unsigned long	dim4897KuoInit[]		=	{	1	,	1	,	3	,	11	,	1	,	59	,	49	,	235	,	345	,	731	,	793	,	1893	,	4735	,	10647	,	31169	,	12051	};
            const unsigned long	dim4898KuoInit[]		=	{	1	,	1	,	7	,	15	,	23	,	51	,	67	,	151	,	253	,	793	,	1673	,	3255	,	4919	,	11309	,	24903	,	9083	};
            const unsigned long	dim4899KuoInit[]		=	{	1	,	3	,	3	,	9	,	7	,	59	,	41	,	225	,	397	,	671	,	513	,	1351	,	2835	,	15789	,	17679	,	28641	};
            const unsigned long	dim4900KuoInit[]		=	{	1	,	3	,	1	,	7	,	17	,	39	,	49	,	103	,	181	,	375	,	1843	,	1183	,	4219	,	2931	,	18385	,	1203	};
            const unsigned long	dim4901KuoInit[]		=	{	1	,	1	,	1	,	3	,	29	,	19	,	113	,	55	,	81	,	69	,	1541	,	3249	,	643	,	841	,	6053	,	63571	};
            const unsigned long	dim4902KuoInit[]		=	{	1	,	1	,	7	,	5	,	1	,	3	,	39	,	237	,	283	,	283	,	813	,	3539	,	5899	,	8489	,	18393	,	22371	};
            const unsigned long	dim4903KuoInit[]		=	{	1	,	3	,	7	,	13	,	5	,	53	,	15	,	129	,	95	,	181	,	657	,	3735	,	7517	,	2435	,	17245	,	4075	};
            const unsigned long	dim4904KuoInit[]		=	{	1	,	1	,	5	,	3	,	9	,	37	,	73	,	95	,	175	,	989	,	1443	,	1745	,	8117	,	5867	,	29729	,	28441	};
            const unsigned long	dim4905KuoInit[]		=	{	1	,	3	,	3	,	11	,	5	,	41	,	25	,	3	,	177	,	735	,	187	,	357	,	5365	,	8161	,	2071	,	41651	};
            const unsigned long	dim4906KuoInit[]		=	{	1	,	1	,	1	,	15	,	29	,	47	,	27	,	233	,	177	,	339	,	1839	,	1761	,	5483	,	7203	,	32411	,	59859	};
            const unsigned long	dim4907KuoInit[]		=	{	1	,	3	,	7	,	9	,	13	,	59	,	25	,	17	,	157	,	247	,	697	,	4011	,	4539	,	4253	,	1381	,	63009	};
            const unsigned long	dim4908KuoInit[]		=	{	1	,	1	,	5	,	13	,	19	,	21	,	19	,	161	,	325	,	389	,	1067	,	525	,	529	,	277	,	13775	,	52265	};
            const unsigned long	dim4909KuoInit[]		=	{	1	,	1	,	5	,	13	,	21	,	17	,	77	,	191	,	329	,	627	,	1857	,	1705	,	2207	,	5329	,	29833	,	1523	};
            const unsigned long	dim4910KuoInit[]		=	{	1	,	3	,	3	,	1	,	21	,	3	,	107	,	85	,	29	,	887	,	793	,	1089	,	1361	,	10361	,	15869	,	1659	};
            const unsigned long	dim4911KuoInit[]		=	{	1	,	1	,	1	,	15	,	11	,	47	,	85	,	137	,	373	,	777	,	875	,	2735	,	4381	,	8831	,	5891	,	44809	};
            const unsigned long	dim4912KuoInit[]		=	{	1	,	1	,	1	,	11	,	19	,	9	,	77	,	59	,	497	,	643	,	51	,	3115	,	7267	,	11927	,	28119	,	28151	};
            const unsigned long	dim4913KuoInit[]		=	{	1	,	3	,	1	,	3	,	13	,	27	,	55	,	201	,	319	,	167	,	1435	,	2197	,	5049	,	5923	,	12759	,	62383	};
            const unsigned long	dim4914KuoInit[]		=	{	1	,	3	,	1	,	3	,	23	,	43	,	45	,	193	,	221	,	647	,	1933	,	2915	,	6461	,	4927	,	22105	,	54557	};
            const unsigned long	dim4915KuoInit[]		=	{	1	,	1	,	5	,	11	,	5	,	23	,	67	,	59	,	79	,	507	,	925	,	3015	,	317	,	3887	,	29967	,	17205	};
            const unsigned long	dim4916KuoInit[]		=	{	1	,	3	,	7	,	7	,	19	,	39	,	51	,	159	,	485	,	315	,	1739	,	2617	,	8017	,	11655	,	1141	,	26055	};
            const unsigned long	dim4917KuoInit[]		=	{	1	,	1	,	3	,	1	,	27	,	53	,	119	,	113	,	135	,	85	,	2023	,	1659	,	307	,	8189	,	28339	,	53297	};
            const unsigned long	dim4918KuoInit[]		=	{	1	,	3	,	3	,	5	,	9	,	61	,	57	,	225	,	137	,	381	,	1045	,	1253	,	1185	,	9885	,	30153	,	29355	};
            const unsigned long	dim4919KuoInit[]		=	{	1	,	1	,	3	,	5	,	19	,	9	,	87	,	87	,	393	,	421	,	1941	,	57	,	2483	,	10855	,	26741	,	28091	};
            const unsigned long	dim4920KuoInit[]		=	{	1	,	1	,	7	,	5	,	3	,	35	,	35	,	137	,	123	,	327	,	933	,	3429	,	281	,	1705	,	17123	,	29337	};
            const unsigned long	dim4921KuoInit[]		=	{	1	,	3	,	7	,	13	,	9	,	61	,	111	,	207	,	45	,	57	,	1093	,	2223	,	575	,	11959	,	21147	,	30273	};
            const unsigned long	dim4922KuoInit[]		=	{	1	,	1	,	5	,	5	,	31	,	37	,	123	,	5	,	277	,	273	,	525	,	1773	,	2685	,	11607	,	32079	,	25199	};
            const unsigned long	dim4923KuoInit[]		=	{	1	,	1	,	7	,	11	,	5	,	29	,	121	,	193	,	437	,	417	,	1521	,	3057	,	4681	,	2773	,	5391	,	59747	};
            const unsigned long	dim4924KuoInit[]		=	{	1	,	3	,	7	,	11	,	27	,	3	,	103	,	17	,	395	,	695	,	1731	,	4055	,	2727	,	2171	,	26893	,	7047	};
            const unsigned long	dim4925KuoInit[]		=	{	1	,	3	,	7	,	5	,	23	,	9	,	25	,	167	,	241	,	755	,	1789	,	1977	,	6325	,	5425	,	23247	,	64181	};


            const unsigned long * const Kuoinitializers[4924]
            =
            {
                dim2KuoInit,
                dim3KuoInit,
                dim4KuoInit,
                dim5KuoInit,
                dim6KuoInit,
                dim7KuoInit,
                dim8KuoInit,
                dim9KuoInit,
                dim10KuoInit,
                dim11KuoInit,
                dim12KuoInit,
                dim13KuoInit,
                dim14KuoInit,
                dim15KuoInit,
                dim16KuoInit,
                dim17KuoInit,
                dim18KuoInit,
                dim19KuoInit,
                dim20KuoInit,
                dim21KuoInit,
                dim22KuoInit,
                dim23KuoInit,
                dim24KuoInit,
                dim25KuoInit,
                dim26KuoInit,
                dim27KuoInit,
                dim28KuoInit,
                dim29KuoInit,
                dim30KuoInit,
                dim31KuoInit,
                dim32KuoInit,
                dim33KuoInit,
                dim34KuoInit,
                dim35KuoInit,
                dim36KuoInit,
                dim37KuoInit,
                dim38KuoInit,
                dim39KuoInit,
                dim40KuoInit,
                dim41KuoInit,
                dim42KuoInit,
                dim43KuoInit,
                dim44KuoInit,
                dim45KuoInit,
                dim46KuoInit,
                dim47KuoInit,
                dim48KuoInit,
                dim49KuoInit,
                dim50KuoInit,
                dim51KuoInit,
                dim52KuoInit,
                dim53KuoInit,
                dim54KuoInit,
                dim55KuoInit,
                dim56KuoInit,
                dim57KuoInit,
                dim58KuoInit,
                dim59KuoInit,
                dim60KuoInit,
                dim61KuoInit,
                dim62KuoInit,
                dim63KuoInit,
                dim64KuoInit,
                dim65KuoInit,
                dim66KuoInit,
                dim67KuoInit,
                dim68KuoInit,
                dim69KuoInit,
                dim70KuoInit,
                dim71KuoInit,
                dim72KuoInit,
                dim73KuoInit,
                dim74KuoInit,
                dim75KuoInit,
                dim76KuoInit,
                dim77KuoInit,
                dim78KuoInit,
                dim79KuoInit,
                dim80KuoInit,
                dim81KuoInit,
                dim82KuoInit,
                dim83KuoInit,
                dim84KuoInit,
                dim85KuoInit,
                dim86KuoInit,
                dim87KuoInit,
                dim88KuoInit,
                dim89KuoInit,
                dim90KuoInit,
                dim91KuoInit,
                dim92KuoInit,
                dim93KuoInit,
                dim94KuoInit,
                dim95KuoInit,
                dim96KuoInit,
                dim97KuoInit,
                dim98KuoInit,
                dim99KuoInit,
                dim100KuoInit,
                dim101KuoInit,
                dim102KuoInit,
                dim103KuoInit,
                dim104KuoInit,
                dim105KuoInit,
                dim106KuoInit,
                dim107KuoInit,
                dim108KuoInit,
                dim109KuoInit,
                dim110KuoInit,
                dim111KuoInit,
                dim112KuoInit,
                dim113KuoInit,
                dim114KuoInit,
                dim115KuoInit,
                dim116KuoInit,
                dim117KuoInit,
                dim118KuoInit,
                dim119KuoInit,
                dim120KuoInit,
                dim121KuoInit,
                dim122KuoInit,
                dim123KuoInit,
                dim124KuoInit,
                dim125KuoInit,
                dim126KuoInit,
                dim127KuoInit,
                dim128KuoInit,
                dim129KuoInit,
                dim130KuoInit,
                dim131KuoInit,
                dim132KuoInit,
                dim133KuoInit,
                dim134KuoInit,
                dim135KuoInit,
                dim136KuoInit,
                dim137KuoInit,
                dim138KuoInit,
                dim139KuoInit,
                dim140KuoInit,
                dim141KuoInit,
                dim142KuoInit,
                dim143KuoInit,
                dim144KuoInit,
                dim145KuoInit,
                dim146KuoInit,
                dim147KuoInit,
                dim148KuoInit,
                dim149KuoInit,
                dim150KuoInit,
                dim151KuoInit,
                dim152KuoInit,
                dim153KuoInit,
                dim154KuoInit,
                dim155KuoInit,
                dim156KuoInit,
                dim157KuoInit,
                dim158KuoInit,
                dim159KuoInit,
                dim160KuoInit,
                dim161KuoInit,
                dim162KuoInit,
                dim163KuoInit,
                dim164KuoInit,
                dim165KuoInit,
                dim166KuoInit,
                dim167KuoInit,
                dim168KuoInit,
                dim169KuoInit,
                dim170KuoInit,
                dim171KuoInit,
                dim172KuoInit,
                dim173KuoInit,
                dim174KuoInit,
                dim175KuoInit,
                dim176KuoInit,
                dim177KuoInit,
                dim178KuoInit,
                dim179KuoInit,
                dim180KuoInit,
                dim181KuoInit,
                dim182KuoInit,
                dim183KuoInit,
                dim184KuoInit,
                dim185KuoInit,
                dim186KuoInit,
                dim187KuoInit,
                dim188KuoInit,
                dim189KuoInit,
                dim190KuoInit,
                dim191KuoInit,
                dim192KuoInit,
                dim193KuoInit,
                dim194KuoInit,
                dim195KuoInit,
                dim196KuoInit,
                dim197KuoInit,
                dim198KuoInit,
                dim199KuoInit,
                dim200KuoInit,
                dim201KuoInit,
                dim202KuoInit,
                dim203KuoInit,
                dim204KuoInit,
                dim205KuoInit,
                dim206KuoInit,
                dim207KuoInit,
                dim208KuoInit,
                dim209KuoInit,
                dim210KuoInit,
                dim211KuoInit,
                dim212KuoInit,
                dim213KuoInit,
                dim214KuoInit,
                dim215KuoInit,
                dim216KuoInit,
                dim217KuoInit,
                dim218KuoInit,
                dim219KuoInit,
                dim220KuoInit,
                dim221KuoInit,
                dim222KuoInit,
                dim223KuoInit,
                dim224KuoInit,
                dim225KuoInit,
                dim226KuoInit,
                dim227KuoInit,
                dim228KuoInit,
                dim229KuoInit,
                dim230KuoInit,
                dim231KuoInit,
                dim232KuoInit,
                dim233KuoInit,
                dim234KuoInit,
                dim235KuoInit,
                dim236KuoInit,
                dim237KuoInit,
                dim238KuoInit,
                dim239KuoInit,
                dim240KuoInit,
                dim241KuoInit,
                dim242KuoInit,
                dim243KuoInit,
                dim244KuoInit,
                dim245KuoInit,
                dim246KuoInit,
                dim247KuoInit,
                dim248KuoInit,
                dim249KuoInit,
                dim250KuoInit,
                dim251KuoInit,
                dim252KuoInit,
                dim253KuoInit,
                dim254KuoInit,
                dim255KuoInit,
                dim256KuoInit,
                dim257KuoInit,
                dim258KuoInit,
                dim259KuoInit,
                dim260KuoInit,
                dim261KuoInit,
                dim262KuoInit,
                dim263KuoInit,
                dim264KuoInit,
                dim265KuoInit,
                dim266KuoInit,
                dim267KuoInit,
                dim268KuoInit,
                dim269KuoInit,
                dim270KuoInit,
                dim271KuoInit,
                dim272KuoInit,
                dim273KuoInit,
                dim274KuoInit,
                dim275KuoInit,
                dim276KuoInit,
                dim277KuoInit,
                dim278KuoInit,
                dim279KuoInit,
                dim280KuoInit,
                dim281KuoInit,
                dim282KuoInit,
                dim283KuoInit,
                dim284KuoInit,
                dim285KuoInit,
                dim286KuoInit,
                dim287KuoInit,
                dim288KuoInit,
                dim289KuoInit,
                dim290KuoInit,
                dim291KuoInit,
                dim292KuoInit,
                dim293KuoInit,
                dim294KuoInit,
                dim295KuoInit,
                dim296KuoInit,
                dim297KuoInit,
                dim298KuoInit,
                dim299KuoInit,
                dim300KuoInit,
                dim301KuoInit,
                dim302KuoInit,
                dim303KuoInit,
                dim304KuoInit,
                dim305KuoInit,
                dim306KuoInit,
                dim307KuoInit,
                dim308KuoInit,
                dim309KuoInit,
                dim310KuoInit,
                dim311KuoInit,
                dim312KuoInit,
                dim313KuoInit,
                dim314KuoInit,
                dim315KuoInit,
                dim316KuoInit,
                dim317KuoInit,
                dim318KuoInit,
                dim319KuoInit,
                dim320KuoInit,
                dim321KuoInit,
                dim322KuoInit,
                dim323KuoInit,
                dim324KuoInit,
                dim325KuoInit,
                dim326KuoInit,
                dim327KuoInit,
                dim328KuoInit,
                dim329KuoInit,
                dim330KuoInit,
                dim331KuoInit,
                dim332KuoInit,
                dim333KuoInit,
                dim334KuoInit,
                dim335KuoInit,
                dim336KuoInit,
                dim337KuoInit,
                dim338KuoInit,
                dim339KuoInit,
                dim340KuoInit,
                dim341KuoInit,
                dim342KuoInit,
                dim343KuoInit,
                dim344KuoInit,
                dim345KuoInit,
                dim346KuoInit,
                dim347KuoInit,
                dim348KuoInit,
                dim349KuoInit,
                dim350KuoInit,
                dim351KuoInit,
                dim352KuoInit,
                dim353KuoInit,
                dim354KuoInit,
                dim355KuoInit,
                dim356KuoInit,
                dim357KuoInit,
                dim358KuoInit,
                dim359KuoInit,
                dim360KuoInit,
                dim361KuoInit,
                dim362KuoInit,
                dim363KuoInit,
                dim364KuoInit,
                dim365KuoInit,
                dim366KuoInit,
                dim367KuoInit,
                dim368KuoInit,
                dim369KuoInit,
                dim370KuoInit,
                dim371KuoInit,
                dim372KuoInit,
                dim373KuoInit,
                dim374KuoInit,
                dim375KuoInit,
                dim376KuoInit,
                dim377KuoInit,
                dim378KuoInit,
                dim379KuoInit,
                dim380KuoInit,
                dim381KuoInit,
                dim382KuoInit,
                dim383KuoInit,
                dim384KuoInit,
                dim385KuoInit,
                dim386KuoInit,
                dim387KuoInit,
                dim388KuoInit,
                dim389KuoInit,
                dim390KuoInit,
                dim391KuoInit,
                dim392KuoInit,
                dim393KuoInit,
                dim394KuoInit,
                dim395KuoInit,
                dim396KuoInit,
                dim397KuoInit,
                dim398KuoInit,
                dim399KuoInit,
                dim400KuoInit,
                dim401KuoInit,
                dim402KuoInit,
                dim403KuoInit,
                dim404KuoInit,
                dim405KuoInit,
                dim406KuoInit,
                dim407KuoInit,
                dim408KuoInit,
                dim409KuoInit,
                dim410KuoInit,
                dim411KuoInit,
                dim412KuoInit,
                dim413KuoInit,
                dim414KuoInit,
                dim415KuoInit,
                dim416KuoInit,
                dim417KuoInit,
                dim418KuoInit,
                dim419KuoInit,
                dim420KuoInit,
                dim421KuoInit,
                dim422KuoInit,
                dim423KuoInit,
                dim424KuoInit,
                dim425KuoInit,
                dim426KuoInit,
                dim427KuoInit,
                dim428KuoInit,
                dim429KuoInit,
                dim430KuoInit,
                dim431KuoInit,
                dim432KuoInit,
                dim433KuoInit,
                dim434KuoInit,
                dim435KuoInit,
                dim436KuoInit,
                dim437KuoInit,
                dim438KuoInit,
                dim439KuoInit,
                dim440KuoInit,
                dim441KuoInit,
                dim442KuoInit,
                dim443KuoInit,
                dim444KuoInit,
                dim445KuoInit,
                dim446KuoInit,
                dim447KuoInit,
                dim448KuoInit,
                dim449KuoInit,
                dim450KuoInit,
                dim451KuoInit,
                dim452KuoInit,
                dim453KuoInit,
                dim454KuoInit,
                dim455KuoInit,
                dim456KuoInit,
                dim457KuoInit,
                dim458KuoInit,
                dim459KuoInit,
                dim460KuoInit,
                dim461KuoInit,
                dim462KuoInit,
                dim463KuoInit,
                dim464KuoInit,
                dim465KuoInit,
                dim466KuoInit,
                dim467KuoInit,
                dim468KuoInit,
                dim469KuoInit,
                dim470KuoInit,
                dim471KuoInit,
                dim472KuoInit,
                dim473KuoInit,
                dim474KuoInit,
                dim475KuoInit,
                dim476KuoInit,
                dim477KuoInit,
                dim478KuoInit,
                dim479KuoInit,
                dim480KuoInit,
                dim481KuoInit,
                dim482KuoInit,
                dim483KuoInit,
                dim484KuoInit,
                dim485KuoInit,
                dim486KuoInit,
                dim487KuoInit,
                dim488KuoInit,
                dim489KuoInit,
                dim490KuoInit,
                dim491KuoInit,
                dim492KuoInit,
                dim493KuoInit,
                dim494KuoInit,
                dim495KuoInit,
                dim496KuoInit,
                dim497KuoInit,
                dim498KuoInit,
                dim499KuoInit,
                dim500KuoInit,
                dim501KuoInit,
                dim502KuoInit,
                dim503KuoInit,
                dim504KuoInit,
                dim505KuoInit,
                dim506KuoInit,
                dim507KuoInit,
                dim508KuoInit,
                dim509KuoInit,
                dim510KuoInit,
                dim511KuoInit,
                dim512KuoInit,
                dim513KuoInit,
                dim514KuoInit,
                dim515KuoInit,
                dim516KuoInit,
                dim517KuoInit,
                dim518KuoInit,
                dim519KuoInit,
                dim520KuoInit,
                dim521KuoInit,
                dim522KuoInit,
                dim523KuoInit,
                dim524KuoInit,
                dim525KuoInit,
                dim526KuoInit,
                dim527KuoInit,
                dim528KuoInit,
                dim529KuoInit,
                dim530KuoInit,
                dim531KuoInit,
                dim532KuoInit,
                dim533KuoInit,
                dim534KuoInit,
                dim535KuoInit,
                dim536KuoInit,
                dim537KuoInit,
                dim538KuoInit,
                dim539KuoInit,
                dim540KuoInit,
                dim541KuoInit,
                dim542KuoInit,
                dim543KuoInit,
                dim544KuoInit,
                dim545KuoInit,
                dim546KuoInit,
                dim547KuoInit,
                dim548KuoInit,
                dim549KuoInit,
                dim550KuoInit,
                dim551KuoInit,
                dim552KuoInit,
                dim553KuoInit,
                dim554KuoInit,
                dim555KuoInit,
                dim556KuoInit,
                dim557KuoInit,
                dim558KuoInit,
                dim559KuoInit,
                dim560KuoInit,
                dim561KuoInit,
                dim562KuoInit,
                dim563KuoInit,
                dim564KuoInit,
                dim565KuoInit,
                dim566KuoInit,
                dim567KuoInit,
                dim568KuoInit,
                dim569KuoInit,
                dim570KuoInit,
                dim571KuoInit,
                dim572KuoInit,
                dim573KuoInit,
                dim574KuoInit,
                dim575KuoInit,
                dim576KuoInit,
                dim577KuoInit,
                dim578KuoInit,
                dim579KuoInit,
                dim580KuoInit,
                dim581KuoInit,
                dim582KuoInit,
                dim583KuoInit,
                dim584KuoInit,
                dim585KuoInit,
                dim586KuoInit,
                dim587KuoInit,
                dim588KuoInit,
                dim589KuoInit,
                dim590KuoInit,
                dim591KuoInit,
                dim592KuoInit,
                dim593KuoInit,
                dim594KuoInit,
                dim595KuoInit,
                dim596KuoInit,
                dim597KuoInit,
                dim598KuoInit,
                dim599KuoInit,
                dim600KuoInit,
                dim601KuoInit,
                dim602KuoInit,
                dim603KuoInit,
                dim604KuoInit,
                dim605KuoInit,
                dim606KuoInit,
                dim607KuoInit,
                dim608KuoInit,
                dim609KuoInit,
                dim610KuoInit,
                dim611KuoInit,
                dim612KuoInit,
                dim613KuoInit,
                dim614KuoInit,
                dim615KuoInit,
                dim616KuoInit,
                dim617KuoInit,
                dim618KuoInit,
                dim619KuoInit,
                dim620KuoInit,
                dim621KuoInit,
                dim622KuoInit,
                dim623KuoInit,
                dim624KuoInit,
                dim625KuoInit,
                dim626KuoInit,
                dim627KuoInit,
                dim628KuoInit,
                dim629KuoInit,
                dim630KuoInit,
                dim631KuoInit,
                dim632KuoInit,
                dim633KuoInit,
                dim634KuoInit,
                dim635KuoInit,
                dim636KuoInit,
                dim637KuoInit,
                dim638KuoInit,
                dim639KuoInit,
                dim640KuoInit,
                dim641KuoInit,
                dim642KuoInit,
                dim643KuoInit,
                dim644KuoInit,
                dim645KuoInit,
                dim646KuoInit,
                dim647KuoInit,
                dim648KuoInit,
                dim649KuoInit,
                dim650KuoInit,
                dim651KuoInit,
                dim652KuoInit,
                dim653KuoInit,
                dim654KuoInit,
                dim655KuoInit,
                dim656KuoInit,
                dim657KuoInit,
                dim658KuoInit,
                dim659KuoInit,
                dim660KuoInit,
                dim661KuoInit,
                dim662KuoInit,
                dim663KuoInit,
                dim664KuoInit,
                dim665KuoInit,
                dim666KuoInit,
                dim667KuoInit,
                dim668KuoInit,
                dim669KuoInit,
                dim670KuoInit,
                dim671KuoInit,
                dim672KuoInit,
                dim673KuoInit,
                dim674KuoInit,
                dim675KuoInit,
                dim676KuoInit,
                dim677KuoInit,
                dim678KuoInit,
                dim679KuoInit,
                dim680KuoInit,
                dim681KuoInit,
                dim682KuoInit,
                dim683KuoInit,
                dim684KuoInit,
                dim685KuoInit,
                dim686KuoInit,
                dim687KuoInit,
                dim688KuoInit,
                dim689KuoInit,
                dim690KuoInit,
                dim691KuoInit,
                dim692KuoInit,
                dim693KuoInit,
                dim694KuoInit,
                dim695KuoInit,
                dim696KuoInit,
                dim697KuoInit,
                dim698KuoInit,
                dim699KuoInit,
                dim700KuoInit,
                dim701KuoInit,
                dim702KuoInit,
                dim703KuoInit,
                dim704KuoInit,
                dim705KuoInit,
                dim706KuoInit,
                dim707KuoInit,
                dim708KuoInit,
                dim709KuoInit,
                dim710KuoInit,
                dim711KuoInit,
                dim712KuoInit,
                dim713KuoInit,
                dim714KuoInit,
                dim715KuoInit,
                dim716KuoInit,
                dim717KuoInit,
                dim718KuoInit,
                dim719KuoInit,
                dim720KuoInit,
                dim721KuoInit,
                dim722KuoInit,
                dim723KuoInit,
                dim724KuoInit,
                dim725KuoInit,
                dim726KuoInit,
                dim727KuoInit,
                dim728KuoInit,
                dim729KuoInit,
                dim730KuoInit,
                dim731KuoInit,
                dim732KuoInit,
                dim733KuoInit,
                dim734KuoInit,
                dim735KuoInit,
                dim736KuoInit,
                dim737KuoInit,
                dim738KuoInit,
                dim739KuoInit,
                dim740KuoInit,
                dim741KuoInit,
                dim742KuoInit,
                dim743KuoInit,
                dim744KuoInit,
                dim745KuoInit,
                dim746KuoInit,
                dim747KuoInit,
                dim748KuoInit,
                dim749KuoInit,
                dim750KuoInit,
                dim751KuoInit,
                dim752KuoInit,
                dim753KuoInit,
                dim754KuoInit,
                dim755KuoInit,
                dim756KuoInit,
                dim757KuoInit,
                dim758KuoInit,
                dim759KuoInit,
                dim760KuoInit,
                dim761KuoInit,
                dim762KuoInit,
                dim763KuoInit,
                dim764KuoInit,
                dim765KuoInit,
                dim766KuoInit,
                dim767KuoInit,
                dim768KuoInit,
                dim769KuoInit,
                dim770KuoInit,
                dim771KuoInit,
                dim772KuoInit,
                dim773KuoInit,
                dim774KuoInit,
                dim775KuoInit,
                dim776KuoInit,
                dim777KuoInit,
                dim778KuoInit,
                dim779KuoInit,
                dim780KuoInit,
                dim781KuoInit,
                dim782KuoInit,
                dim783KuoInit,
                dim784KuoInit,
                dim785KuoInit,
                dim786KuoInit,
                dim787KuoInit,
                dim788KuoInit,
                dim789KuoInit,
                dim790KuoInit,
                dim791KuoInit,
                dim792KuoInit,
                dim793KuoInit,
                dim794KuoInit,
                dim795KuoInit,
                dim796KuoInit,
                dim797KuoInit,
                dim798KuoInit,
                dim799KuoInit,
                dim800KuoInit,
                dim801KuoInit,
                dim802KuoInit,
                dim803KuoInit,
                dim804KuoInit,
                dim805KuoInit,
                dim806KuoInit,
                dim807KuoInit,
                dim808KuoInit,
                dim809KuoInit,
                dim810KuoInit,
                dim811KuoInit,
                dim812KuoInit,
                dim813KuoInit,
                dim814KuoInit,
                dim815KuoInit,
                dim816KuoInit,
                dim817KuoInit,
                dim818KuoInit,
                dim819KuoInit,
                dim820KuoInit,
                dim821KuoInit,
                dim822KuoInit,
                dim823KuoInit,
                dim824KuoInit,
                dim825KuoInit,
                dim826KuoInit,
                dim827KuoInit,
                dim828KuoInit,
                dim829KuoInit,
                dim830KuoInit,
                dim831KuoInit,
                dim832KuoInit,
                dim833KuoInit,
                dim834KuoInit,
                dim835KuoInit,
                dim836KuoInit,
                dim837KuoInit,
                dim838KuoInit,
                dim839KuoInit,
                dim840KuoInit,
                dim841KuoInit,
                dim842KuoInit,
                dim843KuoInit,
                dim844KuoInit,
                dim845KuoInit,
                dim846KuoInit,
                dim847KuoInit,
                dim848KuoInit,
                dim849KuoInit,
                dim850KuoInit,
                dim851KuoInit,
                dim852KuoInit,
                dim853KuoInit,
                dim854KuoInit,
                dim855KuoInit,
                dim856KuoInit,
                dim857KuoInit,
                dim858KuoInit,
                dim859KuoInit,
                dim860KuoInit,
                dim861KuoInit,
                dim862KuoInit,
                dim863KuoInit,
                dim864KuoInit,
                dim865KuoInit,
                dim866KuoInit,
                dim867KuoInit,
                dim868KuoInit,
                dim869KuoInit,
                dim870KuoInit,
                dim871KuoInit,
                dim872KuoInit,
                dim873KuoInit,
                dim874KuoInit,
                dim875KuoInit,
                dim876KuoInit,
                dim877KuoInit,
                dim878KuoInit,
                dim879KuoInit,
                dim880KuoInit,
                dim881KuoInit,
                dim882KuoInit,
                dim883KuoInit,
                dim884KuoInit,
                dim885KuoInit,
                dim886KuoInit,
                dim887KuoInit,
                dim888KuoInit,
                dim889KuoInit,
                dim890KuoInit,
                dim891KuoInit,
                dim892KuoInit,
                dim893KuoInit,
                dim894KuoInit,
                dim895KuoInit,
                dim896KuoInit,
                dim897KuoInit,
                dim898KuoInit,
                dim899KuoInit,
                dim900KuoInit,
                dim901KuoInit,
                dim902KuoInit,
                dim903KuoInit,
                dim904KuoInit,
                dim905KuoInit,
                dim906KuoInit,
                dim907KuoInit,
                dim908KuoInit,
                dim909KuoInit,
                dim910KuoInit,
                dim911KuoInit,
                dim912KuoInit,
                dim913KuoInit,
                dim914KuoInit,
                dim915KuoInit,
                dim916KuoInit,
                dim917KuoInit,
                dim918KuoInit,
                dim919KuoInit,
                dim920KuoInit,
                dim921KuoInit,
                dim922KuoInit,
                dim923KuoInit,
                dim924KuoInit,
                dim925KuoInit,
                dim926KuoInit,
                dim927KuoInit,
                dim928KuoInit,
                dim929KuoInit,
                dim930KuoInit,
                dim931KuoInit,
                dim932KuoInit,
                dim933KuoInit,
                dim934KuoInit,
                dim935KuoInit,
                dim936KuoInit,
                dim937KuoInit,
                dim938KuoInit,
                dim939KuoInit,
                dim940KuoInit,
                dim941KuoInit,
                dim942KuoInit,
                dim943KuoInit,
                dim944KuoInit,
                dim945KuoInit,
                dim946KuoInit,
                dim947KuoInit,
                dim948KuoInit,
                dim949KuoInit,
                dim950KuoInit,
                dim951KuoInit,
                dim952KuoInit,
                dim953KuoInit,
                dim954KuoInit,
                dim955KuoInit,
                dim956KuoInit,
                dim957KuoInit,
                dim958KuoInit,
                dim959KuoInit,
                dim960KuoInit,
                dim961KuoInit,
                dim962KuoInit,
                dim963KuoInit,
                dim964KuoInit,
                dim965KuoInit,
                dim966KuoInit,
                dim967KuoInit,
                dim968KuoInit,
                dim969KuoInit,
                dim970KuoInit,
                dim971KuoInit,
                dim972KuoInit,
                dim973KuoInit,
                dim974KuoInit,
                dim975KuoInit,
                dim976KuoInit,
                dim977KuoInit,
                dim978KuoInit,
                dim979KuoInit,
                dim980KuoInit,
                dim981KuoInit,
                dim982KuoInit,
                dim983KuoInit,
                dim984KuoInit,
                dim985KuoInit,
                dim986KuoInit,
                dim987KuoInit,
                dim988KuoInit,
                dim989KuoInit,
                dim990KuoInit,
                dim991KuoInit,
                dim992KuoInit,
                dim993KuoInit,
                dim994KuoInit,
                dim995KuoInit,
                dim996KuoInit,
                dim997KuoInit,
                dim998KuoInit,
                dim999KuoInit,
                dim1000KuoInit,
                dim1001KuoInit,
                dim1002KuoInit,
                dim1003KuoInit,
                dim1004KuoInit,
                dim1005KuoInit,
                dim1006KuoInit,
                dim1007KuoInit,
                dim1008KuoInit,
                dim1009KuoInit,
                dim1010KuoInit,
                dim1011KuoInit,
                dim1012KuoInit,
                dim1013KuoInit,
                dim1014KuoInit,
                dim1015KuoInit,
                dim1016KuoInit,
                dim1017KuoInit,
                dim1018KuoInit,
                dim1019KuoInit,
                dim1020KuoInit,
                dim1021KuoInit,
                dim1022KuoInit,
                dim1023KuoInit,
                dim1024KuoInit,
                dim1025KuoInit,
                dim1026KuoInit,
                dim1027KuoInit,
                dim1028KuoInit,
                dim1029KuoInit,
                dim1030KuoInit,
                dim1031KuoInit,
                dim1032KuoInit,
                dim1033KuoInit,
                dim1034KuoInit,
                dim1035KuoInit,
                dim1036KuoInit,
                dim1037KuoInit,
                dim1038KuoInit,
                dim1039KuoInit,
                dim1040KuoInit,
                dim1041KuoInit,
                dim1042KuoInit,
                dim1043KuoInit,
                dim1044KuoInit,
                dim1045KuoInit,
                dim1046KuoInit,
                dim1047KuoInit,
                dim1048KuoInit,
                dim1049KuoInit,
                dim1050KuoInit,
                dim1051KuoInit,
                dim1052KuoInit,
                dim1053KuoInit,
                dim1054KuoInit,
                dim1055KuoInit,
                dim1056KuoInit,
                dim1057KuoInit,
                dim1058KuoInit,
                dim1059KuoInit,
                dim1060KuoInit,
                dim1061KuoInit,
                dim1062KuoInit,
                dim1063KuoInit,
                dim1064KuoInit,
                dim1065KuoInit,
                dim1066KuoInit,
                dim1067KuoInit,
                dim1068KuoInit,
                dim1069KuoInit,
                dim1070KuoInit,
                dim1071KuoInit,
                dim1072KuoInit,
                dim1073KuoInit,
                dim1074KuoInit,
                dim1075KuoInit,
                dim1076KuoInit,
                dim1077KuoInit,
                dim1078KuoInit,
                dim1079KuoInit,
                dim1080KuoInit,
                dim1081KuoInit,
                dim1082KuoInit,
                dim1083KuoInit,
                dim1084KuoInit,
                dim1085KuoInit,
                dim1086KuoInit,
                dim1087KuoInit,
                dim1088KuoInit,
                dim1089KuoInit,
                dim1090KuoInit,
                dim1091KuoInit,
                dim1092KuoInit,
                dim1093KuoInit,
                dim1094KuoInit,
                dim1095KuoInit,
                dim1096KuoInit,
                dim1097KuoInit,
                dim1098KuoInit,
                dim1099KuoInit,
                dim1100KuoInit,
                dim1101KuoInit,
                dim1102KuoInit,
                dim1103KuoInit,
                dim1104KuoInit,
                dim1105KuoInit,
                dim1106KuoInit,
                dim1107KuoInit,
                dim1108KuoInit,
                dim1109KuoInit,
                dim1110KuoInit,
                dim1111KuoInit,
                dim1112KuoInit,
                dim1113KuoInit,
                dim1114KuoInit,
                dim1115KuoInit,
                dim1116KuoInit,
                dim1117KuoInit,
                dim1118KuoInit,
                dim1119KuoInit,
                dim1120KuoInit,
                dim1121KuoInit,
                dim1122KuoInit,
                dim1123KuoInit,
                dim1124KuoInit,
                dim1125KuoInit,
                dim1126KuoInit,
                dim1127KuoInit,
                dim1128KuoInit,
                dim1129KuoInit,
                dim1130KuoInit,
                dim1131KuoInit,
                dim1132KuoInit,
                dim1133KuoInit,
                dim1134KuoInit,
                dim1135KuoInit,
                dim1136KuoInit,
                dim1137KuoInit,
                dim1138KuoInit,
                dim1139KuoInit,
                dim1140KuoInit,
                dim1141KuoInit,
                dim1142KuoInit,
                dim1143KuoInit,
                dim1144KuoInit,
                dim1145KuoInit,
                dim1146KuoInit,
                dim1147KuoInit,
                dim1148KuoInit,
                dim1149KuoInit,
                dim1150KuoInit,
                dim1151KuoInit,
                dim1152KuoInit,
                dim1153KuoInit,
                dim1154KuoInit,
                dim1155KuoInit,
                dim1156KuoInit,
                dim1157KuoInit,
                dim1158KuoInit,
                dim1159KuoInit,
                dim1160KuoInit,
                dim1161KuoInit,
                dim1162KuoInit,
                dim1163KuoInit,
                dim1164KuoInit,
                dim1165KuoInit,
                dim1166KuoInit,
                dim1167KuoInit,
                dim1168KuoInit,
                dim1169KuoInit,
                dim1170KuoInit,
                dim1171KuoInit,
                dim1172KuoInit,
                dim1173KuoInit,
                dim1174KuoInit,
                dim1175KuoInit,
                dim1176KuoInit,
                dim1177KuoInit,
                dim1178KuoInit,
                dim1179KuoInit,
                dim1180KuoInit,
                dim1181KuoInit,
                dim1182KuoInit,
                dim1183KuoInit,
                dim1184KuoInit,
                dim1185KuoInit,
                dim1186KuoInit,
                dim1187KuoInit,
                dim1188KuoInit,
                dim1189KuoInit,
                dim1190KuoInit,
                dim1191KuoInit,
                dim1192KuoInit,
                dim1193KuoInit,
                dim1194KuoInit,
                dim1195KuoInit,
                dim1196KuoInit,
                dim1197KuoInit,
                dim1198KuoInit,
                dim1199KuoInit,
                dim1200KuoInit,
                dim1201KuoInit,
                dim1202KuoInit,
                dim1203KuoInit,
                dim1204KuoInit,
                dim1205KuoInit,
                dim1206KuoInit,
                dim1207KuoInit,
                dim1208KuoInit,
                dim1209KuoInit,
                dim1210KuoInit,
                dim1211KuoInit,
                dim1212KuoInit,
                dim1213KuoInit,
                dim1214KuoInit,
                dim1215KuoInit,
                dim1216KuoInit,
                dim1217KuoInit,
                dim1218KuoInit,
                dim1219KuoInit,
                dim1220KuoInit,
                dim1221KuoInit,
                dim1222KuoInit,
                dim1223KuoInit,
                dim1224KuoInit,
                dim1225KuoInit,
                dim1226KuoInit,
                dim1227KuoInit,
                dim1228KuoInit,
                dim1229KuoInit,
                dim1230KuoInit,
                dim1231KuoInit,
                dim1232KuoInit,
                dim1233KuoInit,
                dim1234KuoInit,
                dim1235KuoInit,
                dim1236KuoInit,
                dim1237KuoInit,
                dim1238KuoInit,
                dim1239KuoInit,
                dim1240KuoInit,
                dim1241KuoInit,
                dim1242KuoInit,
                dim1243KuoInit,
                dim1244KuoInit,
                dim1245KuoInit,
                dim1246KuoInit,
                dim1247KuoInit,
                dim1248KuoInit,
                dim1249KuoInit,
                dim1250KuoInit,
                dim1251KuoInit,
                dim1252KuoInit,
                dim1253KuoInit,
                dim1254KuoInit,
                dim1255KuoInit,
                dim1256KuoInit,
                dim1257KuoInit,
                dim1258KuoInit,
                dim1259KuoInit,
                dim1260KuoInit,
                dim1261KuoInit,
                dim1262KuoInit,
                dim1263KuoInit,
                dim1264KuoInit,
                dim1265KuoInit,
                dim1266KuoInit,
                dim1267KuoInit,
                dim1268KuoInit,
                dim1269KuoInit,
                dim1270KuoInit,
                dim1271KuoInit,
                dim1272KuoInit,
                dim1273KuoInit,
                dim1274KuoInit,
                dim1275KuoInit,
                dim1276KuoInit,
                dim1277KuoInit,
                dim1278KuoInit,
                dim1279KuoInit,
                dim1280KuoInit,
                dim1281KuoInit,
                dim1282KuoInit,
                dim1283KuoInit,
                dim1284KuoInit,
                dim1285KuoInit,
                dim1286KuoInit,
                dim1287KuoInit,
                dim1288KuoInit,
                dim1289KuoInit,
                dim1290KuoInit,
                dim1291KuoInit,
                dim1292KuoInit,
                dim1293KuoInit,
                dim1294KuoInit,
                dim1295KuoInit,
                dim1296KuoInit,
                dim1297KuoInit,
                dim1298KuoInit,
                dim1299KuoInit,
                dim1300KuoInit,
                dim1301KuoInit,
                dim1302KuoInit,
                dim1303KuoInit,
                dim1304KuoInit,
                dim1305KuoInit,
                dim1306KuoInit,
                dim1307KuoInit,
                dim1308KuoInit,
                dim1309KuoInit,
                dim1310KuoInit,
                dim1311KuoInit,
                dim1312KuoInit,
                dim1313KuoInit,
                dim1314KuoInit,
                dim1315KuoInit,
                dim1316KuoInit,
                dim1317KuoInit,
                dim1318KuoInit,
                dim1319KuoInit,
                dim1320KuoInit,
                dim1321KuoInit,
                dim1322KuoInit,
                dim1323KuoInit,
                dim1324KuoInit,
                dim1325KuoInit,
                dim1326KuoInit,
                dim1327KuoInit,
                dim1328KuoInit,
                dim1329KuoInit,
                dim1330KuoInit,
                dim1331KuoInit,
                dim1332KuoInit,
                dim1333KuoInit,
                dim1334KuoInit,
                dim1335KuoInit,
                dim1336KuoInit,
                dim1337KuoInit,
                dim1338KuoInit,
                dim1339KuoInit,
                dim1340KuoInit,
                dim1341KuoInit,
                dim1342KuoInit,
                dim1343KuoInit,
                dim1344KuoInit,
                dim1345KuoInit,
                dim1346KuoInit,
                dim1347KuoInit,
                dim1348KuoInit,
                dim1349KuoInit,
                dim1350KuoInit,
                dim1351KuoInit,
                dim1352KuoInit,
                dim1353KuoInit,
                dim1354KuoInit,
                dim1355KuoInit,
                dim1356KuoInit,
                dim1357KuoInit,
                dim1358KuoInit,
                dim1359KuoInit,
                dim1360KuoInit,
                dim1361KuoInit,
                dim1362KuoInit,
                dim1363KuoInit,
                dim1364KuoInit,
                dim1365KuoInit,
                dim1366KuoInit,
                dim1367KuoInit,
                dim1368KuoInit,
                dim1369KuoInit,
                dim1370KuoInit,
                dim1371KuoInit,
                dim1372KuoInit,
                dim1373KuoInit,
                dim1374KuoInit,
                dim1375KuoInit,
                dim1376KuoInit,
                dim1377KuoInit,
                dim1378KuoInit,
                dim1379KuoInit,
                dim1380KuoInit,
                dim1381KuoInit,
                dim1382KuoInit,
                dim1383KuoInit,
                dim1384KuoInit,
                dim1385KuoInit,
                dim1386KuoInit,
                dim1387KuoInit,
                dim1388KuoInit,
                dim1389KuoInit,
                dim1390KuoInit,
                dim1391KuoInit,
                dim1392KuoInit,
                dim1393KuoInit,
                dim1394KuoInit,
                dim1395KuoInit,
                dim1396KuoInit,
                dim1397KuoInit,
                dim1398KuoInit,
                dim1399KuoInit,
                dim1400KuoInit,
                dim1401KuoInit,
                dim1402KuoInit,
                dim1403KuoInit,
                dim1404KuoInit,
                dim1405KuoInit,
                dim1406KuoInit,
                dim1407KuoInit,
                dim1408KuoInit,
                dim1409KuoInit,
                dim1410KuoInit,
                dim1411KuoInit,
                dim1412KuoInit,
                dim1413KuoInit,
                dim1414KuoInit,
                dim1415KuoInit,
                dim1416KuoInit,
                dim1417KuoInit,
                dim1418KuoInit,
                dim1419KuoInit,
                dim1420KuoInit,
                dim1421KuoInit,
                dim1422KuoInit,
                dim1423KuoInit,
                dim1424KuoInit,
                dim1425KuoInit,
                dim1426KuoInit,
                dim1427KuoInit,
                dim1428KuoInit,
                dim1429KuoInit,
                dim1430KuoInit,
                dim1431KuoInit,
                dim1432KuoInit,
                dim1433KuoInit,
                dim1434KuoInit,
                dim1435KuoInit,
                dim1436KuoInit,
                dim1437KuoInit,
                dim1438KuoInit,
                dim1439KuoInit,
                dim1440KuoInit,
                dim1441KuoInit,
                dim1442KuoInit,
                dim1443KuoInit,
                dim1444KuoInit,
                dim1445KuoInit,
                dim1446KuoInit,
                dim1447KuoInit,
                dim1448KuoInit,
                dim1449KuoInit,
                dim1450KuoInit,
                dim1451KuoInit,
                dim1452KuoInit,
                dim1453KuoInit,
                dim1454KuoInit,
                dim1455KuoInit,
                dim1456KuoInit,
                dim1457KuoInit,
                dim1458KuoInit,
                dim1459KuoInit,
                dim1460KuoInit,
                dim1461KuoInit,
                dim1462KuoInit,
                dim1463KuoInit,
                dim1464KuoInit,
                dim1465KuoInit,
                dim1466KuoInit,
                dim1467KuoInit,
                dim1468KuoInit,
                dim1469KuoInit,
                dim1470KuoInit,
                dim1471KuoInit,
                dim1472KuoInit,
                dim1473KuoInit,
                dim1474KuoInit,
                dim1475KuoInit,
                dim1476KuoInit,
                dim1477KuoInit,
                dim1478KuoInit,
                dim1479KuoInit,
                dim1480KuoInit,
                dim1481KuoInit,
                dim1482KuoInit,
                dim1483KuoInit,
                dim1484KuoInit,
                dim1485KuoInit,
                dim1486KuoInit,
                dim1487KuoInit,
                dim1488KuoInit,
                dim1489KuoInit,
                dim1490KuoInit,
                dim1491KuoInit,
                dim1492KuoInit,
                dim1493KuoInit,
                dim1494KuoInit,
                dim1495KuoInit,
                dim1496KuoInit,
                dim1497KuoInit,
                dim1498KuoInit,
                dim1499KuoInit,
                dim1500KuoInit,
                dim1501KuoInit,
                dim1502KuoInit,
                dim1503KuoInit,
                dim1504KuoInit,
                dim1505KuoInit,
                dim1506KuoInit,
                dim1507KuoInit,
                dim1508KuoInit,
                dim1509KuoInit,
                dim1510KuoInit,
                dim1511KuoInit,
                dim1512KuoInit,
                dim1513KuoInit,
                dim1514KuoInit,
                dim1515KuoInit,
                dim1516KuoInit,
                dim1517KuoInit,
                dim1518KuoInit,
                dim1519KuoInit,
                dim1520KuoInit,
                dim1521KuoInit,
                dim1522KuoInit,
                dim1523KuoInit,
                dim1524KuoInit,
                dim1525KuoInit,
                dim1526KuoInit,
                dim1527KuoInit,
                dim1528KuoInit,
                dim1529KuoInit,
                dim1530KuoInit,
                dim1531KuoInit,
                dim1532KuoInit,
                dim1533KuoInit,
                dim1534KuoInit,
                dim1535KuoInit,
                dim1536KuoInit,
                dim1537KuoInit,
                dim1538KuoInit,
                dim1539KuoInit,
                dim1540KuoInit,
                dim1541KuoInit,
                dim1542KuoInit,
                dim1543KuoInit,
                dim1544KuoInit,
                dim1545KuoInit,
                dim1546KuoInit,
                dim1547KuoInit,
                dim1548KuoInit,
                dim1549KuoInit,
                dim1550KuoInit,
                dim1551KuoInit,
                dim1552KuoInit,
                dim1553KuoInit,
                dim1554KuoInit,
                dim1555KuoInit,
                dim1556KuoInit,
                dim1557KuoInit,
                dim1558KuoInit,
                dim1559KuoInit,
                dim1560KuoInit,
                dim1561KuoInit,
                dim1562KuoInit,
                dim1563KuoInit,
                dim1564KuoInit,
                dim1565KuoInit,
                dim1566KuoInit,
                dim1567KuoInit,
                dim1568KuoInit,
                dim1569KuoInit,
                dim1570KuoInit,
                dim1571KuoInit,
                dim1572KuoInit,
                dim1573KuoInit,
                dim1574KuoInit,
                dim1575KuoInit,
                dim1576KuoInit,
                dim1577KuoInit,
                dim1578KuoInit,
                dim1579KuoInit,
                dim1580KuoInit,
                dim1581KuoInit,
                dim1582KuoInit,
                dim1583KuoInit,
                dim1584KuoInit,
                dim1585KuoInit,
                dim1586KuoInit,
                dim1587KuoInit,
                dim1588KuoInit,
                dim1589KuoInit,
                dim1590KuoInit,
                dim1591KuoInit,
                dim1592KuoInit,
                dim1593KuoInit,
                dim1594KuoInit,
                dim1595KuoInit,
                dim1596KuoInit,
                dim1597KuoInit,
                dim1598KuoInit,
                dim1599KuoInit,
                dim1600KuoInit,
                dim1601KuoInit,
                dim1602KuoInit,
                dim1603KuoInit,
                dim1604KuoInit,
                dim1605KuoInit,
                dim1606KuoInit,
                dim1607KuoInit,
                dim1608KuoInit,
                dim1609KuoInit,
                dim1610KuoInit,
                dim1611KuoInit,
                dim1612KuoInit,
                dim1613KuoInit,
                dim1614KuoInit,
                dim1615KuoInit,
                dim1616KuoInit,
                dim1617KuoInit,
                dim1618KuoInit,
                dim1619KuoInit,
                dim1620KuoInit,
                dim1621KuoInit,
                dim1622KuoInit,
                dim1623KuoInit,
                dim1624KuoInit,
                dim1625KuoInit,
                dim1626KuoInit,
                dim1627KuoInit,
                dim1628KuoInit,
                dim1629KuoInit,
                dim1630KuoInit,
                dim1631KuoInit,
                dim1632KuoInit,
                dim1633KuoInit,
                dim1634KuoInit,
                dim1635KuoInit,
                dim1636KuoInit,
                dim1637KuoInit,
                dim1638KuoInit,
                dim1639KuoInit,
                dim1640KuoInit,
                dim1641KuoInit,
                dim1642KuoInit,
                dim1643KuoInit,
                dim1644KuoInit,
                dim1645KuoInit,
                dim1646KuoInit,
                dim1647KuoInit,
                dim1648KuoInit,
                dim1649KuoInit,
                dim1650KuoInit,
                dim1651KuoInit,
                dim1652KuoInit,
                dim1653KuoInit,
                dim1654KuoInit,
                dim1655KuoInit,
                dim1656KuoInit,
                dim1657KuoInit,
                dim1658KuoInit,
                dim1659KuoInit,
                dim1660KuoInit,
                dim1661KuoInit,
                dim1662KuoInit,
                dim1663KuoInit,
                dim1664KuoInit,
                dim1665KuoInit,
                dim1666KuoInit,
                dim1667KuoInit,
                dim1668KuoInit,
                dim1669KuoInit,
                dim1670KuoInit,
                dim1671KuoInit,
                dim1672KuoInit,
                dim1673KuoInit,
                dim1674KuoInit,
                dim1675KuoInit,
                dim1676KuoInit,
                dim1677KuoInit,
                dim1678KuoInit,
                dim1679KuoInit,
                dim1680KuoInit,
                dim1681KuoInit,
                dim1682KuoInit,
                dim1683KuoInit,
                dim1684KuoInit,
                dim1685KuoInit,
                dim1686KuoInit,
                dim1687KuoInit,
                dim1688KuoInit,
                dim1689KuoInit,
                dim1690KuoInit,
                dim1691KuoInit,
                dim1692KuoInit,
                dim1693KuoInit,
                dim1694KuoInit,
                dim1695KuoInit,
                dim1696KuoInit,
                dim1697KuoInit,
                dim1698KuoInit,
                dim1699KuoInit,
                dim1700KuoInit,
                dim1701KuoInit,
                dim1702KuoInit,
                dim1703KuoInit,
                dim1704KuoInit,
                dim1705KuoInit,
                dim1706KuoInit,
                dim1707KuoInit,
                dim1708KuoInit,
                dim1709KuoInit,
                dim1710KuoInit,
                dim1711KuoInit,
                dim1712KuoInit,
                dim1713KuoInit,
                dim1714KuoInit,
                dim1715KuoInit,
                dim1716KuoInit,
                dim1717KuoInit,
                dim1718KuoInit,
                dim1719KuoInit,
                dim1720KuoInit,
                dim1721KuoInit,
                dim1722KuoInit,
                dim1723KuoInit,
                dim1724KuoInit,
                dim1725KuoInit,
                dim1726KuoInit,
                dim1727KuoInit,
                dim1728KuoInit,
                dim1729KuoInit,
                dim1730KuoInit,
                dim1731KuoInit,
                dim1732KuoInit,
                dim1733KuoInit,
                dim1734KuoInit,
                dim1735KuoInit,
                dim1736KuoInit,
                dim1737KuoInit,
                dim1738KuoInit,
                dim1739KuoInit,
                dim1740KuoInit,
                dim1741KuoInit,
                dim1742KuoInit,
                dim1743KuoInit,
                dim1744KuoInit,
                dim1745KuoInit,
                dim1746KuoInit,
                dim1747KuoInit,
                dim1748KuoInit,
                dim1749KuoInit,
                dim1750KuoInit,
                dim1751KuoInit,
                dim1752KuoInit,
                dim1753KuoInit,
                dim1754KuoInit,
                dim1755KuoInit,
                dim1756KuoInit,
                dim1757KuoInit,
                dim1758KuoInit,
                dim1759KuoInit,
                dim1760KuoInit,
                dim1761KuoInit,
                dim1762KuoInit,
                dim1763KuoInit,
                dim1764KuoInit,
                dim1765KuoInit,
                dim1766KuoInit,
                dim1767KuoInit,
                dim1768KuoInit,
                dim1769KuoInit,
                dim1770KuoInit,
                dim1771KuoInit,
                dim1772KuoInit,
                dim1773KuoInit,
                dim1774KuoInit,
                dim1775KuoInit,
                dim1776KuoInit,
                dim1777KuoInit,
                dim1778KuoInit,
                dim1779KuoInit,
                dim1780KuoInit,
                dim1781KuoInit,
                dim1782KuoInit,
                dim1783KuoInit,
                dim1784KuoInit,
                dim1785KuoInit,
                dim1786KuoInit,
                dim1787KuoInit,
                dim1788KuoInit,
                dim1789KuoInit,
                dim1790KuoInit,
                dim1791KuoInit,
                dim1792KuoInit,
                dim1793KuoInit,
                dim1794KuoInit,
                dim1795KuoInit,
                dim1796KuoInit,
                dim1797KuoInit,
                dim1798KuoInit,
                dim1799KuoInit,
                dim1800KuoInit,
                dim1801KuoInit,
                dim1802KuoInit,
                dim1803KuoInit,
                dim1804KuoInit,
                dim1805KuoInit,
                dim1806KuoInit,
                dim1807KuoInit,
                dim1808KuoInit,
                dim1809KuoInit,
                dim1810KuoInit,
                dim1811KuoInit,
                dim1812KuoInit,
                dim1813KuoInit,
                dim1814KuoInit,
                dim1815KuoInit,
                dim1816KuoInit,
                dim1817KuoInit,
                dim1818KuoInit,
                dim1819KuoInit,
                dim1820KuoInit,
                dim1821KuoInit,
                dim1822KuoInit,
                dim1823KuoInit,
                dim1824KuoInit,
                dim1825KuoInit,
                dim1826KuoInit,
                dim1827KuoInit,
                dim1828KuoInit,
                dim1829KuoInit,
                dim1830KuoInit,
                dim1831KuoInit,
                dim1832KuoInit,
                dim1833KuoInit,
                dim1834KuoInit,
                dim1835KuoInit,
                dim1836KuoInit,
                dim1837KuoInit,
                dim1838KuoInit,
                dim1839KuoInit,
                dim1840KuoInit,
                dim1841KuoInit,
                dim1842KuoInit,
                dim1843KuoInit,
                dim1844KuoInit,
                dim1845KuoInit,
                dim1846KuoInit,
                dim1847KuoInit,
                dim1848KuoInit,
                dim1849KuoInit,
                dim1850KuoInit,
                dim1851KuoInit,
                dim1852KuoInit,
                dim1853KuoInit,
                dim1854KuoInit,
                dim1855KuoInit,
                dim1856KuoInit,
                dim1857KuoInit,
                dim1858KuoInit,
                dim1859KuoInit,
                dim1860KuoInit,
                dim1861KuoInit,
                dim1862KuoInit,
                dim1863KuoInit,
                dim1864KuoInit,
                dim1865KuoInit,
                dim1866KuoInit,
                dim1867KuoInit,
                dim1868KuoInit,
                dim1869KuoInit,
                dim1870KuoInit,
                dim1871KuoInit,
                dim1872KuoInit,
                dim1873KuoInit,
                dim1874KuoInit,
                dim1875KuoInit,
                dim1876KuoInit,
                dim1877KuoInit,
                dim1878KuoInit,
                dim1879KuoInit,
                dim1880KuoInit,
                dim1881KuoInit,
                dim1882KuoInit,
                dim1883KuoInit,
                dim1884KuoInit,
                dim1885KuoInit,
                dim1886KuoInit,
                dim1887KuoInit,
                dim1888KuoInit,
                dim1889KuoInit,
                dim1890KuoInit,
                dim1891KuoInit,
                dim1892KuoInit,
                dim1893KuoInit,
                dim1894KuoInit,
                dim1895KuoInit,
                dim1896KuoInit,
                dim1897KuoInit,
                dim1898KuoInit,
                dim1899KuoInit,
                dim1900KuoInit,
                dim1901KuoInit,
                dim1902KuoInit,
                dim1903KuoInit,
                dim1904KuoInit,
                dim1905KuoInit,
                dim1906KuoInit,
                dim1907KuoInit,
                dim1908KuoInit,
                dim1909KuoInit,
                dim1910KuoInit,
                dim1911KuoInit,
                dim1912KuoInit,
                dim1913KuoInit,
                dim1914KuoInit,
                dim1915KuoInit,
                dim1916KuoInit,
                dim1917KuoInit,
                dim1918KuoInit,
                dim1919KuoInit,
                dim1920KuoInit,
                dim1921KuoInit,
                dim1922KuoInit,
                dim1923KuoInit,
                dim1924KuoInit,
                dim1925KuoInit,
                dim1926KuoInit,
                dim1927KuoInit,
                dim1928KuoInit,
                dim1929KuoInit,
                dim1930KuoInit,
                dim1931KuoInit,
                dim1932KuoInit,
                dim1933KuoInit,
                dim1934KuoInit,
                dim1935KuoInit,
                dim1936KuoInit,
                dim1937KuoInit,
                dim1938KuoInit,
                dim1939KuoInit,
                dim1940KuoInit,
                dim1941KuoInit,
                dim1942KuoInit,
                dim1943KuoInit,
                dim1944KuoInit,
                dim1945KuoInit,
                dim1946KuoInit,
                dim1947KuoInit,
                dim1948KuoInit,
                dim1949KuoInit,
                dim1950KuoInit,
                dim1951KuoInit,
                dim1952KuoInit,
                dim1953KuoInit,
                dim1954KuoInit,
                dim1955KuoInit,
                dim1956KuoInit,
                dim1957KuoInit,
                dim1958KuoInit,
                dim1959KuoInit,
                dim1960KuoInit,
                dim1961KuoInit,
                dim1962KuoInit,
                dim1963KuoInit,
                dim1964KuoInit,
                dim1965KuoInit,
                dim1966KuoInit,
                dim1967KuoInit,
                dim1968KuoInit,
                dim1969KuoInit,
                dim1970KuoInit,
                dim1971KuoInit,
                dim1972KuoInit,
                dim1973KuoInit,
                dim1974KuoInit,
                dim1975KuoInit,
                dim1976KuoInit,
                dim1977KuoInit,
                dim1978KuoInit,
                dim1979KuoInit,
                dim1980KuoInit,
                dim1981KuoInit,
                dim1982KuoInit,
                dim1983KuoInit,
                dim1984KuoInit,
                dim1985KuoInit,
                dim1986KuoInit,
                dim1987KuoInit,
                dim1988KuoInit,
                dim1989KuoInit,
                dim1990KuoInit,
                dim1991KuoInit,
                dim1992KuoInit,
                dim1993KuoInit,
                dim1994KuoInit,
                dim1995KuoInit,
                dim1996KuoInit,
                dim1997KuoInit,
                dim1998KuoInit,
                dim1999KuoInit,
                dim2000KuoInit,
                dim2001KuoInit,
                dim2002KuoInit,
                dim2003KuoInit,
                dim2004KuoInit,
                dim2005KuoInit,
                dim2006KuoInit,
                dim2007KuoInit,
                dim2008KuoInit,
                dim2009KuoInit,
                dim2010KuoInit,
                dim2011KuoInit,
                dim2012KuoInit,
                dim2013KuoInit,
                dim2014KuoInit,
                dim2015KuoInit,
                dim2016KuoInit,
                dim2017KuoInit,
                dim2018KuoInit,
                dim2019KuoInit,
                dim2020KuoInit,
                dim2021KuoInit,
                dim2022KuoInit,
                dim2023KuoInit,
                dim2024KuoInit,
                dim2025KuoInit,
                dim2026KuoInit,
                dim2027KuoInit,
                dim2028KuoInit,
                dim2029KuoInit,
                dim2030KuoInit,
                dim2031KuoInit,
                dim2032KuoInit,
                dim2033KuoInit,
                dim2034KuoInit,
                dim2035KuoInit,
                dim2036KuoInit,
                dim2037KuoInit,
                dim2038KuoInit,
                dim2039KuoInit,
                dim2040KuoInit,
                dim2041KuoInit,
                dim2042KuoInit,
                dim2043KuoInit,
                dim2044KuoInit,
                dim2045KuoInit,
                dim2046KuoInit,
                dim2047KuoInit,
                dim2048KuoInit,
                dim2049KuoInit,
                dim2050KuoInit,
                dim2051KuoInit,
                dim2052KuoInit,
                dim2053KuoInit,
                dim2054KuoInit,
                dim2055KuoInit,
                dim2056KuoInit,
                dim2057KuoInit,
                dim2058KuoInit,
                dim2059KuoInit,
                dim2060KuoInit,
                dim2061KuoInit,
                dim2062KuoInit,
                dim2063KuoInit,
                dim2064KuoInit,
                dim2065KuoInit,
                dim2066KuoInit,
                dim2067KuoInit,
                dim2068KuoInit,
                dim2069KuoInit,
                dim2070KuoInit,
                dim2071KuoInit,
                dim2072KuoInit,
                dim2073KuoInit,
                dim2074KuoInit,
                dim2075KuoInit,
                dim2076KuoInit,
                dim2077KuoInit,
                dim2078KuoInit,
                dim2079KuoInit,
                dim2080KuoInit,
                dim2081KuoInit,
                dim2082KuoInit,
                dim2083KuoInit,
                dim2084KuoInit,
                dim2085KuoInit,
                dim2086KuoInit,
                dim2087KuoInit,
                dim2088KuoInit,
                dim2089KuoInit,
                dim2090KuoInit,
                dim2091KuoInit,
                dim2092KuoInit,
                dim2093KuoInit,
                dim2094KuoInit,
                dim2095KuoInit,
                dim2096KuoInit,
                dim2097KuoInit,
                dim2098KuoInit,
                dim2099KuoInit,
                dim2100KuoInit,
                dim2101KuoInit,
                dim2102KuoInit,
                dim2103KuoInit,
                dim2104KuoInit,
                dim2105KuoInit,
                dim2106KuoInit,
                dim2107KuoInit,
                dim2108KuoInit,
                dim2109KuoInit,
                dim2110KuoInit,
                dim2111KuoInit,
                dim2112KuoInit,
                dim2113KuoInit,
                dim2114KuoInit,
                dim2115KuoInit,
                dim2116KuoInit,
                dim2117KuoInit,
                dim2118KuoInit,
                dim2119KuoInit,
                dim2120KuoInit,
                dim2121KuoInit,
                dim2122KuoInit,
                dim2123KuoInit,
                dim2124KuoInit,
                dim2125KuoInit,
                dim2126KuoInit,
                dim2127KuoInit,
                dim2128KuoInit,
                dim2129KuoInit,
                dim2130KuoInit,
                dim2131KuoInit,
                dim2132KuoInit,
                dim2133KuoInit,
                dim2134KuoInit,
                dim2135KuoInit,
                dim2136KuoInit,
                dim2137KuoInit,
                dim2138KuoInit,
                dim2139KuoInit,
                dim2140KuoInit,
                dim2141KuoInit,
                dim2142KuoInit,
                dim2143KuoInit,
                dim2144KuoInit,
                dim2145KuoInit,
                dim2146KuoInit,
                dim2147KuoInit,
                dim2148KuoInit,
                dim2149KuoInit,
                dim2150KuoInit,
                dim2151KuoInit,
                dim2152KuoInit,
                dim2153KuoInit,
                dim2154KuoInit,
                dim2155KuoInit,
                dim2156KuoInit,
                dim2157KuoInit,
                dim2158KuoInit,
                dim2159KuoInit,
                dim2160KuoInit,
                dim2161KuoInit,
                dim2162KuoInit,
                dim2163KuoInit,
                dim2164KuoInit,
                dim2165KuoInit,
                dim2166KuoInit,
                dim2167KuoInit,
                dim2168KuoInit,
                dim2169KuoInit,
                dim2170KuoInit,
                dim2171KuoInit,
                dim2172KuoInit,
                dim2173KuoInit,
                dim2174KuoInit,
                dim2175KuoInit,
                dim2176KuoInit,
                dim2177KuoInit,
                dim2178KuoInit,
                dim2179KuoInit,
                dim2180KuoInit,
                dim2181KuoInit,
                dim2182KuoInit,
                dim2183KuoInit,
                dim2184KuoInit,
                dim2185KuoInit,
                dim2186KuoInit,
                dim2187KuoInit,
                dim2188KuoInit,
                dim2189KuoInit,
                dim2190KuoInit,
                dim2191KuoInit,
                dim2192KuoInit,
                dim2193KuoInit,
                dim2194KuoInit,
                dim2195KuoInit,
                dim2196KuoInit,
                dim2197KuoInit,
                dim2198KuoInit,
                dim2199KuoInit,
                dim2200KuoInit,
                dim2201KuoInit,
                dim2202KuoInit,
                dim2203KuoInit,
                dim2204KuoInit,
                dim2205KuoInit,
                dim2206KuoInit,
                dim2207KuoInit,
                dim2208KuoInit,
                dim2209KuoInit,
                dim2210KuoInit,
                dim2211KuoInit,
                dim2212KuoInit,
                dim2213KuoInit,
                dim2214KuoInit,
                dim2215KuoInit,
                dim2216KuoInit,
                dim2217KuoInit,
                dim2218KuoInit,
                dim2219KuoInit,
                dim2220KuoInit,
                dim2221KuoInit,
                dim2222KuoInit,
                dim2223KuoInit,
                dim2224KuoInit,
                dim2225KuoInit,
                dim2226KuoInit,
                dim2227KuoInit,
                dim2228KuoInit,
                dim2229KuoInit,
                dim2230KuoInit,
                dim2231KuoInit,
                dim2232KuoInit,
                dim2233KuoInit,
                dim2234KuoInit,
                dim2235KuoInit,
                dim2236KuoInit,
                dim2237KuoInit,
                dim2238KuoInit,
                dim2239KuoInit,
                dim2240KuoInit,
                dim2241KuoInit,
                dim2242KuoInit,
                dim2243KuoInit,
                dim2244KuoInit,
                dim2245KuoInit,
                dim2246KuoInit,
                dim2247KuoInit,
                dim2248KuoInit,
                dim2249KuoInit,
                dim2250KuoInit,
                dim2251KuoInit,
                dim2252KuoInit,
                dim2253KuoInit,
                dim2254KuoInit,
                dim2255KuoInit,
                dim2256KuoInit,
                dim2257KuoInit,
                dim2258KuoInit,
                dim2259KuoInit,
                dim2260KuoInit,
                dim2261KuoInit,
                dim2262KuoInit,
                dim2263KuoInit,
                dim2264KuoInit,
                dim2265KuoInit,
                dim2266KuoInit,
                dim2267KuoInit,
                dim2268KuoInit,
                dim2269KuoInit,
                dim2270KuoInit,
                dim2271KuoInit,
                dim2272KuoInit,
                dim2273KuoInit,
                dim2274KuoInit,
                dim2275KuoInit,
                dim2276KuoInit,
                dim2277KuoInit,
                dim2278KuoInit,
                dim2279KuoInit,
                dim2280KuoInit,
                dim2281KuoInit,
                dim2282KuoInit,
                dim2283KuoInit,
                dim2284KuoInit,
                dim2285KuoInit,
                dim2286KuoInit,
                dim2287KuoInit,
                dim2288KuoInit,
                dim2289KuoInit,
                dim2290KuoInit,
                dim2291KuoInit,
                dim2292KuoInit,
                dim2293KuoInit,
                dim2294KuoInit,
                dim2295KuoInit,
                dim2296KuoInit,
                dim2297KuoInit,
                dim2298KuoInit,
                dim2299KuoInit,
                dim2300KuoInit,
                dim2301KuoInit,
                dim2302KuoInit,
                dim2303KuoInit,
                dim2304KuoInit,
                dim2305KuoInit,
                dim2306KuoInit,
                dim2307KuoInit,
                dim2308KuoInit,
                dim2309KuoInit,
                dim2310KuoInit,
                dim2311KuoInit,
                dim2312KuoInit,
                dim2313KuoInit,
                dim2314KuoInit,
                dim2315KuoInit,
                dim2316KuoInit,
                dim2317KuoInit,
                dim2318KuoInit,
                dim2319KuoInit,
                dim2320KuoInit,
                dim2321KuoInit,
                dim2322KuoInit,
                dim2323KuoInit,
                dim2324KuoInit,
                dim2325KuoInit,
                dim2326KuoInit,
                dim2327KuoInit,
                dim2328KuoInit,
                dim2329KuoInit,
                dim2330KuoInit,
                dim2331KuoInit,
                dim2332KuoInit,
                dim2333KuoInit,
                dim2334KuoInit,
                dim2335KuoInit,
                dim2336KuoInit,
                dim2337KuoInit,
                dim2338KuoInit,
                dim2339KuoInit,
                dim2340KuoInit,
                dim2341KuoInit,
                dim2342KuoInit,
                dim2343KuoInit,
                dim2344KuoInit,
                dim2345KuoInit,
                dim2346KuoInit,
                dim2347KuoInit,
                dim2348KuoInit,
                dim2349KuoInit,
                dim2350KuoInit,
                dim2351KuoInit,
                dim2352KuoInit,
                dim2353KuoInit,
                dim2354KuoInit,
                dim2355KuoInit,
                dim2356KuoInit,
                dim2357KuoInit,
                dim2358KuoInit,
                dim2359KuoInit,
                dim2360KuoInit,
                dim2361KuoInit,
                dim2362KuoInit,
                dim2363KuoInit,
                dim2364KuoInit,
                dim2365KuoInit,
                dim2366KuoInit,
                dim2367KuoInit,
                dim2368KuoInit,
                dim2369KuoInit,
                dim2370KuoInit,
                dim2371KuoInit,
                dim2372KuoInit,
                dim2373KuoInit,
                dim2374KuoInit,
                dim2375KuoInit,
                dim2376KuoInit,
                dim2377KuoInit,
                dim2378KuoInit,
                dim2379KuoInit,
                dim2380KuoInit,
                dim2381KuoInit,
                dim2382KuoInit,
                dim2383KuoInit,
                dim2384KuoInit,
                dim2385KuoInit,
                dim2386KuoInit,
                dim2387KuoInit,
                dim2388KuoInit,
                dim2389KuoInit,
                dim2390KuoInit,
                dim2391KuoInit,
                dim2392KuoInit,
                dim2393KuoInit,
                dim2394KuoInit,
                dim2395KuoInit,
                dim2396KuoInit,
                dim2397KuoInit,
                dim2398KuoInit,
                dim2399KuoInit,
                dim2400KuoInit,
                dim2401KuoInit,
                dim2402KuoInit,
                dim2403KuoInit,
                dim2404KuoInit,
                dim2405KuoInit,
                dim2406KuoInit,
                dim2407KuoInit,
                dim2408KuoInit,
                dim2409KuoInit,
                dim2410KuoInit,
                dim2411KuoInit,
                dim2412KuoInit,
                dim2413KuoInit,
                dim2414KuoInit,
                dim2415KuoInit,
                dim2416KuoInit,
                dim2417KuoInit,
                dim2418KuoInit,
                dim2419KuoInit,
                dim2420KuoInit,
                dim2421KuoInit,
                dim2422KuoInit,
                dim2423KuoInit,
                dim2424KuoInit,
                dim2425KuoInit,
                dim2426KuoInit,
                dim2427KuoInit,
                dim2428KuoInit,
                dim2429KuoInit,
                dim2430KuoInit,
                dim2431KuoInit,
                dim2432KuoInit,
                dim2433KuoInit,
                dim2434KuoInit,
                dim2435KuoInit,
                dim2436KuoInit,
                dim2437KuoInit,
                dim2438KuoInit,
                dim2439KuoInit,
                dim2440KuoInit,
                dim2441KuoInit,
                dim2442KuoInit,
                dim2443KuoInit,
                dim2444KuoInit,
                dim2445KuoInit,
                dim2446KuoInit,
                dim2447KuoInit,
                dim2448KuoInit,
                dim2449KuoInit,
                dim2450KuoInit,
                dim2451KuoInit,
                dim2452KuoInit,
                dim2453KuoInit,
                dim2454KuoInit,
                dim2455KuoInit,
                dim2456KuoInit,
                dim2457KuoInit,
                dim2458KuoInit,
                dim2459KuoInit,
                dim2460KuoInit,
                dim2461KuoInit,
                dim2462KuoInit,
                dim2463KuoInit,
                dim2464KuoInit,
                dim2465KuoInit,
                dim2466KuoInit,
                dim2467KuoInit,
                dim2468KuoInit,
                dim2469KuoInit,
                dim2470KuoInit,
                dim2471KuoInit,
                dim2472KuoInit,
                dim2473KuoInit,
                dim2474KuoInit,
                dim2475KuoInit,
                dim2476KuoInit,
                dim2477KuoInit,
                dim2478KuoInit,
                dim2479KuoInit,
                dim2480KuoInit,
                dim2481KuoInit,
                dim2482KuoInit,
                dim2483KuoInit,
                dim2484KuoInit,
                dim2485KuoInit,
                dim2486KuoInit,
                dim2487KuoInit,
                dim2488KuoInit,
                dim2489KuoInit,
                dim2490KuoInit,
                dim2491KuoInit,
                dim2492KuoInit,
                dim2493KuoInit,
                dim2494KuoInit,
                dim2495KuoInit,
                dim2496KuoInit,
                dim2497KuoInit,
                dim2498KuoInit,
                dim2499KuoInit,
                dim2500KuoInit,
                dim2501KuoInit,
                dim2502KuoInit,
                dim2503KuoInit,
                dim2504KuoInit,
                dim2505KuoInit,
                dim2506KuoInit,
                dim2507KuoInit,
                dim2508KuoInit,
                dim2509KuoInit,
                dim2510KuoInit,
                dim2511KuoInit,
                dim2512KuoInit,
                dim2513KuoInit,
                dim2514KuoInit,
                dim2515KuoInit,
                dim2516KuoInit,
                dim2517KuoInit,
                dim2518KuoInit,
                dim2519KuoInit,
                dim2520KuoInit,
                dim2521KuoInit,
                dim2522KuoInit,
                dim2523KuoInit,
                dim2524KuoInit,
                dim2525KuoInit,
                dim2526KuoInit,
                dim2527KuoInit,
                dim2528KuoInit,
                dim2529KuoInit,
                dim2530KuoInit,
                dim2531KuoInit,
                dim2532KuoInit,
                dim2533KuoInit,
                dim2534KuoInit,
                dim2535KuoInit,
                dim2536KuoInit,
                dim2537KuoInit,
                dim2538KuoInit,
                dim2539KuoInit,
                dim2540KuoInit,
                dim2541KuoInit,
                dim2542KuoInit,
                dim2543KuoInit,
                dim2544KuoInit,
                dim2545KuoInit,
                dim2546KuoInit,
                dim2547KuoInit,
                dim2548KuoInit,
                dim2549KuoInit,
                dim2550KuoInit,
                dim2551KuoInit,
                dim2552KuoInit,
                dim2553KuoInit,
                dim2554KuoInit,
                dim2555KuoInit,
                dim2556KuoInit,
                dim2557KuoInit,
                dim2558KuoInit,
                dim2559KuoInit,
                dim2560KuoInit,
                dim2561KuoInit,
                dim2562KuoInit,
                dim2563KuoInit,
                dim2564KuoInit,
                dim2565KuoInit,
                dim2566KuoInit,
                dim2567KuoInit,
                dim2568KuoInit,
                dim2569KuoInit,
                dim2570KuoInit,
                dim2571KuoInit,
                dim2572KuoInit,
                dim2573KuoInit,
                dim2574KuoInit,
                dim2575KuoInit,
                dim2576KuoInit,
                dim2577KuoInit,
                dim2578KuoInit,
                dim2579KuoInit,
                dim2580KuoInit,
                dim2581KuoInit,
                dim2582KuoInit,
                dim2583KuoInit,
                dim2584KuoInit,
                dim2585KuoInit,
                dim2586KuoInit,
                dim2587KuoInit,
                dim2588KuoInit,
                dim2589KuoInit,
                dim2590KuoInit,
                dim2591KuoInit,
                dim2592KuoInit,
                dim2593KuoInit,
                dim2594KuoInit,
                dim2595KuoInit,
                dim2596KuoInit,
                dim2597KuoInit,
                dim2598KuoInit,
                dim2599KuoInit,
                dim2600KuoInit,
                dim2601KuoInit,
                dim2602KuoInit,
                dim2603KuoInit,
                dim2604KuoInit,
                dim2605KuoInit,
                dim2606KuoInit,
                dim2607KuoInit,
                dim2608KuoInit,
                dim2609KuoInit,
                dim2610KuoInit,
                dim2611KuoInit,
                dim2612KuoInit,
                dim2613KuoInit,
                dim2614KuoInit,
                dim2615KuoInit,
                dim2616KuoInit,
                dim2617KuoInit,
                dim2618KuoInit,
                dim2619KuoInit,
                dim2620KuoInit,
                dim2621KuoInit,
                dim2622KuoInit,
                dim2623KuoInit,
                dim2624KuoInit,
                dim2625KuoInit,
                dim2626KuoInit,
                dim2627KuoInit,
                dim2628KuoInit,
                dim2629KuoInit,
                dim2630KuoInit,
                dim2631KuoInit,
                dim2632KuoInit,
                dim2633KuoInit,
                dim2634KuoInit,
                dim2635KuoInit,
                dim2636KuoInit,
                dim2637KuoInit,
                dim2638KuoInit,
                dim2639KuoInit,
                dim2640KuoInit,
                dim2641KuoInit,
                dim2642KuoInit,
                dim2643KuoInit,
                dim2644KuoInit,
                dim2645KuoInit,
                dim2646KuoInit,
                dim2647KuoInit,
                dim2648KuoInit,
                dim2649KuoInit,
                dim2650KuoInit,
                dim2651KuoInit,
                dim2652KuoInit,
                dim2653KuoInit,
                dim2654KuoInit,
                dim2655KuoInit,
                dim2656KuoInit,
                dim2657KuoInit,
                dim2658KuoInit,
                dim2659KuoInit,
                dim2660KuoInit,
                dim2661KuoInit,
                dim2662KuoInit,
                dim2663KuoInit,
                dim2664KuoInit,
                dim2665KuoInit,
                dim2666KuoInit,
                dim2667KuoInit,
                dim2668KuoInit,
                dim2669KuoInit,
                dim2670KuoInit,
                dim2671KuoInit,
                dim2672KuoInit,
                dim2673KuoInit,
                dim2674KuoInit,
                dim2675KuoInit,
                dim2676KuoInit,
                dim2677KuoInit,
                dim2678KuoInit,
                dim2679KuoInit,
                dim2680KuoInit,
                dim2681KuoInit,
                dim2682KuoInit,
                dim2683KuoInit,
                dim2684KuoInit,
                dim2685KuoInit,
                dim2686KuoInit,
                dim2687KuoInit,
                dim2688KuoInit,
                dim2689KuoInit,
                dim2690KuoInit,
                dim2691KuoInit,
                dim2692KuoInit,
                dim2693KuoInit,
                dim2694KuoInit,
                dim2695KuoInit,
                dim2696KuoInit,
                dim2697KuoInit,
                dim2698KuoInit,
                dim2699KuoInit,
                dim2700KuoInit,
                dim2701KuoInit,
                dim2702KuoInit,
                dim2703KuoInit,
                dim2704KuoInit,
                dim2705KuoInit,
                dim2706KuoInit,
                dim2707KuoInit,
                dim2708KuoInit,
                dim2709KuoInit,
                dim2710KuoInit,
                dim2711KuoInit,
                dim2712KuoInit,
                dim2713KuoInit,
                dim2714KuoInit,
                dim2715KuoInit,
                dim2716KuoInit,
                dim2717KuoInit,
                dim2718KuoInit,
                dim2719KuoInit,
                dim2720KuoInit,
                dim2721KuoInit,
                dim2722KuoInit,
                dim2723KuoInit,
                dim2724KuoInit,
                dim2725KuoInit,
                dim2726KuoInit,
                dim2727KuoInit,
                dim2728KuoInit,
                dim2729KuoInit,
                dim2730KuoInit,
                dim2731KuoInit,
                dim2732KuoInit,
                dim2733KuoInit,
                dim2734KuoInit,
                dim2735KuoInit,
                dim2736KuoInit,
                dim2737KuoInit,
                dim2738KuoInit,
                dim2739KuoInit,
                dim2740KuoInit,
                dim2741KuoInit,
                dim2742KuoInit,
                dim2743KuoInit,
                dim2744KuoInit,
                dim2745KuoInit,
                dim2746KuoInit,
                dim2747KuoInit,
                dim2748KuoInit,
                dim2749KuoInit,
                dim2750KuoInit,
                dim2751KuoInit,
                dim2752KuoInit,
                dim2753KuoInit,
                dim2754KuoInit,
                dim2755KuoInit,
                dim2756KuoInit,
                dim2757KuoInit,
                dim2758KuoInit,
                dim2759KuoInit,
                dim2760KuoInit,
                dim2761KuoInit,
                dim2762KuoInit,
                dim2763KuoInit,
                dim2764KuoInit,
                dim2765KuoInit,
                dim2766KuoInit,
                dim2767KuoInit,
                dim2768KuoInit,
                dim2769KuoInit,
                dim2770KuoInit,
                dim2771KuoInit,
                dim2772KuoInit,
                dim2773KuoInit,
                dim2774KuoInit,
                dim2775KuoInit,
                dim2776KuoInit,
                dim2777KuoInit,
                dim2778KuoInit,
                dim2779KuoInit,
                dim2780KuoInit,
                dim2781KuoInit,
                dim2782KuoInit,
                dim2783KuoInit,
                dim2784KuoInit,
                dim2785KuoInit,
                dim2786KuoInit,
                dim2787KuoInit,
                dim2788KuoInit,
                dim2789KuoInit,
                dim2790KuoInit,
                dim2791KuoInit,
                dim2792KuoInit,
                dim2793KuoInit,
                dim2794KuoInit,
                dim2795KuoInit,
                dim2796KuoInit,
                dim2797KuoInit,
                dim2798KuoInit,
                dim2799KuoInit,
                dim2800KuoInit,
                dim2801KuoInit,
                dim2802KuoInit,
                dim2803KuoInit,
                dim2804KuoInit,
                dim2805KuoInit,
                dim2806KuoInit,
                dim2807KuoInit,
                dim2808KuoInit,
                dim2809KuoInit,
                dim2810KuoInit,
                dim2811KuoInit,
                dim2812KuoInit,
                dim2813KuoInit,
                dim2814KuoInit,
                dim2815KuoInit,
                dim2816KuoInit,
                dim2817KuoInit,
                dim2818KuoInit,
                dim2819KuoInit,
                dim2820KuoInit,
                dim2821KuoInit,
                dim2822KuoInit,
                dim2823KuoInit,
                dim2824KuoInit,
                dim2825KuoInit,
                dim2826KuoInit,
                dim2827KuoInit,
                dim2828KuoInit,
                dim2829KuoInit,
                dim2830KuoInit,
                dim2831KuoInit,
                dim2832KuoInit,
                dim2833KuoInit,
                dim2834KuoInit,
                dim2835KuoInit,
                dim2836KuoInit,
                dim2837KuoInit,
                dim2838KuoInit,
                dim2839KuoInit,
                dim2840KuoInit,
                dim2841KuoInit,
                dim2842KuoInit,
                dim2843KuoInit,
                dim2844KuoInit,
                dim2845KuoInit,
                dim2846KuoInit,
                dim2847KuoInit,
                dim2848KuoInit,
                dim2849KuoInit,
                dim2850KuoInit,
                dim2851KuoInit,
                dim2852KuoInit,
                dim2853KuoInit,
                dim2854KuoInit,
                dim2855KuoInit,
                dim2856KuoInit,
                dim2857KuoInit,
                dim2858KuoInit,
                dim2859KuoInit,
                dim2860KuoInit,
                dim2861KuoInit,
                dim2862KuoInit,
                dim2863KuoInit,
                dim2864KuoInit,
                dim2865KuoInit,
                dim2866KuoInit,
                dim2867KuoInit,
                dim2868KuoInit,
                dim2869KuoInit,
                dim2870KuoInit,
                dim2871KuoInit,
                dim2872KuoInit,
                dim2873KuoInit,
                dim2874KuoInit,
                dim2875KuoInit,
                dim2876KuoInit,
                dim2877KuoInit,
                dim2878KuoInit,
                dim2879KuoInit,
                dim2880KuoInit,
                dim2881KuoInit,
                dim2882KuoInit,
                dim2883KuoInit,
                dim2884KuoInit,
                dim2885KuoInit,
                dim2886KuoInit,
                dim2887KuoInit,
                dim2888KuoInit,
                dim2889KuoInit,
                dim2890KuoInit,
                dim2891KuoInit,
                dim2892KuoInit,
                dim2893KuoInit,
                dim2894KuoInit,
                dim2895KuoInit,
                dim2896KuoInit,
                dim2897KuoInit,
                dim2898KuoInit,
                dim2899KuoInit,
                dim2900KuoInit,
                dim2901KuoInit,
                dim2902KuoInit,
                dim2903KuoInit,
                dim2904KuoInit,
                dim2905KuoInit,
                dim2906KuoInit,
                dim2907KuoInit,
                dim2908KuoInit,
                dim2909KuoInit,
                dim2910KuoInit,
                dim2911KuoInit,
                dim2912KuoInit,
                dim2913KuoInit,
                dim2914KuoInit,
                dim2915KuoInit,
                dim2916KuoInit,
                dim2917KuoInit,
                dim2918KuoInit,
                dim2919KuoInit,
                dim2920KuoInit,
                dim2921KuoInit,
                dim2922KuoInit,
                dim2923KuoInit,
                dim2924KuoInit,
                dim2925KuoInit,
                dim2926KuoInit,
                dim2927KuoInit,
                dim2928KuoInit,
                dim2929KuoInit,
                dim2930KuoInit,
                dim2931KuoInit,
                dim2932KuoInit,
                dim2933KuoInit,
                dim2934KuoInit,
                dim2935KuoInit,
                dim2936KuoInit,
                dim2937KuoInit,
                dim2938KuoInit,
                dim2939KuoInit,
                dim2940KuoInit,
                dim2941KuoInit,
                dim2942KuoInit,
                dim2943KuoInit,
                dim2944KuoInit,
                dim2945KuoInit,
                dim2946KuoInit,
                dim2947KuoInit,
                dim2948KuoInit,
                dim2949KuoInit,
                dim2950KuoInit,
                dim2951KuoInit,
                dim2952KuoInit,
                dim2953KuoInit,
                dim2954KuoInit,
                dim2955KuoInit,
                dim2956KuoInit,
                dim2957KuoInit,
                dim2958KuoInit,
                dim2959KuoInit,
                dim2960KuoInit,
                dim2961KuoInit,
                dim2962KuoInit,
                dim2963KuoInit,
                dim2964KuoInit,
                dim2965KuoInit,
                dim2966KuoInit,
                dim2967KuoInit,
                dim2968KuoInit,
                dim2969KuoInit,
                dim2970KuoInit,
                dim2971KuoInit,
                dim2972KuoInit,
                dim2973KuoInit,
                dim2974KuoInit,
                dim2975KuoInit,
                dim2976KuoInit,
                dim2977KuoInit,
                dim2978KuoInit,
                dim2979KuoInit,
                dim2980KuoInit,
                dim2981KuoInit,
                dim2982KuoInit,
                dim2983KuoInit,
                dim2984KuoInit,
                dim2985KuoInit,
                dim2986KuoInit,
                dim2987KuoInit,
                dim2988KuoInit,
                dim2989KuoInit,
                dim2990KuoInit,
                dim2991KuoInit,
                dim2992KuoInit,
                dim2993KuoInit,
                dim2994KuoInit,
                dim2995KuoInit,
                dim2996KuoInit,
                dim2997KuoInit,
                dim2998KuoInit,
                dim2999KuoInit,
                dim3000KuoInit,
                dim3001KuoInit,
                dim3002KuoInit,
                dim3003KuoInit,
                dim3004KuoInit,
                dim3005KuoInit,
                dim3006KuoInit,
                dim3007KuoInit,
                dim3008KuoInit,
                dim3009KuoInit,
                dim3010KuoInit,
                dim3011KuoInit,
                dim3012KuoInit,
                dim3013KuoInit,
                dim3014KuoInit,
                dim3015KuoInit,
                dim3016KuoInit,
                dim3017KuoInit,
                dim3018KuoInit,
                dim3019KuoInit,
                dim3020KuoInit,
                dim3021KuoInit,
                dim3022KuoInit,
                dim3023KuoInit,
                dim3024KuoInit,
                dim3025KuoInit,
                dim3026KuoInit,
                dim3027KuoInit,
                dim3028KuoInit,
                dim3029KuoInit,
                dim3030KuoInit,
                dim3031KuoInit,
                dim3032KuoInit,
                dim3033KuoInit,
                dim3034KuoInit,
                dim3035KuoInit,
                dim3036KuoInit,
                dim3037KuoInit,
                dim3038KuoInit,
                dim3039KuoInit,
                dim3040KuoInit,
                dim3041KuoInit,
                dim3042KuoInit,
                dim3043KuoInit,
                dim3044KuoInit,
                dim3045KuoInit,
                dim3046KuoInit,
                dim3047KuoInit,
                dim3048KuoInit,
                dim3049KuoInit,
                dim3050KuoInit,
                dim3051KuoInit,
                dim3052KuoInit,
                dim3053KuoInit,
                dim3054KuoInit,
                dim3055KuoInit,
                dim3056KuoInit,
                dim3057KuoInit,
                dim3058KuoInit,
                dim3059KuoInit,
                dim3060KuoInit,
                dim3061KuoInit,
                dim3062KuoInit,
                dim3063KuoInit,
                dim3064KuoInit,
                dim3065KuoInit,
                dim3066KuoInit,
                dim3067KuoInit,
                dim3068KuoInit,
                dim3069KuoInit,
                dim3070KuoInit,
                dim3071KuoInit,
                dim3072KuoInit,
                dim3073KuoInit,
                dim3074KuoInit,
                dim3075KuoInit,
                dim3076KuoInit,
                dim3077KuoInit,
                dim3078KuoInit,
                dim3079KuoInit,
                dim3080KuoInit,
                dim3081KuoInit,
                dim3082KuoInit,
                dim3083KuoInit,
                dim3084KuoInit,
                dim3085KuoInit,
                dim3086KuoInit,
                dim3087KuoInit,
                dim3088KuoInit,
                dim3089KuoInit,
                dim3090KuoInit,
                dim3091KuoInit,
                dim3092KuoInit,
                dim3093KuoInit,
                dim3094KuoInit,
                dim3095KuoInit,
                dim3096KuoInit,
                dim3097KuoInit,
                dim3098KuoInit,
                dim3099KuoInit,
                dim3100KuoInit,
                dim3101KuoInit,
                dim3102KuoInit,
                dim3103KuoInit,
                dim3104KuoInit,
                dim3105KuoInit,
                dim3106KuoInit,
                dim3107KuoInit,
                dim3108KuoInit,
                dim3109KuoInit,
                dim3110KuoInit,
                dim3111KuoInit,
                dim3112KuoInit,
                dim3113KuoInit,
                dim3114KuoInit,
                dim3115KuoInit,
                dim3116KuoInit,
                dim3117KuoInit,
                dim3118KuoInit,
                dim3119KuoInit,
                dim3120KuoInit,
                dim3121KuoInit,
                dim3122KuoInit,
                dim3123KuoInit,
                dim3124KuoInit,
                dim3125KuoInit,
                dim3126KuoInit,
                dim3127KuoInit,
                dim3128KuoInit,
                dim3129KuoInit,
                dim3130KuoInit,
                dim3131KuoInit,
                dim3132KuoInit,
                dim3133KuoInit,
                dim3134KuoInit,
                dim3135KuoInit,
                dim3136KuoInit,
                dim3137KuoInit,
                dim3138KuoInit,
                dim3139KuoInit,
                dim3140KuoInit,
                dim3141KuoInit,
                dim3142KuoInit,
                dim3143KuoInit,
                dim3144KuoInit,
                dim3145KuoInit,
                dim3146KuoInit,
                dim3147KuoInit,
                dim3148KuoInit,
                dim3149KuoInit,
                dim3150KuoInit,
                dim3151KuoInit,
                dim3152KuoInit,
                dim3153KuoInit,
                dim3154KuoInit,
                dim3155KuoInit,
                dim3156KuoInit,
                dim3157KuoInit,
                dim3158KuoInit,
                dim3159KuoInit,
                dim3160KuoInit,
                dim3161KuoInit,
                dim3162KuoInit,
                dim3163KuoInit,
                dim3164KuoInit,
                dim3165KuoInit,
                dim3166KuoInit,
                dim3167KuoInit,
                dim3168KuoInit,
                dim3169KuoInit,
                dim3170KuoInit,
                dim3171KuoInit,
                dim3172KuoInit,
                dim3173KuoInit,
                dim3174KuoInit,
                dim3175KuoInit,
                dim3176KuoInit,
                dim3177KuoInit,
                dim3178KuoInit,
                dim3179KuoInit,
                dim3180KuoInit,
                dim3181KuoInit,
                dim3182KuoInit,
                dim3183KuoInit,
                dim3184KuoInit,
                dim3185KuoInit,
                dim3186KuoInit,
                dim3187KuoInit,
                dim3188KuoInit,
                dim3189KuoInit,
                dim3190KuoInit,
                dim3191KuoInit,
                dim3192KuoInit,
                dim3193KuoInit,
                dim3194KuoInit,
                dim3195KuoInit,
                dim3196KuoInit,
                dim3197KuoInit,
                dim3198KuoInit,
                dim3199KuoInit,
                dim3200KuoInit,
                dim3201KuoInit,
                dim3202KuoInit,
                dim3203KuoInit,
                dim3204KuoInit,
                dim3205KuoInit,
                dim3206KuoInit,
                dim3207KuoInit,
                dim3208KuoInit,
                dim3209KuoInit,
                dim3210KuoInit,
                dim3211KuoInit,
                dim3212KuoInit,
                dim3213KuoInit,
                dim3214KuoInit,
                dim3215KuoInit,
                dim3216KuoInit,
                dim3217KuoInit,
                dim3218KuoInit,
                dim3219KuoInit,
                dim3220KuoInit,
                dim3221KuoInit,
                dim3222KuoInit,
                dim3223KuoInit,
                dim3224KuoInit,
                dim3225KuoInit,
                dim3226KuoInit,
                dim3227KuoInit,
                dim3228KuoInit,
                dim3229KuoInit,
                dim3230KuoInit,
                dim3231KuoInit,
                dim3232KuoInit,
                dim3233KuoInit,
                dim3234KuoInit,
                dim3235KuoInit,
                dim3236KuoInit,
                dim3237KuoInit,
                dim3238KuoInit,
                dim3239KuoInit,
                dim3240KuoInit,
                dim3241KuoInit,
                dim3242KuoInit,
                dim3243KuoInit,
                dim3244KuoInit,
                dim3245KuoInit,
                dim3246KuoInit,
                dim3247KuoInit,
                dim3248KuoInit,
                dim3249KuoInit,
                dim3250KuoInit,
                dim3251KuoInit,
                dim3252KuoInit,
                dim3253KuoInit,
                dim3254KuoInit,
                dim3255KuoInit,
                dim3256KuoInit,
                dim3257KuoInit,
                dim3258KuoInit,
                dim3259KuoInit,
                dim3260KuoInit,
                dim3261KuoInit,
                dim3262KuoInit,
                dim3263KuoInit,
                dim3264KuoInit,
                dim3265KuoInit,
                dim3266KuoInit,
                dim3267KuoInit,
                dim3268KuoInit,
                dim3269KuoInit,
                dim3270KuoInit,
                dim3271KuoInit,
                dim3272KuoInit,
                dim3273KuoInit,
                dim3274KuoInit,
                dim3275KuoInit,
                dim3276KuoInit,
                dim3277KuoInit,
                dim3278KuoInit,
                dim3279KuoInit,
                dim3280KuoInit,
                dim3281KuoInit,
                dim3282KuoInit,
                dim3283KuoInit,
                dim3284KuoInit,
                dim3285KuoInit,
                dim3286KuoInit,
                dim3287KuoInit,
                dim3288KuoInit,
                dim3289KuoInit,
                dim3290KuoInit,
                dim3291KuoInit,
                dim3292KuoInit,
                dim3293KuoInit,
                dim3294KuoInit,
                dim3295KuoInit,
                dim3296KuoInit,
                dim3297KuoInit,
                dim3298KuoInit,
                dim3299KuoInit,
                dim3300KuoInit,
                dim3301KuoInit,
                dim3302KuoInit,
                dim3303KuoInit,
                dim3304KuoInit,
                dim3305KuoInit,
                dim3306KuoInit,
                dim3307KuoInit,
                dim3308KuoInit,
                dim3309KuoInit,
                dim3310KuoInit,
                dim3311KuoInit,
                dim3312KuoInit,
                dim3313KuoInit,
                dim3314KuoInit,
                dim3315KuoInit,
                dim3316KuoInit,
                dim3317KuoInit,
                dim3318KuoInit,
                dim3319KuoInit,
                dim3320KuoInit,
                dim3321KuoInit,
                dim3322KuoInit,
                dim3323KuoInit,
                dim3324KuoInit,
                dim3325KuoInit,
                dim3326KuoInit,
                dim3327KuoInit,
                dim3328KuoInit,
                dim3329KuoInit,
                dim3330KuoInit,
                dim3331KuoInit,
                dim3332KuoInit,
                dim3333KuoInit,
                dim3334KuoInit,
                dim3335KuoInit,
                dim3336KuoInit,
                dim3337KuoInit,
                dim3338KuoInit,
                dim3339KuoInit,
                dim3340KuoInit,
                dim3341KuoInit,
                dim3342KuoInit,
                dim3343KuoInit,
                dim3344KuoInit,
                dim3345KuoInit,
                dim3346KuoInit,
                dim3347KuoInit,
                dim3348KuoInit,
                dim3349KuoInit,
                dim3350KuoInit,
                dim3351KuoInit,
                dim3352KuoInit,
                dim3353KuoInit,
                dim3354KuoInit,
                dim3355KuoInit,
                dim3356KuoInit,
                dim3357KuoInit,
                dim3358KuoInit,
                dim3359KuoInit,
                dim3360KuoInit,
                dim3361KuoInit,
                dim3362KuoInit,
                dim3363KuoInit,
                dim3364KuoInit,
                dim3365KuoInit,
                dim3366KuoInit,
                dim3367KuoInit,
                dim3368KuoInit,
                dim3369KuoInit,
                dim3370KuoInit,
                dim3371KuoInit,
                dim3372KuoInit,
                dim3373KuoInit,
                dim3374KuoInit,
                dim3375KuoInit,
                dim3376KuoInit,
                dim3377KuoInit,
                dim3378KuoInit,
                dim3379KuoInit,
                dim3380KuoInit,
                dim3381KuoInit,
                dim3382KuoInit,
                dim3383KuoInit,
                dim3384KuoInit,
                dim3385KuoInit,
                dim3386KuoInit,
                dim3387KuoInit,
                dim3388KuoInit,
                dim3389KuoInit,
                dim3390KuoInit,
                dim3391KuoInit,
                dim3392KuoInit,
                dim3393KuoInit,
                dim3394KuoInit,
                dim3395KuoInit,
                dim3396KuoInit,
                dim3397KuoInit,
                dim3398KuoInit,
                dim3399KuoInit,
                dim3400KuoInit,
                dim3401KuoInit,
                dim3402KuoInit,
                dim3403KuoInit,
                dim3404KuoInit,
                dim3405KuoInit,
                dim3406KuoInit,
                dim3407KuoInit,
                dim3408KuoInit,
                dim3409KuoInit,
                dim3410KuoInit,
                dim3411KuoInit,
                dim3412KuoInit,
                dim3413KuoInit,
                dim3414KuoInit,
                dim3415KuoInit,
                dim3416KuoInit,
                dim3417KuoInit,
                dim3418KuoInit,
                dim3419KuoInit,
                dim3420KuoInit,
                dim3421KuoInit,
                dim3422KuoInit,
                dim3423KuoInit,
                dim3424KuoInit,
                dim3425KuoInit,
                dim3426KuoInit,
                dim3427KuoInit,
                dim3428KuoInit,
                dim3429KuoInit,
                dim3430KuoInit,
                dim3431KuoInit,
                dim3432KuoInit,
                dim3433KuoInit,
                dim3434KuoInit,
                dim3435KuoInit,
                dim3436KuoInit,
                dim3437KuoInit,
                dim3438KuoInit,
                dim3439KuoInit,
                dim3440KuoInit,
                dim3441KuoInit,
                dim3442KuoInit,
                dim3443KuoInit,
                dim3444KuoInit,
                dim3445KuoInit,
                dim3446KuoInit,
                dim3447KuoInit,
                dim3448KuoInit,
                dim3449KuoInit,
                dim3450KuoInit,
                dim3451KuoInit,
                dim3452KuoInit,
                dim3453KuoInit,
                dim3454KuoInit,
                dim3455KuoInit,
                dim3456KuoInit,
                dim3457KuoInit,
                dim3458KuoInit,
                dim3459KuoInit,
                dim3460KuoInit,
                dim3461KuoInit,
                dim3462KuoInit,
                dim3463KuoInit,
                dim3464KuoInit,
                dim3465KuoInit,
                dim3466KuoInit,
                dim3467KuoInit,
                dim3468KuoInit,
                dim3469KuoInit,
                dim3470KuoInit,
                dim3471KuoInit,
                dim3472KuoInit,
                dim3473KuoInit,
                dim3474KuoInit,
                dim3475KuoInit,
                dim3476KuoInit,
                dim3477KuoInit,
                dim3478KuoInit,
                dim3479KuoInit,
                dim3480KuoInit,
                dim3481KuoInit,
                dim3482KuoInit,
                dim3483KuoInit,
                dim3484KuoInit,
                dim3485KuoInit,
                dim3486KuoInit,
                dim3487KuoInit,
                dim3488KuoInit,
                dim3489KuoInit,
                dim3490KuoInit,
                dim3491KuoInit,
                dim3492KuoInit,
                dim3493KuoInit,
                dim3494KuoInit,
                dim3495KuoInit,
                dim3496KuoInit,
                dim3497KuoInit,
                dim3498KuoInit,
                dim3499KuoInit,
                dim3500KuoInit,
                dim3501KuoInit,
                dim3502KuoInit,
                dim3503KuoInit,
                dim3504KuoInit,
                dim3505KuoInit,
                dim3506KuoInit,
                dim3507KuoInit,
                dim3508KuoInit,
                dim3509KuoInit,
                dim3510KuoInit,
                dim3511KuoInit,
                dim3512KuoInit,
                dim3513KuoInit,
                dim3514KuoInit,
                dim3515KuoInit,
                dim3516KuoInit,
                dim3517KuoInit,
                dim3518KuoInit,
                dim3519KuoInit,
                dim3520KuoInit,
                dim3521KuoInit,
                dim3522KuoInit,
                dim3523KuoInit,
                dim3524KuoInit,
                dim3525KuoInit,
                dim3526KuoInit,
                dim3527KuoInit,
                dim3528KuoInit,
                dim3529KuoInit,
                dim3530KuoInit,
                dim3531KuoInit,
                dim3532KuoInit,
                dim3533KuoInit,
                dim3534KuoInit,
                dim3535KuoInit,
                dim3536KuoInit,
                dim3537KuoInit,
                dim3538KuoInit,
                dim3539KuoInit,
                dim3540KuoInit,
                dim3541KuoInit,
                dim3542KuoInit,
                dim3543KuoInit,
                dim3544KuoInit,
                dim3545KuoInit,
                dim3546KuoInit,
                dim3547KuoInit,
                dim3548KuoInit,
                dim3549KuoInit,
                dim3550KuoInit,
                dim3551KuoInit,
                dim3552KuoInit,
                dim3553KuoInit,
                dim3554KuoInit,
                dim3555KuoInit,
                dim3556KuoInit,
                dim3557KuoInit,
                dim3558KuoInit,
                dim3559KuoInit,
                dim3560KuoInit,
                dim3561KuoInit,
                dim3562KuoInit,
                dim3563KuoInit,
                dim3564KuoInit,
                dim3565KuoInit,
                dim3566KuoInit,
                dim3567KuoInit,
                dim3568KuoInit,
                dim3569KuoInit,
                dim3570KuoInit,
                dim3571KuoInit,
                dim3572KuoInit,
                dim3573KuoInit,
                dim3574KuoInit,
                dim3575KuoInit,
                dim3576KuoInit,
                dim3577KuoInit,
                dim3578KuoInit,
                dim3579KuoInit,
                dim3580KuoInit,
                dim3581KuoInit,
                dim3582KuoInit,
                dim3583KuoInit,
                dim3584KuoInit,
                dim3585KuoInit,
                dim3586KuoInit,
                dim3587KuoInit,
                dim3588KuoInit,
                dim3589KuoInit,
                dim3590KuoInit,
                dim3591KuoInit,
                dim3592KuoInit,
                dim3593KuoInit,
                dim3594KuoInit,
                dim3595KuoInit,
                dim3596KuoInit,
                dim3597KuoInit,
                dim3598KuoInit,
                dim3599KuoInit,
                dim3600KuoInit,
                dim3601KuoInit,
                dim3602KuoInit,
                dim3603KuoInit,
                dim3604KuoInit,
                dim3605KuoInit,
                dim3606KuoInit,
                dim3607KuoInit,
                dim3608KuoInit,
                dim3609KuoInit,
                dim3610KuoInit,
                dim3611KuoInit,
                dim3612KuoInit,
                dim3613KuoInit,
                dim3614KuoInit,
                dim3615KuoInit,
                dim3616KuoInit,
                dim3617KuoInit,
                dim3618KuoInit,
                dim3619KuoInit,
                dim3620KuoInit,
                dim3621KuoInit,
                dim3622KuoInit,
                dim3623KuoInit,
                dim3624KuoInit,
                dim3625KuoInit,
                dim3626KuoInit,
                dim3627KuoInit,
                dim3628KuoInit,
                dim3629KuoInit,
                dim3630KuoInit,
                dim3631KuoInit,
                dim3632KuoInit,
                dim3633KuoInit,
                dim3634KuoInit,
                dim3635KuoInit,
                dim3636KuoInit,
                dim3637KuoInit,
                dim3638KuoInit,
                dim3639KuoInit,
                dim3640KuoInit,
                dim3641KuoInit,
                dim3642KuoInit,
                dim3643KuoInit,
                dim3644KuoInit,
                dim3645KuoInit,
                dim3646KuoInit,
                dim3647KuoInit,
                dim3648KuoInit,
                dim3649KuoInit,
                dim3650KuoInit,
                dim3651KuoInit,
                dim3652KuoInit,
                dim3653KuoInit,
                dim3654KuoInit,
                dim3655KuoInit,
                dim3656KuoInit,
                dim3657KuoInit,
                dim3658KuoInit,
                dim3659KuoInit,
                dim3660KuoInit,
                dim3661KuoInit,
                dim3662KuoInit,
                dim3663KuoInit,
                dim3664KuoInit,
                dim3665KuoInit,
                dim3666KuoInit,
                dim3667KuoInit,
                dim3668KuoInit,
                dim3669KuoInit,
                dim3670KuoInit,
                dim3671KuoInit,
                dim3672KuoInit,
                dim3673KuoInit,
                dim3674KuoInit,
                dim3675KuoInit,
                dim3676KuoInit,
                dim3677KuoInit,
                dim3678KuoInit,
                dim3679KuoInit,
                dim3680KuoInit,
                dim3681KuoInit,
                dim3682KuoInit,
                dim3683KuoInit,
                dim3684KuoInit,
                dim3685KuoInit,
                dim3686KuoInit,
                dim3687KuoInit,
                dim3688KuoInit,
                dim3689KuoInit,
                dim3690KuoInit,
                dim3691KuoInit,
                dim3692KuoInit,
                dim3693KuoInit,
                dim3694KuoInit,
                dim3695KuoInit,
                dim3696KuoInit,
                dim3697KuoInit,
                dim3698KuoInit,
                dim3699KuoInit,
                dim3700KuoInit,
                dim3701KuoInit,
                dim3702KuoInit,
                dim3703KuoInit,
                dim3704KuoInit,
                dim3705KuoInit,
                dim3706KuoInit,
                dim3707KuoInit,
                dim3708KuoInit,
                dim3709KuoInit,
                dim3710KuoInit,
                dim3711KuoInit,
                dim3712KuoInit,
                dim3713KuoInit,
                dim3714KuoInit,
                dim3715KuoInit,
                dim3716KuoInit,
                dim3717KuoInit,
                dim3718KuoInit,
                dim3719KuoInit,
                dim3720KuoInit,
                dim3721KuoInit,
                dim3722KuoInit,
                dim3723KuoInit,
                dim3724KuoInit,
                dim3725KuoInit,
                dim3726KuoInit,
                dim3727KuoInit,
                dim3728KuoInit,
                dim3729KuoInit,
                dim3730KuoInit,
                dim3731KuoInit,
                dim3732KuoInit,
                dim3733KuoInit,
                dim3734KuoInit,
                dim3735KuoInit,
                dim3736KuoInit,
                dim3737KuoInit,
                dim3738KuoInit,
                dim3739KuoInit,
                dim3740KuoInit,
                dim3741KuoInit,
                dim3742KuoInit,
                dim3743KuoInit,
                dim3744KuoInit,
                dim3745KuoInit,
                dim3746KuoInit,
                dim3747KuoInit,
                dim3748KuoInit,
                dim3749KuoInit,
                dim3750KuoInit,
                dim3751KuoInit,
                dim3752KuoInit,
                dim3753KuoInit,
                dim3754KuoInit,
                dim3755KuoInit,
                dim3756KuoInit,
                dim3757KuoInit,
                dim3758KuoInit,
                dim3759KuoInit,
                dim3760KuoInit,
                dim3761KuoInit,
                dim3762KuoInit,
                dim3763KuoInit,
                dim3764KuoInit,
                dim3765KuoInit,
                dim3766KuoInit,
                dim3767KuoInit,
                dim3768KuoInit,
                dim3769KuoInit,
                dim3770KuoInit,
                dim3771KuoInit,
                dim3772KuoInit,
                dim3773KuoInit,
                dim3774KuoInit,
                dim3775KuoInit,
                dim3776KuoInit,
                dim3777KuoInit,
                dim3778KuoInit,
                dim3779KuoInit,
                dim3780KuoInit,
                dim3781KuoInit,
                dim3782KuoInit,
                dim3783KuoInit,
                dim3784KuoInit,
                dim3785KuoInit,
                dim3786KuoInit,
                dim3787KuoInit,
                dim3788KuoInit,
                dim3789KuoInit,
                dim3790KuoInit,
                dim3791KuoInit,
                dim3792KuoInit,
                dim3793KuoInit,
                dim3794KuoInit,
                dim3795KuoInit,
                dim3796KuoInit,
                dim3797KuoInit,
                dim3798KuoInit,
                dim3799KuoInit,
                dim3800KuoInit,
                dim3801KuoInit,
                dim3802KuoInit,
                dim3803KuoInit,
                dim3804KuoInit,
                dim3805KuoInit,
                dim3806KuoInit,
                dim3807KuoInit,
                dim3808KuoInit,
                dim3809KuoInit,
                dim3810KuoInit,
                dim3811KuoInit,
                dim3812KuoInit,
                dim3813KuoInit,
                dim3814KuoInit,
                dim3815KuoInit,
                dim3816KuoInit,
                dim3817KuoInit,
                dim3818KuoInit,
                dim3819KuoInit,
                dim3820KuoInit,
                dim3821KuoInit,
                dim3822KuoInit,
                dim3823KuoInit,
                dim3824KuoInit,
                dim3825KuoInit,
                dim3826KuoInit,
                dim3827KuoInit,
                dim3828KuoInit,
                dim3829KuoInit,
                dim3830KuoInit,
                dim3831KuoInit,
                dim3832KuoInit,
                dim3833KuoInit,
                dim3834KuoInit,
                dim3835KuoInit,
                dim3836KuoInit,
                dim3837KuoInit,
                dim3838KuoInit,
                dim3839KuoInit,
                dim3840KuoInit,
                dim3841KuoInit,
                dim3842KuoInit,
                dim3843KuoInit,
                dim3844KuoInit,
                dim3845KuoInit,
                dim3846KuoInit,
                dim3847KuoInit,
                dim3848KuoInit,
                dim3849KuoInit,
                dim3850KuoInit,
                dim3851KuoInit,
                dim3852KuoInit,
                dim3853KuoInit,
                dim3854KuoInit,
                dim3855KuoInit,
                dim3856KuoInit,
                dim3857KuoInit,
                dim3858KuoInit,
                dim3859KuoInit,
                dim3860KuoInit,
                dim3861KuoInit,
                dim3862KuoInit,
                dim3863KuoInit,
                dim3864KuoInit,
                dim3865KuoInit,
                dim3866KuoInit,
                dim3867KuoInit,
                dim3868KuoInit,
                dim3869KuoInit,
                dim3870KuoInit,
                dim3871KuoInit,
                dim3872KuoInit,
                dim3873KuoInit,
                dim3874KuoInit,
                dim3875KuoInit,
                dim3876KuoInit,
                dim3877KuoInit,
                dim3878KuoInit,
                dim3879KuoInit,
                dim3880KuoInit,
                dim3881KuoInit,
                dim3882KuoInit,
                dim3883KuoInit,
                dim3884KuoInit,
                dim3885KuoInit,
                dim3886KuoInit,
                dim3887KuoInit,
                dim3888KuoInit,
                dim3889KuoInit,
                dim3890KuoInit,
                dim3891KuoInit,
                dim3892KuoInit,
                dim3893KuoInit,
                dim3894KuoInit,
                dim3895KuoInit,
                dim3896KuoInit,
                dim3897KuoInit,
                dim3898KuoInit,
                dim3899KuoInit,
                dim3900KuoInit,
                dim3901KuoInit,
                dim3902KuoInit,
                dim3903KuoInit,
                dim3904KuoInit,
                dim3905KuoInit,
                dim3906KuoInit,
                dim3907KuoInit,
                dim3908KuoInit,
                dim3909KuoInit,
                dim3910KuoInit,
                dim3911KuoInit,
                dim3912KuoInit,
                dim3913KuoInit,
                dim3914KuoInit,
                dim3915KuoInit,
                dim3916KuoInit,
                dim3917KuoInit,
                dim3918KuoInit,
                dim3919KuoInit,
                dim3920KuoInit,
                dim3921KuoInit,
                dim3922KuoInit,
                dim3923KuoInit,
                dim3924KuoInit,
                dim3925KuoInit,
                dim3926KuoInit,
                dim3927KuoInit,
                dim3928KuoInit,
                dim3929KuoInit,
                dim3930KuoInit,
                dim3931KuoInit,
                dim3932KuoInit,
                dim3933KuoInit,
                dim3934KuoInit,
                dim3935KuoInit,
                dim3936KuoInit,
                dim3937KuoInit,
                dim3938KuoInit,
                dim3939KuoInit,
                dim3940KuoInit,
                dim3941KuoInit,
                dim3942KuoInit,
                dim3943KuoInit,
                dim3944KuoInit,
                dim3945KuoInit,
                dim3946KuoInit,
                dim3947KuoInit,
                dim3948KuoInit,
                dim3949KuoInit,
                dim3950KuoInit,
                dim3951KuoInit,
                dim3952KuoInit,
                dim3953KuoInit,
                dim3954KuoInit,
                dim3955KuoInit,
                dim3956KuoInit,
                dim3957KuoInit,
                dim3958KuoInit,
                dim3959KuoInit,
                dim3960KuoInit,
                dim3961KuoInit,
                dim3962KuoInit,
                dim3963KuoInit,
                dim3964KuoInit,
                dim3965KuoInit,
                dim3966KuoInit,
                dim3967KuoInit,
                dim3968KuoInit,
                dim3969KuoInit,
                dim3970KuoInit,
                dim3971KuoInit,
                dim3972KuoInit,
                dim3973KuoInit,
                dim3974KuoInit,
                dim3975KuoInit,
                dim3976KuoInit,
                dim3977KuoInit,
                dim3978KuoInit,
                dim3979KuoInit,
                dim3980KuoInit,
                dim3981KuoInit,
                dim3982KuoInit,
                dim3983KuoInit,
                dim3984KuoInit,
                dim3985KuoInit,
                dim3986KuoInit,
                dim3987KuoInit,
                dim3988KuoInit,
                dim3989KuoInit,
                dim3990KuoInit,
                dim3991KuoInit,
                dim3992KuoInit,
                dim3993KuoInit,
                dim3994KuoInit,
                dim3995KuoInit,
                dim3996KuoInit,
                dim3997KuoInit,
                dim3998KuoInit,
                dim3999KuoInit,
                dim4000KuoInit,
                dim4001KuoInit,
                dim4002KuoInit,
                dim4003KuoInit,
                dim4004KuoInit,
                dim4005KuoInit,
                dim4006KuoInit,
                dim4007KuoInit,
                dim4008KuoInit,
                dim4009KuoInit,
                dim4010KuoInit,
                dim4011KuoInit,
                dim4012KuoInit,
                dim4013KuoInit,
                dim4014KuoInit,
                dim4015KuoInit,
                dim4016KuoInit,
                dim4017KuoInit,
                dim4018KuoInit,
                dim4019KuoInit,
                dim4020KuoInit,
                dim4021KuoInit,
                dim4022KuoInit,
                dim4023KuoInit,
                dim4024KuoInit,
                dim4025KuoInit,
                dim4026KuoInit,
                dim4027KuoInit,
                dim4028KuoInit,
                dim4029KuoInit,
                dim4030KuoInit,
                dim4031KuoInit,
                dim4032KuoInit,
                dim4033KuoInit,
                dim4034KuoInit,
                dim4035KuoInit,
                dim4036KuoInit,
                dim4037KuoInit,
                dim4038KuoInit,
                dim4039KuoInit,
                dim4040KuoInit,
                dim4041KuoInit,
                dim4042KuoInit,
                dim4043KuoInit,
                dim4044KuoInit,
                dim4045KuoInit,
                dim4046KuoInit,
                dim4047KuoInit,
                dim4048KuoInit,
                dim4049KuoInit,
                dim4050KuoInit,
                dim4051KuoInit,
                dim4052KuoInit,
                dim4053KuoInit,
                dim4054KuoInit,
                dim4055KuoInit,
                dim4056KuoInit,
                dim4057KuoInit,
                dim4058KuoInit,
                dim4059KuoInit,
                dim4060KuoInit,
                dim4061KuoInit,
                dim4062KuoInit,
                dim4063KuoInit,
                dim4064KuoInit,
                dim4065KuoInit,
                dim4066KuoInit,
                dim4067KuoInit,
                dim4068KuoInit,
                dim4069KuoInit,
                dim4070KuoInit,
                dim4071KuoInit,
                dim4072KuoInit,
                dim4073KuoInit,
                dim4074KuoInit,
                dim4075KuoInit,
                dim4076KuoInit,
                dim4077KuoInit,
                dim4078KuoInit,
                dim4079KuoInit,
                dim4080KuoInit,
                dim4081KuoInit,
                dim4082KuoInit,
                dim4083KuoInit,
                dim4084KuoInit,
                dim4085KuoInit,
                dim4086KuoInit,
                dim4087KuoInit,
                dim4088KuoInit,
                dim4089KuoInit,
                dim4090KuoInit,
                dim4091KuoInit,
                dim4092KuoInit,
                dim4093KuoInit,
                dim4094KuoInit,
                dim4095KuoInit,
                dim4096KuoInit,
                dim4097KuoInit,
                dim4098KuoInit,
                dim4099KuoInit,
                dim4100KuoInit,
                dim4101KuoInit,
                dim4102KuoInit,
                dim4103KuoInit,
                dim4104KuoInit,
                dim4105KuoInit,
                dim4106KuoInit,
                dim4107KuoInit,
                dim4108KuoInit,
                dim4109KuoInit,
                dim4110KuoInit,
                dim4111KuoInit,
                dim4112KuoInit,
                dim4113KuoInit,
                dim4114KuoInit,
                dim4115KuoInit,
                dim4116KuoInit,
                dim4117KuoInit,
                dim4118KuoInit,
                dim4119KuoInit,
                dim4120KuoInit,
                dim4121KuoInit,
                dim4122KuoInit,
                dim4123KuoInit,
                dim4124KuoInit,
                dim4125KuoInit,
                dim4126KuoInit,
                dim4127KuoInit,
                dim4128KuoInit,
                dim4129KuoInit,
                dim4130KuoInit,
                dim4131KuoInit,
                dim4132KuoInit,
                dim4133KuoInit,
                dim4134KuoInit,
                dim4135KuoInit,
                dim4136KuoInit,
                dim4137KuoInit,
                dim4138KuoInit,
                dim4139KuoInit,
                dim4140KuoInit,
                dim4141KuoInit,
                dim4142KuoInit,
                dim4143KuoInit,
                dim4144KuoInit,
                dim4145KuoInit,
                dim4146KuoInit,
                dim4147KuoInit,
                dim4148KuoInit,
                dim4149KuoInit,
                dim4150KuoInit,
                dim4151KuoInit,
                dim4152KuoInit,
                dim4153KuoInit,
                dim4154KuoInit,
                dim4155KuoInit,
                dim4156KuoInit,
                dim4157KuoInit,
                dim4158KuoInit,
                dim4159KuoInit,
                dim4160KuoInit,
                dim4161KuoInit,
                dim4162KuoInit,
                dim4163KuoInit,
                dim4164KuoInit,
                dim4165KuoInit,
                dim4166KuoInit,
                dim4167KuoInit,
                dim4168KuoInit,
                dim4169KuoInit,
                dim4170KuoInit,
                dim4171KuoInit,
                dim4172KuoInit,
                dim4173KuoInit,
                dim4174KuoInit,
                dim4175KuoInit,
                dim4176KuoInit,
                dim4177KuoInit,
                dim4178KuoInit,
                dim4179KuoInit,
                dim4180KuoInit,
                dim4181KuoInit,
                dim4182KuoInit,
                dim4183KuoInit,
                dim4184KuoInit,
                dim4185KuoInit,
                dim4186KuoInit,
                dim4187KuoInit,
                dim4188KuoInit,
                dim4189KuoInit,
                dim4190KuoInit,
                dim4191KuoInit,
                dim4192KuoInit,
                dim4193KuoInit,
                dim4194KuoInit,
                dim4195KuoInit,
                dim4196KuoInit,
                dim4197KuoInit,
                dim4198KuoInit,
                dim4199KuoInit,
                dim4200KuoInit,
                dim4201KuoInit,
                dim4202KuoInit,
                dim4203KuoInit,
                dim4204KuoInit,
                dim4205KuoInit,
                dim4206KuoInit,
                dim4207KuoInit,
                dim4208KuoInit,
                dim4209KuoInit,
                dim4210KuoInit,
                dim4211KuoInit,
                dim4212KuoInit,
                dim4213KuoInit,
                dim4214KuoInit,
                dim4215KuoInit,
                dim4216KuoInit,
                dim4217KuoInit,
                dim4218KuoInit,
                dim4219KuoInit,
                dim4220KuoInit,
                dim4221KuoInit,
                dim4222KuoInit,
                dim4223KuoInit,
                dim4224KuoInit,
                dim4225KuoInit,
                dim4226KuoInit,
                dim4227KuoInit,
                dim4228KuoInit,
                dim4229KuoInit,
                dim4230KuoInit,
                dim4231KuoInit,
                dim4232KuoInit,
                dim4233KuoInit,
                dim4234KuoInit,
                dim4235KuoInit,
                dim4236KuoInit,
                dim4237KuoInit,
                dim4238KuoInit,
                dim4239KuoInit,
                dim4240KuoInit,
                dim4241KuoInit,
                dim4242KuoInit,
                dim4243KuoInit,
                dim4244KuoInit,
                dim4245KuoInit,
                dim4246KuoInit,
                dim4247KuoInit,
                dim4248KuoInit,
                dim4249KuoInit,
                dim4250KuoInit,
                dim4251KuoInit,
                dim4252KuoInit,
                dim4253KuoInit,
                dim4254KuoInit,
                dim4255KuoInit,
                dim4256KuoInit,
                dim4257KuoInit,
                dim4258KuoInit,
                dim4259KuoInit,
                dim4260KuoInit,
                dim4261KuoInit,
                dim4262KuoInit,
                dim4263KuoInit,
                dim4264KuoInit,
                dim4265KuoInit,
                dim4266KuoInit,
                dim4267KuoInit,
                dim4268KuoInit,
                dim4269KuoInit,
                dim4270KuoInit,
                dim4271KuoInit,
                dim4272KuoInit,
                dim4273KuoInit,
                dim4274KuoInit,
                dim4275KuoInit,
                dim4276KuoInit,
                dim4277KuoInit,
                dim4278KuoInit,
                dim4279KuoInit,
                dim4280KuoInit,
                dim4281KuoInit,
                dim4282KuoInit,
                dim4283KuoInit,
                dim4284KuoInit,
                dim4285KuoInit,
                dim4286KuoInit,
                dim4287KuoInit,
                dim4288KuoInit,
                dim4289KuoInit,
                dim4290KuoInit,
                dim4291KuoInit,
                dim4292KuoInit,
                dim4293KuoInit,
                dim4294KuoInit,
                dim4295KuoInit,
                dim4296KuoInit,
                dim4297KuoInit,
                dim4298KuoInit,
                dim4299KuoInit,
                dim4300KuoInit,
                dim4301KuoInit,
                dim4302KuoInit,
                dim4303KuoInit,
                dim4304KuoInit,
                dim4305KuoInit,
                dim4306KuoInit,
                dim4307KuoInit,
                dim4308KuoInit,
                dim4309KuoInit,
                dim4310KuoInit,
                dim4311KuoInit,
                dim4312KuoInit,
                dim4313KuoInit,
                dim4314KuoInit,
                dim4315KuoInit,
                dim4316KuoInit,
                dim4317KuoInit,
                dim4318KuoInit,
                dim4319KuoInit,
                dim4320KuoInit,
                dim4321KuoInit,
                dim4322KuoInit,
                dim4323KuoInit,
                dim4324KuoInit,
                dim4325KuoInit,
                dim4326KuoInit,
                dim4327KuoInit,
                dim4328KuoInit,
                dim4329KuoInit,
                dim4330KuoInit,
                dim4331KuoInit,
                dim4332KuoInit,
                dim4333KuoInit,
                dim4334KuoInit,
                dim4335KuoInit,
                dim4336KuoInit,
                dim4337KuoInit,
                dim4338KuoInit,
                dim4339KuoInit,
                dim4340KuoInit,
                dim4341KuoInit,
                dim4342KuoInit,
                dim4343KuoInit,
                dim4344KuoInit,
                dim4345KuoInit,
                dim4346KuoInit,
                dim4347KuoInit,
                dim4348KuoInit,
                dim4349KuoInit,
                dim4350KuoInit,
                dim4351KuoInit,
                dim4352KuoInit,
                dim4353KuoInit,
                dim4354KuoInit,
                dim4355KuoInit,
                dim4356KuoInit,
                dim4357KuoInit,
                dim4358KuoInit,
                dim4359KuoInit,
                dim4360KuoInit,
                dim4361KuoInit,
                dim4362KuoInit,
                dim4363KuoInit,
                dim4364KuoInit,
                dim4365KuoInit,
                dim4366KuoInit,
                dim4367KuoInit,
                dim4368KuoInit,
                dim4369KuoInit,
                dim4370KuoInit,
                dim4371KuoInit,
                dim4372KuoInit,
                dim4373KuoInit,
                dim4374KuoInit,
                dim4375KuoInit,
                dim4376KuoInit,
                dim4377KuoInit,
                dim4378KuoInit,
                dim4379KuoInit,
                dim4380KuoInit,
                dim4381KuoInit,
                dim4382KuoInit,
                dim4383KuoInit,
                dim4384KuoInit,
                dim4385KuoInit,
                dim4386KuoInit,
                dim4387KuoInit,
                dim4388KuoInit,
                dim4389KuoInit,
                dim4390KuoInit,
                dim4391KuoInit,
                dim4392KuoInit,
                dim4393KuoInit,
                dim4394KuoInit,
                dim4395KuoInit,
                dim4396KuoInit,
                dim4397KuoInit,
                dim4398KuoInit,
                dim4399KuoInit,
                dim4400KuoInit,
                dim4401KuoInit,
                dim4402KuoInit,
                dim4403KuoInit,
                dim4404KuoInit,
                dim4405KuoInit,
                dim4406KuoInit,
                dim4407KuoInit,
                dim4408KuoInit,
                dim4409KuoInit,
                dim4410KuoInit,
                dim4411KuoInit,
                dim4412KuoInit,
                dim4413KuoInit,
                dim4414KuoInit,
                dim4415KuoInit,
                dim4416KuoInit,
                dim4417KuoInit,
                dim4418KuoInit,
                dim4419KuoInit,
                dim4420KuoInit,
                dim4421KuoInit,
                dim4422KuoInit,
                dim4423KuoInit,
                dim4424KuoInit,
                dim4425KuoInit,
                dim4426KuoInit,
                dim4427KuoInit,
                dim4428KuoInit,
                dim4429KuoInit,
                dim4430KuoInit,
                dim4431KuoInit,
                dim4432KuoInit,
                dim4433KuoInit,
                dim4434KuoInit,
                dim4435KuoInit,
                dim4436KuoInit,
                dim4437KuoInit,
                dim4438KuoInit,
                dim4439KuoInit,
                dim4440KuoInit,
                dim4441KuoInit,
                dim4442KuoInit,
                dim4443KuoInit,
                dim4444KuoInit,
                dim4445KuoInit,
                dim4446KuoInit,
                dim4447KuoInit,
                dim4448KuoInit,
                dim4449KuoInit,
                dim4450KuoInit,
                dim4451KuoInit,
                dim4452KuoInit,
                dim4453KuoInit,
                dim4454KuoInit,
                dim4455KuoInit,
                dim4456KuoInit,
                dim4457KuoInit,
                dim4458KuoInit,
                dim4459KuoInit,
                dim4460KuoInit,
                dim4461KuoInit,
                dim4462KuoInit,
                dim4463KuoInit,
                dim4464KuoInit,
                dim4465KuoInit,
                dim4466KuoInit,
                dim4467KuoInit,
                dim4468KuoInit,
                dim4469KuoInit,
                dim4470KuoInit,
                dim4471KuoInit,
                dim4472KuoInit,
                dim4473KuoInit,
                dim4474KuoInit,
                dim4475KuoInit,
                dim4476KuoInit,
                dim4477KuoInit,
                dim4478KuoInit,
                dim4479KuoInit,
                dim4480KuoInit,
                dim4481KuoInit,
                dim4482KuoInit,
                dim4483KuoInit,
                dim4484KuoInit,
                dim4485KuoInit,
                dim4486KuoInit,
                dim4487KuoInit,
                dim4488KuoInit,
                dim4489KuoInit,
                dim4490KuoInit,
                dim4491KuoInit,
                dim4492KuoInit,
                dim4493KuoInit,
                dim4494KuoInit,
                dim4495KuoInit,
                dim4496KuoInit,
                dim4497KuoInit,
                dim4498KuoInit,
                dim4499KuoInit,
                dim4500KuoInit,
                dim4501KuoInit,
                dim4502KuoInit,
                dim4503KuoInit,
                dim4504KuoInit,
                dim4505KuoInit,
                dim4506KuoInit,
                dim4507KuoInit,
                dim4508KuoInit,
                dim4509KuoInit,
                dim4510KuoInit,
                dim4511KuoInit,
                dim4512KuoInit,
                dim4513KuoInit,
                dim4514KuoInit,
                dim4515KuoInit,
                dim4516KuoInit,
                dim4517KuoInit,
                dim4518KuoInit,
                dim4519KuoInit,
                dim4520KuoInit,
                dim4521KuoInit,
                dim4522KuoInit,
                dim4523KuoInit,
                dim4524KuoInit,
                dim4525KuoInit,
                dim4526KuoInit,
                dim4527KuoInit,
                dim4528KuoInit,
                dim4529KuoInit,
                dim4530KuoInit,
                dim4531KuoInit,
                dim4532KuoInit,
                dim4533KuoInit,
                dim4534KuoInit,
                dim4535KuoInit,
                dim4536KuoInit,
                dim4537KuoInit,
                dim4538KuoInit,
                dim4539KuoInit,
                dim4540KuoInit,
                dim4541KuoInit,
                dim4542KuoInit,
                dim4543KuoInit,
                dim4544KuoInit,
                dim4545KuoInit,
                dim4546KuoInit,
                dim4547KuoInit,
                dim4548KuoInit,
                dim4549KuoInit,
                dim4550KuoInit,
                dim4551KuoInit,
                dim4552KuoInit,
                dim4553KuoInit,
                dim4554KuoInit,
                dim4555KuoInit,
                dim4556KuoInit,
                dim4557KuoInit,
                dim4558KuoInit,
                dim4559KuoInit,
                dim4560KuoInit,
                dim4561KuoInit,
                dim4562KuoInit,
                dim4563KuoInit,
                dim4564KuoInit,
                dim4565KuoInit,
                dim4566KuoInit,
                dim4567KuoInit,
                dim4568KuoInit,
                dim4569KuoInit,
                dim4570KuoInit,
                dim4571KuoInit,
                dim4572KuoInit,
                dim4573KuoInit,
                dim4574KuoInit,
                dim4575KuoInit,
                dim4576KuoInit,
                dim4577KuoInit,
                dim4578KuoInit,
                dim4579KuoInit,
                dim4580KuoInit,
                dim4581KuoInit,
                dim4582KuoInit,
                dim4583KuoInit,
                dim4584KuoInit,
                dim4585KuoInit,
                dim4586KuoInit,
                dim4587KuoInit,
                dim4588KuoInit,
                dim4589KuoInit,
                dim4590KuoInit,
                dim4591KuoInit,
                dim4592KuoInit,
                dim4593KuoInit,
                dim4594KuoInit,
                dim4595KuoInit,
                dim4596KuoInit,
                dim4597KuoInit,
                dim4598KuoInit,
                dim4599KuoInit,
                dim4600KuoInit,
                dim4601KuoInit,
                dim4602KuoInit,
                dim4603KuoInit,
                dim4604KuoInit,
                dim4605KuoInit,
                dim4606KuoInit,
                dim4607KuoInit,
                dim4608KuoInit,
                dim4609KuoInit,
                dim4610KuoInit,
                dim4611KuoInit,
                dim4612KuoInit,
                dim4613KuoInit,
                dim4614KuoInit,
                dim4615KuoInit,
                dim4616KuoInit,
                dim4617KuoInit,
                dim4618KuoInit,
                dim4619KuoInit,
                dim4620KuoInit,
                dim4621KuoInit,
                dim4622KuoInit,
                dim4623KuoInit,
                dim4624KuoInit,
                dim4625KuoInit,
                dim4626KuoInit,
                dim4627KuoInit,
                dim4628KuoInit,
                dim4629KuoInit,
                dim4630KuoInit,
                dim4631KuoInit,
                dim4632KuoInit,
                dim4633KuoInit,
                dim4634KuoInit,
                dim4635KuoInit,
                dim4636KuoInit,
                dim4637KuoInit,
                dim4638KuoInit,
                dim4639KuoInit,
                dim4640KuoInit,
                dim4641KuoInit,
                dim4642KuoInit,
                dim4643KuoInit,
                dim4644KuoInit,
                dim4645KuoInit,
                dim4646KuoInit,
                dim4647KuoInit,
                dim4648KuoInit,
                dim4649KuoInit,
                dim4650KuoInit,
                dim4651KuoInit,
                dim4652KuoInit,
                dim4653KuoInit,
                dim4654KuoInit,
                dim4655KuoInit,
                dim4656KuoInit,
                dim4657KuoInit,
                dim4658KuoInit,
                dim4659KuoInit,
                dim4660KuoInit,
                dim4661KuoInit,
                dim4662KuoInit,
                dim4663KuoInit,
                dim4664KuoInit,
                dim4665KuoInit,
                dim4666KuoInit,
                dim4667KuoInit,
                dim4668KuoInit,
                dim4669KuoInit,
                dim4670KuoInit,
                dim4671KuoInit,
                dim4672KuoInit,
                dim4673KuoInit,
                dim4674KuoInit,
                dim4675KuoInit,
                dim4676KuoInit,
                dim4677KuoInit,
                dim4678KuoInit,
                dim4679KuoInit,
                dim4680KuoInit,
                dim4681KuoInit,
                dim4682KuoInit,
                dim4683KuoInit,
                dim4684KuoInit,
                dim4685KuoInit,
                dim4686KuoInit,
                dim4687KuoInit,
                dim4688KuoInit,
                dim4689KuoInit,
                dim4690KuoInit,
                dim4691KuoInit,
                dim4692KuoInit,
                dim4693KuoInit,
                dim4694KuoInit,
                dim4695KuoInit,
                dim4696KuoInit,
                dim4697KuoInit,
                dim4698KuoInit,
                dim4699KuoInit,
                dim4700KuoInit,
                dim4701KuoInit,
                dim4702KuoInit,
                dim4703KuoInit,
                dim4704KuoInit,
                dim4705KuoInit,
                dim4706KuoInit,
                dim4707KuoInit,
                dim4708KuoInit,
                dim4709KuoInit,
                dim4710KuoInit,
                dim4711KuoInit,
                dim4712KuoInit,
                dim4713KuoInit,
                dim4714KuoInit,
                dim4715KuoInit,
                dim4716KuoInit,
                dim4717KuoInit,
                dim4718KuoInit,
                dim4719KuoInit,
                dim4720KuoInit,
                dim4721KuoInit,
                dim4722KuoInit,
                dim4723KuoInit,
                dim4724KuoInit,
                dim4725KuoInit,
                dim4726KuoInit,
                dim4727KuoInit,
                dim4728KuoInit,
                dim4729KuoInit,
                dim4730KuoInit,
                dim4731KuoInit,
                dim4732KuoInit,
                dim4733KuoInit,
                dim4734KuoInit,
                dim4735KuoInit,
                dim4736KuoInit,
                dim4737KuoInit,
                dim4738KuoInit,
                dim4739KuoInit,
                dim4740KuoInit,
                dim4741KuoInit,
                dim4742KuoInit,
                dim4743KuoInit,
                dim4744KuoInit,
                dim4745KuoInit,
                dim4746KuoInit,
                dim4747KuoInit,
                dim4748KuoInit,
                dim4749KuoInit,
                dim4750KuoInit,
                dim4751KuoInit,
                dim4752KuoInit,
                dim4753KuoInit,
                dim4754KuoInit,
                dim4755KuoInit,
                dim4756KuoInit,
                dim4757KuoInit,
                dim4758KuoInit,
                dim4759KuoInit,
                dim4760KuoInit,
                dim4761KuoInit,
                dim4762KuoInit,
                dim4763KuoInit,
                dim4764KuoInit,
                dim4765KuoInit,
                dim4766KuoInit,
                dim4767KuoInit,
                dim4768KuoInit,
                dim4769KuoInit,
                dim4770KuoInit,
                dim4771KuoInit,
                dim4772KuoInit,
                dim4773KuoInit,
                dim4774KuoInit,
                dim4775KuoInit,
                dim4776KuoInit,
                dim4777KuoInit,
                dim4778KuoInit,
                dim4779KuoInit,
                dim4780KuoInit,
                dim4781KuoInit,
                dim4782KuoInit,
                dim4783KuoInit,
                dim4784KuoInit,
                dim4785KuoInit,
                dim4786KuoInit,
                dim4787KuoInit,
                dim4788KuoInit,
                dim4789KuoInit,
                dim4790KuoInit,
                dim4791KuoInit,
                dim4792KuoInit,
                dim4793KuoInit,
                dim4794KuoInit,
                dim4795KuoInit,
                dim4796KuoInit,
                dim4797KuoInit,
                dim4798KuoInit,
                dim4799KuoInit,
                dim4800KuoInit,
                dim4801KuoInit,
                dim4802KuoInit,
                dim4803KuoInit,
                dim4804KuoInit,
                dim4805KuoInit,
                dim4806KuoInit,
                dim4807KuoInit,
                dim4808KuoInit,
                dim4809KuoInit,
                dim4810KuoInit,
                dim4811KuoInit,
                dim4812KuoInit,
                dim4813KuoInit,
                dim4814KuoInit,
                dim4815KuoInit,
                dim4816KuoInit,
                dim4817KuoInit,
                dim4818KuoInit,
                dim4819KuoInit,
                dim4820KuoInit,
                dim4821KuoInit,
                dim4822KuoInit,
                dim4823KuoInit,
                dim4824KuoInit,
                dim4825KuoInit,
                dim4826KuoInit,
                dim4827KuoInit,
                dim4828KuoInit,
                dim4829KuoInit,
                dim4830KuoInit,
                dim4831KuoInit,
                dim4832KuoInit,
                dim4833KuoInit,
                dim4834KuoInit,
                dim4835KuoInit,
                dim4836KuoInit,
                dim4837KuoInit,
                dim4838KuoInit,
                dim4839KuoInit,
                dim4840KuoInit,
                dim4841KuoInit,
                dim4842KuoInit,
                dim4843KuoInit,
                dim4844KuoInit,
                dim4845KuoInit,
                dim4846KuoInit,
                dim4847KuoInit,
                dim4848KuoInit,
                dim4849KuoInit,
                dim4850KuoInit,
                dim4851KuoInit,
                dim4852KuoInit,
                dim4853KuoInit,
                dim4854KuoInit,
                dim4855KuoInit,
                dim4856KuoInit,
                dim4857KuoInit,
                dim4858KuoInit,
                dim4859KuoInit,
                dim4860KuoInit,
                dim4861KuoInit,
                dim4862KuoInit,
                dim4863KuoInit,
                dim4864KuoInit,
                dim4865KuoInit,
                dim4866KuoInit,
                dim4867KuoInit,
                dim4868KuoInit,
                dim4869KuoInit,
                dim4870KuoInit,
                dim4871KuoInit,
                dim4872KuoInit,
                dim4873KuoInit,
                dim4874KuoInit,
                dim4875KuoInit,
                dim4876KuoInit,
                dim4877KuoInit,
                dim4878KuoInit,
                dim4879KuoInit,
                dim4880KuoInit,
                dim4881KuoInit,
                dim4882KuoInit,
                dim4883KuoInit,
                dim4884KuoInit,
                dim4885KuoInit,
                dim4886KuoInit,
                dim4887KuoInit,
                dim4888KuoInit,
                dim4889KuoInit,
                dim4890KuoInit,
                dim4891KuoInit,
                dim4892KuoInit,
                dim4893KuoInit,
                dim4894KuoInit,
                dim4895KuoInit,
                dim4896KuoInit,
                dim4897KuoInit,
                dim4898KuoInit,
                dim4899KuoInit,
                dim4900KuoInit,
                dim4901KuoInit,
                dim4902KuoInit,
                dim4903KuoInit,
                dim4904KuoInit,
                dim4905KuoInit,
                dim4906KuoInit,
                dim4907KuoInit,
                dim4908KuoInit,
                dim4909KuoInit,
                dim4910KuoInit,
                dim4911KuoInit,
                dim4912KuoInit,
                dim4913KuoInit,
                dim4914KuoInit,
                dim4915KuoInit,
                dim4916KuoInit,
                dim4917KuoInit,
                dim4918KuoInit,
                dim4919KuoInit,
                dim4920KuoInit,
                dim4921KuoInit,
                dim4922KuoInit,
                dim4923KuoInit,
                dim4924KuoInit,
                dim4925KuoInit,

            };


    }

    const int SobolRsg::bits_ = 8*sizeof(unsigned long);
    // 1/(2^bits_) (written as (1/2)/(2^(bits_-1)) to avoid long overflow)
    const double SobolRsg::normalizationFactor_ =
        0.5/(1UL<<(SobolRsg::bits_-1));

    SobolRsg::SobolRsg(Size dimensionality,
                       unsigned long seed,
                       DirectionIntegers directionIntegers)
    : dimensionality_(dimensionality), sequenceCounter_(0), firstDraw_(true),
      sequence_(std::vector<Real> (dimensionality), 1.0),
      integerSequence_(dimensionality, 0),
      directionIntegers_(dimensionality,std::vector<unsigned long>(bits_)) {

        QL_REQUIRE(dimensionality>0, 
                   "dimensionality must be greater than 0");
        QL_REQUIRE(dimensionality<=PPMT_MAX_DIM,
                   "dimensionality " << dimensionality
                   << " exceeds the number of available "
                   << "primitive polynomials modulo two ("
                   << PPMT_MAX_DIM << ")");

        // initializes coefficient array of the k-th primitive polynomial
        // and degree of the k-th primitive polynomial
        std::vector<unsigned int> degree(dimensionality_);
        std::vector<long> ppmt(dimensionality_);
        // degree 0 is not used
        ppmt[0]=0;
        degree[0]=0;
        Size k, index;
        unsigned int currentDegree;
        for (k=1,index=0,currentDegree=1; k<dimensionality_; k++,index++){
            ppmt[k] = PrimitivePolynomials[currentDegree-1][index];
            if (ppmt[k]==-1) {
                ++currentDegree;
                index=0;
                ppmt[k] = PrimitivePolynomials[currentDegree-1][index];
            }
            degree[k] = currentDegree;
        }

        // initializes bits_ direction integers for each dimension
        // and store them into directionIntegers_[dimensionality_][bits_]
        //
        // In each dimension k with its associated primitive polynomial,
        // the first degree_[k] direction integers can be chosen freely
        // provided that only the l leftmost bits can be non-zero, and
        // that the l-th leftmost bit must be set

        // degenerate (no free direction integers) first dimension
        int j;
        for (j=0; j<bits_; j++)
            directionIntegers_[0][j] = (1UL<<(bits_-j-1));


        Size maxTabulated;
        // dimensions from 2 (k=1) to maxTabulated (k=maxTabulated-1) included
        // are initialized from tabulated coefficients
        switch (directionIntegers) {
            case Unit:
                maxTabulated=dimensionality_;
                for (k=1; k<maxTabulated; k++) {
                    for (Size l=1; l<=degree[k]; l++) {
                        directionIntegers_[k][l-1] = 1UL;
                        directionIntegers_[k][l-1] <<= (bits_-l);
                    }
                }
                break;
            case Jaeckel:
                // maxTabulated=32
                maxTabulated = sizeof(initializers)/sizeof(unsigned long *)+1;
                for (k=1; k<std::min(dimensionality_, maxTabulated); k++) {
                    j = 0;
                    // 0UL marks coefficients' end for a given dimension
                    while (initializers[k-1][j] != 0UL) {
                        directionIntegers_[k][j] = initializers[k-1][j];
                        directionIntegers_[k][j] <<= (bits_-j-1);
                        j++;
                    }
                }
                break;
            case SobolLevitan:
                // maxTabulated=40
                maxTabulated = sizeof(SLinitializers)/sizeof(unsigned long *)+1;
                for (k=1; k<std::min(dimensionality_, maxTabulated); k++) {
                    j = 0;
                    // 0UL marks coefficients' end for a given dimension
                    while (SLinitializers[k-1][j] != 0UL) {
                        directionIntegers_[k][j] = SLinitializers[k-1][j];
                        directionIntegers_[k][j] <<= (bits_-j-1);
                        j++;
                    }
                }
                break;
            case SobolLevitanLemieux:
                // maxTabulated=360
                maxTabulated = sizeof(Linitializers)/sizeof(unsigned long *)+1;
                for (k=1; k<std::min(dimensionality_, maxTabulated); k++) {
                    j = 0;
                    // 0UL marks coefficients' end for a given dimension
                    while (Linitializers[k-1][j] != 0UL) {
                        directionIntegers_[k][j] = Linitializers[k-1][j];
                        directionIntegers_[k][j] <<= (bits_-j-1);
                        j++;
                    }
                }
                break;
            case Kuo:
                // maxTabulated=4925
                maxTabulated = sizeof(Kuoinitializers)/sizeof(unsigned long *)+1;
                for (k=1; k<std::min(dimensionality_, maxTabulated); k++) {
                    j = 0;
                    // 0UL marks coefficients' end for a given dimension
                    while (Linitializers[k-1][j] != 0UL) {
                        directionIntegers_[k][j] = Kuoinitializers[k-1][j];
                        directionIntegers_[k][j] <<= (bits_-j-1);
                        j++;
                    }
                }
                break;
            default:
                break;
        }

        // random initialization for higher dimensions
        if (dimensionality_>maxTabulated) {
            MersenneTwisterUniformRng uniformRng(seed);
            for (k=maxTabulated; k<dimensionality_; k++) {
                for (Size l=1; l<=degree[k]; l++) {
                    do {
                        // u is in (0,1)
                        double u = uniformRng.next().value;
                        // the direction integer has at most the
                        // rightmost l bits non-zero
                        directionIntegers_[k][l-1] =
                            (unsigned long)(u*(1UL<<l));
                    } while (!(directionIntegers_[k][l-1] & 1UL));
                    // iterate until the direction integer is odd
                    // that is it has the rightmost bit set

                    // shifting bits_-l bits to the left
                    // we are guaranteed that the l-th leftmost bit
                    // is set, and only the first l leftmost bit
                    // can be non-zero
                    directionIntegers_[k][l-1] <<= (bits_-l);
                }
            }
        }

        // computation of directionIntegers_[k][l] for l>=degree_[k]
        // by recurrence relation
        for (k=1; k<dimensionality_; k++) {
            unsigned int gk = degree[k];
            for (int l=gk; l<bits_; l++) {
                // eq. 8.19 "Monte Carlo Methods in Finance" by P. J�ckel
                unsigned long n = (directionIntegers_[k][l-gk]>>gk);
                // a[k][j] are the coefficients of the monomials in ppmt[k]
                // The highest order coefficient a[k][0] is not actually
                // used in the recurrence relation, and the lowest order
                // coefficient a[k][gk] is always set: this is the reason
                // why the highest and lowest coefficient of
                // the polynomial ppmt[k] are not included in its encoding,
                // provided that its degree is known.
                // That is: a[k][j] = ppmt[k] >> (gk-j-1)
                for (Size j=1; j<gk; j++) {
                    // XORed with a selection of (unshifted) direction
                    // integers controlled by which of the a[k][j] are set
                    if ((ppmt[k] >> (gk-j-1)) & 1UL)
                        n ^= directionIntegers_[k][l-j];
                }
                // a[k][gk] is always set, so directionIntegers_[k][l-gk]
                // will always enter
                n ^= directionIntegers_[k][l-gk];
                directionIntegers_[k][l]=n;
            }
        }

        // in case one needs to check the directionIntegers used
        /* bool printDirectionIntegers = false;
           if (printDirectionIntegers) {
               std::ofstream outStream("directionIntegers.txt");
               for (k=0; k<std::min(32UL,dimensionality_); k++) {
                   outStream << std::endl << k+1       << "\t"
                                          << degree[k] << "\t"
                                          << ppmt[k]   << "\t";
                   for (j=0; j<10; j++) {
                       outStream << io::power_of_two(
                           directionIntegers_[k][j]) << "\t";
                   }
               }
               outStream.close();
           }
        */

        // initialize the Sobol integer/double vectors
        // first draw
        for (k=0; k<dimensionality_; k++) {
            integerSequence_[k]=directionIntegers_[k][0];
        }
    }

    void SobolRsg::skipTo(unsigned long skip) {
        unsigned long N = skip+1;
        unsigned int ops = (unsigned int)(std::log((double)N)/M_LN2)+1;

        // Convert to Gray code
        unsigned long G = N ^ (N>>1);
        for (Size k=0; k<dimensionality_; k++) {
            integerSequence_[k] = 0;
            for (Size index=0; index<ops; index++) {
                if (G>>index & 1)
                    integerSequence_[k] ^= directionIntegers_[k][index];
            }
        }

        sequenceCounter_ = skip;
    }


    const std::vector<unsigned long>& SobolRsg::nextInt32Sequence() const {
        if (firstDraw_) {
            // it was precomputed in the constructor
            firstDraw_ = false;
            return integerSequence_;
        }
        // increment the counter
        sequenceCounter_++;
        // did we overflow?
        QL_REQUIRE(sequenceCounter_ != 0, "period exceeded");

        // instead of using the counter n as new unique generating integer
        // for the n-th draw use the Gray code G(n) as proposed
        // by Antonov and Saleev
        unsigned long n = sequenceCounter_;
        // Find rightmost zero bit of n
        int j = 0;
        while (n & 1) { n >>= 1; j++; }
        for (Size k=0; k<dimensionality_; k++) {
            // XOR the appropriate direction number into each component of
            // the integer sequence to obtain a new Sobol integer for that
            // component
            integerSequence_[k] ^= directionIntegers_[k][j];
        }
        return integerSequence_;
    }

}
