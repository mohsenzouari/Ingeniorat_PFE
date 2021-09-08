/****************************************************************************/
// Eclipse SUMO, Simulation of Urban MObility; see https://eclipse.org/sumo
// Copyright (C) 2001-2021 German Aerospace Center (DLR) and others.
// This program and the accompanying materials are made available under the
// terms of the Eclipse Public License 2.0 which is available at
// https://www.eclipse.org/legal/epl-2.0/
// This Source Code may also be made available under the following Secondary
// Licenses when the conditions for such availability set forth in the Eclipse
// Public License 2.0 are satisfied: GNU General Public License, version 2
// or later which is available at
// https://www.gnu.org/licenses/old-licenses/gpl-2.0-standalone.html
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
/****************************************************************************/
/// @file    GNEClosingReroute.cpp
/// @author  Pablo Alvarez Lopez
/// @date    Jan 2017
///
//
/****************************************************************************/
#include <config.h>

#include "GNEClosingReroute.h"
#include <netedit/changes/GNEChange_Attribute.h>

#include <netedit/GNEUndoList.h>
#include <netedit/GNENet.h>


// ===========================================================================
// member method definitions
// ===========================================================================

GNEClosingReroute::GNEClosingReroute(GNEAdditional* rerouterIntervalParent, GNEEdge* closedEdge, SVCPermissions permissions) :
    GNEAdditional(rerouterIntervalParent->getNet(), GLO_CALIBRATOR, SUMO_TAG_CLOSING_REROUTE, "",
{}, {}, {}, {rerouterIntervalParent}, {}, {}, {}, {},
std::map<std::string, std::string>()),
    myClosedEdge(closedEdge),
myPermissions(permissions) {
    // update centering boundary without updating grid
    updateCenteringBoundary(false);
}


GNEClosingReroute::~GNEClosingReroute() {}


GNEMoveOperation*
GNEClosingReroute::getMoveOperation(const double /* shapeOffset */) {
    // GNEClosingReroutes cannot be moved
    return nullptr;
}


void
GNEClosingReroute::updateGeometry() {
    // use geometry of rerouter parent
    myAdditionalGeometry = getParentAdditionals().front()->getAdditionalGeometry();
}


Position
GNEClosingReroute::getPositionInView() const {
    return myBoundary.getCenter();
}


void
GNEClosingReroute::updateCenteringBoundary(const bool /*updateGrid*/) {
    // use boundary of parent element
    myBoundary = getParentAdditionals().front()->getCenteringBoundary();
}


void
GNEClosingReroute::splitEdgeGeometry(const double /*splitPosition*/, const GNENetworkElement* /*originalElement*/, const GNENetworkElement* /*newElement*/, GNEUndoList* /*undoList*/) {
    // geometry of this element cannot be splitted
}


std::string
GNEClosingReroute::getParentName() const {
    return getParentAdditionals().at(0)->getID();
}


void
GNEClosingReroute::drawGL(const GUIVisualizationSettings& /* s */) const {
    // Currently this additional isn't drawn
}


std::string
GNEClosingReroute::getAttribute(SumoXMLAttr key) const {
    switch (key) {
        case SUMO_ATTR_ID:
            return getID();
        case SUMO_ATTR_EDGE:
            return myClosedEdge->getID();
        case SUMO_ATTR_ALLOW:
            return getVehicleClassNames(myPermissions);
        case SUMO_ATTR_DISALLOW:
            return getVehicleClassNames(invertPermissions(myPermissions));
        case GNE_ATTR_PARENT:
            return getParentAdditionals().at(0)->getID();
        case GNE_ATTR_PARAMETERS:
            return getParametersStr();
        default:
            throw InvalidArgument(getTagStr() + " doesn't have an attribute of type '" + toString(key) + "'");
    }
}


double
GNEClosingReroute::getAttributeDouble(SumoXMLAttr key) const {
    throw InvalidArgument(getTagStr() + " doesn't have a double attribute of type '" + toString(key) + "'");
}


void
GNEClosingReroute::setAttribute(SumoXMLAttr key, const std::string& value, GNEUndoList* undoList) {
    if (value == getAttribute(key)) {
        return; //avoid needless changes, later logic relies on the fact that attributes have changed
    }
    switch (key) {
        case SUMO_ATTR_ID:
        case SUMO_ATTR_EDGE:
        case SUMO_ATTR_ALLOW:
        case SUMO_ATTR_DISALLOW:
        case GNE_ATTR_PARAMETERS:
            undoList->p_add(new GNEChange_Attribute(this, key, value));
            break;
        default:
            throw InvalidArgument(getTagStr() + " doesn't have an attribute of type '" + toString(key) + "'");
    }
}


bool
GNEClosingReroute::isValid(SumoXMLAttr key, const std::string& value) {
    switch (key) {
        case SUMO_ATTR_ID:
            return isValidAdditionalID(value);
        case SUMO_ATTR_EDGE:
            return (myNet->retrieveEdge(value, false) != nullptr);
        case SUMO_ATTR_ALLOW:
            return canParseVehicleClasses(value);
        case SUMO_ATTR_DISALLOW:
            return canParseVehicleClasses(value);
        case GNE_ATTR_PARAMETERS:
            return Parameterised::areParametersValid(value);
        default:
            throw InvalidArgument(getTagStr() + " doesn't have an attribute of type '" + toString(key) + "'");
    }
}


bool
GNEClosingReroute::isAttributeEnabled(SumoXMLAttr /* key */) const {
    return true;
}


std::string
GNEClosingReroute::getPopUpID() const {
    return getTagStr();
}


std::string
GNEClosingReroute::getHierarchyName() const {
    return getTagStr() + ": " + myClosedEdge->getID();
}

// ===========================================================================
// private
// ===========================================================================

void
GNEClosingReroute::setAttribute(SumoXMLAttr key, const std::string& value) {
    switch (key) {
        case SUMO_ATTR_ID:
            myNet->getAttributeCarriers()->updateID(this, value);
            break;
        case SUMO_ATTR_EDGE:
            myClosedEdge = myNet->retrieveEdge(value);
            break;
        case SUMO_ATTR_ALLOW:
            myPermissions = parseVehicleClasses(value);
            break;
        case SUMO_ATTR_DISALLOW:
            myPermissions = invertPermissions(parseVehicleClasses(value));
            break;
        case GNE_ATTR_PARAMETERS:
            setParametersStr(value);
            break;
        default:
            throw InvalidArgument(getTagStr() + " doesn't have an attribute of type '" + toString(key) + "'");
    }
}


void
GNEClosingReroute::setMoveShape(const GNEMoveResult& /*moveResult*/) {
    // nothing to do
}


void
GNEClosingReroute::commitMoveShape(const GNEMoveResult& /*moveResult*/, GNEUndoList* /*undoList*/) {
    // nothing to do
}


/****************************************************************************/
