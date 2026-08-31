#include "natureprovider.h"

#include <QDate>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QUrl>
#include <QLoggingCategory>
#include <QFile>
#include <QDir>

#include <KIO/StoredTransferJob>
#include <KPluginFactory>

Q_LOGGING_CATEGORY(LOG_NATURE_PROVIDER, "plasma.potd.natureprovider")

namespace
{
const QUrl baseUrl(
    QStringLiteral(
        "https://blueheron786.github.io/"
        "potd-nature-wallpapers-plasma/"
    )
);

const QDate epoch(2026, 1, 1);

// Hardcoded count for now - this should match the number of wallpapers
// deployed to GitHub Pages. When the manifest format is finalized,
// this can be replaced with a manifest fetch.
constexpr int DefaultWallpaperCount = 365;

QDate requestedDate(const QVariantList &args)
{
    for (const QVariant &arg : args) {
        if (arg.canConvert<QString>()) {
            const QDate date = QDate::fromString(
                arg.toString(),
                Qt::ISODate
            );

            if (date.isValid()) {
                return date;
            }
        }
    }

    return QDate::currentDate();
}

QString wallpaperFileName(int number)
{
    return QStringLiteral("wallpaper_%1.jpg")
        .arg(number, 3, 10, QLatin1Char('0'));
}

int calculateWallpaperNumber(const QDate &date, int count)
{
    if (count <= 0) {
        return -1;
    }

    const qint64 days = epoch.daysTo(date);

    // Normalize negative modulo results for dates before epoch
    const int number = static_cast<int>(((days % count) + count) % count) + 1;

    return number;
}
}

/**
 * Simple structure to hold attribution data for a wallpaper.
 * This is prepared for future use when proper licensing is established.
 *
 * NOTE: Attribution should ONLY be displayed if the image licensing
 * explicitly permits it and attribution requirements are met.
 */
struct Attribution {
    QString photographer;
    QString photographerUrl;
    QString sourceUrl;

    bool isValid() const {
        return !photographer.isEmpty() && !sourceUrl.isEmpty();
    }
};

// Placeholder for future attribution parsing
// This would be implemented when a manifest with attribution data is available
Attribution parseAttributionData(const QJsonObject &object, int wallpaperNumber)
{
    Q_UNUSED(object);
    Q_UNUSED(wallpaperNumber);

    // TODO: Implement when manifest format is finalized and licensing is verified
    // Example structure for future implementation:
    /*
    Attribution attr;

    // Try to find attribution for this specific wallpaper
    // This would depend on the manifest structure

    return attr;
    */

    return Attribution{};
}

NatureProvider::NatureProvider(
    QObject *parent,
    const KPluginMetaData &data,
    const QVariantList &args
)
    : PotdProvider(parent, data, args)
{
    const QDate date = requestedDate(args);

    m_infoUrl = baseUrl;
    m_title = QStringLiteral("Nature Wallpaper");

    // Use the default wallpaper count. In the future, this could be
    // fetched from a manifest file, but for now we use a fixed count
    // that matches the deployed wallpaper collection.
    const int count = DefaultWallpaperCount;

    if (count <= 0) {
        qCWarning(LOG_NATURE_PROVIDER) << "Invalid wallpaper count:" << count;
        Q_EMIT error(this);
        return;
    }

    const int number = calculateWallpaperNumber(date, count);

    if (number < 1) {
        qCWarning(LOG_NATURE_PROVIDER) << "Failed to calculate wallpaper number for date:" << date.toString(Qt::ISODate);
        Q_EMIT error(this);
        return;
    }

    const QString filename = wallpaperFileName(number);

    m_remoteUrl =
        baseUrl.resolved(
            QUrl(
                QStringLiteral("wallpapers/") +
                filename
            )
        );

    qCDebug(LOG_NATURE_PROVIDER) << "Requesting wallpaper:" << filename << "for date:" << date.toString(Qt::ISODate);

    // TODO: Future enhancement - fetch and parse attribution data
    // This requires:
    // 1. A manifest with attribution information
    // 2. Verified licensing that permits redistribution with attribution
    // 3. Explicit consent from photographers for this specific use
    //
    // WARNING: Do NOT display attribution without verifying:
    // - Image licensing permits the intended use
    // - Attribution meets the license requirements
    // - You have the right to redistribute the images
    //
    // See: https://unsplash.com/license for Unsplash license terms

    KIO::StoredTransferJob *imageJob =
        KIO::storedGet(
            m_remoteUrl,
            KIO::NoReload,
            KIO::HideProgressInfo
        );

    connect(
        imageJob,
        &KIO::StoredTransferJob::finished,
        this,
        &NatureProvider::imageRequestFinished
    );
}

void NatureProvider::imageRequestFinished(KJob *job)
{
    auto *transferJob =
        static_cast<KIO::StoredTransferJob *>(job);

    if (transferJob->error()) {
        qCWarning(LOG_NATURE_PROVIDER) << "Failed to download wallpaper:" << transferJob->errorString()
                                       << "URL:" << m_remoteUrl.toString();
        Q_EMIT error(this);
        return;
    }

    const QByteArray imageData = transferJob->data();

    if (imageData.isEmpty()) {
        qCWarning(LOG_NATURE_PROVIDER) << "Downloaded wallpaper data is empty";
        Q_EMIT error(this);
        return;
    }

    const QImage image = QImage::fromData(imageData);

    if (image.isNull()) {
        qCWarning(LOG_NATURE_PROVIDER) << "Failed to decode wallpaper image data";
        Q_EMIT error(this);
        return;
    }

    Q_EMIT finished(this, image);
}

K_PLUGIN_CLASS_WITH_JSON(
    NatureProvider,
    "natureprovider.json"
)

#include "natureprovider.moc"
