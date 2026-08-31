#pragma once

#include <plasma/potdprovider/potdprovider.h>

class KJob;

class NatureProvider : public PotdProvider
{
    Q_OBJECT

public:
    explicit NatureProvider(
        QObject *parent,
        const KPluginMetaData &data,
        const QVariantList &args
    );

private:
    void manifestRequestFinished(KJob *job);
    void imageRequestFinished(KJob *job);
};
