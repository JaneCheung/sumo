/****************************************************************************/
// Eclipse SUMO, Simulation of Urban MObility; see https://eclipse.org/sumo
// Copyright (C) 2001-2018 German Aerospace Center (DLR) and others.
// This program and the accompanying materials
// are made available under the terms of the Eclipse Public License v2.0
// which accompanies this distribution, and is available at
// http://www.eclipse.org/legal/epl-v20.html
/****************************************************************************/
/// @file    MSCFModel_Kerner.cpp
/// @author  Daniel Krajzewicz
/// @author  Laura Bieker
/// @author  Michael Behrisch
/// @date    03.04.2010
/// @version $Id$
///
// car-following model by B. Kerner
/****************************************************************************/


// ===========================================================================
// included modules
// ===========================================================================
#ifdef _MSC_VER
#include <windows_config.h>
#else
#include <config.h>
#endif

#include <microsim/MSVehicle.h>
#include <microsim/MSLane.h>
#include "MSCFModel_Kerner.h"
#include <utils/common/RandHelper.h>


// ===========================================================================
// method definitions
// ===========================================================================
MSCFModel_Kerner::MSCFModel_Kerner(const MSVehicleType* vtype, double accel,
                                   double decel, double emergencyDecel, double apparentDecel,
                                   double headwayTime, double k, double phi) :
    MSCFModel(vtype, accel, decel, emergencyDecel, apparentDecel, headwayTime),
    myK(k), myPhi(phi),
    myTauDecel(decel * headwayTime) {
}


MSCFModel_Kerner::~MSCFModel_Kerner() {}


double
MSCFModel_Kerner::finalizeSpeed(MSVehicle* const veh, double vPos) const {
    const double vNext = MSCFModel::finalizeSpeed(veh, vPos);
    VehicleVariables* vars = (VehicleVariables*)veh->getCarFollowVariables();
    vars->rand = RandHelper::rand();
    return vNext;
}


double
MSCFModel_Kerner::followSpeed(const MSVehicle* const veh, double speed, double gap, double predSpeed, double /*predMaxDecel*/) const {
    return MIN2(_v(veh, speed, maxNextSpeed(speed, veh), gap, predSpeed), maxNextSpeed(speed, veh));
}


double
MSCFModel_Kerner::stopSpeed(const MSVehicle* const veh, const double speed, double gap) const {
    return MIN2(_v(veh, speed, maxNextSpeed(speed, veh), gap, 0), maxNextSpeed(speed, veh));
}


MSCFModel::VehicleVariables*
MSCFModel_Kerner::createVehicleVariables() const {
    VehicleVariables* ret = new VehicleVariables();
    ret->rand = RandHelper::rand();
    return ret;
}


double
MSCFModel_Kerner::_v(const MSVehicle* const veh, double speed, double vfree, double gap, double predSpeed) const {
    if (predSpeed == 0 && gap < 0.01) {
        return 0;
    }
    // !!! in the following, the prior step is not considered!!!
    double G = MAX2((double) 0, (double)(SPEED2DIST(myK * speed) + myPhi / myAccel * speed * (speed - predSpeed)));
    double vcond = gap > G ? speed + ACCEL2SPEED(myAccel) : speed + MAX2(ACCEL2SPEED(-myDecel), MIN2(ACCEL2SPEED(myAccel), predSpeed - speed));
    double vsafe = (double)(-1. * myTauDecel + sqrt(myTauDecel * myTauDecel + (predSpeed * predSpeed) + (2. * myDecel * gap)));
    VehicleVariables* vars = (VehicleVariables*)veh->getCarFollowVariables();
    double va = MAX2((double) 0, MIN3(vfree, vsafe, vcond)) + vars->rand;
    double v = MAX2((double) 0, MIN4(vfree, va, speed + ACCEL2SPEED(myAccel), vsafe));
    return v;
}


MSCFModel*
MSCFModel_Kerner::duplicate(const MSVehicleType* vtype) const {
    return new MSCFModel_Kerner(vtype, myAccel, myDecel, myEmergencyDecel, myApparentDecel, myHeadwayTime, myK, myPhi);
}
