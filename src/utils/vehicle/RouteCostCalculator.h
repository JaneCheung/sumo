/****************************************************************************/
// Eclipse SUMO, Simulation of Urban MObility; see https://eclipse.org/sumo
// Copyright (C) 2002-2018 German Aerospace Center (DLR) and others.
// This program and the accompanying materials
// are made available under the terms of the Eclipse Public License v2.0
// which accompanies this distribution, and is available at
// http://www.eclipse.org/legal/epl-v20.html
/****************************************************************************/
/// @file    RouteCostCalculator.h
/// @author  Daniel Krajzewicz
/// @author  Michael Behrisch
/// @author  Jakob Erdmann
/// @date    Sept 2002
/// @version $Id$
///
// Calculators for route costs and probabilities
/****************************************************************************/
#ifndef RouteCostCalculator_h
#define RouteCostCalculator_h


// ===========================================================================
// included modules
// ===========================================================================
#ifdef _MSC_VER
#include <windows_config.h>
#else
#include <config.h>
#endif

#include <vector>
#include <map>
#include <cmath>
#include <utils/common/StdDefs.h>
#include <utils/common/SUMOTime.h>
#include <utils/options/OptionsCont.h>


// ===========================================================================
// class definitions
// ===========================================================================
/**
 * @class RouteCostCalculator
 * @brief Abstract base class providing static factory method.
 */
template<class R, class E, class V>
class RouteCostCalculator {
public:
    static RouteCostCalculator<R, E, V>& getCalculator();

    static void cleanup() {
        delete myInstance;
        myInstance = 0;
    }

    virtual void setCosts(R* route, const double costs, const bool isActive = false) const = 0;

    /** @brief calculate the probabilities in the logit model */
    virtual void calculateProbabilities(std::vector<R*> alternatives, const V* const veh, const SUMOTime time) = 0;

    int getMaxRouteNumber() const {
        return myMaxRouteNumber;
    }

    bool keepRoutes() const {
        return myKeepRoutes;
    }

    bool skipRouteCalculation() const {
        return mySkipRouteCalculation;
    }

protected:
    /// @brief Constructor
    RouteCostCalculator() {
        OptionsCont& oc = OptionsCont::getOptions();
        myMaxRouteNumber = oc.getInt("max-alternatives");
        myKeepRoutes = oc.getBool("keep-all-routes");
        mySkipRouteCalculation = oc.getBool("skip-new-routes");
    }

    /// @brief Destructor
    virtual ~RouteCostCalculator() {}

private:
    static RouteCostCalculator* myInstance;

    /// @brief The maximum route alternatives number
    int myMaxRouteNumber;

    /// @brief Information whether all routes should be saved
    bool myKeepRoutes;

    /// @brief Information whether new routes should be calculated
    bool mySkipRouteCalculation;

};


// ===========================================================================
// static member definitions
// ===========================================================================
template<class R, class E, class V>
RouteCostCalculator<R, E, V>* RouteCostCalculator<R, E, V>::myInstance = 0;


#include "GawronCalculator.h"
#include "LogitCalculator.h"

template<class R, class E, class V>
RouteCostCalculator<R, E, V>& RouteCostCalculator<R, E, V>::getCalculator() {
    if (myInstance == 0) {
        OptionsCont& oc = OptionsCont::getOptions();
        if (oc.getString("route-choice-method") == "logit") {
            myInstance = new LogitCalculator<R, E, V>(oc.getFloat("logit.beta"), oc.getFloat("logit.gamma"), oc.getFloat("logit.theta"));
        } else if (oc.getString("route-choice-method") == "gawron") {
            myInstance = new GawronCalculator<R, E, V>(oc.getFloat("gawron.beta"), oc.getFloat("gawron.a"));
        }
    }
    return *myInstance;
}
#endif

/****************************************************************************/

