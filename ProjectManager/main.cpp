#include <QCoreApplication>
#include <QFile>
#include <QFuture>
#include <QFutureWatcher>
#include <QMap>
#include <QTemporaryFile>
#include <QTextStream>
#include <QVector>
#include <QtDebug>
#include <QtConcurrent/QtConcurrentMap>

class FileTask
{
public:
    virtual void run() = 0;
};

class FileTasks
{
public:
    inline static void runFileTask(FileTask *task)
    {
        return task->run();
    }
    inline void runMultiThreaded()
    {
        QFuture<void> res = QtConcurrent::map(m_tasks, runFileTask);
        QEventLoop loop;
        QFutureWatcher<void> watcher;
        watcher.setFuture(res);
        QObject::connect(&watcher, &QFutureWatcher<void>::finished, &loop, &QEventLoop::quit);
        QObject::connect(&watcher, &QFutureWatcher<void>::progressValueChanged, [](const int value) {
            qDebug() << "Done" << value;
        });
        loop.exec(QEventLoop::ExcludeUserInputEvents);
    }
    QVector<FileTask *> m_tasks;
};

class FileTaskModify : public FileTask
{
public:
    explicit FileTaskModify(
            const QString &filePath,
            const std::optional<QMap<int, QString>> &lineReplacements = std::nullopt,
            const std::optional<QString> &newFilePath = std::nullopt) :
        m_filePath(filePath),
        m_newFilePath(newFilePath),
        m_lineReplacements(lineReplacements)
    {
    }
    inline void run() override
    {
        m_isFinishedSuccessfully = tryChangePath() && tryReplaceLines();
    }

    /// if newFilePath set tries to rename a file
    /// working m_filePath will be changed on success
    inline bool tryChangePath()
    {
        return (!m_newFilePath.has_value() || trySetNewFilePath(*m_newFilePath));
    }
    /// if newFilePath set tries to rename a file to orig
    /// working m_filePath will be set to m_filePathOrig
    inline bool revertChangePath()
    {
        return (!m_newFilePath.has_value() || trySetNewFilePath(m_filePathOrig));
    }
    inline bool tryReplaceLines()
    {
        return (!m_lineReplacements.has_value() || tryReplaceLines(*m_lineReplacements));
    }
    inline bool revertReplaceLines()
    {
        return (!m_lineReplacements.has_value() || tryReplaceLines(m_linesReplaced));
    }
private:
    inline bool trySetNewFilePath(const QString &path)
    {
        const bool isOk = QFile::rename(m_filePath, path);
        if (isOk)
            m_filePath = path;
        return isOk;
    }
    /// warning: mem intense
    /// opens file, reads all in buffer (changes lines, and remember
    /// old lines into m_linesReplaced). then writes it changed
    inline bool tryReplaceLines(const QMap<int, QString> &lineReplacements)
    {
        if (lineReplacements.isEmpty())
            return true;
        QFile file(m_filePath);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
            return false;
        QStringList lines;
        int nChanges = 0;
        {
            int lineInd = 0;
            QTextStream in(&file);
            while (!in.atEnd()) {
                const QString line = in.readLine();
                if (lineReplacements.contains(lineInd)) {
                    ++nChanges;
                    lines.append(lineReplacements.value(lineInd));
                    m_linesReplaced[lineInd] = line;
                } else {
                    lines.append(line);
                }

                ++lineInd;
            }
        }
        file.close();
        if (nChanges == 0)
            return true;

        if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text))
            return false;
        {
            QTextStream out(&file);
            while (!lines.isEmpty())
                out << lines.takeFirst() << "\n";
        }
        file.flush();
        file.close();
        return true;
    }
    const QString m_filePathOrig;
    QString m_filePath;
    std::optional<QString> m_newFilePath = std::nullopt;
    QMap<int, QString> m_linesReplaced;
    std::optional<QMap<int, QString>> m_lineReplacements = std::nullopt;
    bool m_isFinishedSuccessfully = false;
};

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
//    FileTasks ft;

//    for (int i = 0; i < 40; ++i) {
//        auto t = new FileTaskModify(
//                    QString("/home/alex/Desktop/Test/Test1/file%1.cpp").arg(QString::number(i).rightJustified(3, '0')),
//                    std::nullopt,
//                    QString("/home/alex/Desktop/Test/Test2/file%1.cpp").arg(QString::number(i).rightJustified(3, '0')));
//        ft.m_tasks << t;
//    }

//    ft.runMultiThreaded();

    return a.exec();
}
