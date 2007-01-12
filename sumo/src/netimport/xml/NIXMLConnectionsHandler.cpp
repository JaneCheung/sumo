/***************************************************************************
                          NIXMLConnectionsHandler.cpp
			  Used to parse the XML-descriptions of connections between
              edges or lanes given in a XML-format
                             -------------------
    project              : SUMO
    subproject           : netbuilder / netconverter
    begin                : Thu, 17 Oct 2002
    copyright            : (C) 2001 by DLR http://ivf.dlr.de/
    author               : Daniel Krajzewicz
    email                : Daniel.Krajzewicz@dlr.de
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
namespace
{
    const char rcsid[] =
    "$Id$";
}
// $Log$
// Revision 1.15  2007/01/12 08:14:24  dkrajzew
// [ feature-request-1599821 ] Right-of-way in Junctions implemented
//
// Revision 1.14  2006/12/06 08:24:39  dkrajzew
// further work in order to import ORINOKO definitions
//
// Revision 1.13  2006/11/14 13:04:10  dkrajzew
// warnings removed
//
// Revision 1.12  2006/04/18 08:05:45  dkrajzew
// beautifying: output consolidation
//
// Revision 1.11  2006/04/05 05:32:27  dkrajzew
// code beautifying: embedding string in strings removed
//
// Revision 1.10  2006/01/11 12:01:03  dkrajzew
// patched reassignment of explicite connections
//
// Revision 1.9  2005/10/17 09:18:44  dkrajzew
// got rid of the old MSVC memory leak checker
//
// Revision 1.8  2005/10/07 11:41:16  dkrajzew
// THIRD LARGE CODE RECHECK: patched problems on Linux/Windows configs
//
// Revision 1.7  2005/09/23 06:04:00  dkrajzew
// SECOND LARGE CODE RECHECK: converted doubles and floats to SUMOReal
//
// Revision 1.6  2005/09/15 12:03:36  dkrajzew
// LARGE CODE RECHECK
//
// Revision 1.5  2005/07/12 12:35:23  dkrajzew
// elmar2 importer included; debugging
//
// Revision 1.4  2005/04/27 12:24:42  dkrajzew
// level3 warnings removed; made netbuild-containers non-static
//
// Revision 1.3  2003/06/18 11:17:29  dkrajzew
// new message and error processing: output to user may be a message, warning or an error now; it is reported to a Singleton (MsgHandler); this handler puts it further to output instances. changes: no verbose-parameter needed; messages are exported to singleton
//
// Revision 1.2  2003/03/17 14:23:26  dkrajzew
//
// unneeded connection-type parameter usage removed
//
// Revision 1.1  2003/02/07 11:16:30  dkrajzew
// names changed
//
// Revision 1.1  2002/10/17 13:28:11  dkrajzew
// initial commit of classes to import connection definitions
//
/* =========================================================================
 * compiler pragmas
 * ======================================================================= */
#pragma warning(disable: 4786)


/* =========================================================================
 * included modules
 * ======================================================================= */
#ifdef HAVE_CONFIG_H
#ifdef WIN32
#include <windows_config.h>
#else
#include <config.h>
#endif
#endif // HAVE_CONFIG_H

#include <string>
#include <iostream>
#include <sax/HandlerBase.hpp>
#include <sax/AttributeList.hpp>
#include <sax/SAXParseException.hpp>
#include <sax/SAXException.hpp>
#include "NIXMLConnectionsHandler.h"
#include <netbuild/NBEdge.h>
#include <netbuild/NBEdgeCont.h>
#include <netbuild/nodes/NBNode.h>
#include <utils/common/StringTokenizer.h>
#include <utils/sumoxml/SUMOSAXHandler.h>
#include <utils/sumoxml/SUMOXMLDefinitions.h>
#include <utils/common/ToString.h>
#include <utils/common/TplConvert.h>
#include <utils/common/TplConvertSec.h>
#include <utils/xml/XMLBuildingExceptions.h>
#include <utils/common/UtilExceptions.h>
#include <utils/common/MsgHandler.h>

#ifdef _DEBUG
#include <utils/dev/debug_new.h>
#endif // _DEBUG


/* =========================================================================
 * used namespaces
 * ======================================================================= */
using namespace std;


/* =========================================================================
 * method definitions
 * ======================================================================= */
NIXMLConnectionsHandler::NIXMLConnectionsHandler(NBEdgeCont &ec)
    : SUMOSAXHandler("xml-connection-description"), myEdgeCont(ec)
{
}


NIXMLConnectionsHandler::~NIXMLConnectionsHandler()
{
}


NBConnection
NIXMLConnectionsHandler::parseConnection(const std::string &defRole, const string &def)
{
    // split from/to
    size_t div = def.find("->");
    if(div==string::npos) {
        addError("Missing connection divider in " + defRole + " '" + def + "'");
        return NBConnection(0, 0);
    }
    string fromDef = def.substr(0, div);
    string toDef = def.substr(div+2);

    // retrieve the edges
        // check whether the definition includes a lane information (do not process it)
    if(fromDef.find('_')!=string::npos) {
        fromDef = fromDef.substr(0, fromDef.find('_'));
    }
    if(toDef.find('_')!=string::npos) {
        toDef = fromDef.substr(0, toDef.find('_'));
    }
        // retrieve them now
    NBEdge *fromE = myEdgeCont.retrieve(fromDef);
    NBEdge *toE = myEdgeCont.retrieve(toDef);
        // check
    if(fromE==0) {
        addError("Could not find edge '" + fromDef + "' in " + defRole + " '" + def + "'");
        return NBConnection(0, 0);
    }
    if(toE==0) {
        addError("Could not find edge '" + toDef + "' in " + defRole + " '" + def + "'");
        return NBConnection(0, 0);
    }
    return NBConnection(fromE, toE);
}


