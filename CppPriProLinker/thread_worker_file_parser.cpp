#include "thread_worker_file_parser.h"
#include <QtDebug>

ThreadWorkerFileParser::ThreadWorkerFileParser(
        Linker *linker,
        const QHash<QString, FileFactory *> &extToFileFactoryHash,
        const QStringList &filePathes,
        QObject *parent) :
    ThreadWorkerInterruptable(parent),
    m_linker(linker),
    m_extToFileFactoryHash(extToFileFactoryHash),
    m_pathes(filePathes),
    m_count(filePathes.size())
{
    Q_ASSERT(linker);
    Q_ASSERT(m_count);
    setFailPolicy(FailPolicy::IfAllStepsFailed);
    setStepsTotal(m_count);
}

ThreadWorkerFileParser::~ThreadWorkerFileParser()
{
    // do not delete anything
    // files are just created
    // linker and factories are global
    // within this context
}

void ThreadWorkerFileParser::start()
{
    m_files = QVector<File *>(m_count, nullptr);
    m_refs = QVector<QHash<RefFile, QString>>(m_count);
    m_linker->addFilePathes(m_pathes);
}

QString ThreadWorkerFileParser::step(int i)
{
    const QString filePath = m_pathes.at(i);
    const QFileInfo info(filePath);
    const QString ext = info.suffix();
    FileFactory *factory = m_extToFileFactoryHash.value(ext, nullptr);
    if (not factory)
        return QString("No factory found for %1!").arg(filePath);
    QString err;
    File *file = factory->parse(filePath, err);
    if (not file or not err.isEmpty())
        return err;
    m_files[i] = file;
    for (const RefFile &ref : file->refs()){
        QHash<RefFile, QString> &hash = m_refs[i];
        const QString refPath = m_linker->findFilePathForRef(filePath, ref);
        hash.insert(ref, refPath);
    }
    qDebug() << "ok" << filePath;
    return QString();
}

void ThreadWorkerFileParser::finish()
{

}

QStringList ThreadWorkerFileParser::pathes() const
{
    return m_pathes;
}

QVector<File *> ThreadWorkerFileParser::files() const
{
    return m_files;
}

QVector<QHash<RefFile, QString> > ThreadWorkerFileParser::refs() const
{
    return m_refs;
}
