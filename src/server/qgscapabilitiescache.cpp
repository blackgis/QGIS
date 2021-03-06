/***************************************************************************
                              qgscapabilitiescache.h
                              ----------------------
  begin                : May 11th, 2011
  copyright            : (C) 2011 by Marco Hugentobler
  email                : marco dot hugentobler at sourcepole dot ch
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "qgscapabilitiescache.h"
#include "qgslogger.h"
#include <QCoreApplication>

QgsCapabilitiesCache::QgsCapabilitiesCache()
{
  QObject::connect( &mFileSystemWatcher, SIGNAL( fileChanged( const QString& ) ), this, SLOT( removeChangedEntry( const QString& ) ) );
}

QgsCapabilitiesCache::~QgsCapabilitiesCache()
{
}

const QDomDocument* QgsCapabilitiesCache::searchCapabilitiesDocument( const QString& configFilePath, const QString& key )
{
  QCoreApplication::processEvents(); //get updates from file system watcher

  if ( mCachedCapabilities.contains( configFilePath ) && mCachedCapabilities[ configFilePath ].contains( key ) )
  {
    return &mCachedCapabilities[ configFilePath ][ key ];
  }
  else
  {
    return nullptr;
  }
}

void QgsCapabilitiesCache::insertCapabilitiesDocument( const QString& configFilePath, const QString& key, const QDomDocument* doc )
{
  if ( mCachedCapabilities.size() > 40 )
  {
    //remove another cache entry to avoid memory problems
    QHash<QString, QHash<QString, QDomDocument> >::iterator capIt = mCachedCapabilities.begin();
    mFileSystemWatcher.removePath( capIt.key() );
    mCachedCapabilities.erase( capIt );
  }

  if ( !mCachedCapabilities.contains( configFilePath ) )
  {
    mFileSystemWatcher.addPath( configFilePath );
    mCachedCapabilities.insert( configFilePath, QHash<QString, QDomDocument>() );
  }

  mCachedCapabilities[ configFilePath ].insert( key, doc->cloneNode().toDocument() );
}

void QgsCapabilitiesCache::removeCapabilitiesDocument( const QString& path )
{
  mCachedCapabilities.remove( path );
  mFileSystemWatcher.removePath( path );
}

void QgsCapabilitiesCache::removeChangedEntry( const QString& path )
{
  QgsDebugMsg( "Remove capabilities cache entry because file changed" );
  mCachedCapabilities.remove( path );
  mFileSystemWatcher.removePath( path );
}
