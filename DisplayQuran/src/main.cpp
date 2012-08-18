#include <QStringList>
#include <QFile>
#include <QTextStream>
#include <QTextCodec>

bool displayAyatFromFile(const QString &file, unsigned int sourat, unsigned int ayatFrom,
                         unsigned int ayatTo = 0)
{
    QFile f(file);
    if (!f.open(QIODevice::ReadOnly))
        return false;
    QTextStream ts(&f);
    unsigned int curSourat = 0;
	QString result = "";
    while (!ts.atEnd())
    {
        QString line = ts.readLine();
		QStringList sections = line.split("<><><>");
		if (sections.count() == 3)
        {
			uint ayatNumber = sections[1].toUInt();
            if (ayatNumber == 1)
            {
                curSourat++;
            }
            if ((sourat == curSourat) && (ayatFrom <= ayatNumber) &&
                    (!ayatTo || (ayatTo >= ayatNumber)))
            {
                result += QString("%1 {%2}").arg(sections[0]).arg(ayatNumber);
                if (!ayatTo || (ayatTo == ayatNumber))
                {
					QString souratName = sections[2];
                    printf("In the name of Allah, the Beneficent, the Merciful: \"%s\", %s.\n",
                           qPrintable(result), qPrintable(souratName));
                    return true;
                }
                result += " ";
            }
        }
    }
    f.close();
    printf("I can not find ayat %d in sourat number %d\n", ayatFrom, sourat);
    return false;
}

int main(int argc, char **argv)
{
	if (argc < 3)
	{
		return -1;
	}
    unsigned int sourat = QString(argv[1]).toUInt();
	QStringList parts = QString(argv[2]).split("-");
    unsigned int ayatFrom = parts[0].toUInt();
	unsigned int ayatTo = 0;
	if (parts.count() == 2)
	{
		ayatTo = parts[1].toUInt();
	}
    //displayAyatFromFile("data/arabic.qur", sourat, ayat);
    return displayAyatFromFile(":/data/m_pickthal.qur", sourat, ayatFrom, ayatTo) - 1;
}
