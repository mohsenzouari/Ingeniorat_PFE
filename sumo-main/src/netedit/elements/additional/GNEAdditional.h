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
/// @file    GNEAdditional.h
/// @author  Pablo Alvarez Lopez
/// @date    Jan 2016
///
// A abstract class for representation of additional elements
/****************************************************************************/
#pragma once
#include <config.h>

#include <netedit/elements/GNEHierarchicalElement.h>
#include <netedit/GNEGeometry.h>
#include <netedit/GNEPathManager.h>
#include <netedit/GNEMoveElement.h>
#include <utils/common/Parameterised.h>
#include <utils/geom/PositionVector.h>
#include <utils/gui/globjects/GUIGlObject.h>
#include <utils/gui/images/GUITextureSubSys.h>

// ===========================================================================
// class declarations
// ===========================================================================

class GNEViewNet;
class GNENetworkElement;
class GUIGLObjectPopupMenu;

// ===========================================================================
// class definitions
// ===========================================================================

/**
 * @class GNEAdditional
 * @brief An Element which don't belongs to GNENet but has influency in the simulation
 */
class GNEAdditional : public GUIGlObject, public GNEHierarchicalElement, public GNEMoveElement, public GNEPathManager::PathElement, public Parameterised {

public:
    /**@brief Constructor
     * @param[in] id Gl-id of the additional element (Must be unique)
     * @param[in] net pointer to GNENet of this additional element belongs
     * @param[in] type GUIGlObjectType of additional
     * @param[in] tag Type of xml tag that define the additional element (SUMO_TAG_BUS_STOP, SUMO_TAG_REROUTER, etc...)
     * @param[in] name Additional name
     * @param[in] junctionParents vector of junction parents
     * @param[in] edgeParents vector of edge parents
     * @param[in] laneParents vector of lane parents
     * @param[in] additionalParents vector of additional parents
     * @param[in] shapeParents vector of shape parents
     * @param[in] TAZElementParents vector of TAZElement parents
     * @param[in] demandElementParents vector of demand element parents
     * @param[in] genericDataParents vector of generic data parents
     * @param[in] parameters generic parameters
     */
    GNEAdditional(const std::string& id, GNENet* net, GUIGlObjectType type, SumoXMLTag tag, std::string additionalName,
                  const std::vector<GNEJunction*>& junctionParents,
                  const std::vector<GNEEdge*>& edgeParents,
                  const std::vector<GNELane*>& laneParents,
                  const std::vector<GNEAdditional*>& additionalParents,
                  const std::vector<GNEShape*>& shapeParents,
                  const std::vector<GNETAZElement*>& TAZElementParents,
                  const std::vector<GNEDemandElement*>& demandElementParents,
                  const std::vector<GNEGenericData*>& genericDataParents,
                  const std::map<std::string, std::string>& parameters);

    /**@brief Constructor for additional with parents
     * @param[in] net pointer to GNENet of this additional element belongs
     * @param[in] type GUIGlObjectType of additional
     * @param[in] tag Type of xml tag that define the additional element (SUMO_TAG_BUS_STOP, SUMO_TAG_REROUTER, etc...)
     * @param[in] name Additional name
     * @param[in] junctionParents vector of junction parents
     * @param[in] edgeParents vector of edge parents
     * @param[in] laneParents vector of lane parents
     * @param[in] additionalParents vector of additional parents
     * @param[in] shapeParents vector of shape parents
     * @param[in] TAZElementParents vector of TAZElement parents
     * @param[in] demandElementParents vector of demand element parents
     * @param[in] genericDataParents vector of generic data parents
     * @param[in] parameters generic parameters
     */
    GNEAdditional(GNENet* net, GUIGlObjectType type, SumoXMLTag tag, std::string additionalName,
                  const std::vector<GNEJunction*>& junctionParents,
                  const std::vector<GNEEdge*>& edgeParents,
                  const std::vector<GNELane*>& laneParents,
                  const std::vector<GNEAdditional*>& additionalParents,
                  const std::vector<GNEShape*>& shapeParents,
                  const std::vector<GNETAZElement*>& TAZElementParents,
                  const std::vector<GNEDemandElement*>& demandElementParents,
                  const std::vector<GNEGenericData*>& genericDataParents,
                  const std::map<std::string, std::string>& parameters);

    /// @brief Destructor
    ~GNEAdditional();

    /**@brief get move operation for the given shapeOffset
     * @note returned GNEMoveOperation can be nullptr
     */
    virtual GNEMoveOperation* getMoveOperation(const double shapeOffset) = 0;

    /// @brief remove geometry point in the clicked position (Currently unused in shapes)
    void removeGeometryPoint(const Position clickedPosition, GNEUndoList* undoList);

    /// @brief get ID
    const std::string& getID() const;

    /// @brief get GUIGlObject associated with this AttributeCarrier
    GUIGlObject* getGUIGlObject();

