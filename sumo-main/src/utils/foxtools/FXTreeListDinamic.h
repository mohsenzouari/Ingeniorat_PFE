/****************************************************************************/
// Eclipse SUMO, Simulation of Urban MObility; see https://eclipse.org/sumo
// Copyright (C) 2004-2021 German Aerospace Center (DLR) and others.
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
/// @file    FXTreeListDinamic.h
/// @author  Pablo Alvarez Lopez
/// @date    Feb 2021
///
//
/****************************************************************************/

#pragma once
#include <config.h>


#include "fxheader.h"

/// @brief FXTreeListDinamic
class FXTreeListDinamic : public FXTreeList {
    /// @brief FOX-declaration
    FXDECLARE(FXTreeListDinamic)

public:
    /// @brief Construct a menu check
    FXTreeListDinamic(FXComposite* p, FXObject* tgt, FXSelector sel);

    /// @brief Show FXTreeListDinamic
    void show();

    /// @name FOX calls
    /// @{
    long onLeftBtnPress(FXObject*, FXSelector, void*);
    /// @}

protected:
    /// @brief default constructor
    FXTreeListDinamic();

private:
    /// @brief Invalidated copy constructor.
    FXTreeListDinamic(const FXTreeListDinamic&) = delete;

    /// @brief Invalidated assignment operator.
    FXTreeListDinamic& operator=(const FXTreeListDinamic&) = delete;
};
