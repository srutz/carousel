#ifndef IMAGESPECTABLEMODEL_H
#define IMAGESPECTABLEMODEL_H

#include <QAbstractTableModel>
#include "gallery.h"
#include <memory>
#include <vector>

using namespace std;

class ImageSpecTableModel : public QAbstractTableModel
{
public:
    explicit ImageSpecTableModel(QObject *parent = nullptr)
        : QAbstractTableModel(parent) {}

    void setImageSpecs(const vector<shared_ptr<ImageSpec>>& specs) {
        beginResetModel();
        m_specs = specs;
        endResetModel();
    }

    int rowCount(const QModelIndex &parent = QModelIndex()) const override {
        if (parent.isValid())
            return 0;
        return static_cast<int>(m_specs.size());
    }

    int columnCount(const QModelIndex &parent = QModelIndex()) const override {
        if (parent.isValid())
            return 0;
        return 4; // Filename, Directory, Last Modified, and Size columns
    }

    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override {
        if (role != Qt::DisplayRole)
            return QVariant();

        if (orientation == Qt::Horizontal) {
            switch (section) {
                case 0:
                    return tr("Filename");
                case 1:
                    return tr("Directory");
                case 2:
                    return tr("Last Modified");
                case 3:
                    return tr("Size");
                default:
                    return QVariant();
            }
        }
        return QVariant();
    }

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override {
        if (!index.isValid() || index.row() >= m_specs.size())
            return QVariant();

        auto ptr = m_specs.at(index.row());
        const ImageSpec &spec = *ptr;

        if (role == Qt::DisplayRole) {
            switch (index.column()) {
                case 0:
                    return spec.m_filename;
                case 1:
                    return spec.m_directory;
                case 2:
                    return QDateTime::fromMSecsSinceEpoch(spec.m_lastModified);
                case 3:
                    return QLocale().toString(spec.m_size);
                default:
                    return QVariant();
            }
        } else if (role == Qt::TextAlignmentRole) {
            auto c = index.column();
            return QVariant(int((c == 2 || c == 3) ? Qt::AlignRight | Qt::AlignVCenter : Qt::AlignLeft | Qt::AlignVCenter));
        }

        return QVariant();
    }

    Qt::ItemFlags flags(const QModelIndex &index) const override {
        if (!index.isValid())
            return Qt::NoItemFlags;

        Qt::ItemFlags flags = QAbstractTableModel::flags(index);
        //if (index.column() == 1) {
        //    flags |= Qt::ItemIsUserCheckable;
        //}
        return flags;
    }

    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override {
        if (!index.isValid() || index.row() >= m_specs.size())
            return false;

        if (index.column() == 1 && role == Qt::CheckStateRole) {
            auto ptr = m_specs.at(index.row());
            ptr->m_valid = (value.toInt() == Qt::Checked);
            emit dataChanged(index, index, {role});
            return true;
        }

        return false;
    }

private:
    vector<shared_ptr<ImageSpec>> m_specs;
};

#endif // IMAGESPECTABLEMODEL_H

