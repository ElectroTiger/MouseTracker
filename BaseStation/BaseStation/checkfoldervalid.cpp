#include "checkfoldervalid.h"

CheckFolderValid::CheckFolderValid(QObject *parent)
    : QAbstractItemModel(parent)
{
}

QVariant CheckFolderValid::headerData(int section, Qt::Orientation orientation, int role) const
{
    // FIXME: Implement me!
}

QModelIndex CheckFolderValid::index(int row, int column, const QModelIndex &parent) const
{
    // FIXME: Implement me!
}

QModelIndex CheckFolderValid::parent(const QModelIndex &index) const
{
    // FIXME: Implement me!
}

int CheckFolderValid::rowCount(const QModelIndex &parent) const
{
    if (!parent.isValid())
        return 0;

    // FIXME: Implement me!
}

int CheckFolderValid::columnCount(const QModelIndex &parent) const
{
    if (!parent.isValid())
        return 0;

    // FIXME: Implement me!
}

QVariant CheckFolderValid::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    // FIXME: Implement me!
    return QVariant();
}
