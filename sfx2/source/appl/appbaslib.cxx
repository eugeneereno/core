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

#include <appbaslib.hxx>

#include <sfx2/app.hxx>

#include <tools/diagnose_ex.h>
#include <com/sun/star/uno/XComponentContext.hpp>

using namespace ::com::sun::star::uno;
using namespace ::com::sun::star::lang;
using namespace ::com::sun::star::script;
using namespace ::com::sun::star::embed;


SfxBasicManagerHolder::SfxBasicManagerHolder() {}

void SfxBasicManagerHolder::Notify(SfxBroadcaster& rBC, SfxHint const& rHint)
{
    (void) rBC;
    if (SfxHintId::Dying == rHint.GetId())
    {
        mxBasicContainer.clear();
        mxDialogContainer.clear();
    }
}

void SfxBasicManagerHolder::storeAllLibraries()
{
#if HAVE_FEATURE_SCRIPTING
    try
    {
        if ( mxBasicContainer.is() )
            mxBasicContainer->storeLibraries();
        if ( mxDialogContainer.is() )
            mxDialogContainer->storeLibraries();
    }
    catch( const Exception& )
    {
        DBG_UNHANDLED_EXCEPTION("sfx.appl");
    }
#endif
}

void SfxBasicManagerHolder::setStorage( const Reference< XStorage >& _rxStorage )
{
#if !HAVE_FEATURE_SCRIPTING
    (void) _rxStorage;
#else
    try
    {
        if ( mxBasicContainer.is() )
            mxBasicContainer->setRootStorage( _rxStorage );
        if ( mxDialogContainer.is() )
            mxDialogContainer->setRootStorage( _rxStorage );
    }
    catch( const Exception& )
    {
        DBG_UNHANDLED_EXCEPTION("sfx.appl");
    }
#endif
}

void SfxBasicManagerHolder::storeLibrariesToStorage( const Reference< XStorage >& _rxStorage )
{
#if !HAVE_FEATURE_SCRIPTING
    (void) _rxStorage;
#else
    if ( mxBasicContainer.is() )
        mxBasicContainer->storeLibrariesToStorage( _rxStorage );
    if ( mxDialogContainer.is() )
        mxDialogContainer->storeLibrariesToStorage( _rxStorage );
#endif
}

XLibraryContainer * SfxBasicManagerHolder::getLibraryContainer( ContainerType _eType )
{
    switch ( _eType )
    {
    case SCRIPTS:   return mxBasicContainer.get();
    case DIALOGS:   return mxDialogContainer.get();
    }
    OSL_FAIL( "SfxBasicManagerHolder::getLibraryContainer: illegal container type!" );
    return nullptr;
}

void SfxBasicManagerHolder::impl_releaseContainers()
{
    mxBasicContainer.clear();
    mxDialogContainer.clear();
}

bool SfxBasicManagerHolder::LegacyPsswdBinaryLimitExceeded( std::vector< OUString >& sModules )
{
#if !HAVE_FEATURE_SCRIPTING
    (void) sModules;
#endif
    return true;
}

// Service for application library container
extern "C" SAL_DLLPUBLIC_EXPORT css::uno::XInterface *
com_sun_star_comp_sfx2_ApplicationDialogLibraryContainer_get_implementation(
    css::uno::XComponentContext *,
    css::uno::Sequence<css::uno::Any> const &)
{
    css::uno::XInterface* pRet = SfxGetpApp()->GetDialogContainer();
    pRet->acquire();
    return pRet;
}

// Service for application library container
extern "C" SAL_DLLPUBLIC_EXPORT css::uno::XInterface *
com_sun_star_comp_sfx2_ApplicationScriptLibraryContainer_get_implementation(
    css::uno::XComponentContext *,
    css::uno::Sequence<css::uno::Any> const &)
{
    css::uno::XInterface* pRet = SfxGetpApp()->GetBasicContainer();
    pRet->acquire();
    return pRet;
}

/* vim:set shiftwidth=4 softtabstop=4 expandtab: */
