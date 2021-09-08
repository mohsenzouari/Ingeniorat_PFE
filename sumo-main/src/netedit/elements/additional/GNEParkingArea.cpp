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
/// @file    GNEParkingArea.cpp
/// @author  Pablo Alvarez Lopez
/// @date    Feb 2018
///
// A lane area vehicles can park at (GNE version)
/****************************************************************************/
#include <netedit/GNENet.h>
#include <netedit/GNEUndoList.h>
#include <netedit/GNEViewNet.h>
#include <netedit/changes/GNEChange_Attribute.h>
#include <utils/gui/div/GLHelper.h>
#include <utils/options/OptionsCont.h>
#include <utils/gui/globjects/GLIncludes.h>
#include <utils/vehicle/SUMORouteHandler.h>

#include "GNEParkingArea.h"


// ===========================================================================
// method definitions
// ===========================================================================

GNEParkingArea::GNEParkingArea(const std::string& id, GNELane* lane, GNENet* net, const double startPos, const double endPos,
                               const std::string& departPos, const std::string& name, bool friendlyPosition, int roadSideCapacity, bool onRoad, double width,
                               const double length, double angle, const std::map<std::string, std::string>& parameters) :
    GNEStoppingPlace(id, net, GLO_PARKING_AREA, SUMO_TAG_PARKING_AREA, lane, startPos, endPos, name, friendlyPosition, parameters),
    myDepartPos(departPos),
    myRoadSideCapacity(roadSideCapacity),
    myOnRoad(onRoad),
    myWidth(width),
    myLength(length),
    myAngle(angle) {
    // update centering boundary without updating grid
    updateCenteringBoundary(false);
}


GNEParkingArea::~GNEParkingArea() {}


void 
GNEParkingArea::writeAdditional(OutputDevice& device) const {
    // use write additional of gneAdditional
    GNEAdditional::writeAdditional(device);
}


void
GNEParkingArea::updateGeometry() {
    // Get value of option "lefthand"
    const double offsetSign = OptionsCont::getOptions().getBool("lefthand") ? -1 : 1;
    // calculate spaceDim
    const double spaceDim = myRoadSideCapacity > 0 ? (getAttributeDouble(SUMO_ATTR_ENDPOS) - getAttributeDouble(SUMO_ATTR_STARTPOS)) / myRoadSideCapacity * getParentLanes().front()->getLengthGeometryFactor() : 7.5;
    // calculate lenght
    const double length = (myLength > 0) ? myLength : spaceDim;
    // Update common geometry of stopping place
    setStoppingPlaceGeometry(myWidth);
    // Obtain a copy of the shape
    PositionVector tmpShape = myAdditionalGeometry.getShape();
    // Move shape to side
    tmpShape.move2side(1.5 * offsetSign + myWidth);
    // Get position of the sign
    mySignPos = tmpShape.getLineCenter();
    // clear LotSpaceDefinitions
    myLotSpaceDefinitions.clear();
    // iterate over
    for (int i = 0; i < myRoadSideCapacity; ++i) {
        // calculate pos
        const Position pos = GeomHelper::calculateLotSpacePosition(myAdditionalGeometry.getShape(), i, spaceDim, myAngle, myWidth, length);
        // calculate angle
        const double angle = GeomHelper::calculateLotSpaceAngle(myAdditionalGeometry.getShape(), i, spaceDim, myAngle);
        // add GNElotEntry
        myLotSpaceDefinitions.push_back(GNELotSpaceDefinition(pos.x(), pos.y(), pos.z(), angle, myWidth, length));
    }
}


