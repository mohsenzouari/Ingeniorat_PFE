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
/// @file    GNENetHelper.h
/// @author  Pablo Alvarez Lopez
/// @date    Mar 2020
///
// Helper for GNENet
//
/****************************************************************************/
#pragma once
#include <config.h>

#include <utils/foxtools/fxheader.h>
#include <foreign/rtree/SUMORTree.h>
#include <netbuild/NBEdge.h>
#include <netbuild/NBTrafficLightLogicCont.h>
#include <netbuild/NBVehicle.h>
#include <netedit/changes/GNEChange.h>
#include <utils/common/IDSupplier.h>
#include <utils/common/SUMOVehicleClass.h>
#include <utils/geom/Boundary.h>
#include <utils/geom/PositionVector.h>
#include <utils/gui/globjects/GUIGlObject.h>
#include <utils/gui/globjects/GUIShapeContainer.h>
#include <utils/gui/settings/GUIVisualizationSettings.h>
#include <utils/router/SUMOAbstractRouter.h>
#include <utils/xml/SUMOXMLDefinitions.h>


// ===========================================================================
// class declarations
// ===========================================================================

class NBNetBuilder;
class GNEAdditional;
class GNEDataSet;
class GNEDemandElement;
class GNEApplicationWindow;
class GNEAttributeCarrier;
class GNEConnection;
class GNECrossing;
class GNEJunction;
class GNEEdgeType;
class GNELaneType;
class GNEEdge;
class GNELane;
class GNENetworkElement;
class GNEPOI;
class GNEPoly;
class GNEShape;
class GNETAZElement;
class GNEUndoList;
class GNEViewNet;

// ===========================================================================
// class definitions
// ===========================================================================

struct GNENetHelper {

    /// @brief struct used for saving all attribute carriers of net, in different formats
    class AttributeCarriers {

        /// @brief declare friend class
        //friend class GNEAdditionalHandler;
        friend class GNEAdditionalHandler;
        friend class GNERouteHandler;
        friend class GNEChange_Junction;
        friend class GNEChange_EdgeType;
        friend class GNEChange_LaneType;
        friend class GNEChange_Edge;
        friend class GNEChange_Additional;
        friend class GNEChange_Shape;
        friend class GNEChange_TAZElement;
        friend class GNEChange_DemandElement;
        friend class GNEChange_DataSet;
        friend class GNEChange_DataInterval;
        friend class GNEChange_GenericData;

    public:
        /// @brief constructor
        AttributeCarriers(GNENet* net);

        /// @brief destructor
        ~AttributeCarriers();

        /// @brief update ID
        void updateID(GNEAttributeCarrier* AC, const std::string newID);

        /// @brief remap junction and edge IDs
        void remapJunctionAndEdgeIds();

        /// @name function for junctions
        /// @{
        /// @brief registers a junction in GNENet containers
        GNEJunction* registerJunction(GNEJunction* junction);

        /// @brief map with the ID and pointer to junctions of net
        const std::map<std::string, GNEJunction*>& getJunctions() const;

        /// @brief clear junctions
        void clearJunctions();

        /// @brief get number of selected junctions
        int getNumberOfSelectedJunctions() const;

        /// @}

        /// @name function for edgeTypes
        /// @{
        /// @brief registers a edge in GNENet containers
        GNEEdgeType* registerEdgeType(GNEEdgeType* edgeType);

        /// @brief map with the ID and pointer to edgeTypes of net
        const std::map<std::string, GNEEdgeType*>& getEdgeTypes() const;

        /// @brief clear edgeTypes
        void clearEdgeTypes();

        /// @}

        /// @name function for edges
        /// @{
        /// @brief registers an edge with GNENet containers
        GNEEdge* registerEdge(GNEEdge* edge);

        /// @brief map with the ID and pointer to edges of net
        const std::map<std::string, GNEEdge*>& getEdges() const;

        /// @brief clear edges
        void clearEdges();

        /// @brief get number of selected edges
        int getNumberOfSelectedEdges() const;

        /// @brief get number of selected lanes
        int getNumberOfSelectedLanes() const;

        /// @}

        /// @brief get number of selected connections
        int getNumberOfSelectedConnections() const;

        /// @brief get number of selected crossings
        int getNumberOfSelectedCrossings() const;

        /// @name function for additionals
        /// @{
        /// @brief get additionals
        const std::map<SumoXMLTag, std::map<std::string, GNEAdditional*> >& getAdditionals() const;

        /// @brief clear additionals
        void clearAdditionals();

        /// @brief get number of selected additionals
        int getNumberOfSelectedAdditionals() const;

        /// @}

        /// @name function for shapes
        /// @{
        /// @brief get shapes
        const std::map<SumoXMLTag, std::map<std::string, GNEShape*> >& getShapes() const;

        /// @brief clear shapes
        void clearShapes();

