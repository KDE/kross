/***************************************************************************
 * actioncollectionmodel.h
 * This file is part of the KDE project
 * copyright (C) 2006-2007 by Sebastian Sauer (mail@dipe.org)
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 * You should have received a copy of the GNU Library General Public License
 * along with this program; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 ***************************************************************************/

#ifndef KROSS_MODEL_H
#define KROSS_MODEL_H

#include <kross/ui/krossui_export.h>

#include <QModelIndex>
#include <QSortFilterProxyModel>

namespace Kross
{

// Forward declarations.
class Action;
class ActionCollection;
class ActionCollectionModelItem;

/**
 * The ActionCollectionModel class implements a QAbstractItemModel to provide
 * a model for views of a \a ActionCollection instance that manages a
 * collection of \a Action instances.
 *
 * Important implementation detatils:
 * \li An action can not have children.
 * \li A collection can have both collections and actions as children.
 * \li This model lists actions before collections.
 * \li The internalPointer() of QModelIndex is used to hold a pointer to the parent collection.
 */
class KROSSUI_EXPORT ActionCollectionModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    enum Mode {
        None = 0,
        Icons = 1,
        ToolTips = 2,
        UserCheckable = 4
                        //Editable = 8
    };

    explicit ActionCollectionModel(QObject *parent, ActionCollection *collection = nullptr, Mode mode = Mode(Icons | ToolTips));
    ~ActionCollectionModel() override;

    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &index) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;

    bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
    bool insertColumns(int column, int count, const QModelIndex &parent = QModelIndex()) override;
    bool removeColumns(int column, int count, const QModelIndex &parent = QModelIndex()) override;

    //Qt::DropActions supportedDragActions() const;
    QStringList mimeTypes() const override;
    QMimeData *mimeData(const QModelIndexList &indexes) const override;
    bool dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent) override;

    Qt::DropActions supportedDropActions() const override;

    QModelIndex indexForCollection(ActionCollection *collection) const;
    QModelIndex indexForAction(Action *action) const;
    /// Return the root collection
    ActionCollection *rootCollection() const;

    /**
    * \return the \a Action instance the as argument passed QModelIndex
    * represents or NULL if the QModelIndex is not a \a Action .
    */
    static Action *action(const QModelIndex &index);

    /**
    * \return the \a ActionCollection instance the as argument passed QModelIndex
    * represents or NULL if the QModelIndex is not a \a ActionCollection .
    */
    static ActionCollection *collection(const QModelIndex &index);

protected:
    /// @returns the row number of the @p collection
    int rowNumber(ActionCollection *collection) const;

private Q_SLOTS:
    void slotUpdated();

    void slotDataChanged(ActionCollection *);
    void slotDataChanged(Action *);

    void slotCollectionToBeInserted(ActionCollection *child, ActionCollection *parent);
    void slotCollectionInserted(ActionCollection *child, ActionCollection *parent);
    void slotCollectionToBeRemoved(ActionCollection *child, ActionCollection *parent);
    void slotCollectionRemoved(ActionCollection *child, ActionCollection *parent);

    void slotActionToBeInserted(Action *child, ActionCollection *parent);
    void slotActionInserted(Action *child, ActionCollection *parent);
    void slotActionToBeRemoved(Action *child, ActionCollection *parent);
    void slotActionRemoved(Action *child, ActionCollection *parent);
private:
    /// \internal d-pointer class.
    class Private;
    /// \internal d-pointer instance.
    Private *const d;
};

/**
 * The ActionCollectionProxyModel class implements a QSortFilterProxyModel
 * for a \a ActionCollectionModel instance.
 */
class KROSSUI_EXPORT ActionCollectionProxyModel : public QSortFilterProxyModel
{
public:
    explicit ActionCollectionProxyModel(QObject *parent, ActionCollectionModel *model = nullptr);
    ~ActionCollectionProxyModel() override;

private:
    /// Set the \a ActionCollectionModel source model we are proxy for.
    void setSourceModel(QAbstractItemModel *sourceModel) override;
    /// Implements a filter for the QModelIndex instances.
    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const override;
};

}

#endif