void
GNEParkingArea::drawGL(const GUIVisualizationSettings& s) const {
    // Obtain exaggeration of the draw
    const double parkingAreaExaggeration = s.addSize.getExaggeration(s, this);
    // first check if additional has to be drawn
    if (myNet->getViewNet()->getDataViewOptions().showAdditionals()) {
        // check exaggeration
        if (s.drawAdditionals(parkingAreaExaggeration)) {
            // check if boundary has to be drawn
            if (s.drawBoundaries) {
                GLHelper::drawBoundary(getCenteringBoundary());
            }
            // declare colors
            RGBColor baseColor, signColor;
            // set colors
            if (mySpecialColor) {
                baseColor = *mySpecialColor;
                signColor = baseColor.changedBrightness(-32);
            } else if (drawUsingSelectColor()) {
                baseColor = s.colorSettings.selectedAdditionalColor;
                signColor = baseColor.changedBrightness(-32);
            } else {
                baseColor = s.stoppingPlaceSettings.parkingAreaColor;
                signColor = s.stoppingPlaceSettings.parkingAreaColorSign;
            }
            // Start drawing adding an gl identificator
            GLHelper::pushName(getGlID());
            // Add a draw matrix
            GLHelper::pushMatrix();
            // translate to front
            myNet->getViewNet()->drawTranslateFrontAttributeCarrier(this, GLO_PARKING_AREA);
            // set base color
            GLHelper::setColor(baseColor);
            // Draw the area using shape, shapeRotations, shapeLengths and value of exaggeration
            GNEGeometry::drawGeometry(myNet->getViewNet(), myAdditionalGeometry, myWidth * 0.5 * parkingAreaExaggeration);
            // draw detail
            if (s.drawDetail(s.detailSettings.stoppingPlaceDetails, parkingAreaExaggeration)) {
                // draw sign
                drawSign(s, parkingAreaExaggeration, baseColor, signColor, "P");
                // draw lock icon
                GNEViewNetHelper::LockIcon::drawLockIcon(getType(), this, getPositionInView(), parkingAreaExaggeration);
                // Traslate to front
                glTranslated(0, 0, 0.1);
                // draw lotSpaceDefinitions
                for (const auto& lsd : myLotSpaceDefinitions) {
                    GLHelper::drawSpaceOccupancies(parkingAreaExaggeration, lsd.position, lsd.rotation, lsd.width, lsd.length, true);
                }
            }
            // pop draw matrix
            GLHelper::popMatrix();
            // Pop name
            GLHelper::popName();
            // check if dotted contours has to be drawn
            if (s.drawDottedContour() || myNet->getViewNet()->isAttributeCarrierInspected(this)) {
                GNEGeometry::drawDottedContourShape(GNEGeometry::DottedContourType::INSPECT, s, myAdditionalGeometry.getShape(), myWidth * 0.5, parkingAreaExaggeration);
            }
            if (s.drawDottedContour() || myNet->getViewNet()->getFrontAttributeCarrier() == this) {
                GNEGeometry::drawDottedContourShape(GNEGeometry::DottedContourType::FRONT, s, myAdditionalGeometry.getShape(), myWidth * 0.5, parkingAreaExaggeration);
            }
            // draw child spaces
            for (const auto& parkingSpace : getChildAdditionals()) {
                parkingSpace->drawGL(s);
            }
            // draw child demand elements
            for (const auto& demandElement : getChildDemandElements()) {
                if (!demandElement->getTagProperty().isPlacedInRTree()) {
                    demandElement->drawGL(s);
                }
            }
        }
        // Draw additional ID
        drawAdditionalID(s);
        // draw additional name
        drawAdditionalName(s);
    }
}


std::string
GNEParkingArea::getAttribute(SumoXMLAttr key) const {
    switch (key) {
        case SUMO_ATTR_ID:
            return getID();
        case SUMO_ATTR_LANE:
            return getParentLanes().front()->getID();
        case SUMO_ATTR_STARTPOS:
            if (myStartPosition != INVALID_DOUBLE) {
                return toString(myStartPosition);
            } else {
                return "";
            }
        case SUMO_ATTR_ENDPOS:
            if (myEndPosition != INVALID_DOUBLE) {
                return toString(myEndPosition);
            } else {
                return "";
            }
        case SUMO_ATTR_DEPARTPOS:
            return myDepartPos;
        case SUMO_ATTR_NAME:
            return myAdditionalName;
        case SUMO_ATTR_FRIENDLY_POS:
            return toString(myFriendlyPosition);
        case SUMO_ATTR_ROADSIDE_CAPACITY:
            return toString(myRoadSideCapacity);
        case SUMO_ATTR_ONROAD:
            return toString(myOnRoad);
        case SUMO_ATTR_WIDTH:
            return toString(myWidth);
        case SUMO_ATTR_LENGTH:
            return toString(myLength);
        case SUMO_ATTR_ANGLE:
            return toString(myAngle);
        case GNE_ATTR_SELECTED:
            return toString(isAttributeCarrierSelected());
        case GNE_ATTR_PARAMETERS:
            return getParametersStr();
        default:
            throw InvalidArgument(getTagStr() + " doesn't have an attribute of type '" + toString(key) + "'");
    }
}