        /// @brief get number of selected polygons
        int getNumberOfSelectedPolygons() const;

        /// @brief get number of selected POIs
        int getNumberOfSelectedPOIs() const;

        /// @}

        /// @name function for TAZElements
        /// @{
        /// @brief get TAZElements
        const std::map<SumoXMLTag, std::map<std::string, GNETAZElement*> >& getTAZElements() const;

        /// @brief clear TAZElements
        void clearTAZElements();

        /// @brief get number of selected TAZs
        int getNumberOfSelectedTAZs() const;

        /// @}

        /// @name function for demand elements
        /// @{
        /// @brief get demand elements
        const std::map<SumoXMLTag, std::map<std::string, GNEDemandElement*> >& getDemandElements() const;

        /// @brief clear demand elements
        void clearDemandElements();

        /// @brief add default VTypes
        void addDefaultVTypes();

        /// @brief get number of selected routes
        int getNumberOfSelectedRoutes() const;

        /// @brief get number of selected vehicles
        int getNumberOfSelectedVehicles() const;

        /// @brief get number of selected persons
        int getNumberOfSelectedPersons() const;

        /// @brief get number of selected person trips
        int getNumberOfSelectedPersonTrips() const;

        /// @brief get number of selected walks
        int getNumberOfSelectedWalks() const;

        /// @brief get number of selected rides
        int getNumberOfSelectedRides() const;

        /// @brief get number of selected containers
        int getNumberOfSelectedContainers() const;

        /// @brief get number of selected transports
        int getNumberOfSelectedTransport() const;

        /// @brief get number of selected tranships
        int getNumberOfSelectedTranships() const;

        /// @brief get number of selected stops
        int getNumberOfSelectedStops() const;

        /// @}

        /// @name function for data sets
        /// @{
        /// @brief get demand elements
        const std::map<std::string, GNEDataSet*>& getDataSets() const;

        /// @brief clear demand elements
        void clearDataSets();

        /// @}

        /// @name function for generic datas
        /// @{
        /// @brief retrieve generic datas within the given interval
        std::vector<GNEGenericData*> retrieveGenericDatas(const SumoXMLTag genericDataTag, const double begin, const double end);

        /// @brief get number of selected edge datas
        int getNumberOfSelectedEdgeDatas() const;

        /// @brief get number of selected edge rel datas
        int getNumberOfSelectedEdgeRelDatas() const;

        /// @brief get number of selected edge TAZ Rels
        int getNumberOfSelectedEdgeTAZRel() const;

        /// @}

    protected:
        /// @name Insertion and erasing of GNEJunctions
        /// @{
        /// @brief inserts a single junction into the net and into the underlying netbuild-container
        void insertJunction(GNEJunction* junction);

        /// @brief deletes a single junction
        void deleteSingleJunction(GNEJunction* junction);

        /// @brief update junction ID in container
        void updateJunctionID(GNEAttributeCarrier* AC, const std::string& newID);

        /// @}

        /// @name Insertion and erasing of GNEEdgeTypes
        /// @{
        /// @brief return true if given edgeType exist
        bool edgeTypeExist(const GNEEdgeType* edgeType) const;

        /// @brief inserts a single edgeType into the net and into the underlying netbuild-container
        void insertEdgeType(GNEEdgeType* edgeType);

        /// @brief deletes edgeType
        void deleteEdgeType(GNEEdgeType* edgeType);

        /// @brief update edgeType ID in container
        void updateEdgeTypeID(GNEAttributeCarrier* AC, const std::string& newID);

        /// @}

        /// @name Insertion and erasing of GNEEdges
        /// @{
        /// @brief inserts a single edge into the net and into the underlying netbuild-container
        void insertEdge(GNEEdge* edge);

        /// @brief deletes a single edge
        void deleteSingleEdge(GNEEdge* edge);

        /// @brief update edge ID in container
        void updateEdgeID(GNEAttributeCarrier* AC, const std::string& newID);

        /// @}

        /// @name Insertion and erasing of GNEAdditionals items
        /// @{

        /// @brief return true if given additional exist
        bool additionalExist(const GNEAdditional* additional) const;

        /**@brief Insert a additional element int GNENet container.
         * @throw processError if route was already inserted
         */
        void insertAdditional(GNEAdditional* additional);

        /**@brief delete additional element of GNENet container
         * @throw processError if additional wasn't previously inserted
         */
        void deleteAdditional(GNEAdditional* additional);

        /// @brief update additional ID in container
        void updateAdditionalID(GNEAttributeCarrier* AC, const std::string& newID);

        /// @}

        /// @name Insertion and erasing of GNEShapes items
        /// @{

        /// @brief return true if given shape exist
        bool shapeExist(const GNEShape* shape) const;

        /**@brief Insert a shape element int GNENet container.
         * @throw processError if route was already inserted
         */
        void insertShape(GNEShape* shape);

