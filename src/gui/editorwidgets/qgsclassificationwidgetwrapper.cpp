/***************************************************************************
    qgsclassificationwidgetwrapper.cpp
     --------------------------------------
    Date                 : 5.1.2014
    Copyright            : (C) 2014 Matthias Kuhn
    Email                : matthias at opengis dot ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "qgsclassificationwidgetwrapper.h"

#include "qgscategorizedsymbolrenderer.h"
#include "qgsvectorlayer.h"

QgsClassificationWidgetWrapper::QgsClassificationWidgetWrapper( QgsVectorLayer* vl, int fieldIdx, QWidget* editor, QWidget* parent )
    :  QgsEditorWidgetWrapper( vl, fieldIdx, editor, parent )
    , mComboBox( nullptr )
{
}

QVariant QgsClassificationWidgetWrapper::value() const
{
  return mComboBox->currentData();
}

void QgsClassificationWidgetWrapper::showIndeterminateState()
{
  whileBlocking( mComboBox )->setCurrentIndex( -1 );
}

QWidget*QgsClassificationWidgetWrapper::createWidget( QWidget* parent )
{
  return new QComboBox( parent );
}

void QgsClassificationWidgetWrapper::initWidget( QWidget* editor )
{
  mComboBox = qobject_cast<QComboBox*>( editor );

  if ( mComboBox )
  {
    const QgsCategorizedSymbolRenderer *csr = dynamic_cast<const QgsCategorizedSymbolRenderer *>( layer()->renderer() );
    if ( csr )
    {
      const QgsCategoryList categories = csr->categories();
      for ( int i = 0; i < categories.size(); i++ )
      {
        QString label = categories[i].label();
        QString value = categories[i].value().toString();
        if ( label.isEmpty() )
          label = value;

        mComboBox->addItem( label, value );
      }
    }

    connect( mComboBox, SIGNAL( currentIndexChanged( int ) ), this, SLOT( valueChanged() ) );
  }
}

bool QgsClassificationWidgetWrapper::valid() const
{
  return mComboBox;
}

void QgsClassificationWidgetWrapper::setValue( const QVariant& value )
{
  mComboBox->setCurrentIndex( mComboBox->findData( value ) );
}