double
GNEParkingArea::getAttributeDouble(SumoXMLAttr key) const {
    switch (key) {
        case SUMO_ATTR_STARTPOS:
            if (myStartPosition != INVALID_DOUBLE) {
                return myStartPosition;
            } else {
                return 0;
            }
        case SUMO_ATTR_ENDPOS:
            if (myEndPosition != INVALID_DOUBLE) {
                return myEndPosition;
            } else {
                return getParentLanes().front()->getParentEdge()->getNBEdge()->getFinalLength();
            }
        case GNE_ATTR_CENTER:
            return ((getAttributeDouble(SUMO_ATTR_ENDPOS) - getAttributeDouble(SUMO_ATTR_STARTPOS)) * 0.5) + getAttributeDouble(SUMO_ATTR_STARTPOS);
        case SUMO_ATTR_WIDTH:
            return myWidth;
        case SUMO_ATTR_LENGTH: {
            // calculate spaceDim
            const double spaceDim = myRoadSideCapacity > 0 ? (getAttributeDouble(SUMO_ATTR_ENDPOS) - getAttributeDouble(SUMO_ATTR_STARTPOS)) / myRoadSideCapacity * getParentLanes().front()->getLengthGeometryFactor() : 7.5;
            return (myLength > 0) ? myLength : spaceDim;
        }
        case SUMO_ATTR_ANGLE:
            return myAngle;
        default:
            throw InvalidArgument(getTagStr() + " doesn't have a double attribute of type '" + toString(key) + "'");
    }
}


void
GNEParkingArea::setAttribute(SumoXMLAttr key, const std::string& value, GNEUndoList* undoList) {
    if (value == getAttribute(key)) {
        return; //avoid needless changes, later logic relies on the fact that attributes have changed
    }
    switch (key) {
        case SUMO_ATTR_ID:
        case SUMO_ATTR_LANE:
        case SUMO_ATTR_STARTPOS:
        case SUMO_ATTR_ENDPOS:
        case SUMO_ATTR_DEPARTPOS:
        case SUMO_ATTR_NAME:
        case SUMO_ATTR_FRIENDLY_POS:
        case SUMO_ATTR_ROADSIDE_CAPACITY:
        case SUMO_ATTR_ONROAD:
        case SUMO_ATTR_WIDTH:
        case SUMO_ATTR_LENGTH:
        case SUMO_ATTR_ANGLE:
        case GNE_ATTR_SELECTED:
        case GNE_ATTR_PARAMETERS:
            undoList->p_add(new GNEChange_Attribute(this, key, value));
            break;
        default:
            throw InvalidArgument(getTagStr() + " doesn't have an attribute of type '" + toString(key) + "'");
    }
}


bool
GNEParkingArea::isValid(SumoXMLAttr key, const std::string& value) {
    switch (key) {
        case SUMO_ATTR_ID:
            return isValidAdditionalID(value);
        case SUMO_ATTR_LANE:
            if (myNet->retrieveLane(value, false) != nullptr) {
                return true;
            } else {
                return false;
            }
        case SUMO_ATTR_STARTPOS:
            if (value.empty()) {
                return true;
            } else if (canParse<double>(value)) {
                return SUMORouteHandler::isStopPosValid(parse<double>(value), getAttributeDouble(SUMO_ATTR_ENDPOS), getParentLanes().front()->getParentEdge()->getNBEdge()->getFinalLength(), POSITION_EPS, myFriendlyPosition);
            } else {
                return false;
            }
        case SUMO_ATTR_ENDPOS:
            if (value.empty()) {
                return true;
            } else if (canParse<double>(value)) {
                return SUMORouteHandler::isStopPosValid(getAttributeDouble(SUMO_ATTR_STARTPOS), parse<double>(value), getParentLanes().front()->getParentEdge()->getNBEdge()->getFinalLength(), POSITION_EPS, myFriendlyPosition);
            } else {
                return false;
            }
        case SUMO_ATTR_DEPARTPOS:
            if (value.empty()) {
                return true;
            } else if (canParse<double>(value)) {
                // parse value
                const double departPos = parse<double>(value);
                if ((departPos >= 0) && (departPos <= getParentLanes().front()->getParentEdge()->getNBEdge()->getFinalLength())) {
                    return true;
                } else {
                    return false;
                }
            } else {
                return false;
            }
        case SUMO_ATTR_NAME:
            return SUMOXMLDefinitions::isValidAttribute(value);
        case SUMO_ATTR_FRIENDLY_POS:
            return canParse<bool>(value);
        case SUMO_ATTR_ROADSIDE_CAPACITY:
            return canParse<double>(value) && (parse<double>(value) >= 0);
        case SUMO_ATTR_ONROAD:
            return canParse<bool>(value);
        case SUMO_ATTR_WIDTH:
            return canParse<double>(value) && (parse<double>(value) > 0);
        case SUMO_ATTR_LENGTH:
            if (value.empty()) {
                return true;
            } else {
                return canParse<double>(value) && (parse<double>(value) > 0);
            }
        case SUMO_ATTR_ANGLE:
            return canParse<double>(value);
        case GNE_ATTR_SELECTED:
            return canParse<bool>(value);
        case GNE_ATTR_PARAMETERS:
            return Parameterised::areParametersValid(value);
        default:
            throw InvalidArgument(getTagStr() + " doesn't have an attribute of type '" + toString(key) + "'");
    }
}