        /**@brief delete shape element of GNENet container
         * @throw processError if shape wasn't previously inserted
         */
        void deleteShape(GNEShape* shape);

        /// @brief update shape ID in container
        void updateShapeID(GNEAttributeCarrier* AC, const std::string& newID);

        /// @name Insertion and erasing of GNETAZElements items
        /// @{

        /// @brief return true if given TAZElement exist
        bool TAZElementExist(const GNETAZElement* TAZElement) const;

        /**@brief Insert a TAZElement element int GNENet container.
         * @throw processError if route was already inserted
         */
        void insertTAZElement(GNETAZElement* TAZElement);

        /**@brief delete TAZElement element of GNENet container
         * @throw processError if TAZElement wasn't previously inserted
         */
        void deleteTAZElement(GNETAZElement* TAZElement);

        /// @brief update TAZElement ID in container
        void updateTAZElementID(GNEAttributeCarrier* AC, const std::string& newID);

        /// @}

        /// @name Insertion and erasing of GNEDemandElements items
        /// @{

        /// @brief return true if given demand element exist
        bool demandElementExist(const GNEDemandElement* demandElement) const;

        /**@brief Insert a demand element element int GNENet container.
         * @throw processError if route was already inserted
         */
        void insertDemandElement(GNEDemandElement* demandElement);

        /**@brief delete demand element element of GNENet container
         * @throw processError if demand element wasn't previously inserted
         */
        void deleteDemandElement(GNEDemandElement* demandElement);

        /// @brief update demand element ID in container
        void updateDemandElementID(GNEAttributeCarrier* AC, const std::string& newID);

        /// @}

        /// @name Insertion and erasing of GNEDataSets items
        /// @{

        /// @brief return true if given demand element exist
        bool dataSetExist(const GNEDataSet* dataSet) const;

        /**@brief Insert a demand element element int GNENet container.
         * @throw processError if route was already inserted
         */
        void insertDataSet(GNEDataSet* dataSet);

        /**@brief delete demand element element of GNENet container
         * @throw processError if demand element wasn't previously inserted
         */
        void deleteDataSet(GNEDataSet* dataSet);

        /// @brief update data element ID in container
        void updateDataSetID(GNEAttributeCarrier* AC, const std::string& newID);

        /// @}

    private:
        /// @brief map with the ID and pointer to junctions of net
        std::map<std::string, GNEJunction*> myJunctions;

        /// @brief map with the ID and pointer to edgeTypes of net
        std::map<std::string, GNEEdgeType*> myEdgeTypes;

        /// @brief map with the ID and pointer to edges of net
        std::map<std::string, GNEEdge*> myEdges;

        /// @brief map with the ID and pointer to additional elements of net
        std::map<SumoXMLTag, std::map<std::string, GNEAdditional*> > myAdditionals;

        /// @brief map with the ID and pointer to shape elements of net
        std::map<SumoXMLTag, std::map<std::string, GNEShape*> > myShapes;

        /// @brief map with the ID and pointer to TAZElement elements of net
        std::map<SumoXMLTag, std::map<std::string, GNETAZElement*> > myTAZElements;

        /// @brief map with the ID and pointer to demand elements of net
        std::map<SumoXMLTag, std::map<std::string, GNEDemandElement*> > myDemandElements;

        /// @brief map with the ID and pointer to data sets of net
        std::map<std::string, GNEDataSet*> myDataSets;

        /// @brief pointer to net
        GNENet* myNet;

        /// @brief flag used to indicate if created shape can be undo
        bool myAllowUndoShapes;

        /// @brief Invalidated copy constructor.
        AttributeCarriers(const AttributeCarriers&) = delete;

        /// @brief Invalidated assignment operator.
        AttributeCarriers& operator=(const AttributeCarriers&) = delete;
    };

    /// @brief class for GNEChange_ReplaceEdgeInTLS
    class GNEChange_ReplaceEdgeInTLS : public GNEChange {
        FXDECLARE_ABSTRACT(GNEChange_ReplaceEdgeInTLS)

    public:
        /// @brief constructor
        GNEChange_ReplaceEdgeInTLS(NBTrafficLightLogicCont& tllcont, NBEdge* replaced, NBEdge* by);

        /// @bief destructor
        ~GNEChange_ReplaceEdgeInTLS();

        /// @brief undo name
        FXString undoName() const;

        /// @brief get Redo name
        FXString redoName() const;

        /// @brief undo action
        void undo();

        /// @brief redo action
        void redo();

        /// @brief wether original and new value differ
        bool trueChange();

    private:
        /// @brief container for traffic light logic
        NBTrafficLightLogicCont& myTllcont;

        /// @brief replaced NBEdge
        NBEdge* myReplaced;

        /// @brief replaced by NBEdge
        NBEdge* myBy;
    };
};
