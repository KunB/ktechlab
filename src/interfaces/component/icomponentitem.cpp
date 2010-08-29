/*
    Copyright (C) 2009-2010 Julian Bäume <julian@svg4all.de>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

*/

#include "icomponentitem.h"
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <KDebug>
#include <idocumentmodel.h>

using namespace KTechLab;

IComponentItem::IComponentItem(QGraphicsItem* parentItem)
    : QGraphicsSvgItem(parentItem),
      m_document(0)
{
    setAcceptHoverEvents(true);
    setFlags(
        ItemIsFocusable | ItemIsSelectable |
        ItemIsMovable | ItemSendsScenePositionChanges
    );
}

IComponentItem::~IComponentItem()
{

}

void IComponentItem::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{
    if (   event->button() == Qt::LeftButton
        && contains(event->scenePos())
        && contains(event->buttonDownScenePos(Qt::LeftButton)) ){
        if (event->modifiers() != Qt::ControlModifier)
            scene()->clearSelection();
        setSelected(true);
        event->accept();
    }
    QGraphicsSvgItem::mouseReleaseEvent(event);
}

QString IComponentItem::id() const
{
    if (!m_id.isEmpty())
        return m_id;
    if (data(0).toMap().contains("id")) {
        return data(0).toMap().value("id").toString();
    }

    kWarning() << "no id for IComponentItem:" << this;
    return QString();
}

void IComponentItem::setDocumentModel(IDocumentModel* model)
{
    m_document = model;
}

IDocumentModel* IComponentItem::documentModel() const
{
    return m_document;
}

QVariant KTechLab::IComponentItem::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant& value)
{
    if (change == QGraphicsItem::ItemScenePositionHasChanged && m_document){
        QPointF p = value.toPointF();
        QVariantMap newData = data(0).toMap();
        newData.insert("x", p.x());
        newData.insert("y", p.y());
        setData(0, newData);
        m_document->updateData( "position", newData);
    }
    return QGraphicsItem::itemChange(change, value);
}

void IComponentItem::updateData(const QString& name, const QVariantMap& data)
{

}