// ===========================================================================
// protected
// ===========================================================================

GNEParkingArea::GNELotSpaceDefinition::GNELotSpaceDefinition() :
    rotation(0),
    width(0),
    length(0) {
}


GNEParkingArea::GNELotSpaceDefinition::GNELotSpaceDefinition(double x, double y, double z, double rotation_, double width_, double length_) :
    position(Position(x, y, z)),
    rotation(rotation_),
    width(width_),
    length(length_) {
}

// ===========================================================================
// private
// ===========================================================================

void
GNEParkingArea::setAttribute(SumoXMLAttr key, const std::string& value) {
    switch (key) {
        case SUMO_ATTR_ID:
            myNet->getAttributeCarriers()->updateID(this, value);
            // Change IDs of all parking Spaces
            for (const auto& parkingSpace : getChildAdditionals()) {
                parkingSpace->setMicrosimID(getID());
            }
            break;
        case SUMO_ATTR_LANE:
            replaceAdditionalParentLanes(value);
            break;
        case SUMO_ATTR_STARTPOS:
            if (value == "") {
                myStartPosition = INVALID_DOUBLE;
            } else {
                myStartPosition = parse<double>(value);
            }
            // update boundary
            updateCenteringBoundary(true);
            break;
        case SUMO_ATTR_ENDPOS:
            if (value == "") {
                myEndPosition = INVALID_DOUBLE;
            } else {
                myEndPosition = parse<double>(value);
            }
            // update boundary
            updateCenteringBoundary(true);
            break;
        case SUMO_ATTR_DEPARTPOS:
            myDepartPos = value;
            break;
        case SUMO_ATTR_NAME:
            myAdditionalName = value;
            break;
        case SUMO_ATTR_FRIENDLY_POS:
            myFriendlyPosition = parse<bool>(value);
            break;
        case SUMO_ATTR_ROADSIDE_CAPACITY:
            myRoadSideCapacity = parse<int>(value);
            // update boundary
            updateCenteringBoundary(true);
            break;
        case SUMO_ATTR_ONROAD:
            myOnRoad = parse<bool>(value);
            break;
        case SUMO_ATTR_WIDTH:
            myWidth = parse<double>(value);
            // update geometry of all spaces
            for (const auto& space : getChildAdditionals()) {
                space->updateGeometry();
            }
            // update boundary
            updateCenteringBoundary(true);
            break;
        case SUMO_ATTR_LENGTH:
            myLength = parse<double>(value);
            // update geometry of all spaces
            for (const auto& space : getChildAdditionals()) {
                space->updateGeometry();
            }
            break;
        case SUMO_ATTR_ANGLE:
            myAngle = parse<double>(value);
            break;
        case GNE_ATTR_SELECTED:
            if (parse<bool>(value)) {
                selectAttributeCarrier();
            } else {
                unselectAttributeCarrier();
            }
            break;
        case GNE_ATTR_PARAMETERS:
            setParametersStr(value);
            break;
        default:
            throw InvalidArgument(getTagStr() + " doesn't have an attribute of type '" + toString(key) + "'");
    }
}


/****************************************************************************/
