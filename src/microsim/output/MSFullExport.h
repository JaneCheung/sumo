/****************************************************************************/
// Eclipse SUMO, Simulation of Urban MObility; see https://eclipse.org/sumo
// Copyright (C) 2012-2018 German Aerospace Center (DLR) and others.
// This program and the accompanying materials
// are made available under the terms of the Eclipse Public License v2.0
// which accompanies this distribution, and is available at
// http://www.eclipse.org/legal/epl-v20.html
/****************************************************************************/
/// @file    MSFullExport.h
/// @author  Mario Krumnow
/// @date    2012-04-26
/// @version $Id$
///
// Dumping a hugh List of Parameters available in the Simulation
/****************************************************************************/
#ifndef MSFullExport_h
#define MSFullExport_h


// ===========================================================================
// included modules
// ===========================================================================
#ifdef _MSC_VER
#include <windows_config.h>
#else
#include <config.h>
#endif

#include <utils/common/SUMOTime.h>


// ===========================================================================
// class declarations
// ===========================================================================
class OutputDevice;
class MSEdgeControl;
class MSEdge;
class MSLane;


// ===========================================================================
// class definitions
// ===========================================================================
/**
 * @class MSFullExport
 * @brief Dumping a hugh List of Parameters available in the Simulation
 *
 *  The class offers a static method, which writes a hugh List of Parameters
 *  available in the Simulation into the given OutputDevice.
 *
 * @todo consider error-handling on write (using IOError)
 */
class MSFullExport {
public:
    /**@brief Dumping a hugh List of Parameters available in the Simulation
    *
    *  The class offers a static method, which writes a hugh List of Parameters
    *  available in the Simulation into the given OutputDevice.
    *
    * @param[in] of The output device to use
    * @param[in] timestep The current time step
    * @exception IOError If an error on writing occurs (!!! not yet implemented)
    */
    static void write(OutputDevice& of, SUMOTime timestep);


private:
    /// @brief Invalidated copy constructor.
    MSFullExport(const MSFullExport&);

    /// @brief Invalidated assignment operator.
    MSFullExport& operator=(const MSFullExport&);

    /// @brief Writes the XML Nodes for the vehicles (e.g. speed, position, emissions)
    static void writeVehicles(OutputDevice& of);

    /// @brief Writes the XML Nodes for the edges (e.g. traveltime)
    static void writeEdge(OutputDevice& of);

    /// @brief Writes the XML Nodes for the lanes (e.g. emissions, occupancy)
    static void writeLane(OutputDevice& of, const MSLane& lane);

    /// @brief Writes the XML Nodes for the traffic lights (e.g. actual state)
    static void writeTLS(OutputDevice& of, SUMOTime timestep);

};


#endif

/****************************************************************************/

