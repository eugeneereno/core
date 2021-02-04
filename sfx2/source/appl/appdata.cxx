/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */
/*
 * This file is part of the LibreOffice project.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * This file incorporates work covered by the following license notice:
 *
 *   Licensed to the Apache Software Foundation (ASF) under one or more
 *   contributor license agreements. See the NOTICE file distributed
 *   with this work for additional information regarding copyright
 *   ownership. The ASF licenses this file to you under the Apache
 *   License, Version 2.0 (the "License"); you may not use this file
 *   except in compliance with the License. You may obtain a copy of
 *   the License at http://www.apache.org/licenses/LICENSE-2.0 .
 */

#include <config_features.h>

#include <appdata.hxx>
#include <sfxpicklist.hxx>
#include <sfx2/dispatch.hxx>
#include <sfx2/doctempl.hxx>
#include <sfx2/fcontnr.hxx>
#include <sfx2/module.hxx>
#include <sfx2/msgpool.hxx>
#include <sfx2/sidebar/Theme.hxx>
#include <sfx2/objsh.hxx>
#include <appbaslib.hxx>
#include <childwinimpl.hxx>
#include <ctrlfactoryimpl.hxx>
#include <shellimpl.hxx>
#include <unoctitm.hxx>
#include <svl/svdde.hxx>

using ::com::sun::star::uno::Reference;
using ::com::sun::star::frame::XModel;
using ::com::sun::star::uno::XInterface;

class SfxBasicManagerCreationListener
{
public:
    explicit SfxBasicManagerCreationListener(SfxAppData_Impl& _rAppData)
    {}

    virtual ~SfxBasicManagerCreationListener();
};

SfxBasicManagerCreationListener::~SfxBasicManagerCreationListener()
{}

SfxAppData_Impl::SfxAppData_Impl()
    : pPool(nullptr)
    , pProgress(nullptr)
    , nDocModalMode(0)
    , nRescheduleLocks(0)
    , pBasMgrListener( new SfxBasicManagerCreationListener( *this ) )
    , pViewFrame( nullptr )
    , bDowning( true )
    , bInQuit( false )

{}

SfxAppData_Impl::~SfxAppData_Impl()
{
    DeInitDDE();
}

SfxDocumentTemplates* SfxAppData_Impl::GetDocumentTemplates()
{
    if ( !pTemplates )
        pTemplates.reset(new SfxDocumentTemplates);
    else
        pTemplates->ReInitFromComponent();
    return pTemplates.get();
}

/* vim:set shiftwidth=4 softtabstop=4 expandtab: */
