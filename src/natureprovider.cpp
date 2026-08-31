#include "natureprovider.h"

#include <QDate>
#include <QJsonDocument>
#include <QJsonObject>
#include <QUrl>

#include <KIO/StoredTransferJob>
#include <KPluginFactory>

namespace
{
const QUrl baseUrl(
    QStringLiteral(
        "https://blueheron786.github.io/"
        "potd-nature-wallpapers-plasma/"
    )
);

const QUrl manifestUrl =
    baseUrl.resolved(QUrl(QStringLiteral("wallpapers.json")));

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

    // We need the count before we can determine which
    // wallpaper belongs to this date.
    KIO::StoredTransferJob *job =
        KIO::storedGet(
            manifestUrl,
            KIO::NoReload,
            KIO::HideProgressInfo
        );

    job->setProperty(
        "requestedDate",
        date.toString(Qt::ISODate)
    );

    connect(
        job,
        &KIO::StoredTransferJob::finished,
        this,
        &NatureProvider::manifestRequestFinished
    );
}

void NatureProvider::manifestRequestFinished(KJob *job)
{
    auto *transferJob =
        static_cast<KIO::StoredTransferJob *>(job);

    if (transferJob->error()) {
        Q_EMIT error(this);
        return;
    }

    const QJsonDocument document =
        QJsonDocument::fromJson(transferJob->data());

    if (!document.isObject()) {
        Q_EMIT error(this);
        return;
    }

    const QJsonObject object = document.object();

    const int count =
        object.value(QStringLiteral("count")).toInt();

    if (count <= 0) {
        Q_EMIT error(this);
        return;
    }

    const QDate date =
        QDate::fromString(
            transferJob->property("requestedDate").toString(),
            Qt::ISODate
        );

    if (!date.isValid()) {
        Q_EMIT error(this);
        return;
    }

    // 2026-01-01 is wallpaper 001.
    const QDate epoch(2026, 1, 1);

    const qint64 days =
        epoch.daysTo(date);

    const int number =
        static_cast<int>(
            ((days % count) + count) % count
        ) + 1;

    const QString filename =
        wallpaperFileName(number);

    m_remoteUrl =
        baseUrl.resolved(
            QUrl(
                QStringLiteral("wallpapers/") +
                filename
            )
        );

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
        Q_EMIT error(this);
        return;
    }

    const QImage image =
        QImage::fromData(transferJob->data());

    if (image.isNull()) {
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