    /// @brief obtain additional geometry
    const GNEGeometry::Geometry& getAdditionalGeometry() const;

    /// @brief set special color
    void setSpecialColor(const RGBColor* color);

    /// @name members and functions relative to write additionals into XML
    /// @{
    /**@brief writte additional element into a xml file
     * @param[in] device device in which write parameters of additional element
     */
    virtual void writeAdditional(OutputDevice& device) const;

    /// @brief check if current additional is valid to be writed into XML (by default true, can be reimplemented in children)
    virtual bool isAdditionalValid() const;

    /// @brief return a string with the current additional problem (by default empty, can be reimplemented in children)
    virtual std::string getAdditionalProblem() const;

    /// @brief fix additional problem (by default throw an exception, has to be reimplemented in children)
    virtual void fixAdditionalProblem();
    /// @}

    /**@brief open Additional Dialog
     * @note: if additional needs an additional dialog, this function has to be implemented in childrens (see GNERerouter and GNEVariableSpeedSign)
     * @throw invalid argument if additional doesn't have an additional Dialog
     */
    virtual void openAdditionalDialog();

    /// @brief update pre-computed geometry information
    virtual void updateGeometry() = 0;

    /// @brief Returns position of additional in view
    virtual Position getPositionInView() const = 0;

    /// @brief Returns the boundary to which the view shall be centered in order to show the object
    Boundary getCenteringBoundary() const;

    /// @brief update centering boundary (implies change in RTREE)
    virtual void updateCenteringBoundary(const bool updateGrid) = 0;

    /// @brief split geometry
    virtual void splitEdgeGeometry(const double splitPosition, const GNENetworkElement* originalElement, const GNENetworkElement* newElement, GNEUndoList* undoList) = 0;
    /// @}

    /// @name inherited from GUIGlObject
    /// @{

    /**@brief Returns an own popup-menu
     *
     * @param[in] app The application needed to build the popup-menu
     * @param[in] parent The parent window needed to build the popup-menu
     * @return The built popup-menu
     * @see GUIGlObject::getPopUpMenu
     */
    GUIGLObjectPopupMenu* getPopUpMenu(GUIMainWindow& app, GUISUMOAbstractView& parent);

    /**@brief Returns an own parameter window
     *
     * @param[in] app The application needed to build the parameter window
     * @param[in] parent The parent window needed to build the parameter window
     * @return The built parameter window
     * @see GUIGlObject::getParameterWindow
     */
    GUIParameterTableWindow* getParameterWindow(GUIMainWindow& app, GUISUMOAbstractView& parent);

    /// @brief Returns the additional name
    const std::string& getOptionalAdditionalName() const;

    /**@brief Draws the object
     * @param[in] s The settings for the current view (may influence drawing)
     * @see GUIGlObject::drawGL
     */
    virtual void drawGL(const GUIVisualizationSettings& s) const = 0;

    /// @}

    /// @name inherited from GNEPathManager::PathElement
    /// @{

    /// @brief compute pathElement
    void computePathElement();

    /**@brief Draws partial object (lane)
     * @param[in] s The settings for the current view (may influence drawing)
     * @param[in] lane GNELane in which draw partial
     * @param[in] segment segment geometry
     * @note currently only E2Multilane detectors use drawPartialGL
     */
    void drawPartialGL(const GUIVisualizationSettings& s, const GNELane* lane, const GNEPathManager::Segment* segment, const double offsetFront) const;

    /**@brief Draws partial object (junction)
     * @param[in] s The settings for the current view (may influence drawing)
     * @param[in] fromLane from GNELane
     * @param[in] toLane to GNELane
     * @param[in] drawGeometry flag to enable/disable draw geometry (lines, boxLines, etc.)
     * @note currently only E2Multilane detectors use drawPartialGL
     */
    void drawPartialGL(const GUIVisualizationSettings& s, const GNELane* fromLane, const GNELane* toLane, const GNEPathManager::Segment* segment, const double offsetFront) const;

    /// @brief get first path lane
    GNELane* getFirstPathLane() const;

    /// @brief get last path lane
    GNELane* getLastPathLane() const;

    /// @brief get path element depart lane pos
    double getPathElementDepartValue() const;

    /// @brief get path element depart position
    Position getPathElementDepartPos() const;

    /// @brief get path element arrival lane pos
    double getPathElementArrivalValue() const;

    /// @brief get path element arrival position
    Position getPathElementArrivalPos() const;

    /// @}

    /// @name inherited from GNEAttributeCarrier
    /// @{
    /* @brief method for getting the Attribute of an XML key
     * @param[in] key The attribute key
     * @return string with the value associated to key
     */
    virtual std::string getAttribute(SumoXMLAttr key) const = 0;

