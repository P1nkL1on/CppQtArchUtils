#include "file.h"

void File::addRawRefs(const QVector<RefFile> &rawRefs)
{
    for (const RefFile &r : rawRefs)
        if (not m_refs.contains(r))
            m_refs.insert(r, nullptr);
}
