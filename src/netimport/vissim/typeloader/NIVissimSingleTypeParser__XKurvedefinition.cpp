/****************************************************************************/
// Eclipse SUMO, Simulation of Urban MObility; see https://eclipse.org/sumo
// Copyright (C) 2001-2018 German Aerospace Center (DLR) and others.
// This program and the accompanying materials
// are made available under the terms of the Eclipse Public License v2.0
// which accompanies this distribution, and is available at
// http://www.eclipse.org/legal/epl-v20.html
/****************************************************************************/
/// @file    NIVissimSingleTypeParser__XKurvedefinition.cpp
/// @author  Daniel Krajzewicz
/// @author  Michael Behrisch
/// @date    Wed, 30 Apr 2003
/// @version $Id$
///
//
/****************************************************************************/


// ===========================================================================
// included modules
// ===========================================================================
#ifdef _MSC_VER
#include <windows_config.h>
#else
#include <config.h>
#endif

#include <iostream>
#include <utils/common/TplConvert.h>
#include "../NIImporter_Vissim.h"
#include "../tempstructs/NIVissimSource.h"
#include "NIVissimSingleTypeParser__XKurvedefinition.h"


// ===========================================================================
// method definitions
// ===========================================================================
NIVissimSingleTypeParser__XKurvedefinition::NIVissimSingleTypeParser__XKurvedefinition(NIImporter_Vissim& parent)
    : NIImporter_Vissim::VissimSingleTypeParser(parent) {}


NIVissimSingleTypeParser__XKurvedefinition::~NIVissimSingleTypeParser__XKurvedefinition() {}


bool
NIVissimSingleTypeParser__XKurvedefinition::parse(std::istream&) {
    return true;
}



/****************************************************************************/