    /* @brief method for getting the Attribute of an XML key in double format (to avoid unnecessary parse<double>(...) for certain attributes)
     * @param[in] key The attribute key
     * @return double with the value associated to key
     */
    virtual double getAttributeDouble(SumoXMLAttr key) const = 0;

    /**@brief method for setting the attribute and letting the object perform additional changes
     * @param[in] key The attribute key
     * @param[in] value The new value
     * @param[in] undoList The undoList on which to register changes
     */
    virtual void setAttribute(SumoXMLAttr key, const std::string& value, GNEUndoList* undoList) = 0;

    /**@brief method for checking if the key and their conrrespond attribute are valids
     * @param[in] key The attribute key
     * @param[in] value The value asociated to key key
     * @return true if the value is valid, false in other case
     */
    virtual bool isValid(SumoXMLAttr key, const std::string& value) = 0;

    /* @brief method for enable attribute
     * @param[in] key The attribute key
     * @param[in] undoList The undoList on which to register changes
     * @note certain attributes can be only enabled, and can produce the disabling of other attributes
     */
    void enableAttribute(SumoXMLAttr key, GNEUndoList* undoList);

    /* @brief method for disable attribute
     * @param[in] key The attribute key
     * @param[in] undoList The undoList on which to register changes
     * @note certain attributes can be only enabled, and can produce the disabling of other attributes
     */
    void disableAttribute(SumoXMLAttr key, GNEUndoList* undoList);

    /* @brief method for check if the value for certain attribute is set
     * @param[in] key The attribute key
     */
    virtual bool isAttributeEnabled(SumoXMLAttr key) const = 0;

    /// @brief get PopPup ID (Used in AC Hierarchy)
    virtual std::string getPopUpID() const = 0;

    /// @brief get Hierarchy Name (Used in AC Hierarchy)
    virtual std::string getHierarchyName() const = 0;
    /// @}

    /// @brief get parameters map
    const std::map<std::string, std::string>& getACParametersMap() const;

protected:
    /// @brief Additional Boundary
    Boundary myBoundary;

    /// @brief geometry to be precomputed in updateGeometry(...)
    GNEGeometry::Geometry myAdditionalGeometry;

    /// @brief name of additional
    std::string myAdditionalName;

    /// @brief pointer to special color (used for drawing Additional with a certain color, mainly used for selections)
    const RGBColor* mySpecialColor = nullptr;

    /// @brief change all attributes of additional with their default values (note: this cannot be undo)
    void setDefaultValues();

    /// @name Functions relative to change values in setAttribute(...)
    /// @{

    /// @brief check if a new additional ID is valid
    bool isValidAdditionalID(const std::string& newID) const;

    /// @brief check if a new detector ID is valid
    bool isValidDetectorID(const std::string& newID) const;

    /// @}

    /// @brief draw additional ID
    void drawAdditionalID(const GUIVisualizationSettings& s) const;

    /// @brief draw additional name
    void drawAdditionalName(const GUIVisualizationSettings& s) const;

    /// @brief replace additional parent edges
    void replaceAdditionalParentEdges(const std::string& value);

    /// @brief replace additional parent lanes
    void replaceAdditionalParentLanes(const std::string& value);

    /// @brief replace additional child edges
    void replaceAdditionalChildEdges(const std::string& value);

    /// @brief replace additional child lanes
    void replaceAdditionalChildLanes(const std::string& value);

    /// @brief replace additional parent
    void replaceAdditionalParent(SumoXMLTag tag, const std::string& value, const int parentIndex);

    /// @brief replace demand element parent
    void replaceDemandElementParent(SumoXMLTag tag, const std::string& value, const int parentIndex);

    /// @brief calculate perpendicular line between lane parents
    void calculatePerpendicularLine(const double endLaneposition);

    /// @brief draw squared additional
    void drawSquaredAdditional(const GUIVisualizationSettings& s, const Position& pos, const double size, GUITexture texture, GUITexture selectedTexture) const;

private:
    /**@brief check restriction with the number of children
     * @throw ProcessError if is called without be reimplemented in child class
     */
    virtual bool checkChildAdditionalRestriction() const;

    /// @brief method for setting the attribute and nothing else (used in GNEChange_Attribute)
    virtual void setAttribute(SumoXMLAttr key, const std::string& value) = 0;

    /// @brief set move shape
    virtual void setMoveShape(const GNEMoveResult& moveResult) = 0;

    /// @brief commit move shape
    virtual void commitMoveShape(const GNEMoveResult& moveResult, GNEUndoList* undoList) = 0;

    /// @brief method for enabling the attribute and nothing else (used in GNEChange_EnableAttribute)
    void setEnabledAttribute(const int enabledAttributes);

    /// @brief Invalidated copy constructor.
    GNEAdditional(const GNEAdditional&) = delete;

    /// @brief Invalidated assignment operator.
    GNEAdditional& operator=(const GNEAdditional&) = delete;
};

