#ifndef THREADWORKERFILEPARSER_H
#define THREADWORKERFILEPARSER_H

#include "thread_worker_interruptable.h"
#include "file_factory.h"
#include "linker.h"

class ThreadWorkerFileParser : public ThreadWorkerInterruptable
{
public:
    ThreadWorkerFileParser(
            Linker *linker,
            const QHash<QString, FileFactory *> &extToFileFactoryHash,
            const QStringList &filePathes,
            QObject *parent = nullptr);
    ~ThreadWorkerFileParser() override;
    QVector<File *> parsingFiles() const;

    QVector<QHash<RefFile, QString> > refs() const;
    QVector<File *> files() const;
    QStringList pathes() const;

protected:
    void start() override;
    QString step(int i) override;
    void finish() override;

    Linker *m_linker = nullptr;
    QHash<QString, FileFactory *> m_extToFileFactoryHash;
    QStringList m_pathes;
    QVector<File *> m_files;
    QVector<QHash<RefFile, QString>> m_refs;
    int m_count = -1;
};

#endif // THREADWORKERFILEPARSER_H