NBNode *
NIXMLConnectionsHandler::getNode(const string &def)
{
    // split from/to (we can omit some checks as they already have been done in parseConnection)
    size_t div = def.find("->");
    string fromDef = def.substr(0, div);
    if(fromDef.find('_')!=string::npos) {
        fromDef = fromDef.substr(0, fromDef.find('_'));
    }
    return myEdgeCont.retrieve(fromDef)->getToNode();
}


void
NIXMLConnectionsHandler::myStartElement(int /*element*/, const std::string &name,
                                  const Attributes &attrs)
{
    if(name=="connection") {
        string from = getStringSecure(attrs, SUMO_ATTR_FROM, "");
        string to = getStringSecure(attrs, SUMO_ATTR_TO, "");
        if(from.length()==0||to.length()==0) {
            addError("Either a from-edge or a to-edge is not specified within one of the connections");
            return;
        }
        // extract edges
        NBEdge *fromEdge = myEdgeCont.retrieve(from);
        NBEdge *toEdge = myEdgeCont.retrieve(to);
        // check whether they are valid
        if(fromEdge==0) {
            addError("The connection-source edge '" + from + "' is not known.");
            return;
        }
        if(toEdge==0) {
            addError("The connection-destination edge '" + to + "' is not known.");
            return;
        }
        // parse the id
        string type = getStringSecure(attrs, SUMO_ATTR_TYPE, "");
        string laneConn = getStringSecure(attrs, SUMO_ATTR_LANE, "");
        if(type=="edgebound"||laneConn=="") {
            parseEdgeBound(attrs, fromEdge, toEdge);
        } else if(type=="lanebound"||laneConn.size()!=0) {
            parseLaneBound(attrs, fromEdge, toEdge);
        } else {
            addError("Unknown type of connection");
        }
    }
    if(name=="prohibition") {
        string prohibitor = getStringSecure(attrs, "prohibitor", "");
        string prohibited = getStringSecure(attrs, "prohibited", "");
        NBConnection prohibitorC = parseConnection("prohibitor", prohibitor);
        NBConnection prohibitedC = parseConnection("prohibited", prohibited);
        if(prohibitorC.getFrom()==0||prohibitedC.getFrom()==0) {
            // something failed
            return;
        }
        NBNode *n = getNode(prohibitor);
        n->addSortedLinkFoes(prohibitorC, prohibitedC);
    }
}

void
NIXMLConnectionsHandler::parseEdgeBound(const Attributes &/*attrs*/,
                                        NBEdge *from,
                                        NBEdge *to)
{
    from->addEdge2EdgeConnection(to);
/*    int noLanes;
    try {
        noLanes = getIntSecure(attrs, SUMO_ATTR_NOLANES, -1);
    } catch (NumberFormatException e) {
        addError("Not numeric lane in connection");
        return;
    }
    if(noLanes<0) {
        // !!! (what to do??)
    } else {
        // add connection
        for(size_t i=0; i<noLanes; i++) {
            from->addLane2LaneConnection(i, to, i);
        }
    }*/
}


void
NIXMLConnectionsHandler::parseLaneBound(const Attributes &attrs,
                                        NBEdge *from,
                                        NBEdge *to)
{
    string laneConn = getStringSecure(attrs, SUMO_ATTR_LANE, "");
    if(laneConn.length()==0) {
        addError("Not specified lane to lane connection");
        return;
    } else {
        // split the information
        StringTokenizer st(laneConn, ':');
        if(st.size()!=2) {
            addError("False lane to lane connection occured.");
            return;
        }
        // get the begin and the end lane
        int fromLane;
        int toLane;
        try {
            fromLane = TplConvertSec<char>::_2intSec(st.next().c_str(), -1);
            toLane = TplConvertSec<char>::_2intSec(st.next().c_str(), -1);
            if(!from->addLane2LaneConnection(fromLane, to, toLane, false)) {
                NBEdge *nFrom = from;
                bool toNext = true;
                do {
                    if(nFrom->getToNode()->getOutgoingEdges().size()!=1) {
                        toNext = false;
                        break;
                    }
                    NBEdge *t = nFrom->getToNode()->getOutgoingEdges()[0];
                    if(t->getID().substr(0, t->getID().find('/'))!=nFrom->getID().substr(0, nFrom->getID().find('/'))) {
                        toNext = false;
                        break;
                    }
                    if(toNext) {
                        nFrom = t;
                    }
                } while(toNext);
                if(nFrom==0||!nFrom->addLane2LaneConnection(fromLane, to, toLane, false)) {
                    WRITE_WARNING("Could not set loaded connection from '" + from->getID() + "_" + toString<int>(fromLane) + "' to '" + to->getID() + "_" + toString<int>(toLane) + "'.");
                }
            }
        } catch (NumberFormatException) {
            addError("At least one of the defined lanes was not numeric");
        }
    }
}


void
NIXMLConnectionsHandler::myCharacters(int /*element*/,
                                      const std::string &/*name*/,
                                      const std::string &/*chars*/)
{
}


void
NIXMLConnectionsHandler::myEndElement(int /*element*/,
                                      const std::string &/*name*/)
{
}


/**************** DO NOT DEFINE ANYTHING AFTER THE INCLUDE *****************/

// Local Variables:
// mode:C++
// End:

